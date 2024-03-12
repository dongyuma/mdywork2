// RUN: %check_clang_tidy %s gjb8114-invalid-variable-name %t

//CSR-271	禁止仅依赖小写字母“l"与数字“1”区分的变量	R-1-13-9	gjb8114-invalid-variable-name	Rule 7.3	misrac2012-VarName 
//CSR-271 GJB 8114-2013 R-1-13-9
//违背示例
int main_1(void)
{
    int fSpeedl;
    int fSpeed1; //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止仅依赖小写字母“l”与数字“1”区分的变量 [gjb8114-invalid-variable-name]
    return (0);
}

//CSR-272	禁止仅依赖大小写区分变量				R-1-13-8	gjb8114-invalid-variable-name		
//CSR-272 GJB 8114-2013 R-1-13-8
//违背示例
int main_2(void)
{
    int fSpeed;
    int fspeed; //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止仅依赖大小写区分的变量 [gjb8114-invalid-variable-name]
    return (0);
}

//CSR-273	禁止仅依赖大写字母“O”与数字“0”区分的变量				R-1-13-10	gjb8114-invalid-variable-name
//CSR-273 GJB 8114-2013 R-1-13-10
//违背示例
int main_3(void)
{
    int fSpeedO;
    int fSpeed0; //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止仅依赖大写字母“O”与数字“0”区分的变量 [gjb8114-invalid-variable-name]
    return (0);
}

//CSR-100	禁止单独使用小写字母“l”和大写字母“O”作为变量名		4.8.1.1	gb5369-Invalid-Varibale-Name	R-1-13-11	gjb8114-invalid-variable-name		
//CSR-100 GJB 8114-2013 R-1-13-11
//违背示例
int main_4(void)
{
    int l=1;  //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止单独使用小写字母“l”或大写字母“O”作为变量名 [gjb8114-invalid-variable-name]
    int O=0;  //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止单独使用小写字母“l”或大写字母“O”作为变量名 [gjb8114-invalid-variable-name]
    l=O;
    O=l;
    return (0);
}