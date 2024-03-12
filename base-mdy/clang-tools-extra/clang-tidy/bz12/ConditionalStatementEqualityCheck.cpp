//===--- ConditionalStatementEqualityCheck.cpp - clang-tidy ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ConditionalStatementEqualityCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

void ConditionalStatementEqualityCheck::registerMatchers(MatchFinder *Finder) {
    Finder->addMatcher(ifStmt(unless(isExpansionInSystemHeader())).bind("ifStmt"), this);
    Finder->addMatcher(whileStmt(unless(isExpansionInSystemHeader())).bind("whileStmt"), this);
    Finder->addMatcher(forStmt(unless(isExpansionInSystemHeader())).bind("forStmt"), this);
    Finder->addMatcher(doStmt(unless(isExpansionInSystemHeader())).bind("doStmt"), this);
}

void ConditionalStatementEqualityCheck::equality(const Stmt *stmt) {
    if(const BinaryOperator *BO = dyn_cast<BinaryOperator>(stmt)) {
        if(BO->isAssignmentOp()){
            diag(BO->getBeginLoc(), "在条件语句中不建议使用符号:%0 .")
                << BO->getOpcodeStr();
        }
    }
}

void ConditionalStatementEqualityCheck::check(const MatchFinder::MatchResult &Result) {
    if(const auto *MatchedIfStmt = Result.Nodes.getNodeAs<IfStmt>("ifStmt")) {
        equality(MatchedIfStmt->getCond()->IgnoreParenImpCasts());
        // MatchedIfStmt->getCond()->IgnoreParenImpCasts()->dump();
    }
    if(const auto *MatchedWhileStmt = Result.Nodes.getNodeAs<WhileStmt>("whileStmt")) {
        equality(MatchedWhileStmt->getCond()->IgnoreParenImpCasts());
        // MatchedWhileStmt->getCond()->IgnoreParenImpCasts()->dump();
    }
    if(const auto *MatchedForStmt = Result.Nodes.getNodeAs<ForStmt>("forStmt")) {
        equality(MatchedForStmt->getCond()->IgnoreParenImpCasts());
        // MatchedForStmt->getCond()->IgnoreParenImpCasts()->dump();
    }
    if(const auto *MatchedDoStmt= Result.Nodes.getNodeAs<DoStmt>("doStmt")) {
        equality(MatchedDoStmt->getCond()->IgnoreParenImpCasts());
        // MatchedDoStmt->getCond()->IgnoreParenImpCasts()->dump();
    }

    // diag(MatchedDecl->getLocation(), "function %0 is insufficiently awesome")
    //     << MatchedDecl;
    // diag(MatchedDecl->getLocation(), "insert 'awesome'", DiagnosticIDs::Note)
    //     << FixItHint::CreateInsertion(MatchedDecl->getLocation(), "awesome_");
}

} // namespace bz12
} // namespace tidy
} // namespace clang
