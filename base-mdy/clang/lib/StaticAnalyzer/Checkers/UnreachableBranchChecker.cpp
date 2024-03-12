//==- UnreachableBranchChecker.cpp - Generalized dead code checker -*- C++ -*-==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// This file implements a generalized unreachable code checker using a
// path-sensitive analysis. 
// 通过检查每个条件分支(非代码层面的if else而是CFG中的分支)的可达与否，记录下函数中
// 每个分支的可达性，在一个函数分析结束后检查该函数内部所有分支条件是否只有不可达的情况
// 如果某个分支从未被标记为可达则，该分支一定为不可达分支
//===----------------------------------------------------------------------===//

#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include "llvm/ADT/FoldingSet.h"
#include "clang/AST/ParentMap.h"
#include "clang/AST/StmtObjC.h"
using namespace clang;
using namespace ento;

namespace {
class UnreachableBranchChecker : public Checker< check::BranchCondition, check::EndAnalysis, check::EndOfTranslationUnit > 
{
public:
    void checkBranchCondition(const Stmt *Condition, CheckerContext &C) const;
	void checkEndAnalysis(ExplodedGraph &G, BugReporter &B, ExprEngine &Eng) const;
    void checkEndOfTranslationUnit(const TranslationUnitDecl *TU, AnalysisManager &Mgr, BugReporter &BR) const;

    bool enableMainFlowUnreachableBranchCheck = true;

private:
    mutable std::map<const Stmt *, bool> FunCondReachableMap;
    mutable std::map<const Stmt *, bool> TuCondReachableMap;

	void setCondReachableFlag(const Stmt *CS, bool reachable, CheckerContext &C) const;
};
}

void UnreachableBranchChecker::checkBranchCondition(const Stmt *Condition,
                                           CheckerContext &C) const {
    ConstraintManager &CM = C.getConstraintManager();
    ProgramStateRef stateReachable, stateUnreachable;

    if (!Condition)
    {
        return;
    }

    SVal Val = C.getSVal(Condition);
    Optional<DefinedSVal> FDV = Val.getAs<DefinedSVal>();
    if (!FDV)
    {
        return;
    }
    std::tie(stateReachable, stateUnreachable) = CM.assumeDual(C.getState(), *FDV);
    if (!stateReachable && stateUnreachable)
    {
        //在当前状态下一定不可达
		setCondReachableFlag(Condition, false, C);
        return;
    }
    //这条路径存在可达的情况
	setCondReachableFlag(Condition, true, C);
}


void UnreachableBranchChecker::checkEndAnalysis(ExplodedGraph &G, BugReporter &B, ExprEngine &Eng) const
{
    std::map<const Stmt *, bool>::iterator  iter;
	for (iter = FunCondReachableMap.begin(); iter != FunCondReachableMap.end(); iter++)
    {
        const Stmt *Condition = iter->first;
        //该分支仅存在不可达的情况
		if (Condition && !iter->second)
        {
			const ExplodedNode *GraphRoot = *G.roots_begin();
			const LocationContext *LC = GraphRoot->getLocation().getLocationContext();
			const Decl *D = LC->getDecl();
            PathDiagnosticLocation ELoc(Condition->getBeginLoc(), B.getSourceManager());
			B.EmitBasicReport(D, this, "不可达分支", categories::LogicError, "以main起始的流程中分支条件永不成立，存在不可达分支", ELoc, Condition->getSourceRange());
        }
    }
    FunCondReachableMap.clear();
}

void UnreachableBranchChecker::setCondReachableFlag(const Stmt *CS, bool reachable, CheckerContext &C) const
{
    //设置整个TU记录的条件分支状态
    std::map<const Stmt *,bool>::iterator  iter = TuCondReachableMap.find(CS);
	if (iter == TuCondReachableMap.end())
    {
		auto ins_res = TuCondReachableMap.insert(std::pair<const Stmt *, bool>(CS, reachable));
        if (ins_res.second)
        {
            iter = ins_res.first;
        }
    }
	if (iter != TuCondReachableMap.end())
    {
		if (reachable)
        {
            //仅标记可达，当某个表达式没有可达的情况则一定不可达
			iter->second = reachable;
        }
    }

	if (enableMainFlowUnreachableBranchCheck && C.getStateManager().isMainAnalysis())
    {
		//设置单个函数分支中条件分支状态
		iter = FunCondReachableMap.find(CS);
		if (iter == FunCondReachableMap.end())
		{
			auto ins_res = FunCondReachableMap.insert(std::pair<const Stmt *, bool>(CS, reachable));
			if (ins_res.second)
			{
				iter = ins_res.first;
			}
		}
		if (iter != FunCondReachableMap.end())
		{
			if (reachable)
			{
				//仅标记可达，当某个表达式没有可达的情况则一定不可达
				iter->second = reachable;
			}
		}
    }
}

void UnreachableBranchChecker::checkEndOfTranslationUnit(const TranslationUnitDecl *TU, AnalysisManager &Mgr, BugReporter &BR) const
{
	std::map<const Stmt *, bool>::iterator  iter;
	for (iter = TuCondReachableMap.begin(); iter != TuCondReachableMap.end(); iter++)
	{
		const Stmt *Condition = iter->first;
		//该分支仅存在不可达的情况
		if (Condition && !iter->second)
		{
			PathDiagnosticLocation ELoc(Condition->getBeginLoc(), Mgr.getSourceManager());
			BR.EmitBasicReport(TU->getTranslationUnitDecl(), this, "不可达分支", categories::LogicError, "分支条件永不成立，存在不可达分支", ELoc, Condition->getSourceRange());
		}
	}
    TuCondReachableMap.clear();
}



void ento::registerUnreachableBranchChecker(CheckerManager &mgr) {
	UnreachableBranchChecker *checker = mgr.registerChecker<UnreachableBranchChecker>();
    checker->enableMainFlowUnreachableBranchCheck = mgr.getAnalyzerOptions().getCheckerBooleanOption(mgr.getCurrentCheckerName(), "enableMainFlowUnreachableBranchCheck");
}

bool ento::shouldRegisterUnreachableBranchChecker(const CheckerManager &mgr) {
  return true;
}