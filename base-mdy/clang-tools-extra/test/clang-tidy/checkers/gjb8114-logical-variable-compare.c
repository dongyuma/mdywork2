// RUN: %check_clang_tidy %s gjb8114-logical-variable-compare %t

//CSR-224	禁止对逻辑量进行大于或小于的逻辑比较				R-1-12-1	gjb8114-logical-variable-compare
//CSR-224 GJB 8114-2013 R-1-12-1
//违背示例
typedef  signed char   bool;
#define TRUE   1
#define FALSE  0
typedef  signed char     BOOL;
#define bOOL signed char
int main_1(void)
{
    bool outReg1,outReg2;
    int r=100,h=500,flag=0;
    outReg1=FALSE;
    outReg2=FALSE;
    if(outReg1>!outReg2)  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 禁止对逻辑量进行大于或小于的逻辑比较 [gjb8114-logical-variable-compare]
    {
        flag=1;
    }
    if(outReg1<outReg2)  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 禁止对逻辑量进行大于或小于的逻辑比较 [gjb8114-logical-variable-compare]
    {
        flag=2;
    }
    return (0);
}
int main_2(void)
{
    bool outReg1,outReg2;
    int r=100,h=500,flag=0;
    outReg1=(r>100);
    outReg2=(h>300);
    if(outReg1 && (!outReg2))  //遵循3
    {
        flag=1;
    }
    if((!outReg1) && outReg2)  //遵循4
    {
        flag=2;
    }
    return (0);
}
/*
  * R-1-12-1：禁止对逻辑量进行大于或小于的逻辑比较。（二类）
  * 错误场景：
  */
void R_1_12_1_case1()
{
        BOOL bA = FALSE;
        BOOL bB = TRUE;
        if (bA > bB)  //违背3
        // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 禁止对逻辑量进行大于或小于的逻辑比较 [gjb8114-logical-variable-compare]
        {}
        bOOL bC=FALSE;
        bOOL bD=TRUE;
        if(bC>bD)  //违背4(暂不支持define)
        {

        }
}

typedef struct
{
        unsigned short int usA;
        unsigned short int usB;
        unsigned char ucC;
        unsigned char ucD;
        unsigned int uiE;
}ST_TEST;