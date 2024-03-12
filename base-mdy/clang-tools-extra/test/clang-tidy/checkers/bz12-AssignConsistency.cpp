// RUN: %check_clang_tidy %s bz12-AssignConsistency %t
/*
  极性问题：
  “语句块内部赋值语句一致性错误”大类错误，包括“赋值左侧一致性错误”、“赋值右侧一致性错误，单一量赋值”和“赋值右侧一致性错误，赋值量为表达式”三种情况
*/


int main(void) {
    //算法首先提取所有的if语句块，然后提取if语句块中所有的等式关系，存储等式左侧的变量和右侧的单一变量和右侧表达式中的变量
    //目前已知问题
    //1.语句块内为同一数组元素（不同语句块间存在结构相似性，使用的数组不同）对数组的下标没有检查
    //2.赋值右侧为表达式，只对二元操作表达式进行检测，多层嵌套表达式未检测
    //3.报错位置部分在if语句后报错



    /*
      大类：语句块内部赋值语句一致性错误：赋值左侧一致性错误
      子类：RJZ-1-3-7-case1:条件语句块内为同一组数组元素赋值（不同语句块间存在结构相似性，使用的数组不同）
    */
    int RJZ_polardemo_case_first_x = 1;
    int RJZ_polardemo_case_first_array_Fai[10];
    int RJZ_polardemo_case_first_array_Psi[10];
    if (RJZ_polardemo_case_first_x > 0) {
        RJZ_polardemo_case_first_array_Fai[1] = 1;
        RJZ_polardemo_case_first_array_Fai[2] = 2;
        RJZ_polardemo_case_first_array_Fai[3] = 3;  //遵循
    }

    if (RJZ_polardemo_case_first_x > 0) {  //违背1-1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内为同一组数组元素赋值(不同语句块间存在结构相似性,使用的数组不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_first_array_Fai[1] = 1;
        RJZ_polardemo_case_first_array_Fai[2] = 2;
        RJZ_polardemo_case_first_array_Psi[3] = 3;  
    }

    if (RJZ_polardemo_case_first_x > 0) {  //违背1-2
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内为同一组数组元素赋值(不同语句块间存在结构相似性,使用的数组不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_first_array_Fai[1] = 1;
        RJZ_polardemo_case_first_array_Psi[2] = 2;
        RJZ_polardemo_case_first_array_Fai[3] = 3;
    }

    if (RJZ_polardemo_case_first_x > 0) {  //违背1-3
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内为同一组数组元素赋值(不同语句块间存在结构相似性,使用的数组不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_first_array_Psi[1] = 1;
        RJZ_polardemo_case_first_array_Fai[2] = 2;
        RJZ_polardemo_case_first_array_Fai[3] = 3;  
    }

    if (RJZ_polardemo_case_first_x > 0) {  //违背1-4
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内为同一组数组元素赋值(不同语句块间存在结构相似性,使用的数组不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_first_array_Fai[0] = 0;
        RJZ_polardemo_case_first_array_Fai[1] = 1;
        RJZ_polardemo_case_first_array_Fai[2] = 2;
        RJZ_polardemo_case_first_array_Psi[3] = 3;
    }

    if (RJZ_polardemo_case_first_x > 0) {  //违背1-5
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内为同一组数组元素赋值(不同语句块间存在结构相似性,使用的数组不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_first_array_Fai[0] = 0;
        RJZ_polardemo_case_first_array_Psi[1] = 1;
        RJZ_polardemo_case_first_array_Psi[2] = 2;
        RJZ_polardemo_case_first_array_Psi[3] = 3;
    }


    /*
      大类：语句块内部赋值语句一致性错误：赋值左侧一致性错误
      子类：RJZ-1-3-7-case2:条件语句块内为同一组结构体元素赋值（不同语句块间存在结构相似性，使用的结构体不同）
    */
    struct struct_two
    {
        int RJZ_polardemo_x;
        int RJZ_polardemo_y;
        int RJZ_polardemo_z;
        int RJZ_polardemo_Fai;
        int RJZ_polardemo_Psi;
        int RJZ_polardemo_Gam;
    };

    struct struct_two RJZ_polardemo_case_two_struct_Fai, RJZ_polardemo_case_two_struct_Psi;
    int RJZ_polardemo_case_two_x=1;
    if (RJZ_polardemo_case_two_x > 0) {
        RJZ_polardemo_case_two_struct_Fai.RJZ_polardemo_x = 0;
        RJZ_polardemo_case_two_struct_Fai.RJZ_polardemo_y = 1;
        RJZ_polardemo_case_two_struct_Fai.RJZ_polardemo_z = 2;  //遵循
    }
    
    if (RJZ_polardemo_case_two_x > 0) {  //违背2-1
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内为同一组结构体元素赋值(不同语句块间存在结构相似性,使用的结构体不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_two_struct_Fai.RJZ_polardemo_x = 0;
        RJZ_polardemo_case_two_struct_Fai.RJZ_polardemo_y = 1;
        RJZ_polardemo_case_two_struct_Psi.RJZ_polardemo_z = 2;
    }

    if (RJZ_polardemo_case_two_x > 0) {  //违背2-2
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内为同一组结构体元素赋值(不同语句块间存在结构相似性,使用的结构体不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_two_struct_Fai.RJZ_polardemo_x = 0;
        RJZ_polardemo_case_two_struct_Psi.RJZ_polardemo_y = 1;
        RJZ_polardemo_case_two_struct_Fai.RJZ_polardemo_z = 2;
    }

    if (RJZ_polardemo_case_two_x > 0) {  //违背2-3
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内为同一组结构体元素赋值(不同语句块间存在结构相似性,使用的结构体不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_two_struct_Psi.RJZ_polardemo_x = 0;
        RJZ_polardemo_case_two_struct_Fai.RJZ_polardemo_y = 1;
        RJZ_polardemo_case_two_struct_Fai.RJZ_polardemo_z = 2;
    }

    if (RJZ_polardemo_case_two_x > 0) {  //违背2-4
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内为同一组结构体元素赋值(不同语句块间存在结构相似性,使用的结构体不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_two_struct_Psi.RJZ_polardemo_x = 0;
        RJZ_polardemo_case_two_struct_Psi.RJZ_polardemo_y = 1;
        RJZ_polardemo_case_two_struct_Fai.RJZ_polardemo_z = 2;
    }

    if (RJZ_polardemo_case_two_x > 0) {  //违背2-5
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内为同一组结构体元素赋值(不同语句块间存在结构相似性,使用的结构体不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_two_struct_Fai.RJZ_polardemo_x = 0;
        RJZ_polardemo_case_two_struct_Psi.RJZ_polardemo_y = 1;
        RJZ_polardemo_case_two_struct_Psi.RJZ_polardemo_z = 2;
    }


    /*
      大类：语句块内部赋值语句一致性错误：赋值左侧一致性错误
      子类：RJZ-1-3-7-case3:条件语句块内为同一维数组（二维数组）元素赋值（不同语句块间存在结构相似性，使用的数组维数不同）
    */
    int RJZ_polardemo_case_third_array_Fai[10];
    int RJZ_polardemo_case_third_array_Psi[10][10];
    int RJZ_polardemo_case_third_x = 1;

    if (RJZ_polardemo_case_third_x > 1) {
        RJZ_polardemo_case_third_array_Fai[0] = 0;
        RJZ_polardemo_case_third_array_Fai[1] = 1;
        RJZ_polardemo_case_third_array_Fai[2] = 2;
        RJZ_polardemo_case_third_array_Fai[3] = 3;  //遵循
    }

    if (RJZ_polardemo_case_third_x > 1) {
        RJZ_polardemo_case_third_array_Fai[0] = 0;
        RJZ_polardemo_case_third_array_Fai[1] = 1;
        RJZ_polardemo_case_third_array_Fai[2] = 2;
        RJZ_polardemo_case_third_array_Psi[3][0] = 3;  //违背3-1
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 条件语句块内为同一维数组元素赋值(不同语句块间存在结构相似性,使用的数组维数不同) [bz12-AssignConsistency]
    }

    if (RJZ_polardemo_case_third_x > 1) {
        RJZ_polardemo_case_third_array_Fai[0] = 0;
        RJZ_polardemo_case_third_array_Fai[1] = 1;
        RJZ_polardemo_case_third_array_Psi[2][0] = 2;  //违背3-2
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 条件语句块内为同一维数组元素赋值(不同语句块间存在结构相似性,使用的数组维数不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_third_array_Fai[3] = 3;
    }

    if (RJZ_polardemo_case_third_x > 1) {
        RJZ_polardemo_case_third_array_Fai[0] = 0;
        RJZ_polardemo_case_third_array_Psi[1][0] = 1; //违背3-3
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 条件语句块内为同一维数组元素赋值(不同语句块间存在结构相似性,使用的数组维数不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_third_array_Fai[2] = 2;
        RJZ_polardemo_case_third_array_Fai[3] = 3;
    }

    if (RJZ_polardemo_case_third_x > 1) {
        RJZ_polardemo_case_third_array_Psi[0][0] = 0;  
        RJZ_polardemo_case_third_array_Fai[1] = 1;  //违背3-4
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 条件语句块内为同一维数组元素赋值(不同语句块间存在结构相似性,使用的数组维数不同) [bz12-AssignConsistency]  
    }

    if (RJZ_polardemo_case_third_x > 1) {
        RJZ_polardemo_case_third_array_Fai[0] = 0;
        RJZ_polardemo_case_third_array_Fai[1] = 1;
        RJZ_polardemo_case_third_array_Fai[2] = 2;
        RJZ_polardemo_case_third_array_Fai[3] = 3;  
        RJZ_polardemo_case_third_array_Psi[4][0] = 3;  //违背3-5
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 条件语句块内为同一维数组元素赋值(不同语句块间存在结构相似性,使用的数组维数不同) [bz12-AssignConsistency]
    }


    /*
      大类：语句块内部赋值语句一致性错误：赋值左侧一致性错误
      子类：RJZ-1-3-7-case4:条件语句块内为数组型结构体/数组型子结构体同一维结构体中元素赋值（不同语句块间存在结构相似性，使用的数组下标不同）
    */
    struct struct_four
    {
        int RJZ_polardemo_x;
        int RJZ_polardemo_y;
        int RJZ_polardemo_z;
        int RJZ_polardemo_Fai;
        int RJZ_polardemo_Psi;
        int RJZ_polardemo_Gam;
    };

    struct struct_four RJZ_polardemo_case_four_struct_Fai[10];
    int RJZ_polardemo_case_four_x = 1;
    if (RJZ_polardemo_case_four_x > 0) {
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_x = 0;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_y = 1;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_z = 2;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_Fai = 3;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_Psi = 4;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_Gam = 5;  //遵循
        
    }

    if (RJZ_polardemo_case_four_x > 0) {
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_x = 0;
        RJZ_polardemo_case_four_struct_Fai[1].RJZ_polardemo_y = 1;  //违背4-1
        // CHECK-MESSAGES: :[[@LINE-1]]:44: warning: 条件语句块内为数组型结构体赋值(不同语句块间存在结构相似性,使用的数组下标不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_z = 2;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_Fai = 3;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_Psi = 4;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_Gam = 5;
    }

    if (RJZ_polardemo_case_four_x > 0) {
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_x = 0;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_y = 1;
        RJZ_polardemo_case_four_struct_Fai[1].RJZ_polardemo_z = 2;  //违背4-2
        // CHECK-MESSAGES: :[[@LINE-1]]:44: warning: 条件语句块内为数组型结构体赋值(不同语句块间存在结构相似性,使用的数组下标不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_Fai = 3;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_Psi = 4;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_Gam = 5;
    }

    if (RJZ_polardemo_case_four_x > 0) {
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_x = 0;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_y = 1;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_z = 2;
        RJZ_polardemo_case_four_struct_Fai[1].RJZ_polardemo_Fai = 3;  //违背4-3
        // CHECK-MESSAGES: :[[@LINE-1]]:44: warning: 条件语句块内为数组型结构体赋值(不同语句块间存在结构相似性,使用的数组下标不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_Psi = 4;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_Gam = 5;
    }

    if (RJZ_polardemo_case_four_x > 0) {
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_x = 0;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_y = 1;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_z = 2;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_Fai = 3;
        RJZ_polardemo_case_four_struct_Fai[1].RJZ_polardemo_Psi = 4;  //违背4-4
        // CHECK-MESSAGES: :[[@LINE-1]]:44: warning: 条件语句块内为数组型结构体赋值(不同语句块间存在结构相似性,使用的数组下标不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_Gam = 5;
    }

    if (RJZ_polardemo_case_four_x > 0) {
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_x = 0;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_y = 1;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_z = 2;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_Fai = 3;
        RJZ_polardemo_case_four_struct_Fai[0].RJZ_polardemo_Psi = 4;
        RJZ_polardemo_case_four_struct_Fai[1].RJZ_polardemo_Gam = 5;  //违背4-5
        // CHECK-MESSAGES: :[[@LINE-1]]:44: warning: 条件语句块内为数组型结构体赋值(不同语句块间存在结构相似性,使用的数组下标不同) [bz12-AssignConsistency]
    }


    /*
      大类：语句块内部赋值语句一致性错误：赋值右侧一致性错误，单一量赋值
      子类：RJZ-1-3-8-case1:条件语句块内赋值量为同一组数组元素（不同语句块间存在结构相似性，使用的数组不同）
    */
    int RJZ_polardemo_case_five_x = 1, RJZ_polardemo_case_five_y = 2, RJZ_polardemo_case_five_z = 3;
    int RJZ_polardemo_case_five_array_Fai[10] = {}, RJZ_polardemo_case_five_array_Psi[10];

    if (RJZ_polardemo_case_five_x > 0) {
        RJZ_polardemo_case_five_x = RJZ_polardemo_case_five_array_Fai[0];
        RJZ_polardemo_case_five_y = RJZ_polardemo_case_five_array_Fai[1];
        RJZ_polardemo_case_five_z = RJZ_polardemo_case_five_array_Fai[2];  //遵循
    }

    if (RJZ_polardemo_case_five_x > 0) {  //违背5-1
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值量为同一组数组元素(不同语句块间存在结构相似性,使用的数组不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_five_x = RJZ_polardemo_case_five_array_Fai[0];
        RJZ_polardemo_case_five_y = RJZ_polardemo_case_five_array_Fai[1];
        RJZ_polardemo_case_five_z = RJZ_polardemo_case_five_array_Psi[2];
    }

    if (RJZ_polardemo_case_five_x > 0) {  //违背5-2
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值量为同一组数组元素(不同语句块间存在结构相似性,使用的数组不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_five_x = RJZ_polardemo_case_five_array_Fai[0];
        RJZ_polardemo_case_five_y = RJZ_polardemo_case_five_array_Psi[1];
        RJZ_polardemo_case_five_z = RJZ_polardemo_case_five_array_Fai[2];
    }

    if (RJZ_polardemo_case_five_x > 0) {  //违背5-3
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值量为同一组数组元素(不同语句块间存在结构相似性,使用的数组不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_five_x = RJZ_polardemo_case_five_array_Psi[0];
        RJZ_polardemo_case_five_y = RJZ_polardemo_case_five_array_Fai[1];
        RJZ_polardemo_case_five_z = RJZ_polardemo_case_five_array_Fai[2];
    }

    if (RJZ_polardemo_case_five_x > 0) {  //违背5-4
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值量为同一组数组元素(不同语句块间存在结构相似性,使用的数组不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_five_x = RJZ_polardemo_case_five_array_Psi[0];
        RJZ_polardemo_case_five_y = RJZ_polardemo_case_five_array_Psi[1];
        RJZ_polardemo_case_five_z = RJZ_polardemo_case_five_array_Fai[2];
    }

    if (RJZ_polardemo_case_five_x > 0) {  //违背5-5
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值量为同一组数组元素(不同语句块间存在结构相似性,使用的数组不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_five_x = RJZ_polardemo_case_five_array_Fai[0];
        RJZ_polardemo_case_five_y = RJZ_polardemo_case_five_array_Psi[1];
        RJZ_polardemo_case_five_z = RJZ_polardemo_case_five_array_Psi[2];
    }


    /*
      大类：语句块内部赋值语句一致性错误：赋值右侧一致性错误，单一量赋值
      子类：RJZ-1-3-8-case2:条件语句块内赋值量为同一组结构体元素（不同语句块间存在结构相似性，使用的结构体不同）
    */
    struct struct_six
    {
        int RJZ_polardemo_x;
        int RJZ_polardemo_y;
        int RJZ_polardemo_z;
        int RJZ_polardemo_Fai;
        int RJZ_polardemo_Psi;
        int RJZ_polardemo_Gam;
    };

    struct struct_six RJZ_polardemo_case_six_struct_Fai, RJZ_polardemo_case_six_struct_Psi;
    int RJZ_polardemo_case_six_x = 1, RJZ_polardemo_case_six_y = 2, RJZ_polardemo_case_six_z = 3;
    if (RJZ_polardemo_case_six_x > 0) {
        RJZ_polardemo_case_six_x = RJZ_polardemo_case_six_struct_Fai.RJZ_polardemo_x;
        RJZ_polardemo_case_six_y = RJZ_polardemo_case_six_struct_Fai.RJZ_polardemo_y;
        RJZ_polardemo_case_six_z = RJZ_polardemo_case_six_struct_Fai.RJZ_polardemo_z;  //遵循
    }

    if (RJZ_polardemo_case_six_x > 0) {  //违背6-1
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值量为同一组结构体元素(不同语句块间存在结构相似性,使用的结构体不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_six_x = RJZ_polardemo_case_six_struct_Fai.RJZ_polardemo_x;
        RJZ_polardemo_case_six_y = RJZ_polardemo_case_six_struct_Fai.RJZ_polardemo_y;
        RJZ_polardemo_case_six_z = RJZ_polardemo_case_six_struct_Psi.RJZ_polardemo_z;
    }
    
    if (RJZ_polardemo_case_six_x > 0) {  //违背6-2
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值量为同一组结构体元素(不同语句块间存在结构相似性,使用的结构体不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_six_x = RJZ_polardemo_case_six_struct_Fai.RJZ_polardemo_x;
        RJZ_polardemo_case_six_y = RJZ_polardemo_case_six_struct_Psi.RJZ_polardemo_y;
        RJZ_polardemo_case_six_z = RJZ_polardemo_case_six_struct_Fai.RJZ_polardemo_z;
    }

    if (RJZ_polardemo_case_six_x > 0) {  //违背6-3
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值量为同一组结构体元素(不同语句块间存在结构相似性,使用的结构体不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_six_x = RJZ_polardemo_case_six_struct_Psi.RJZ_polardemo_x;
        RJZ_polardemo_case_six_y = RJZ_polardemo_case_six_struct_Fai.RJZ_polardemo_y;
        RJZ_polardemo_case_six_z = RJZ_polardemo_case_six_struct_Fai.RJZ_polardemo_z;
    }

    if (RJZ_polardemo_case_six_x > 0) {  //违背6-4
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值量为同一组结构体元素(不同语句块间存在结构相似性,使用的结构体不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_six_x = RJZ_polardemo_case_six_struct_Psi.RJZ_polardemo_x;
        RJZ_polardemo_case_six_y = RJZ_polardemo_case_six_struct_Psi.RJZ_polardemo_y;
        RJZ_polardemo_case_six_z = RJZ_polardemo_case_six_struct_Fai.RJZ_polardemo_z;
    }

    if (RJZ_polardemo_case_six_x > 0) {  //违背6-5
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值量为同一组结构体元素(不同语句块间存在结构相似性,使用的结构体不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_six_x = RJZ_polardemo_case_six_struct_Fai.RJZ_polardemo_x;
        RJZ_polardemo_case_six_y = RJZ_polardemo_case_six_struct_Psi.RJZ_polardemo_y;
        RJZ_polardemo_case_six_z = RJZ_polardemo_case_six_struct_Psi.RJZ_polardemo_z;
    }


    /*
      大类：语句块内部赋值语句一致性错误：赋值右侧一致性错误，单一量赋值
      子类：RJZ-1-3-8-case3:条件语句块内赋值量为同一维数组（二维数组）元素（不同语句块间存在结构相似性，使用的数组维数不同）
    */
    int RJZ_polardemo_case_seven_array_Fai[10];
    int RJZ_polardemo_case_seven_array_Psi[10][10];
    int RJZ_polardemo_case_seven_x = 1, RJZ_polardemo_case_seven_y = 2, RJZ_polardemo_case_seven_z = 3;

    if (RJZ_polardemo_case_seven_x > 0) {
        RJZ_polardemo_case_seven_x = RJZ_polardemo_case_seven_array_Fai[0];
        RJZ_polardemo_case_seven_y = RJZ_polardemo_case_seven_array_Fai[1];
        RJZ_polardemo_case_seven_z = RJZ_polardemo_case_seven_array_Fai[2];  //遵循
    }

    if (RJZ_polardemo_case_seven_x > 0) {
        RJZ_polardemo_case_seven_x = RJZ_polardemo_case_seven_array_Fai[0];
        RJZ_polardemo_case_seven_y = RJZ_polardemo_case_seven_array_Fai[1];
        RJZ_polardemo_case_seven_z = RJZ_polardemo_case_seven_array_Psi[2][0];  //违背7-1
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 条件语句块内赋值量为同一维数组元素(不同语句块间存在结构相似性,使用的数组维数不同) [bz12-AssignConsistency]
    }

    if (RJZ_polardemo_case_seven_x > 0) {
        RJZ_polardemo_case_seven_x = RJZ_polardemo_case_seven_array_Fai[0];
        RJZ_polardemo_case_seven_y = RJZ_polardemo_case_seven_array_Psi[1][0];  //违背7-2
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 条件语句块内赋值量为同一维数组元素(不同语句块间存在结构相似性,使用的数组维数不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_seven_z = RJZ_polardemo_case_seven_array_Fai[2];
    }

    if (RJZ_polardemo_case_seven_x > 0) {
        RJZ_polardemo_case_seven_x = RJZ_polardemo_case_seven_array_Psi[0][0];
        RJZ_polardemo_case_seven_y = RJZ_polardemo_case_seven_array_Fai[1];  //违背7-3
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 条件语句块内赋值量为同一维数组元素(不同语句块间存在结构相似性,使用的数组维数不同) [bz12-AssignConsistency]
    }

    if (RJZ_polardemo_case_seven_x > 0) {
        RJZ_polardemo_case_seven_x = RJZ_polardemo_case_seven_array_Fai[0];
        RJZ_polardemo_case_seven_y = RJZ_polardemo_case_seven_array_Psi[1][0];  //违背7-4
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 条件语句块内赋值量为同一维数组元素(不同语句块间存在结构相似性,使用的数组维数不同) [bz12-AssignConsistency]
    }

    if (RJZ_polardemo_case_seven_x > 0) {
        RJZ_polardemo_case_seven_x = RJZ_polardemo_case_seven_array_Psi[0][0];
        RJZ_polardemo_case_seven_y = RJZ_polardemo_case_seven_array_Psi[1][0];
        RJZ_polardemo_case_seven_z = RJZ_polardemo_case_seven_array_Fai[2];  //违背7-5
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 条件语句块内赋值量为同一维数组元素(不同语句块间存在结构相似性,使用的数组维数不同) [bz12-AssignConsistency]
    }


    /*
      大类：语句块内部赋值语句一致性错误：赋值右侧一致性错误，单一量赋值
      子类：RJZ-1-3-8-case4:条件语句块内赋值量为数组型结构体/数组型子结构体同一维结构体中元素（不同语句块间存在结构相似性，使用的数组下标不同）
    */
    struct struct_eight
    {
        int RJZ_polardemo_x;
        int RJZ_polardemo_y;
        int RJZ_polardemo_z;
        int RJZ_polardemo_Fai;
        int RJZ_polardemo_Psi;
        int RJZ_polardemo_Gam;
    };

    struct struct_eight RJZ_polardemo_case_eight_struct_Fai[10];
    int RJZ_polardemo_case_eight_x = 1, RJZ_polardemo_case_eight_y = 2, RJZ_polardemo_case_eight_z = 3;
    if (RJZ_polardemo_case_eight_x > 0) {
        RJZ_polardemo_case_eight_x = RJZ_polardemo_case_eight_struct_Fai[0].RJZ_polardemo_x;
        RJZ_polardemo_case_eight_y = RJZ_polardemo_case_eight_struct_Fai[0].RJZ_polardemo_y;
        RJZ_polardemo_case_eight_z = RJZ_polardemo_case_eight_struct_Fai[0].RJZ_polardemo_z;  //遵循
    }

    if (RJZ_polardemo_case_eight_x > 0) {
        RJZ_polardemo_case_eight_x = RJZ_polardemo_case_eight_struct_Fai[0].RJZ_polardemo_x;
        RJZ_polardemo_case_eight_y = RJZ_polardemo_case_eight_struct_Fai[0].RJZ_polardemo_y;
        RJZ_polardemo_case_eight_z = RJZ_polardemo_case_eight_struct_Fai[1].RJZ_polardemo_z;  //违背8-1
        // CHECK-MESSAGES: :[[@LINE-1]]:74: warning: 条件语句块内赋值量为数组型结构体(不同语句块间存在结构相似性,使用的数组下标不同) [bz12-AssignConsistency]
    }

    if (RJZ_polardemo_case_eight_x > 0) {
        RJZ_polardemo_case_eight_x = RJZ_polardemo_case_eight_struct_Fai[0].RJZ_polardemo_x;
        RJZ_polardemo_case_eight_y = RJZ_polardemo_case_eight_struct_Fai[1].RJZ_polardemo_y;  //违背8-2
        // CHECK-MESSAGES: :[[@LINE-1]]:74: warning: 条件语句块内赋值量为数组型结构体(不同语句块间存在结构相似性,使用的数组下标不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_eight_z = RJZ_polardemo_case_eight_struct_Fai[0].RJZ_polardemo_z;
    }

    if (RJZ_polardemo_case_eight_x > 0) {
        RJZ_polardemo_case_eight_x = RJZ_polardemo_case_eight_struct_Fai[1].RJZ_polardemo_x;
        RJZ_polardemo_case_eight_y = RJZ_polardemo_case_eight_struct_Fai[0].RJZ_polardemo_y;  //违背8-3
        // CHECK-MESSAGES: :[[@LINE-1]]:74: warning: 条件语句块内赋值量为数组型结构体(不同语句块间存在结构相似性,使用的数组下标不同) [bz12-AssignConsistency]
    }

    if (RJZ_polardemo_case_eight_x > 0) {
        RJZ_polardemo_case_eight_x = RJZ_polardemo_case_eight_struct_Fai[1].RJZ_polardemo_x;
        RJZ_polardemo_case_eight_y = RJZ_polardemo_case_eight_struct_Fai[1].RJZ_polardemo_y;
        RJZ_polardemo_case_eight_z = RJZ_polardemo_case_eight_struct_Fai[0].RJZ_polardemo_z;  //违背8-4
        // CHECK-MESSAGES: :[[@LINE-1]]:74: warning: 条件语句块内赋值量为数组型结构体(不同语句块间存在结构相似性,使用的数组下标不同) [bz12-AssignConsistency]
    }

    if (RJZ_polardemo_case_eight_x > 0) {
        RJZ_polardemo_case_eight_x = RJZ_polardemo_case_eight_struct_Fai[1].RJZ_polardemo_x;
        RJZ_polardemo_case_eight_y = RJZ_polardemo_case_eight_struct_Fai[1].RJZ_polardemo_y;
        RJZ_polardemo_case_eight_z = RJZ_polardemo_case_eight_struct_Fai[2].RJZ_polardemo_z;  //违背8-5
        // CHECK-MESSAGES: :[[@LINE-1]]:74: warning: 条件语句块内赋值量为数组型结构体(不同语句块间存在结构相似性,使用的数组下标不同) [bz12-AssignConsistency]
    }


    /*
      大类：语句块内部赋值语句一致性错误：赋值右侧一致性错误，赋值量为表达式
      子类：RJZ-1-3-9-case1:条件语句块内赋值语句右侧含同一组数组元素（不同语句块间存在结构相似性，使用的数组不同）
    */
    int RJZ_polardemo_case_nine_x = 1, RJZ_polardemo_case_nine_y = 2, RJZ_polardemo_case_nine_z = 3;
    int RJZ_polardemo_case_nine_array_Fai[10], RJZ_polardemo_case_nine_array_Psi[10];

    if (RJZ_polardemo_case_nine_x > 0) {
        RJZ_polardemo_case_nine_x = RJZ_polardemo_case_nine_array_Fai[0] + 1;
        RJZ_polardemo_case_nine_y = RJZ_polardemo_case_nine_array_Fai[1] + 1;
        RJZ_polardemo_case_nine_z = RJZ_polardemo_case_nine_array_Fai[2] + 1;  //遵循
    }

    if (RJZ_polardemo_case_nine_x > 0) {  //违背9-1
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值语句右侧含同一组数组元素(不同语句块间存在结构相似性,使用的数组不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_nine_x = RJZ_polardemo_case_nine_array_Fai[0] + 1;
        RJZ_polardemo_case_nine_y = RJZ_polardemo_case_nine_array_Fai[1] + 1;
        RJZ_polardemo_case_nine_z = RJZ_polardemo_case_nine_array_Psi[2] + 1;
    }

    if (RJZ_polardemo_case_nine_x > 0) {  //违背9-2
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值语句右侧含同一组数组元素(不同语句块间存在结构相似性,使用的数组不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_nine_x = RJZ_polardemo_case_nine_array_Fai[0] + 1;
        RJZ_polardemo_case_nine_y = RJZ_polardemo_case_nine_array_Psi[1] + 1;
        RJZ_polardemo_case_nine_z = RJZ_polardemo_case_nine_array_Fai[2] + 1;
    }

    if (RJZ_polardemo_case_nine_x > 0) {  //违背9-3
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值语句右侧含同一组数组元素(不同语句块间存在结构相似性,使用的数组不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_nine_x = RJZ_polardemo_case_nine_array_Psi[0] + 1;
        RJZ_polardemo_case_nine_y = RJZ_polardemo_case_nine_array_Fai[1] + 1;
        RJZ_polardemo_case_nine_z = RJZ_polardemo_case_nine_array_Fai[2] + 1;
    }

    if (RJZ_polardemo_case_nine_x > 0) {  //违背9-4
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值语句右侧含同一组数组元素(不同语句块间存在结构相似性,使用的数组不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_nine_x = 1 + RJZ_polardemo_case_nine_array_Psi[0];
        RJZ_polardemo_case_nine_y = 2 + RJZ_polardemo_case_nine_array_Fai[1];
        RJZ_polardemo_case_nine_z = 3 + RJZ_polardemo_case_nine_array_Fai[2];
    }

    if (RJZ_polardemo_case_nine_x > 0) {  //违背9-5
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值语句右侧含同一组数组元素(不同语句块间存在结构相似性,使用的数组不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_nine_x = 1 + RJZ_polardemo_case_nine_array_Fai[0];
        RJZ_polardemo_case_nine_y = 2 + RJZ_polardemo_case_nine_array_Fai[1];
        RJZ_polardemo_case_nine_z = 3 + RJZ_polardemo_case_nine_array_Psi[2];
    }


    /*
      大类：语句块内部赋值语句一致性错误：赋值右侧一致性错误，赋值量为表达式
      子类：RJZ-1-3-9-case2:条件语句块内赋值语句右侧含同一组结构体元素（不同语句块间存在结构相似性，使用的结构体不同）
    */
    struct struct_ten
    {
        int RJZ_polardemo_x;
        int RJZ_polardemo_y;
        int RJZ_polardemo_z;
        int RJZ_polardemo_Fai;
        int RJZ_polardemo_Psi;
        int RJZ_polardemo_Gam;
    };

    struct struct_six RJZ_polardemo_case_ten_struct_Fai, RJZ_polardemo_case_ten_struct_Psi;
    int RJZ_polardemo_case_ten_x = 1, RJZ_polardemo_case_ten_y = 2, RJZ_polardemo_case_ten_z = 3;
    if (RJZ_polardemo_case_ten_x > 0) {
        RJZ_polardemo_case_ten_x = RJZ_polardemo_case_ten_struct_Fai.RJZ_polardemo_x;
        RJZ_polardemo_case_ten_y = RJZ_polardemo_case_ten_struct_Fai.RJZ_polardemo_y;
        RJZ_polardemo_case_ten_z = RJZ_polardemo_case_ten_struct_Fai.RJZ_polardemo_z;  //遵循
    }

    if (RJZ_polardemo_case_ten_x > 0) {  //违背10-1
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值语句右侧含同一结构体元素(不同语句块间存在结构相似性,使用的结构体不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_ten_x = RJZ_polardemo_case_ten_struct_Fai.RJZ_polardemo_x + 1;
        RJZ_polardemo_case_ten_y = RJZ_polardemo_case_ten_struct_Fai.RJZ_polardemo_y + 2;
        RJZ_polardemo_case_ten_z = RJZ_polardemo_case_ten_struct_Psi.RJZ_polardemo_z + 3;
    }

    if (RJZ_polardemo_case_ten_x > 0) {  //违背10-2
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值语句右侧含同一结构体元素(不同语句块间存在结构相似性,使用的结构体不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_ten_x = RJZ_polardemo_case_ten_struct_Fai.RJZ_polardemo_x + 1;
        RJZ_polardemo_case_ten_y = RJZ_polardemo_case_ten_struct_Psi.RJZ_polardemo_y + 2;
        RJZ_polardemo_case_ten_z = RJZ_polardemo_case_ten_struct_Fai.RJZ_polardemo_z + 3;
    }

    if (RJZ_polardemo_case_ten_x > 0) {  //违背10-3
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值语句右侧含同一结构体元素(不同语句块间存在结构相似性,使用的结构体不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_ten_x = RJZ_polardemo_case_ten_struct_Psi.RJZ_polardemo_x + 1;
        RJZ_polardemo_case_ten_y = RJZ_polardemo_case_ten_struct_Fai.RJZ_polardemo_y + 2;
        RJZ_polardemo_case_ten_z = RJZ_polardemo_case_ten_struct_Fai.RJZ_polardemo_z + 3;
    }

    if (RJZ_polardemo_case_ten_x > 0) {  //违背10-4
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值语句右侧含同一结构体元素(不同语句块间存在结构相似性,使用的结构体不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_ten_x = 1 + RJZ_polardemo_case_ten_struct_Psi.RJZ_polardemo_x;
        RJZ_polardemo_case_ten_y = 2 + RJZ_polardemo_case_ten_struct_Fai.RJZ_polardemo_y;
        RJZ_polardemo_case_ten_z = 3 + RJZ_polardemo_case_ten_struct_Fai.RJZ_polardemo_z;
    }

    if (RJZ_polardemo_case_ten_x > 0) {  //违背10-5
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 条件语句块内赋值语句右侧含同一结构体元素(不同语句块间存在结构相似性,使用的结构体不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_ten_x = 1 + RJZ_polardemo_case_ten_struct_Fai.RJZ_polardemo_x;
        RJZ_polardemo_case_ten_y = 2 + RJZ_polardemo_case_ten_struct_Fai.RJZ_polardemo_y;
        RJZ_polardemo_case_ten_z = 3 + RJZ_polardemo_case_ten_struct_Psi.RJZ_polardemo_z;
    }


    /*
      大类：语句块内部赋值语句一致性错误：赋值右侧一致性错误，赋值量为表达式
      子类：RJZ-1-3-9-case3:条件语句块内赋值语句右侧含同一维数组（二维数组）元素（不同语句块间存在结构相似性，使用的数组维数不同）
    */
    int RJZ_polardemo_case_eleven_array_Fai[10];
    int RJZ_polardemo_case_eleven_array_Psi[10][10];
    int RJZ_polardemo_case_eleven_x = 1, RJZ_polardemo_case_eleven_y = 2, RJZ_polardemo_case_eleven_z = 3;

    if (RJZ_polardemo_case_eleven_x > 0) {
        RJZ_polardemo_case_eleven_x = 1 + RJZ_polardemo_case_eleven_array_Fai[0];
        RJZ_polardemo_case_eleven_y = 2 + RJZ_polardemo_case_eleven_array_Fai[1];
        RJZ_polardemo_case_eleven_z = 3 + RJZ_polardemo_case_eleven_array_Fai[2];  //遵循
    }

    if (RJZ_polardemo_case_eleven_x > 0) {
        RJZ_polardemo_case_eleven_x = 1 + RJZ_polardemo_case_eleven_array_Fai[0];
        RJZ_polardemo_case_eleven_y = 2 + RJZ_polardemo_case_eleven_array_Fai[1];
        RJZ_polardemo_case_eleven_z = 3 + RJZ_polardemo_case_eleven_array_Psi[2][0];  //违背11-1
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 条件语句块内赋值语句右侧含同一维数组元素(不同语句块间存在结构相似性,使用的数组维数不同) [bz12-AssignConsistency]
    }

    if (RJZ_polardemo_case_eleven_x > 0) {
        RJZ_polardemo_case_eleven_x = 1 + RJZ_polardemo_case_eleven_array_Fai[0];
        RJZ_polardemo_case_eleven_y = 2 + RJZ_polardemo_case_eleven_array_Psi[1][0];  //违背11-2
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 条件语句块内赋值语句右侧含同一维数组元素(不同语句块间存在结构相似性,使用的数组维数不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_eleven_z = 3 + RJZ_polardemo_case_eleven_array_Fai[2];
    }

    if (RJZ_polardemo_case_eleven_x > 0) {
        RJZ_polardemo_case_eleven_x = 1 + RJZ_polardemo_case_eleven_array_Psi[0][0];
        RJZ_polardemo_case_eleven_y = 2 + RJZ_polardemo_case_eleven_array_Fai[1];  //违背11-3
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 条件语句块内赋值语句右侧含同一维数组元素(不同语句块间存在结构相似性,使用的数组维数不同) [bz12-AssignConsistency]
    }

    if (RJZ_polardemo_case_eleven_x > 0) {
        RJZ_polardemo_case_eleven_x = RJZ_polardemo_case_eleven_array_Psi[0][0] + 1;
        RJZ_polardemo_case_eleven_y = RJZ_polardemo_case_eleven_array_Fai[1] + 2;  //违背11-4
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 条件语句块内赋值语句右侧含同一维数组元素(不同语句块间存在结构相似性,使用的数组维数不同) [bz12-AssignConsistency]
    }

    if (RJZ_polardemo_case_eleven_x > 0) {
        RJZ_polardemo_case_eleven_x = RJZ_polardemo_case_eleven_array_Fai[0] + 1;
        RJZ_polardemo_case_eleven_y = RJZ_polardemo_case_eleven_array_Psi[1][0] + 2;  //违背11-5
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 条件语句块内赋值语句右侧含同一维数组元素(不同语句块间存在结构相似性,使用的数组维数不同) [bz12-AssignConsistency]
    }


    /*
      大类：语句块内部赋值语句一致性错误：赋值右侧一致性错误，赋值量为表达式
      子类：RJZ-1-3-9-case4:条件语句块内赋值语句右侧含数组型结构体/数组型子结构体同一维结构体中元素（不同语句块间存在结构相似性，使用的数组下标不同）
    */
    struct struct_twelve
    {
        int RJZ_polardemo_x;
        int RJZ_polardemo_y;
        int RJZ_polardemo_z;
        int RJZ_polardemo_Fai;
        int RJZ_polardemo_Psi;
        int RJZ_polardemo_Gam;
    };

    struct struct_twelve RJZ_polardemo_case_twelve_struct_Fai[10];
    int RJZ_polardemo_case_twelve_x = 1, RJZ_polardemo_case_twelve_y = 2, RJZ_polardemo_case_twelve_z = 3;
    if (RJZ_polardemo_case_twelve_x > 0) {
        RJZ_polardemo_case_twelve_x = RJZ_polardemo_case_twelve_struct_Fai[0].RJZ_polardemo_x + 1;
        RJZ_polardemo_case_twelve_y = RJZ_polardemo_case_twelve_struct_Fai[0].RJZ_polardemo_y + 2;
        RJZ_polardemo_case_twelve_z = RJZ_polardemo_case_twelve_struct_Fai[0].RJZ_polardemo_z + 3;  //遵循
    }

    if (RJZ_polardemo_case_twelve_x > 0) {
        RJZ_polardemo_case_twelve_x = RJZ_polardemo_case_twelve_struct_Fai[0].RJZ_polardemo_x + 1;
        RJZ_polardemo_case_twelve_y = RJZ_polardemo_case_twelve_struct_Fai[0].RJZ_polardemo_y + 2;
        RJZ_polardemo_case_twelve_z = RJZ_polardemo_case_twelve_struct_Fai[1].RJZ_polardemo_z + 3;  //违背12-1
        // CHECK-MESSAGES: :[[@LINE-1]]:76: warning: 条件语句块内赋值语句右侧含数组型结构体(不同语句块间存在结构相似性,使用的数组下标不同) [bz12-AssignConsistency]
    }

    if (RJZ_polardemo_case_twelve_x > 0) {
        RJZ_polardemo_case_twelve_x = RJZ_polardemo_case_twelve_struct_Fai[0].RJZ_polardemo_x + 1;
        RJZ_polardemo_case_twelve_y = RJZ_polardemo_case_twelve_struct_Fai[1].RJZ_polardemo_y + 2;  //违背12-2
        // CHECK-MESSAGES: :[[@LINE-1]]:76: warning: 条件语句块内赋值语句右侧含数组型结构体(不同语句块间存在结构相似性,使用的数组下标不同) [bz12-AssignConsistency]
        RJZ_polardemo_case_twelve_z = RJZ_polardemo_case_twelve_struct_Fai[0].RJZ_polardemo_z + 3;
    }

    if (RJZ_polardemo_case_twelve_x > 0) {
        RJZ_polardemo_case_twelve_x = RJZ_polardemo_case_twelve_struct_Fai[1].RJZ_polardemo_x + 1;
        RJZ_polardemo_case_twelve_y = RJZ_polardemo_case_twelve_struct_Fai[0].RJZ_polardemo_y + 2;  //违背12-3
        // CHECK-MESSAGES: :[[@LINE-1]]:76: warning: 条件语句块内赋值语句右侧含数组型结构体(不同语句块间存在结构相似性,使用的数组下标不同) [bz12-AssignConsistency]
    }

    if (RJZ_polardemo_case_twelve_x > 0) {
        RJZ_polardemo_case_twelve_x = RJZ_polardemo_case_twelve_struct_Fai[1].RJZ_polardemo_x + 1;
        RJZ_polardemo_case_twelve_y = RJZ_polardemo_case_twelve_struct_Fai[1].RJZ_polardemo_y + 2;
        RJZ_polardemo_case_twelve_z = RJZ_polardemo_case_twelve_struct_Fai[0].RJZ_polardemo_z + 3;  //违背12-4
        // CHECK-MESSAGES: :[[@LINE-1]]:76: warning: 条件语句块内赋值语句右侧含数组型结构体(不同语句块间存在结构相似性,使用的数组下标不同) [bz12-AssignConsistency]
    }

    if (RJZ_polardemo_case_twelve_x > 0) {
        RJZ_polardemo_case_twelve_x = 1 + RJZ_polardemo_case_twelve_struct_Fai[1].RJZ_polardemo_x;
        RJZ_polardemo_case_twelve_y = 2 + RJZ_polardemo_case_twelve_struct_Fai[1].RJZ_polardemo_y;
        RJZ_polardemo_case_twelve_z = 3 + RJZ_polardemo_case_twelve_struct_Fai[0].RJZ_polardemo_z;  //违背12-5
        // CHECK-MESSAGES: :[[@LINE-1]]:80: warning: 条件语句块内赋值语句右侧含数组型结构体(不同语句块间存在结构相似性,使用的数组下标不同) [bz12-AssignConsistency]
    }
    return 0;
}