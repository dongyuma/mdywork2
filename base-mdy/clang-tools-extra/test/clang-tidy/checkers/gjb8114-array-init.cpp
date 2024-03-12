// RUN: %check_clang_tidy %s gjb8114-array-init %t

//CSR-19	数组定义禁止没有显示的边界限定		4.1.1.19	gb5369-array-init	R-1-1-18	gjb8114-array-init	Rule 18.7	misrac2012-PointerArray
//CSR-19  GJB 8114-2013 R-1-1-18
//违背示例
int main_1(void)
{
    int array[]={0,1,2};  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 数组定义禁止没有显示的边界限定 [gjb8114-array-init]
    int i;
    int data=0;
    for(i=0;i<3;i++)
    {
        data=data+array[i];
    }
    return (0);
}

//遵循示例
int main_2(void)
{
    int array[3]={0,1,2};  //遵循1
    int i;
    int data=0;
    for(i=0;i<3;i++)
    {
        data=data+array[i];
    }
    return (0);
}
