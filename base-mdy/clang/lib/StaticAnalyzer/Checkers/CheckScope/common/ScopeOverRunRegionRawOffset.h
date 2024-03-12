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
#ifndef _SCOPE_OVER_RUN_REGION_RAW_OFFSET_H_
#define _SCOPE_OVER_RUN_REGION_RAW_OFFSET_H_

#include "CheckScope.h"
#include "ScopeSValCmp.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ParentMapContext.h"

using namespace clang;
using namespace ento;


namespace scope
{
	class OverRunRegionRawOffset {
	private:
		const SubRegion* baseRegion;
		SVal byteOffset;

	

	public:
		OverRunRegionRawOffset()
			: baseRegion(nullptr), byteOffset(UnknownVal()) {}
		OverRunRegionRawOffset(const SubRegion* base, SVal offset)
			: baseRegion(base), byteOffset(offset) {}

		NonLoc getByteOffset() const { return byteOffset.castAs<NonLoc>(); }
		const SubRegion* getRegion() const { return baseRegion; }

		static OverRunRegionRawOffset computeOffset(ProgramStateRef state,
			SValBuilder& svalBuilder,
			SVal location);

		void dump() const;
		void dumpToStream(raw_ostream& os) const;
	};

}





#endif