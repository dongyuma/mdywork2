//===--- MemsetMultipleRankPointerCheck.h - clang-tidy ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_MEMSETMULTIPLERANKPOINTERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_MEMSETMULTIPLERANKPOINTERCHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace bz12 {

/// 检测memset设置多重指针的情况
///
/// RJZ_1_7_case1

class MemsetMultipleRankPointerCheck : public ClangTidyCheck {
public:
    MemsetMultipleRankPointerCheck(StringRef Name, ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context) {}
    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace bz12
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_MEMSETMULTIPLERANKPOINTERCHECK_H
