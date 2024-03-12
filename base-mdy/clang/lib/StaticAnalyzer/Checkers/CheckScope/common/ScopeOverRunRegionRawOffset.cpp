/*!
 * FileName: ScopeOverRunRegionRawOffset.cpp
 *
 * Author:   Zhouzihao
 * Date:     2021-11-22
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 计算数组类型内存的偏移
 */
#include "ScopeOverRunRegionRawOffset.h"

using namespace clang;
using namespace ento;

namespace scope
{
#ifndef NDEBUG
	LLVM_DUMP_METHOD void OverRunRegionRawOffset::dump() const {
		dumpToStream(llvm::errs());
	}

	void OverRunRegionRawOffset::dumpToStream(raw_ostream& os) const {
		os << "raw_offset_v2{" << getRegion() << ',' << getByteOffset() << '}';
	}
#endif


	// Lazily computes a value to be used by 'computeOffset'.  If 'val'
	// is unknown or undefined, we lazily substitute '0'.  Otherwise,
	// return 'val'.
	static inline SVal getValue(SVal val, SValBuilder& svalBuilder) {
		return val.getAs<UndefinedVal>() ? svalBuilder.makeArrayIndex(0) : val;
	}

	// Scale a base value by a scaling factor, and return the scaled
	// value as an SVal.  Used by 'computeOffset'.
	static inline SVal scaleValue(ProgramStateRef state,
		NonLoc baseVal, CharUnits scaling,
		SValBuilder& sb) {
		return sb.evalBinOpNN(state, BO_Mul, baseVal,
			sb.makeArrayIndex(scaling.getQuantity()),
			sb.getArrayIndexType());
	}


	// Add an SVal to another, treating unknown and undefined values as
// summing to UnknownVal.  Used by 'computeOffset'.
	static SVal addValue(ProgramStateRef state, SVal x, SVal y,
		SValBuilder& svalBuilder) {
		// We treat UnknownVals and UndefinedVals the same here because we
		// only care about computing offsets.
		if (x.isUnknownOrUndef() || y.isUnknownOrUndef())
			return UnknownVal();

		return svalBuilder.evalBinOpNN(state, BO_Add, x.castAs<NonLoc>(),
			y.castAs<NonLoc>(),
			svalBuilder.getArrayIndexType());
	}

	/// Compute a raw byte offset from a base region.  Used for array bounds
/// checking.
	OverRunRegionRawOffset OverRunRegionRawOffset::computeOffset(ProgramStateRef state,
		SValBuilder& svalBuilder,
		SVal location)
	{
		const MemRegion* region = location.getAsRegion();
		SVal offset = UndefinedVal();

		while (region) {
			switch (region->getKind()) {
			default: {
				if (const SubRegion* subReg = dyn_cast<SubRegion>(region)) {
					offset = getValue(offset, svalBuilder);
					if (!offset.isUnknownOrUndef())
					{
						return OverRunRegionRawOffset(subReg, offset);
					}

				}
				return OverRunRegionRawOffset();
			}
			case MemRegion::ElementRegionKind: {
				const ElementRegion* elemReg = cast<ElementRegion>(region);
				SVal index = elemReg->getIndex();
				if (!index.getAs<NonLoc>())
					return OverRunRegionRawOffset();
				QualType elemType = elemReg->getElementType();
				// If the element is an incomplete type, go no further.
				ASTContext& astContext = svalBuilder.getContext();
				if (elemType->isIncompleteType())
					return OverRunRegionRawOffset();

				// Update the offset.

				offset = addValue(state,
					getValue(offset, svalBuilder),
					scaleValue(state,
						index.castAs<NonLoc>(),
						astContext.getTypeSizeInChars(elemType),
						svalBuilder),
					svalBuilder);

				if (offset.isUnknownOrUndef())
					return OverRunRegionRawOffset();

				region = elemReg->getSuperRegion();
				continue;
			}
			}
		}
		return OverRunRegionRawOffset();
	}


}
