//===--- StructDefinationDeclareConsistenceCheck.h - clang-tidy -*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_STRUCTDEFINATIONDECLARECONSISTENCECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_STRUCTDEFINATIONDECLARECONSISTENCECHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace bz12 {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/bz12-struct-defination-declare-consistence.html
class StructDefinationDeclareConsistenceCheck : public ClangTidyCheck {
public:
    StructDefinationDeclareConsistenceCheck(StringRef Name, ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context) {}
    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
    void getStructVec(const RecordDecl *RD, std::vector<std::string> &struct_vec);
    void getInitListVec(const InitListExpr *IL, std::vector<std::string> &init_list_vec);
    void getInitListVecFromString(std::string init_str, std::vector<std::string> &init_list_vec);
};

} // namespace bz12
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_STRUCTDEFINATIONDECLARECONSISTENCECHECK_H
