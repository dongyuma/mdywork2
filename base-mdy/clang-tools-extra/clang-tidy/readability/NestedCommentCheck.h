//===--- NestedCommentCheck.h - clang-tidy ----------------------*- C++ -*-===//
//
//
//   FileName: NestedCommentCheck.h
//   Author:   YuXinglin
//   Date:     2021-3-4
//   Code:     UTF-8 without BOM
//   Describe: 禁止使用嵌套注释
//   此checker完成关于  4.10.1.1 禁止使用嵌套注释的检测
//  
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_READABILITY_NESTEDCOMMENTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_READABILITY_NESTEDCOMMENTCHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace readability {

class NestedCommentCheck : public ClangTidyCheck {
public:
  NestedCommentCheck(StringRef Name, ClangTidyContext *Context);
    
  ~NestedCommentCheck();

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;

private:
  class NestedCommentHandler;
  std::unique_ptr<NestedCommentHandler> Handler;
};

} // namespace readability
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_READABILITY_NESTEDCOMMENTCHECK_H
