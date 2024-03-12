//=== PointerAssignChecker.cpp - Pointer subtraction checker ------*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//


#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"

using namespace clang;
using namespace ento;

namespace {
    class PointerAssignChecker
        : public Checker< check::PreStmt<BinaryOperator> > {

    public:
        void checkPreStmt(const BinaryOperator* B, CheckerContext& C) const;
    };
}

void PointerAssignChecker::checkPreStmt(const BinaryOperator* B,
    CheckerContext& C) const {
   
}

void ento::registerPointerAssignChecker(CheckerManager& mgr) {
    mgr.registerChecker<PointerAssignChecker>();
}

bool ento::shouldRegisterPointerAssignChecker(const CheckerManager& mgr) {
    return true;
}
