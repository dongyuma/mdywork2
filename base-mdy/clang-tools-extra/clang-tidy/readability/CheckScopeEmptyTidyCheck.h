/*!
 * FileName: CheckScopeEmptyTidyCheck.h
 *
 * Author:   ZhangChaoZe
 * Date:     2021-8-12
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 空tidy checker
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_READABILITY_CHECK_SCOPE_EMPTY_TIDY_CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_READABILITY_CHECK_SCOPE_EMPTY_TIDY_CHECK_H
#include "../ClangTidyCheck.h"

namespace clang {
	namespace tidy {
		namespace readability {

			// Detect function declarations that have const value parameters and discourage
			// them.
			class CheckScopeEmptyTidyCheck : public ClangTidyCheck {
			public:
				CheckScopeEmptyTidyCheck(StringRef Name, ClangTidyContext *Context)
					: ClangTidyCheck(Name, Context) {}
			};

		} // namespace readability
	} // namespace tidy
} // namespace clang

#endif
