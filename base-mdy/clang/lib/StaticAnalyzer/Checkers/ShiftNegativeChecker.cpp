//== ShiftNegativeChecker.cpp - The number of digits moved in a move operation is negative checker --------------*- C++ -*--==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This defines ShiftNegativeChecker, a builtin check in ExprEngine that performs
// checks for the number of digits moved in a move operation is negative.
//
//===----------------------------------------------------------------------===//

#include <iostream>
#include "Taint.h"
#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"

using namespace clang;
using namespace ento;
using namespace taint;

namespace {
class ShiftNegativeChecker : public Checker< check::PreStmt<BinaryOperator> > {
    mutable std::unique_ptr<BuiltinBug> BT;
    void reportBug(const char *Msg, ProgramStateRef StateNegative, CheckerContext &C,
                 std::unique_ptr<BugReporterVisitor> Visitor = nullptr) const;

public:
    void checkPreStmt(const BinaryOperator *B, CheckerContext &C) const;
};
} // end anonymous namespace


static const Expr *getDenomExpr(const ExplodedNode *N) {
    const Stmt *S = N->getLocationAs<PreStmt>()->getStmt();
    if (const auto *BE = dyn_cast<BinaryOperator>(S))
        return BE->getRHS();
    return nullptr;
}

void ShiftNegativeChecker::reportBug(
    const char *Msg, ProgramStateRef StateNegative, CheckerContext &C,
    std::unique_ptr<BugReporterVisitor> Visitor) const {
    if (ExplodedNode *N = C.generateErrorNode(StateNegative)) {
        if (!BT)
            BT.reset(new BuiltinBug(this, "移动的位数是负数"));

    auto R = std::make_unique<PathSensitiveBugReport>(*BT, Msg, N);
    R->addVisitor(std::move(Visitor));
    bugreporter::trackExpressionValue(N, getDenomExpr(N), *R);
    C.emitReport(std::move(R));
  }
}

void ShiftNegativeChecker::checkPreStmt(const BinaryOperator *B,
                                  CheckerContext &C) const {                                 
    BinaryOperator::Opcode Op = B->getOpcode();
    if (Op != BO_Shl &&
        Op != BO_Shr &&
        Op != BO_ShlAssign &&
        Op != BO_ShrAssign)
        return;
    
    if (!B->getRHS()->getType()->isScalarType())
        return;
    ConstraintManager &CM = C.getConstraintManager();
    ProgramStateRef stateNotNegative, stateNegative;
    if(!(B->getRHS()->getType()->isFloatingType()))
    {
        SValBuilder &svalBuilder = C.getSValBuilder();
        SVal RHS = C.getSVal(B->getRHS());

        SVal lowerEqual = svalBuilder.evalBinOp(C.getState(), BO_LT, RHS,svalBuilder.makeZeroVal(C.getASTContext().IntTy),
                                            svalBuilder.getConditionType());

        Optional<DefinedSVal> FDV = lowerEqual.getAs<DefinedSVal>();
        if (!FDV)
        return;

        std::tie(stateNegative, stateNotNegative) = CM.assumeDual(C.getState(), *FDV);

        bool TaintedD = isTainted(C.getState(), *FDV);
        if (stateNegative && TaintedD) {
            reportBug("移动的位数是污点值，可能是负数", stateNegative, C,
                std::make_unique<taint::TaintBugVisitor>(*FDV));
            return;
        }
        if (!stateNotNegative && stateNegative) {
            // assert(stateZero);
            reportBug("移动的位数是负数", stateNegative, C);
            return;
        }

        C.addTransition(stateNotNegative);
    } else {
        
    }
}

void ento::registerShiftNegativeChecker(CheckerManager &mgr) {
  mgr.registerChecker<ShiftNegativeChecker>();
}

bool ento::shouldRegisterShiftNegativeChecker(const CheckerManager &mgr) {
  return true;
}
