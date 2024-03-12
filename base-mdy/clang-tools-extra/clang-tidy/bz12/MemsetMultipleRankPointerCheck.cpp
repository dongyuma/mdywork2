//===--- MemsetMultipleRankPointerCheck.cpp - clang-tidy ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MemsetMultipleRankPointerCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

void MemsetMultipleRankPointerCheck::registerMatchers(MatchFinder *Finder) 
{
    Finder->addMatcher(callExpr(
        callee(functionDecl(
            hasName("memset")
        )),
        unless(isExpansionInSystemHeader())
        ).bind("callExpr"), this);
}

void MemsetMultipleRankPointerCheck::check(const MatchFinder::MatchResult &Result) 
{
    if(const auto *MatchedCallExpr = Result.Nodes.getNodeAs<CallExpr>("callExpr"))  
    {
        if (MatchedCallExpr->getNumArgs() < 1)
            return;
        if (auto CallExprArgs = MatchedCallExpr->getArg(0))
        {
            auto CallExprArgsType =   CallExprArgs->IgnoreParenImpCasts()->getType();
            if (CallExprArgsType->isPointerType())
            {
                if (CallExprArgsType->getPointeeType()->isPointerType())
                {
                    diag(MatchedCallExpr->getArg(0)->getBeginLoc(), "memset函数使用地址的地址，可能存在错误");
                }
            }
        }
    }
}

} // namespace bz12
} // namespace tidy
} // namespace clang
