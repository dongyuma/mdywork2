//===--- UninitializedStructElementCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_UNINITIALIZEDSTRUCTELEMENTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_UNINITIALIZEDSTRUCTELEMENTCHECK_H

#include "../ClangTidyCheck.h"

using namespace std;

namespace clang {
namespace tidy {
namespace bz12 {

class UninitializedStructElementCheck : public ClangTidyCheck {
public:
  UninitializedStructElementCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void onEndOfTranslationUnit() override;
  
  //结构体定义节点的集合
  set<const RecordDecl*> defStructSet;
  //变量定义节点的集合
  set<const VarDecl*> varDeclSet;

  //被初始化节点的集合
  set<string> declRefNameSet;

  //key：初始化的结构体类型，value：相应结构体初始化的元素的集合
  map<string,set<string>> initializedStructElementNameMap;
  //key：typedef后的类型，value：原始类型
  map<string,string> typedefCorrespondMap;
  
  //类型字符串中的多余信息，可根据实际情况继续扩充
  vector<string> uselessSubstr = {"struct ", " *", ".", " "};
};

} // namespace bz12
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_UNINITIALIZEDSTRUCTELEMENTCHECK_H