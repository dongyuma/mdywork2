//===--- ZirconTidyModule.cpp - clang-tidy---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../ClangTidy.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "AbsParameterCheck.h"
#include "ArgumentsAndStructMembersSameNameCheck.h"
#include "AssignconsistencyCheck.h"
#include "BasicBlockDoubleAssignCheck.h"
#include "BasicBlockUnusedOrRepeatusedCheck.h"
#include "BlockConsistencyCheck.h"
#include "BranchAssignmentDifferenceCheck.h"
#include "BranchVariableStatisticsCheck.h"
#include "ConditionalStatementEqualityCheck.h"
#include "DifferentFunctionDeclarationAndDefinitionCheck.h"
#include "ExternDefinationDeclareTypeCheck.h"
#include "InfinitelLoopCheck.h"
#include "InitialValueCheck.h"
#include "MatrixCalcParametersCheck.h"
#include "MemsetMultipleRankPointerCheck.h"
#include "StructDefaultAlignmentCheck.h"
#include "StructDefinationDeclareConsistenceCheck.h"
#include "TypeCastCheck.h"
#include "UninitializedStructElementCheck.h"
#include "UnusedStructElementCheck.h"
#include "VariableNameAnalysisCheck.h"
#include "VariableNameInIfAnalysisCheck.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 { 

/// This module is for Zircon-specific checks.
class Bz12Module : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<AssignconsistencyCheck>(
        "bz12-AssignConsistency");
    CheckFactories.registerCheck<AbsParameterCheck>(
        "bz12-abs-parameter");
    CheckFactories.registerCheck<ArgumentsAndStructMembersSameNameCheck>(
        "bz12-arguments-and-struct-members-same-name");
    CheckFactories.registerCheck<BasicBlockDoubleAssignCheck>(
        "bz12-basic-block-double-assign");
    CheckFactories.registerCheck<BasicBlockUnusedOrRepeatusedCheck>(
        "bz12-basic-block-unused-or-repeatused");
    CheckFactories.registerCheck<BlockConsistencyCheck>(
        "bz12-block-consistency");
    CheckFactories.registerCheck<BranchAssignmentDifferenceCheck>(
        "bz12-branch-assignment-difference");
    CheckFactories.registerCheck<BranchVariableStatisticsCheck>(
        "bz12-branch-variable-statistics");
    CheckFactories.registerCheck<ConditionalStatementEqualityCheck>(
        "bz12-conditional-statement-equality");
    CheckFactories.registerCheck<DifferentFunctionDeclarationAndDefinitionCheck>(
        "bz12-different-function-declaration-and-definition");
    CheckFactories.registerCheck<ExternDefinationDeclareTypeCheck>(
        "bz12-extern-defination-declare-type");
    CheckFactories.registerCheck<InfinitelLoopCheck>(
        "bz12-infinitel-loop");
    CheckFactories.registerCheck<InitialValueCheck>(
        "bz12-initial-value");
    CheckFactories.registerCheck<MatrixCalcParametersCheck>(
        "bz12-matrix-calc-parameters");
    CheckFactories.registerCheck<MemsetMultipleRankPointerCheck>(
        "bz12-memset-multiple-rank-pointer");
    CheckFactories.registerCheck<StructDefaultAlignmentCheck>(
        "bz12-struct-default-alignment");
    CheckFactories.registerCheck<StructDefinationDeclareConsistenceCheck>(
        "bz12-struct-defination-declare-consistence");
    CheckFactories.registerCheck<TypeCastCheck>(
        "bz12-type-cast");
    CheckFactories.registerCheck<UninitializedStructElementCheck>(
        "bz12-uninitialized-struct-element");
    CheckFactories.registerCheck<UnusedStructElementCheck>(
        "bz12-unused-struct-element");
    CheckFactories.registerCheck<VariableNameAnalysisCheck>(
        "bz12-variable-name-analysis");
    CheckFactories.registerCheck<VariableNameInIfAnalysisCheck>(
        "bz12-variable-name-in-if-analysis");
  }
};

// Register the ZirconTidyModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<Bz12Module>
    X("Bz12-module", "Adds GB 5369 checks.");
} // namespace bz12

// This anchor is used to force the linker to link in the generated object file
// and thus register the Bz12Module.
volatile int Bz12ModuleAnchorSource = 0;

} // namespace tidy
} // namespace clang
