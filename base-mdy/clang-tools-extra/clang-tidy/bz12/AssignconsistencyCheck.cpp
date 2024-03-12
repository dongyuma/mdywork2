//===--- AssignconsistencyCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AssignconsistencyCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

void AssignconsistencyCheck::registerMatchers(MatchFinder *Finder) {
    Finder->addMatcher(ifStmt().bind("ifStmt"), this);
    Finder->addMatcher(switchStmt().bind("switchStmt"), this);
}

std::string AssignconsistencyCheck::getInterLiteralExprString(const BinaryOperator *BO) const{
    std::string str = "";
    if (BO) {
        const BinaryOperator *temp_bo = BO;
        if (temp_bo->getLHS())
        {
            if (const IntegerLiteral* IL = dyn_cast<IntegerLiteral>(temp_bo->getLHS())) {
                str = "." + IL->getValue().toString(10, true) + str;
            }
        }
        if (temp_bo->getRHS())
        {
            if (const IntegerLiteral* IR = dyn_cast<IntegerLiteral>(temp_bo->getRHS())) {
                str = str + "." + IR->getValue().toString(10, true);
            }
        }
        if (temp_bo->getLHS())
        {
            if (const BinaryOperator* BOL = dyn_cast<BinaryOperator>(temp_bo->getLHS())) {
                str = getInterLiteralExprString(BOL) + str;
            }
        }
        if (temp_bo->getRHS())
        {
            if (const BinaryOperator* BOR = dyn_cast<BinaryOperator>(temp_bo->getRHS())) {
                str = str + getInterLiteralExprString(BOR);
            }
        }
    } 
    return str;
}

std::string AssignconsistencyCheck::getArraySubscriptExprString(
                                    const ArraySubscriptExpr * ASE) const{
    std::string str = "";
    if(ASE) {
        const ArraySubscriptExpr * temp_ase = ASE;
        while (true) {
            if (!temp_ase->getRHS())
                break;
            if(const IntegerLiteral* IL = dyn_cast<IntegerLiteral>(temp_ase->getRHS()->IgnoreParenImpCasts())) {
                str = "." + IL->getValue().toString(10, true) + str;
            } 
            else if (const BinaryOperator *BO = dyn_cast<BinaryOperator>(temp_ase->getRHS()->IgnoreParenImpCasts())) {
                str = getInterLiteralExprString(BO) + str;
            }
            else {
                str = "";
                break;
            }
            if (!temp_ase->getLHS())
                break;
            if(const DeclRefExpr* DRE=dyn_cast<DeclRefExpr>(temp_ase->getLHS()->IgnoreParenImpCasts())) {
                str = DRE->getDecl()->getDeclName().getAsString() + str;
                break;
            } else if(const ArraySubscriptExpr* LASE=dyn_cast<ArraySubscriptExpr>(temp_ase->getLHS()->IgnoreParenImpCasts())) {
                temp_ase = LASE;
            } else if(const MemberExpr* LME=dyn_cast<MemberExpr>(temp_ase->getLHS()->IgnoreParenImpCasts())) {
                str = getMemberExprString(LME) + str;
                break;
            } else {
                break;
            }
        }
    }
    return str;
}

std::string AssignconsistencyCheck::getMemberExprString(
                                        const MemberExpr * ME) const{
    std::string str = "";
    if(ME) {
        const MemberExpr * temp_me = ME;
        while (true) {
            if (!temp_me->getMemberDecl())
                break;
            str = "." + temp_me->getMemberDecl()->getDeclName().getAsString() + str;
            if (!temp_me->getBase())
                break;
            if(const DeclRefExpr* DRE=dyn_cast<DeclRefExpr>(temp_me->getBase()->IgnoreParenImpCasts())) {
                str = DRE->getDecl()->getDeclName().getAsString() + str;
                break;
            } else if(const MemberExpr* BME=dyn_cast<MemberExpr>(temp_me->getBase()->IgnoreParenImpCasts())) {
                temp_me = BME;
            } else if(const ArraySubscriptExpr* BASE=dyn_cast<ArraySubscriptExpr>(temp_me->getBase()->IgnoreParenImpCasts())) {
                str = getArraySubscriptExprString(BASE) + str; 
                break;
            } else {
                break;
            }
        }
    }
    return str;
}

std::string AssignconsistencyCheck::getDeclRefExprString(
                                        const DeclRefExpr * DRE) const{
    std::string str = "";
    if(DRE){
        str =  DRE->getDecl()->getDeclName().getAsString();
    }
    return str;
}

void AssignconsistencyCheck::TraverseStmt(const Stmt *S, SourceLocation SL) {
    if(isa<BinaryOperator>(*S)) {
        const BinaryOperator *BO = dyn_cast<BinaryOperator>(S);
        if(BO->getOpcodeStr() == "="||
           BO->getOpcodeStr() == "+=" || 
           BO->getOpcodeStr() == "-=" || 
           BO->getOpcodeStr() == "*=" || 
           BO->getOpcodeStr() == "/=" || 
           BO->getOpcodeStr() == "%="  ) {
            std::string var_name = "";
            if (BO->getLHS())
                return;
            if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
                var_name = getDeclRefExprString(DRE);
            } 
            else if(const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
                var_name = getArraySubscriptExprString(ASE);
            } else if(const MemberExpr *ME = dyn_cast<MemberExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
                var_name = getMemberExprString(ME);
            }
            if (!var_name.empty()) {
                ArrayName.insert(var_name);
                KeyLocation[var_name] = SL;
            }
        }
    }else if (isa<CompoundStmt>(*S)) {
        const CompoundStmt* CPS = dyn_cast<CompoundStmt>(S);
        using body_iterator = Stmt*;
        body_iterator const * begin = nullptr, *end = nullptr;
        if (CPS->body_begin() != nullptr)
            begin = CPS->body_begin();
        else return;
        if (CPS->body_end() != nullptr)
            end = CPS->body_end();
        else return;
        for (; begin != end; begin++) {
            TraverseStmt(*begin, (*begin)->getBeginLoc());
        }
    } else {}
}

void AssignconsistencyCheck::rTraverseStmt(const Stmt *S, SourceLocation SL) {
    std::string rvar_name = "";
    if (const BinaryOperator *RBO = dyn_cast<BinaryOperator>(S)) {
        if(RBO->getRHS())
        {
            return;
        }
        if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(RBO->getRHS()->IgnoreParenImpCasts())) {
            rvar_name = getDeclRefExprString(DRE);
        } 
        else if(const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(RBO->getRHS()->IgnoreParenImpCasts())) {
            rvar_name = getArraySubscriptExprString(ASE);
        } else if(const MemberExpr *ME = dyn_cast<MemberExpr>(RBO->getRHS()->IgnoreParenImpCasts())) {
            rvar_name = getMemberExprString(ME);
        }
        else if (const BinaryOperator *RRBO = dyn_cast<BinaryOperator>(RBO->getRHS()->IgnoreParenImpCasts())) {
            rvar_name = getRHSName(RRBO);
        }
    }
    if (!rvar_name.empty()) {
        rArrayName.insert(rvar_name);
        rKeyLocation[rvar_name] = SL;
    }

}

void AssignconsistencyCheck::checkAssignconsistency(const CompoundStmt *CPS) {
    using body_iterator = Stmt *;
    body_iterator const * begin = nullptr, *end = nullptr;
    if (CPS->body_begin() != nullptr)
        begin = CPS->body_begin();
    else return;
    if (CPS->body_end() != nullptr)
        end = CPS->body_end();
    else return;
    for (auto iter = ArrayName.begin(); iter != ArrayName.end(); iter++) {
        std::string var_name = *iter;
        int length = var_name.length(), location = 0;
        for (int i = length - 1; i >= 0; i--) {
            if (var_name[i] == '.') {
                location = i;
                break;
            }
        }
        if (location > 0 && location < var_name.size()) {
            var_name = var_name.substr(0, location);
            KeyArrayName.insert(var_name);
        }
    }
    
    std::string var_name1 = "", var_name2 = "";
    int num = KeyArrayName.size(), num1 = 0, num2 = 0;
    
    if (num < 3)return;
    var_name1 = *KeyArrayName.begin();
    
    var_name2 = *KeyArrayName.rbegin();
    for (auto iter = KeyArrayName.begin(); iter != KeyArrayName.end(); iter++) {
        if (*iter == var_name1) {
            num1++;
        }else if (*iter == var_name2) {
            num2++;
        }
        else {
            return;
        }
    }
    int numofdot1 = 0, numofdot2 = 0;
    for (int i = 0; i < var_name1.size(); i++) {
        if (var_name1[i] == '.') numofdot1++;
    }
    for (int i = 0; i < var_name2.size(); i++) {
        if (var_name2[i] == '.') numofdot2++;
    }
    if (numofdot1 != numofdot2) return;
    if (num1 == num-1 && num2 == 1 ) {
        for (auto iter = KeyArrayName.begin(); iter != KeyArrayName.end(); iter++) {
            if (*iter == var_name2) {
                begin = CPS->body_begin();
                end = CPS->body_end();
                for(; begin!=end; begin++) {
                    if(const BinaryOperator *BO = dyn_cast<BinaryOperator>(*begin)) {   
                        if(BO->getOpcodeStr() == "="||
                            BO->getOpcodeStr() == "+=" || 
                            BO->getOpcodeStr() == "-=" || 
                            BO->getOpcodeStr() == "*=" || 
                            BO->getOpcodeStr() == "/=" || 
                            BO->getOpcodeStr() == "%="  ) {
                            std::string var_name = "";
                            if (!BO->getLHS())
                                continue;
                            if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
                                var_name = getDeclRefExprString(DRE);
                                if (var_name.find(var_name2) != std::string::npos && KeyLocation.find(var_name) != KeyLocation.end()) {
                                    diag(DRE->getBeginLoc(),"条件语句块内部赋值语句一致性错误");
                                }
                            } 
                            else if(const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
                                var_name = getArraySubscriptExprString(ASE);
                                if (var_name.find(var_name2) != std::string::npos && KeyLocation.find(var_name) != KeyLocation.end()) {
                                    diag(ASE->getBeginLoc(),"条件语句块内部赋值语句一致性错误");
                                }
                            } else if(const MemberExpr *ME = dyn_cast<MemberExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
                                var_name = getMemberExprString(ME);
                                if (var_name.find(var_name2) != std::string::npos && KeyLocation.find(var_name) != KeyLocation.end()) {
                                    diag(ME->getBeginLoc(),"条件语句块内部赋值语句一致性错误");
                                }
                            }
                        }
                        
                    }
                }
            }
        }
    }
    else if (num1 == 1 && num2 == num - 1) {
        for (auto iter = KeyArrayName.begin(); iter != KeyArrayName.end(); iter++) {
            if (*iter == var_name1) {
                begin = CPS->body_begin();
                end = CPS->body_end();
                for(; begin!=end; begin++) {
                    if(const BinaryOperator *BO = dyn_cast<BinaryOperator>(*begin)) {   
                        if(BO->getOpcodeStr() == "="||
                            BO->getOpcodeStr() == "+=" || 
                            BO->getOpcodeStr() == "-=" || 
                            BO->getOpcodeStr() == "*=" || 
                            BO->getOpcodeStr() == "/=" || 
                            BO->getOpcodeStr() == "%="  ) {
                            std::string var_name = "";
                            if (!BO->getLHS())
                                continue;
                            if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
                                var_name = getDeclRefExprString(DRE);
                                if (var_name.find(var_name1) != std::string::npos && KeyLocation.find(var_name) != KeyLocation.end()) {
                                    diag(DRE->getBeginLoc(),"条件语句块内部赋值语句一致性错误");
                                }
                            } 
                            else if(const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
                                var_name = getArraySubscriptExprString(ASE);
                                if (var_name.find(var_name1) != std::string::npos && KeyLocation.find(var_name) != KeyLocation.end()) {
                                    diag(ASE->getBeginLoc(),"条件语句块内部赋值语句一致性错误");
                                }
                            } else if(const MemberExpr *ME = dyn_cast<MemberExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
                                var_name = getMemberExprString(ME);
                                if (var_name.find(var_name1) != std::string::npos && KeyLocation.find(var_name) != KeyLocation.end()) {
                                    diag(ME->getBeginLoc(),"条件语句块内部赋值语句一致性错误");
                                }
                            }
                        }
                        
                    }
                }
            }
        }
    }

}



void AssignconsistencyCheck::rcheckAssignconsistency(const CompoundStmt *CPS) {
    using body_iterator = Stmt *;
    if (CPS == nullptr) return;
    body_iterator const * begin = nullptr, *end = nullptr;
    if (CPS->body_begin() != nullptr)
        begin = CPS->body_begin();
    else return;
    if (CPS->body_end() != nullptr)
        end = CPS->body_end();
    else return;

    for (auto iter = rArrayName.begin(); iter != rArrayName.end(); iter++) {
        //std::cout << "变量名称为："<< *iter << std::endl;
        std::string rvar_name = *iter;
        int length = rvar_name.length(), rlocation = 0;
        for (int i = length - 1; i >= 0; i--) {
            if (rvar_name[i] == '.') {
                rlocation = i;
                break;
            }
        }
        if (rlocation > 0 && rlocation < rvar_name.size()) {
            rvar_name = rvar_name.substr(0, rlocation);
            rKeyArrayName.insert(rvar_name);
        }
    }
    std::string rvar_name1 = "", rvar_name2 = "";
    int rnum = rKeyArrayName.size(), rnum1 = 0, rnum2 = 0;
    
    if (rnum < 3)return;

    rvar_name1 = *rKeyArrayName.begin();
    
    rvar_name2 = *rKeyArrayName.rbegin();

    for (auto iter = rKeyArrayName.begin(); iter != rKeyArrayName.end(); iter++) {
        if (*iter == rvar_name1) {
            rnum1++;
        }else if (*iter == rvar_name2) {
            rnum2++;
        }
        else {
            return;
        }
    }
    int numofdot1 = 0, numofdot2 = 0;
    for (int i = 0; i < rvar_name1.size(); i++) {
        if (rvar_name1[i] == '.') numofdot1++;
    }
    for (int i = 0; i < rvar_name2.size(); i++) {
        if (rvar_name2[i] == '.') numofdot2++;
    }
    if (numofdot1 != numofdot2) return;
    if (rnum1 == rnum-1 && rnum2 == 1 ) {
        for (auto iter = rKeyArrayName.begin(); iter != rKeyArrayName.end(); iter++) {
            if (*iter == rvar_name2) {
                begin = CPS->body_begin();
                end = CPS->body_end();
                for(; begin!=end; begin++) {
                    const BinaryOperator *BO = nullptr;
                    if (isa<CaseStmt>(*begin)) {
                        const CaseStmt *CS = dyn_cast<CaseStmt>(*begin);
                        if (isa<BinaryOperator>(CS->getSubStmt())) {
                            BO = dyn_cast<BinaryOperator>(CS->getSubStmt());
                        }
                    }
                    if (isa<DefaultStmt>(*begin)) {
                        const DefaultStmt* DS = dyn_cast<DefaultStmt>(*begin);
                        if (isa<BinaryOperator>(DS->getSubStmt())) {
                            BO = dyn_cast<BinaryOperator>(DS->getSubStmt());
                        }
                    }
                    if(isa<BinaryOperator>(*begin)) {   
                        BO = dyn_cast<BinaryOperator>(*begin);
                    } 

                    if(BO != nullptr && (BO->getOpcodeStr() == "="||
                        BO->getOpcodeStr() == "+=" || 
                        BO->getOpcodeStr() == "-=" || 
                        BO->getOpcodeStr() == "*=" || 
                        BO->getOpcodeStr() == "/=" || 
                        BO->getOpcodeStr() == "%=" ) ) {
                        std::string rvar_name = "";
                        if (const BinaryOperator *RBO = dyn_cast<BinaryOperator>(BO->getRHS()->IgnoreParenImpCasts())) {
                            rvar_name =  getRHSName(RBO);
                            if (rvar_name.find(rvar_name2) != std::string::npos && rKeyLocation.find(rvar_name) != rKeyLocation.end()) {
                                diag(BO->getBeginLoc(),"条件语句块内部赋值语句一致性错误");
                                return;
                            }
                        }else if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(BO->getRHS()->IgnoreParenImpCasts())) {
                            rvar_name = getDeclRefExprString(DRE);
                            if (rvar_name.find(rvar_name2) != std::string::npos && rKeyLocation.find(rvar_name) != rKeyLocation.end()) {
                                diag(BO->getBeginLoc(),"条件语句块内部赋值语句一致性错误");
                                return;
                            }
                        } 
                        else if(const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(BO->getRHS()->IgnoreParenImpCasts())) {
                            rvar_name = getArraySubscriptExprString(ASE);
                            if (rvar_name.find(rvar_name2) != std::string::npos && rKeyLocation.find(rvar_name) != rKeyLocation.end()) {
                                diag(BO->getBeginLoc(),"条件语句块内部赋值语句一致性错误");
                                return;
                            }
                        } else if(const MemberExpr *ME = dyn_cast<MemberExpr>(BO->getRHS()->IgnoreParenImpCasts())) {
                            rvar_name = getMemberExprString(ME);
                            if (rvar_name.find(rvar_name2) != std::string::npos && rKeyLocation.find(rvar_name) != rKeyLocation.end()) {
                                diag(BO->getBeginLoc(),"条件语句块内部赋值语句一致性错误");
                                return;
                            }
                        }
                    }
                    
                }
            }
        }
    }
    else if (rnum1 == 1 && rnum2 == rnum - 1) {
        for (auto iter = rKeyArrayName.begin(); iter != rKeyArrayName.end(); iter++) {
            if (*iter == rvar_name1) {
                begin = CPS->body_begin();
                end = CPS->body_end();
                for(; begin!=end; begin++) {
                    const BinaryOperator *BO = nullptr;
                    std::string rvar_name = "";
                    if (isa<CaseStmt>(*begin)) {
                        const CaseStmt *CS = dyn_cast<CaseStmt>(*begin);
                        if (isa<BinaryOperator>(CS->getSubStmt())) {
                            BO = dyn_cast<BinaryOperator>(CS->getSubStmt());
                        }
                    }
                    if (isa<DefaultStmt>(*begin)) {
                        const DefaultStmt* DS = dyn_cast<DefaultStmt>(*begin);
                        if (isa<BinaryOperator>(DS->getSubStmt())) {
                            BO = dyn_cast<BinaryOperator>(DS->getSubStmt());
                        }
                    }
                    if(isa<BinaryOperator>(*begin)) {   
                        BO = dyn_cast<BinaryOperator>(*begin);
                    }

                    if(BO != nullptr && (BO->getOpcodeStr() == "="||
                        BO->getOpcodeStr() == "+=" || 
                        BO->getOpcodeStr() == "-=" || 
                        BO->getOpcodeStr() == "*=" || 
                        BO->getOpcodeStr() == "/=" || 
                        BO->getOpcodeStr() == "%="  )) {
                        if(const MemberExpr *ME = dyn_cast<MemberExpr>(BO->getRHS()->IgnoreParenImpCasts())) {
                            rvar_name = getMemberExprString(ME);
                            if (rvar_name.find(rvar_name1) != std::string::npos && rKeyLocation.find(rvar_name) != rKeyLocation.end()) {
                                diag(BO->getBeginLoc(),"条件语句块内部赋值语句一致性错误");
                                return;
                            }
                        }else if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(BO->getRHS()->IgnoreParenImpCasts())) {
                            rvar_name = getDeclRefExprString(DRE);
                            if (rvar_name.find(rvar_name1) != std::string::npos && rKeyLocation.find(rvar_name) != rKeyLocation.end()) {
                                diag(BO->getBeginLoc(),"条件语句块内部赋值语句一致性错误");
                                return;
                            }
                        }else if(const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(BO->getRHS()->IgnoreParenImpCasts())) {
                            rvar_name = getArraySubscriptExprString(ASE);
                            if (rvar_name.find(rvar_name1) != std::string::npos && rKeyLocation.find(rvar_name) != rKeyLocation.end()) {
                                diag(BO->getBeginLoc(),"条件语句块内部赋值语一致性错误");
                                return;
                            }
                        }
                        else if (const BinaryOperator *RBO = dyn_cast<BinaryOperator>(BO->getRHS()->IgnoreParenImpCasts())) {
                            rvar_name =  getRHSName(RBO);
                            if (rvar_name.find(rvar_name1) != std::string::npos && rKeyLocation.find(rvar_name) != rKeyLocation.end()) {
                                diag(BO->getBeginLoc(),"条件语句块内部赋值语句一致性错误");
                                return;
                            }
                        }
                    }           
                    
                }
            }
        }
    }
}

std::string AssignconsistencyCheck::getRHSName(const BinaryOperator *BOR) {
    std::string rvar_name = "";
    if (const BinaryOperator *BO = dyn_cast<BinaryOperator>(BOR->getLHS()->IgnoreParenImpCasts())) {
        rvar_name =  getRHSName(BO);
    }else if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(BOR->getLHS()->IgnoreParenImpCasts())) {
        rvar_name = getDeclRefExprString(DRE);
    } 
    else if(const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(BOR->getLHS()->IgnoreParenImpCasts())) {
        rvar_name = getArraySubscriptExprString(ASE);
    } else if(const MemberExpr *ME = dyn_cast<MemberExpr>(BOR->getLHS()->IgnoreParenImpCasts())) {
        rvar_name = getMemberExprString(ME);
    }
    return rvar_name;
}
void AssignconsistencyCheck::check(const MatchFinder::MatchResult &Result) {

    if(const auto *switchStmt = Result.Nodes.getNodeAs<SwitchStmt>("switchStmt")) {
        if (!ArrayName.empty())ArrayName.clear();
        if (!KeyArrayName.empty())KeyArrayName.clear();
        if (!KeyLocation.empty()) KeyLocation.clear();
        if (!rArrayName.empty())rArrayName.clear();
        if (!rKeyArrayName.empty())rKeyArrayName.clear();
        if (!rKeyLocation.empty()) rKeyLocation.clear();
        const CompoundStmt *CPS = nullptr;
        if (!switchStmt->getBody())
            return;
        if(isa<CompoundStmt>(switchStmt->getBody())) {
            // 遍历统计Switch的内部结构
            CPS = dyn_cast<CompoundStmt>(switchStmt->getBody());
            using body_iterator = Stmt *;
            body_iterator const * begin = nullptr, *end = nullptr;
            if (!CPS)
                return;
            if (CPS->body_begin() != nullptr)
                begin = CPS->body_begin();
            else return;
            if (CPS->body_end() != nullptr)
                end = CPS->body_end();
            else return;
            if (begin == nullptr || end == nullptr) return;
            for(; begin!=end; begin++) {                
                if(isa<CaseStmt>(*begin)) {
                    checkAssignconsistency(CPS);
                    rcheckAssignconsistency(CPS);
                    if (!ArrayName.empty())ArrayName.clear();
                    if (!KeyArrayName.empty())KeyArrayName.clear();
                    if (!KeyLocation.empty()) KeyLocation.clear();
                    if (!rArrayName.empty())rArrayName.clear();
                    if (!rKeyArrayName.empty())rKeyArrayName.clear();
                    if (!rKeyLocation.empty()) rKeyLocation.clear();
                    const CaseStmt *CS = dyn_cast<CaseStmt>(*begin);
                    if (!CS)
                        continue;
                    TraverseStmt(CS->getSubStmt(), CS->getBeginLoc());
                    rTraverseStmt(CS->getSubStmt(), CS->getBeginLoc());
                } else if(isa<DefaultStmt>(*begin)){
                    checkAssignconsistency(CPS);
                    rcheckAssignconsistency(CPS);
                    if (!ArrayName.empty())ArrayName.clear();
                    if (!KeyArrayName.empty())KeyArrayName.clear();
                    if (!KeyLocation.empty()) KeyLocation.clear();
                    if (!rArrayName.empty())rArrayName.clear();
                    if (!rKeyArrayName.empty())rKeyArrayName.clear();
                    if (!rKeyLocation.empty()) rKeyLocation.clear();
                    const CaseStmt *CS = dyn_cast<CaseStmt>(*begin);
                    const DefaultStmt *DS = dyn_cast<DefaultStmt>(*begin);
                    if (!DS)
                        continue;
                    TraverseStmt(DS->getSubStmt(), DS->getBeginLoc());
                    rTraverseStmt(DS->getSubStmt(), DS->getBeginLoc());
                } else {
                    TraverseStmt(*begin, (*begin)->getBeginLoc());
                    rTraverseStmt(*begin, (*begin)->getBeginLoc());
                }
                
            }
           
        } 
        
    }

    if(const auto *ifStmt = Result.Nodes.getNodeAs<IfStmt>("ifStmt")) {
        if (!ArrayName.empty())ArrayName.clear();
        if (!KeyArrayName.empty())KeyArrayName.clear();
        if (!rArrayName.empty())rArrayName.clear();
        if (!rKeyArrayName.empty())rKeyArrayName.clear();
        const CompoundStmt *CPS = nullptr;
        if (!ifStmt->getThen())
        {
            return;
        }
        if(isa<CompoundStmt>(ifStmt->getThen())) {
            CPS = dyn_cast<CompoundStmt>(ifStmt->getThen());
        }else {
            return;
        }
         
        using body_iterator = Stmt *;
        body_iterator const * begin = nullptr, *end = nullptr;
        if (CPS->body_begin() != nullptr)
            begin = CPS->body_begin();
        else return;
        if (CPS->body_end() != nullptr)
            end = CPS->body_end();
        else return;
        if (begin == nullptr || end == nullptr) return;
        for(; begin!=end; begin++) {
            if(const BinaryOperator *BO = dyn_cast<BinaryOperator>(*begin)) {   
                if(BO->getOpcodeStr() == "="||
                    BO->getOpcodeStr() == "+=" || 
                    BO->getOpcodeStr() == "-=" || 
                    BO->getOpcodeStr() == "*=" || 
                    BO->getOpcodeStr() == "/=" || 
                    BO->getOpcodeStr() == "%="  ) {
                    std::string var_name = "";
                    if (BO->getLHS())
                        continue;
                    if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
                        var_name = getDeclRefExprString(DRE);
                    } 
                    else if(const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
                        var_name = getArraySubscriptExprString(ASE);
                    } else if(const MemberExpr *ME = dyn_cast<MemberExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
                        var_name = getMemberExprString(ME);
                    }
                    if (!var_name.empty()) {
                        ArrayName.insert(var_name);
                        KeyLocation[var_name] = (*begin)->getBeginLoc();
                    }
                    std::string rvar_name = "";
                    if (BO->getRHS())
                        continue;
                    if (const BinaryOperator *BOR = dyn_cast<BinaryOperator>(BO->getRHS()->IgnoreParenImpCasts())) {
                        rvar_name =  getRHSName(BOR);
                    }else if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(BO->getRHS()->IgnoreParenImpCasts())) {
                        rvar_name = getDeclRefExprString(DRE);
                    } else if(const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(BO->getRHS()->IgnoreParenImpCasts())) {
                        rvar_name = getArraySubscriptExprString(ASE);
                    } else if(const MemberExpr *ME = dyn_cast<MemberExpr>(BO->getRHS()->IgnoreParenImpCasts())) {
                        rvar_name = getMemberExprString(ME);
                    }
                    if (!rvar_name.empty()) {
                        rArrayName.insert(rvar_name);
                        rKeyLocation[rvar_name] = BO->getBeginLoc();
                    }
                }
                
            }
        }

        checkAssignconsistency(CPS);
        rcheckAssignconsistency(CPS);
    }
}

} // namespace bz12
} // namespace tidy
} // namespace clang
