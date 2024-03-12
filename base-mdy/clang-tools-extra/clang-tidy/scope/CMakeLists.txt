set(LLVM_LINK_COMPONENTS
  FrontendOpenMP
  Support
  )

add_clang_library(clangTidyScopeModule
  EmptyConditionCheck.cpp
  IntegerDivLossAccuracyCheck.cpp
  ScopeTidyModule.cpp

  LINK_LIBS
  clangTidy
  clangTidyUtils

  DEPENDS
  omp_gen
  )

clang_target_link_libraries(clangTidyScopeModule
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  )
