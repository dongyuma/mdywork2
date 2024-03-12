// RUN: %check_clang_tidy %s misrac2012-compatible-declarations %t
#include <stdint.h>

extern void func41(void);
extern void func42(int16_t x, int16_t y);
extern void func43(int16_t x, int16_t y);

extern int16_t count;
int16_t count = 0;      //遵循1

extern uint16_t speed = 6000u; //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: 定义外部链接的全局变量时必须提供兼容性声明 [misrac2012-compatible-declarations]

uint8_t pressure = 101u;       //违背2
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 定义外部链接的全局变量时必须提供兼容性声明 [misrac2012-compatible-declarations]


extern int32_t ext_val1;
int32_t ext_val1 = 3;

int32_t ext_val2;
int32_t ext_val2 = 3;

int32_t ext_val3; //违背3
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 定义外部链接的全局变量时必须提供兼容性声明 [misrac2012-compatible-declarations]

static int32_t static_int1;

void func41(void) //遵循
{
	int aaaa = 11;
	static int ssssss = 10;
}

void func42(int16_t x, int16_t y) //遵循
{
}


void func44(void) //违背4
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: 定义外部链接的函数时必须提供兼容性声明 [misrac2012-compatible-declarations]
{
}

extern int16_t count2;
extern int16_t count2;  //违背5 存在多个声明
// CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 外部链接的全局变量存在多个相同的声明 [misrac2012-compatible-declarations]
int16_t count2 = 0;

void fun46(void);
extern void fun46(void); //违背6 存在多个声明
// CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 外部链接的函数存在多个相同的声明 [misrac2012-compatible-declarations]
void fun46(void)
{

}

static void func45(void) //遵循
{
}

static void R_8_4(void) //遵循
{
}