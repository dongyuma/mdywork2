//===--- BlockConsistencyCheck.h - clang-tidy -------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_BLOCKCONSISTENCYCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_BLOCKCONSISTENCYCHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace bz12 {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/bz12-block-consistency.html
class BlockConsistencyCheck : public ClangTidyCheck {
public:
    BlockConsistencyCheck(StringRef Name, ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context) {}
    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
    void onEndOfTranslationUnit() override;

    void traverseSwitchBody(const Stmt * S);
    void analysisSwitchBody(const Stmt * S, std::vector<const Stmt*> stmt_vec);

    void traverseIfStmt(const IfStmt * IF);
    void analysisIfStmt();
    void analysisBody(const Stmt* S);

    std::map<const Stmt *, std::vector<const Stmt*>> switch_branch_map;
    std::vector<const IfStmt*> if_stmt_vec;
    std::map<const IfStmt*, const IfStmt*> if_stmt_parent_map;
    std::map<std::string, std::string> mutex_level_map;

};

} // namespace bz12
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_BLOCKCONSISTENCYCHECK_H
