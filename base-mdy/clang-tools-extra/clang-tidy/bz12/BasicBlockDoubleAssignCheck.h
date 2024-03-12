//===--- BasicBlockDoubleAssignCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_BASICBLOCKDOUBLEASSIGNCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_BASICBLOCKDOUBLEASSIGNCHECK_H

#include "../ClangTidyCheck.h"
#include "clang/Analysis/CFG.h"
#include "utils.h"

namespace clang {
namespace tidy {
namespace bz12 {

class BasicBlockDoubleAssignCheck : public ClangTidyCheck {
public:
    BasicBlockDoubleAssignCheck(StringRef Name, ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context) {}
    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
    void onEndOfTranslationUnit() override;
    bool isArrayString(std::string var_name, std::string &arr_prefix, std::string &arr_index);
    void countVarName(std::string var_name, std::vector<std::string> double_assign_arr_negative_vec, std::map<std::string, unsigned> muliple_var_map,
                      std::map<std::string, unsigned> &muliple_var_count_map);

    std::vector<SourceLocation> diag_loc_vec;
    std::vector<SourceRange> for_init_range_vec;
};

} // namespace bz12
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_BASICBLOCKDOUBLEASSIGNCHECK_H
