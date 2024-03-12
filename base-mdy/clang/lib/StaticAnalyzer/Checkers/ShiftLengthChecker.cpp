//== DivZeroChecker.cpp - Division by zero checker --------------*- C++ -*--==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This defines ShiftLengthChecker, a builtin check in ExprEngine that performs
// checks for the displacement operation exceeds the variable length.
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
class ShiftLengthChecker : public Checker< check::PreStmt<BinaryOperator> > {
    mutable std::unique_ptr<BuiltinBug> BT;
    void reportBug(const char *Msg, ProgramStateRef StateExceed, CheckerContext &C,
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

void ShiftLengthChecker::reportBug(
    const char *Msg, ProgramStateRef StateExceed, CheckerContext &C,
    std::unique_ptr<BugReporterVisitor> Visitor) const {
    if (ExplodedNode *N = C.generateErrorNode(StateExceed)) {
        if (!BT)
            BT.reset(new BuiltinBug(this, "移位超过变量长度"));

    auto R = std::make_unique<PathSensitiveBugReport>(*BT, Msg, N);
    R->addVisitor(std::move(Visitor));
    bugreporter::trackExpressionValue(N, getDenomExpr(N), *R);
    C.emitReport(std::move(R));
  }
}

void ShiftLengthChecker::checkPreStmt(const BinaryOperator *B,
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
    ProgramStateRef stateNotExceed, stateExceed;
    if(!(B->getRHS()->getType()->isFloatingType()))
    {
        SValBuilder &svalBuilder = C.getSValBuilder();
        SVal RHS = C.getSVal(B->getRHS());
        uint64_t  NumOfLHS = C.getASTContext().getTypeInfo(B->getLHS()->getType()).Width;
        QualType Type = B->getLHS()->getType();
        bool isUnsigned = 0;
        SVal lowerEqual;
        if (Type->isSignedChar() || Type->isSignedIntegerType()){
            isUnsigned = 0;
        }
        else if (Type->isUnsignedChar() || Type->isUnsignedIntType() || Type->isUnsignedShortType() || 
        Type->isUnsignedLongType() || Type->isUnsignedIntegerType()){
            isUnsigned = 1;
        }
        lowerEqual = svalBuilder.evalBinOp(C.getState(), BO_LT, RHS,svalBuilder.makeIntVal(NumOfLHS, isUnsigned),
                                            svalBuilder.getConditionType());

        Optional<DefinedSVal> FDV = lowerEqual.getAs<DefinedSVal>();
        if (!FDV)
        return;

        std::tie(stateNotExceed, stateExceed) = CM.assumeDual(C.getState(), *FDV);

        bool TaintedD = isTainted(C.getState(), *FDV);
        if (stateExceed && TaintedD) {
            reportBug("移动的位数是污点值，可能超过变量长度", stateExceed, C,
                std::make_unique<taint::TaintBugVisitor>(*FDV));
            return;
        }
        if (!stateNotExceed && stateExceed) {
            // assert(stateZero);
            reportBug("移动的位数超过变量长度", stateExceed, C);
            return;
        }

        C.addTransition(stateNotExceed);
    } else {
        
    }
}

void ento::registerShiftLengthChecker(CheckerManager &mgr) {
  mgr.registerChecker<ShiftLengthChecker>();
}

bool ento::shouldRegisterShiftLengthChecker(const CheckerManager &mgr) {
  return true;
}
