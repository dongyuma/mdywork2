//===--- EmptyconditionCheck.cpp - clang-tidy -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "EmptyConditionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace scope {

void EmptyconditionCheck::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  Finder->addMatcher(ifStmt(unless(isExpansionInSystemHeader())).bind("ifStmt"), this);
}

static bool ShouldDiagnoseEmptyStmtBody(const SourceManager& SourceMgr,
	SourceLocation StmtLoc,
	const NullStmt* Body) {
	// Do not warn if the body is a macro that expands to nothing, e.g:
	//
	// #define CALL(x)
	// if (condition)
	//   CALL(0);
	if (SourceMgr.isMacroArgExpansion(StmtLoc))
		return false;
	if (SourceMgr.isMacroArgExpansion(Body->getSemiLoc()))
		return false;
	if (Body->hasLeadingEmptyMacro())
		return false;

	// Get line numbers of statement and body.
	bool StmtLineInvalid;
	unsigned StmtLine = SourceMgr.getPresumedLineNumber(StmtLoc,
		&StmtLineInvalid);
	if (StmtLineInvalid)
		return false;

	bool BodyLineInvalid;
	unsigned BodyLine = SourceMgr.getSpellingLineNumber(Body->getSemiLoc(),
		&BodyLineInvalid);
	if (BodyLineInvalid)
		return false;

	// Warn if null statement and body are on the same line.
	if (StmtLine != BodyLine)
		return false;

	return true;
}

static bool isEmptyStmt(const Stmt *stmt,const Expr * EXPR, SourceManager* SourceMgr)
{
	bool res = false;
	//stmt->dump();
	if (!stmt)
	{
		return res;
	}

	if (stmt->getStmtClass() == Stmt::NullStmtClass)
	{
		
		if(EXPR)
		{
			SourceLocation StmtLoc = EXPR->getEndLoc();

			const NullStmt* NBody = dyn_cast<NullStmt>(stmt);
			if(!NBody)
				return res;
			if (ShouldDiagnoseEmptyStmtBody(*SourceMgr, StmtLoc, NBody))
				res = true;
			
		}
		else
		{
			res = false;
		}

	}

	return res;
}


void EmptyconditionCheck::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  if(const auto *ifStmt = Result.Nodes.getNodeAs<IfStmt>("ifStmt"))
  {
	auto SourceMgr = Result.SourceManager;
	const Expr* EXPR = ifStmt->getCond();
	const Stmt *Then = ifStmt->getThen();
	if(isEmptyStmt(Then, EXPR, SourceMgr))
	{
		diag(ifStmt->getBeginLoc(), "if分支为空");

	}

  }
}
} // namespace scope
} // namespace tidy
} // namespace clang
