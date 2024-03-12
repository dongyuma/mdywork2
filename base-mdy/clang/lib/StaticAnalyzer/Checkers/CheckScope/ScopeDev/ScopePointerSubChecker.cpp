//=== ScopePointerSubChecker.cpp - Pointer subtraction checker ------*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This files defines ScopePointerSubChecker, a builtin checker that checks for
// pointer subtractions on two pointers pointing to different memory chunks.
// This check corresponds to CWE-469.
//
//===----------------------------------------------------------------------===//
#include "common/CheckScope.h"
#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"

using namespace clang;
using namespace ento;
//using namespace std;

namespace {
class ScopePointerSubChecker
  : public Checker< check::PreStmt<BinaryOperator> > {
  mutable std::unique_ptr<BuiltinBug> BT;
public:
  void checkPreStmt(const BinaryOperator *B, CheckerContext &C) const;
};
} 

void ScopePointerSubChecker::checkPreStmt(const BinaryOperator *B,
                                     //CheckerContext &C) const {
                                     CheckerContext &C) const 
{
  // When doing pointer subtraction, if the two pointers do not point to the
  // same memory chunk, emit a warning.
  // if (B->getOpcode() != BO_Sub)
  //   return;
  if (B->getOpcode() == BO_Sub)
  {
  

  SVal LV = C.getSVal(B->getLHS());
  SVal RV = C.getSVal(B->getRHS());

  
  const MemRegion *LR = LV.getAsRegion();
  const MemRegion *RR = RV.getAsRegion();

  if (!(LR && RR))
    return;

  const MemRegion *BaseLR = LR->getBaseRegion();
  const MemRegion *BaseRR = RR->getBaseRegion();

  if (BaseLR == BaseRR)
  {
    return;
  }
  else
  {
    if (ExplodedNode *N = C.generateNonFatalErrorNode()) 
    {
      if (!BT)
        BT.reset(
            new BuiltinBug(this, "指针减法",
                          "减去不指向同一内存块的两个指针可能会导致不正确的结果。"));
      auto R =
          std::make_unique<PathSensitiveBugReport>(*BT, BT->getDescription(), N);
      R->addRange(B->getSourceRange());
      C.emitReport(std::move(R));
    }
  }
}
  if (B->getOpcode() == BO_Sub ||
        B->getOpcode() == BO_GT ||
        B->getOpcode() == BO_GE ||
        B->getOpcode() == BO_LT ||
        B->getOpcode() == BO_LE)
{
  
  SVal LV = C.getSVal(B->getLHS());
  SVal RV = C.getSVal(B->getRHS());

  
  const MemRegion *LR = LV.getAsRegion();
  const MemRegion *RR = RV.getAsRegion();

  if (!(LR && RR))
    return;

  const MemRegion *BaseLR = LR->getBaseRegion();
  const MemRegion *BaseRR = RR->getBaseRegion();

  if (BaseLR == BaseRR)
  {
    return;
  }
  else
  {
    if (ExplodedNode *N = C.generateNonFatalErrorNode()) 
    {
      if (!BT)
        BT.reset(
            new BuiltinBug(this, "指针减法",
                          "关系操作符不应该在指针上使用除非他们指向同一个对象"));
      auto R =
          std::make_unique<PathSensitiveBugReport>(*BT, BT->getDescription(), N);
      R->addRange(B->getSourceRange());
      C.emitReport(std::move(R));
    }
  }
}
}

// void ento::registerScopePointerSubChecker(CheckerManager &mgr) {
//   mgr.registerChecker<ScopePointerSubChecker>();
// }

// bool ento::shouldRegisterScopePointerSubChecker(const CheckerManager &mgr) {
//   return true;
// }

//编写注册逻辑
class ScopePointerSubCheckerRegister :public scope::CheckerRegisterFactory
{
public:
	virtual void registerCheckers(clang::ento::CheckerRegistry &registry)
	{
		//声明要用到的checker名称(这样可以降低内存开销)
		llvm::StringRef CheckPointerSub= getCheckScopeDevNameStr("PointerSub");

		//添加Checker
		registry.addChecker<ScopePointerSubChecker>(CheckPointerSub,
			"Check the function of PointerSub ", NOT_CHECKER_DOC, NOT_HIDDEN_CHECKER);
	}
};
//显式声明全局对象实例以激活注册
SCOPE_REGISTER_CLASS(ScopePointerSubCheckerRegister)