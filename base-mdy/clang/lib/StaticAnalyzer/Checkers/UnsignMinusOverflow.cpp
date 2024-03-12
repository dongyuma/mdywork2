//== DivZeroChecker.cpp - Division by zero checker --------------*- C++ -*--==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This defines DivZeroChecker, a builtin check in ExprEngine that performs
// checks for division by zeros.
//
//===----------------------------------------------------------------------===//

#include "clang/AST/StmtVisitor.h"
#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h"

using namespace clang;
using namespace ento;

namespace {
class UnsignedMinusChecker : public Checker< check::PreStmt<BinaryOperator> > {
  mutable std::unique_ptr<BuiltinBug> BT;
  void reportBug(const char *Msg,
                 ProgramStateRef StateZero,
                 CheckerContext &C,
                 const BinaryOperator *B) const ;
public:
  void checkPreStmt(const BinaryOperator *B, CheckerContext &C) const;
};
} // end anonymous namespace

void UnsignedMinusChecker::reportBug(const char *Msg,
                               ProgramStateRef StateZero,
                               CheckerContext &C,
                               const BinaryOperator *B) const {
  if (ExplodedNode *N = C.generateErrorNode(StateZero)) {
    if (!BT)
      BT.reset(new BuiltinBug(this, "Invalid Values", Msg));

    // FIXME: It would be nice to eventually make this diagnostic more clear,
    // e.g., by referencing the original declaration or by saying *why* this
    // reference is outside the range.

    // Generate a report for this bug.
    auto report =
        std::make_unique<PathSensitiveBugReport>(*BT, BT->getDescription(), N);

    report->addRange(SourceRange(B->getBeginLoc(), B->getEndLoc()));
    C.emitReport(std::move(report));
    return;
  }
}

void UnsignedMinusChecker::checkPreStmt(const BinaryOperator *B,
                                  CheckerContext &C) const {

  BinaryOperator::Opcode Op = B->getOpcode();
  if (Op != BO_Sub && Op != BO_SubAssign)
    return;

  if (!B->getRHS()->getType()->isScalarType())
    return;

  if((B->getLHS()->getType()->isUnsignedIntegerType()) && (B->getRHS()->getType()->isUnsignedIntegerType()))
  {
    SVal DenomR = C.getSVal(B->getRHS());
    Optional<DefinedSVal> DVR = DenomR.getAs<DefinedSVal>();

    // Divide-by-undefined handled in the generic checking for uses of
    // undefined values.
    if (!DVR)
      return;

    SVal DenomL = C.getSVal(B->getLHS());
    Optional<DefinedSVal> DVL = DenomL.getAs<DefinedSVal>();

    // Divide-by-undefined handled in the generic checking for uses of
    // undefined values.
    if (!DVL)
      return;
    // Check for divide by zero.

    SValBuilder &svalBuilder = C.getSValBuilder();
    ProgramStateRef state = C.getState();

    QualType Ty = B->getRHS()->getType();

    SVal LessThan = svalBuilder.evalBinOp(state, BO_LT, DenomL, DenomR, Ty);

    if (Optional<DefinedSVal> LessThanDVal = LessThan.getAs<DefinedSVal>()) {
      ConstraintManager &CM = C.getConstraintManager();
      ProgramStateRef StatePos, StateNeg;

      std::tie(StateNeg, StatePos) = CM.assumeDual(state, *LessThanDVal);
      if (StateNeg) {
        reportBug("Left value maybe less than right value", state, C, B);
        return;
      }
      state = StatePos;
    }
    assert(state);
    C.addTransition(state);
  }
}

void ento::registerUnsignedMinusChecker(CheckerManager &mgr) {
  mgr.registerChecker<UnsignedMinusChecker>();
}

bool ento::shouldRegisterUnsignedMinusChecker(const CheckerManager &mgr) {
  return true;
}