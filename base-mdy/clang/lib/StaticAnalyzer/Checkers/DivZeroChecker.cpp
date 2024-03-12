//== DivZeroChecker.cpp - Division by zero checker --------------*- C++ -*--==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This defines DivZeroChecker, a builtin check in ExprEngine that performs
// checks for division by zeros.
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
class DivZeroChecker : public Checker< check::PreStmt<BinaryOperator> > {
  mutable std::unique_ptr<BuiltinBug> BT;
  void reportBug(const char *Msg, ProgramStateRef StateZero, CheckerContext &C,
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

void DivZeroChecker::reportBug(
    const char *Msg, ProgramStateRef StateZero, CheckerContext &C,
    std::unique_ptr<BugReporterVisitor> Visitor) const {
  if (ExplodedNode *N = C.generateNonFatalErrorNode(StateZero)) {
    if (!BT)
      BT.reset(new BuiltinBug(this, "除零"));

    auto R = std::make_unique<PathSensitiveBugReport>(*BT, Msg, N);
    R->addVisitor(std::move(Visitor));
    bugreporter::trackExpressionValue(N, getDenomExpr(N), *R);
    C.emitReport(std::move(R));
  }
}

void DivZeroChecker::checkPreStmt(const BinaryOperator *B,
                                  CheckerContext &C) const {
  BinaryOperator::Opcode Op = B->getOpcode();
  if (Op != BO_Div &&
      Op != BO_Rem &&
      Op != BO_DivAssign &&
      Op != BO_RemAssign)
    return;

  if (!B->getRHS()->getType()->isScalarType())
    return;
  ConstraintManager &CM = C.getConstraintManager();
  ProgramStateRef stateNotZero, stateZero;

  if(!(B->getRHS()->getType()->isFloatingType()))
  {
    SVal Denom = C.getSVal(B->getRHS());
    Optional<DefinedSVal> FDV = Denom.getAs<DefinedSVal>();

    if (!FDV) {
      reportBug("除数是未定义的，可能导致除零", stateZero, C);
      return;
    }
    std::tie(stateNotZero, stateZero) = CM.assumeDual(C.getState(), *FDV);

    if (stateZero) {
      // assert(stateZero);
      reportBug("可能是除零", stateZero, C);
      return;
    }

    bool TaintedD = isTainted(C.getState(), *FDV);
    if ((stateNotZero && stateZero && TaintedD)) {
      reportBug("被污点值除，可能为零", stateZero, C,
                std::make_unique<taint::TaintBugVisitor>(*FDV));
      return;
    }

    C.addTransition(stateNotZero);
  } else {
    SVal Denom = C.getSVal(B->getRHS());
    Optional<DefinedSVal> DV = Denom.getAs<DefinedSVal>();

    // Divide-by-undefined handled in the generic checking for uses of
    // undefined values.
    if (!DV) {
      reportBug("除数是未定义的，可能导致除零", stateZero, C);
      return;
    }

    // Check for divide by zero.
    std::tie(stateNotZero, stateZero) = CM.assumeDual(C.getState(), *DV);

    if (stateZero) {
      // assert(stateZero);
      reportBug("可能是除零", stateZero, C);
      return;
    }

    bool TaintedD = isTainted(C.getState(), *DV);
    if ((stateNotZero && stateZero && TaintedD)) {
      reportBug("被污点值除，可能为零", stateZero, C,
                std::make_unique<taint::TaintBugVisitor>(*DV));
      return;
    }

    // If we get here, then the denom should not be zero. We abandon the implicit
    // zero denom case for now.
    C.addTransition(stateNotZero);
  }
}

void ento::registerDivZeroChecker(CheckerManager &mgr) {
  mgr.registerChecker<DivZeroChecker>();
}

bool ento::shouldRegisterDivZeroChecker(const CheckerManager &mgr) {
  return true;
}
