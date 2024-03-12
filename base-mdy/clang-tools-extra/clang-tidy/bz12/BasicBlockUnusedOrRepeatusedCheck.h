//===--- BasicBlockUnusedOrRepeatusedCheck.h - clang-tidy -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_BASICBLOCKUNUSEDORREPEATUSEDCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_BASICBLOCKUNUSEDORREPEATUSEDCHECK_H

#include "../ClangTidyCheck.h"
#include "clang/Analysis/CFG.h"

using namespace std;

namespace clang {
namespace tidy {
namespace bz12 {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/bz12-basic-block-unused-or-repeatused.html
class BasicBlockUnusedOrRepeatusedCheck : public ClangTidyCheck {
public:
    BasicBlockUnusedOrRepeatusedCheck(StringRef Name, ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context) {}
    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
    void traverse_cfg_block_to_vertor(const CFGBlock *CFGBlock, vector<string> & left_var_name_vec, vector<string> & right_var_name_vec);
    void traverse_cfg_preds_to_vector(const CFGBlock *CFGBlock, vector<string> & var_name_vec);

    void analysis_unuse(vector<string> & left_var_name_vec, vector<string> & zero_vec, vector<string> & zero_key_vec, map<string, vector<string>> & group_name_vec,
                        map<string, set<int>> & group_index_set,  map<string, set<string>> & var_key_map,  map<string, string> & group_key_map, const CFGBlock *CFGBlock);

    void TraverseCompoundStmt(const Stmt *S, string var_name, string arr_lenth);
    string getUnsedMember(string group_name, string var_name, string unused_key);

    bool custom_case_sensitive;
    vector<string> negative_string_vector;

    set<string> custom_string_set;
    map<string,set<string>> custom_string_level_map;
    list<string> custom_string_list_sorted_by_lenth;

    set<string> default_string_set;
    map<string,set<string>> default_string_level_map;
    list<string> default_string_list_sorted_by_lenth;

    map<string, string> sub_name_group_map;
    map<string, string> sub_name_level_map;
    map<string, map<string, string>> group_map;

    map<string, string> array_lenth_map;

    int left_right_sign = 0;

};

} // namespace bz12
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_BASICBLOCKUNUSEDORREPEATUSEDCHECK_H
