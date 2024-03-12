//===--- ExternDefinationDeclareTypeCheck.cpp - clang-tidy ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ExternDefinationDeclareTypeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

void ExternDefinationDeclareTypeCheck::registerMatchers(MatchFinder *Finder) {
    Finder->addMatcher(varDecl(unless(isExpansionInSystemHeader())).bind("vardecl"), this);
}

void ExternDefinationDeclareTypeCheck::check(const MatchFinder::MatchResult &Result) {
    if(const auto *MatchedVarDecl = Result.Nodes.getNodeAs<VarDecl>("vardecl")) {
        // MatchedVarDecl->dumpColor();
        if(MatchedVarDecl->hasExternalStorage()) {
            if(MatchedVarDecl->hasDefinition()) {
                if(MatchedVarDecl->getType() != MatchedVarDecl->getDefinition()->getType()) {
                    diag(MatchedVarDecl->getBeginLoc(), "Extern变量的定义和声明的类型不一致");
                }
            }
        }
    }
}

} // namespace bz12
} // namespace tidy
} // namespace clang
