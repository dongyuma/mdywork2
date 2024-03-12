set(LLVM_LINK_COMPONENTS
  FrontendOpenMP
  Support
  )

add_clang_executable(clang-keygen
  ClangKeygen.cpp
  )

clang_target_link_libraries(tool-template
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangTooling
  clangToolingRefactoring
  )
