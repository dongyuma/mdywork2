// RUN: %check_clang_tidy %s gjb8114-real-parameter-same-with-formal-parameter %t

#include<cstring>
//CSR-246	禁止实参与形参不一致		4.7.1.9	FunctionProcedureChecker	R-1-7-2	gjb8114-Function-Procedure		
//CSR-246 GJB 8114-2013 R-1-7-2
// //违背示例
unsigned int sum(unsigned int p1,unsigned short p2);
short sum_1(int data1,short data2,char data3);
int sum_2(int data1,short data2);
int sum_3();
int main(void)
{
    unsigned int ix,iy,iz;
    int number=4;
    short k=4;
    ix=1;
    iy=2;
    iz=sum(ix,iy);  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 禁止函数的实参和形参类型不一致 [gjb8114-real-parameter-same-with-formal-parameter]
    sum_1(1,(short)2,'c');  //遵循1
    sum_1(1,(short)2,3);  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止函数的实参和形参类型不一致 [gjb8114-real-parameter-same-with-formal-parameter]
    sum_1(1.3,(short)2,(char)5);  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止函数的实参和形参类型不一致 [gjb8114-real-parameter-same-with-formal-parameter]
    sum_1((int)1,(short)2,(char)3);  //遵循2
    sum_2(number,iy);  //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止函数的实参和形参类型不一致 [gjb8114-real-parameter-same-with-formal-parameter]
    sum_2(5,k);  //遵循3
    sum_2(number,(short)number);  //遵循4
    // sum_3(number);
    // 参数个数不一致错误信息会被系统错误覆盖，不报禁止函数的实参和形参类型不一致
    // gjb8114-real-parameter-same-with-formal-parameter.cpp.tmp.cpp:29:5: error: no matching function for call to 'sum_3' [clang-diagnostic-error]
    return (0);
}
unsigned int sum(unsigned int p1,unsigned short p2)
{
    unsigned int ret;
    ret=(unsigned int)(p1+p2);
    return ret;
}
short sum_1(int data1,short data2,char data3)
{
    return 1;
}
int sum_2(int data1,short data2)
{
    return 3;
}
int sum_3()
{
    return 4;
}
int sum_4(void * data1,int data2)
{
    return 5;
}

unsigned int sum2(unsigned int p1,unsigned short p2);
int main2(void)
{
    unsigned int ix,iy,iz;
    ix=1;
    iy=2;
    iz=sum2(ix,iy);  //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 禁止函数的实参和形参类型不一致 [gjb8114-real-parameter-same-with-formal-parameter]
    return (0);
}
unsigned int sum2(unsigned int p1,unsigned short p2)
{
    unsigned int ret;
    ret=(unsigned int)(p1+p2);
    return ret;
}
//遵循示例
unsigned int sum3(unsigned int p1,unsigned short p2);
int main3(void)
{
    unsigned int ix,iy,iz;
    ix=1;
    iy=2;
    iz=sum3(ix,(unsigned short)iy);  
    return (0);
}
unsigned int sum3(unsigned int p1,unsigned short p2)
{
    unsigned int ret;
    ret=(unsigned int)(p1+p2);
    return ret;
}
