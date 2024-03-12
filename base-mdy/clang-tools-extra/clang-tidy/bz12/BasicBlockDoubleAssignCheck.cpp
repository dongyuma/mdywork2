//===--- BasicBlockDoubleAssignCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "BasicBlockDoubleAssignCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

void BasicBlockDoubleAssignCheck::registerMatchers(MatchFinder *Finder) {
    Finder->addMatcher(functionDecl(unless(isExpansionInSystemHeader())).bind("functionDecl"), this);
    Finder->addMatcher(forStmt(unless(isExpansionInSystemHeader())).bind("forStmt"), this);
}

bool BasicBlockDoubleAssignCheck::isArrayString(std::string var_name, std::string &arr_prefix, std::string &arr_index) {
    size_t index;
    bool sign = false;
    for( index = var_name.size(); index > 0; index--) {
        if(isalnum(var_name[index-1])) {
            ;
        } else if(var_name[index-1]=='+') {
            if(index-1 > 0) {
                if(var_name[index-2]=='+') {
                    arr_prefix = var_name.substr(0, index-2);
                    arr_index = var_name.substr(index);
                    sign = true;
                    break;
                } else {
                    sign = false;
                    break;
                }
            } else {
                sign = false;
                break;
            }
        } else {
            sign = false;
            break;
        }
    }
    return sign;
}

void BasicBlockDoubleAssignCheck::countVarName(std::string var_name, vector<std::string> double_assign_arr_negative_vec, 
                                               std::map<std::string, unsigned> muliple_var_map, 
                                               std::map<std::string, unsigned> &muliple_var_count_map) {
    std::string arr_prefix = "";
    std::string arr_index_string = "";
    bool is_array = isArrayString(var_name, arr_prefix, arr_index_string);
    if(is_array) {
        size_t arr_index;
        for(arr_index=0; arr_index<double_assign_arr_negative_vec.size(); arr_index++) {
            if(arr_prefix.find(double_assign_arr_negative_vec[arr_index]) != arr_prefix.npos) {
                break;
            }
        }
        if(arr_index!=double_assign_arr_negative_vec.size()) {
            std::map<std::string, unsigned>::iterator mul_iter_begin = muliple_var_count_map.begin();
            std::map<std::string, unsigned>::iterator mul_iter_end = muliple_var_count_map.end();
            std::string mul_prefix_string;
            std::string mul_arr_index = "";
            for(; mul_iter_begin!=mul_iter_end; mul_iter_begin++) {
                bool mul_is_array = isArrayString(mul_iter_begin->first, mul_prefix_string, mul_arr_index);
                if(mul_is_array) {
                    if(StartWith(mul_iter_begin->first, arr_prefix)) {
                        muliple_var_count_map[mul_iter_begin->first] = 0;
                    }
                }
            }
        } else {
            if(muliple_var_map.find(var_name) != muliple_var_map.end()) {
                if (muliple_var_count_map[var_name] > 0) {
                    muliple_var_count_map[var_name] = muliple_var_count_map[var_name] - 1;
                }
            } else {
                muliple_var_count_map[var_name] = 0;
            }
        }
    } else {
        if(muliple_var_map.find(var_name) != muliple_var_map.end()) {
            if (muliple_var_count_map[var_name] > 0) {
                muliple_var_count_map[var_name] = muliple_var_count_map[var_name] - 1;
            }
        } else {
            muliple_var_count_map[var_name] = 0;
        }
    }
}

void BasicBlockDoubleAssignCheck::check(const MatchFinder::MatchResult &Result) {
    std::vector<std::string> double_assign_arr_negative_vec;
    read_double_assign_negative_vector(double_assign_arr_negative_vec);
    if(const auto *MatchedForStmt = Result.Nodes.getNodeAs<ForStmt>("forStmt")) {
        if(MatchedForStmt->getInit())
        {
            for_init_range_vec.push_back(MatchedForStmt->getInit()->getSourceRange());
        }
    }
    if(const auto *MatchedFunctionDecl = Result.Nodes.getNodeAs<FunctionDecl>("functionDecl"))
    {
        if(isa<CXXConstructorDecl>(MatchedFunctionDecl) || isa<CXXDestructorDecl>(MatchedFunctionDecl)) {
            return;
        }
        if(!MatchedFunctionDecl->hasBody()) {
            return;
        }
        if (MatchedFunctionDecl->isTemplateInstantiation())
            return;
        
        ASTContext *Context = Result.Context;
        Stmt *FunctionDeclBody = MatchedFunctionDecl->getBody();
        std::unique_ptr<CFG> SourceCFG = CFG::buildCFG(
            MatchedFunctionDecl, FunctionDeclBody, Context, clang::CFG::BuildOptions());
        if (!SourceCFG)
            return;
        for (const CFGBlock *CB :*SourceCFG)
        {
            if(CB) {
                std::map<std::string, unsigned> var_map;
                std::map<std::string, unsigned> array_subscript_expr_map;
                std::map<std::string, unsigned> member_expr_map;
                std::map<std::string, std::vector<std::string>> array_index_prefix_map;
                for(CFGBlock::const_iterator IB=CB->begin(); IB!=CB->end(); ++IB) {
                    const CFGElement CBE = *IB;
                    if (Optional<CFGStmt> LastStmt = CBE.getAs<CFGStmt>()) {
                        if (!LastStmt->getStmt())
                            continue;
                        if(const BinaryOperator *BO = dyn_cast<BinaryOperator>(LastStmt->getStmt())) {
                            if(BO->getOpcode() == BO_Assign) {
                                std::string var_name = "";
                                if(!BO->getLHS())
                                    continue;
                                if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
                                    var_name = getDeclRefExprString(DRE);
                                } else if(const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
                                    var_name = getArraySubscriptExprString(ASE);
                                } else if(const MemberExpr *ME = dyn_cast<MemberExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
                                    var_name = getMemberExprString(ME);
                                } else {
                                    var_name = "";
                                }
                                if(var_name != "") {
                                    if(var_map.find(var_name) != var_map.end()) {
                                        var_map[var_name] = var_map[var_name] + 1;
                                    } else {
                                        var_map[var_name] = 1;
                                    }
                                }
                            }
                        } //else if(const DeclStmt *DS = dyn_cast<DeclStmt>(LastStmt->getStmt())) {
                            // 暂时关闭
                            // if(DS-> isSingleDecl()) {
                            //     const VarDecl *VD = dyn_cast<VarDecl>(DS->getSingleDecl());
                            //     std::string var_name = VD->getDeclName().getAsString();
                            //     if(var_name != "" && VD->hasInit()) {
                            //         if(var_map.find(var_name) != var_map.end()) {
                            //             var_map[var_name] = var_map[var_name] + 1;
                            //         } else {
                            //             var_map[var_name] = 1;
                            //         }
                            //     }
                            // }
                        //} else {
                        //}
                    }
                }
                // 遍历变量名出现的次数，大于1次的才会被处理
                std::map<std::string, unsigned> muliple_var_map;
                std::map<std::string, unsigned> muliple_var_count_map;

                for(std::map<std::string, unsigned>::iterator iter=var_map.begin(); iter != var_map.end(); iter++) {
                    if(iter->second > 1) {
                        muliple_var_map[iter->first] = iter->second;
                    }
                }
                // ForStmt

                if(muliple_var_map.size() > 0) {
                    for(CFGBlock::const_iterator MIB=CB->begin(); MIB!=CB->end(); ++MIB) {
                        const CFGElement MCBE = *MIB;
                        if (Optional<CFGStmt> LastStmt = MCBE.getAs<CFGStmt>()) {
                            if (!LastStmt->getStmt())
                                continue;
                            if(const BinaryOperator *BO = dyn_cast<BinaryOperator>(LastStmt->getStmt())) {
                                if(BO->getOpcode() == BO_Assign) {
                                    std::string var_name = "";
                                    std::string var_name2 = "";
                                    std::string arr_str = "";
                                    std::vector<std::string> vec_str;
                                    if(!BO->getRHS())
                                        continue;
                                    Expr *RE = BO->getRHS()->IgnoreCasts()->IgnoreParenImpCasts();
                                    var_name = "";
                                    var_name2 = "";
                                    if (!RE)
                                        continue;
                                    if(const DeclRefExpr *RDRE = dyn_cast<DeclRefExpr>(RE)){
                                        var_name = getDeclRefExprString(RDRE);
                                    } else if(const ArraySubscriptExpr *ASERE = dyn_cast<ArraySubscriptExpr>(RE)){
                                        var_name = getArraySubscriptExprString(ASERE);
                                        getArraySubscriptExprStringVector(ASERE, vec_str);
                                    } else if(const MemberExpr *MERE = dyn_cast<MemberExpr>(RE)){
                                        var_name = getMemberExprString(MERE);
                                    } else if(const BinaryOperator * RBO = dyn_cast<BinaryOperator>(RE)){
                                        getBinaryStringVector(RBO, vec_str);
                                    } else if(const CallExpr * CE = dyn_cast<CallExpr>(RE)) {
                                        getCallExprStringVector(CE, vec_str, arr_str);
                                    } else if(const UnaryOperator *CUO  = dyn_cast<UnaryOperator>(RE)) {
                                        getUnaryOperatorVector(CUO, vec_str, arr_str);
                                    }else {}
                                    if(var_name != "") {
                                        countVarName(var_name, double_assign_arr_negative_vec, muliple_var_map, muliple_var_count_map);
                                    }
                                    for(std::size_t i=0; i<vec_str.size(); i++) {
                                        if(vec_str[i] != "") {
                                            countVarName(vec_str[i], double_assign_arr_negative_vec, muliple_var_map, muliple_var_count_map);
                                        } 
                                    }
                                    // if(arr_str != "") {
                                    //     std::map<std::string, unsigned>::iterator it;
                                    //     for(it=muliple_var_count_map.begin(); it!=muliple_var_count_map.end(); it++) {
                                    //         if(StartWith(it->first, arr_str + "++")){
                                    //             muliple_var_count_map[it->first] = 0;
                                    //         }
                                    //     }
                                    // }
                                    vec_str.clear();
                                    if (!BO->getLHS())
                                        continue;
                                    if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(BO->getLHS())) {
                                        var_name = getDeclRefExprString(DRE);
                                    } else if(const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(BO->getLHS())) {
                                        var_name = getArraySubscriptExprString(ASE);
                                        getArraySubscriptExprStringVector(ASE, vec_str);
                                    } else if(const MemberExpr *ME = dyn_cast<MemberExpr>(BO->getLHS())) {
                                        var_name = getMemberExprString(ME);
                                    }
                                    bool has_unoin = false;
                                    if(var_name != "") {
                                        std::string arr_prefix;
                                        std::string arr_index_string;
                                        bool is_array = isArrayString(var_name, arr_prefix, arr_index_string);
                                        if(is_array) {
                                            size_t ai;
                                            for(ai = 0; ai < arr_index_string.size(); ai++) {
                                                if(isalpha(arr_index_string[ai]) || arr_index_string[ai]=='_') {
                                                    ;
                                                } else {
                                                    break;
                                                }
                                            }
                                            if(ai == arr_index_string.size()) {
                                                if(array_index_prefix_map.find(arr_index_string) != array_index_prefix_map.end()) {
                                                    array_index_prefix_map[arr_index_string].push_back(arr_prefix);
                                                } else {
                                                    std::vector<std::string> prefix_vec;
                                                    prefix_vec.push_back(arr_prefix);
                                                    array_index_prefix_map[arr_index_string] = prefix_vec;
                                                }
                                            }

                                            has_unoin = getExprUnoinSign(BO->getLHS());
                                            if(muliple_var_map.find(var_name) != muliple_var_map.end()) {
                                                muliple_var_count_map[var_name] = muliple_var_count_map[var_name] + 1;
                                                if (muliple_var_count_map[var_name] > 1 && !has_unoin) {
                                                    diag_loc_vec.push_back(BO->getLHS()->getBeginLoc());
                                                }
                                            } else {
                                                muliple_var_count_map[var_name] = 1;
                                            }

                                        } else {
                                            if(array_index_prefix_map.find(var_name) != array_index_prefix_map.end()) {
                                                size_t arr_prefix_index;
                                                for(arr_prefix_index=0; arr_prefix_index < array_index_prefix_map[var_name].size(); arr_prefix_index++) {
                                                    std::map<std::string, unsigned>::iterator mul_iter_begin = muliple_var_count_map.begin();
                                                    std::map<std::string, unsigned>::iterator mul_iter_end = muliple_var_count_map.end();
                                                    std::string mul_prefix_string;
                                                    std::string mul_arr_index = "";
                                                    for(; mul_iter_begin!=mul_iter_end; mul_iter_begin++) {
                                                        bool mul_is_array = isArrayString(mul_iter_begin->first, mul_prefix_string, mul_arr_index);
                                                        if(mul_is_array) {
                                                            if(StartWith(mul_iter_begin->first, array_index_prefix_map[var_name][arr_prefix_index])) {
                                                                muliple_var_count_map[mul_iter_begin->first] = 0;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            has_unoin = getExprUnoinSign(BO->getLHS());
                                            if(muliple_var_map.find(var_name) != muliple_var_map.end()) {
                                                muliple_var_count_map[var_name] = muliple_var_count_map[var_name] + 1;
                                                if (muliple_var_count_map[var_name] > 1 && !has_unoin) {
                                                    diag_loc_vec.push_back(BO->getLHS()->getBeginLoc());
                                                }
                                            } else {
                                                muliple_var_count_map[var_name] = 1;
                                            }
                                        }
                                    }
                                    for(std::size_t i=0; i<vec_str.size(); i++) {
                                        if(vec_str[i] != "") {
                                            countVarName(vec_str[i], double_assign_arr_negative_vec, muliple_var_map, muliple_var_count_map);
                                        }
                                    }
                                }
                            } else if(const DeclStmt *DS = dyn_cast<DeclStmt>(LastStmt->getStmt())) {
                                if(DS-> isSingleDecl()) {
                                    const VarDecl *VD = dyn_cast<VarDecl>(DS->getSingleDecl());
                                    if (!VD)
                                        continue;
                                    std::string var_name = VD->getDeclName().getAsString();
                                    // if(var_name != "" && VD->hasInit()) {
                                    //     if(muliple_var_map.find(var_name) != muliple_var_map.end()) {
                                    //         muliple_var_count_map[var_name] = muliple_var_count_map[var_name] + 1;
                                    //         if (muliple_var_count_map[var_name] > 1) {
                                    //             FullSourceLoc fullSourceLoc = FullSourceLoc(VD->getLocation(), BR.getSourceManager());
                                    //             SourceRange Sr = SourceRange(fullSourceLoc.getSpellingLoc());
                                    //             PathDiagnosticLocation Loc(fullSourceLoc.getSpellingLoc(), BR.getSourceManager());
                                    //             BR.EmitBasicReport(
                                    //                 VD, this, "双重赋值赋值错误",
                                    //                 categories::LogicError, "在同一个块中存在对同一个变量值赋值两次的问题，可能会导致错误",
                                    //                 Loc, Sr);
                                    //         }
                                    //     } else {
                                    //         muliple_var_count_map[var_name] = 1;
                                    //     }
                                    // }
                                    if(VD->hasInit()) {
                                        const Expr *RE = VD->getInit()->IgnoreParenImpCasts();
                                        var_name = "";
                                        std::string var_name2 = "";
                                        std::string arr_str = "";
                                        std::vector<std::string> vec_str;
                                        if(isa<DeclRefExpr>(RE)){
                                            const DeclRefExpr *RDRE = dyn_cast<DeclRefExpr>(RE);
                                            var_name = getDeclRefExprString(RDRE);
                                        } else if(isa<ArraySubscriptExpr>(RE)){
                                            const ArraySubscriptExpr *ASERE = dyn_cast<ArraySubscriptExpr>(RE);
                                            var_name = getArraySubscriptExprString(ASERE);
                                        } else if(isa<MemberExpr>(RE)){
                                            const MemberExpr *MERE = dyn_cast<MemberExpr>(RE);
                                            var_name = getMemberExprString(MERE);
                                        } else if(isa<BinaryOperator>(RE)){
                                            const BinaryOperator * RBO = dyn_cast<BinaryOperator>(RE);
                                            getBinaryStringVector(RBO, vec_str);
                                        } else if(isa<CallExpr>(RE)) {
                                            const CallExpr * CE = dyn_cast<CallExpr>(RE);
                                            getCallExprStringVector(CE, vec_str, arr_str);
                                        } else {}
                                        if(var_name != "") {
                                            countVarName(var_name, double_assign_arr_negative_vec, muliple_var_map, muliple_var_count_map);
                                        }
                                        for(std::size_t i=0; i<vec_str.size(); i++) {
                                            if(vec_str[i] != "") {
                                                countVarName(vec_str[i], double_assign_arr_negative_vec, muliple_var_map, muliple_var_count_map);
                                            } 
                                        }
                                        // if(arr_str != "") {
                                        //     std::map<std::string, unsigned>::iterator it;
                                        //     for(it=muliple_var_count_map.begin(); it!=muliple_var_count_map.end(); it++) {
                                        //         if(StartWith(it->first, arr_str + "++")){
                                        //             muliple_var_count_map[it->first] = 0;
                                        //         }
                                        //     }
                                        // }
                                    }
                                }
                            } else if(const CallExpr *CE = dyn_cast<CallExpr> (LastStmt->getStmt())){
                                unsigned NumArgs = CE->getNumArgs();
                                for (unsigned i = 0; i < NumArgs; i++){
                                    const Expr *RE = CE->getArg(i)->IgnoreParenImpCasts();
                                    std::string var_name = "";
                                    std::string arr_str = "";
                                    std::vector<std::string> vec_str;
                                    if (!RE)
                                        continue;
                                    if(isa<DeclRefExpr>(RE)) {
                                        const DeclRefExpr *RDRE = dyn_cast<DeclRefExpr>(RE);
                                        if (!RDRE)
                                            continue;
                                        if(RDRE->getType()->isArrayType()) {
                                            arr_str = getDeclRefExprString(RDRE);
                                        } else if(RDRE->getType()->isPointerType()) {
                                            arr_str = getDeclRefExprString(RDRE);
                                        }
                                        var_name = getDeclRefExprString(RDRE);
                                    } else if(isa<ArraySubscriptExpr>(RE)){
                                        const ArraySubscriptExpr *ASERE = dyn_cast<ArraySubscriptExpr>(RE);
                                        var_name = getArraySubscriptExprString(ASERE);
                                    } else if(isa<MemberExpr>(RE)){
                                        const MemberExpr *MERE = dyn_cast<MemberExpr>(RE);
                                        var_name = getMemberExprString(MERE);
                                    } else if(isa<BinaryOperator>(RE)){
                                        const BinaryOperator * RBO = dyn_cast<BinaryOperator>(RE);
                                        getBinaryStringVector(RBO, vec_str);
                                    } else if(const UnaryOperator *UO = dyn_cast<UnaryOperator>(RE)) {
                                        getUnaryOperatorVector(UO, vec_str, arr_str);
                                    } else if(isa<CXXConstructExpr>(RE)) {
                                        const CXXConstructExpr * CCERE = dyn_cast<CXXConstructExpr>(RE);
                                        // CCERE;
                                    } else {}
                                    if(var_name != "") {
                                        if(array_index_prefix_map.find(var_name) != array_index_prefix_map.end()) {
                                            size_t arr_prefix_index;
                                            for(arr_prefix_index=0; arr_prefix_index < array_index_prefix_map[var_name].size(); arr_prefix_index++) {
                                                std::map<std::string, unsigned>::iterator mul_iter_begin = muliple_var_count_map.begin();
                                                std::map<std::string, unsigned>::iterator mul_iter_end = muliple_var_count_map.end();
                                                std::string mul_prefix_string;
                                                std::string mul_arr_index = "";
                                                for(; mul_iter_begin!=mul_iter_end; mul_iter_begin++) {
                                                    bool mul_is_array = isArrayString(mul_iter_begin->first, mul_prefix_string, mul_arr_index);
                                                    if(mul_is_array) {
                                                        if(StartWith(mul_iter_begin->first, array_index_prefix_map[var_name][arr_prefix_index])) {
                                                            muliple_var_count_map[mul_iter_begin->first] = 0;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        countVarName(var_name, double_assign_arr_negative_vec, muliple_var_map, muliple_var_count_map);
                                    }
                                    for(std::size_t i=0; i<vec_str.size(); i++) {
                                        if(vec_str[i] != "") {
                                            if(array_index_prefix_map.find(vec_str[i]) != array_index_prefix_map.end()) {
                                                size_t arr_prefix_index;
                                                for(arr_prefix_index=0; arr_prefix_index < array_index_prefix_map[vec_str[i]].size(); arr_prefix_index++) {
                                                    std::map<std::string, unsigned>::iterator mul_iter_begin = muliple_var_count_map.begin();
                                                    std::map<std::string, unsigned>::iterator mul_iter_end = muliple_var_count_map.end();
                                                    std::string mul_prefix_string;
                                                    std::string mul_arr_index = "";
                                                    for(; mul_iter_begin!=mul_iter_end; mul_iter_begin++) {
                                                        bool mul_is_array = isArrayString(mul_iter_begin->first, mul_prefix_string, mul_arr_index);
                                                        if(mul_is_array) {
                                                            if(StartWith(mul_iter_begin->first, array_index_prefix_map[vec_str[i]][arr_prefix_index])) {
                                                                muliple_var_count_map[mul_iter_begin->first] = 0;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            countVarName(vec_str[i], double_assign_arr_negative_vec, muliple_var_map, muliple_var_count_map);
                                        } 
                                    }
                                    // if(arr_str != "") {
                                    //     std::map<std::string, unsigned>::iterator it;
                                    //     for(it=muliple_var_count_map.begin(); it!=muliple_var_count_map.end(); it++) {
                                    //         if(StartWith(it->first, arr_str + "++")){
                                    //             muliple_var_count_map[it->first] = 0;
                                    //         }
                                    //     }
                                    // }
                                }
                            } else if(const UnaryOperator *UO = dyn_cast<UnaryOperator> (LastStmt->getStmt())){
                                std::string arr_str = "";
                                std::vector<std::string> vec_str;
                                getUnaryOperatorVector(UO, vec_str, arr_str);
                                for(std::size_t i=0; i<vec_str.size(); i++) {
                                    if(vec_str[i] != "") {
                                        if(array_index_prefix_map.find(vec_str[i]) != array_index_prefix_map.end()) {
                                            size_t arr_prefix_index;
                                            for(arr_prefix_index=0; arr_prefix_index < array_index_prefix_map[vec_str[i]].size(); arr_prefix_index++) {
                                                std::map<std::string, unsigned>::iterator mul_iter_begin = muliple_var_count_map.begin();
                                                std::map<std::string, unsigned>::iterator mul_iter_end = muliple_var_count_map.end();
                                                std::string mul_prefix_string;
                                                std::string mul_arr_index = "";
                                                for(; mul_iter_begin!=mul_iter_end; mul_iter_begin++) {
                                                    bool mul_is_array = isArrayString(mul_iter_begin->first, mul_prefix_string, mul_arr_index);
                                                    if(mul_is_array) {
                                                        if(StartWith(mul_iter_begin->first, array_index_prefix_map[vec_str[i]][arr_prefix_index])) {
                                                            muliple_var_count_map[mul_iter_begin->first] = 0;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        countVarName(vec_str[i], double_assign_arr_negative_vec, muliple_var_map, muliple_var_count_map);
                                    } 
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void BasicBlockDoubleAssignCheck::onEndOfTranslationUnit() {
    size_t index, index2;
    for(index=0; index<diag_loc_vec.size(); index++) {
        for(index2=0; index2<for_init_range_vec.size(); index2++) {
            if(diag_loc_vec[index] >= for_init_range_vec[index].getBegin() && diag_loc_vec[index] <= for_init_range_vec[index].getEnd()) {
                break;
            }
        }
        if(index2 == for_init_range_vec.size())
            diag(diag_loc_vec[index], "在同一个基本块中存在对同一个变量值赋值两次的问题，可能会导致错误");
    }
}

} // namespace bz12
} // namespace tidy
} // namespace clang
