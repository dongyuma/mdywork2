// RUN: %check_clang_tidy %s misrac2012-convert-between-pointer-and-arithmetic %t

typedef signed   char           int8_t;
typedef signed   short          int16_t;
typedef signed   int            int32_t;
typedef signed   long           int64_t;
typedef          float          float32_t;
typedef          double         float64_t;
typedef          long double    float128_t;
#include<stdio.h>
void fun1()
{
    int16_t p0;
    int16_t * p;
    float32_t f;
    float32_t * f1;
    float128_t f2;
    float128_t * f3;
    bool b1;
    bool *b2;
    //f = ( float32_t ) p;  编译器支持
    //p = ( int16_t * ) f;  编译器支持
    //p1=(int16_t)p;  编译器支持
    //f1=(float32_t*)f;  编译器支持
    //f=(float32_t)f1;  编译器支持
    //p=(int16_t *)f;  编译器支持
    //f3=(float128_t *)f;  编译器支持
    b1=(bool)b2;  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 指向对象的指针和非整型的算术类型不能相互转换 [misrac2012-convert-between-pointer-and-arithmetic]
    b2=(bool *)b1;  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 指向对象的指针和非整型的算术类型不能相互转换 [misrac2012-convert-between-pointer-and-arithmetic]
    p=(int16_t *)b1;  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 指向对象的指针和非整型的算术类型不能相互转换 [misrac2012-convert-between-pointer-and-arithmetic]
    b1=(bool)f3;  //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 指向对象的指针和非整型的算术类型不能相互转换 [misrac2012-convert-between-pointer-and-arithmetic]
    b2=(bool *)p0;  //遵循
    b1=(bool)f1;  //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 指向对象的指针和非整型的算术类型不能相互转换 [misrac2012-convert-between-pointer-and-arithmetic]
}
