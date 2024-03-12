//===--- InterruptFunctionStorageCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "InterruptFunctionStorageCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bugprone {

void InterruptFunctionStorageCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
	  functionDecl(isInterruptStorageClass()).bind("functionDecl"), this);
}

void InterruptFunctionStorageCheck::check(const MatchFinder::MatchResult &Result) {
   const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("functionDecl");
	if (!FD)
	{
		return;
	}
	diag(FD->getLocation(),"interrupt关键字！！！");
}

} // namespace bugprone
} // namespace tidy
} // namespace clang
