// RUN: %check_clang_tidy %s gjb8114-sequential-call-same-variable %t

//只检查在一个表达式中同一变量地址或指针在不同函数调用中多次出现的情况,同一变量地址或指针在同一函数调用中出现多次不会报错
//CSR-243     禁止在同一表达式中调用多个顺序相关的函数    R-1-7-14
//CSR-243  GJB 8114-2013 R-1-7-14
//违背示例
unsigned int Vel(unsigned int *pcData);
unsigned int Acc(unsigned int *pcData);
unsigned int fun1(unsigned int *pcData1,unsigned int *pcData2);
unsigned int fun2(int data1);
unsigned int fun3(int data1);
int main(void)
{
    unsigned int dis, hei,tai;
    unsigned int *pointer1=&dis;
    unsigned int *pointer2=&tai;
    dis = 3;
    hei = Vel(&dis) + Acc(&dis);  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止在同一表达式中调用多个顺序相关的函数 [gjb8114-sequential-call-same-variable]
    // CHECK-MESSAGES: :[[@LINE-2]]:23: warning: 禁止在同一表达式中调用多个顺序相关的函数 [gjb8114-sequential-call-same-variable]
    hei=Vel(&dis)+Vel(&dis);  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止在同一表达式中调用多个顺序相关的函数 [gjb8114-sequential-call-same-variable]
    // CHECK-MESSAGES: :[[@LINE-2]]:19: warning: 禁止在同一表达式中调用多个顺序相关的函数 [gjb8114-sequential-call-same-variable]
    hei=fun1(&dis,&dis)+Acc(&tai);  //遵循1
    hei=fun1(&dis,&tai)+Acc(&tai);  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止在同一表达式中调用多个顺序相关的函数 [gjb8114-sequential-call-same-variable]
    // CHECK-MESSAGES: :[[@LINE-2]]:25: warning: 禁止在同一表达式中调用多个顺序相关的函数 [gjb8114-sequential-call-same-variable]
    hei=Vel(&dis)+Acc(&tai);  //遵循2
    hei=fun2(dis)+fun3(dis);  //遵循3
    hei=Vel(&dis)+Vel(&dis)+Acc(&dis);  //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止在同一表达式中调用多个顺序相关的函数 [gjb8114-sequential-call-same-variable]
    // CHECK-MESSAGES: :[[@LINE-2]]:19: warning: 禁止在同一表达式中调用多个顺序相关的函数 [gjb8114-sequential-call-same-variable]
    // CHECK-MESSAGES: :[[@LINE-3]]:29: warning: 禁止在同一表达式中调用多个顺序相关的函数 [gjb8114-sequential-call-same-variable]
    hei=Vel(pointer1)+Acc(pointer1);  //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止在同一表达式中调用多个顺序相关的函数 [gjb8114-sequential-call-same-variable]
    // CHECK-MESSAGES: :[[@LINE-2]]:23: warning: 禁止在同一表达式中调用多个顺序相关的函数 [gjb8114-sequential-call-same-variable]
    hei=Vel(pointer1)+Vel(pointer1)+Acc(pointer1);  //违背6
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止在同一表达式中调用多个顺序相关的函数 [gjb8114-sequential-call-same-variable]
    // CHECK-MESSAGES: :[[@LINE-2]]:23: warning: 禁止在同一表达式中调用多个顺序相关的函数 [gjb8114-sequential-call-same-variable]
    // CHECK-MESSAGES: :[[@LINE-3]]:37: warning: 禁止在同一表达式中调用多个顺序相关的函数 [gjb8114-sequential-call-same-variable]
    hei=fun1(pointer1,pointer1)+Acc(pointer1);  //违背7
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止在同一表达式中调用多个顺序相关的函数 [gjb8114-sequential-call-same-variable]
    // CHECK-MESSAGES: :[[@LINE-2]]:33: warning: 禁止在同一表达式中调用多个顺序相关的函数 [gjb8114-sequential-call-same-variable]
    hei=Vel(pointer1)+Acc(pointer2);  //遵循4
    hei=fun2(hei)+fun3(tai)+fun2(hei);  //遵循5
    return (0);
}
unsigned int Vel(unsigned int *pcData)
{
    unsigned int x = (*pcData);
    (*pcData) = x*x;
    return x;
}
unsigned int Acc(unsigned int *pcData)
{
    unsigned int x = (*pcData);
    (*pcData) = 2*x;
    return x;
}
unsigned int fun1(unsigned int *pcData1,unsigned int *pcData2)
{
    unsigned int x = (*pcData1);
    unsigned int y = (*pcData2);
    (*pcData1) = 2*x+y;
    return x;
}
unsigned int fun2(int data1)
{
    return 1;
}
unsigned int fun3(int data1)
{
    return 1;
}