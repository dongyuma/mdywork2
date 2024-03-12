//===--- DifferentFunctionDeclarationAndDefinitionCheck.h - clang-tidy *- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_DIFFERENTFUNCTIONDECLARATIONANDDEFINITIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_DIFFERENTFUNCTIONDECLARATIONANDDEFINITIONCHECK_H

#include "../ClangTidyCheck.h"
#include <vector>

namespace clang {
namespace tidy {
namespace bz12 {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/bz12-different-function-declaration-and-definition.html
class DifferentFunctionDeclarationAndDefinitionCheck : public ClangTidyCheck {
public:
  DifferentFunctionDeclarationAndDefinitionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void onEndOfTranslationUnit()override;
  bool isLanguageVersionSupported(const LangOptions& LangOpts) const override {
      return LangOpts.C99;
  }

private:
  std::vector<const FunctionDecl*> vDecl;
  std::vector<const FunctionDecl*> vDef;
};

} // namespace bz12
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_DIFFERENTFUNCTIONDECLARATIONANDDEFINITIONCHECK_H
