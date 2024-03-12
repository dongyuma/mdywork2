/*!
 * FileName: ScopeExprCmp.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2021-7-29
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: checkscope通用表达式比较函数
 */

#include "ScopeExprCmp.h"

using namespace clang;
using namespace ento;

namespace scope
{
	ConstraintManager::ProgramStatePair isNegativeExpr(const Expr *E, CheckerContext &C)
	{
		std::pair<ProgramStateRef, ProgramStateRef> res;

		if (E->getType()->isSignedIntegerType())
		{
			//仅对有符号整型进行负值的判断
			res = getExprCmpInt(E, BO_LT, 0, C);
		}

		return res;
	}


	ConstraintManager::ProgramStatePair getExprCmpInt(const Expr *E, BinaryOperatorKind operator_kind, uint64_t integer, CheckerContext &C)
	{
		//参考  C.isGreaterOrEqual();  返回一对状态以判断是否一定成立 或者可能成立

		ConstraintManager::ProgramStatePair res;
		if (!E)
		{
			return res;
		}
		//获取程序状态、符号构造器
		const ProgramStateRef &PSR = C.getState();
		SValBuilder &SVB = C.getSValBuilder();
		ConstraintManager &CM = C.getConstraintManager();

		//创建符号
		SVal ESVal = C.getSVal(E);
		if (ESVal.isUnknownOrUndef())
		{
			return res;
		}
		if (!ESVal.getAs<NonLoc>())
		{
			ESVal = PSR->getStateManager().getStoreManager().getBinding(PSR->getStore(), ESVal.castAs<Loc>());
			if (ESVal.isUnknownOrUndef() || !ESVal.getAs<NonLoc>())
			{
				return res;
			}
		}

		SVal cmpVal = SVB.makeIntVal(integer, false);

		//构造符号表达式
		SVal CondSVal = SVB.evalBinOp(PSR, operator_kind, ESVal, cmpVal, SVB.getConditionType());
		Optional<DefinedSVal> FDV = CondSVal.getAs<DefinedSVal>();
		if (!FDV)
		{
			//未定义的符号值
			return res;
		}

		res = CM.assumeDual(PSR, *FDV);
		return res;
	}

}

