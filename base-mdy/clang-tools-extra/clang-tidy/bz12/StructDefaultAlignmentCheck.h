//===--- StructDefaultAlignmentCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_STRUCTDEFAULTALIGNMENTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_STRUCTDEFAULTALIGNMENTCHECK_H

#include "../ClangTidyCheck.h"
#include "../ExpandModularHeadersPPCallbacks.h"


namespace clang {
namespace tidy {
namespace bz12 {

/// 检测结构体对齐现象
/// 目前支持char、unsigned char、signed char、int、unsigned int、short、unsigned short、
/// long、unsigned long、float、double、long double等类型的检测

class StructDefaultAlignmentCheck : public ClangTidyCheck {
public:
    StructDefaultAlignmentCheck(StringRef Name, ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context) {
        for (const auto &KeyValue :
            std::vector<std::pair<llvm::StringRef, unsigned>>(
                {{"char", 1},
                {"unsigned char", 1},
                {"signed char", 1},
                {"unsigned short", 2},
                {"short", 2},
                {"unsigned int", 4},
                {"int", 4},
                {"long", 8},
                {"unsigned long", 8},
                {"float", 4},
                {"double", 8},
                {"long double", 16}})) {
            type_lenth_map.insert(KeyValue);
        }
    }
    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
    void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                            Preprocessor *ModuleExpanderPP) override;

private:
    SmallVector<StringRef, 12> type_name_vec = {
        "char", "unsigned char", "signed char", "unsigned short", "short", "unsigned int", \
        "int", "long", "unsigned long", "float", "double", "long double"};
    llvm::StringMap<unsigned>  type_lenth_map;
    SmallVector<StringRef, 1024> field_name_vec;
    SmallVector<FieldDecl *, 1024> field_vec;
    SmallVector<unsigned, 1024> field_count_vec;

    std::map<SourceLocation, int> pack_sourcelocation_alignment_map;

    unsigned field_count = 0;
};

} // namespace bz12
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_STRUCTDEFAULTALIGNMENTCHECK_H
