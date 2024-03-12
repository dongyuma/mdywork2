//===--- UnusedStructElementCheck.h - clang-tidy ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//该检查器用于检测诸元（结构体）存在未使用元素
//元素类型应包括结构体普通变量元素、结构体数组元素、结构体子结构体元素以及前几种类型混合
//检查之处应包含条件语句、计算赋值表达式、函数参数、数组下标等位置
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_UnusedStructElementCheck_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_UnusedStructElementCheck_H

#include "../ClangTidyCheck.h"

using namespace std;

namespace clang {
namespace tidy {
namespace bz12 {

class UnusedStructElementCheck : public ClangTidyCheck {
public:
  UnusedStructElementCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void onEndOfTranslationUnit() override;
  string removeUselessSubstr(string targetStr);
  string exchangStructType(string targetStructType);
  
  //结构体定义节点的集合
  set<const RecordDecl*> defStructSet;

  //key：使用的结构体类型，value：相应结构体使用的元素的集合
  map<string,set<string>> usedStructElementNameMap;
  //key：typedef后的类型，value：原始类型
  map<string,string> typedefCorrespondMap;
  
  //类型字符串中的多余信息，可根据实际情况继续扩充
  vector<string> uselessSubstr = {"struct ", " *", ".", " "};
  
  //记录当前节点
  const MemberExpr *currnetNode;
};

} // namespace bz12
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_UnusedStructElementCheck_H
