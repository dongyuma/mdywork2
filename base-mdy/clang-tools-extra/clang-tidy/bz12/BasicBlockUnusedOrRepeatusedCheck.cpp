//===--- BasicBlockUnusedOrRepeatusedCheck.cpp - clang-tidy ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "BasicBlockUnusedOrRepeatusedCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "utils.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

void BasicBlockUnusedOrRepeatusedCheck::traverse_cfg_preds_to_vector(const CFGBlock *CFGBlock, vector<string> & var_name_vec) {
    if(CFGBlock) {
        CFGBlock::const_pred_iterator cfg_begin = CFGBlock->pred_begin();
        CFGBlock::const_pred_iterator cfg_end = CFGBlock->pred_end();
        for(; cfg_begin!=cfg_end; cfg_begin++) {
            traverse_cfg_block_to_vertor(*cfg_begin, var_name_vec, var_name_vec);
        }
    }
}

void BasicBlockUnusedOrRepeatusedCheck::traverse_cfg_block_to_vertor(const CFGBlock *CFGBlock, vector<string> & left_var_name_vec, vector<string> & right_var_name_vec) {
    for(CFGBlock::const_iterator IB=CFGBlock->begin(); IB!=CFGBlock->end(); ++IB) {
        const CFGElement CBE = *IB;
        if (Optional<CFGStmt> LastStmt = CBE.getAs<CFGStmt>()) {
            if(const BinaryOperator *BO = dyn_cast<BinaryOperator>(LastStmt->getStmt())) {
                if(BO->getOpcode() == BO_Assign) {
                    getExprArrayLenthMap(BO, array_lenth_map);

                    unsigned sign_arr1 = 0, sign_arr2 = 0;
                    string arr1_string = "", arr2_string = "";
                    if(const ArraySubscriptExpr *ASERE = dyn_cast<ArraySubscriptExpr>(BO->getRHS()->IgnoreCasts()->IgnoreParenImpCasts())) {
                        arr1_string = getArraySubscriptExprString(ASERE);
                        sign_arr1 = 1;
                    }
                    if(const ArraySubscriptExpr *ASELE = dyn_cast<ArraySubscriptExpr>(BO->getLHS()->IgnoreCasts()->IgnoreParenImpCasts())) {
                        arr2_string = getArraySubscriptExprString(ASELE);
                        sign_arr2 = 1;
                    }
                    if(sign_arr1 && sign_arr2) {
                        size_t index;
                        for(index = arr1_string.size()-1; index > 0; index--) {
                            if(arr1_string[index] == '+' && arr1_string[index-1] == '+'){
                                break;
                            }
                        }
                        arr1_string = arr1_string.substr(0, index-1);
                        for(index = arr2_string.size()-1; index > 0; index--) {
                            if(arr2_string[index] == '+' && arr2_string[index-1] == '+'){
                                break;
                            }
                        }
                        arr2_string = arr2_string.substr(0, index-1);

                        if(array_lenth_map.find(arr1_string) != array_lenth_map.end() && array_lenth_map.find(arr2_string) != array_lenth_map.end()) {
                            if(array_lenth_map[arr1_string] > array_lenth_map[arr2_string]) {
                                continue;
                            }
                        }
                    }
                    std::string var_name = "";
                    std::string arr_str = "";
                    std::vector<std::string> vec_str;
                    Expr *RE = BO->getRHS()->IgnoreCasts()->IgnoreParenImpCasts();
                    var_name = "";
                    if(const DeclRefExpr *RDRE = dyn_cast<DeclRefExpr>(RE)) {
                        var_name = getDeclRefExprString(RDRE);
                    } else if(const ArraySubscriptExpr *ASERE = dyn_cast<ArraySubscriptExpr>(RE)) {
                        int sign = 0;
                        if(const DeclRefExpr *DRE2 = dyn_cast<DeclRefExpr>(ASERE->getBase()->IgnoreCasts()->IgnoreParenImpCasts())) {
                            if(const ConstantArrayType *CAT= dyn_cast<ConstantArrayType>(DRE2->getDecl()->getType())) {
                                if(CAT->getSize().toString(10, true)=="1" || CAT->getSize().toString(10, true)=="2") {
                                    sign = 1;
                                }
                            }
                        }
                        if(sign == 0) {
                            var_name = getArraySubscriptExprString(ASERE);
                            getArraySubscriptExprStringVector(ASERE, vec_str);
                        }
                    } else if(const MemberExpr *MERE = dyn_cast<MemberExpr>(RE)) {
                        var_name = getMemberExprString(MERE);
                        getMemberExprVector(MERE, vec_str);
                    } else if(const BinaryOperator * RBO = dyn_cast<BinaryOperator>(RE)) {
                        getBinaryStringVector(RBO, vec_str);
                    } else if(const CallExpr * CE = dyn_cast<CallExpr>(RE)) {
                        getCallExprStringVector(CE, vec_str, arr_str);
                    } else {}
                    if(var_name != "") {
                        right_var_name_vec.push_back(var_name);
                    }
                    for(std::size_t i=0; i<vec_str.size(); i++) {
                        if(vec_str[i] != "") {
                            right_var_name_vec.push_back(vec_str[i]);
                        } 
                    }
                    if(arr_str != "") {
                        right_var_name_vec.push_back(arr_str);
                    }

                    vec_str.clear();
                    if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(BO->getLHS())) {
                        var_name = getDeclRefExprString(DRE);
                    } else if(const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(BO->getLHS())) {
                        int sign = 0;
                        if(const DeclRefExpr *DRE2 = dyn_cast<DeclRefExpr>(ASE->getBase()->IgnoreCasts()->IgnoreParenImpCasts())) {
                            if(const ConstantArrayType *CAT= dyn_cast<ConstantArrayType>(DRE2->getDecl()->getType())) {
                                if(CAT->getSize().toString(10, true)=="1" || CAT->getSize().toString(10, true)=="2") {
                                    sign = 1;
                                }
                            }
                        }
                        if(sign == 0) {
                            var_name = getArraySubscriptExprString(ASE);
                            getArraySubscriptExprStringVector(ASE, vec_str);
                        }
                    } else if(const MemberExpr *ME = dyn_cast<MemberExpr>(BO->getLHS())) {
                        var_name = getMemberExprString(ME);
                        getMemberExprVector(ME, vec_str);
                    }

                    if(var_name != "") {
                        left_var_name_vec.push_back(var_name);
                    }
                    for(std::size_t i=0; i<vec_str.size(); i++) {
                        if(vec_str[i] != "") {
                            left_var_name_vec.push_back(vec_str[i]);
                        }
                    }
                }
            } else if(const DeclStmt *DS = dyn_cast<DeclStmt>(LastStmt->getStmt())) {
                if(DS-> isSingleDecl()) {
                    const VarDecl *VD = dyn_cast<VarDecl>(DS->getSingleDecl());
                    std::string var_name_decl = VD->getDeclName().getAsString();

                    if(VD->hasInit()) {
                        left_var_name_vec.push_back(var_name_decl);
                        const Expr *RE = VD->getInit()->IgnoreParenImpCasts();
                        std::string var_name = "";
                        std::string arr_str = "";
                        std::vector<std::string> vec_str;
                        if(const DeclRefExpr *RDRE = dyn_cast<DeclRefExpr>(RE)) {
                            var_name = getDeclRefExprString(RDRE);
                        } else if(const ArraySubscriptExpr *ASERE = dyn_cast<ArraySubscriptExpr>(RE)) {
                            int sign = 0;
                            if(const DeclRefExpr *DRE2 = dyn_cast<DeclRefExpr>(ASERE->getBase()->IgnoreCasts()->IgnoreParenImpCasts())) {
                                if(const ConstantArrayType *CAT= dyn_cast<ConstantArrayType>(DRE2->getDecl()->getType())) {
                                    if(CAT->getSize().toString(10, true)=="1" || CAT->getSize().toString(10, true)=="2") {
                                        sign = 1;
                                    }
                                }
                            }
                            if(sign == 0) {
                                var_name = getArraySubscriptExprString(ASERE);
                                getArraySubscriptExprStringVector(ASERE, vec_str);
                            }
                        } else if(const MemberExpr *MERE = dyn_cast<MemberExpr>(RE)) {
                            var_name = getMemberExprString(MERE);
                            getMemberExprVector(MERE, vec_str);
                        } else if(const BinaryOperator * RBO = dyn_cast<BinaryOperator>(RE)) {
                            getBinaryStringVector(RBO, vec_str);
                        } else if(const CallExpr * CE = dyn_cast<CallExpr>(RE)) {
                            getCallExprStringVector(CE, vec_str, arr_str);
                        } else {}
                        if(var_name != "") {
                            right_var_name_vec.push_back(var_name);
                        }
                        for(std::size_t i=0; i<vec_str.size(); i++) {
                            if(vec_str[i] != "") {
                                right_var_name_vec.push_back(vec_str[i]);
                            } 
                        }
                        if(arr_str != "") {
                            right_var_name_vec.push_back(arr_str);
                        }
                    }
                }
            } else if(const CallExpr *CE = dyn_cast<CallExpr> (LastStmt->getStmt())) {
                std::vector<std::string> vec_str;
                std::string arr_str = "";
                getCallExprStringVector(CE, vec_str, arr_str);
                for(std::size_t i=0; i<vec_str.size(); i++) {
                    if(vec_str[i] != "") {
                        right_var_name_vec.push_back(vec_str[i]);
                    } 
                }
                if(arr_str != "") {
                    right_var_name_vec.push_back(arr_str);
                }
            } 
        }
    }
}

void BasicBlockUnusedOrRepeatusedCheck::registerMatchers(MatchFinder *Finder) {
    Finder->addMatcher(functionDecl(unless(isExpansionInSystemHeader())).bind("functionDecl"), this);
    Finder->addMatcher(forStmt(unless(isExpansionInSystemHeader())).bind("forStmt"), this);
}

void BasicBlockUnusedOrRepeatusedCheck::analysis_unuse(vector<string> & var_name_vec, vector<string> & zero_vec, vector<string> & zero_key_vec, map<string, 
    vector<string>> & group_name_vec, map<string, set<int>> & group_index_set,  map<string, set<string>> & var_key_map,  map<string, string> & group_key_map,
    const CFGBlock *CFGBlock) {
    int sign1 = 0, sign2=0;
    map<string, set<int>> start_pos_map;

    for(vector<string>::iterator it=var_name_vec.begin(); it!=var_name_vec.end(); it++, sign1++) {
        sign2 = sign1;
        for(vector<string>::iterator it2=it; it2!=var_name_vec.end(); it2++, sign2++) {
            if(*it!=*it2){
                string sub_name1, sub_name2;
                int start_pos1 = 0, start_pos2 = 0;
                bool analysisResult = analysisArraySubNameSimilarity(
                    default_string_list_sorted_by_lenth, custom_string_list_sorted_by_lenth, 
                    *it, *it2, sub_name1, sub_name2, start_pos1, start_pos2);
                if(!analysisResult)
                    analysisResult = analysisNameSimilarity(
                        default_string_list_sorted_by_lenth, custom_string_list_sorted_by_lenth, 
                        *it, *it2, sub_name1, sub_name2, start_pos1, start_pos2);
                if(analysisResult) {
                    if(sub_name_group_map[sub_name1] == sub_name_group_map[sub_name2] || (StartWith(sub_name1, "arrayindex") && StartWith(sub_name2, "arrayindex"))) {
                        string group_name = it->substr(0, start_pos1);
                        // cout << "*********************" << endl;
                        // cout << " *it: " << *it << endl;
                        // cout << "*it2: " << *it2 << endl;
                        // cout << "sub_name1: " << sub_name1 << endl;
                        // cout << "sub_name2: " << sub_name2 << endl;
                        // cout << "group_name: " << group_name << endl;
                        if(group_name != "") {
                            if(!StartWith(sub_name1, "arrayindex"))
                                group_name += sub_name_group_map[sub_name1];
                            if(group_name_vec.find(group_name) != group_name_vec.end()) {
                                if(group_index_set[group_name].find(sign1) == group_index_set[group_name].end()) {
                                    group_name_vec[group_name].push_back(*it);
                                    group_index_set[group_name].insert(sign1);
                                }

                                if(group_index_set[group_name].find(sign2) == group_index_set[group_name].end()) {
                                    group_name_vec[group_name].push_back(*it2);
                                    group_index_set[group_name].insert(sign2);
                                }

                                var_key_map[*it].insert(sub_name1);
                                var_key_map[*it2].insert(sub_name2);
                                start_pos_map[*it].insert(start_pos1);
                                start_pos_map[*it2].insert(start_pos2);

                                if(group_key_map.find(group_name) == group_key_map.end()) {
                                    group_key_map[group_name] = sub_name_group_map[sub_name1];
                                }
                            } else {
                                vector<string> temp_var_name_vec;
                                temp_var_name_vec.push_back(*it);
                                temp_var_name_vec.push_back(*it2);
                                group_name_vec[group_name] = temp_var_name_vec;
                                group_index_set[group_name].insert(sign1);
                                group_index_set[group_name].insert(sign2);
                                var_key_map[*it].insert(sub_name1);
                                var_key_map[*it2].insert(sub_name2);
                                start_pos_map[*it].insert(start_pos1);
                                start_pos_map[*it2].insert(start_pos2);

                                if(group_key_map.find(group_name) == group_key_map.end()) {
                                    group_key_map[group_name] = sub_name_group_map[sub_name1];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for(map<string, vector<string>>::iterator it_var_name = group_name_vec.begin(); it_var_name != group_name_vec.end(); it_var_name++) {
        string group_name_all = it_var_name->first;
        vector<string>::iterator it_kv_begin = it_var_name->second.begin(), it_kve_end = it_var_name->second.end();
        if(group_name_all.size() > 2 && group_name_all[group_name_all.size()-1] == '+' && group_name_all[group_name_all.size()-2] == '+') {
            int map_size = array_lenth_map.size();
            string array_prefix = group_name_all.substr(0, group_name_all.size()-2);
            
            if(array_lenth_map.find(array_prefix) != array_lenth_map.end()) {
                int int_arr_lenth = atoi(array_lenth_map[array_prefix].c_str());
                vector<int> arr_sign_vec;
                for(size_t index=0; index<int_arr_lenth; index++) {
                    arr_sign_vec.push_back(0);
                }
                vector<string>::iterator it_kv_begin = it_var_name->second.begin(), it_kve_end = it_var_name->second.end();
                for(; it_kv_begin!=it_kve_end; it_kv_begin++) {
                    string arr_index = it_kv_begin->substr(group_name_all.size());
                    int index = 0;
                    for(; index!=arr_index.size(); index++) {
                        if(arr_index[index] < '0' || arr_index[index] > '9') {
                            break;
                        }
                    }
                    if(index==arr_index.size() && index != 0) {
                        int int_arr_index = atoi(arr_index.c_str());
                        arr_sign_vec[int_arr_index] = 1;
                    }
                }

                int sign_count = 0;
                int sign_pos = 0;
                for(size_t index=0; index<arr_sign_vec.size(); index++) {
                    if(arr_sign_vec[index]==0) {
                        sign_count += 1;
                        sign_pos = index;
                    }
                }
                if(sign_count==1) {
                    vector<string> pred_string_vec;
                    string key = "";
                    traverse_cfg_preds_to_vector(CFGBlock, pred_string_vec);
                    size_t index;
                    for(index = 0; index < pred_string_vec.size(); index++) {
                        if(reverseVariableStringToNormal(array_prefix) + "++" + to_string(sign_pos) == pred_string_vec[index]) {
                            break;
                        }
                    }
                    if(index==pred_string_vec.size()) {
                        key = "数组" + reverseVariableStringToNormal(array_prefix) + "下标为" + to_string(sign_pos) + "; ";
                    }
                    if(key != "") {
                        key = key.substr(0, key.size() - 2);
                        const CFGElement CBE = *(CFGBlock->begin());
                        if (Optional<CFGStmt> LastStmt = CBE.getAs<CFGStmt>()) {
                            if(left_right_sign)
                                diag(LastStmt->getStmt()->getBeginLoc(),"在本段代码中：" + key + "的变量存在未被使用的情况");
                            else
                                diag(LastStmt->getStmt()->getBeginLoc(),"在本段代码中：" + key + "的变量存在未被赋值的情况");
                        }
                    }
                }
            }
        } else {
            if(group_key_map.find(group_name_all) == group_key_map.end()) {
                continue;
            }
            string group_name = group_key_map[group_name_all];
            if(group_map.find(group_name) == group_map.end()) {
                continue;
            }
            map<string, string> key_value_map = group_map[group_name];
            vector<string> key_value_vec = it_var_name->second;

            vector<string> zero_temp;  

            for(map<string, string>::iterator itk=key_value_map.begin(); itk!=key_value_map.end(); itk++) {
                vector<string>::iterator it_kv = key_value_vec.begin(), it_kve = key_value_vec.end();
                int key_count = 0;
                for(; it_kv!=it_kve; it_kv++) {
                    if(var_key_map[*it_kv].find(itk->second) != var_key_map[*it_kv].end()) {
                        key_count += 1;
                    }
                }

                if(key_count == 0) {
                    zero_temp.push_back(itk->second);
                }
            }

            if(zero_temp.size() == 1) {
                zero_vec.push_back(zero_temp[0]);
                zero_key_vec.push_back(it_var_name->first);
            }
        }

    }

    if(zero_vec.size() != 0) {
        string key = "";
        for(int i = 0; i < zero_vec.size(); i++) {
            string unused_var = getUnsedMember(zero_key_vec[i], group_name_vec[zero_key_vec[i]][0], zero_vec[i]);
            size_t index;
            for(index = 0; index < var_name_vec.size(); index++) {
                if(var_name_vec[index] == unused_var)
                    break;
            }
            if(index != var_name_vec.size()) {
                vector<string> pred_string_vec;
                traverse_cfg_preds_to_vector(CFGBlock, pred_string_vec);
                for(index = 0; index < pred_string_vec.size(); index++) {
                    if(pred_string_vec[index] == unused_var)
                        break;
                }
                if(index == pred_string_vec.size())
                    key += reverseVariableStringToNormal(group_name_vec[zero_key_vec[i]][0]) + "变量所在的组中关键字为" + reverseKeyStringToNormal(zero_vec[i]) + "; ";
            }
        }
        if(key != "") {
            key = key.substr(0, key.size() - 2);
            const CFGElement CBE = *(CFGBlock->begin());
            if (Optional<CFGStmt> LastStmt = CBE.getAs<CFGStmt>()) {
                if(left_right_sign)
                    diag(LastStmt->getStmt()->getBeginLoc(),"在本段代码中：" + key + "等关键字的变量存在未被使用的情况");
                else
                    diag(LastStmt->getStmt()->getBeginLoc(),"在本段代码中：" + key + "等关键字的变量存在未被赋值的情况");
            }
        }
    }
}

string BasicBlockUnusedOrRepeatusedCheck::getUnsedMember(string group_name, string var_name, string unused_key) {
    string prefix = group_name.substr(0, group_name.find_first_of("group_"));
    string suffix = var_name.substr(group_name.find_first_of("group_") + unused_key.length() - 1);
    return prefix + unused_key + suffix;
}

void BasicBlockUnusedOrRepeatusedCheck::TraverseCompoundStmt(const Stmt *S, string var_name, string arr_lenth) {
    if(const CompoundStmt *CS = dyn_cast<CompoundStmt>(S)) {
        using body_iterator = Stmt *;
        body_iterator const * begin = CS->body_begin();
        body_iterator const * end = CS->body_end();
        for(; begin!=end; begin++) {
            if(isa<BinaryOperator>(*begin)) {
                const BinaryOperator *BO = dyn_cast<BinaryOperator>(*begin);
                if(BO->getOpcodeStr() == "=" ||
                    BO->getOpcodeStr() == "+=" ||
                    BO->getOpcodeStr() == "-=" ||
                    BO->getOpcodeStr() == "*=" ||
                    BO->getOpcodeStr() == "/=" ||
                    BO->getOpcodeStr() == "%=") {
                    if(const ArraySubscriptExpr *LASE = dyn_cast<ArraySubscriptExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
                        if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(LASE->getIdx()->IgnoreParenImpCasts())) {
                            string l_var_name = DRE->getDecl()->getDeclName().getAsString();
                            if(l_var_name == var_name) {
                                if(const DeclRefExpr *DRE2 = dyn_cast<DeclRefExpr>(LASE->getBase()->IgnoreCasts()->IgnoreParenImpCasts())) {
                                    if(const ConstantArrayType *CAT= dyn_cast<ConstantArrayType>(DRE2->getDecl()->getType())) {
                                        if(CAT->getSize().toString(10, true) != arr_lenth) {
                                            diag(LASE->getBeginLoc(),"数组长度和循环变量不一致");
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if(const ArraySubscriptExpr *RASE = dyn_cast<ArraySubscriptExpr>(BO->getRHS()->IgnoreParenImpCasts())) {
                        if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(RASE->getIdx()->IgnoreParenImpCasts())) {
                            string r_var_name = DRE->getDecl()->getDeclName().getAsString();
                            if(r_var_name == var_name) {
                                if(const DeclRefExpr *DRE2 = dyn_cast<DeclRefExpr>(RASE->getBase()->IgnoreCasts()->IgnoreParenImpCasts())) {
                                    if(const ConstantArrayType *CAT= dyn_cast<ConstantArrayType>(DRE2->getDecl()->getType())) {
                                        if(CAT->getSize().toString(10, true) != arr_lenth) {
                                            diag(RASE->getBeginLoc(),"数组长度和循环变量不一致");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            } else {}
        }
    } else if(const BinaryOperator *BO = dyn_cast<BinaryOperator>(S)) {
        if(BO->getOpcodeStr() == "=" ||
            BO->getOpcodeStr() == "+=" ||
            BO->getOpcodeStr() == "-=" ||
            BO->getOpcodeStr() == "*=" ||
            BO->getOpcodeStr() == "/=" ||
            BO->getOpcodeStr() == "%=") {
            if(const ArraySubscriptExpr *LASE = dyn_cast<ArraySubscriptExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
                if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(LASE->getIdx()->IgnoreParenImpCasts())) {
                    string l_var_name = DRE->getDecl()->getDeclName().getAsString();
                    if(l_var_name == var_name) {
                        if(const DeclRefExpr *DRE2 = dyn_cast<DeclRefExpr>(LASE->getBase()->IgnoreCasts()->IgnoreParenImpCasts())) {
                            if(const ConstantArrayType *CAT= dyn_cast<ConstantArrayType>(DRE2->getDecl()->getType())) {
                                if(CAT->getSize().toString(10, true) != arr_lenth) {
                                    diag(LASE->getBeginLoc(),"数组长度和循环变量不一致");
                                }
                            }
                        }
                    }
                }
            }
            if(const ArraySubscriptExpr *RASE = dyn_cast<ArraySubscriptExpr>(BO->getRHS()->IgnoreParenImpCasts())) {
                if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(RASE->getIdx()->IgnoreParenImpCasts())) {
                    string r_var_name = DRE->getDecl()->getDeclName().getAsString();
                    if(r_var_name == var_name) {
                        if(const DeclRefExpr *DRE2 = dyn_cast<DeclRefExpr>(RASE->getBase()->IgnoreCasts()->IgnoreParenImpCasts())) {
                            if(const ConstantArrayType *CAT= dyn_cast<ConstantArrayType>(DRE2->getDecl()->getType())) {
                                if(CAT->getSize().toString(10, true) != arr_lenth) {
                                    diag(RASE->getBeginLoc(),"数组长度和循环变量不一致");
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void BasicBlockUnusedOrRepeatusedCheck::check(const MatchFinder::MatchResult &Result) {
    //获取FunctionDecl
    if(const auto *MatchedFunctionDecl = Result.Nodes.getNodeAs<FunctionDecl>("functionDecl"))
    {
        if(isa<CXXConstructorDecl>(MatchedFunctionDecl) || isa<CXXDestructorDecl>(MatchedFunctionDecl)) {
            return;
        }
        if(!MatchedFunctionDecl->hasBody()) {
            return;
        }
        // MatchedFunctionDecl->dump();
        if (MatchedFunctionDecl->isTemplateInstantiation())
            return;
        negative_string_vector.clear();
        default_string_level_map.clear();
        default_string_set.clear();
        custom_string_set.clear();
        custom_string_level_map.clear();
        sub_name_group_map.clear();
        sub_name_level_map.clear();
        group_map.clear();
        default_string_list_sorted_by_lenth.clear();
        custom_string_list_sorted_by_lenth.clear();
        read_negative_string_vector(negative_string_vector);
        read_default_string_level_map(default_string_level_map);
        read_default_string_set(default_string_set);
        read_custom_string_set (custom_string_set);
        read_custom_string_level_map(custom_string_level_map);
        read_subname_map(sub_name_group_map, sub_name_level_map, group_map);
        custom_case_sensitive = read_custom_case_sensitive ();
        transform_default_string_set_to_list(default_string_list_sorted_by_lenth, default_string_set);
        transform_custom_string_set_to_list(custom_string_list_sorted_by_lenth, custom_string_set);
        ASTContext *Context = Result.Context;
        Stmt *FunctionDeclBody = MatchedFunctionDecl->getBody();
        std::unique_ptr<CFG> SourceCFG = CFG::buildCFG(
            MatchedFunctionDecl, FunctionDeclBody, Context, clang::CFG::BuildOptions());
        for (const CFGBlock *CFGBlock :*SourceCFG)
        {
            if(CFGBlock) {
                vector<string> left_var_name_vec;
                vector<string> right_var_name_vec;
                vector<string> zero_vec;
                vector<string> zero_key_vec;
                map<string, vector<string>> group_name_vec;
                map<string, set<int>> group_index_set;
                map<string, set<string>> var_key_map;
                map<string, string> group_key_map;
                traverse_cfg_block_to_vertor(CFGBlock, left_var_name_vec, right_var_name_vec);
                left_right_sign = 0;
                analysis_unuse(left_var_name_vec, zero_vec, zero_key_vec, group_name_vec, group_index_set, var_key_map, group_key_map, CFGBlock);
                zero_vec.clear();
                zero_key_vec.clear();
                group_name_vec.clear();
                group_index_set.clear();
                var_key_map.clear();
                group_key_map.clear();
                left_right_sign = 1;
                analysis_unuse(right_var_name_vec, zero_vec, zero_key_vec, group_name_vec, group_index_set, var_key_map, group_key_map, CFGBlock);
            }
        }
    }

    if(const auto *MatchedForStmt = Result.Nodes.getNodeAs<ForStmt>("forStmt")) {
        string var_name = "";
        string arr_lenth = "";
        if(MatchedForStmt->getCond() == NULL) {
            return;
        } else if(const BinaryOperator *BO = dyn_cast<BinaryOperator>(MatchedForStmt->getCond())) {
            if(BO->getOpcode()==BO_LT) {
                if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
                    var_name = DRE->getDecl()->getDeclName().getAsString();
                } else {
                    return;
                }
                if(const IntegerLiteral *IL = dyn_cast<IntegerLiteral>(BO->getRHS()->IgnoreParenImpCasts())) {
                    arr_lenth = IL->getValue().toString(10, false);
                } else {
                    return;
                }
            } else {
                return;
            }
        }
        if(var_name != "" && arr_lenth != "") {
            TraverseCompoundStmt(MatchedForStmt->getBody(), var_name, arr_lenth);
        }
        
    }
}

} // namespace bz12
} // namespace tidy
} // namespace clang
