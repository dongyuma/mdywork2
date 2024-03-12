//===--- IntegerdivlossaccuracyCheck.cpp - clang-tidy ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "IntegerDivLossAccuracyCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/ADT/STLExtras.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace scope {

void IntegerdivlossaccuracyCheck::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
   Finder->addMatcher(binaryOperator(hasOperatorName("/"),
        unless(isExpansionInSystemHeader())).bind("binaryOperator"), this);
}


void IntegerdivlossaccuracyCheck::check(const MatchFinder::MatchResult &Result) {
    const BinaryOperator *BO = Result.Nodes.getNodeAs<BinaryOperator>("binaryOperator");
    if (!BO)
    {
        return;
    }
	if (!BO->getLHS()->getType()->isIntegerType() ||
        !BO->getRHS()->getType()->isIntegerType())
    {
        return;
    }
	ASTContext *AC = Result.Context;

    auto Parents = AC->getParents(*BO);
    while (!Parents.empty())
    {
        const Expr *E= Parents.begin()->get<Expr>();
        if (!E)
        {
            break;
        }

        //E->dump();
		if (!isa<ImplicitCastExpr>(E) && !isa<BinaryOperator>(E) && !isa<UnaryOperator>(E) && !isa<ParenExpr>(E))
        {
			//父表达式不为这几种的任何一种直接返回(关于该表达式的全部运算阶段性地结束)
            break;
        }

        //整数除法的结果隐式转换或参与浮点运算则应使用浮点除法
        if (E->getType()->isFloatingType())
        {
            diag(BO->getOperatorLoc(), "使用整数除法而意外损失了算术运算精度");
            break;
        }
        Parents = AC->getParents(*E);
    }
}

} // namespace scope
} // namespace tidy
} // namespace clang
