/*!
 * FileName: KeyAuth.h
 *
 * Author:   ZhangChaoZe
 * Date:     2021-7-26
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: clang clang-tidy key正确性检测
 */


#ifndef _LLVM_CLANG_KEY_AUTH_H_
#define _LLVM_CLANG_KEY_AUTH_H_

#include <string>
#include <vector>

namespace clang
{
	namespace keyauth
	{
		void CheckEnabledCheckerList(const std::vector<std::string> &enabled_list, const std::string &cip_path, const std::string &tool_name);
	}
}


#endif