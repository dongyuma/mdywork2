//= ScopeSinEvaluateChecker.cpp - Checks calls to C string functions --------*- C++ -*-//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions. 
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This defines ScopeSinEvaluateChecker, which is an assortment of checks on calls
// to functions in <string.h>.//这定义了ScopeSinEvaluateChecker，它是对＜string.h＞中函数调用的各种检查。
//
//===----------------------------------------------------------------------===//
#include "common/CheckScope.h"
#include <iostream>
#include <math.h>
#include <stdint.h>
#include "clang/Basic/CharInfo.h"
#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/DynamicExtent.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ProgramStateTrait.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Support/raw_ostream.h"
#define TAB_N 256                                           //定义查表的点数
#define PI 3.1415926535897932384626433832795028841971       //定义圆周率值

using namespace clang;
using namespace ento;
using namespace std;

namespace {
class ScopeSinEvaluateChecker : public Checker< eval::Call
                                         > {
  //mutable std::unique_ptr<BugType> BT_NotCString;
  mutable const char *CurrentFunctionDescription;
public:
  //  The filter is used to filter out the diagnostics which are not enabled by
  //  the user.

  static void *getTag() { static int tag; return &tag; }

  bool evalCall(const CallEvent &Call, CheckerContext &C) const;
  typedef void (ScopeSinEvaluateChecker::*FnCheck)(CheckerContext &,
                                          const CallExpr *) const;
  CallDescriptionMap<FnCheck> Callbacks = {
    {{CDF_MaybeBuiltin, "fabs", 1}, &ScopeSinEvaluateChecker::evalFabs},
      {{CDF_MaybeBuiltin, "sin", 1}, &ScopeSinEvaluateChecker::evalSin},
      {{CDF_MaybeBuiltin, "cos", 1}, &ScopeSinEvaluateChecker::evalCos},
  };
  FnCheck identifyCall(const CallEvent &Call, CheckerContext &C) const;
  void evalFabs(CheckerContext &C, const CallExpr *CE) const;
  void evalFabsCommon(CheckerContext &C,
                           const CallExpr *CE,
                           bool IsStrnlen = false) const;
  void evalSin(CheckerContext &C, const CallExpr *CE) const;
  void evalCos(CheckerContext &C, const CallExpr *CE) const;
  void evalSinCommon(CheckerContext &C,
                           const CallExpr *CE,
                           bool IsStrnlen = false) const;
  void evalCosCommon(CheckerContext &C,
                           const CallExpr *CE,
                           bool IsStrnlen = false) const;
};

} //end anonymous namespace


//===----------------------------------------------------------------------===//
// Individual checks and utility methods.
//===----------------------------------------------------------------------===//




void ScopeSinEvaluateChecker::evalFabs(CheckerContext &C,
                                   const CallExpr *CE) const {
  evalFabsCommon(C, CE, /* IsStrnlen = */ false);
}
void ScopeSinEvaluateChecker::evalSin(CheckerContext &C,
                                   const CallExpr *CE) const {
  evalSinCommon(C, CE, /* IsStrnlen = */ false);
}
void ScopeSinEvaluateChecker::evalCos(CheckerContext &C,
                                   const CallExpr *CE) const {
  evalCosCommon(C, CE, /* IsStrnlen = */ false);
}

void ScopeSinEvaluateChecker::evalFabsCommon(CheckerContext &C, const CallExpr *CE,
                                         bool IsStrnlen) const {
  CurrentFunctionDescription = "evalFabsCommon function";
  ProgramStateRef state = C.getState();
  const LocationContext *LCtx = C.getLocationContext();
  IsStrnlen=true;
  ProgramStateRef stateZeroSize;
 if (IsStrnlen) 
 {
    const Expr *maxlenExpr = CE->getArg(0);
    SVal maxlenVal = state->getSVal(maxlenExpr, LCtx);
    if(maxlenVal.getAs<NonLoc>())
    {
      if(maxlenVal.getAs<nonloc::ConcreteFloat>())
      {
        nonloc::ConcreteFloat CI = maxlenVal.castAs<nonloc::ConcreteFloat>();
        llvm::APFloat AI = CI.getValue();
        float DAI = AI.convertToFloat();
        float result=0.0;
        result=fabs(DAI);
        SValBuilder &SVB = C.getSValBuilder();
        DefinedSVal FloatVal = SVB.makeFloatVal(result);
        state = state->BindExpr(CE, LCtx, FloatVal);
        C.addTransition(state);
      }
    }
  }
}

void ScopeSinEvaluateChecker::evalSinCommon(CheckerContext &C, const CallExpr *CE,
                                         bool IsStrnlen) const {
  CurrentFunctionDescription = "string sin function";
  ProgramStateRef state = C.getState();
  const LocationContext *LCtx = C.getLocationContext();
  IsStrnlen=true;
  ProgramStateRef stateZeroSize;
  if (IsStrnlen) 
  {
    const Expr *maxlenExpr = CE->getArg(0);
    SVal maxlenVal = state->getSVal(maxlenExpr, LCtx);
    if(maxlenVal.getAs<NonLoc>())
    {
      if(maxlenVal.getAs<nonloc::ConcreteFloat>())
      {
        nonloc::ConcreteFloat CI = maxlenVal.castAs<nonloc::ConcreteFloat>();
        llvm::APFloat AI = CI.getValue();
        double DAI = AI.convertToDouble();
        double result=sin(DAI);
        SValBuilder &SVB = C.getSValBuilder();
        DefinedSVal FloatVal = SVB.makeFloatVal(result);
        state = state->BindExpr(CE, LCtx, FloatVal);
        C.addTransition(state);
      }
    }
  }
}

void ScopeSinEvaluateChecker::evalCosCommon(CheckerContext &C, const CallExpr *CE,
                                      bool IsStrnlen) const {
  CurrentFunctionDescription = "string cos function";
  ProgramStateRef state = C.getState();
  const LocationContext *LCtx = C.getLocationContext();
  IsStrnlen=true;
  ProgramStateRef stateZeroSize;
  if (IsStrnlen) 
  {
    const Expr *maxlenExpr = CE->getArg(0);
    SVal maxlenVal = state->getSVal(maxlenExpr, LCtx);
    if(maxlenVal.getAs<NonLoc>())
    {
      if(maxlenVal.getAs<nonloc::ConcreteFloat>())
      {
        nonloc::ConcreteFloat CI = maxlenVal.castAs<nonloc::ConcreteFloat>();
        llvm::APFloat AI = CI.getValue();
        double DAI = AI.convertToDouble();
        double result=cos(DAI);
        SValBuilder &SVB = C.getSValBuilder();
        DefinedSVal FloatVal = SVB.makeFloatVal(result);
        state = state->BindExpr(CE, LCtx, FloatVal);
        C.addTransition(state);
      }
    }
  }
}




//===----------------------------------------------------------------------===//
// The driver method, and other Checker callbacks.
//===----------------------------------------------------------------------===//

ScopeSinEvaluateChecker::FnCheck ScopeSinEvaluateChecker::identifyCall(const CallEvent &Call,
                                                     CheckerContext &C) const {
  const auto *CE = dyn_cast_or_null<CallExpr>(Call.getOriginExpr());
  if (!CE)
  {
    return nullptr;
  }
  const FunctionDecl *FD = dyn_cast_or_null<FunctionDecl>(Call.getDecl());
  if (!FD)
  { 
    return nullptr;
  }
  const FnCheck *Callback = Callbacks.lookup(Call);
  if (Callback)
  {
    return *Callback;
  }
  {
    return nullptr;
  }
}

bool ScopeSinEvaluateChecker::evalCall(const CallEvent &Call, CheckerContext &C) const {
  FnCheck Callback = identifyCall(Call, C);
  // If the callee isn't a string function, let another checker handle it.
  if (!Callback)
  {
    return false;
  }
  // Check and evaluate the call.
  const auto *CE = cast<CallExpr>(Call.getOriginExpr());
  (this->*Callback)(C, CE);
  // If the evaluate call resulted in no change, chain to the next eval call
  // handler.
  // Note, the custom CString evaluation calls assume that basic safety
  // properties are held. However, if the user chooses to turn off some of these
  // checks, we ignore the issues and leave the call evaluation to a generic
  // handler.
  return C.isDifferent();
}



//编写注册逻辑
class ScopeSinEvaluateCheckerRegister :public scope::CheckerRegisterFactory
{
public:
	virtual void registerCheckers(clang::ento::CheckerRegistry &registry) 
	{
		//声明要用到的checker名称(这样可以降低内存开销)
		llvm::StringRef CheckMemoryAllocation = getCheckScopeAlphaNameStr("ScopeSinEvaluate");

		//添加Checker
		registry.addChecker<ScopeSinEvaluateChecker>(CheckMemoryAllocation,
			"Check SinEvaluate modeling", NOT_CHECKER_DOC, NOT_HIDDEN_CHECKER);
	}
}; 
//显式声明全局对象实例以激活注册
SCOPE_REGISTER_CLASS(ScopeSinEvaluateCheckerRegister)

