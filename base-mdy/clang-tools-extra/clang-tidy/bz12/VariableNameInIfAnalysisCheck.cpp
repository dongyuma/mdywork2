//===--- VariableNameInIfAnalysisCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "VariableNameInIfAnalysisCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <iostream>
#include <fstream>
#include <string>
#include "rapidjson/include/rapidjson/document.h"
#include "utils.h"

using namespace std;
using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

void VariableNameInIfAnalysisCheck::registerMatchers(MatchFinder *Finder) {
    //匹配If语句块及其每个比较binaryOperator后继节点
    Finder->addMatcher(ifStmt
                        (
                        unless(isExpansionInSystemHeader()),
                        forEachDescendant(binaryOperator(isComparisonOperator()).bind("binaryOperator"))
                        ).bind("ifStmt")
                        , this);
}

void VariableNameInIfAnalysisCheck::check(const MatchFinder::MatchResult &Result) {    
    /*获取比较类型的BinaryOperator节点，判断BinaryOperator是否位于if条件语句中，调用travelBinaryOperator遍历BinaryOperator*/
    if(const auto *MatchedBinaryOperator = Result.Nodes.getNodeAs<BinaryOperator>("binaryOperator")) {
        if(const auto *MatchedIfStmt = Result.Nodes.getNodeAs<IfStmt>("ifStmt")){
            unsigned IfStmtCondBeginLocation=MatchedIfStmt->getCond()->getBeginLoc().getHashValue();
            unsigned IfStmtCondEndLocation=MatchedIfStmt->getCond()->getEndLoc().getHashValue();
            unsigned BinaryOperatorLocation=MatchedBinaryOperator->getOperatorLoc().getHashValue();
            if(BinaryOperatorLocation>IfStmtCondBeginLocation && BinaryOperatorLocation<IfStmtCondEndLocation){
                travelBinaryOperator(MatchedBinaryOperator);
            }
        }
    }
}
/*BinaryOperator遍历函数,将BinaryOperator涉及的变量名以及等式两边的对应关系加入相应集合*/
void VariableNameInIfAnalysisCheck::travelBinaryOperator(const BinaryOperator* MatchedBinaryOperator){
    //若BinaryOperator两均为DeclRefExpr、MemberExpr或ArraySubscriptExpr
    if((isa<DeclRefExpr>(MatchedBinaryOperator->getLHS()->IgnoreParenImpCasts()) || \
        isa<MemberExpr>(MatchedBinaryOperator->getLHS()->IgnoreParenImpCasts()) || \
        isa<ArraySubscriptExpr>(MatchedBinaryOperator->getLHS()->IgnoreParenImpCasts())) && \
        (isa<DeclRefExpr>(MatchedBinaryOperator->getRHS()->IgnoreParenImpCasts()) || \
        isa<MemberExpr>(MatchedBinaryOperator->getRHS()->IgnoreParenImpCasts()) || \
        isa<ArraySubscriptExpr>(MatchedBinaryOperator->getRHS()->IgnoreParenImpCasts()))) {

        checkBinaryOperator(MatchedBinaryOperator->getLHS()->IgnoreParenImpCasts());
        checkBinaryOperator(MatchedBinaryOperator->getRHS()->IgnoreParenImpCasts());
        binaryoperator_correspond_map[MatchedBinaryOperator->getLHS()->IgnoreParenImpCasts()]=MatchedBinaryOperator->getRHS()->IgnoreParenImpCasts();
    }
}
/*根据default_string_level_set对等价集合equivalent_string_map进行初始化*/
void VariableNameInIfAnalysisCheck::onEndOfTranslationUnit() {
    read_negative_string_vector(negative_string_vector);
    read_default_string_level_map(default_string_level_map);
    read_default_string_set(default_string_set);
    read_custom_string_set (custom_string_set);
    read_custom_string_level_map(custom_string_level_map);
    read_subname_map(sub_name_group_map, sub_name_level_map, group_map);

    custom_case_sensitive = read_custom_case_sensitive ();
    transform_default_string_set_to_list(default_string_list_sorted_by_lenth, default_string_set);
    transform_custom_string_set_to_list(custom_string_list_sorted_by_lenth, custom_string_set);

    analysisVariableName();
}

void VariableNameInIfAnalysisCheck::analysisVariableName() {
    //生成每个变量名的关键字
    for(set<string>::iterator it=variable_name_set.begin(); it!=variable_name_set.end(); it++){
        for(set<string>::iterator it2=variable_name_set.begin(); it2!=variable_name_set.end(); it2++){
            if(*it!=*it2){
                string sub_name1, sub_name2;
                int start_pos1=0, start_pos2=0;
                bool analysisResult = analysisNameSimilarity(
                    default_string_list_sorted_by_lenth, custom_string_list_sorted_by_lenth, 
                    *it, *it2, sub_name1, sub_name2, start_pos1, start_pos2);
                if(analysisResult) {
                    variable_keyword_map[*it] = sub_name1;
                    variable_keyword_map[*it2] = sub_name2;
                }
            }
        }
    }
    //遍历检查每一个binaryoperator等式
    map<const Expr*,const Expr*>::iterator iter;
    for (iter=binaryoperator_correspond_map.begin(); iter!=binaryoperator_correspond_map.end();iter++){
        if(isa<DeclRefExpr>(iter->first)){
            const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(iter->first);
            checkConsistency(DRE->getNameInfo().getName().getAsString(), iter->second);
        }
        else if(isa<MemberExpr>(iter->first)){
            const MemberExpr *ME = dyn_cast<MemberExpr>(iter->first);
            // checkConsistency(ME->getType().getAsString()+ME->getMemberDecl()->getDeclName().getAsString(),iter->second);
            checkConsistency(ME->getMemberDecl()->getDeclName().getAsString(),iter->second);
        }
        else if(isa<ArraySubscriptExpr>(iter->first)) {
            const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(iter->first);
            //数组元素仅检查下标
            if(const IntegerLiteral *IL = dyn_cast<IntegerLiteral>(ASE->getIdx())){
                checkConsistency("arrayindex"+std::to_string(IL->getValue().getSExtValue()),iter->second);
            }
            //增加对数组下标为表达式的支持
            else if(const BinaryOperator *BO = dyn_cast<BinaryOperator>(ASE->getIdx())){
                int index = calcIndex(BO);
                if(index != -1)
                {
                    checkConsistency("arrayindex"+std::to_string(index),iter->second);
                }
            }
        }
    }
}
//变量名一致性检查函数
void VariableNameInIfAnalysisCheck::checkConsistency(string expr_name, const Expr* right_expr){
    size_t index = 0;

    bool ShouldCheck = false;
    string set_key_word = "";
    if(expr_name.find("arrayindex")==string::npos) {
        if(variable_keyword_map.find(expr_name) != variable_keyword_map.end()) {
            expr_name = variable_keyword_map[expr_name];
            ShouldCheck = true;
        }
    } else if(case_insensitive_find_in_set(expr_name, default_string_set, set_key_word)) {  //检查数组元素下标是否位于等价集合
        ShouldCheck=true;
    }

    bool found = case_insensitive_find_in_negative_vector(expr_name, negative_string_vector, index);
    if(found) {
        return;
    }

    if(ShouldCheck==true) {
        if(isa<DeclRefExpr>(right_expr)) {
            const DeclRefExpr *MatchedDeclRefExpr = dyn_cast<DeclRefExpr>(right_expr);
            bool Check=false;
            string key_word=MatchedDeclRefExpr->getNameInfo().getName().getAsString();

            if(case_insensitive_find_in_negative_vector(key_word, negative_string_vector, index)) {
                return;
            }

            if(variable_keyword_map.find(key_word)!=variable_keyword_map.end()) {
                key_word=variable_keyword_map[key_word];
                Check=true;
            }
            string lhs_level = "", rhs_level = "";

            if(lhs_level == "") {
                lhs_level = get_default_string_level(expr_name, default_string_level_map);
            }
            if(lhs_level == "") {
                lhs_level = get_custom_string_level(expr_name, custom_string_level_map);
            }

            if(rhs_level == "") {
                rhs_level = get_default_string_level(key_word, default_string_level_map);
            }
            if(rhs_level == "") {
                rhs_level = get_custom_string_level(key_word, custom_string_level_map);
            }

            if(lhs_level != "" && rhs_level != "") {
                if(lhs_level == rhs_level) {
                    Check = false;
                }
            }

            if(Check){
                if(default_string_level_map[expr_name].find(key_word)==default_string_level_map[expr_name].end() && sub_name_group_map[key_word] != sub_name_group_map[expr_name]){
                    diag(MatchedDeclRefExpr->getBeginLoc(),"左值与右值不匹配");
                }
            }
        }
        else if(isa<MemberExpr>(right_expr)){
            const MemberExpr *ME = dyn_cast<MemberExpr>(right_expr);
            bool Check=false;
            string key_word=ME->getMemberDecl()->getDeclName().getAsString();
            //忽略大小写
            for (int i = 0; i < key_word.size(); i++) {
                key_word[i] = tolower(key_word[i]);
            }
            //对负向字符串进行过滤
            for (int i = 0; i < negative_string_vector.size(); i++) {
                if (key_word.find(negative_string_vector[i]) != key_word.npos) {
                    return;
                }
            }
            if(variable_keyword_map.find(key_word)!=variable_keyword_map.end()){
                key_word=variable_keyword_map[key_word];
                Check=true;
            }

            string lhs_level = "", rhs_level = "";

            if(lhs_level == "") {
                lhs_level = get_default_string_level(expr_name, default_string_level_map);
            }
            if(lhs_level == "") {
                lhs_level = get_custom_string_level(expr_name, custom_string_level_map);
            }

            if(rhs_level == "") {
                rhs_level = get_default_string_level(key_word, default_string_level_map);
            }
            if(rhs_level == "") {
                rhs_level = get_custom_string_level(key_word, custom_string_level_map);
            }

            if(lhs_level != "" && rhs_level != "" && sub_name_group_map[key_word] != sub_name_group_map[expr_name]) {
                if(lhs_level == rhs_level) {
                    Check = false;
                } else {
                    diag(ME->getBeginLoc(),"左值与右值不匹配");
                }
            }

        }
        else if(isa<ArraySubscriptExpr>(right_expr)){
            const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(right_expr);
            if(const IntegerLiteral *IL = dyn_cast<IntegerLiteral>(ASE->getIdx())){
                //检查数组下标是否位于等价集合

                string key_word = "arrayindex"+std::to_string(IL->getValue().getSExtValue());
                string lhs_level = "", rhs_level = "";

                if(lhs_level == "") {
                    lhs_level = get_default_string_level(expr_name, default_string_level_map);
                }
                if(lhs_level == "") {
                    lhs_level = get_custom_string_level(expr_name, custom_string_level_map);
                }

                if(rhs_level == "") {
                    rhs_level = get_default_string_level(key_word, default_string_level_map);
                }
                if(rhs_level == "") {
                    rhs_level = get_custom_string_level(key_word, custom_string_level_map);
                }

                if(lhs_level != "" && rhs_level != "") {
                    if(lhs_level == rhs_level || sub_name_group_map[key_word] == sub_name_group_map[expr_name]) {
                        return;
                    } else {
                        diag(ASE->getBeginLoc(),"左值与右值不匹配");
                    }
                }

            } else if(const BinaryOperator *BO = dyn_cast<BinaryOperator>(ASE->getIdx())){
                int index = calcIndex(BO);
                string key_word = "arrayindex" + std::to_string(index);
                string lhs_level = "", rhs_level = "";

                if(lhs_level == "") {
                    lhs_level = get_default_string_level(expr_name, default_string_level_map);
                }
                if(lhs_level == "") {
                    lhs_level = get_custom_string_level(expr_name, custom_string_level_map);
                }

                if(rhs_level == "") {
                    rhs_level = get_default_string_level(key_word, default_string_level_map);
                }
                if(rhs_level == "") {
                    rhs_level = get_custom_string_level(key_word, custom_string_level_map);
                }

                if(lhs_level != "" && rhs_level != "") {
                    if(lhs_level == rhs_level || sub_name_group_map[key_word] == sub_name_group_map[expr_name]) {
                        return;
                    } else {
                        diag(ASE->getBeginLoc(),"左值与右值不匹配");
                    }
                }
            }
        }
    }
}

/*抽象函数，用于将BinaryOperator两侧的变量名加入variable_name_set集合*/
void VariableNameInIfAnalysisCheck::checkBinaryOperator(const Expr* MatchedExpr){
    if(isa<DeclRefExpr>(MatchedExpr)){
        const DeclRefExpr *MatchedDeclRefExpr = dyn_cast<DeclRefExpr>(MatchedExpr);
        string var_name = MatchedDeclRefExpr->getNameInfo().getName().getAsString();
        // for (int i = 0; i < var_name.size(); i++) {
        //     var_name[i] = tolower(var_name[i]);
        // }
        variable_name_set.insert(var_name);
    } else if(isa<MemberExpr>(MatchedExpr)) {
        const MemberExpr *MatchedMemberExpr = dyn_cast<MemberExpr>(MatchedExpr);
        string var_name = MatchedMemberExpr->getMemberDecl()->getDeclName().getAsString();
        // for (int i = 0; i < var_name.size(); i++) {
        //     var_name[i] = tolower(var_name[i]);
        // }
        variable_name_set.insert(var_name);
    }
    //不对数组元素进行处理
}

//计算常量表达式的结果
int VariableNameInIfAnalysisCheck::calcIndex(const BinaryOperator *BO){
    int lValue = 0;
    int rValue = 0;

    if(const IntegerLiteral *L_IL = dyn_cast<IntegerLiteral>(BO->getLHS()->IgnoreParenImpCasts())){
        lValue = L_IL->getValue().getSExtValue();
    }
    else if(const BinaryOperator *L_BO = dyn_cast<BinaryOperator>(BO->getLHS()->IgnoreParenImpCasts())){
        lValue = calcIndex(L_BO);
        if(lValue == -1) {
            return -1;
        }
    } else {
        return -1;
    }

    if(const IntegerLiteral *R_IL = dyn_cast<IntegerLiteral>(BO->getRHS()->IgnoreParenImpCasts())) {
        rValue = R_IL->getValue().getSExtValue();
    } else if(const BinaryOperator *R_BO = dyn_cast<BinaryOperator>(BO->getRHS()->IgnoreParenImpCasts())){
        rValue = calcIndex(R_BO);
        if(rValue == -1) {
            return -1;
        }
    } else {
        return -1;
    }

    switch(BO->getOpcode())
    {
        case BO_Add:
        return lValue+rValue;
        case BO_Sub:
        return lValue-rValue;
        case BO_Mul:
        return lValue*rValue;
        case BO_Div:
        return lValue/rValue;
        case BO_Rem:
        return lValue%rValue;
        default:
        return -1;
    }
}

} // namespace bz12
} // namespace tidy
} // namespace clang
