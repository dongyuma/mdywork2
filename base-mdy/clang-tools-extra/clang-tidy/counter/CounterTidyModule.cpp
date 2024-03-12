//===--- CounterTidyModule.cpp - clang-tidy --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// 统计器相关Checker
//===----------------------------------------------------------------------===//

#include "../ClangTidy.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "llvm/Support/Process.h"
#include "llvm/Support/FileSystem.h"

#include "CounterTidyModule.h"
#include "FunctionInfoCheck.h"


using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace counter {

class CounterModule : public ClangTidyModule 
{
public:
	void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override 
	{
    CheckFactories.registerCheck<FunctionInfoCheck>(
        "counter-function-info");
	}
};

std::string getAnalyzerResultOutDir()
{
	std::string res;
	std::string outPath;
	llvm::Optional<std::string> out_env = llvm::sys::Process::GetEnv("CHECKSCOPE_ANALYZE_RESULT_OUT_DIR");
	if (out_env)
	{
		outPath = out_env.getValue();
	}
	if (!outPath.empty())
	{
		SmallString<128> outDir = StringRef(outPath);
		llvm::sys::path::native(outDir);
		res = outDir.str().str();
	}
	return res;
}


std::string getSourceCountInfoFilePath(const StringRef sourcePath)
{
	std::string res;
	std::string resultOutDirPath = getAnalyzerResultOutDir();
	std::string sourceFilePath = sourcePath.str();
	StringRef resultOutDir = resultOutDirPath;
	
	if (sourceFilePath.size() > 1 && sourceFilePath[0] != '/' && sourceFilePath[1] == ':')
	{
		sourceFilePath.erase(1, 1);
	}
	SmallString<128> pathTmp = StringRef(sourceFilePath);
	llvm::sys::path::native(pathTmp);
	StringRef sourceFile = pathTmp.str();

	if (!resultOutDir.empty() && !sourceFilePath.empty())
	{
		SmallString<128> InfoFile = resultOutDir;
		llvm::sys::path::append(InfoFile, "sc_out", sourceFile);
		if (llvm::sys::fs::exists(InfoFile.str() + ".count"))
		{
			res = InfoFile.str().str() + ".count";
		}
	}
	return res;
}

// Register the CounterTidyModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<CounterModule> X("counter-module", "Adds performance checks.");

}  // namespace counter
volatile int CounterModuleAnchorSource = 0;
}  // namespace tidy
}  // namespace clang
