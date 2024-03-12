// RUN: %check_clang_tidy %s gjb8114-conditional-expression-without-assignment-statement %t

//CSR-257	禁止在逻辑表达式中使用赋值语句				R-1-6-3	gjb8114-conditional-expression-without-assignment-statement
//CSR-257  GJB 8114-2013 R-1-6-3
//违背示例
int main_1(void)
{
  int i = 0, j = 0,number=0;
  if(i = 1)  //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  {
    number++;
  }
  if(i += 1)  //违背2
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  {
    number++;
  }
  if(i -= 1)  //违背3
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  {
    number++;
  }
  if(i *= 1)  //违背4
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  {
    number++;
  }
  if(i /= 1)  //违背5
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  {
    number++;
  }
  if(i==1 && (i=2))  //违背6
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  {
    number++;
  }
  if((((((i=4))))) || (((j=5))))  //违背7
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  {
    number--;
  }
  if(i == 1)  //遵循1
  {
    j = j + 1;
  }
  return (0);
}

int main_2(void)
{
  int i , j = 0,number=0;
  for(i=0;i = 1;i++)  //违背8
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  {
    number++;
  }
  for(i=0;i += 1;i++)  //违背9
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  {
    number++;
  }
  for(i=0;i -= 1;i++)  //违背10
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  {
    number++;
  }
  for(i=0;i *= 1;i++)  //违背11
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  {
    number++;
  }
  for(i=0;i /= 1;i++)  //违背12
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  {
    number++;
  }
  for(int j=0;i==1 && (j=3);j++)  //违背13
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  {
    number++;
  }
  for(i=0;i == 1;i++)  //遵循1
  {
    number++;
  }
  for(i=0;i < 1;i++)  //遵循2
  {
    j = j + 1;
  }
  return (0);
}

int main_3(void)
{
  int i=0 , j = 0;
  while(i = 1)  //违背14
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  {
    i++;
  }
  while(i += 1)  //违背15
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  {
    j = 2;
    i=i+1;
  }
  while(i -= 1)  //违背16
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  {
    j = 2;
    i=i+1;
  }
  while(i *= 1)  //违背17
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  {
    j = 2;
    i=i+1;
  }
  while(i /= 1)  //违背18
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  {
    j = 2;
    i=i+1;
  }
  while(i == 1)  //遵循1
  {
    j = 2;
    i=i+1;
  }
  while(i < 1)  //遵循2
  {
    j = 2;
    i=i+1;
  }
  return (0);
}

int main_4(void)
{
  int i=0 , j = 0;
  do
  {
    j = 2;
    i=i+1;
  }while(i = 1);//违背19
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  do
  {
    j = 2;
    i=i+1;
  }while(i += 1);//违背20
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  do
  {
    j = 2;
    i=i+1;
  }while(i -= 1);//违背21
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  do
  {
  j = 2;
  i=i+1;
  }while(i *= 1);//违背22
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  do
  {
  j = 2;
  i=i+1;
  }while(i /= 1);//违背23
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
  do
  {
  j = 2;
  i=i+1;
  }while(i == 1);//遵循1
  do
  {
  j = 2;
  i=i+1;
  }while(i < 1);//遵循2
  return (0);
}
void R_1_6_3()
{
    int aa = 0;
    int bb = 1;
    if (aa = bb) //违背24
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止在逻辑表达式中使用赋值语句 [gjb8114-conditional-expression-without-assignment-statement]
    {
        aa = 1;
    }
}
