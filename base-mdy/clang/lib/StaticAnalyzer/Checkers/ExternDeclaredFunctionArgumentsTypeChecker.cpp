

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
std::set<const FunctionDecl*> MatchedFunctionDeclSet;
namespace
{
	class ExternDeclaredFunctionArgumentsTypeChecker :public Checker<check::ASTDecl<FunctionDecl>, check::EndOfTranslationUnit>
	{
	public:

		void checkASTDecl(const FunctionDecl* D,
			AnalysisManager& Mgr,
			BugReporter& BR) const;
		void checkEndOfTranslationUnit(const TranslationUnitDecl* TU,
			AnalysisManager& Mgr,
			BugReporter& BR) const;

		void reportBug(BugReporter& BR, const FunctionDecl* VD, std::string msg) const;
	};
}

void ExternDeclaredFunctionArgumentsTypeChecker::checkASTDecl(const FunctionDecl* D,
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
	if (!D->hasBody())
	{
		if (!D->getDefinition())
		{
			//在当前上下文中找不到对应的定义 则需要在跨翻译单元中查找
			MatchedFunctionDeclSet.insert(D);
		}
	}
}

void ExternDeclaredFunctionArgumentsTypeChecker::checkEndOfTranslationUnit(const TranslationUnitDecl* TU,
	AnalysisManager& Mgr,
	BugReporter& BR) const
{
	cross_tu::CrossTranslationUnitContext* CTUCtx = Mgr.getCrossTranslationUnitContext();
	if (!CTUCtx)
	{
		return;
	}

	AnalyzerOptions& Opts = Mgr.options;
	std::set<const FunctionDecl*>::iterator iter;
	for (iter = MatchedFunctionDeclSet.begin(); iter != MatchedFunctionDeclSet.end(); iter++)
	{
		const FunctionDecl* FD = *iter;
		if (!FD)
		{
			continue;
		}

		if (FD->getDefinition())
		{
			//在checkEndOfTranslationUnit时上下文中找到了对应声明的定义(类型一致)
			continue;
		}
		else
		{
			//由于函数的LookupName有参数类型，因此当声明和定义参数类型不一致时，getCrossTUDefinition肯定会返回失败
			//此时我们再使用findFuncDefInDeclContext接口在ast索引文件里找有没有函数名一样参数近似的函数，有就认为是这个错误
			const llvm::Optional<std::string> LookupName = CTUCtx->getLookupName(FD);
			if (!LookupName)
			{
				continue;
			}

			llvm::Expected<const FunctionDecl*> CTUDeclOrNull = CTUCtx->getCrossTUDefinition(FD, Opts.CTUDir, Opts.CTUIndexName, Opts.DisplayCTUProgress, true);
			if (!CTUDeclOrNull)
			{
				if (CTUCtx->hasApproachFunctionDecl((*LookupName), Opts.CTUDir, Opts.CTUIndexName, Opts.DisplayCTUProgress))
				{
					std::string msg;
					llvm::raw_string_ostream O(msg);
					O << "函数的声明和定义参数类型不符" ;
					reportBug(BR, FD, msg);
				}
			}
		}
	}
}

void ExternDeclaredFunctionArgumentsTypeChecker::reportBug(BugReporter& BR, const FunctionDecl* VD, std::string msg) const
{
	if (!VD)
	{
		return;
	}
	PathDiagnosticLocation ELoc(VD->getBeginLoc(), VD->getASTContext().getSourceManager());

	BR.EmitBasicReport(VD, this, "类型不一致", categories::SecurityError, msg, ELoc, VD->getSourceRange());

}

void ento::registerExternDeclaredFunctionArgumentsTypeChecker(CheckerManager& mgr) {
	mgr.registerChecker<ExternDeclaredFunctionArgumentsTypeChecker>();
}

bool ento::shouldRegisterExternDeclaredFunctionArgumentsTypeChecker(const CheckerManager& mgr) {
	const LangOptions& LO = mgr.getLangOpts();
	return LO.C99;
}


