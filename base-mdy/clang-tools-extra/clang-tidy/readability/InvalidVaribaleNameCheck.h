//===--- InvalidVaribaleNameCheck.cpp - clang-tidy -------------------------===//
//
//
//   FileName: FunctionParametersCheck.cpp 
//   Author:   YuXinglin
//   Date:     2021-2-23
//   Code:     UTF-8 without BOM
//   Describe: 语句使用的检测
//   此checker完成关于
//    4.8.1.1 禁止单独使用小写字母“l”和大写字母“O”作为变量名
//    4.8.1.3 使用八进制数必须加以注释
//  的检测
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_READABILITY_INVALIDVARIBALENAMECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_READABILITY_INVALIDVARIBALENAMECHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace readability {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/readability-Invalid-Varibale-Name.html
class InvalidVaribaleNameCheck : public ClangTidyCheck {
public:
  InvalidVaribaleNameCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace readability
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_READABILITY_INVALIDVARIBALENAMECHECK_H
