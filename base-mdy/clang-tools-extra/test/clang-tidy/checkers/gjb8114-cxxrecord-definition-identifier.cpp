// RUN: %check_clang_tidy %s gjb8114-cxxrecord-definition-identifier %t

// CSR-26 GJB-8114 R-1-1-8
// 结构、联合、枚举的定义中必须定义标识名
struct  //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 结构体的定义中必须定义标识名 [gjb8114-cxxrecord-definition-identifier]
{
    int data1;
    int data2;
}sData_v1;
union  //违背2
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 联合体的定义中必须定义标识名 [gjb8114-cxxrecord-definition-identifier]
{
    unsigned char cd[4];
    int id;
}uData_v1;
enum  //违背3
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 枚举的定义中必须定义标识名 [gjb8114-cxxrecord-definition-identifier]
{
    A_Level=0,
    B_Level,
    C_Level,
    D_Level
}eLevel_v1;
int main_v1(void)
{
    eLevel_v1=B_Level;
    sData_v1.data1=2000;
    uData_v1.id=sData_v1.data1;
    return (0);
}


//遵循示例
struct  S_Data  //遵循1
{
    int data1;
    int data2;
}sData_v2;
union  U_Data //遵循2
{
    unsigned char cd[4];
    int id;
}uData_v2;
enum  E_Level  //遵循3
{
    A_Level_v2=0,
    B_Level_v2,
    C_Level_v2,
    D_Level_v2
}eLevel_v2;
int main_v2(void)
{
    eLevel_v2=B_Level_v2;
    sData_v2.data1=2000;
    uData_v2.id=sData_v2.data1;
    return (0);
}
