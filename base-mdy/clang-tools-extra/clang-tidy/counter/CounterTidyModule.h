//===--- CounterTidyModule.h - clang-tidy --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// 统计器相关Checker 通用函数
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_COUNTER_TIDY_MODULE_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_COUNTER_TIDY_MODULE_H

#include "../ClangTidy.h"
#include "../ClangTidyModule.h"

namespace clang {
namespace tidy {
namespace counter {

	//************************************
	// Method:    getAnalyzerResultOutDir
	// FullName:  clang::tidy::counter::getAnalyzerResultOutDir
	// Access:    public 
	// Returns:   std::string
	// Qualifier: 获取分析结果输出目录 不存在时返回空字符串
	//************************************
	std::string getAnalyzerResultOutDir();

	//************************************
	// Method:    getSourceCountInfoFilePath
	// FullName:  clang::tidy::counter::getSourceCountInfoFilePath
	// Access:    public 
	// Returns:   std::string
	// Qualifier: 根据源文件路径获取该源文件统计信息文件路径 不存在时返回空字符串
	// Parameter: const StringRef sourcePath
	//************************************
	std::string getSourceCountInfoFilePath(const StringRef sourcePath);
}
}
}


#endif