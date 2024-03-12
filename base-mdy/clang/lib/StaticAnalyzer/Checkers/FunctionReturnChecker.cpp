/*!
 * FileName: FunctionReturnChecker.cpp
 *
 * Author:   CaoZehui
 * Date:     2021-6-1
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 函数返回值检测
 */

#include <iostream>
#include <set>

#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTNodeTraverser.h"
#include "clang/AST/ParentMap.h"
#include "clang/AST/PrettyPrinter.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/Analysis/Analyses/ReachableCode.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include "clang/Sema/Sema.h"


using namespace clang;
using namespace ento;
using namespace std;


enum ControlFlowKind {
	UnknownFallThrough,
	NeverFallThrough,
	MaybeFallThrough,
	AlwaysFallThrough,
	NeverFallThroughOrReturn
};

namespace
{
	class FunctionReturnChecker :public Checker<check::ASTCodeBody>
	{
	public:

		void checkASTCodeBody(const Decl *D, AnalysisManager &Mgr, BugReporter &BR) const;
		
	public:
		
		
	};
}




static ControlFlowKind CheckFallThrough(AnalysisDeclContext &AC) {
	CFG *cfg = AC.getCFG();
	if (!cfg) return UnknownFallThrough;

	// The CFG leaves in dead things, and we don't want the dead code paths to
	// confuse us, so we mark all live things first.
	llvm::BitVector live(cfg->getNumBlockIDs());
	unsigned count = reachable_code::ScanReachableFromBlock(&cfg->getEntry(),
		live);

	bool AddEHEdges = AC.getAddEHEdges();
	if (!AddEHEdges && count != cfg->getNumBlockIDs())
		// When there are things remaining dead, and we didn't add EH edges
		// from CallExprs to the catch clauses, we have to go back and
		// mark them as live.
		for (const auto *B : *cfg) {
			if (!live[B->getBlockID()]) {
				if (B->pred_begin() == B->pred_end()) {
					const Stmt *Term = B->getTerminatorStmt();
					if (Term && isa<CXXTryStmt>(Term))
						// When not adding EH edges from calls, catch clauses
						// can otherwise seem dead.  Avoid noting them as dead.
						count += reachable_code::ScanReachableFromBlock(B, live);
					continue;
				}
			}
		}

	// Now we know what is live, we check the live precessors of the exit block
	// and look for fall through paths, being careful to ignore normal returns,
	// and exceptional paths.
	bool HasLiveReturn = false;
	bool HasFakeEdge = false;
	bool HasPlainEdge = false;
	bool HasAbnormalEdge = false;

	// Ignore default cases that aren't likely to be reachable because all
	// enums in a switch(X) have explicit case statements.
	CFGBlock::FilterOptions FO;
	FO.IgnoreDefaultsWithCoveredEnums = 1;

	for (CFGBlock::filtered_pred_iterator I =
		cfg->getExit().filtered_pred_start_end(FO);
		I.hasMore(); ++I) {
		const CFGBlock &B = **I;
		if (!live[B.getBlockID()])
			continue;

		// Skip blocks which contain an element marked as no-return. They don't
		// represent actually viable edges into the exit block, so mark them as
		// abnormal.
		if (B.hasNoReturnElement()) {
			HasAbnormalEdge = true;
			continue;
		}

		// Destructors can appear after the 'return' in the CFG.  This is
		// normal.  We need to look pass the destructors for the return
		// statement (if it exists).
		CFGBlock::const_reverse_iterator ri = B.rbegin(), re = B.rend();

		for (; ri != re; ++ri)
			if (ri->getAs<CFGStmt>())
				break;

		// No more CFGElements in the block?
		if (ri == re) {
			const Stmt *Term = B.getTerminatorStmt();
			if (Term && isa<CXXTryStmt>(Term)) {
				HasAbnormalEdge = true;
				continue;
			}
			// A labeled empty statement, or the entry block...
			HasPlainEdge = true;
			continue;
		}

		CFGStmt CS = ri->castAs<CFGStmt>();
		const Stmt *S = CS.getStmt();
		if (isa<ReturnStmt>(S) || isa<CoreturnStmt>(S)) {
			HasLiveReturn = true;
			continue;
		}
		if (isa<ObjCAtThrowStmt>(S)) {
			HasFakeEdge = true;
			continue;
		}
		if (isa<CXXThrowExpr>(S)) {
			HasFakeEdge = true;
			continue;
		}
		if (isa<MSAsmStmt>(S)) {
			// TODO: Verify this is correct.
			HasFakeEdge = true;
			HasLiveReturn = true;
			continue;
		}
		if (isa<CXXTryStmt>(S)) {
			HasAbnormalEdge = true;
			continue;
		}
		if (std::find(B.succ_begin(), B.succ_end(), &cfg->getExit())
			== B.succ_end()) {
			HasAbnormalEdge = true;
			continue;
		}

		HasPlainEdge = true;
	}
	if (!HasPlainEdge) {
		if (HasLiveReturn)
			return NeverFallThrough;
		return NeverFallThroughOrReturn;
	}
	if (HasAbnormalEdge || HasFakeEdge || HasLiveReturn)
		return MaybeFallThrough;
	// This says AlwaysFallThrough for calls to functions that are not marked
	// noreturn, that don't return.  If people would like this warning to be more
	// accurate, such functions should be marked as noreturn.
	return AlwaysFallThrough;
}

void FunctionReturnChecker::checkASTCodeBody(const Decl *D, AnalysisManager &Mgr, BugReporter &BR) const
{
	//cout<<"进入ASTCode"<<endl;
	if (!isa<FunctionDecl>(D))
	{
		return;
	}
	const FunctionDecl *FD = dyn_cast_or_null<FunctionDecl>(D);
	if (FD == NULL)
	{
		return;
	}

	bool ReturnsVoid = false;
	bool HasNoReturn = false;


	const auto *CBody = dyn_cast<CoroutineBodyStmt>(FD->getBody());
	if (CBody != NULL)
	{
		ReturnsVoid = CBody->getFallthroughHandler() != nullptr;
	}
	else
	{
		ReturnsVoid = FD->getReturnType()->isVoidType();
	}	
	HasNoReturn = FD->isNoReturn();

	//检查是否存在应该从返回一个值的地方退出了函数
	ControlFlowKind kind = CheckFallThrough(*(Mgr.getAnalysisDeclContext(D)));
	//cout<<"查看是否有返回值"<<endl;
	if (kind == MaybeFallThrough || kind == AlwaysFallThrough)
	{
		if (HasNoReturn)
		{
			
			//永不返回的函数(像exec)在函数的某个地方退出了
			PathDiagnosticLocation ELoc(FD->getBeginLoc(), BR.getSourceManager());
			std::string warn = "函数";
			warn.append(FD->getNameAsString());
			warn.append("声明为'no return' 不应该返回值");
			BR.EmitBasicReport(FD, this, "退出函数", categories::LogicError, warn, ELoc);
		}
		else if (!ReturnsVoid)
		{
			cout<<"没有返回值"<<endl;
			//有返回值的函数在函数的某个位置直接退出了而不是以return 返回值的方式结束
			PathDiagnosticLocation ELoc(FD->getBeginLoc(), BR.getSourceManager());
			std::string warn = "非空函数";
			warn.append(FD->getNameAsString());
			warn.append("在所有的路径中都没有返回值");
			BR.EmitBasicReport(FD, this, "退出函数", categories::LogicError, warn, ELoc);
		}
	}

	
	
}




void ento::registerFunctionReturnChecker(CheckerManager &Mgr)
{
	Mgr.registerChecker<FunctionReturnChecker>();
}

bool ento::shouldRegisterFunctionReturnChecker(const CheckerManager &Mgr)
{
	return true;
}