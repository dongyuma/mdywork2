//CSR-223   禁止使用无实质作用的类型转换   R-1-10-6
//CSR-223   GJB 8114-2013 R-1-10-6
//违背示例
int main_1(void)
{
  unsigned int sx, sy=10,sz;
  sx=(unsigned int)sy+2; //违背1 
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止使用无实质作用的类型转换 [gjb8114-madongyu2-vertify]
  sz=(unsigned int)((float)((sx+sy)*2/3)+0.5); //违背2 
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止使用无实质作用的类型转换 [gjb8114-madongyu2-vertify]
  return (0);
}
//0 warnings 编译器不支持

// 遵循示例
int main_1f(void)
{
  unsigned int sx, sy=10, sz;
  sx=sy+2; //遵循1 
  sz=(unsigned int)(((float)(sx+ sy)*2/3)+ 0.5); //遵循2 
  return(0);
}

//CSR-49   变量禁止未赋值就使用   R-1-11-1
//CSR-49   GJB 8114-2013 R-1-11-1
//违背示例
int main_2(void)
{
  int i;
  float x,y,z;
  x=z; //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 变量禁止未赋值就使用 [gjb8114-madongyu2-vertify] 
  if(0==i) //违背2 
  // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 变量禁止未赋值就使用 [gjb8114-madongyu2-vertify]
  {
    y=z; //违背3 
    // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: 变量禁止未赋值就使用 [gjb8114-madongyu2-vertify]
  }
  return(0);
}
//0 warnings 编译器不支持

//遵循示例
int main_2f(void)
{
int i=0; float x,y,z;
z=0.0;
x=z; //遵循1 
if(0==i) //遵循2
{
y=z; //遵循3
}
 return(0);
}

//CSR-48   结构体初始化的嵌套结构必须与定义一致   R-1-11-3
//CSR-48   GJB 8114-2013 R-1-11-3
//违背示例
struct Spixel
{
  unsigned int colour;
  struct Scoords
  {
    unsigned int x; 
    unsigned int y; 
  }coords;
};
 
int main_3(void)
{
  struct Spixel pixel={1,2,3}; //违背1 
  // CHECK-MESSAGES: :[[@LINE-1]]:26: warning: 结构体初始化的嵌套结构必须与定义一致 [gjb8114-madongyu2-vertify]
  return(0);
}
//0 warnings 编译器不支持

//遵循示例
struct Spixel
{
  unsigned int colour;
  struct Scoords
  {
    unsigned int x; 
    unsigned int y; 
  }coords;
};
 
int main_3f(void)
{
  struct Spixel pixel={1,{2,3}}; //遵循1 
  // CHECK-MESSAGES: :[[@LINE-1]]:26: warning: 结构体初始化的嵌套结构必须与定义一致 [gjb8114-madongyu2-vertify]
  return(0);
}

//CSR-94   禁止在头文件前有可执行代码   R-1-2-3
// CSR-94   GJB 8114-2013 R-1-2-3
//违背示例
int main_4(void)
{
#include "..\061\061.h"//违背1      //fatal error: '..\061\061.h' file not found
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 禁止在头文件前有可执行代码 [gjb8114-madongyu2-vertify]
int ix=0; 
int iy=1; 
int iz=2;
idata = ix+iy+iz; 
return(0);
}
//编译器不支持
//1 error 编译器不支持(虽然是1 error，但是报的是找不到文件的错，而不是禁止在头文件前有可执行代码相关的错，所以是编译器不支持)

//遵循示例
#include "..\061\061.h"//遵循1      //fatal error: '..\061\061.h' file not found
int main_4f(void)
{
int ix=0; 
int iy=1; 
int iz=2;
idata = ix+iy+iz; 
return(0);
}

//CSR-243   禁止同一个表达式中调用多个顺序相关函数   R-1-7-14
//CSR-243   GJB 8114-2013 R-1-7-14
//违背示例
unsigned int Vel(unsigned int *pcData); 
unsigned int Acc(unsigned int *pcData); 
int main_5(void)
{
  unsigned int dis, hei; 
  dis =3;
  hei=Vel(&dis)+Acc(&dis); //违背1 
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: 禁止同一个表达式中调用多个顺序相关函数 [gjb8114-madongyu2-vertify]
  return(0);
}
unsigned int Vel(unsigned int *pcData)
{
  unsigned int x=(*pcData);
  (*pcData)= x*x; 
  return x;
}
unsigned int Acc(unsigned int *pcData)
{
  unsigned int x=(*pcData);
  (*pcData)= 2*x; 
  return x;
}
//0 warnings 编译器不支持

// 遵循示例
unsigned int Vel(unsigned int *pcData); 
unsigned int Acc(unsigned int *pcData); 
int main_5f(void)
{
  unsigned int dis, hei,temp1,temp2; 
  dis =3;
  temp1=Vel(&dis);
  temp2=Acc(&dis);
  hei=temp1+temp2; //遵循1 
  return(0);
}
unsigned int Vel(unsigned int *pcData)
{
  unsigned int x=(*pcData);
  (*pcData)= x*x; 
  return x;
}
unsigned int Acc(unsigned int *pcData)
{
  unsigned int x=(*pcData);
  (*pcData)= 2*x; 
  return x;
}


//CSR-105   禁止不可达语句   R-1-8-1
//CSR-105   GJB 8114-2013 R-1-8-1
//违背示例
int main_6(void) 
{
  int local =0; 
  int para = 0;
  //...
  switch(para)
  {
    local=para; //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止不可达语句 [gjb8114-madongyu2-vertify] 
    case 1:
    //...
      break; 
    case 2:
    //...
      break; 
    default:
    //..
      break;
  }
  return local;
  para++; //违背2 
  //CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止不可达语句 [gjb8114-madongyu2-vertify] 
}
//0 warnings 编译器不支持

//CSR-206 具有返回值的函数,其返回值如果不被使用,调用时应有(void)说明   R-1-7-11
//CSR-206   GJB 8114-2013 R-1-7-11
//违背示例
int func (int para) 
{
  int stat;
  if(para >= 0)
  {
    //... 
    stat=1;
  }
  else
  {
    //..
    stat=-1;
  }
    return(stat);
  }
int main_7(void)
{
  int local=0;
  //..
  func(local); //违背1 
  //CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 具有返回值的函数,其返回值如果不被使用,调用时应有(void)说明 [gjb8114-madongyu2-vertify] 
  //...
  return(0);
}
//0 warnings 编译器不支持

//遵循示例
int func_f (int para) 
{
  int stat;
  if(para >= 0)
  {
    //... 
    stat=1;
  }
  else
  {
    //..
    stat=-1;
  }
    return(stat);
  }
int main_7f(void)
{
  int local=0;
  int sign=0;
  //..
  sign=func(local); //遵循1 
  //...
  (void)func(local); //遵循2
   //...
  return(0);
}

// A-25  函数中的#define和#undef必须配对使用  R-1-1-6
// A-25  GJB 8114-2013  R-1-1-6
//违背示例
unsigned int fun1(void); 
unsigned int fun2(void); 
int main_8(void)
{
  unsigned int a1=0U; 
  unsigned int a2=0U; 
  unsigned int a3=0U;
  a1=fun1(); 
  a2=fun2(); 
  a3=a1+a2; 
  return(0);
}
unsigned int fun1(void)
{
  unsigned int x=0U;
  #define BLOCKDEF 2U //违背1 
  //CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 函数中的#define和#undef必须配对使用 [gjb8114-madongyu2-vertify] 
  x=x+BLOCKDEF;
  return x;
}

unsigned int fun2(void)
{
  unsigned int x=0U; 
  x=x+BLOCKDEF;
  #undef BLOCKDEF //违背2 
  //CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 函数中的#define和#undef必须配对使用 [gjb8114-madongyu2-vertify] 
  return x;
}
//0 warnings 编译器不支持

//遵循示例
unsigned int fun1(void); 
unsigned int fun2(void); 
int main_8f(void)
{
  unsigned int a1=0U; 
  unsigned int a2=0U; 
  unsigned int a3=0U;
  a1=fun1(); 
  a2=fun2(); 
  a3=a1+a2; 
  return(0);
}
unsigned int fun1_f(void)
{
  unsigned int x=0U;
  #define BLOCKDEF1 2U //遵循1 
  x=x+BLOCKDEF1;
  #undef BLOCKDEF1   //遵循2 
  return x;
}

unsigned int fun2_f(void)
{
  unsigned int x=0U; 
  #define BLOCKDEF2 2U //遵循3 
  x=x+BLOCKDEF2;
  #undef BLOCKDEF2 //遵循4 
  return x;
}

// CSR-275  程序外部可以改变的变量,必须使用volatile类型说明  R-1-13-12
// CSR-275  GJB 8114-2013  R-1-13-12
//违背示例(这个没给具体的测试用例，这么写对吗？)
unsigned int v=1;    //违背1 
//CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 程序外部可以改变的变量,必须使用volatile类型说明 [gjb8114-madongyu2-vertify] 
int main_9(void)
{
  unsigned int i,z[100]; 
  for(i=0;i<100;i++)
  {
    z[i]=3*v; 
  }
  return(0);
}
//0 warnings 编译器不支持

//遵循示例(这个没给具体的测试用例，这么写对吗？)
volatile unsigned int v=1;    //遵循1 
int main_9(void)
{
  unsigned int i,z[100]; 
  for(i=0;i<100;i++)
  {
    z[i]=3*v; 
  }
  return(0);
}

// CSR-276  禁止在表达式中出现多个同一volatile类型变量的运算  R-1-13-13
// CSR-276  GJB 8114-2013  R-1-13-13
//违背示例
int main_10(void)
{
  unsigned int i,z[100]; 
  volatile unsigned int v=1;
  for(i=0;i<100;i++)
  {
    z[i]=3*v*v+ 2*v +i; //违背1 
    //CHECK-MESSAGES: :[[@LINE-1]]:12: warning: 禁止在表达式中出现多个同一volatile类型变量的运算 [gjb8114-madongyu2-vertify] 
  }
  return(0);
}
//0 warnings 编译器不支持

//遵循示例
int main_10f(void)
{
  unsigned int i,j,z[100]; 
  volatile unsigned int v=1;
  for(i=0;i<100;i++)
  {
    j=v;                  //遵循1 
    z[i]=3*j*j+ 2*j +i; 
  }
  return(0);
}