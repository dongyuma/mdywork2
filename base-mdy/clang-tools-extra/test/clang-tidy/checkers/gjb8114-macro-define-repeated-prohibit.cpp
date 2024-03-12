// RUN: %check_clang_tidy %s gjb8114-macro-define-repeated-prohibit %t

//CSR-24	禁止#define被重复定义				R-1-1-5	gjb8114-macro-define-repeated-prohibit
//CSR-24  GJB 8114-2013 R-1-1-5
//违背示例
#define BLOCKDEF 1U
unsigned int fun1(void);
unsigned int fun2(void);
int main_1(void)
{
  unsigned int a1 = 0U;
  unsigned int a2 = 0U;
  unsigned int a3 = 0U;
  unsigned int a4 = 0U;
  a1 = fun1();
  a2 = a1 + BLOCKDEF;
  a3 = fun2();
  a4 = a3 + BLOCKDEF;
  return (0);
}
#define BLOCKDEF 2U  //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止#define被重复定义 [gjb8114-macro-define-repeated-prohibit]

unsigned int fun1(void)
{
  unsigned int x = 0U;
  x = x +BLOCKDEF;
  return x;
}

unsigned int fun2(void)
{
  unsigned int x = 0U;
  x = x + BLOCKDEF; 
  return x;
}

//遵循示例
#define BLOCKDEFA 1U
unsigned int fun3(void);
unsigned int fun4(void);
int main_2(void)
{
  unsigned int a1 = 0U;
  unsigned int a2 = 0U;
  unsigned int a3 = 0U;
  unsigned int a4 = 0U;
  a1 = fun3();
  a2 = a1 + BLOCKDEFA;
  a3 = fun4();
  a4 = a3 + BLOCKDEFA;
  return (0);
}
unsigned int fun3(void)
{
  unsigned int x = 0U;
  x = x +BLOCKDEFA;
  return x;
}

unsigned int fun4(void)
{
  unsigned int x = 0U;
  x = x + BLOCKDEFA;
  return x;
}


#define BLOCKDEFB 1U
unsigned int fun5(void);
unsigned int fun6(void);
int main_3(void)
{
    unsigned int a1 = 0U;
    unsigned int a2 = 0U;
    unsigned int a3 = 0U;
    unsigned int a4 = 0U;
    a1 = fun5();
    a2 = a1 + BLOCKDEFB;
    a3 = fun6();
    a4 = a3 + BLOCKDEFB;
    return (0);
}
#undef BLOCKDEFB
#define BLOCKDEFB 2U //遵循1
unsigned int fun5(void)
{
    unsigned int x = 0U;
    x = x +BLOCKDEFB;
    return x;
}

unsigned int fun6(void)
{
    unsigned int x = 0U;
    x = x + BLOCKDEFB;
    return x;
}

