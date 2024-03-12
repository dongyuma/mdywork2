// RUN: %check_clang_tidy %s gjb8114-use-enum-out-of-bound %t

//CSR-72	禁止非枚举类型变量使用枚举类型的值		4.6.1.13	BinaryOperatorChecker	R-1-6-11	gjb8114-use-enum-out-of-bound
//CSR-72  GJB 8114-2013 R-1-6-11
//违背示例
enum Edata {Enum1 = 0, Enum2, Enum3};
int pfun2(int pb)
{
    return 1;
}
int pfun3(unsigned int pb,unsigned int pb1,int pb3)
{
    return 1;
}
int pfun4(enum Edata k)
{
    return 1;
}
int main_1(void)
{
    unsigned int data;
    data = Enum2;  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 禁止非枚举类型变量使用枚举类型的值 [gjb8114-use-enum-out-of-bound]
    unsigned int data1=1+Enum2+Enum3;  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:26: warning: 禁止非枚举类型变量使用枚举类型的值 [gjb8114-use-enum-out-of-bound]
    // CHECK-MESSAGES: :[[@LINE-2]]:32: warning: 禁止非枚举类型变量使用枚举类型的值 [gjb8114-use-enum-out-of-bound]
    Edata data2=Enum1;  //遵循1
    pfun2(Enum3);  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止非枚举类型变量使用枚举类型的值 [gjb8114-use-enum-out-of-bound]
    pfun2(5);  //遵循2
    pfun2('c');  //遵循3
    pfun2(1.2);  //遵循4
    pfun3(Enum1,Enum2,2);  //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止非枚举类型变量使用枚举类型的值 [gjb8114-use-enum-out-of-bound]
    // CHECK-MESSAGES: :[[@LINE-2]]:17: warning: 禁止非枚举类型变量使用枚举类型的值 [gjb8114-use-enum-out-of-bound]
    pfun4(Enum1); //遵循5
    data1=1+Enum2+Enum3;  //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 禁止非枚举类型变量使用枚举类型的值 [gjb8114-use-enum-out-of-bound]
    // CHECK-MESSAGES: :[[@LINE-2]]:19: warning: 禁止非枚举类型变量使用枚举类型的值 [gjb8114-use-enum-out-of-bound]
    data1=Enum3+1+Enum1+Enum2;  //违背6
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止非枚举类型变量使用枚举类型的值 [gjb8114-use-enum-out-of-bound]
    // CHECK-MESSAGES: :[[@LINE-2]]:19: warning: 禁止非枚举类型变量使用枚举类型的值 [gjb8114-use-enum-out-of-bound]
    // CHECK-MESSAGES: :[[@LINE-3]]:25: warning: 禁止非枚举类型变量使用枚举类型的值 [gjb8114-use-enum-out-of-bound]
    return (0);
}

//遵循示例
int main_2(void)
{
    enum Edata {Enum1=0,Enum2,Enum3} data;
    data=Enum2; //遵循1
    Edata data1;
    data1=Enum3;  //遵循2
    return (0);
}