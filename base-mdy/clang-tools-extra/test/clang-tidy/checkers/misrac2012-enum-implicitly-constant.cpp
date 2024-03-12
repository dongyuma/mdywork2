// RUN: %check_clang_tidy %s misrac2012-enum-implicitly-constant %t
void R_08_12(void)
{
	enum colour { red = 3, blue, green, yellow = 5 }; //违背
	// CHECK-MESSAGES: :[[@LINE-1]]:31: warning: 隐式枚举常量'green'在该枚举类型中不是唯一的 [misrac2012-enum-implicitly-constant]
	enum TestEnum { EN1 = 3, EN2, EN3 = 5, EN4 = 5 }; //遵循
	enum TestEnum2 { EN5 = 3, EN6, EN7, EN8 = 6 }; //遵循
}