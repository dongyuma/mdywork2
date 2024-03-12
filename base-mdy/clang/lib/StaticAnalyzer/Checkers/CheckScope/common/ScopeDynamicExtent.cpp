/*!
 * FileName: ScopeDynamicExtent.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2021-8-11
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 域大小计算相关
 */

#include "ScopeDynamicExtent.h"

#include "clang/StaticAnalyzer/Core/PathSensitive/DynamicExtent.h"

using namespace clang;
using namespace ento;

namespace scope
{
	SVal getDynamicElementCountWithOffset(ProgramStateRef State, SVal BufV, QualType ElementTy)
	{
		SValBuilder &SVB = State->getStateManager().getSValBuilder();
		Optional<DefinedOrUnknownSVal> Size = getDynamicExtentWithOffset(State, BufV).getAs<DefinedOrUnknownSVal>();
		if (!Size || !Size->getAs<NonLoc>())
		{
			return UnknownVal();
		}

		/*if (Size->isUnknown())
		{
			std::cerr << "===============================\n";
		}
		

		Size->getAs<NonLoc>()->dump();
		return UnknownVal();
		Size->dump();*/

		SVal ElementSize = getElementExtent(ElementTy, SVB);

		SVal ElementCount = SVB.evalBinOp(State, BO_Div, *Size, ElementSize, SVB.getArrayIndexType());

		return ElementCount;
	}
}