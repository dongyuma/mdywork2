set(LLVM_LINK_COMPONENTS
  FrontendOpenMP
  Support
  )

add_clang_library(clangTidyCounterModule

  CounterTidyModule.cpp
  FunctionInfoCheck.cpp

  LINK_LIBS
  clangTidy
  clangTidyUtils

  DEPENDS
  omp_gen
  )

clang_target_link_libraries(clangTidyCounterModule
  PRIVATE
  clangAnalysis
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  )
