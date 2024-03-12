// RUN: %check_clang_tidy %s gjb8114-bit-define %t

//CSR-65	有符号类型的位长度必须大于等于两位		4.6.1.6	BinaryOperatorChecker	R-1-1-10	gb8114-BitDefine		
//CSR-65  GJB 8114-2013 R-1-1-10
//违背示例
// 类型名称          字节数      取值范围
// signed char        1         -2^7 ~ 2^7-1    -128～+127
// short int          2         -2^14 ~ 2^14-1  -32768～+32767
// int                4         -2^31 ~ 2^31-1  -2147483648～+2147483647
// unsigned int       4         0 ~ 2^32-1      0 ~ 4294967295
// long int           4         -2^31 ~ 2^31-1  -2147483648～+2141483647   （此类型开发环境下是8字节）
// unsigned long      4         0 ~ 2^32-1      0～4294967295（此类型开发环境下是8字节）
// long long int      8         -2^63 ~ 2^63-1  -9223372036854775808～+9223372036854775807
// unsigned long long 8         0 ~ 2^64-1      18446744073709551615

// 可以通过解注释下面代码来测定当前系统的类型对应位数，如果出现“定义位禁止跨越类型的长度”，说明位数超过了前面的类型位数
// typedef struct 
// {
//     // 类型名称          位数 （开发环境下类型对应位数情况如下：）
//     signed char s1: 2;   //9
//     short int s2  : 2;     //16
//     int  s3: 2;          // 32 
//     unsigned int s4: 2;  //32 
//     long int   s5: 2;    // 64  //开发环境下long是4字节
//     unsigned long s6: 2; //64 
//     long long int s7: 2;  //64
//     unsigned long long s8: 2; //64
//     // float s9: 2;
//     // error: bit-field 's9' has non-integral type 'float' [clang-diagnostic-error]
//     // float s9: 2;
// };

typedef struct 
{
    signed int si01 : 1;  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:23: warning: 位定义的有符号整型变量位长必须大于1 [gjb8114-bit-define]
    signed int si02 : 2;
    unsigned int serv : 29;
    unsigned int serv1 : 1;
    signed serv2  :1; //违背1-1
    // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: 位定义的有符号整型变量位长必须大于1 [gjb8114-bit-define]
}sData;
int main_c6(void)
{
    sData my_data;
    my_data.si01=-1;
    my_data.si02=-1;
    return (0);
}
//遵循示例
typedef struct 
{
    signed int si01 : 2;  //遵循1
    signed int si02 : 2;
    unsigned int serv : 28;
}sData1;
int main_c6_f(void)
{
    sData1 my_data;
    my_data.si01=-1;
    my_data.si02=-1;
    return (0);
}

//CSR-66	位的定义必须是有符号整数或无符号整数		4.6.1.7	BinaryOperatorChecker	R-1-1-11	gb8114-BitDefine		
//CSR-66  GJB 8114-2013 R-1-1-11
//违背示例
typedef struct
{
    short d01 : 2;  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 位定义的整数型变量必须明确定义是有符号还是无符号的 [gjb8114-bit-define]
    short d02 : 2;  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 位定义的整数型变量必须明确定义是有符号还是无符号的 [gjb8114-bit-define]
    short res : 12;  //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 位定义的整数型变量必须明确定义是有符号还是无符号的 [gjb8114-bit-define]
}sData2;
int main_c7(void)
{
    sData2 my_data;
    my_data.d01=1;
    my_data.d02=1;
    my_data.res=0;
    return (0);
}
//遵循示例
typedef struct
{
    signed short d01 : 2;  //遵循2
    unsigned short d02 : 2;  //遵循3
    unsigned short res : 12;  //遵循4
}sData3;
int main_c7_f(void)
{
    sData3 my_data;
    my_data.d01=1;
    my_data.d02=1;
    my_data.res=0;
    return (0);
}

//CSR-45	位定义的变量必须是同长度的类型且定义位禁止跨越类型的长度				R-1-1-12	gb8114-BitDefine		
//CSR-45  GJB 8114-2013 R-1-1-12
//违背示例
typedef struct 
{
    unsigned char  d01 : 2;
    unsigned char  d02 : 2;
    unsigned char  d03 : 2;
    unsigned short  d04 : 2;  //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: 位定义的变量必须是同长度的类型 [gjb8114-bit-define]
}stData4;
typedef struct 
{
    unsigned short  d01 : 2;
    unsigned short  d02 : 2;
    unsigned short  d03 : 2;
    unsigned short  d04 : 12;  //违背6 在开发环境下unsigned short 是16位，12并未违背；（此用例是标准中的违背例子）
    unsigned short  d04_1 : 18;  //违背6-1
    // CHECK-MESSAGES: :[[@LINE-1]]:29: warning: 定义位禁止跨越类型的长度 [gjb8114-bit-define]
    unsigned short  d05 : 2;
    unsigned short  d06 : 2;
    unsigned short  d07 : 2;
    unsigned short  d08 : 8;
}stData5;
int main_A45(void)
{
    stData4 my_data1;
    stData5 my_data2;
    return (0);
}
//遵循示例
typedef struct 
{
    unsigned char  d01 : 2;
    unsigned char  d02 : 2;
    unsigned char  d03 : 2;
    unsigned char  d04 : 2;  //遵循5
}stData6;
typedef struct 
{
    unsigned short  d01 : 2;
    unsigned short  d02 : 2;
    unsigned short  d03 : 2;
    unsigned short  d04 : 10;  //遵循6
    unsigned short  d05 : 2;
    unsigned short  d06 : 2;
    unsigned short  d07 : 2;
    unsigned short  d08 : 10;

}stData7;
int main_A45_f(void)
{
    stData6 my_data1;
    stData7 my_data2;
    return (0);
}