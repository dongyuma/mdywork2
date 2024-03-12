//===--- ScopeTidyModule.cpp - clang-tidy---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// 
// 阅境tidy部分checker
//===----------------------------------------------------------------------===//

#include "../ClangTidy.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "EmptyConditionCheck.h"
#include "IntegerDivLossAccuracyCheck.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace scope {

/// This module is for checkscope checks.
class ScopeModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<EmptyconditionCheck>(
        "scope-empty-condition");
    CheckFactories.registerCheck<IntegerdivlossaccuracyCheck>(
        "scope-integer-div-loss-accuracy");
  }
};

// Register the ScopeModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<ScopeModule>
    X("Scope-module", "Adds Scope checks.");
} // namespace scope

// This anchor is used to force the linker to link in the generated object file
// and thus register the ScopeModule.
volatile int ScopeModuleAnchorSource = 0;

} // namespace tidy
} // namespace clang
