// RUN: %check_clang_tidy %s gjb8114-type-cast %t

//CSR-218	浮点数变量赋值给整型变量必须强制转换				R-1-10-1	gjb8114-type-cast		
//CSR-218  GJB 8114-2013 R-1-10-1
//违背示例
int main_h5(void)
{
    int ix, iy;
    float fx = 1.85;
    float fy = -1.85;
    ix = fx;  //违背5-1
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 浮点数变量赋给整型变量必须强制转换 [gjb8114-type-cast]
    iy = fy;  //违背5-2
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 浮点数变量赋给整型变量必须强制转换 [gjb8114-type-cast]
    iy = fx + fy; //违背5-3
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 浮点数变量赋给整型变量必须强制转换 [gjb8114-type-cast]
    fx = ix; //IntegralToFloating
    return (0);
}

// TODO: 參數賦值時候，显示转化
//   i = 0.5;   
//   void f(int); f(0.5);

//遵循示例
#define Round(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))
int main_h5_f(void)
{
    int ix, iy;
    float fx = 1.85;
    float fy = -1.85;
    ix = (int)(fx);  //遵循5-1
    ix = Round(fx);  //遵循5-2
    iy = (int)(fy);  //遵循5-3
    iy = Round(fy);  //遵循5-4
    return (0);
}

//CSR-219	长整数变量赋值给短整数变量必须强制转换				R-1-10-2	gjb8114-type-cast		
//CSR-219  GJB 8114-2013 R-1-10-2
//违背示例
int main_h6(void)
{
    signed char cVar=0;
    short sVar=0;
    int iVar=0;
    long lVar=0;
    cVar=sVar;  //违背6-1
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 长整数变量赋给短整数变量必须强制转换 [gjb8114-type-cast]
    sVar=iVar;  //违背6-2
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 长整数变量赋给短整数变量必须强制转换 [gjb8114-type-cast]
    iVar=lVar;  //违背6-3
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 长整数变量赋给短整数变量必须强制转换 [gjb8114-type-cast]
    lVar=iVar; //IntegralCast
    return (0);
}
//遵循示例
int main_h6_f(void)
{
    signed char cVar=0;
    short sVar=0;
    int iVar=0;
    long lVar=0;
    cVar=(signed char)sVar;  //遵循6-1
    sVar=(short)iVar;  //遵循6-2
    iVar=(int)lVar;  //遵循6-3
    return (0);
}

//CSR-220	double型变量赋给float型变量必须强制转换				R-1-10-3	gjb8114-type-cast		
//CSR-220  GJB 8114-2013 R-1-10-3
//违背示例
int main_h7(void)
{
    double dData=0.0;
    float fData;
    fData=dData;  //违背7-1
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: double型变量赋给float型变量必须强制转换 [gjb8114-type-cast]
    dData = fData;
    return (0);
}
//遵循示例
int main_h7_f(void)
{
    double dData=0.0;
    float fData;
    fData=(float)dData;  //遵循7-1
    return (0);
}

// //CSR-221	指针变量的赋值类型必须与指针变量类型一致				R-1-10-4	gjb8114-type-cast		
// //CSR-221  GJB 8114-2013 R-1-10-4
// //违背示例
// #include<stdlib.h>
// int main_h8(void)
// {
//     unsigned int *ptr=NULL;
//     unsigned short uid=0;
//     ptr=(unsigned short *)(&uid);  //违背8-1
//     ptr=(&uid);  //违背8-2
//     return (0);
// }
// //遵循示例
// // #include<stdlib.h>
// int main_h8_f(void)
// {
//     unsigned int *ptr=NULL;
//     unsigned short uid=0;
//     ptr=(unsigned int *)(&uid);  //遵循8-1
//     return (0);
// }

// //CSR-222	将指针量赋予非指针变量或非指针量赋予指针变量,必须使用强制转换				R-1-10-5	gjb8114-type-cast		
// //CSR-222  GJB 8114-2013 R-1-10-5
// //违背示例
// // #include<stdlib.h>
// int main_h9(void)
// {
//     unsigned int *ptr=NULL;
//     unsigned int adr=0;
//     unsigned int uid=0;
//     ptr=adr;  //违背9-1
//     adr=&uid;  //违背9-2

//     return (0);
// }
// //遵循示例
// // #include<stdlib.h>
// int main_h9_f(void)
// {
//     unsigned int *ptr=NULL;
//     unsigned int adr=0;
//     unsigned int uid=0;
//     ptr=(unsigned int *)adr;  //遵循9-1
//     adr=(unsigned int)(&uid);  //遵循9-2
//     return (0);
// }

// H8/H9系统支持
