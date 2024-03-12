#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_HTMLGENERATE_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_HTMLGENERATE_H

#include "llvm/ADT/StringRef.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Core/HTMLRewrite.h"
#include "ClangTidyDiagnosticConsumer.h"



namespace clang {

namespace tidy {

class HTMLGenerate{
private:
	SourceLocation Loc;
	StringRef Message;
	ClangTidyContext *Context;

public:
  HTMLGenerate(SourceLocation Loc, StringRef Message, ClangTidyContext *Context);
  bool GenerateHtml();
  StringRef showHelpJavascript();
  StringRef generateKeyboardNavigationJavascript();
  std::string showRelevantLinesJavascript();
  // void dumpCoverageData(const PathDiagnostic &D, const PathPieces &path, llvm::raw_string_ostream &os);
};

} // namespace tidy
} // namespace clang



#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_HTMLGENERATE_H