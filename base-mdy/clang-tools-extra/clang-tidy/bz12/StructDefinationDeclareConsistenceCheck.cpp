//===--- StructDefinationDeclareConsistenceCheck.cpp - clang-tidy ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "StructDefinationDeclareConsistenceCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

void StructDefinationDeclareConsistenceCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(recordDecl(isStruct(),isDefinition(),unless(isExpansionInSystemHeader()),
                       unless(hasParent(recordDecl(isStruct(),isDefinition())))).bind("RecordDecl"), this);
  Finder->addMatcher(varDecl(unless(isExpansionInSystemHeader()),hasInitializer(expr())).bind("varDecl"), this);
}

void StructDefinationDeclareConsistenceCheck::getStructVec(const RecordDecl *RD, std::vector<std::string> &struct_vec) {
    for(auto *FD : RD->fields()) {
        if(const ElaboratedType *ET = dyn_cast<ElaboratedType>(FD->getType())) {
            if(const RecordDecl *SRD = dyn_cast<RecordDecl>(ET->getAsRecordDecl())) {
                if(SRD->isStruct()) {
                    struct_vec.push_back("{");
                    getStructVec(SRD->getDefinition(), struct_vec);
                    struct_vec.push_back("}");
                }
            }
        } else  if(const ConstantArrayType *CAT = dyn_cast<ConstantArrayType>(FD->getType())) {
            if(CAT->getElementType()->isStructureType()) {
                int arr_size = atoi(CAT->getSize().toString(10, false).c_str());
                struct_vec.push_back("{");
                for(int i=0; i<arr_size; i++) {
                    if(const RecordDecl *SRD = CAT->getElementType()->getAsRecordDecl()->getDefinition()) {
                        struct_vec.push_back("{");
                        getStructVec(SRD, struct_vec);
                        struct_vec.push_back("}");
                    }
                }
                struct_vec.push_back("}");
            } else {
                struct_vec.push_back("array");
            }
        } else {
            struct_vec.push_back("var");
        }
    }
}

void StructDefinationDeclareConsistenceCheck::getInitListVec(const InitListExpr *IL, std::vector<std::string> &init_list_vec) {
    for(auto *el : IL->children()) {
        if(const InitListExpr *IE = dyn_cast<InitListExpr>(el)) {
            init_list_vec.push_back("{");
            getInitListVec(IE, init_list_vec);
            init_list_vec.push_back("}");
        } else if(const ImplicitValueInitExpr *IVIE = dyn_cast<ImplicitValueInitExpr>(el)) {
            ;
        } else {
            init_list_vec.push_back("var");
        }
    }
}

void StructDefinationDeclareConsistenceCheck::getInitListVecFromString(std::string init_str, std::vector<std::string> &init_list_vec) {
    int sign = 0;
    int count = 0;
    int char_sign = 0;
    for(unsigned i=0; i<init_str.size(); i++) {
        if(init_str[i]==' ' || init_str[i]=='\t') {
            ;
        } else if(init_str[i]=='{') {
            if(sign == 0) {
                ;
            } else {
                init_list_vec.push_back("{");
            }
            sign = 1;
            count ++ ;
        } else if(init_str[i] == ',') {
            if(char_sign == 1) {
                init_list_vec.push_back("var");
            }
            char_sign = 0;
        } else if(init_str[i] == '}') {
            if(char_sign == 1)
                init_list_vec.push_back("var");
            init_list_vec.push_back("}");
            char_sign = 0;
            count --;
        } else {
            char_sign = 1;
        }
    }
    if(count == 0) {
        init_list_vec.pop_back();
    }
}

void StructDefinationDeclareConsistenceCheck::check(const MatchFinder::MatchResult &Result) {
    if(const VarDecl *MatchedVarDecl = Result.Nodes.getNodeAs<VarDecl>("varDecl")) {
        if(MatchedVarDecl->getType()->isStructureType()) {
            std::vector<std::string> struct_vec;
            std::vector<std::string> init_list_vec;
            const VarDecl *tempVarDecl = MatchedVarDecl;

            if(const RecordDecl *RD = tempVarDecl->getType()->getAsRecordDecl()->getDefinition()) {
                getStructVec(RD, struct_vec);
            }
            // for(int i=0; i<struct_vec.size(); i++) {
            //     std::cout << struct_vec[i] << ", ";
            // }
            // std::cout << std::endl;

            if(const InitListExpr *init_expr =  dyn_cast<InitListExpr>(MatchedVarDecl->getInit())) {
                const ASTContext &Context = *Result.Context;
                const SourceManager &SM = Context.getSourceManager();
                // getInitListVec(init_expr, init_list_vec);
                const StringRef InitCode = Lexer::getSourceText(
                                                    CharSourceRange::getTokenRange(init_expr->getSourceRange()), 
                                                    SM,Context.getLangOpts());
                //获取初始化内容文本
                std::string InitText = std::string(InitCode);
                getInitListVecFromString(InitText, init_list_vec);
            }

            // for(int i=0; i<init_list_vec.size(); i++) {
            //     std::cout << init_list_vec[i] << ", ";
            // }
            // std::cout << std::endl;
            unsigned i, j;
            if(init_list_vec.size() == 0)
                return;
            for(i=0, j=0; i<struct_vec.size(); i++) {
                if(struct_vec[i] == "array") {
                    if(init_list_vec[j] == "var") {
                        diag(MatchedVarDecl->getInit()->getBeginLoc(),"结构体初始化的嵌套结构必须与定义一致");
                        break;
                    } else {
                        unsigned count = 0;
                        while (true) {
                            if(init_list_vec[j] == "{") {
                                count += 1;
                                j++;
                            } else if (init_list_vec[j] == "}") {
                                count -= 1;
                                j++;
                                if(count == 0) {
                                    break;
                                }
                            } else {
                                j++;
                            }
                            if(j > init_list_vec.size()) {
                                diag(MatchedVarDecl->getInit()->getBeginLoc(),"结构体初始化的嵌套结构必须与定义一致");
                                break;
                            }
                        }
                    }
                } else {
                    if(struct_vec[i] != init_list_vec[j] || ++j > init_list_vec.size()) {
                        diag(MatchedVarDecl->getInit()->getBeginLoc(),"结构体初始化的嵌套结构必须与定义一致");
                        break;
                    }
                }
            }
        }
    }
}

} // namespace bz12
} // namespace tidy
} // namespace clang
