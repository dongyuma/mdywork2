set(LLVM_LINK_COMPONENTS
  FrontendOpenMP
  Support
  )

add_clang_library(clangTidyBz12Module 
  AbsParameterCheck.cpp
  ArgumentsAndStructMembersSameNameCheck.cpp
  BasicBlockDoubleAssignCheck.cpp
  BasicBlockUnusedOrRepeatusedCheck.cpp
  BlockConsistencyCheck.cpp
  BranchAssignmentDifferenceCheck.cpp
  DifferentFunctionDeclarationAndDefinitionCheck.cpp
  ExternDefinationDeclareTypeCheck.cpp
  InfinitelLoopCheck.cpp
  InitialValueCheck.cpp
  MatrixCalcParametersCheck.cpp
  MemsetMultipleRankPointerCheck.cpp
  StructDefinationDeclareConsistenceCheck.cpp
  TypeCastCheck.cpp
  utils.cpp
  AssignconsistencyCheck.cpp
  BranchVariableStatisticsCheck.cpp
  Bz12TidyModule.cpp
  ConditionalStatementEqualityCheck.cpp
  StructDefaultAlignmentCheck.cpp
  UninitializedStructElementCheck.cpp
  UnusedStructElementCheck.cpp
  VariableNameAnalysisCheck.cpp
  VariableNameInIfAnalysisCheck.cpp

  LINK_LIBS
  clangTidy
  clangTidyUtils

  DEPENDS
  omp_gen
  )

clang_target_link_libraries(clangTidyBz12Module
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  )
