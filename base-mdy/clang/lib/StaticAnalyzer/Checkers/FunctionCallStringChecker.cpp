#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/DynamicExtent.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ExprEngine.h"
#include<iostream>

using namespace clang;
using namespace ento;
namespace
{
	class FunctionCallStringChecker :public Checker< check::PreStmt<CallExpr> >
	{
	public:
		void checkPreStmt(const CallExpr* CE, CheckerContext& C) const;


	};
}

void FunctionCallStringChecker::checkPreStmt(const CallExpr *CE, CheckerContext &C) const
{
	int arg_num = CE->getNumArgs();
	if (arg_num <= 0)
	{
		return;
	}

	ProgramStateRef PSR = C.getState();
	SValBuilder &svalBuilder = C.getSValBuilder();

	for (int argIndex = 0; argIndex < arg_num; argIndex++)
	{
		const Expr *arg = CE->getArg(argIndex);

		//去掉typedefine 和 修饰符
		QualType argType = arg->getType();
		argType = argType->isReferenceType() ? argType.getNonReferenceType() : argType;
		argType = QualType(argType->getUnqualifiedDesugaredType(), 0);

		if (arg->isNullPointerConstant(C.getASTContext(), Expr::NPC_ValueDependentIsNull) || argType->isNullPtrType())
		{
			//NULL指针不进行检测
			continue;
		}
		
		if (argType->isPointerType())
		{
			QualType pointeeType = argType->getPointeeType();
			pointeeType = pointeeType->isReferenceType() ? pointeeType.getNonReferenceType() : pointeeType;
			pointeeType = QualType(pointeeType->getUnqualifiedDesugaredType(), 0);

			if (!pointeeType->isAnyCharacterType())
			{
				//仅检测字符指针类型
				continue;
			}

			//构造参数的符号表达式
			SVal argSval = PSR->getSVal(arg, C.getLocationContext());

			//或表达式存储区域
			const MemRegion *argRegion = argSval.getAsRegion();
			if (argRegion == nullptr)
			{
				continue;
			}

			//获取存储区域动态大小
			//DefinedOrUnknownSVal sizeSval = getDynamicExtent(PSR, argRegion->getBaseRegion(), svalBuilder);  //Why? argRegion->getBaseRegion()才能获取正确的size而argRegion却不行
			DefinedOrUnknownSVal sizeSval = getDynamicElementCount(PSR, argRegion->getBaseRegion(), svalBuilder, pointeeType); //区别于上面的方法(获取存储区域字节大小)获取元素的数量
			Optional<nonloc::ConcreteInt> o_concerteInt = sizeSval.getAs<nonloc::ConcreteInt>();
			if (!o_concerteInt)
			{
				//如果不是确定大小的存储区则放弃检测
				continue;
			}

			//获取确定大小
			const llvm::APSInt &arraySize = o_concerteInt->getValue();


			const SubRegion *SuperRegion{ nullptr };
			if (const ElementRegion *const ER = argRegion->getAs<ElementRegion>()) 
			{
				SuperRegion = cast<SubRegion>(ER->getSuperRegion());
			}
			if (!SuperRegion)
			{
				continue;
			}

			bool isExistZero = false;
			//循环遍历存储区元素
			for (int i = 0; i < arraySize; i++)
			{
				const NonLoc Idx = svalBuilder.makeArrayIndex(i);
				const ElementRegion *ER = argRegion->getMemRegionManager().getElementRegion(pointeeType, Idx, SuperRegion, C.getASTContext());

				StoreManager &SM = C.getStoreManager();

				SVal ESval = SM.getBinding(PSR->getStore(), loc::MemRegionVal(ER->getAs<MemRegion>()));
				if (ESval.isUnknownOrUndef())
				{
					break;
				}

				QualType BoolType = svalBuilder.getConditionType();
				SVal ZeroVal = svalBuilder.makeZeroVal(pointeeType);

				//构造比较符号表达式 判断元素的值是否为0
				SVal TmpSVal1 = svalBuilder.evalBinOp(PSR, BO_EQ, ESval, ZeroVal, BoolType);
				Optional<DefinedSVal> VerifyCond = TmpSVal1.getAs<DefinedSVal>();
				if (!VerifyCond)
				{
					continue;
				}

				//判定条件表达式是否为真
				ProgramStateRef ArrayString = PSR->assume(*VerifyCond, true);

				if (ArrayString)
				{
					isExistZero = true;
					break;
				}
			}

			if (!isExistZero)
			{
				PathDiagnosticLocation ELoc(arg->getBeginLoc(), C.getSourceManager());
				C.getBugReporter().EmitBasicReport(C.getCurrentAnalysisDeclContext()->getDecl(), this, "结尾标志", categories::SecurityError, "字符串未以'\\0'结尾", ELoc, arg->getSourceRange());
			}
		}
	}
}

void ento::registerFunctionCallStringChecker(CheckerManager& Mgr)
{
	Mgr.registerChecker<FunctionCallStringChecker>();
}

bool ento::shouldRegisterFunctionCallStringChecker(const CheckerManager& Mgr)
{
	return true;
}
/*
#include<stdio.h>
int main()
{
	char a[3] = { 0 };
	a[1] = 'a';
	a[0] = 'b';
	a[2] = 'c';
	char buff[1024];

	scanf("%s\n", buff);
	//a[3] = 'd';
	//a[4] = 'z';
	char *ppppp = a;
	printf("%s %s %s %c", buff, ppppp, NULL, a[2]);
	return 0;
}
*/
