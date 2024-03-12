//==- InterruptFunctionChecker.cpp ---------------------------*- C++ -*--//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This checker adds an assumption that constant globals of certain types* are
//  non-null, as otherwise they generally do not convey any useful information.
//  The assumption is useful, as many framework use e. g. global const strings,
//  and the analyzer might not be able to infer the global value if the
//  definition is in a separate translation unit.
//  The following types (and their typedef aliases) are considered to be
//  non-null:
//   - `char* const`
//   - `const CFStringRef` from CoreFoundation
//   - `NSString* const` from Foundation
//   - `CFBooleanRef` from Foundation
//
//===----------------------------------------------------------------------===//

#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ExprEngine.h"

using namespace clang;
using namespace ento;

namespace {

class InterruptFunctionChecker : public  Checker< check::ASTDecl<FunctionDecl> >  {

public:
          	void checkASTDecl(const FunctionDecl* D,
                                AnalysisManager& Mgr,
                                BugReporter& BR) const;
};

} // namespace
 
void InterruptFunctionChecker::checkASTDecl(const FunctionDecl* D,
                                              AnalysisManager& Mgr,
                                              BugReporter& BR) const
{
  const FunctionDecl *FD =D;
  if (!FD)
  return;
  if (FD->getStorageClass() == SC_Interrupt) 
  {
    {
      PathDiagnosticLocation ELoc(FD->getBeginLoc(), BR.getSourceManager());
      BR.EmitBasicReport(FD, this, "中断关键字", categories::SecurityError, "是涉及中断关键字的函数", ELoc, FD->getSourceRange());
      return;
    }
  }
}

void ento::registerInterruptFunctionChecker(CheckerManager &Mgr) {
  Mgr.registerChecker<InterruptFunctionChecker>();
}

bool ento::shouldRegisterInterruptFunctionChecker(const CheckerManager &mgr) {
  return true;
}
