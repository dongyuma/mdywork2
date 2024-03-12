// RUN: %check_clang_tidy %s gjb8114-pointer-without-check-null %t

//CSR-144	动态分配的指针变量第一次使用前必须进行是否为NULL的判别				R-1-3-8	gb8114-PointerBeforeuse		
//CSR-144 GJB 8114-2013 R-1-3-8
//违背示例
#include<malloc.h>
int returndata(int number)
{
    return ++number;
}
int main(void)
{
    int z=1;
    returndata(z);
    int *y=(int *)malloc(sizeof(int));
    if(z==1)
    {
        *y=5;  //违背1
        // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 动态分配的指针变量第一次使用前必须进行是否为NULL的判别 [gjb8114-pointer-without-check-null]
    }
    if(y==NULL)
    {
        z++;
    }
    int *x=(int *)malloc(sizeof(int));
    if(x!=NULL)
    {
        *x=3;  //遵循1
    }
    for(int i=0;i<8;i++)
    {
        *x++;  //遵循2
    }
    int *p=(int *)malloc(sizeof(int));
    *p=0;  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: 动态分配的指针变量第一次使用前必须进行是否为NULL的判别 [gjb8114-pointer-without-check-null]
    *p=8;  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: 动态分配的指针变量第一次使用前必须进行是否为NULL的判别 [gjb8114-pointer-without-check-null]
    if(p!=NULL)
    {
        for(int j=0;j<5;j++)
        {
            *p++;  ///遵循3
        }
    }
    *p=2;  //遵循4
    while(z<5)
    {
        z++;
        *p++;  //遵循5
    }
    double *pointer=(double *)malloc(sizeof(double));
    for(int i=0;pointer!=nullptr;*pointer=*pointer+1.2)  //遵循6
    {
        *pointer=*pointer+1.5;  //遵循7
    }
    int *ptr=(int *)malloc(sizeof(int));
    while (z<90)
    {
      z++;
      for(int g=0;g<3;g++)
      {
          if(g==45)
          {
              *ptr=3;  //违背4
              // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 动态分配的指针变量第一次使用前必须进行是否为NULL的判别 [gjb8114-pointer-without-check-null]
              if(*ptr>4)  //违背5
              // CHECK-MESSAGES: :[[@LINE-1]]:19: warning: 动态分配的指针变量第一次使用前必须进行是否为NULL的判别 [gjb8114-pointer-without-check-null]
              {
                  z--;
              }
              int number=*ptr;  //违背6
              // CHECK-MESSAGES: :[[@LINE-1]]:27: warning: 动态分配的指针变量第一次使用前必须进行是否为NULL的判别 [gjb8114-pointer-without-check-null]
              int number1=g==45?*ptr:0;  //违背7
              // CHECK-MESSAGES: :[[@LINE-1]]:34: warning: 动态分配的指针变量第一次使用前必须进行是否为NULL的判别 [gjb8114-pointer-without-check-null]
          }
      }
    }
    int *ptrpointer=(int *)malloc(sizeof(int));
    do{
       *ptrpointer=5;  //违背8
       // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 动态分配的指针变量第一次使用前必须进行是否为NULL的判别 [gjb8114-pointer-without-check-null]
    }while (ptrpointer!=nullptr);
    if(p!=NULL)
    {
        z++;
    }
    int *test=&z;
    *test=5;  //遵循8(这不是一个动态指针)
    int datapointer=1;
    datapointer=5;
    int *datapointer1;
    datapointer1=(int *)malloc(sizeof(int)); //遵循9(指针变量在赋值号左边，不能算作"指针使用")
    *datapointer1=5;  //违背9
    // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: 动态分配的指针变量第一次使用前必须进行是否为NULL的判别 [gjb8114-pointer-without-check-null]
    return (0);
}