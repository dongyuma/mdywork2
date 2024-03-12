// RUN: %check_clang_tidy %s gjb8114-condition-with-brace %t

//CSR-91	循环体必须用大括号括起来		4.2.1.2	gb5369-ConditionalBodyBrace	R-1-2-1	gjb8114-condition-with-brace
//CSR-91  GJB 8114-2013 R-1-2-1
int main_GJB8114_2013_1_2_1(void)
{
  int i;
  int data[10];
  for(i=0;i<10;i++)//违背1 
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 循环体必须用大括号括起来 [gjb8114-condition-with-brace]
    data[i]=0; 
  while (i < 100)//违背2
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 循环体必须用大括号括起来 [gjb8114-condition-with-brace]
    if (i >5)
    {
      i++;
    }
  do//违背3
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 循环体必须用大括号括起来 [gjb8114-condition-with-brace]
    i--;
  while (i>10);
  return(0);
}
//CSR-103	if、else if、else必须用大括号括起来				R-1-2-2	gjb8114-condition-with-brace
//CSR-103  GJB 8114-2013 R-1-2-2
int main_GJB8114_2013_1_2_2(void) 
{
  int i=0; 
  int j;
  if(0==i) 
    j=1; //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: If, else if, else 必须用大括号括起来 [gjb8114-condition-with-brace]
  else if(1==i)
    j=3; //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: If, else if, else 必须用大括号括起来 [gjb8114-condition-with-brace]
  else
    j=5; //违背6
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: If, else if, else 必须用大括号括起来 [gjb8114-condition-with-brace]
  return(0);
}
int main_GJB8114_2013_1_2_2_3(void) 
{
  int i=0; 
  int j;
  if(0==i)
  {
    j=1;
  }
  else 
    j=5; //违背7
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: If, else if, else 必须用大括号括起来 [gjb8114-condition-with-brace]
  return(0);
}
int main_GJB8114_2013_1_2_2_4(void) 
{
  int i=0; 
  int j;
  if(0==i)
  {
    j=1; 
  }
  else
  {
    j=5; 
  }
  return(0);
}

