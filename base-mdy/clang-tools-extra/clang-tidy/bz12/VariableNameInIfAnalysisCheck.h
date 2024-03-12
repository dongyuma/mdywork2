//===--- VariableNameInIfAnalysisCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_VARIABLENAMEINIFANALYSISCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_VARIABLENAMEINIFANALYSISCHECK_H

#include "../ClangTidyCheck.h"
#include<vector>
#include<set>
#include<string>
#include<map>
#include<queue>
#include <fstream>
#include <vector>
#include <unistd.h>
using namespace std;
namespace clang {
namespace tidy {
namespace bz12 {

/// 检测极性第十大类问题，目前版本已支持所有用例无误报及漏报
/// 但存在以下待改进点：
/// 1. 目前短变量名前面添加的是其类型，可能会带来误报
/// 2. 针对数组下标为常量表达式的情况，目前只支持两个常量计算
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/bz12-variable-name-in-if-analysis.html
class VariableNameInIfAnalysisCheck : public ClangTidyCheck {
public:
    VariableNameInIfAnalysisCheck(StringRef Name, ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context) {}
    void analysisVariableName();
    void onEndOfTranslationUnit() override;
    void checkBinaryOperator(const Expr* MatchedExpr);
    void checkConsistency(string name1,const Expr* right_expr);
    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
    void travelBinaryOperator(const BinaryOperator* MatchedBinaryOperator);
    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
    int calcIndex(const BinaryOperator *BO);
  
    //Used to store reverse filtering strings
    vector<string> negative_string_vector;
    set<string> custom_string_set;
    list<string> custom_string_list_sorted_by_lenth;
    set<string> variable_name_set;
    map<string,string> variable_keyword_map;
    map<string,set<string>> default_string_level_map;
    map<const Expr*,const Expr*> binaryoperator_correspond_map;
    set<string> default_string_set;
    list<string> default_string_list_sorted_by_lenth;

    map<string,set<string>> custom_string_level_map;
    bool custom_case_sensitive;

    map<string, string> sub_name_group_map;
    map<string, string> sub_name_level_map;
    map<string, map<string, string>> group_map;
};

} // namespace bz12
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_VARIABLENAMEINIFANALYSISCHECK_H
