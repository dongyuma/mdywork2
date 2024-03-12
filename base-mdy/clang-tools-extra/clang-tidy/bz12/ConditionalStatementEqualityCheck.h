//===--- ConditionalStatementEqualityCheck.h - clang-tidy -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_CONDITIONALSTATEMENTEQUALITYCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_CONDITIONALSTATEMENTEQUALITYCHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace bz12 {

/// AS-15：检测条件语句使用=而不是==
/// 目前支持if、while、for循环的条件语句的支持

class ConditionalStatementEqualityCheck : public ClangTidyCheck {
public:
    ConditionalStatementEqualityCheck(StringRef Name, ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context) {}
    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
    void equality(const Stmt *stmt);
};

} // namespace bz12
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_CONDITIONALSTATEMENTEQUALITYCHECK_H
