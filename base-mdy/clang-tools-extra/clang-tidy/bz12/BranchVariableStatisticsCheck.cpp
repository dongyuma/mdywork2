//===--- BranchVariableStatisticsCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <iostream>
#include "BranchVariableStatisticsCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

void BranchVariableStatisticsCheck::registerMatchers(MatchFinder *Finder) {
    Finder->addMatcher(switchStmt().bind("switchStmt"), this);
    Finder->addMatcher(ifStmt().bind("ifStmt"), this);
}

void BranchVariableStatisticsCheck::TraverseStmt(const Stmt *S, unsigned branch_num) {
    if(isa<BinaryOperator>(*S)) {
        const BinaryOperator *BO = dyn_cast<BinaryOperator>(S);
        if(BO->getOpcodeStr() == "="||
           BO->getOpcodeStr() == "+=" || 
           BO->getOpcodeStr() == "-=" || 
           BO->getOpcodeStr() == "*=" || 
           BO->getOpcodeStr() == "/=" || 
           BO->getOpcodeStr() == "%="  ) {
            if(isa<DeclRefExpr>(BO->getLHS())) {
                const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(BO->getLHS());
                std::string var_name = DRE->getDecl()->getDeclName().getAsString();
                if(switch_var_name_vector_map.find(var_name)!=switch_var_name_vector_map.end()) {
                    switch_var_name_vector_map[var_name].push_back(branch_num);
                } else {
                    std::vector<unsigned> branch_vec;
                    branch_vec.push_back(branch_num);
                    switch_var_name_vector_map[var_name] = branch_vec;
                }
            }
        }
    }
    else if (isa<UnaryOperator>(*S)) {
        const UnaryOperator* UO = dyn_cast<UnaryOperator>(S);
        if (UO->getOpcode() == UnaryOperatorKind::UO_PostInc ||
            UO->getOpcode() == UnaryOperatorKind::UO_PostDec ||
            UO->getOpcode() == UnaryOperatorKind::UO_PreInc ||
            UO->getOpcode() == UnaryOperatorKind::UO_PreDec
            )
        {
            const DeclRefExpr* DRE = dyn_cast<DeclRefExpr>(UO->getSubExpr());
            if (!DRE)
                return;
            std::string var_name = DRE->getDecl()->getDeclName().getAsString();
            if (switch_var_name_vector_map.find(var_name) != switch_var_name_vector_map.end()) {
                switch_var_name_vector_map[var_name].push_back(branch_num);
            }
            else {
                std::vector<unsigned> branch_vec;
                branch_vec.push_back(branch_num);
                switch_var_name_vector_map[var_name] = branch_vec;
            }
        }

       
    } else if (isa<CompoundStmt>(*S)) {
        const CompoundStmt* CPS = dyn_cast<CompoundStmt>(S);
        using body_iterator = Stmt*;
        body_iterator const* begin = CPS->body_begin();
        body_iterator const* end = CPS->body_end();
        for (; begin != end; begin++) {
            TraverseStmt(*begin, branch_num);
        }
    } else {}
}

void BranchVariableStatisticsCheck::CountIfStmt(const IfStmt *IS) {
    if(if_stmt_count_map.find(IS)!=if_stmt_count_map.end()){
        if_stmt_count_map[IS] += 1;
    } else {
        if_stmt_count_map[IS] = 1;
    }
    if(IS->getElse()) {
        if(isa<IfStmt>(IS->getElse())) {
            const IfStmt *ISE = dyn_cast<IfStmt>(IS->getElse());
            CountIfStmt(ISE);
        }
    }
}

void BranchVariableStatisticsCheck::TraverseCompoundStmt(const CompoundStmt *CS, std::map<std::string, std::vector<unsigned>> *map, unsigned branch_num) {
    using body_iterator = Stmt *;
    body_iterator const * begin = CS->body_begin();
    body_iterator const * end = CS->body_end();
    for(; begin!=end; begin++) {
        if(isa<BinaryOperator>(*begin)){
            const BinaryOperator *BO = dyn_cast<BinaryOperator>(*begin);
            if(BO->getOpcodeStr() == "=" ||
                BO->getOpcodeStr() == "+=" ||
                BO->getOpcodeStr() == "-=" ||
                BO->getOpcodeStr() == "*=" ||
                BO->getOpcodeStr() == "/=" ||
                BO->getOpcodeStr() == "%=") {
                if(isa<DeclRefExpr>(BO->getLHS())) {
                    const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(BO->getLHS());
                    std::string var_name = DRE->getDecl()->getDeclName().getAsString();
                    if(map->find(var_name)!=map->end()) {
                        (*map)[var_name].push_back(branch_num);
                    } else {
                        std::vector<unsigned> branch_vec;
                        branch_vec.push_back(branch_num);
                        (*map)[var_name] = branch_vec;
                    }
                }
            }
        }else if (isa<UnaryOperator>(*begin)) {
            const UnaryOperator* UO = dyn_cast<UnaryOperator>(*begin);
            if (UO->getOpcode() == UnaryOperatorKind::UO_PostInc ||
                UO->getOpcode() == UnaryOperatorKind::UO_PostDec ||
                UO->getOpcode() == UnaryOperatorKind::UO_PreInc ||
                UO->getOpcode() == UnaryOperatorKind::UO_PreDec) {
                if (isa<DeclRefExpr>(UO->getSubExpr())) {
                    const DeclRefExpr* DRE = dyn_cast<DeclRefExpr>(UO->getSubExpr());
                    std::string var_name = DRE->getDecl()->getDeclName().getAsString();
                    if (map->find(var_name) != map->end()) {
                        (*map)[var_name].push_back(branch_num);
                    }
                    else {
                        std::vector<unsigned> branch_vec;
                        branch_vec.push_back(branch_num);
                        (*map)[var_name] = branch_vec;
                    }
                }
            }
            
        }else if(isa<CompoundStmt>(*begin)) {
            const CompoundStmt *CPS = dyn_cast<CompoundStmt>(*begin);
            TraverseCompoundStmt(CPS, map, branch_num);
        } else {}
    }
}

void BranchVariableStatisticsCheck::TraverseIfStmt(const IfStmt *IS, std::map<std::string, std::vector<unsigned>> *map, unsigned branch_num) {
    if(isa<CompoundStmt>(IS->getThen())) {
        const CompoundStmt *CPS = dyn_cast<CompoundStmt>(IS->getThen());
        TraverseCompoundStmt(CPS, map, branch_num);
    } else if(isa<BinaryOperator>(IS->getThen())) {
        const BinaryOperator *BO = dyn_cast<BinaryOperator>(IS->getThen());
        if (BO->getOpcodeStr() == "=" ||
            BO->getOpcodeStr() == "+=" ||
            BO->getOpcodeStr() == "-=" ||
            BO->getOpcodeStr() == "*=" ||
            BO->getOpcodeStr() == "/=" ||
            BO->getOpcodeStr() == "%=") {
            if(isa<DeclRefExpr>(BO->getLHS())) {
                const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(BO->getLHS());
                std::string var_name = DRE->getDecl()->getDeclName().getAsString();
                if(map->find(var_name)!=map->end()) {
                    (*map)[var_name].push_back(branch_num);
                } else {
                    std::vector<unsigned> branch_vec;
                    branch_vec.push_back(branch_num);
                    (*map)[var_name] = branch_vec;
                }
            }
        }
    } else if (isa<UnaryOperator>(IS->getThen())) 
    {
        const UnaryOperator* UO = dyn_cast<UnaryOperator>(IS->getThen());
        if (UO->getOpcode() == UnaryOperatorKind::UO_PostInc ||
            UO->getOpcode() == UnaryOperatorKind::UO_PostDec ||
            UO->getOpcode() == UnaryOperatorKind::UO_PreInc ||
            UO->getOpcode() == UnaryOperatorKind::UO_PreDec
            )
        {
            if (isa<DeclRefExpr>(UO->getSubExpr()))
            {
                const DeclRefExpr* DRE = dyn_cast<DeclRefExpr>(UO->getSubExpr());
                std::string var_name = DRE->getDecl()->getDeclName().getAsString();
                if (map->find(var_name) != map->end())
                {
                    (*map)[var_name].push_back(branch_num);
                }
                else
                {
                    std::vector<unsigned> branch_vec;
                    branch_vec.push_back(branch_num);
                    (*map)[var_name] = branch_vec;
                }
            }
        }
     } else {}


    if(IS->getElse()) {
        branch_num += 1;
        if(isa<IfStmt>(IS->getElse())){
            const IfStmt *ISE = dyn_cast<IfStmt>(IS->getElse());
            TraverseIfStmt(ISE, map, branch_num);
        } else if(isa<CompoundStmt>(IS->getElse())) {
            const CompoundStmt *CPS = dyn_cast<CompoundStmt>(IS->getElse());
            TraverseCompoundStmt(CPS, map, branch_num);
        } else if(isa<BinaryOperator>(IS->getThen())){
        const BinaryOperator *BO = dyn_cast<BinaryOperator>(IS->getThen());
            if (BO->getOpcodeStr() == "=" ||
                BO->getOpcodeStr() == "+=" ||
                BO->getOpcodeStr() == "-=" ||
                BO->getOpcodeStr() == "*=" ||
                BO->getOpcodeStr() == "/=" ||
                BO->getOpcodeStr() == "%=") {
                if(isa<DeclRefExpr>(BO->getLHS())) {
                    const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(BO->getLHS());
                    std::string var_name = DRE->getDecl()->getDeclName().getAsString();
                    if(map->find(var_name)!=map->end()) {
                        (*map)[var_name].push_back(branch_num);
                    } else {
                        std::vector<unsigned> branch_vec;
                        branch_vec.push_back(branch_num);
                        (*map)[var_name] = branch_vec;
                    }
                }
            }
        }
        else if (isa<UnaryOperator>(IS->getThen())) {
            const UnaryOperator* UO = dyn_cast<UnaryOperator>(IS->getThen());
            if (UO->getOpcode() == UnaryOperatorKind::UO_PostInc ||
                UO->getOpcode() == UnaryOperatorKind::UO_PostDec ||
                UO->getOpcode() == UnaryOperatorKind::UO_PreInc ||
                UO->getOpcode() == UnaryOperatorKind::UO_PreDec
                )
            {
                if (isa<DeclRefExpr>(UO->getSubExpr())) {
                    const DeclRefExpr* DRE = dyn_cast<DeclRefExpr>(UO->getSubExpr());
                    std::string var_name = DRE->getDecl()->getDeclName().getAsString();
                    if (map->find(var_name) != map->end()) {
                        (*map)[var_name].push_back(branch_num);
                    }
                    else {
                        std::vector<unsigned> branch_vec;
                        branch_vec.push_back(branch_num);
                        (*map)[var_name] = branch_vec;
                    }
                }
            }
        }
        else {}
    }
}

void BranchVariableStatisticsCheck::AnalysisIfStmt(const IfStmt *IS) {
    unsigned branch_num = 0;
    const IfStmt * ISRoot = IS;

    while(IS) {
        branch_num += 1;
        if(IS->getElse()){
            if(isa<IfStmt>(IS->getElse())) {
                IS = dyn_cast<IfStmt>(IS->getElse());
            } else {
                branch_num += 1;
                break;
            }
        } else {
            break;
        }
    }

    // 只处理分支数大于3的情况
    std::map<std::string, std::vector<unsigned>> var_name_count_vec_map;
    std::map<std::string, std::vector<unsigned>> var_name_count_vec_map_gt_half;
    std::set<std::string> var_name_set;
    if(branch_num >=3) {
        IS = ISRoot;
        TraverseIfStmt(IS, &var_name_count_vec_map, 0);
        for(std::map<std::string, std::vector<unsigned>>::iterator it=var_name_count_vec_map.begin(); it!=var_name_count_vec_map.end(); it++){
            if(it->second.size() * 2 >= branch_num){
                var_name_count_vec_map_gt_half[it->first] = it->second;
                var_name_set.insert(it->first);
            }
        }
    }
    // 将变量名-->分支的关系转换为分支-->变量名的关系
    std::map<unsigned, std::set<std::string>> branch_num_var_name_vec_map;
    for(unsigned i=0; i<branch_num; i++){
        branch_num_var_name_vec_map[i] = var_name_set;
    }
    for(std::map<std::string, std::vector<unsigned>>::iterator it=var_name_count_vec_map_gt_half.begin(); it!=var_name_count_vec_map_gt_half.end(); it++) {
        for(std::vector<unsigned>::iterator itv=it->second.begin(); itv!=it->second.end(); itv++) {
            branch_num_var_name_vec_map[*itv].erase(it->first);
        }
    }

    if(branch_num_var_name_vec_map.size() > 0) {
        branch_num = 0;
        IS = ISRoot;
        while(IS) {
            branch_num += 1;
            if(branch_num_var_name_vec_map[branch_num-1].size() > 0) {
                std::string var_name_join = "";
                for(std::set<std::string>::iterator it=branch_num_var_name_vec_map[branch_num-1].begin(); it!=branch_num_var_name_vec_map[branch_num-1].end(); it++) {
                    var_name_join += *it + ", ";
                }
                if(var_name_join != "") {
                    var_name_join = var_name_join.substr(0, var_name_join.size()-2);
                    diag(IS->getBeginLoc(), "If分支可能需要对以下变量进行赋值操作：%0")
                        << var_name_join;
                }
            }
            if(IS->getElse()){
                if(isa<IfStmt>(IS->getElse())) {
                    IS = dyn_cast<IfStmt>(IS->getElse());
                } else {
                    branch_num += 1;
                    std::string var_name_join = "";
                    for(std::set<std::string>::iterator it=branch_num_var_name_vec_map[branch_num-1].begin(); it!=branch_num_var_name_vec_map[branch_num-1].end(); it++) {
                        var_name_join += *it + ", ";
                    }
                    if(var_name_join != "") {
                        var_name_join = var_name_join.substr(0, var_name_join.size()-2);
                        diag(IS->getElse()->getBeginLoc(), "Else分支可能需要对以下变量进行赋值操作：%0")
                            << var_name_join;
                    }
                    break;
                }
            } else {
                break;
            }
        }
    }

}

void BranchVariableStatisticsCheck::check(const MatchFinder::MatchResult &Result) {
    if(const auto *switchStmt = Result.Nodes.getNodeAs<SwitchStmt>("switchStmt")) {
        switch_var_name_vector_map.clear();
        unsigned branch_num = 0;

        if(isa<CompoundStmt>(switchStmt->getBody())) {
            // 遍历统计Switch的内部结构
            const CompoundStmt *CPS = dyn_cast<CompoundStmt>(switchStmt->getBody());
            using body_iterator = Stmt *;
            body_iterator const * begin = CPS->body_begin();
            body_iterator const * end = CPS->body_end();
            for(; begin!=end; begin++) {
                if(isa<CaseStmt>(*begin)) {
                    branch_num++;
                    const CaseStmt *CS = dyn_cast<CaseStmt>(*begin);
                    TraverseStmt(CS->getSubStmt(), branch_num);
                } else if(isa<BreakStmt>(*begin)) {
                    ;
                } else if(isa<DefaultStmt>(*begin)){
                    branch_num++;
                    const DefaultStmt *DS = dyn_cast<DefaultStmt>(*begin);
                    TraverseStmt(DS->getSubStmt(), branch_num);
                } else {
                    TraverseStmt(*begin, branch_num);
                }
            }

            // 统计大于一半分支数量的变量信息
            std::set<std::string> checked_var_name_set; 
            for(std::map<std::string, std::vector<unsigned>>::iterator it=switch_var_name_vector_map.begin(); it!=switch_var_name_vector_map.end(); it++)
            {
                std::vector<unsigned> distinct_vec;
                if(it->second.size() >= 1) {
                    distinct_vec.push_back(it->second[0]);
                    for(unsigned i=1; i<it->second.size(); i++) {
                        if(it->second[i] != it->second[i-1]) {
                            distinct_vec.push_back(it->second[i]);
                        }
                    }
                    if(distinct_vec.size()*2 >= branch_num) {
                        checked_var_name_set.insert(it->first);
                        switch_var_name_vector_distinct_map[it->first] = distinct_vec;
                    }
                }
            }

            // 从统计变量名-->分支，到从分支-->变量名

            if(switch_var_name_vector_distinct_map.size() > 0) {
                for(unsigned i=0; i<branch_num; i++) {
                    switch_branch_num_var_vector_map[i] = checked_var_name_set;
                }
                for(std::map<std::string, std::vector<unsigned>>::iterator it=switch_var_name_vector_distinct_map.begin(); it!=switch_var_name_vector_distinct_map.end(); it++) {
                    for(unsigned i=0; i<it->second.size(); i++) {
                        switch_branch_num_var_vector_map[it->second[i]-1].erase(it->first);
                    }
                }

                begin = CPS->body_begin();
                branch_num = 0;
                for(; begin!=end; begin++) {
                    if(isa<CaseStmt>(*begin)) {
                        const CaseStmt *CS = dyn_cast<CaseStmt>(*begin);
                        if (switch_branch_num_var_vector_map[branch_num].size() > 0) {
                            std::string var_name_join = "";
                            for(std::set<std::string>::iterator it=switch_branch_num_var_vector_map[branch_num].begin(); it!=switch_branch_num_var_vector_map[branch_num].end(); it++) {
                                var_name_join += *it + ", ";
                            }
                            if(var_name_join != "") {
                                var_name_join = var_name_join.substr(0, var_name_join.size()-2);
                                diag(CS->getBeginLoc(), "Switch结构的当前Case分支可能需要对以下变量进行赋值操作：%0")
                                    << var_name_join;
                            }
                        }
                        branch_num++;
                    }  else if(isa<DefaultStmt>(*begin)){
                        const DefaultStmt *DS = dyn_cast<DefaultStmt>(*begin);
                        if (switch_branch_num_var_vector_map[branch_num].size() > 0) {
                            std::string var_name_join = "";
                            for(std::set<std::string>::iterator it=switch_branch_num_var_vector_map[branch_num].begin(); it!=switch_branch_num_var_vector_map[branch_num].end(); it++) {
                                var_name_join += *it + ", ";
                            }
                            if(var_name_join != "") {
                                var_name_join = var_name_join.substr(0, var_name_join.size()-2);
                                diag(DS->getBeginLoc(), "Switch结构的当前Default分支可能需要对以下变量进行赋值操作：%0")
                                    << var_name_join;
                            }                        }
                        branch_num++;
                    } else {}
                }
                
            }
        }

    }

    // 统计IfStmt出现的次数，出现次数为1的表示是根IfStmt
    if(const auto *ifStmt = Result.Nodes.getNodeAs<IfStmt>("ifStmt")) {
        const IfStmt *IS = dyn_cast<IfStmt>(ifStmt);
        CountIfStmt(IS);
    }
}

void BranchVariableStatisticsCheck::onEndOfTranslationUnit() {
    for(std::map<const IfStmt *, unsigned>::iterator it=if_stmt_count_map.begin(); it!=if_stmt_count_map.end(); it++){
        if(it->second == 1){
            AnalysisIfStmt(it->first);
        }
    }
}

} // namespace bz12
} // namespace tidy
} // namespace clang
