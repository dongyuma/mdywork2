//===--- FunctionParametersCheck.h - clang-tidy -----------------*- C++ -*-===//
//
//   FileName: FunctionParametersCheck.h 
//   Author:   YuXinglin
//   Date:     2021-2-21
//   Code:     UTF-8 without BOM
//   Describe: 名称、符号与变量的使用中关于函数参数名称的检测
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_READABILITY_FUNCTIONPARAMETERSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_READABILITY_FUNCTIONPARAMETERSCHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace readability {

///  FunctionParametersCheck 
class FunctionParametersCheck : public ClangTidyCheck {
public:
  FunctionParametersCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  std::set<std::string> globalVariblenameSet;
  std::set<std::string> identifiernameSet;
};

} // namespace readability
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_READABILITY_FUNCTIONPARAMETERSCHECK_H
