/*!
 * FileName: ScopeDynamicExtent.h
 *
 * Author:   ZhangChaoZe
 * Date:     2021-8-11
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 域大小计算相关
 */

#ifndef _SCOPE_DYNAMIC_EXTENT_H_
#define _SCOPE_DYNAMIC_EXTENT_H_

#include "clang/AST/Expr.h"
#include "clang/Basic/LLVM.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/MemRegion.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ProgramState.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/SValBuilder.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/SVals.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/SymbolManager.h"

namespace scope
{
	clang::ento::SVal getDynamicElementCountWithOffset(clang::ento::ProgramStateRef State, clang::ento::SVal BufV, clang::QualType ElementTy);
}

#endif