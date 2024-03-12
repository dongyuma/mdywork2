//CSR-107	禁止使用无效语句	R-1-8-3
//CSR-107  GJB 8114-2013 R-1-8-3
//违背示例
int main(void)
{
    unsigned int local=0;
    unsigned int para=0;
    //...
    local;  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 表达式结果未使用 [-Wunused-value]
    para-0; //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 表达式结果未使用 [-Wunused-value]
    local==para; //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 相等结果未使用 [-Wunused-comparison]
    local>para;  //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 比较结果未使用 [-Wunused-comparison]
    return (0);
}

//CSR-201	函数声明和函数定义中的返回类型必须一致   4.9.1.5		R-1-7-6
//CSR-201  GJB 8114-2013 R-1-7-6
//违背示例
unsigned int fun(unsigned int par);  //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:14: note: 此处是先前的声明位置
int main(void)
{
    unsigned int i=1;
    unsigned int j;
    j=fun(i);
    return (0);
}
int fun(unsigned int par)  //违背2
// CHECK-MESSAGES: :[[@LINE-1]]:5: error: 函数声明和函数定义中的返回类型必须一致 [clang-diagnostic-error]
{
    if(1==par)
    {
        return (-1);
    }
    else if(2==par)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

//CSR-203	禁止使用旧形式的函数参数表定义形式		R-1-7-4	
//CSR-203  GJB 8114-2013 R-1-7-4
//违背示例
void fun(int *p1,int *p2);
int main(void)
{
    int i=0,j=0;
    fun(&i,&j);
    return (0);
}
void fun(p1,p2)  //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:10: error: 未识别的变量名 'p1' [clang-diagnostic-error]
// CHECK-MESSAGES: :[[@LINE-2]]:13: error: 未识别的变量名 'p2' [clang-diagnostic-error]
// CHECK-MESSAGES: :[[@LINE-3]]:16: error: 在函数声明后缺失; [clang-diagnostic-error]
int *p1;
int *p2;
{
// CHECK-MESSAGES: :[[@LINE-2]]:1: error: 需要标识符 [clang-diagnostic-error]
    *p1=*p1+1;
    *p2=*p2+2;
}

//CSR-221	指针变量的赋值类型必须与指针变量类型一致	R-1-10-4
//CSR-221  GJB 8114-2013 R-1-10-4
//违背示例
#include<stdlib.h>
int main(void)
{
    unsigned int *ptr=NULL;
    unsigned short uid=0;
    ptr=(unsigned short *)(&uid);  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:9: error: 不兼容的指针类型:转换后类型:'unsigned int *',转换前类型:'unsigned short *' [clang-diagnostic-error]
    ptr=(&uid);  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:9: error: 不兼容的指针类型:转换后类型:'unsigned int *',转换前类型:'unsigned short *' [clang-diagnostic-error]
    return (0);
}
//遵循示例
#include<stdlib.h>
int main_1(void)
{
    unsigned int *ptr=NULL;
    unsigned short uid=0;
    ptr=(unsigned int *)(&uid);  //遵循1
    return (0);
}

//CSR-222	将指针量赋予非指针变量或非指针量赋予指针变量,必须使用强制转换	R-1-10-5
//CSR-222  GJB 8114-2013 R-1-10-5
//违背示例
#include<stdlib.h>
int main(void)
{
    unsigned int *ptr=NULL;
    unsigned int adr=0;
    unsigned int uid=0;
    ptr=adr;  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:9: error: 不兼容的整数类型向指针类型的转换:转换后类型:'unsigned int *',转换前类型:'unsigned int';应添加地址符号& [clang-diagnostic-error]
    adr=&uid;  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:9: error: 不兼容的指针类型向整数类型的转换:转换后类型:'unsigned int',转换前类型:'unsigned int *';应去除&符号 [clang-diagnostic-error]
    return (0);
}
//遵循示例
#include<stdlib.h>
int main_1(void)
{
    unsigned int *ptr=NULL;
    unsigned int adr=0;
    unsigned int uid=0;
    ptr=(unsigned int *)adr;  //遵循1
    adr=(unsigned int)(&uid);  //遵循2
    // CHECK-MESSAGES: :[[@LINE-1]]:9: error: cast from pointer to smaller type 'unsigned int' loses information [clang-diagnostic-error]
    return (0);
}

//I-1	实参与形参的个数必须一致		4.7.1.1		R-1-7-3	
//I-1  GJB 8114-2013 R-1-7-3
//违背示例
int fcal(int x,int y);
// CHECK-MESSAGES: :[[@LINE-1]]:5: note: 候选function不可用:需要2个arguments,但是3个参数被提供
int main(void)
{
    int datax,datay,dataz;
    datax=2;
    datay=1;
    dataz=fcal(datax,datay,datax);  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:11: error: 没有用于调用的匹配函数:'fcal' [clang-diagnostic-error]
    return (0);
}
int facl(int x,int y)
{
    int ret=x+y;
    return ret;
}

//CSR-277	禁止将NULL作为整型数0使用				R-1-13-14
//CSR-277  GJB 8114-2013 R-1-13-14
//违背示例
#include<stdlib.h>
int fun(int width);
int main(void)
{
    int i;
    i=fun(NULL);  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 不允许的强制转换:被转换类型:空指针,转换类型:'int' [-Wnull-conversion]
    return (0);
}
int fun(int width)
{
    int w;
    w=width-10;
    return w;
}
//遵循示例
#include<stdlib.h>
int fun(int width);
int main(void)
{
    int i;
    i=fun(0);  //遵循1
    return (0);
}
int fun(int width)
{
    int w;
    w=width-10;
    return w;
}