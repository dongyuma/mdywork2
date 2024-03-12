// RUN: %check_clang_tidy %s gjb8114-pointer-logical-compare %t

//CSR-225	禁止对指针进行大于或小于的逻辑比较				R-1-12-2	gjb8114-pointer-logical-compare		
//CSR-225 GJB 8114-2013 R-1-12-2
//违背示例
int fsub(int *a,int *b);
int main(void)
{
    int sub=0;
    int a=1,b=2;
    sub=fsub(&a,&b);
    return (0);
}
int fsub(int *a,int *b)
{
    int sub=0;
    if(a>b)  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 禁止对指针进行大于或小于的逻辑比较 [gjb8114-pointer-logical-compare]
    {
        sub=(*a)-(*b);
    }
    else if(a<b)  //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: 禁止对指针进行大于或小于的逻辑比较 [gjb8114-pointer-logical-compare]
    {
        sub=(*b)-(*a);
    }
    else
    {
        sub=0;
    }
    if ((char *)a < (char*)b)
    // CHECK-MESSAGES: :[[@LINE-1]]:19: warning: 禁止对指针进行大于或小于的逻辑比较 [gjb8114-pointer-logical-compare]
    {
        sub=(*b)-(*a);
    }
    if ((a) < (b))
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 禁止对指针进行大于或小于的逻辑比较 [gjb8114-pointer-logical-compare]
    {
        sub=(*b)-(*a);
    }
    
    return sub;
}
/*
//遵循示例
int fsub(int *a,int *b);
int main(void)
{
    int sub=0;
    int a=1,b=2;
    sub=fsub(&a,&b);
    return (0);
}
int fsub(int *a,int *b)
{
    int sub=0;
    if((*a)>(*b))  //遵循1
    {
        sub=(*a)-(*b);
    }
    else if((*a)<(*b))  //遵循2
    {
        sub=(*b)-(*a);
    }
    else
    {
        sub=0;
    }
    return sub;
}
*/