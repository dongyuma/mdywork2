// RUN: %check_clang_tidy %s bz12-variable-name-analysis %t
/*
  极性问题：
  “赋值时右侧内容出错”大类错误，包括“赋值语句右侧为单一量赋值”和“赋值语句右侧为表达式”两种情况
*/

#define NUMZERO 0
#define NUMONE 1
#define NUMTWO 2
#define NUMTHREE 3
int main(void)
{
    //算法的实质延续了先前算法，首先提取全部的变量和等式关系，寻找每个变量中的关键字(如Fai、Psi等，并非每个变量都有关键字)，随后依次遍历每个等式，检查等式左右侧存在关键字的变量是否对应
    //对于数组元素，不对数组名称进行一致性检查，仅检查等价集合中存在的数组下标，若等式两侧中只有一侧的下标处于等价集合中也不会检查
    //算法仅对最外层元素进行一致性检查，例如现有变量名为a.b.c[0].d.f，算法仅对名称f进行一致性检查
    //目前已知问题
    //1.关键字提取算法不完善，如果有两个关键字分别为wxyk和wzek的话(关键字首字母和最后一个字母相同)，算法无法提取
    //2.暂时不支持++，--，+=，-=等情况
    //3.关键字提取算法可能提取出错，例如现有变量xyz_1_x,xyz_1_y,xyz_2_x,xyz_2_y,变量关键字有多个，但目前算法只会保留最后一个，从而导致误报或漏报的产生


    /*
      大类：赋值时右侧内容出错：赋值语句右侧为单一量赋值
      子类：RJZ-1-3-3-case1:左右侧含简单变量
    */
    int RJZ_polardemo_case_first_x=1,RJZ_polardemo_case_first_y=2,RJZ_polardemo_case_first_z=3;
    int RJZ_polardemo_case_first_F=1,RJZ_polardemo_case_first_P=2,RJZ_polardemo_case_first_G=3;

    RJZ_polardemo_case_first_x=RJZ_polardemo_case_first_F;
    RJZ_polardemo_case_first_y=RJZ_polardemo_case_first_P;
    RJZ_polardemo_case_first_z=RJZ_polardemo_case_first_G;  //遵循

    RJZ_polardemo_case_first_x=RJZ_polardemo_case_first_F;
    RJZ_polardemo_case_first_y=RJZ_polardemo_case_first_P;
    RJZ_polardemo_case_first_z=RJZ_polardemo_case_first_P;  //违背1-1
    // CHECK-MESSAGES: :[[@LINE-1]]:32: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_first_x=RJZ_polardemo_case_first_G;  //违背1-2
    // CHECK-MESSAGES: :[[@LINE-1]]:32: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    RJZ_polardemo_case_first_y=RJZ_polardemo_case_first_P;
    RJZ_polardemo_case_first_z=RJZ_polardemo_case_first_G;

    int RJZ_polardemo_case_first_Fai=RJZ_polardemo_case_first_x;
    int RJZ_polardemo_case_first_Psi=RJZ_polardemo_case_first_y;
    int RJZ_polardemo_case_first_Gam=RJZ_polardemo_case_first_x;  //违背1-3
    // CHECK-MESSAGES: :[[@LINE-1]]:38: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    int RJZ_polardemo_case_first_a=RJZ_polardemo_case_first_x;
    int RJZ_polardemo_case_first_b=RJZ_polardemo_case_first_y;
    int RJZ_polardemo_case_first_c=RJZ_polardemo_case_first_x;  //违背1-4
    // CHECK-MESSAGES: :[[@LINE-1]]:36: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_first_a=RJZ_polardemo_case_first_F;
    RJZ_polardemo_case_first_b=RJZ_polardemo_case_first_F;  //违背1-5
    // CHECK-MESSAGES: :[[@LINE-1]]:32: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    RJZ_polardemo_case_first_c=RJZ_polardemo_case_first_G;


    /*
      大类：赋值时右侧内容出错：赋值语句右侧为单一量赋值
      子类：RJZ-1-3-3-case2:左右侧含简单数组
    */
    int RJZ_polardemo_case_second_array_Fai[10];
    int RJZ_polardemo_case_second_array_Psi[10];
    int RJZ_polardemo_case_second_array_Gam[10];

    RJZ_polardemo_case_second_array_Fai[NUMZERO]=RJZ_polardemo_case_second_array_Fai[NUMZERO];
    RJZ_polardemo_case_second_array_Fai[NUMONE]=RJZ_polardemo_case_second_array_Fai[NUMONE];
    RJZ_polardemo_case_second_array_Fai[NUMTWO]=RJZ_polardemo_case_second_array_Fai[NUMTWO];

    RJZ_polardemo_case_second_array_Fai[NUMZERO]=RJZ_polardemo_case_second_array_Fai[NUMZERO];
    RJZ_polardemo_case_second_array_Fai[NUMONE]=RJZ_polardemo_case_second_array_Fai[NUMONE];
    RJZ_polardemo_case_second_array_Fai[NUMTWO]=RJZ_polardemo_case_second_array_Fai[NUMONE];  //违背2-1
    //CHECK-MESSAGES: :[[@LINE-1]]:49: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    
    RJZ_polardemo_case_second_array_Fai[0]=RJZ_polardemo_case_second_array_Fai[NUMZERO];
    RJZ_polardemo_case_second_array_Fai[1]=RJZ_polardemo_case_second_array_Fai[NUMONE];
    RJZ_polardemo_case_second_array_Fai[2]=RJZ_polardemo_case_second_array_Fai[NUMONE];  //违背2-2
    //CHECK-MESSAGES: :[[@LINE-1]]:44: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_second_array_Fai[NUMZERO]=RJZ_polardemo_case_second_array_Fai[0];
    RJZ_polardemo_case_second_array_Fai[NUMONE]=RJZ_polardemo_case_second_array_Fai[1];
    RJZ_polardemo_case_second_array_Fai[NUMTWO]=RJZ_polardemo_case_second_array_Fai[0];  //违背2-3
    //CHECK-MESSAGES: :[[@LINE-1]]:49: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_second_array_Fai[0]=RJZ_polardemo_case_second_array_Fai[0];
    RJZ_polardemo_case_second_array_Fai[1]=RJZ_polardemo_case_second_array_Fai[1];
    RJZ_polardemo_case_second_array_Fai[2]=RJZ_polardemo_case_second_array_Fai[0];  //违背2-4
    //CHECK-MESSAGES: :[[@LINE-1]]:44: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_second_array_Fai[0]=RJZ_polardemo_case_second_array_Fai[1];  //违背2-5
    //CHECK-MESSAGES: :[[@LINE-1]]:44: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    RJZ_polardemo_case_second_array_Fai[1]=RJZ_polardemo_case_second_array_Fai[1];
    RJZ_polardemo_case_second_array_Fai[2]=RJZ_polardemo_case_second_array_Fai[2];

    RJZ_polardemo_case_second_array_Psi[0]=RJZ_polardemo_case_second_array_Gam[0];  //数组名称是不会进行一致性检查的，数组只检查下标的一致性
    RJZ_polardemo_case_second_array_Psi[1]=RJZ_polardemo_case_second_array_Gam[1];
    RJZ_polardemo_case_second_array_Psi[2]=RJZ_polardemo_case_second_array_Gam[0];  //违背2-6
    //CHECK-MESSAGES: :[[@LINE-1]]:44: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_second_array_Psi[0]=RJZ_polardemo_case_second_array_Gam[7];  //不在等价集合中的数组下标不会进行检查(如本行的数组下标7)
    RJZ_polardemo_case_second_array_Psi[1]=RJZ_polardemo_case_second_array_Gam[1];
    RJZ_polardemo_case_second_array_Psi[2]=RJZ_polardemo_case_second_array_Gam[0];  //违背2-7
    //CHECK-MESSAGES: :[[@LINE-1]]:44: warning: 左值与右值不匹配 [bz12-variable-name-analysis]


    /*
      大类：赋值时右侧内容出错：赋值语句右侧为单一量赋值
      子类：RJZ-1-3-3-case3:左右侧含结构体普通元素
    */
    struct struct_third
    {
        int RJZ_polardemo_x;
        int RJZ_polardemo_y;
        int RJZ_polardemo_z;
        int RJZ_polardemo_Fai;
        int RJZ_polardemo_Psi;
        int RJZ_polardemo_Gam;
    };
    struct struct_third RJZ_polardemo_case_third_struct_Fai;
    int RJZ_polardemo_case_third_x=1,RJZ_polardemo_case_third_y=2,RJZ_polardemo_case_third_z=3;
    int RJZ_polardemo_case_third_array_Fai[10];

    RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Fai = RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_x;
    RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Psi = RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_y;
    RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Gam = RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_z;  //遵循

    RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Fai = RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_x;
    RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Psi = RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_y;
    RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Gam = RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_x;  //违背3-1
    //CHECK-MESSAGES: :[[@LINE-1]]:61: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_third_x=RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_x;
    RJZ_polardemo_case_third_y=RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_y;
    RJZ_polardemo_case_third_z=RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_x;  //违背3-2
    //CHECK-MESSAGES: :[[@LINE-1]]:32: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_third_array_Fai[0]=RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_x;
    RJZ_polardemo_case_third_array_Fai[1]=RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_x;  //违背3-3
    //CHECK-MESSAGES: :[[@LINE-1]]:43: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    RJZ_polardemo_case_third_array_Fai[2]=RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_z;

    RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_x=RJZ_polardemo_case_third_x;
    RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_y=RJZ_polardemo_case_third_y;
    RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_z=RJZ_polardemo_case_third_x;  //违背3-4
    //CHECK-MESSAGES: :[[@LINE-1]]:57: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_third_array_Fai[NUMZERO]=RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_x;
    RJZ_polardemo_case_third_array_Fai[NUMONE]=RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_x;  //违背3-5
    //CHECK-MESSAGES: :[[@LINE-1]]:48: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    RJZ_polardemo_case_third_array_Fai[NUMTWO]=RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_z;

    RJZ_polardemo_case_third_array_Fai[0]=RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_x;
    RJZ_polardemo_case_third_array_Fai[1]=RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_y;
    RJZ_polardemo_case_third_array_Fai[2]=RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_x;  //违背3-6
    //CHECK-MESSAGES: :[[@LINE-1]]:43: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_x=RJZ_polardemo_case_third_array_Fai[0];
    RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_y=RJZ_polardemo_case_third_array_Fai[1];
    RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_z=RJZ_polardemo_case_third_array_Fai[0];  //违背3-7
    //CHECK-MESSAGES: :[[@LINE-1]]:57: warning: 左值与右值不匹配 [bz12-variable-name-analysis]


    /*
      大类：赋值时右侧内容出错：赋值语句右侧为单一量赋值
      子类：RJZ-1-3-3-case4:左右侧含结构体数组元素
    */
    struct struct_fourth
    {
        int RJZ_polardemo_array_Fai[10];
        int RJZ_polardemo_array_Psi[10];
        int RJZ_polardemo_a;
        int RJZ_polardemo_b;
        int RJZ_polardemo_c;
    };
    struct struct_fourth RJZ_polardemo_case_fourth_struct_Fai;
    int RJZ_polardemo_case_fourth_x=1,RJZ_polardemo_case_fourth_y=2,RJZ_polardemo_case_fourth_z=3;

    RJZ_polardemo_case_fourth_x=RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[NUMZERO];
    RJZ_polardemo_case_fourth_y=RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[NUMONE];
    RJZ_polardemo_case_fourth_z=RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[NUMTWO];  //遵循

    RJZ_polardemo_case_fourth_x=RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[NUMZERO];
    RJZ_polardemo_case_fourth_y=RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[NUMONE];
    RJZ_polardemo_case_fourth_z=RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[NUMONE];  //违背4-1
    //CHECK-MESSAGES: :[[@LINE-1]]:33: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_fourth_x=RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[0];
    RJZ_polardemo_case_fourth_y=RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[1];
    RJZ_polardemo_case_fourth_z=RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[0];  //违背4-2
    //CHECK-MESSAGES: :[[@LINE-1]]:33: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[0]=RJZ_polardemo_case_fourth_x;
    RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[1]=RJZ_polardemo_case_fourth_y;
    RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[2]=RJZ_polardemo_case_fourth_x;  //违背4-3
    //CHECK-MESSAGES: :[[@LINE-1]]:69: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[0]=RJZ_polardemo_case_fourth_z;  //违背4-4
    //CHECK-MESSAGES: :[[@LINE-1]]:69: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[1]=RJZ_polardemo_case_fourth_y;
    RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[2]=RJZ_polardemo_case_fourth_z;
    
    RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[NUMZERO]=RJZ_polardemo_case_fourth_z;  //违背4-5
    //CHECK-MESSAGES: :[[@LINE-1]]:75: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[NUMONE]=RJZ_polardemo_case_fourth_y;
    RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[NUMTWO]=RJZ_polardemo_case_fourth_z;

    RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[7]=RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Psi[1];  //数组名不纳入检查，不在等价集合的数组下标也不列入检查
    RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[2]=RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Psi[2];
    RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[0]=RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Psi[2];  //违背4-6
    //CHECK-MESSAGES: :[[@LINE-1]]:69: warning: 左值与右值不匹配 [bz12-variable-name-analysis]


    /*
      大类：赋值时右侧内容出错：赋值语句右侧为单一量赋值
      子类：RJZ-1-3-3-case5:左右侧含嵌套结构体的普通元素
    */
    struct struct_fifth_Fai
    {
        int RJZ_polardemo_1;
        int RJZ_polardemo_2;
        int RJZ_polardemo_3;
        int RJZ_polardemo_F;
        int RJZ_polardemo_P;
        int RJZ_polardemo_G;
    };
    struct struct_fifth_Psi
    {
        struct struct_fifth_Fai struct_fifth_variable;
    };
    struct struct_fifth_Psi struct_fifth_Psi_variable;

    struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1=struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_F;
    struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_2=struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_P;
    struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_3=struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_G;  //遵循

    struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1=struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_F;
    struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_2=struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_P;
    struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_3=struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_F;  //违背5-1
    //CHECK-MESSAGES: :[[@LINE-1]]:69: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    int RJZ_polardemo_case_fifth_a=struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1;
    int RJZ_polardemo_case_fifth_b=struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_2;
    int RJZ_polardemo_case_fifth_c=struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1;  //违背5-2
    //CHECK-MESSAGES: :[[@LINE-1]]:36: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    int RJZ_polardemo_case_fifth_F=struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_F;
    int RJZ_polardemo_case_fifth_P=struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_P;
    int RJZ_polardemo_case_fifth_G=struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_P;  //违背5-3
    //CHECK-MESSAGES: :[[@LINE-1]]:36: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_fifth_F=struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_F;
    RJZ_polardemo_case_fifth_P=struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_P;
    RJZ_polardemo_case_fifth_G=struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_P;  //违背5-4
    //CHECK-MESSAGES: :[[@LINE-1]]:32: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_F=RJZ_polardemo_case_fifth_F;
    struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_P=RJZ_polardemo_case_fifth_P;
    struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_G=RJZ_polardemo_case_fifth_F;  //违背5-5
    //CHECK-MESSAGES: :[[@LINE-1]]:69: warning: 左值与右值不匹配 [bz12-variable-name-analysis]


    /*
      大类：赋值时右侧内容出错：赋值语句右侧为单一量赋值
      子类：RJZ-1-3-3-case6:左右侧含嵌套结构体的数组元素
    */
    struct struct_sixth_Fai
    {
        int RJZ_polardemo_case_sixth_array_Fai[10];
    };
    struct struct_sixth_Psi
    {
        struct struct_sixth_Fai struct_sixth_Fai_variable;
    };
    struct struct_sixth_Psi struct_sixth_Psi_variable;
    int RJZ_polardemo_case_sixth_a=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[0];
    int RJZ_polardemo_case_sixth_b=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[1];
    int RJZ_polardemo_case_sixth_c=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[2];  //遵循

    int RJZ_polardemo_case_sixth_x=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMZERO];
    int RJZ_polardemo_case_sixth_y=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMONE];
    int RJZ_polardemo_case_sixth_z=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMZERO];  //违背6-1
    //CHECK-MESSAGES: :[[@LINE-1]]:36: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMZERO]=RJZ_polardemo_case_sixth_x;
    struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMONE]=RJZ_polardemo_case_sixth_y;
    struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMTWO]=RJZ_polardemo_case_sixth_y;  //违背6-2
    //CHECK-MESSAGES: :[[@LINE-1]]:100: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMZERO]=RJZ_polardemo_case_sixth_x;
    struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMONE]=RJZ_polardemo_case_sixth_x;  //违背6-3
    //CHECK-MESSAGES: :[[@LINE-1]]:100: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMTWO]=RJZ_polardemo_case_sixth_z;

    struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMZERO];
    struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_2=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMONE];
    struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_3=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMZERO];  //违背6-4
    //CHECK-MESSAGES: :[[@LINE-1]]:69: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMZERO]=struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1;
    struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMONE]=struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_2;
    struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMTWO]=struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1;  //违背6-5
    //CHECK-MESSAGES: :[[@LINE-1]]:100: warning: 左值与右值不匹配 [bz12-variable-name-analysis]


    /*
      大类：赋值时右侧内容出错：赋值语句右侧为单一量赋值
      子类：RJZ-1-3-3-case7:左右侧含主结构体为数组型，嵌套普通子结构体的元素（元素为普通变量或数组元素）
    */
     struct struct_seventh_Fai
    {
        int RJZ_polardemo_1;
        int RJZ_polardemo_2;
        int RJZ_polardemo_3;
        int RJZ_polardemo_F;
        int RJZ_polardemo_P;
        int RJZ_polardemo_G;
        int RJZ_polardemo_array_Fai[10];
    };
    struct struct_seventh_Psi
    {
        struct struct_seventh_Fai struct_seventh_Fai_variable;
    };
    struct struct_seventh_Psi struct_seventh_Psi_array[10];

    int RJZ_polardemo_case_seventh_F=struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_1;
    int RJZ_polardemo_case_seventh_P=struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_2;
    int RJZ_polardemo_case_seventh_G=struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_3;  //遵循

    RJZ_polardemo_case_seventh_F=struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_1;
    RJZ_polardemo_case_seventh_P=struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_3;  //违背7-1
    //CHECK-MESSAGES: :[[@LINE-1]]:34: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    RJZ_polardemo_case_seventh_G=struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_3;

    struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_1=RJZ_polardemo_case_seventh_F;
    struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_2=RJZ_polardemo_case_seventh_P;
    struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_3=RJZ_polardemo_case_seventh_F;  //违背7-2
    //CHECK-MESSAGES: :[[@LINE-1]]:77: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[NUMZERO]=RJZ_polardemo_case_seventh_G;  //违背7-3
    //CHECK-MESSAGES: :[[@LINE-1]]:94: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[NUMONE]=RJZ_polardemo_case_seventh_P;
    struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[NUMTWO]=RJZ_polardemo_case_seventh_G;

    int RJZ_polardemo_case_seventh_x=struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[0];
    int RJZ_polardemo_case_seventh_y=struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[1];
    int RJZ_polardemo_case_seventh_z=struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[1];  //违背7-4
    //CHECK-MESSAGES: :[[@LINE-1]]:38: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[0]=RJZ_polardemo_case_seventh_x;
    struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[1]=RJZ_polardemo_case_seventh_y;
    struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[2]=RJZ_polardemo_case_seventh_x;  //违背7-5
    //CHECK-MESSAGES: :[[@LINE-1]]:88: warning: 左值与右值不匹配 [bz12-variable-name-analysis]


    /*
      大类：赋值时右侧内容出错：赋值语句右侧为单一量赋值
      子类：RJZ-1-3-3-case8:左右侧含主结构体，嵌套数组型子结构体的元素（元素为普通变量或数组元素）
    */
    struct struct_eighth_Fai
    {
        int RJZ_polardemo_a;
        int RJZ_polardemo_b;
        int RJZ_polardemo_c;
        int RJZ_polardemo_Fai;
        int RJZ_polardemo_Psi;
        int RJZ_polardemo_Gam;
        int RJZ_polardemo_array_Fai[10];
    };
    struct struct_eighth_Psi
    {
        struct struct_eighth_Fai struct_eighth_Fai_array[10];
    };
    struct struct_eighth_Psi struct_eighth_Psi_variable;

    int RJZ_polardemo_case_eighth_x=struct_eighth_Psi_variable.struct_eighth_Fai_array[NUMZERO].RJZ_polardemo_a;
    int RJZ_polardemo_case_eighth_y=struct_eighth_Psi_variable.struct_eighth_Fai_array[NUMZERO].RJZ_polardemo_b;
    int RJZ_polardemo_case_eighth_z=struct_eighth_Psi_variable.struct_eighth_Fai_array[NUMZERO].RJZ_polardemo_c;  //遵循

    RJZ_polardemo_case_eighth_x=struct_eighth_Psi_variable.struct_eighth_Fai_array[NUMZERO].RJZ_polardemo_a;
    RJZ_polardemo_case_eighth_y=struct_eighth_Psi_variable.struct_eighth_Fai_array[NUMZERO].RJZ_polardemo_b;
    RJZ_polardemo_case_eighth_z=struct_eighth_Psi_variable.struct_eighth_Fai_array[NUMZERO].RJZ_polardemo_a;  //违背8-1
    //CHECK-MESSAGES: :[[@LINE-1]]:33: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_eighth_x=struct_eighth_Psi_variable.struct_eighth_Fai_array[NUMZERO].RJZ_polardemo_array_Fai[NUMZERO];
    RJZ_polardemo_case_eighth_y=struct_eighth_Psi_variable.struct_eighth_Fai_array[NUMZERO].RJZ_polardemo_array_Fai[NUMONE];
    RJZ_polardemo_case_eighth_z=struct_eighth_Psi_variable.struct_eighth_Fai_array[NUMZERO].RJZ_polardemo_array_Fai[NUMONE];  //违背8-2
    //CHECK-MESSAGES: :[[@LINE-1]]:33: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_eighth_Psi_variable.struct_eighth_Fai_array[NUMZERO].RJZ_polardemo_array_Fai[NUMZERO]=RJZ_polardemo_case_eighth_x;
    struct_eighth_Psi_variable.struct_eighth_Fai_array[NUMZERO].RJZ_polardemo_array_Fai[NUMONE]=RJZ_polardemo_case_eighth_x;  //违背8-3
    //CHECK-MESSAGES: :[[@LINE-1]]:97: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    struct_eighth_Psi_variable.struct_eighth_Fai_array[NUMZERO].RJZ_polardemo_array_Fai[NUMTWO]=RJZ_polardemo_case_eighth_z;

    struct_eighth_Psi_variable.struct_eighth_Fai_array[NUMZERO].RJZ_polardemo_array_Fai[NUMZERO]=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMZERO];
    struct_eighth_Psi_variable.struct_eighth_Fai_array[NUMZERO].RJZ_polardemo_array_Fai[NUMONE]=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMONE];
    struct_eighth_Psi_variable.struct_eighth_Fai_array[NUMZERO].RJZ_polardemo_array_Fai[NUMTWO]=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMZERO];  //违背8-4
    //CHECK-MESSAGES: :[[@LINE-1]]:97: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_eighth_Psi_variable.struct_eighth_Fai_array[NUMZERO].RJZ_polardemo_a=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMZERO];
    struct_eighth_Psi_variable.struct_eighth_Fai_array[NUMZERO].RJZ_polardemo_b=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMONE];
    struct_eighth_Psi_variable.struct_eighth_Fai_array[NUMZERO].RJZ_polardemo_c=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMZERO];  //违背8-5
    //CHECK-MESSAGES: :[[@LINE-1]]:81: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    

    /*
      大类：赋值时右侧内容出错：赋值语句右侧为单一量赋值
      子类：RJZ-1-3-3-case9:左右侧含主结构体为数组型，嵌套数组型子结构体的元素（元素为普通变量或数组元素）
    */
    struct struct_ninth_Fai
    {
        int RJZ_polardemo_x;
        int RJZ_polardemo_y;
        int RJZ_polardemo_z;
        int RJZ_polardemo_F;
        int RJZ_polardemo_P;
        int RJZ_polardemo_G;
        int RJZ_polardemo_array_Fai[10];
    };
    struct struct_ninth_Psi
    {
        struct struct_ninth_Fai struct_ninth_Fai_array[10];
    };
    struct struct_ninth_Psi struct_ninth_Psi_array[10];

    int RJZ_polardemo_case_ninth_x=struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_F;
    int RJZ_polardemo_case_ninth_y=struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_P;
    int RJZ_polardemo_case_ninth_z=struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_G;  //遵循

    RJZ_polardemo_case_ninth_x=struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_F;
    RJZ_polardemo_case_ninth_y=struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_F;  //违背9-1
    //CHECK-MESSAGES: :[[@LINE-1]]:32: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    RJZ_polardemo_case_ninth_z=struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_G;

    RJZ_polardemo_case_ninth_x=struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[NUMZERO];
    RJZ_polardemo_case_ninth_y=struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[NUMONE];
    RJZ_polardemo_case_ninth_z=struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[NUMONE];  //违背9-2
    //CHECK-MESSAGES: :[[@LINE-1]]:32: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_F=struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_x;
    struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_P=struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_y;
    struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_G=struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_x;  //违背9-3
    //CHECK-MESSAGES: :[[@LINE-1]]:73: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[0]=struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_array_Fai[0];
    struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[1]=struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_array_Fai[1];
    struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[2]=struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_array_Fai[0];  //违背9-4
    //CHECK-MESSAGES: :[[@LINE-1]]:84: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_F=RJZ_polardemo_case_ninth_x;
    struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_P=RJZ_polardemo_case_ninth_y;
    struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_G=RJZ_polardemo_case_ninth_x;  //违背9-5
    //CHECK-MESSAGES: :[[@LINE-1]]:73: warning: 左值与右值不匹配 [bz12-variable-name-analysis]


    /*
      大类：赋值时右侧内容出错：赋值语句右侧为表达式
      子类：RJZ-1-3-4-case1:左右侧含简单变量
    */
    int RJZ_polardemo_case_tenth_x=1,RJZ_polardemo_case_tenth_y=2,RJZ_polardemo_case_tenth_z=3;
    int RJZ_polardemo_case_tenth_a=1,RJZ_polardemo_case_tenth_b=2,RJZ_polardemo_case_tenth_c=3;
    int RJZ_polardemo_case_tenth_F=1,RJZ_polardemo_case_tenth_P=2,RJZ_polardemo_case_tenth_G=3;

    RJZ_polardemo_case_tenth_x=(RJZ_polardemo_case_tenth_a*RJZ_polardemo_case_tenth_F)%2+5;
    RJZ_polardemo_case_tenth_y=(RJZ_polardemo_case_tenth_b*RJZ_polardemo_case_tenth_P)%2+5;
    RJZ_polardemo_case_tenth_z=(RJZ_polardemo_case_tenth_c*RJZ_polardemo_case_tenth_G)%2+5;  //遵循

    RJZ_polardemo_case_tenth_x=(RJZ_polardemo_case_tenth_a*RJZ_polardemo_case_tenth_F)%2+5;
    RJZ_polardemo_case_tenth_y=(RJZ_polardemo_case_tenth_b*RJZ_polardemo_case_tenth_P)%2+5;
    RJZ_polardemo_case_tenth_z=(RJZ_polardemo_case_tenth_a*RJZ_polardemo_case_tenth_G)%2+5;  //违背1-1
    //CHECK-MESSAGES: :[[@LINE-1]]:33: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_tenth_x=(RJZ_polardemo_case_tenth_a*RJZ_polardemo_case_tenth_F)%2+5;
    RJZ_polardemo_case_tenth_y=(RJZ_polardemo_case_tenth_b*RJZ_polardemo_case_tenth_P)%2+5;
    RJZ_polardemo_case_tenth_z=(RJZ_polardemo_case_tenth_a*RJZ_polardemo_case_tenth_P)%2+5;  //违背1-2
    //CHECK-MESSAGES: :[[@LINE-1]]:33: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    //CHECK-MESSAGES: :[[@LINE-2]]:60: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    int RJZ_polardemo_case_tenth_Fai=RJZ_polardemo_case_tenth_a+RJZ_polardemo_case_tenth_x/2;
    int RJZ_polardemo_case_tenth_Psi=RJZ_polardemo_case_tenth_b+RJZ_polardemo_case_tenth_x/2;  //违背1-3
    //CHECK-MESSAGES: :[[@LINE-1]]:65: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    int RJZ_polardemo_case_tenth_Gam=RJZ_polardemo_case_tenth_c+RJZ_polardemo_case_tenth_z/2;

    RJZ_polardemo_case_tenth_Fai=RJZ_polardemo_case_tenth_x*RJZ_polardemo_case_tenth_a-RJZ_polardemo_case_tenth_F;
    RJZ_polardemo_case_tenth_Psi=RJZ_polardemo_case_tenth_y*RJZ_polardemo_case_tenth_b-RJZ_polardemo_case_tenth_P;
    RJZ_polardemo_case_tenth_Gam=RJZ_polardemo_case_tenth_z*RJZ_polardemo_case_tenth_c-RJZ_polardemo_case_tenth_F;  //违背1-4
    //CHECK-MESSAGES: :[[@LINE-1]]:88: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_tenth_Fai=(((RJZ_polardemo_case_tenth_x)))*RJZ_polardemo_case_tenth_a-RJZ_polardemo_case_tenth_F;
    RJZ_polardemo_case_tenth_Psi=(((RJZ_polardemo_case_tenth_y)))*RJZ_polardemo_case_tenth_b-RJZ_polardemo_case_tenth_P;
    RJZ_polardemo_case_tenth_Gam=(((RJZ_polardemo_case_tenth_y)))*RJZ_polardemo_case_tenth_c-RJZ_polardemo_case_tenth_G;  //违背1-5
    //CHECK-MESSAGES: :[[@LINE-1]]:37: warning: 左值与右值不匹配 [bz12-variable-name-analysis]


    /*
      大类：赋值时右侧内容出错：赋值语句右侧为表达式
      子类：RJZ-1-3-4-case2:左右侧含简单数组
    */
    int RJZ_polardemo_case_eleventh_array_First[10];
    int RJZ_polardemo_case_eleventh_array_Second[10];

    int RJZ_polardemo_case_eleventh_Fai=(RJZ_polardemo_case_eleventh_array_First[0]+RJZ_polardemo_case_eleventh_array_Second[0])*3+2;
    int RJZ_polardemo_case_eleventh_Psi=(RJZ_polardemo_case_eleventh_array_First[1]+RJZ_polardemo_case_eleventh_array_Second[1])*3+2;
    int RJZ_polardemo_case_eleventh_Gam=(RJZ_polardemo_case_eleventh_array_First[2]+RJZ_polardemo_case_eleventh_array_Second[2])*3+2;  //遵循

    int RJZ_polardemo_case_eleventh_F=(RJZ_polardemo_case_eleventh_array_First[0]+RJZ_polardemo_case_eleventh_array_Second[0])*3+2;
    int RJZ_polardemo_case_eleventh_P=(RJZ_polardemo_case_eleventh_array_First[1]+RJZ_polardemo_case_eleventh_array_Second[1])*3+2;
    int RJZ_polardemo_case_eleventh_G=(RJZ_polardemo_case_eleventh_array_First[1]+RJZ_polardemo_case_eleventh_array_Second[1])*3+2;  //违背2-1
    //CHECK-MESSAGES: :[[@LINE-1]]:40: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    //CHECK-MESSAGES: :[[@LINE-2]]:83: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_eleventh_F=(RJZ_polardemo_case_eleventh_array_First[NUMZERO]+RJZ_polardemo_case_eleventh_array_Second[NUMZERO])*3+2;
    RJZ_polardemo_case_eleventh_P=(RJZ_polardemo_case_eleventh_array_First[NUMONE]+RJZ_polardemo_case_eleventh_array_Second[NUMONE])*3+2;
    RJZ_polardemo_case_eleventh_G=(RJZ_polardemo_case_eleventh_array_First[NUMTWO]+RJZ_polardemo_case_eleventh_array_Second[NUMONE])*3+2;  //违背2-2
    //CHECK-MESSAGES: :[[@LINE-1]]:84: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_eleventh_array_First[NUMZERO]=RJZ_polardemo_case_eleventh_F*RJZ_polardemo_case_eleventh_Fai%2*3;
    RJZ_polardemo_case_eleventh_array_First[NUMONE]=RJZ_polardemo_case_eleventh_F*RJZ_polardemo_case_eleventh_Gam%2*3;  //违背2-3
    //CHECK-MESSAGES: :[[@LINE-1]]:53: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    //CHECK-MESSAGES: :[[@LINE-2]]:83: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    RJZ_polardemo_case_eleventh_array_First[NUMTWO]=RJZ_polardemo_case_eleventh_G*RJZ_polardemo_case_eleventh_Gam%2*3;

    RJZ_polardemo_case_eleventh_array_First[0]=((RJZ_polardemo_case_eleventh_F+2)*3/2)%3;
    RJZ_polardemo_case_eleventh_array_First[1]=((RJZ_polardemo_case_eleventh_P+2)*3/2)%3;
    RJZ_polardemo_case_eleventh_array_First[2]=((RJZ_polardemo_case_eleventh_F+2)*3/2)%3;  //违背2-4
    //CHECK-MESSAGES: :[[@LINE-1]]:50: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_eleventh_array_First[0]=((RJZ_polardemo_case_eleventh_array_Second[0]+2)*3/2)%3;
    RJZ_polardemo_case_eleventh_array_First[1]=((RJZ_polardemo_case_eleventh_array_Second[1]+2)*3/2)%3;
    RJZ_polardemo_case_eleventh_array_First[2]=((RJZ_polardemo_case_eleventh_array_Second[0]+2)*3/2)%3;  //违背2-5
    //CHECK-MESSAGES: :[[@LINE-1]]:50: warning: 左值与右值不匹配 [bz12-variable-name-analysis]


    /*
      大类：赋值时右侧内容出错：赋值语句右侧为表达式
      子类：RJZ-1-3-4-case3:左右侧含结构体普通元素
    */
    struct struct_twelfth
    {
        int RJZ_polardemo_1;
        int RJZ_polardemo_2;
        int RJZ_polardemo_3;
        int RJZ_polardemo_a;
        int RJZ_polardemo_b;
        int RJZ_polardemo_c;
    };
    struct struct_twelfth struct_twelfth_variable;

    int RJZ_polardemo_case_twelfth_F=(struct_twelfth_variable.RJZ_polardemo_1+struct_twelfth_variable.RJZ_polardemo_a)*3/2;
    int RJZ_polardemo_case_twelfth_P=(struct_twelfth_variable.RJZ_polardemo_2+struct_twelfth_variable.RJZ_polardemo_b)*3/2;
    int RJZ_polardemo_case_twelfth_G=(struct_twelfth_variable.RJZ_polardemo_3+struct_twelfth_variable.RJZ_polardemo_c)*3/2;  //遵循

    RJZ_polardemo_case_twelfth_F=(struct_twelfth_variable.RJZ_polardemo_1/struct_twelfth_variable.RJZ_polardemo_a)*3/2;
    RJZ_polardemo_case_twelfth_P=(struct_twelfth_variable.RJZ_polardemo_3/struct_twelfth_variable.RJZ_polardemo_b)*3/2;  //违背3-1
    //CHECK-MESSAGES: :[[@LINE-1]]:35: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    RJZ_polardemo_case_twelfth_G=(struct_twelfth_variable.RJZ_polardemo_3/struct_twelfth_variable.RJZ_polardemo_c)*3/2;

    struct_twelfth_variable.RJZ_polardemo_1=RJZ_polardemo_case_twelfth_F-struct_twelfth_variable.RJZ_polardemo_a;
    struct_twelfth_variable.RJZ_polardemo_2=RJZ_polardemo_case_twelfth_P-struct_twelfth_variable.RJZ_polardemo_a;  //违背3-2
    //CHECK-MESSAGES: :[[@LINE-1]]:74: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    struct_twelfth_variable.RJZ_polardemo_3=RJZ_polardemo_case_twelfth_G-struct_twelfth_variable.RJZ_polardemo_c;

    int RJZ_polardemo_case_twelfth_a=(struct_twelfth_variable.RJZ_polardemo_1*struct_twelfth_variable.RJZ_polardemo_a)*3/2%NUMONE;
    int RJZ_polardemo_case_twelfth_b=(struct_twelfth_variable.RJZ_polardemo_2*struct_twelfth_variable.RJZ_polardemo_b)*3/2%NUMTWO;
    int RJZ_polardemo_case_twelfth_c=(struct_twelfth_variable.RJZ_polardemo_3*struct_twelfth_variable.RJZ_polardemo_a)*3/2%NUMTHREE;  //违背3-3
    //CHECK-MESSAGES: :[[@LINE-1]]:79: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_twelfth_a=(struct_twelfth_variable.RJZ_polardemo_1*RJZ_polardemo_case_twelfth_F)*3/2%NUMONE;
    RJZ_polardemo_case_twelfth_b=(struct_twelfth_variable.RJZ_polardemo_2*RJZ_polardemo_case_twelfth_P)*3/2%NUMTWO;
    RJZ_polardemo_case_twelfth_c=(struct_twelfth_variable.RJZ_polardemo_1*RJZ_polardemo_case_twelfth_G)*3/2%NUMTHREE;  //违背3-4
    //CHECK-MESSAGES: :[[@LINE-1]]:35: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_twelfth_variable.RJZ_polardemo_1=3+4*2/2-struct_twelfth_variable.RJZ_polardemo_2;  //违背3-5
    //CHECK-MESSAGES: :[[@LINE-1]]:53: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    struct_twelfth_variable.RJZ_polardemo_2=3+4*2/2-struct_twelfth_variable.RJZ_polardemo_2;
    struct_twelfth_variable.RJZ_polardemo_3=3+4*2/2-struct_twelfth_variable.RJZ_polardemo_3;


    /*
      大类：赋值时右侧内容出错：赋值语句右侧为表达式
      子类：RJZ-1-3-4-case4:左右侧含结构体数组元素
    */
    struct struct_thirteen
    {
        int RJZ_polardemo_array_Fai[10];
    };
    struct struct_thirteen struct_thirteen_variable;
    int RJZ_polardemo_case_thirteen_array_Fai[10];
    
    int RJZ_polardemo_case_thirteen_F=(struct_thirteen_variable.RJZ_polardemo_array_Fai[0]+NUMONE)*3/2;
    int RJZ_polardemo_case_thirteen_P=(struct_thirteen_variable.RJZ_polardemo_array_Fai[1]+NUMONE)*3/2;
    int RJZ_polardemo_case_thirteen_G=(struct_thirteen_variable.RJZ_polardemo_array_Fai[2]+NUMONE)*3/2;  //遵循

    RJZ_polardemo_case_thirteen_F=(struct_thirteen_variable.RJZ_polardemo_array_Fai[0]+NUMONE)*3/2;
    RJZ_polardemo_case_thirteen_P=(struct_thirteen_variable.RJZ_polardemo_array_Fai[1]+NUMONE)*3/2;
    RJZ_polardemo_case_thirteen_G=(struct_thirteen_variable.RJZ_polardemo_array_Fai[0]+NUMONE)*3/2;  //违背4-1
    //CHECK-MESSAGES: :[[@LINE-1]]:36: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_thirteen_F=(struct_thirteen_variable.RJZ_polardemo_array_Fai[0]+RJZ_polardemo_case_thirteen_F)*3/2+1;
    RJZ_polardemo_case_thirteen_P=(struct_thirteen_variable.RJZ_polardemo_array_Fai[1]+RJZ_polardemo_case_thirteen_P)*3/2+2;
    RJZ_polardemo_case_thirteen_G=(struct_thirteen_variable.RJZ_polardemo_array_Fai[2]+RJZ_polardemo_case_thirteen_F)*3/2+3;  //违背4-2
    //CHECK-MESSAGES: :[[@LINE-1]]:88: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_thirteen_variable.RJZ_polardemo_array_Fai[0]=RJZ_polardemo_case_thirteen_array_Fai[0]-RJZ_polardemo_case_thirteen_F/2;
    struct_thirteen_variable.RJZ_polardemo_array_Fai[1]=RJZ_polardemo_case_thirteen_array_Fai[1]-RJZ_polardemo_case_thirteen_P/2;
    struct_thirteen_variable.RJZ_polardemo_array_Fai[2]=RJZ_polardemo_case_thirteen_array_Fai[0]-RJZ_polardemo_case_thirteen_G/2;  //违背4-3
    //CHECK-MESSAGES: :[[@LINE-1]]:57: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_thirteen_variable.RJZ_polardemo_array_Fai[NUMZERO]=RJZ_polardemo_case_thirteen_array_Fai[0]/3;
    struct_thirteen_variable.RJZ_polardemo_array_Fai[NUMONE]=RJZ_polardemo_case_thirteen_array_Fai[1]/3;
    struct_thirteen_variable.RJZ_polardemo_array_Fai[NUMTWO]=RJZ_polardemo_case_thirteen_array_Fai[0]/3;  //违背4-4
    //CHECK-MESSAGES: :[[@LINE-1]]:62: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_thirteen_F=struct_thirteen_variable.RJZ_polardemo_array_Fai[NUMZERO]/(RJZ_polardemo_case_thirteen_array_Fai[NUMZERO]);
    RJZ_polardemo_case_thirteen_P=struct_thirteen_variable.RJZ_polardemo_array_Fai[NUMONE]/(RJZ_polardemo_case_thirteen_array_Fai[NUMONE]);
    RJZ_polardemo_case_thirteen_G=struct_thirteen_variable.RJZ_polardemo_array_Fai[NUMTWO]/(RJZ_polardemo_case_thirteen_array_Fai[NUMZERO]);  //违背4-5
    //CHECK-MESSAGES: :[[@LINE-1]]:93: warning: 左值与右值不匹配 [bz12-variable-name-analysis]


    /*
      大类：赋值时右侧内容出错：赋值语句右侧为表达式
      子类：RJZ-1-3-4-case5:左右侧含嵌套结构体的普通元素
    */
    struct struct_fourteen_Fai
    {
        int RJZ_polardemo_x;
        int RJZ_polardemo_y;
        int RJZ_polardemo_z;
        int RJZ_polardemo_Fai;
        int RJZ_polardemo_Psi;
        int RJZ_polardemo_Gam;
    };
    struct struct_fourteen_Psi
    {
        struct struct_fourteen_Fai struct_fourteen_Fai_variable;
    };
    struct struct_fourteen_Psi struct_fourteen_Psi_variable;
    int RJZ_polardemo_case_fourteen_array_Fai[10];
    
    int RJZ_polardemo_case_fourteen_F=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Fai*struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_x+1;
    int RJZ_polardemo_case_fourteen_P=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Psi*struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_y+1;
    int RJZ_polardemo_case_fourteen_G=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Gam*struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_z+1;  //遵循

    RJZ_polardemo_case_fourteen_F=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Fai*struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_x+1;
    RJZ_polardemo_case_fourteen_P=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Psi*struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_x+1;  //违背5-1
    //CHECK-MESSAGES: :[[@LINE-1]]:111: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    RJZ_polardemo_case_fourteen_G=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Gam*struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_z+1;

    struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Fai=RJZ_polardemo_case_fourteen_array_Fai[0]*struct_thirteen_variable.RJZ_polardemo_array_Fai[0];
    struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Psi=RJZ_polardemo_case_fourteen_array_Fai[1]*struct_thirteen_variable.RJZ_polardemo_array_Fai[1];
    struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Gam=RJZ_polardemo_case_fourteen_array_Fai[2]*struct_thirteen_variable.RJZ_polardemo_array_Fai[0];  //违背5-2
    //CHECK-MESSAGES: :[[@LINE-1]]:122: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    RJZ_polardemo_case_fourteen_F=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Fai*RJZ_polardemo_case_fourteen_array_Fai[0]/3+2;
    RJZ_polardemo_case_fourteen_P=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Psi*RJZ_polardemo_case_fourteen_array_Fai[0]/3+2;  //违背5-3
    //CHECK-MESSAGES: :[[@LINE-1]]:111: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    RJZ_polardemo_case_fourteen_G=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Gam*RJZ_polardemo_case_fourteen_array_Fai[2]/3+2;

    int RJZ_polardemo_case_fourteen_a=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Fai%2+3*4-1;
    int RJZ_polardemo_case_fourteen_b=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Psi%2+3*4-1;
    int RJZ_polardemo_case_fourteen_c=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Fai%2+3*4-1;  //违背5-4
    //CHECK-MESSAGES: :[[@LINE-1]]:39: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Fai=RJZ_polardemo_case_fourteen_a-RJZ_polardemo_case_fourteen_F+2;
    struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Psi=RJZ_polardemo_case_fourteen_b-RJZ_polardemo_case_fourteen_P+2;
    struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Gam=RJZ_polardemo_case_fourteen_c-RJZ_polardemo_case_fourteen_F+2;  //违背5-5
    //CHECK-MESSAGES: :[[@LINE-1]]:111: warning: 左值与右值不匹配 [bz12-variable-name-analysis]


    /*
      大类：赋值时右侧内容出错：赋值语句右侧为表达式
      子类：RJZ-1-3-4-case6:左右侧含嵌套结构体的数组元素
    */
    struct struct_fifteen_Fai
    {
        int RJZ_polardemo_array_Fai[20];
    };
    struct struct_fifteen_Psi
    {
        struct struct_fifteen_Fai struct_fifteen_Fai_variable;
    };
    struct struct_fifteen_Psi struct_fifteen_Psi_variable;
    
    int RJZ_polardemo_case_fifteen_F=struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[0]*1;
    int RJZ_polardemo_case_fifteen_P=struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[1]*2;
    int RJZ_polardemo_case_fifteen_G=struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[2]*3;  //遵循

    RJZ_polardemo_case_fifteen_F=struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[NUMZERO]*1;
    RJZ_polardemo_case_fifteen_P=struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[NUMONE]*2;
    RJZ_polardemo_case_fifteen_G=struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[NUMZERO]*3;  //违背6-1
    //CHECK-MESSAGES: :[[@LINE-1]]:34: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[NUMZERO]=RJZ_polardemo_case_fifteen_F*RJZ_polardemo_case_fifteen_G;  //违背6-2
    //CHECK-MESSAGES: :[[@LINE-1]]:123: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[NUMONE]=RJZ_polardemo_case_fifteen_P*RJZ_polardemo_case_fifteen_P;
    struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[NUMTWO]=RJZ_polardemo_case_fifteen_G*RJZ_polardemo_case_fifteen_G;

    struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[NUMZERO]=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Fai*RJZ_polardemo_case_fifteen_F;
    struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[NUMONE]=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Psi*RJZ_polardemo_case_fifteen_P;
    struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[NUMTWO]=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Gam*RJZ_polardemo_case_fifteen_P;  //违背6-3
    //CHECK-MESSAGES: :[[@LINE-1]]:169: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[NUMZERO]=struct_twelfth_variable.RJZ_polardemo_1*RJZ_polardemo_case_fifteen_F/2;
    struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[NUMONE]=struct_twelfth_variable.RJZ_polardemo_2*RJZ_polardemo_case_fifteen_P%2;
    struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[NUMTWO]=struct_twelfth_variable.RJZ_polardemo_3*RJZ_polardemo_case_fifteen_F*2;  //违背6-4
    //CHECK-MESSAGES: :[[@LINE-1]]:133: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[0]=struct_twelfth_variable.RJZ_polardemo_1/2+RJZ_polardemo_case_fifteen_F;
    struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[1]=struct_twelfth_variable.RJZ_polardemo_2%2-RJZ_polardemo_case_fifteen_P;
    struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[2]=struct_twelfth_variable.RJZ_polardemo_1*2-RJZ_polardemo_case_fifteen_G;  //违背6-5
    //CHECK-MESSAGES: :[[@LINE-1]]:88: warning: 左值与右值不匹配 [bz12-variable-name-analysis]


    /*
      大类：赋值时右侧内容出错：赋值语句右侧为表达式
      子类：RJZ-1-3-4-case7:左右侧含主结构体为数组型，嵌套普通子结构体的元素（元素为普通变量或数组元素）
    */
    struct struct_sixteen_Fai
    {
        int RJZ_polardemo_x;
        int RJZ_polardemo_y;
        int RJZ_polardemo_z;
        int RJZ_polardemo_Fai;
        int RJZ_polardemo_Psi;
        int RJZ_polardemo_Gam;
        int RJZ_polardemo_array_Fai[100];
    };
    struct struct_sixteen_Psi
    {
        struct struct_sixteen_Fai struct_sixteen_Fai_variable;
    };
    struct struct_sixteen_Psi struct_sixteen_Psi_array[10];
    
    int RJZ_polardemo_case_sixteen_F=struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_Fai+8;
    int RJZ_polardemo_case_sixteen_P=struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_Psi+8;
    int RJZ_polardemo_case_sixteen_G=struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_Gam+8;  //遵循

    RJZ_polardemo_case_sixteen_F=struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_Fai*struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_array_Fai[NUMZERO];
    RJZ_polardemo_case_sixteen_P=struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_Psi*struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_array_Fai[NUMONE];
    RJZ_polardemo_case_sixteen_G=struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_Gam*struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_array_Fai[NUMZERO];  //违背7-1
    //CHECK-MESSAGES: :[[@LINE-1]]:108: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_array_Fai[NUMZERO]=struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[NUMZERO]-RJZ_polardemo_case_sixteen_F;
    struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_array_Fai[NUMONE]=struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[NUMONE]-RJZ_polardemo_case_sixteen_P;
    struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_array_Fai[NUMTWO]=struct_fifteen_Psi_variable.struct_fifteen_Fai_variable.RJZ_polardemo_array_Fai[NUMTWO]-RJZ_polardemo_case_sixteen_F;  //违背7-2
    //CHECK-MESSAGES: :[[@LINE-1]]:181: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_Fai=struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_array_Fai[NUMZERO]*2/3+1;
    struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_Psi=struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_array_Fai[NUMONE]*2/3+1;
    struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_Gam=struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_array_Fai[NUMZERO]*2/3+1;  //违背7-3
    //CHECK-MESSAGES: :[[@LINE-1]]:79: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_Fai=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Fai*2/3+1;
    struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_Psi=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Psi*2/3+1;
    struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_Gam=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Fai*2/3+1;  //违背7-4
    //CHECK-MESSAGES: :[[@LINE-1]]:79: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Fai=335*struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_Fai;
    struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Psi=335*struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_Psi;
    struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Gam=335*struct_sixteen_Psi_array[0].struct_sixteen_Fai_variable.RJZ_polardemo_Fai;  //违背7-5
    //CHECK-MESSAGES: :[[@LINE-1]]:85: warning: 左值与右值不匹配 [bz12-variable-name-analysis]


    /*
      大类：赋值时右侧内容出错：赋值语句右侧为表达式
      子类：RJZ-1-3-4-case8:左右侧含主结构体，嵌套数组型子结构体的元素（元素为普通变量或数组元素）
    */
     struct struct_seventeen_Fai
    {
        int RJZ_polardemo_1;
        int RJZ_polardemo_2;
        int RJZ_polardemo_3;
        int RJZ_polardemo_F;
        int RJZ_polardemo_P;
        int RJZ_polardemo_G;
        int RJZ_polardemo_array[100];
    };
    struct struct_seventeen_Psi
    {
        struct struct_seventeen_Fai struct_seventeen_Fai_array[10];
    };
    struct struct_seventeen_Psi struct_seventeen_Psi_variable;
    
    int RJZ_polardemo_case_seventeen_F=struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_1+8;
    int RJZ_polardemo_case_seventeen_P=struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_2+8;
    int RJZ_polardemo_case_seventeen_G=struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_3+8; //遵循

    RJZ_polardemo_case_seventeen_F=struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_1+8;
    RJZ_polardemo_case_seventeen_P=struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_2+8;
    RJZ_polardemo_case_seventeen_G=struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_1+8; //违背8-1
    //CHECK-MESSAGES: :[[@LINE-1]]:36: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_1=RJZ_polardemo_case_seventeen_F+1/2;
    struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_2=RJZ_polardemo_case_seventeen_P+2/3;
    struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_3=RJZ_polardemo_case_seventeen_F+3/4; //违背8-2
    //CHECK-MESSAGES: :[[@LINE-1]]:81: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_array[NUMZERO]=RJZ_polardemo_case_seventeen_F+1/2;
    struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_array[NUMONE]=RJZ_polardemo_case_seventeen_P+2/3;
    struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_array[NUMTWO]=RJZ_polardemo_case_seventeen_F+3/4; //违背8-3
    //CHECK-MESSAGES: :[[@LINE-1]]:93: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_array[NUMZERO]=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Fai+1/2;
    struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_array[NUMONE]=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Psi+2/3;
    struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_array[NUMTWO]=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Fai+3/4; //违背8-4
    //CHECK-MESSAGES: :[[@LINE-1]]:93: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_array[NUMZERO]=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Fai+struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Fai/2;
    struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_array[NUMONE]=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Psi+struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Psi/3;
    struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_array[NUMTWO]=struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Gam+struct_fourteen_Psi_variable.struct_fourteen_Fai_variable.RJZ_polardemo_Fai/4; //违背8-5
    //CHECK-MESSAGES: :[[@LINE-1]]:169: warning: 左值与右值不匹配 [bz12-variable-name-analysis]


    /*
      大类：赋值时右侧内容出错：赋值语句右侧为表达式
      子类：RJZ-1-3-4-case9:左右侧含主结构体为数组型，嵌套数组型子结构体的元素（元素为普通变量或数组元素）
    */
    struct struct_eighteen_Fai
    {
        int RJZ_polardemo_x;
        int RJZ_polardemo_y;
        int RJZ_polardemo_z;
        int RJZ_polardemo_F;
        int RJZ_polardemo_P;
        int RJZ_polardemo_G;
        int RJZ_polardemo_array_Fai[100];
    };
    struct struct_eighteen_Psi
    {
        struct struct_eighteen_Fai struct_eighteen_Fai_array[10];
    };
    struct struct_eighteen_Psi struct_eighteen_Psi_array[10];
    
    int RJZ_polardemo_case_eighteen_F=struct_eighteen_Psi_array[0].struct_eighteen_Fai_array[0].RJZ_polardemo_F+8;
    int RJZ_polardemo_case_eighteen_P=struct_eighteen_Psi_array[0].struct_eighteen_Fai_array[0].RJZ_polardemo_P+8;
    int RJZ_polardemo_case_eighteen_G=struct_eighteen_Psi_array[0].struct_eighteen_Fai_array[0].RJZ_polardemo_G+8; //遵循

    RJZ_polardemo_case_eighteen_F=struct_eighteen_Psi_array[0].struct_eighteen_Fai_array[0].RJZ_polardemo_F+8;
    RJZ_polardemo_case_eighteen_P=struct_eighteen_Psi_array[0].struct_eighteen_Fai_array[0].RJZ_polardemo_F+8;  //违背9-1
    //CHECK-MESSAGES: :[[@LINE-1]]:35: warning: 左值与右值不匹配 [bz12-variable-name-analysis]
    RJZ_polardemo_case_eighteen_G=struct_eighteen_Psi_array[0].struct_eighteen_Fai_array[0].RJZ_polardemo_G+8;

    struct_eighteen_Psi_array[0].struct_eighteen_Fai_array[0].RJZ_polardemo_F=struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_array[NUMZERO]+3/2;
    struct_eighteen_Psi_array[0].struct_eighteen_Fai_array[0].RJZ_polardemo_P=struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_array[NUMONE]-4/3;
    struct_eighteen_Psi_array[0].struct_eighteen_Fai_array[0].RJZ_polardemo_G=struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_array[NUMZERO]+3/2;  //违背9-2
    //CHECK-MESSAGES: :[[@LINE-1]]:79: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_eighteen_Psi_array[0].struct_eighteen_Fai_array[0].RJZ_polardemo_array_Fai[NUMZERO]=struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_1+3/2;
    struct_eighteen_Psi_array[0].struct_eighteen_Fai_array[0].RJZ_polardemo_array_Fai[NUMONE]=struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_2-4/3;
    struct_eighteen_Psi_array[0].struct_eighteen_Fai_array[0].RJZ_polardemo_array_Fai[NUMTWO]=struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_2+3/2;  //违背9-3
    //CHECK-MESSAGES: :[[@LINE-1]]:95: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    struct_eighteen_Psi_array[0].struct_eighteen_Fai_array[0].RJZ_polardemo_array_Fai[NUMZERO]=struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_1+struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_array[NUMZERO];
    struct_eighteen_Psi_array[0].struct_eighteen_Fai_array[0].RJZ_polardemo_array_Fai[NUMONE]=struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_2-struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_array[NUMONE];
    struct_eighteen_Psi_array[0].struct_eighteen_Fai_array[0].RJZ_polardemo_array_Fai[NUMTWO]=struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_3+struct_seventeen_Psi_variable.struct_seventeen_Fai_array[0].RJZ_polardemo_array[NUMONE];  //违背9-4
    //CHECK-MESSAGES: :[[@LINE-1]]:171: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    int RJZ_polardemo_case_eighteen_x=struct_eighteen_Psi_array[0].struct_eighteen_Fai_array[0].RJZ_polardemo_F+8;
    int RJZ_polardemo_case_eighteen_y=struct_eighteen_Psi_array[0].struct_eighteen_Fai_array[0].RJZ_polardemo_P+8;
    int RJZ_polardemo_case_eighteen_z=struct_eighteen_Psi_array[0].struct_eighteen_Fai_array[0].RJZ_polardemo_P+8; //违背9-5
    //CHECK-MESSAGES: :[[@LINE-1]]:39: warning: 左值与右值不匹配 [bz12-variable-name-analysis]

    return (0);
}