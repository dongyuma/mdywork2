/*!
 * FileName: ScopeSValCmp.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2021-8-12
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 符号比较相关
 */

#include "ScopeSValCmp.h"
using namespace clang;
using namespace ento;

namespace scope
{
	ConstraintManager::ProgramStatePair isNegativeSval(SVal V, CheckerContext &C)
	{
		ConstraintManager::ProgramStatePair res;
		Optional<DefinedSVal> DV = V.getAs<DefinedSVal>();
		if (!DV)
		{
			return res;
		}
		ProgramStateRef State = C.getState();
		SValBuilder &SVB = C.getSValBuilder();
		DefinedSVal zero = SVB.makeIntVal(0, false);
		return getSValCmp(V, BinaryOperatorKind::BO_LT, zero, C);
	}

	ConstraintManager::ProgramStatePair isConcreteNegativeSval(SVal V, CheckerContext &C)
	{
		ConstraintManager::ProgramStatePair res;
		ProgramStateRef State = C.getState();
		SValBuilder &SVB = C.getSValBuilder();
		if (!V.getAs<nonloc::ConcreteInt>())
		{
			return res;
		}
		DefinedSVal zero = SVB.makeIntVal(0, false);
		return getConcreteIntCmp(V, BinaryOperatorKind::BO_LT, zero, C);
	}


	ConstraintManager::ProgramStatePair getSValCmp(SVal V1, BinaryOperatorKind operator_kind, SVal V2, CheckerContext &C)
	{		
		//参考 C.isGreaterOrEqual()
		ConstraintManager::ProgramStatePair res;
		ProgramStateRef State = C.getState();
		SValBuilder &SVB = C.getSValBuilder();

		if (V1.isUnknownOrUndef() || V2.isUnknownOrUndef())
		{
			return res;
		}

		if (!V1.getAs<NonLoc>())
		{
			V1 = State->getStateManager().getStoreManager().getBinding(State->getStore(), V1.castAs<Loc>());
			if (V1.isUnknownOrUndef() || !V1.getAs<NonLoc>())
			{
				return res;
			}
		}

		if (!V2.getAs<NonLoc>())
		{
			V2 = State->getStateManager().getStoreManager().getBinding(State->getStore(), V2.castAs<Loc>());
			if (V2.isUnknownOrUndef() || !V2.getAs<NonLoc>())
			{
				return res;
			}
		}

		SVal condSval = SVB.evalBinOp(State, operator_kind, V1, V2, SVB.getConditionType());
		Optional<DefinedSVal> DV = condSval.getAs<DefinedSVal>();
		if (!DV || condSval.isUndef())
		{
			return res;
		}
		res = C.getConstraintManager().assumeDual(State, *DV);
		return res;
	}

	clang::ento::ConstraintManager::ProgramStatePair getConcreteIntCmp(SVal V1, BinaryOperatorKind operator_kind, SVal V2, CheckerContext &C)
	{
		ConstraintManager::ProgramStatePair res;
		ProgramStateRef State = C.getState();
		SValBuilder &SVB = C.getSValBuilder();
		if (!V1.getAs<nonloc::ConcreteInt>() || !V2.getAs<nonloc::ConcreteInt>())
		{
			return res;
		}

		SVal condSval = SVB.evalBinOp(State, operator_kind, V1, V2, SVB.getConditionType());
		Optional<DefinedSVal> DV = condSval.getAs<DefinedSVal>();
		if (!DV || condSval.isUndef())
		{
			return res;
		}
		res = C.getConstraintManager().assumeDual(State, *DV);
		return res;
	}

}