// RUN: %check_clang_tidy %s misrac2012-void-pointer-convert-figure %t

typedef unsigned int uint32_t;
void R_11_6 ( void )
{
   void *p;
   int *p1;
   *p1=5;
   uint32_t u;
   int u1;

   p=(void*)0x1234u;  //违背1
   // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: 类型为空的指针不能与数值类型互相转换 [misrac2012-void-pointer-convert-figure]
   p=(void*)1234;  //违背2
   // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: 类型为空的指针不能与数值类型互相转换 [misrac2012-void-pointer-convert-figure]
   p=(void*)056565;  //违背3
   // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: 类型为空的指针不能与数值类型互相转换 [misrac2012-void-pointer-convert-figure]
   p=(void*)0;  //遵循1
   p=(void*)0x0;  //遵循2
   p=(void*)00;  //遵循3
   //p=(void*)0.1f;  违背4(编译器支持)
   //p=(void*)1024.0f;  违背5(编译器支持)
   u=(uint32_t)p;  //违背6
   // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: 类型为空的指针不能与数值类型互相转换 [misrac2012-void-pointer-convert-figure]
   u1=(int) p;  //违背7
   // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: 类型为空的指针不能与数值类型互相转换 [misrac2012-void-pointer-convert-figure]
}
