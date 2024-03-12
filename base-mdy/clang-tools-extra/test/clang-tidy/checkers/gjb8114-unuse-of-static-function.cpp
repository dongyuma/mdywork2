// RUN: %check_clang_tidy %s gjb8114-unuse-of-static-function %t

int fun1(int data1);
double fun2(int data1);
static int fun3(int paData);
static short fun4(void);
static double fun5(double data1);
static long fun6(long data1)  //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 静态函数必须被使用 [gjb8114-unuse-of-static-function]
{
    return data1*2;
}
static int fun7(void);
int main(void)
{
    int i,thData;
    thData=2;
    i=2*thData;
    fun4();
    return (0);
}
int fun1(int data1)  //遵循1
{
    return (1);
}
double fun2(int data1)  //遵循2
{
    return 3.14;
}
static int fun3(int paData)  //违背2
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 静态函数必须被使用 [gjb8114-unuse-of-static-function]
{
    int i;
    i=2*paData;
    return i;
}
static short fun4(void)  //遵循3
{
    return 1;
}
static double fun5(double data1)  //违背3
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 静态函数必须被使用 [gjb8114-unuse-of-static-function]
{
    double returnvalue=data1*2;
    return returnvalue;
}