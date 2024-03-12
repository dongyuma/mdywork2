//===--- StructDefaultAlignmentCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "StructDefaultAlignmentCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/DeclBase.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/LiteralSupport.h"
#include "utils.h"
#include "clang/Lex/DirectoryLookup.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

namespace {
class PragmaPackPPCallbacks : public PPCallbacks {
public:
  PragmaPackPPCallbacks(Preprocessor *PP, StructDefaultAlignmentCheck *Check,  std::map<SourceLocation, int> * pack_sourcelocation_alignment_map)
      : PP(PP), Check(Check), pack_sourcelocation_alignment_map(pack_sourcelocation_alignment_map) {}

    virtual void PragmaDirective(SourceLocation Loc,
                               PragmaIntroducerKind Introducer);

private:

  Preprocessor *PP;//预处理器
  StructDefaultAlignmentCheck *Check;
  std::map<SourceLocation, int> * pack_sourcelocation_alignment_map;
  
};
} // namespace

void PragmaPackPPCallbacks::PragmaDirective(SourceLocation Loc,
                               PragmaIntroducerKind Introducer) {
    
    //仅分析主文件
    if (!PP->isInPrimaryFile()) 
	{
		return;
	}
    // PP->getCodeCompletionLoc().dump(PP->getSourceManager());
    
    // if(PP->getSourceManager().isInSystemHeader(PP->getCodeCompletionLoc()))
    // {
    //     return;
    // }
    //cout << "dir:" << PP->getCurrentFileLexer()->getFileEntry()->getDir()->getName().str() << endl;

    // if(PP->GetCurDirLookup())
    // {
    //     cout << "dir:" << PP->getCurrentFileLexer()->getPP()->GetCurDirLookup()->getName().str();
    //     if(PP->GetCurDirLookup()->isSystemHeaderDirectory())
    //     {
    //         return;
    //     }
    // }
    // else
    // {
    //     cout << "null" << endl;
    //     return;
    // }

    if(Introducer != PIK_HashPragma)
    {
        return;
    }

    Token Tok;
    if(!PP)
    {
        return;
    }
    if(!PP->getCurrentLexer())
    {
        return;
    }
    if(!PP->getCurrentLexer()->getPP())
    {
        return;
    }
    
    PP->getCurrentLexer()->getPP()->LexUnexpandedToken(Tok) ;

    if(!Tok.getIdentifierInfo())
    {
        return;
    }

    if(Tok.getIdentifierInfo()->getName().str() != "pack") {
        return;
    }

    PP->Lex(Tok);
    if (Tok.isNot(tok::l_paren)) {
        return;
    }
    // Sema::PragmaMsStackAction Action = Sema::PSK_Reset;
    StringRef SlotLabel;
    int Alignment = 0;
    PP->Lex(Tok);

    if (Tok.is(tok::numeric_constant)) {
        SmallString<64> IntegerBuffer;
        bool NumberInvalid = false;
        StringRef Spelling = PP->getSpelling(Tok, IntegerBuffer,
                                                    &NumberInvalid);
        if (NumberInvalid) {
            // PP.Diag(AlignLoc, diag::err_pragma_pack_malformed);
            return;
        }
        Alignment = atoi(Spelling.str().c_str());
        PP->Lex(Tok);
    }
    if (Tok.isNot(tok::r_paren)) {
        return;
    }
    SourceLocation RParenLoc = Tok.getLocation();
    // PP->Lex(Tok);
    // if (Tok.isNot(tok::eod)) {
    //     return;
    // }

    (*pack_sourcelocation_alignment_map)[Tok.getLocation()] = Alignment;
}


void StructDefaultAlignmentCheck::registerMatchers(MatchFinder *Finder) {
    // FIXME: Add matchers.
    Finder->addMatcher(recordDecl(unless(isExpansionInSystemHeader())).bind("recordDecl"), this);
}

void StructDefaultAlignmentCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<PragmaPackPPCallbacks>(PP, this, &pack_sourcelocation_alignment_map));
}

void StructDefaultAlignmentCheck::check(const MatchFinder::MatchResult &Result) {
    
    // FIXME: Add callback implementation.
    std::map<SourceLocation, int>::iterator it_begin = pack_sourcelocation_alignment_map.begin();
    std::map<SourceLocation, int>::iterator it_end = pack_sourcelocation_alignment_map.end();
    list<SourceLocation> pack_sourcelocation_ordered_list;
    // for(; it_begin!=it_end; it_begin++) {
    //     it_begin->first.dump(Result.Context->getSourceManager());
    //     cout << it_begin->second << endl;
    // }
    it_begin = pack_sourcelocation_alignment_map.begin();
    for(; it_begin!=it_end; it_begin++) {
        list<SourceLocation>::iterator lit_begin = pack_sourcelocation_ordered_list.begin();
        list<SourceLocation>::iterator lit_end = pack_sourcelocation_ordered_list.end();
        for(; lit_begin!=lit_end; lit_begin++){
            if(it_begin->first <= *lit_begin) {
                pack_sourcelocation_ordered_list.insert(lit_begin, it_begin->first);
                break;
            }
        }
        if(lit_begin==lit_end) {
            pack_sourcelocation_ordered_list.insert(lit_begin, it_begin->first);
        }
    }
    list<SourceLocation>::iterator lit_begin = pack_sourcelocation_ordered_list.begin();
    list<SourceLocation>::iterator lit_end = pack_sourcelocation_ordered_list.end();
    // for(; lit_begin!=lit_end; lit_begin++) {
    //     lit_begin->dump(Result.Context->getSourceManager());
    // }
    unsigned max_alignment;
    unsigned max_mod;
    const auto *recordDecl = Result.Nodes.getNodeAs<RecordDecl>("recordDecl");
    field_name_vec.clear();
    field_vec.clear();
    field_count_vec.clear();
    field_count = 0;
    if (recordDecl->isStruct()) {
        if (recordDecl->isCompleteDefinition()) {
            // recordDecl->dumpColor();
            if(recordDecl->hasAttr<PackedAttr>()) {
                return;
            }
            int default_alignment;
            read_struct_alignment(default_alignment);

            lit_begin = pack_sourcelocation_ordered_list.begin();
            for(; lit_begin!=lit_end; lit_begin++) {
                if(recordDecl->getLocation() < *lit_begin) {
                    break;
                }
            }
            if(lit_begin == pack_sourcelocation_ordered_list.begin()) {

            } else {
                int last_align = pack_sourcelocation_alignment_map[*(--lit_begin)];
                default_alignment = last_align!=0 ? last_align : default_alignment;
            }

            if( recordDecl->getMaxAlignment() != 0) {
                default_alignment = recordDecl->getMaxAlignment() / 8;
            }
            using field_iterator = DeclContext::specific_decl_iterator<FieldDecl>;
            field_iterator begin = recordDecl->field_begin(), end =  recordDecl->field_end();
            // 统计需要记录的内建类型
            for(; begin!=end; begin++) {
                if (begin->getType()->isBuiltinType()) {
                    if(field_count >= 1024) {
                        break;
                    }
                    field_count++;
                    field_vec.push_back(*begin);
                    field_count_vec.push_back(1);
                    if(begin->getType()->isUnsignedChar()) {
                        field_name_vec.push_back("unsigned char");
                    } else if(begin->getType()->isSignedChar()) {
                        field_name_vec.push_back("signed char");
                    } else if(begin->getType()->isCharType()) {
                        field_name_vec.push_back("char");
                    } else if (begin->getType()->isUnsignedShortType()) {
                        field_name_vec.push_back("unsigned short");
                    } else if (begin->getType()->isShortType()) {
                        field_name_vec.push_back("short");
                    } else if (begin->getType()->isUnsignedLongType()) {
                        field_name_vec.push_back("unsigned long");
                    } else if (begin->getType()->isLongType()) {
                        field_name_vec.push_back("long");
                    } else if (begin->getType()->isUnsignedIntType()) {
                        field_name_vec.push_back("unsigned int");
                    } else if (begin->getType()->isIntegerType()) {
                        field_name_vec.push_back("int");
                    } else if (begin->getType()->isLongDoubleType()) {
                        field_name_vec.push_back("long double");
                    } else if (begin->getType()->isDoubleType()) {  //double必须在float之前因为float包含double
                        field_name_vec.push_back("double");
                    } else if (begin->getType()->isFloatOnlyType()) {
                        field_name_vec.push_back("float");
                    } else {
                        field_count--;
                        field_vec.pop_back();
                        field_count_vec.pop_back();
                    }
                } else if(begin->getType()->isConstantArrayType()){
                    /* struct my_struct 
                     * { 
                     *    char a[5];
                     *    char b;
                     *    int c;
                     *    int d; 
                     * }; 
                    */
                    if(field_count >= 1024) {
                        break;
                    }
                    std::string type_name = "";
                    const ConstantArrayType* t_type = (dyn_cast<ConstantArrayType>(begin->getType()));
                    if(t_type->getElementType()->isBuiltinType()) {
                        if(t_type->getElementType()->isUnsignedChar()) {
                            type_name = "unsigned char";
                        } else if(t_type->getElementType()->isSignedChar()) {
                            type_name = "signed char";
                        } else if(t_type->getElementType()->isCharType()) {
                            type_name = "char";
                        } else if (t_type->getElementType()->isUnsignedShortType()) {
                            type_name = "unsigned short";
                        } else if (t_type->getElementType()->isShortType()) {
                            type_name = "short";
                        } else if (t_type->getElementType()->isUnsignedLongType()) {
                            type_name = "unsigned long";
                        } else if (t_type->getElementType()->isLongType()) {
                            type_name = "long";
                        } else if (t_type->getElementType()->isUnsignedIntType()) {
                            type_name = "unsigned int";
                        } else if (t_type->getElementType()->isIntegerType()) {
                            type_name = "int";
                        } else if (t_type->getElementType()->isLongDoubleType()) {
                            type_name = "long double";
                        } else if (t_type->getElementType()->isDoubleType()) {  //double必须在float之前因为float包含double
                            type_name = "double";
                        } else if (t_type->getElementType()->isFloatOnlyType()) {
                            type_name = "float";
                        } else { }
                        if(type_name != "") {
                            unsigned arr_size = *(dyn_cast<ConstantArrayType>(begin->getType())->getSize().getRawData());
                            field_name_vec.push_back(type_name);
                            field_vec.push_back(*begin);
                            field_count_vec.push_back(arr_size);
                            field_count++;
                        }
                    }
                } else if(begin->getType()->isStructureType()) {
                    /* struct my_struct1 
                     * { 
                     *    char struct1_a;
                     *    int struct1_b;
                     * }; 
                     * struct my_struct2
                     * { 
                     *    struct my_struct1 a;
                     *    int struct2_a;
                     *    char struct2_b; 
                     * }; 
                    */
                    RecordDecl *record = nullptr;
                    if(const auto *e_type = dyn_cast<ElaboratedType>(begin->getType()))
                    {
                        if(const auto *r_type = dyn_cast<RecordType>(e_type->desugar()))
                        {
                            record = cast<RecordDecl>(r_type->getDecl());
                        }
                    }
                    else if(const auto *td_type = dyn_cast<TypedefType>(begin->getType()))
                    {
                        if(const auto *e_type = dyn_cast<ElaboratedType>(td_type->desugar()))
                        {
                            if(const auto *r_type = dyn_cast<RecordType>(e_type->desugar()))
                            {
                                record = cast<RecordDecl>(r_type->getDecl());
                            }
                        }
                    }
                    else
                    {
                        continue;
                    }

                    if (record->isCompleteDefinition()) {
                        field_iterator second_begin = record->field_begin(), second_end =  record->field_end();
                        // 统计需要记录的内建类型
                        for(; second_begin!=second_end; second_begin++) {
                            if (second_begin->getType()->isBuiltinType()) {
                                if(field_count >= 1024) {
                                    break;
                                }
                                field_count++;
                                field_vec.push_back(*second_begin);
                                field_count_vec.push_back(1);
                                if(second_begin->getType()->isUnsignedChar()) {
                                    field_name_vec.push_back("unsigned char");
                                } else if(second_begin->getType()->isSignedChar()) {
                                    field_name_vec.push_back("signed char");
                                } else if(second_begin->getType()->isCharType()) {
                                    field_name_vec.push_back("char");
                                } else if (second_begin->getType()->isUnsignedShortType()) {
                                    field_name_vec.push_back("unsigned short");
                                } else if (second_begin->getType()->isShortType()) {
                                    field_name_vec.push_back("short");
                                } else if (second_begin->getType()->isUnsignedLongType()) {
                                    field_name_vec.push_back("unsigned long");
                                } else if (second_begin->getType()->isLongType()) {
                                    field_name_vec.push_back("long");
                                } else if (second_begin->getType()->isUnsignedIntType()) {
                                    field_name_vec.push_back("unsigned int");
                                } else if (second_begin->getType()->isIntegerType()) {
                                    field_name_vec.push_back("int");
                                } else if (second_begin->getType()->isLongDoubleType()) {
                                    field_name_vec.push_back("long double");
                                } else if (second_begin->getType()->isDoubleType()) {  //double必须在float之前因为float包含double
                                    field_name_vec.push_back("double");
                                } else if (second_begin->getType()->isFloatOnlyType()) {
                                    field_name_vec.push_back("float");
                                } else {
                                    field_count--;
                                    field_vec.pop_back();
                                    field_count_vec.pop_back();
                                }
                            } else if(second_begin->getType()->isConstantArrayType()){
                                /* struct my_struct 
                                * { 
                                *    char a[5];
                                *    char b;
                                *    int c;
                                *    int d; 
                                * }; 
                                */
                                if(field_count >= 1024) {
                                    break;
                                }
                                std::string type_name = "";
                                const ConstantArrayType* t_type = (dyn_cast<ConstantArrayType>(second_begin->getType()));
                                if(t_type->getElementType()->isBuiltinType()) {
                                    if(t_type->getElementType()->isUnsignedChar()) {
                                        type_name = "unsigned char";
                                    } else if(t_type->getElementType()->isSignedChar()) {
                                        type_name = "signed char";
                                    } else if(t_type->getElementType()->isCharType()) {
                                        type_name = "char";
                                    } else if (t_type->getElementType()->isUnsignedShortType()) {
                                        type_name = "unsigned short";
                                    } else if (t_type->getElementType()->isShortType()) {
                                        type_name = "short";
                                    } else if (t_type->getElementType()->isUnsignedLongType()) {
                                        type_name = "unsigned long";
                                    } else if (t_type->getElementType()->isLongType()) {
                                        type_name = "long";
                                    } else if (t_type->getElementType()->isUnsignedIntType()) {
                                        type_name = "unsigned int";
                                    } else if (t_type->getElementType()->isIntegerType()) {
                                        type_name = "int";
                                    } else if (t_type->getElementType()->isLongDoubleType()) {
                                        type_name = "long double";
                                    } else if (t_type->getElementType()->isDoubleType()) {  //double必须在float之前因为float包含double
                                        type_name = "double";
                                    } else if (t_type->getElementType()->isFloatOnlyType()) {
                                        type_name = "float";
                                    } else {}
                                    if(type_name != "") {
                                        unsigned arr_size = *(dyn_cast<ConstantArrayType>(second_begin->getType())->getSize().getRawData());
                                        field_name_vec.push_back(type_name);
                                        field_vec.push_back(*second_begin);
                                        field_count_vec.push_back(arr_size);
                                        field_count++;
                                    }
                                }
                            }
                        }
                    }
                } else {}
            }

            // 获取结构体内部定义的最大长度和最大模数（对齐一般以这个数为准，大多数情况下和最大长度）
            max_alignment = 0;
            for(auto iter=field_name_vec.begin(); iter!=field_name_vec.end(); iter++)
            {
                if (type_lenth_map[*iter] > max_alignment) {
                    max_alignment = type_lenth_map[*iter];
                }
            }

            // 找出是否出现结构体对齐现象


            int alignment = max_alignment > default_alignment ? default_alignment : max_alignment;

            unsigned lenth=0, mod=0;
            unsigned count = 0;
            unsigned sign = 0;
            for(auto iter=field_name_vec.begin(); iter!=field_name_vec.end(); iter++)
            {
                if(type_lenth_map[*iter] >= alignment) {
                    mod = 0;
                } else {
                    if (mod!=0) {
                        sign = 1;
                    }
                    if(field_count_vec[count] == 1) {
                        mod += type_lenth_map[*iter];
                    } else if(field_count_vec[count] > 1){
                        unsigned total_mod = field_count_vec[count] * type_lenth_map[*iter];
                        mod += total_mod % alignment;
                    } else {}
                    if(sign == 1 && mod <= alignment) {
                        diag(field_vec[count]->getLocation(), "结构体定义中的 %0 项会出现结构体对齐现象")
                            << field_vec[count];
                    } else {
                        sign = 0;
                        if(field_count_vec[count] == 1) {
                            mod = type_lenth_map[*iter];
                        } else if(field_count_vec[count] > 1){
                            unsigned total_mod = field_count_vec[count] * type_lenth_map[*iter];
                            mod = total_mod % alignment;
                        } else {}
                    }
                }
                count ++;
            }

        } else { }
    }
}

} // namespace bz12
} // namespace tidy
} // namespace clang
