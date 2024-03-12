// RUN: %check_clang_tidy %s gjb8114-octonary-number-use %t

//CSR-102	使用八进制数必须加以注释		4.8.1.3	gb5369-Invalid-Varibale-Name	R-1-8-4	gjb8114-octonary-number-use
//CSR-102 GJB 8114-2013 R-1-8-4
//违背示例
int main_1(void)
{
    int code[3];
    code[0]=109;
    code[1]=100;
    code[2]=011;  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 使用八进制数必须明确注释 [gjb8114-octonary-number-use]
    int z=022323;  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 使用八进制数必须明确注释 [gjb8114-octonary-number-use]
    int data1=03247+5;  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 使用八进制数必须明确注释 [gjb8114-octonary-number-use]
    if(data1==032)  //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 使用八进制数必须明确注释 [gjb8114-octonary-number-use]
    {
        return (1);
    }
    int data2=932;  //遵循1
    int data3=0xaabbcc;  //遵循2
    int data4=032*5+1;  //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 使用八进制数必须明确注释 [gjb8114-octonary-number-use]
    int data5=032; /*octal*/ //遵循3
    int data6=073; /*                                 octal            */  //遵循4
    int data7=0234+1.0;  //违背6
    // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 使用八进制数必须明确注释 [gjb8114-octonary-number-use]
    return (0);
}