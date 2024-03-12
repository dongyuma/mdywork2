// RUN: %check_clang_tidy %s gjb8114-error-use-of-loop-variable %t


//CSR-178	for循环控制变量必须使用局部变量				R-1-9-1	gjb8114-error-use-of-loop-variable
//CSR-178 GJB 8114-2013 R-1-9-1
//违背示例
int k=0;
int main(void)
{
  int data;
  int i=10;
  for(k=0;;k++)  //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: for循环控制变量必须使用局部变量 [gjb8114-error-use-of-loop-variable]
  {
      data++;
  }
  for(k=0;k<10;)  //违背2
  // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: for循环控制变量必须使用局部变量 [gjb8114-error-use-of-loop-variable]
  {
      data++;
  }
  for(k=0;k<10;k++)  //违背3
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: for循环控制变量必须使用局部变量 [gjb8114-error-use-of-loop-variable]
  {
      data++;
  }
  for(;k<10;)  //违背4
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: for循环控制变量必须使用局部变量 [gjb8114-error-use-of-loop-variable]
  {
      data++;
  }
  for(;;k++)  //违背5
  // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: for循环控制变量必须使用局部变量 [gjb8114-error-use-of-loop-variable]
  {
      data++;
  }
  for(;10<k;) //违背6
  // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: for循环控制变量必须使用局部变量 [gjb8114-error-use-of-loop-variable]
  {
      data++;
  }
  for(unsigned int m=0;;m++)  //遵循1
  {
      data++;
  }
  for(unsigned int m=0;m<10;)  //遵循2
  {
      data++;
  }
  for(i=0;i<5;i++)  //遵循3
  {
      data++;
  }
  for(i=4;;i--)  //遵循4
  {
      data++;
  }
  for(i=9;i<8;)  //遵循5
  {
      data++;
  }
  for(i=3;8>i;)  //遵循6
  {
      data++;
  }
  return (0);
}

//CSR-179	for循环控制变量必须使用整数型变量				R-1-9-2	gjb8114-error-use-of-loop-variable
//CSR-179 GJB 8114-2013 R-1-9-2
//违背示例
int main_1(void)
{
    float f=0.0,g=1.0;
    for(f=0.0;f<10.0;f=f+1.0)  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: for循环控制变量必须使用整数型变量 [gjb8114-error-use-of-loop-variable]
    {
        g=f+g;
    }
    for(int i=0;i<3;i++)  //遵循1
    {
        g=g+0.2;
    }
    for(unsigned int i=0;i<3;i++)  //遵循2
    {
        g=g+0.2;
    }
    for(char i=0;i<3;i++)  //遵循3
    {
        g=g+0.2;
    }
    int temp;
    for(temp=0;temp<5;--temp)  //遵循4
    {
        g=g+0.2;
    }
    double k;
    for(k=2.4;k<8.9;k=k+0.123)  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:21: warning: for循环控制变量必须使用整数型变量 [gjb8114-error-use-of-loop-variable]
    {
        g=g+0.2;
    }
    for(long circle=3;circle<50;--circle)  //遵循5
    {
        g=g+0.2;
    }
    for(int i=0;i<5;)  //遵循6
    {
        g=g+0.2;
        i++;  //违背3
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止在for循环体内部修改循环控制变量 [gjb8114-error-use-of-loop-variable]
    }
    for(double k=0;k<35;)  //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: for循环控制变量必须使用整数型变量 [gjb8114-error-use-of-loop-variable]
    {
        g=g+0.2;
    }
    for(float k=1.9;3.9>k;)  //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:21: warning: for循环控制变量必须使用整数型变量 [gjb8114-error-use-of-loop-variable]
    {
        g=g+0.2;
    }
    return (0);
}
//CSR-180	禁止在for循环体内部修改循环控制变量				R-1-9-3	gjb8114-Loop-Control		
//CSR-180 GJB 8114-2013 R-1-9-3
//违背示例
int main_2(void)
{
    int i,j,k;
    j=100;
    k=0;
    for(i=0;i<j;i+=1)
    {
        i=2*i;  //违背1
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止在for循环体内部修改循环控制变量 [gjb8114-error-use-of-loop-variable]
        i++;  //违背2
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止在for循环体内部修改循环控制变量 [gjb8114-error-use-of-loop-variable]
        k=k+1;
        i*=2;  //违背3
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止在for循环体内部修改循环控制变量 [gjb8114-error-use-of-loop-variable]
        i--;  //违背4
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止在for循环体内部修改循环控制变量 [gjb8114-error-use-of-loop-variable]
        i++;  //违背5
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止在for循环体内部修改循环控制变量 [gjb8114-error-use-of-loop-variable]
    }
    for(j=3;j<10;j++)
    {
        j+=1;  //违背6
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止在for循环体内部修改循环控制变量 [gjb8114-error-use-of-loop-variable]
        j++;  //违背7
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止在for循环体内部修改循环控制变量 [gjb8114-error-use-of-loop-variable]
        ++j;  //违背8
        // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止在for循环体内部修改循环控制变量 [gjb8114-error-use-of-loop-variable]
        --j;  //违背9
        // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止在for循环体内部修改循环控制变量 [gjb8114-error-use-of-loop-variable]
        j=j*3; //违背10
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止在for循环体内部修改循环控制变量 [gjb8114-error-use-of-loop-variable]
        if(k==0)
        {
            int j;
            j=j+1;  //遵循1
            j++;  //遵循2
        }
        j=k+5+4;  //违背11
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止在for循环体内部修改循环控制变量 [gjb8114-error-use-of-loop-variable]
    }
    for(j=3;5>j;)
    {
        ++j;  //违背12
        // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止在for循环体内部修改循环控制变量 [gjb8114-error-use-of-loop-variable]
        j--;  //违背13
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止在for循环体内部修改循环控制变量 [gjb8114-error-use-of-loop-variable]
    }
    return (0);
}
