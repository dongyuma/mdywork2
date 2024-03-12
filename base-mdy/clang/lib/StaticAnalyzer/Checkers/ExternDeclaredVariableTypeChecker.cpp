

#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTNodeTraverser.h"
#include "clang/AST/ParentMap.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/CrossTU/CrossTranslationUnit.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ExprEngine.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h"
#include <set>

using namespace clang;
using namespace ento;
std::set<const VarDecl*> MatchedVarDeclSet;
namespace
{
	class ExternDeclaredVariableTypeChecker :public Checker<check::ASTDecl<VarDecl>, check::EndOfTranslationUnit>
	{
	public:

		//void checkBeginFunction(CheckerContext& C) const;
		void checkASTDecl(const VarDecl* D,
			AnalysisManager& Mgr,
			BugReporter& BR) const;
		void checkEndOfTranslationUnit(const TranslationUnitDecl* TU,
			AnalysisManager& Mgr,
			BugReporter& BR) const;

		void reportBug(BugReporter &BR, const VarDecl *VD, std::string msg) const;
	};
}

void ExternDeclaredVariableTypeChecker::checkASTDecl(const VarDecl* D,
	AnalysisManager& Mgr,
	BugReporter& BR) const
{
	
	if (!D)
	{
		return;
	}
	if (Mgr.getSourceManager().isInSystemHeader(D->getLocation()))
	{
		return;
	}
	if (D->hasGlobalStorage() && D->hasExternalStorage() && !D->hasInit() && D->getType().isTrivialType(Mgr.getASTContext()))
	{
		if (!D->getDefinition())
		{
			//在当前上下文中找不到对应的定义 则需要在跨翻译单元中查找
			MatchedVarDeclSet.insert(D);
		}
	}
}

static bool isEquivalentQualType(const QualType &a, const QualType &b)
{
	bool res = false;
	QualType T1 = a->isReferenceType() ? a.getNonReferenceType() : a;
	QualType T2 = b->isReferenceType() ? b.getNonReferenceType() : b;

	T1 = QualType(T1->getUnqualifiedDesugaredType(), 0).getCanonicalType();
	T2 = QualType(T2->getUnqualifiedDesugaredType(), 0).getCanonicalType();

	//后期优化更多数据类型的比较
	if (T1->isBuiltinType() || T1->isEnumeralType() || T1->isStructureType())
	{
		res = T1 == T2;
	}
	else if (T1->isPointerType() && !T1->isFunctionPointerType())
	{
		if (T2->isPointerType() || T2->isArrayType())
		{
			//如果源也是指针或数组则继续比较子元素类型
			res = isEquivalentQualType(QualType(T1->getPointeeOrArrayElementType(), 0), QualType(T2->getPointeeOrArrayElementType(), 0));
		}
		else
		{
			res = false;
		}
	}
	else
	{
		//其余类型(函数指针 类什么的) 暂时不进行检测
		res = true;
	}
	return res;
}

static bool isEquivalentArrayIndex(const QualType& a, const QualType& b)
{
	bool res = false;
	QualType T1 = a->isReferenceType() ? a.getNonReferenceType() : a;
	QualType T2 = b->isReferenceType() ? b.getNonReferenceType() : b;

	T1 = QualType(T1->getUnqualifiedDesugaredType(), 0).getCanonicalType();
	T2 = QualType(T2->getUnqualifiedDesugaredType(), 0).getCanonicalType();

	//只有两个数组下标都为常量才进行比较
	
	if (T1->isConstantArrayType() && T2->isConstantArrayType())
	{
		if(dyn_cast_or_null<ConstantArrayType>(T1.getTypePtr())->getSize().getZExtValue()==
			dyn_cast_or_null<ConstantArrayType>(T2.getTypePtr())->getSize().getZExtValue())
		{
			//如果源也是指针或数组则继续比较子元素类型
			res = isEquivalentQualType(QualType(T1->getPointeeOrArrayElementType(), 0), QualType(T2->getPointeeOrArrayElementType(), 0));
		}
		else
		{
			res = false;
		}
	}
	else
	{
		res = true;
	}
	return res;
}

void ExternDeclaredVariableTypeChecker::checkEndOfTranslationUnit(const TranslationUnitDecl* TU,
	AnalysisManager& Mgr,
	BugReporter& BR) const
{
	cross_tu::CrossTranslationUnitContext *CTUCtx = Mgr.getCrossTranslationUnitContext();
	if (!CTUCtx)
	{
		return;
	}
	
	AnalyzerOptions& Opts = Mgr.options;
	std::set<const VarDecl*>::iterator iter;
	for (iter = MatchedVarDeclSet.begin(); iter != MatchedVarDeclSet.end(); iter++)
	{
		const VarDecl *VD = *iter;
		if (!VD)
		{
			continue;
		}

		if (VD->getDefinition())
		{
			//在checkEndOfTranslationUnit时上下文中找到了对应声明的定义(类型一致)
			continue;
		}
		else
		{
			//在当前上下文中未找到类型一致的定义 使用getCrossTUDefinition的notImport尝试查找相似类型的定义
			const llvm::Optional<std::string> LookupName = CTUCtx->getLookupName(VD);
			if (!LookupName)
			{
				continue;
			}
			llvm::Expected<const VarDecl *> CTUDeclOrNull = CTUCtx->getCrossTUDefinition(VD, Opts.CTUDir, Opts.CTUIndexName, Opts.DisplayCTUProgress, true);
			if (CTUDeclOrNull)
			{
				if (!isEquivalentQualType(VD->getType(), (*CTUDeclOrNull)->getType()))
				{
					std::string msg;
					llvm::raw_string_ostream O(msg);
					O << "变量的声明和定义不符,原型为'" << (*CTUDeclOrNull)->getType().getAsString() << " " << (*CTUDeclOrNull)->getNameAsString() << "'";
					reportBug(BR, VD, msg);
				}
				else
				{
					//在数组类型一致的时候再做数组范围相等判断，如果类型不一致则不会执行这部分代码
					if (!isEquivalentArrayIndex(VD->getType(), (*CTUDeclOrNull)->getType()))
					{
						std::string msg;
						llvm::raw_string_ostream O(msg);
						O << "数组声明和定义长度不一致，原型为" << (*CTUDeclOrNull)->getType().getAsString() << " " << (*CTUDeclOrNull)->getNameAsString() << "'";
						reportBug(BR, VD, msg);
					}

				}
			}
			//CTU中找不到的暂不报bug 因为如果是外部库或者外部源文件中的定义 则可能会导致大量误报
		    //reportBug(BR, VD);
		}	
	}
}

void ExternDeclaredVariableTypeChecker::reportBug(BugReporter &BR, const VarDecl *VD, std::string msg) const
{
	if (!VD)
	{
		return;
	}
	PathDiagnosticLocation ELoc(VD->getBeginLoc(), VD->getASTContext().getSourceManager());
	
	BR.EmitBasicReport(VD, this, "类型不一致", categories::SecurityError, msg, ELoc, VD->getSourceRange());

}

void ento::registerExternDeclaredVariableTypeChecker(CheckerManager& mgr) {
	mgr.registerChecker<ExternDeclaredVariableTypeChecker>();
}

bool ento::shouldRegisterExternDeclaredVariableTypeChecker(const CheckerManager& mgr) {
	return true;
}


