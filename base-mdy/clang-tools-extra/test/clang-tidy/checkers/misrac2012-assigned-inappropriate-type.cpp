// RUN: %check_clang_tidy %smisrac2012-assigned-inappropriate-type %t

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum enum_tag { ENUM_0, ENUM_1, ENUM_2 } enum_t;

enum { K1 = 1, K2 = 128 };

static uint8_t foo1(uint16_t x)
{
	return x;                  //违背0
}

void use_u64_foo(uint64_t x)
{
}


void R_10_3(void)
{
	uint8_t u8b = 10;   //遵循
	uint8_t u8c = 11;   //遵循
	uint8_t u8d = 12;   //遵循
	int8_t s8a;
	const uint8_t *pu8a;
	const uint8_t *pu8b = &u8b; //遵循

	uint16_t u16a = 13; //遵循
	uint32_t u32a = 0, u32b = 1, u32c = 3;
	int32_t s32a = 14; //遵循

	char cha = 'a'; //遵循

	enum_t ena;

	uint8_t u8a = 0;         //遵循            
	bool  flag = (bool)0;    //遵循
	bool  set = true;
	bool  get = (u8b > u8c); //遵循

	ena = ENUM_1;            //遵循
	s8a = K1;            //遵循
	u8a = 2;             //遵循
	u8a = 2 * 24;        //遵循
	cha += 1;            //遵循 (违背) 种情况也应该算为违背 有符号整型和字符类型


	pu8a = pu8b;                  //遵循
	u8a = u8b + u8c + u8d;        //遵循
	u8a = (uint8_t)s8a;           //遵循

	u32a = u16a;                  //遵循
	u32a = 2U + 125U;             //遵循


	uint8_t u8f = 1.0f;           //违背1 - unsigned and floating 
	bool bla = 0;                 //违背2 - boolean and signed 
	cha = 7;                      //违背3 - character and signed 

	u8a = 'a';                    //违背4 - unsigned and character 

	u8b = 1 - 2;                  //违背5 - unsigned and signed 

	u8c += 'a';                   //遵循(违背) 这种情况也应该算为违背 无符号数和字符类型


	s8a = K2;                    //违背6 - Constant value does not fit 

	u16a = u32a;                 //违背7 - uint32_t to uint16_t 

	s8a = -123L;                //违背8 - signed long to int8_t 

	u8a = 6L;                    //违背(遵循) - signed long to uint8_t  这种情况应该为遵循的 并没有超过目标类型的范围
	u8a = (uint16_t)2U + (uint16_t)3U;  //遵循

	u16a = 50000U + 50000U; //违背9 超范围

	u8a = (uint16_t)(2U + 3U); //违背(遵循)  这种情况应该为遵循的 并没有超过目标类型的范围

	(void)foo1(u16a);          //遵循  传参
	foo1(u32a);                //违背10 传参 uint32 -> uint16 

}


void R_10_6(void)
{
	uint8_t u8a = 0;

	uint16_t u16a = 1;
	uint16_t u16b = 2;

	uint32_t u32a = 11;
	uint32_t u32b = 12;
	uint32_t u32c = 13;

	uint64_t u64a;

	u32c = u32a + u32b;              //遵循
	u32a = (uint32_t)u16a + u16b;    //遵循

	u64a = u32a + u32b;            //违背11
	use_u64_foo(u32a + u32b);      //违背12
	use_u64_foo(u32c);             //遵循
	foo1(u8a + u16a);                 //遵循

	u32a = ++u16a;                 //遵循
	use_u64_foo(u16a);             //遵循

	u32a = ~u32a;                   //遵循
	u64a = ~u32a;                   //违背13 非运算未复合表达式 将窄类型赋值给宽类型

	u32a = ~(uint32_t)u16a;         //遵循

	u64a = (uint32_t)(~u32a);        //遵循 强制转换后的复合表达式 再赋值给宽类型是允许的

	u64a = +(u32a * u32a);         //违背14

	u64a = +u32a;                  //遵循 正运算不算是复合表达式
}

void R_10_8(void)
{
	uint32_t u32a = 1;
	uint32_t u32b = 2;
	uint16_t u16a;
	uint16_t u16b = 3;
	int32_t s32a = 4;
	int32_t s32b = 8;

	u16a = (uint16_t)(u32a + u32b); //遵循    无符号32位->无符号16位
	u16a = (uint16_t)(s32a + s32b); //违背15  有符号的复合类型转换为了无符号

	u16a = (uint16_t)s32a;
	u32a = (uint32_t)(u16a + u16b);  //违背16 cast to wider essential type


	u32a = (uint32_t)(uint16_t)(u16a + u16b); //遵循 强制类型转换int32_t不是一个复合类型

}