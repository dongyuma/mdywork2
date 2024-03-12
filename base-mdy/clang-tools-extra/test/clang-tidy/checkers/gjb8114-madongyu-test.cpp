//CSR-28	函数声明必须与函数原型一致(注:一致性要求包括函数类型、参数类型、参数名。)				R-1-1-14
//CSR-28 GJB 8114-2013 R-1-1-14
//违背示例
//编译器不支持
int fun1(short height); //违背1
float fun2(int height); //违背2
int fun3(int height); //违背3
int main_2(void)
{
  int i,j1,j2,j3; 
  i=1000; 
  j1=fun1(i);
  j2=fun2(i); 
  j3=fun3(i); 
  return(0);
}
int fun1(int height)
{
  int h;
  h=height+10; 
  return h;
}
int fun2(int height)
// CHECK-MESSAGES: :[[@LINE-1]]:5: error: 函数声明和函数定义中的返回类型必须一致 [clang-diagnostic-error]
{
  int h;
  h=height-10;
  return h;
}
int fun3(int width)
{
  int w;
  w = width-10;
  return w;
}

// CSR-29	函数中的参数必须使用类型声明			R-1-1-15
// CSR-29 GJB 8114-2013 R-1-1-15
//违背示例
int fun(height) //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:9: error: 未识别的变量名'height' [clang-diagnostic-error]
{
  int h;
  h = height+10;
  // CHECK-MESSAGES: :[[@LINE-1]]:7: error: 未识别的变量名'height' [clang-diagnostic-error]
  return h;
}
int main_3(void)
{
  int i,j; 
  i=1000;
  j=fun(i);
  return (0);
}
//遵循示例
int fun(int height) //遵循1 
{
int h;
h=height+10;
return h;
}
int main_3f(void)
{
int i,j;
 i=1000;
 j=fun(i);
return (0);
}

// CSR-30	外部声明的变量,类型必须与定义一致			R-1-1-16
// CSR-30 GJB 8114-2013 R-1-1-16
//违背示例
//不清楚测试方法，无法判定编译器是否支持
//程序文件1 
int zdata= 0;
//程序文件2
extern short zdata; //违背1
int main_4(void)
{
  short x, y; 
  x=1;
  y=2;
  zdata=x+y; 
  return(0);
}
//遵循示例
//程序文件1 
int zdata=0;
//程序文件2
extern int zdata; //遵循1 
int main_4f(void)
{
  short x,y; 
  x=1;
  y=2;
  zdata=x+y;
  return (0);
}

// CSR-49	变量禁止未赋值就使用		R-1-11-1
// CSR-49 GJB 8114-2013 R-1-11-1
//违背示例
//编译器不支持
int main_5(void)
{
  int i;
  float x,y,z;
  x=z; //违背1 
  if(0==i) //违背2
  {
    y=z; //违背3 
  }
  return(0);
}
//遵循示例
int main_5f(void)
{
  int i=0;
  float x,y, z;
  z=0.0;
  x=z; //遵循1 
  if(0==i) //遵循2
  {
    y=z; //遵循3 
  }
  return(0);
}

// CSR-198	禁止无返回值函数的返回语句带有返回值			R-1-7-8
// CSR-198 GJB 8114-2013 R-1-7-8
//违背示例
void fun(int a,int *b);
int main_6(void)
{
  int i=1,j=2;
  fun(i,&j); 
  return(0);
}
void fun(int a, int *b)
{
  if(0==a)
  {
    *b=*b+a;
  }
  else
  {
    *b=*b-a;
  }
  return(a+1); //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:3: error: 类型为空的函数 'fun' 不能返回数值 [clang-diagnostic-return-type]
}