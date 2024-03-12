// RUN: %check_clang_tidy %s misrac2012-function-parameters-name %t
#include <stdint.h>

extern int16_t func1(int16_t n);   //遵循1
extern void func2(int16_t);        //违背1  参数未命名
// CHECK-MESSAGES: :[[@LINE-1]]:19: warning: 函数参数应带有名称 [misrac2012-function-parameters-name]

static int16_t func3();            //违背2  未包含参数类型原型
// CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 函数声明应为带有参数类型的原型格式: func3(void) [misrac2012-function-parameters-name]

extern int16_t func4(void);        //遵循2


int16_t func1(int16_t n)           //遵循3
{
	return 52;
}



typedef int16_t(*pf2_t)(int16_t);   //违背3  函数指针参数未命名(未支持)
typedef int16_t(*pf3_t)(int16_t n); //遵循4

static int16_t get_pf(int16_t n)
{
	return n;
}

void R_8_2(void)
{
	int16_t(*pf0) ();                    //违背4  函数指针未包含参数类型原型(未支持)
	int16_t(*pf0v) (void);               //遵循5
}

typedef uint16_t width_t;
typedef uint16_t height_t;
typedef uint32_t area_t;

extern void f3(signed int x);
void f3(int x);  //遵循6


//extern void g3(const  int *p1);  //编译器不允许
void g3(int *p1); //违背5 声明实现类型不一致
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: 函数声明应使用和定义相同的参数名称和参数类型 [misrac2012-function-parameters-name]


extern int16_t func(int16_t num, int16_t den); //违背6 参数名称不一致
// CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 函数声明应使用和定义相同的参数名称和参数类型 [misrac2012-function-parameters-name]

int16_t func(int16_t den, int16_t num)
{
	return num / den;
}

extern area_t area(width_t w, height_t h); //违背7 声明实现类型不一致
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 函数声明应使用和定义相同的参数名称和参数类型 [misrac2012-function-parameters-name]

area_t area(width_t w, width_t h)
{
	return (area_t)w * h;
}

void f3(int x) //遵循7
{

}

void g3(int *p2)
{

}