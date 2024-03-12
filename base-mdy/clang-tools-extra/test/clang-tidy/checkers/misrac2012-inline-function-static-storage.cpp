// RUN: %check_clang_tidy %s misrac2012-inline-function-static-storage %t
#include <stdint.h>

inline int32_t max(int32_t val1, int32_t val2);  //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 内联函数应该使用 static 进行修饰 [misrac2012-inline-function-static-storage]
inline int32_t max(int32_t val1, int32_t val2)   //违背2
// CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 内联函数应该使用 static 进行修饰 [misrac2012-inline-function-static-storage]
{
	return (val1 > val2) ? val1 : val2;
}

static inline void no_inline_fun(void)           //遵循1
{

}

static void R_8_10(void)
{

}