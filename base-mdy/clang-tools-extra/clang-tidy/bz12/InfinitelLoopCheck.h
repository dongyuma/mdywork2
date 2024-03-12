//===--- InfinitelLoopCheck.h - clang-tidy ----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_INFINITELLOOPCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_INFINITELLOOPCHECK_H

#include "../ClangTidyCheck.h"
#include<unordered_map>
#include<set>
namespace clang {
namespace tidy {
namespace bz12 {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/bz12-infinitel-loop.html
class InfinitelLoopCheck : public ClangTidyCheck {
public:
  InfinitelLoopCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void checkFunction(const clang::Expr *Cond,const clang::Stmt *LoopStmt,const clang::FunctionDecl *Func,const clang::ast_matchers::MatchFinder::MatchResult &);
  void onEndOfTranslationUnit() override;
private:
  std::unordered_map<const BreakStmt*,unsigned> LocationBeginMap;
  std::unordered_map<const BreakStmt*,unsigned> LocationEndMap;
  std::unordered_map<const Stmt *,std::set<const BreakStmt *>> BreakStmtMap;
  std::set<const Stmt *> ResultSet;
};

} // namespace bz12
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_INFINITELLOOPCHECK_H
