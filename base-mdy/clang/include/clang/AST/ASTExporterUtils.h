/*!
 * FileName: ASTExporterUtils.h
 *
 * Author:   ZhangChaoZe
 * Date:     2022-9-13
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: AST倒出器相关类型定义
 */
#ifndef LLVM_CLANG_AST_ASTEXPORTERUTILS_H
#define LLVM_CLANG_AST_ASTEXPORTERUTILS_H

#include "llvm/Support/raw_ostream.h"

namespace clang {

	/// Used to specify the format for printing AST Export information.
	enum ASTExporterOutputFormat {
		AEOF_BINIOU,
		AEOF_YOJSON,
		AEOF_JSON
	};
}

#endif