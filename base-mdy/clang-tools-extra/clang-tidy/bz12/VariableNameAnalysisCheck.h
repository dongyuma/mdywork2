//===--- VariableNameAnalysisCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_VARIABLENAMEANALYSISCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_VARIABLENAMEANALYSISCHECK_H

#include "../ClangTidyCheck.h"
#include<vector>
#include<set>
#include<string>
#include<map>
#include<queue>
#include <fstream>
#include <vector>
using namespace std;

namespace clang {
namespace tidy {
namespace bz12 {

/// 在一些程序中，变量的命名和赋值是有一定规律的，对于一些特殊变量的赋值，左值和
/// 右值必须一一对应，本检测器就是实现这样的功能。
/// 注意：变量名中的x、y、z与1、2、3一一对应，与数组下标0、1、2一一对应。

class VariableNameAnalysisCheck : public ClangTidyCheck {
public:
    VariableNameAnalysisCheck(StringRef Name, ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context) {}
    void analysisVariableName();
    void onEndOfTranslationUnit() override;
    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
    void checkConsistency(string name1,vector<const Expr*> expr_vector);
    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
    string getVarName(const Expr* MatchedExpr);
    bool shouldCheck(string keyword1, string keyword2);
    void reportBug(string expr_name, string key_word, const Expr* MatchedExpr);
    void doCheck(string expr_name, string keyword, vector<const Expr*> expr_vector, size_t &index);
    vector<Expr*> handlingBinaryOperator(const BinaryOperator *CircleBinaryOperator);    
    std::string getInterLiteralExprString(const BinaryOperator *BO) const;
    std::string getArraySubscriptExprString(const ArraySubscriptExpr *ASE) const;    
    std::string getMemberExprString(const MemberExpr *ME) const;    
    std::string getDeclRefExprString(const DeclRefExpr *DRE) const;       
    int calcIndex(const BinaryOperator *BO) const;
    void travelExpr(const Expr* matchedExpr, set<const Expr*> &exprSet);
    
    bool unread = true;
    
    //默认数组为三通道,故最大下标为2
    int maxIndexValue = 2;
    
    //Used to store reverse filtering strings
    vector<string> negative_string_vector;
    set<string> custom_string_set;
    set<string> variable_name_set;
    map<string, set<string>> variable_keyword_map;
    map<const VarDecl*,vector<const Expr*>> vardecl_correspond_map;
    map<const Expr*,vector<const Expr*>> binaryoperator_correspond_map;
    set<string> default_string_set;
    map<string,set<string>> default_string_level_map;
    map<string,set<string>> custom_string_level_map;

    //临时新增两个map，用于报错时报在整个表达式而非某个变量，从而减少报错量，后续优化代码
    map<const Expr*, const VarDecl*> expr_varDecl_map;
    map<const Expr*, const BinaryOperator*> expr_binaryOperator_map;

    map<string, set<string>> keyword_levelset_map;
    string nested_variable_check_mode;

    bool custom_case_sensitive;

    list<string> custom_string_list_sorted_by_lenth;
    list<string> default_string_list_sorted_by_lenth;

    //key:关键字，value:组号，如:group1
    map<string, string> sub_name_group_map;
    //
    map<string, string> sub_name_level_map;
    //key:组号，value-key:level，value-value:关键字
    map<string, map<string, string>> group_map;

};

} // namespace bz12
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_VARIABLENAMEANALYSISCHECK_H
