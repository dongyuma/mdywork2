//===--- BranchVariableStatisticsCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_BRANCHVARIABLESTATISTICSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_BRANCHVARIABLESTATISTICSCHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace bz12 {

/// 检测在if-else结构和switch-case结构中对变量的使用，如果在一半分支及以上对某一个变量
/// 进行了赋值，可能会在别的分支也会对该变量，本检测器的目的就是对这些分支进行提示。
/// 注意：只有if-else结构的分支数大于等于3个才会检测。

class BranchVariableStatisticsCheck : public ClangTidyCheck {
public:
  BranchVariableStatisticsCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void onEndOfTranslationUnit() override;
  void TraverseStmt(const Stmt *S, unsigned branch_num);
  void CountIfStmt(const IfStmt *IS);
  void AnalysisIfStmt(const IfStmt *IS);
  void TraverseIfStmt(const IfStmt *IS, std::map<std::string, std::vector<unsigned>> *map, unsigned branch_num);
  void TraverseCompoundStmt(const CompoundStmt *CS, std::map<std::string, std::vector<unsigned>> *map, unsigned branch_num);

  std::map<std::string, std::vector<unsigned>> switch_var_name_vector_map;
  std::map<std::string, std::vector<unsigned>> switch_var_name_vector_distinct_map;
  std::map<unsigned, std::set<std::string>> switch_branch_num_var_vector_map;
  
  std::map<const IfStmt *, unsigned> if_stmt_count_map;
  // std::map<std::string, unsigned> if_var_name_map;
};

} // namespace bz12
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_BRANCHVARIABLESTATISTICSCHECK_H
