//===--- BlockConsistencyCheck.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "BlockConsistencyCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "utils.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

void BlockConsistencyCheck::registerMatchers(MatchFinder *Finder) {
    // FIXME: Add matchers.
    Finder->addMatcher(ifStmt(unless(isExpansionInSystemHeader())).bind("matchedIfStmt"), this);
    Finder->addMatcher(switchStmt(unless(isExpansionInSystemHeader())).bind("matchedSwitchStmt"), this);
}

void BlockConsistencyCheck::check(const MatchFinder::MatchResult &Result) {
    mutex_level_map.clear();
    read_mutex_level_map(mutex_level_map);

    if(const auto *matchedIfStmt = Result.Nodes.getNodeAs<IfStmt>("matchedIfStmt")) {
        traverseIfStmt(matchedIfStmt);
    }

    switch_branch_map.clear();
    if(const auto *matchedSwitchStmt = Result.Nodes.getNodeAs<SwitchStmt>("matchedSwitchStmt")) {
        traverseSwitchBody(matchedSwitchStmt->getBody());
        map<const Stmt *, vector<const Stmt*>>::iterator it_begin = switch_branch_map.begin();
        map<const Stmt *, vector<const Stmt*>>::iterator it_end = switch_branch_map.end();
        for(; it_begin!=it_end; it_begin++) {
            analysisSwitchBody(it_begin->first, it_begin->second);
        }
    }
}

void BlockConsistencyCheck::onEndOfTranslationUnit() {
    mutex_level_map.clear();
    read_mutex_level_map(mutex_level_map);
    analysisIfStmt();
}

void BlockConsistencyCheck::traverseSwitchBody(const Stmt * S) {
    if(const CompoundStmt *CPS = dyn_cast<CompoundStmt>(S)) {
        CompoundStmt::const_body_iterator it_begin = CPS->body_begin();
        CompoundStmt::const_body_iterator it_end = CPS->body_end();
        vector<const Stmt*> case_body_vec;
        const Stmt *CaseOrDefaultStmt;
        for(; it_begin!= it_end; it_begin++) {
            if(const CaseStmt *CS = dyn_cast<CaseStmt>(*it_begin)) {
                CaseOrDefaultStmt = CS;
                case_body_vec.push_back(CS->getSubStmt());
                switch_branch_map[CS] = case_body_vec;
            } else if(const DefaultStmt *DS = dyn_cast<DefaultStmt>(*it_begin)) {
                CaseOrDefaultStmt = DS;
                case_body_vec.push_back(DS->getSubStmt());
                switch_branch_map[DS] = case_body_vec;
            } else if(const BreakStmt *BS = dyn_cast<BreakStmt>(*it_begin)) {
                switch_branch_map[CaseOrDefaultStmt].push_back(BS);
                case_body_vec.clear();
            } else if(const Stmt *S = dyn_cast<Stmt>(*it_begin)) {
                switch_branch_map[CaseOrDefaultStmt].push_back(S);
            }
        }
    }
}

void BlockConsistencyCheck::analysisIfStmt() {
    for(size_t i=0; i<if_stmt_vec.size(); i++) {
        if(if_stmt_parent_map.find(if_stmt_vec[i]) == if_stmt_parent_map.end()) {
            const IfStmt * IF = if_stmt_vec[i];
            do {
                analysisBody(IF->getThen());
                if(!IF->hasElseStorage()) {
                    break;
                }
                if(const IfStmt * IF2 =dyn_cast<IfStmt>(IF->getElse())){
                    IF = IF2;
                } else if(IF->hasElseStorage()) {
                    analysisBody(IF->getElse());
                    break;
                } else {
                    break;
                }
            } while(true);
        }
    }
}

void BlockConsistencyCheck::traverseIfStmt(const IfStmt * IF) {
    if_stmt_vec.push_back(IF);
    const IfStmt * TempIF = IF;
    while(true) {
        if(!TempIF->hasElseStorage()) {
            break;
        }
        if(const IfStmt* EIF = dyn_cast<IfStmt>(TempIF->getElse())) {
            if_stmt_parent_map[EIF] = IF;
            TempIF = EIF;
        } else {
            break;
        }
    }
}

void BlockConsistencyCheck::analysisBody(const Stmt* S) {
    if(const CompoundStmt *CPS = dyn_cast<CompoundStmt>(S)) {
        CompoundStmt::const_body_iterator it_begin = CPS->body_begin();
        CompoundStmt::const_body_iterator it_end = CPS->body_end();
        vector<string> vec_str;
        vector<int> vec_sign;
        for(; it_begin!= it_end; it_begin++) {
            string var_name = "";
            string arr_str = "";
            vector<string> vec_str2;
            if(const DeclRefExpr *RDRE = dyn_cast<DeclRefExpr>(*it_begin)){
                ;
            } else if(const ArraySubscriptExpr *ASERE = dyn_cast<ArraySubscriptExpr>(*it_begin)){
                getArraySubscriptExprStringVector(ASERE, vec_str2);
            } else if(const MemberExpr *MERE = dyn_cast<MemberExpr>(*it_begin)){
                getMemberExprVector(MERE, vec_str2);
            } else if(const BinaryOperator * RBO = dyn_cast<BinaryOperator>(*it_begin)){
                getBinaryStringVector(RBO, vec_str2);
            } else if(const CallExpr * CE = dyn_cast<CallExpr>(*it_begin)) {
                getCallExprStringVector(CE, vec_str2, arr_str);
            } else if(const UnaryOperator * UO = dyn_cast<UnaryOperator>(*it_begin)) {
                getUnaryOperatorVector(UO, vec_str2, arr_str);
            } else {
            }
            size_t index, index2, index3;
            for(index2=0; index2<vec_str2.size(); index2++) {
                if(mutex_level_map.find(reverseVariableStringToNormal(vec_str2[index2])) != mutex_level_map.end()) {
                    string level2 = mutex_level_map[reverseVariableStringToNormal(vec_str2[index2])];
                    for(index=0; index<vec_str.size(); index++) {
                        if(mutex_level_map.find(reverseVariableStringToNormal(vec_str[index])) != mutex_level_map.end()) {
                            string level = mutex_level_map[reverseVariableStringToNormal(vec_str[index])];
                            for(index3=0; index3<vec_str.size(); index3++){
                                if(vec_str[index3] == vec_str2[index2])
                                    break;
                            }
                            if(index3==vec_str.size()) {
                                if(level == level2 && vec_str2[index2]!=vec_str[index] && vec_sign[index]==0) {
                                    vec_sign[index] = 1;
                                    vec_str.push_back(vec_str2[index2]);
                                    vec_sign.push_back(1);
                                    diag((*it_begin)->getBeginLoc(), "本语句中的%0与上面的%1是互斥关系")
                                        << reverseVariableStringToNormal(vec_str2[index2]) << reverseVariableStringToNormal(vec_str[index]);
                                }
                            } else {
                                if(level == level2 && vec_str2[index2]!=vec_str[index] && vec_sign[index]==0 && vec_sign[index3]==0) {
                                    vec_sign[index] = 1;
                                    vec_sign[index3] = 1;
                                    diag((*it_begin)->getBeginLoc(), "本语句中的%0与上面的%1是互斥关系")
                                        << reverseVariableStringToNormal(vec_str2[index2]) << reverseVariableStringToNormal(vec_str[index]);
                                }
                            }
                        }
                    }
                }
            }
            for(index2=0; index2<vec_str2.size(); index2++) {
                for(index=0; index<vec_str.size(); index++) {
                    if(vec_str2[index2] == vec_str[index]) {
                        break;
                    }
                }
                if(index == vec_str.size()) {
                    vec_str.push_back(vec_str2[index2]);
                    vec_sign.push_back(0);
                }
            }
        }
    }
}

void BlockConsistencyCheck::analysisSwitchBody(const Stmt * S, vector<const Stmt*> stmt_vec) {
    if(stmt_vec.size()) {
        vector<const Stmt*>::iterator it_begin = stmt_vec.begin();
        vector<const Stmt*>::iterator it_end = stmt_vec.end();
        vector<string> vec_str;
        vector<int> vec_sign;
        for(; it_begin!= it_end; it_begin++) {
            string var_name = "";
            string arr_str = "";
            vector<string> vec_str2;
            if(const DeclRefExpr *RDRE = dyn_cast<DeclRefExpr>(*it_begin)){
                ;
            } else if(const ArraySubscriptExpr *ASERE = dyn_cast<ArraySubscriptExpr>(*it_begin)){
                getArraySubscriptExprStringVector(ASERE, vec_str2);
            } else if(const MemberExpr *MERE = dyn_cast<MemberExpr>(*it_begin)){
                getMemberExprVector(MERE, vec_str2);
            } else if(const BinaryOperator * RBO = dyn_cast<BinaryOperator>(*it_begin)){
                getBinaryStringVector(RBO, vec_str2);
            } else if(const CallExpr * CE = dyn_cast<CallExpr>(*it_begin)) {
                getCallExprStringVector(CE, vec_str2, arr_str);
            } else if(const UnaryOperator * UO = dyn_cast<UnaryOperator>(*it_begin)) {
                getUnaryOperatorVector(UO, vec_str2, arr_str);
            } else {
            }
            size_t index, index2, index3;
            for(index2=0; index2<vec_str2.size(); index2++) {
                if(mutex_level_map.find(reverseVariableStringToNormal(vec_str2[index2])) != mutex_level_map.end()) {
                    string level2 = mutex_level_map[reverseVariableStringToNormal(vec_str2[index2])];
                    for(index=0; index<vec_str.size(); index++) {
                        if(mutex_level_map.find(reverseVariableStringToNormal(vec_str[index])) != mutex_level_map.end()) {
                            string level = mutex_level_map[reverseVariableStringToNormal(vec_str[index])];
                            for(index3=0; index3<vec_str.size(); index3++){
                                if(vec_str[index3] == vec_str2[index2])
                                    break;
                            }
                            if(index3==vec_str.size()) {
                                if(level == level2 && vec_str2[index2]!=vec_str[index] && vec_sign[index]==0) {
                                    vec_sign[index] = 1;
                                    vec_str.push_back(vec_str2[index2]);
                                    vec_sign.push_back(1);
                                    diag((*it_begin)->getBeginLoc(), "本语句中的%0与上面的%1是互斥关系")
                                        << reverseVariableStringToNormal(vec_str2[index2]) << reverseVariableStringToNormal(vec_str[index]);
                                }
                            } else {
                                if(level == level2 && vec_str2[index2]!=vec_str[index] && vec_sign[index]==0 && vec_sign[index3]==0) {
                                    vec_sign[index] = 1;
                                    vec_sign[index3] = 1;
                                    diag((*it_begin)->getBeginLoc(), "本语句中的%0与上面的%1是互斥关系")
                                        << reverseVariableStringToNormal(vec_str2[index2]) << reverseVariableStringToNormal(vec_str[index]);
                                }
                            }
                        }
                    }
                }
            }
            for(index2=0; index2<vec_str2.size(); index2++) {
                for(index=0; index<vec_str.size(); index++) {
                    if(vec_str2[index2] == vec_str[index]) {
                        break;
                    }
                }
                if(index == vec_str.size()) {
                    vec_str.push_back(vec_str2[index2]);
                    vec_sign.push_back(0);
                }
            }
        }
    }
}

} // namespace bz12
} // namespace tidy
} // namespace clang
