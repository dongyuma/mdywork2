// RUN: %check_clang_tidy %s bz12-variable-name-in-if-analysis %t
#define NUMZERO 0
#define NUMONE 1
#define NUMTWO 2
#define NUMTHREE 3
//违背示例
int main_1(void) 
{
    //算法的实质延续了先前算法，首先提取if条件判定语句中比较语句(如>,>=,!=,==等)以及比较语句中涉及的变量(目前可以检测DeclRefExpr、MemberExpr、ArraySubscriptExpr)，寻找每个变量中的关键字(如Fai、Psi、F等，并非每个变量都有关键字)，随后依次遍历每个比较关系式，检查关系式左右侧存在关键字的变量是否对应
    //对于数组元素，不对数组名称进行一致性检查，仅检查等价集合中存在的数组下标，若等式两侧中只有一侧的下标处于等价集合中也不会检查
    //算法仅对最外层元素进行一致性检查，例如现有变量名为a.b.c[0].d.f，算法仅对名称f进行一致性检查
    //目前已知问题
    //1.关键字提取算法不完善，例如有两个关键字分别为wxyk和wzek的话(关键字首字母和最后一个字母相同)，算法无法提取
    //2.暂时不支持++，--，+=，-=等情况
    //3.关键字提取算法可能提取出错，例如现有变量xyz_1_x,xyz_1_y,xyz_2_x,xyz_2_y,变量关键字有多个，但目前算法只会保留最后一个，从而导致误报或漏报的产生

    /*
      条件语句左右侧比对不一致错误：左右侧为单一量（包括多重条件用“与、或”连接）
      RJZ-1-4-1-case1:左右侧含简单变量
    */
    int RJZ_polardemo_case_first_x=1,RJZ_polardemo_case_first_y=2,RJZ_polardemo_case_first_z=3;
    int RJZ_polardemo_case_first_F=1,RJZ_polardemo_case_first_P=2,RJZ_polardemo_case_first_G=3;
    int RJZ_polardemo_case_first_a=1,RJZ_polardemo_case_first_b=2,RJZ_polardemo_case_first_c=3;
    int RJZ_polardemo_case_first_Fai=1,RJZ_polardemo_case_first_Psi=2,RJZ_polardemo_case_first_Gam=3;
    int RJZ_polardemo_case_first_data=0;

    if(RJZ_polardemo_case_first_x<RJZ_polardemo_case_first_F){
        RJZ_polardemo_case_first_data++;
    }
    if(RJZ_polardemo_case_first_y<RJZ_polardemo_case_first_P){
        RJZ_polardemo_case_first_data++;
    }
    if(RJZ_polardemo_case_first_z<RJZ_polardemo_case_first_G){  //遵循
        RJZ_polardemo_case_first_data++;
    }

    if(RJZ_polardemo_case_first_x<RJZ_polardemo_case_first_F || (RJZ_polardemo_case_first_a<RJZ_polardemo_case_first_Fai)){
        RJZ_polardemo_case_first_data++;
    }
    if(RJZ_polardemo_case_first_y<RJZ_polardemo_case_first_P || (RJZ_polardemo_case_first_b<RJZ_polardemo_case_first_Psi)){
        RJZ_polardemo_case_first_data++;
    }
    if(RJZ_polardemo_case_first_z<RJZ_polardemo_case_first_G || (RJZ_polardemo_case_first_c<RJZ_polardemo_case_first_Fai)){  //违背1-1
    // CHECK-MESSAGES: :[[@LINE-1]]:93: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_first_data++;
    }

    if(RJZ_polardemo_case_first_x<RJZ_polardemo_case_first_F && (RJZ_polardemo_case_first_a<RJZ_polardemo_case_first_Fai)){
        RJZ_polardemo_case_first_data++;
    }
    if(RJZ_polardemo_case_first_y<RJZ_polardemo_case_first_P && (RJZ_polardemo_case_first_a<RJZ_polardemo_case_first_Psi)){  //违背1-2
    // CHECK-MESSAGES: :[[@LINE-1]]:93: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_first_data++;
    }
    if(RJZ_polardemo_case_first_z<RJZ_polardemo_case_first_G && (RJZ_polardemo_case_first_c<RJZ_polardemo_case_first_Gam)){
        RJZ_polardemo_case_first_data++;
    }

    if((RJZ_polardemo_case_first_x>RJZ_polardemo_case_first_a || RJZ_polardemo_case_first_x<RJZ_polardemo_case_first_F) && (RJZ_polardemo_case_first_a<RJZ_polardemo_case_first_Fai)){
        RJZ_polardemo_case_first_data++;
    }
    if((RJZ_polardemo_case_first_y>RJZ_polardemo_case_first_b || RJZ_polardemo_case_first_y<RJZ_polardemo_case_first_P) && (RJZ_polardemo_case_first_b<RJZ_polardemo_case_first_Psi)){
        RJZ_polardemo_case_first_data++;
    }
    if((RJZ_polardemo_case_first_z>RJZ_polardemo_case_first_a || RJZ_polardemo_case_first_z<RJZ_polardemo_case_first_G) && (RJZ_polardemo_case_first_c<RJZ_polardemo_case_first_Gam)){  //违背1-3
    // CHECK-MESSAGES: :[[@LINE-1]]:36: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_first_data++;
    }

    if(RJZ_polardemo_case_first_x>=RJZ_polardemo_case_first_F && !(RJZ_polardemo_case_first_a>=RJZ_polardemo_case_first_Fai)){
        RJZ_polardemo_case_first_data++;
    }
    if(RJZ_polardemo_case_first_y>=RJZ_polardemo_case_first_P && !(RJZ_polardemo_case_first_a>=RJZ_polardemo_case_first_Psi)){  //违背1-4
    // CHECK-MESSAGES: :[[@LINE-1]]:96: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_first_data++;
    }
    if(RJZ_polardemo_case_first_z>=RJZ_polardemo_case_first_G && !(RJZ_polardemo_case_first_c>=RJZ_polardemo_case_first_Gam)){
        RJZ_polardemo_case_first_data++;
    }

    if(!(((RJZ_polardemo_case_first_a==RJZ_polardemo_case_first_Psi)))){  //违背1-5
    // CHECK-MESSAGES: :[[@LINE-1]]:40: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_first_data++;
    }
    if(!(((RJZ_polardemo_case_first_b==RJZ_polardemo_case_first_Psi)))){
        RJZ_polardemo_case_first_data++;
    }
    if(!(((RJZ_polardemo_case_first_c==RJZ_polardemo_case_first_Gam)))){
        RJZ_polardemo_case_first_data++;
    }


    /*
      条件语句左右侧比对不一致错误：左右侧为单一量（包括多重条件用“与、或”连接）
      RJZ-1-4-1-case2:左右侧含简单数组
    */
    int RJZ_polardemo_case_second_array_Fai[10];
    int RJZ_polardemo_case_second_array_Psi[10];
    int RJZ_polardemo_case_second_x=1,RJZ_polardemo_case_second_y=2,RJZ_polardemo_case_second_z=3;
    int RJZ_polardemo_case_second_F=1,RJZ_polardemo_case_second_P=2,RJZ_polardemo_case_second_G=3;
    int RJZ_polardemo_case_second_data=0;

    if(RJZ_polardemo_case_second_array_Fai[0]<RJZ_polardemo_case_second_x && RJZ_polardemo_case_second_array_Fai[0]<RJZ_polardemo_case_second_F){
        RJZ_polardemo_case_second_data++;
    }
    if(RJZ_polardemo_case_second_array_Fai[1]<RJZ_polardemo_case_second_y && RJZ_polardemo_case_second_array_Fai[1]<RJZ_polardemo_case_second_P){
        RJZ_polardemo_case_second_data++;
    }
    if(RJZ_polardemo_case_second_array_Fai[2]<RJZ_polardemo_case_second_z && RJZ_polardemo_case_second_array_Fai[2]<RJZ_polardemo_case_second_G){  //遵循
        RJZ_polardemo_case_second_data++;
    }

    if(RJZ_polardemo_case_second_array_Fai[NUMZERO]<RJZ_polardemo_case_second_x){
        RJZ_polardemo_case_second_data++;
    }
    if(RJZ_polardemo_case_second_array_Fai[NUMONE]<RJZ_polardemo_case_second_y){
        RJZ_polardemo_case_second_data++;
    }
    if(RJZ_polardemo_case_second_array_Fai[NUMZERO]<RJZ_polardemo_case_second_z){  //违背2-1
    // CHECK-MESSAGES: :[[@LINE-1]]:53: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_second_data++;
    }

    if(RJZ_polardemo_case_second_array_Fai[NUMZERO]!=RJZ_polardemo_case_second_x){
        RJZ_polardemo_case_second_data++;
    }
    if(RJZ_polardemo_case_second_array_Fai[NUMZERO]!=RJZ_polardemo_case_second_y){  //违背2-2
    // CHECK-MESSAGES: :[[@LINE-1]]:54: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_second_data++;
    }
    if(RJZ_polardemo_case_second_array_Fai[NUMTWO]!=RJZ_polardemo_case_second_z){
        RJZ_polardemo_case_second_data++;
    }

    if(!(RJZ_polardemo_case_second_array_Fai[NUMZERO]>=RJZ_polardemo_case_second_x)){
        RJZ_polardemo_case_second_data++;
    }
    if(!(RJZ_polardemo_case_second_array_Fai[NUMZERO]>=RJZ_polardemo_case_second_y)){  //违背2-3
    // CHECK-MESSAGES: :[[@LINE-1]]:56: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_second_data++;
    }
    if(!(RJZ_polardemo_case_second_array_Fai[NUMTWO]>=RJZ_polardemo_case_second_z)){
        RJZ_polardemo_case_second_data++;
    }

    if(!(RJZ_polardemo_case_second_array_Fai[NUMZERO]>=RJZ_polardemo_case_second_x) || RJZ_polardemo_case_second_F<RJZ_polardemo_case_second_array_Fai[NUMONE]){  //违背2-4
    // CHECK-MESSAGES: :[[@LINE-1]]:116: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_second_data++;
    }
    if(!(RJZ_polardemo_case_second_array_Fai[NUMONE]>=RJZ_polardemo_case_second_y) || RJZ_polardemo_case_second_P<RJZ_polardemo_case_second_array_Fai[NUMONE]){
        RJZ_polardemo_case_second_data++;
    }
    if(!(RJZ_polardemo_case_second_array_Fai[NUMTWO]>=RJZ_polardemo_case_second_z) || RJZ_polardemo_case_second_G<RJZ_polardemo_case_second_array_Fai[NUMTWO]){
        RJZ_polardemo_case_second_data++;
    }

    if(RJZ_polardemo_case_second_array_Fai[0]==RJZ_polardemo_case_second_x){
        RJZ_polardemo_case_second_data++;
    }
    if(RJZ_polardemo_case_second_array_Fai[1]==RJZ_polardemo_case_second_x){  //违背2-5
    // CHECK-MESSAGES: :[[@LINE-1]]:48: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_second_data++;
    }
    if(RJZ_polardemo_case_second_array_Fai[2]==RJZ_polardemo_case_second_z){
        RJZ_polardemo_case_second_data++;
    }

    if(RJZ_polardemo_case_second_array_Fai[0]<RJZ_polardemo_case_second_array_Psi[0]){  //不对数组名称进行一致性检查，仅检查位于等价集合中的数组下标
        RJZ_polardemo_case_second_data++;
    }
    if(RJZ_polardemo_case_second_array_Fai[1]<RJZ_polardemo_case_second_array_Psi[1]){
        RJZ_polardemo_case_second_data++;
    }
    if(RJZ_polardemo_case_second_array_Fai[2]<RJZ_polardemo_case_second_array_Psi[0]){  //违背2-6
    // CHECK-MESSAGES: :[[@LINE-1]]:47: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_second_data++;
    }

    if(RJZ_polardemo_case_second_array_Fai[2]<RJZ_polardemo_case_second_array_Psi[9]){  //右侧数组名不在等价集合中，不检查
        RJZ_polardemo_case_second_data++;
    }
    if(RJZ_polardemo_case_second_array_Fai[2]<RJZ_polardemo_case_second_array_Psi[2]){
        RJZ_polardemo_case_second_data++;
    }
    if(RJZ_polardemo_case_second_array_Fai[1]<RJZ_polardemo_case_second_array_Psi[2]){  //违背2-7
    // CHECK-MESSAGES: :[[@LINE-1]]:47: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_second_data++;
    }

    if(RJZ_polardemo_case_second_array_Fai[9]<RJZ_polardemo_case_second_array_Psi[7]){  //两侧数组名不在等价集合中，不检查
        RJZ_polardemo_case_second_data++;
    }
    if(RJZ_polardemo_case_second_array_Fai[7]<RJZ_polardemo_case_second_array_Psi[1]){  //左侧数组名不在等价集合中，不检查
        RJZ_polardemo_case_second_data++;
    }
    if(RJZ_polardemo_case_second_array_Fai[0]<RJZ_polardemo_case_second_array_Psi[2]){  //违背2-8
    // CHECK-MESSAGES: :[[@LINE-1]]:47: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_second_data++;
    }

    if(RJZ_polardemo_case_second_array_Fai[8]>RJZ_polardemo_case_second_x){  //不检查
        RJZ_polardemo_case_second_data++;
    }
    if(RJZ_polardemo_case_second_array_Fai[0]>RJZ_polardemo_case_second_x){
        RJZ_polardemo_case_second_data++;
    }
    if(RJZ_polardemo_case_second_array_Fai[1]>RJZ_polardemo_case_second_x){  //违背2-9
    // CHECK-MESSAGES: :[[@LINE-1]]:47: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_second_data++;
    }

    if(RJZ_polardemo_case_second_x!=RJZ_polardemo_case_second_array_Fai[0]){
        RJZ_polardemo_case_second_data++;
    }
    if(RJZ_polardemo_case_second_y!=RJZ_polardemo_case_second_array_Fai[4]){  //不检查
        RJZ_polardemo_case_second_data++;
    }
    if(RJZ_polardemo_case_second_z!=RJZ_polardemo_case_second_array_Fai[1]){  //违背2-10
    // CHECK-MESSAGES: :[[@LINE-1]]:37: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_second_data++;
    }


    /*
      条件语句左右侧比对不一致错误：左右侧为单一量（包括多重条件用“与、或”连接）
      RJZ-1-4-1-case3:左右侧含结构体普通元素
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
    int RJZ_polardemo_case_third_data=0;

    if(RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Fai<RJZ_polardemo_case_third_x){
        RJZ_polardemo_case_third_data++;
    }
    if(RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Psi<RJZ_polardemo_case_third_y){
        RJZ_polardemo_case_third_data++;
    }
    if(RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Gam<RJZ_polardemo_case_third_z){  //遵循
        RJZ_polardemo_case_third_data++;
    }

    if(RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Fai<RJZ_polardemo_case_third_array_Fai[0]){
        RJZ_polardemo_case_third_data++;
    }
    if(RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Psi<RJZ_polardemo_case_third_array_Fai[1]){
        RJZ_polardemo_case_third_data++;
    }
    if(RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Fai<RJZ_polardemo_case_third_array_Fai[2]){  //违背3-1
    // CHECK-MESSAGES: :[[@LINE-1]]:62: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_third_data++;
    }

    if(RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_x>=RJZ_polardemo_case_third_array_Fai[NUMZERO]){
        RJZ_polardemo_case_third_data++;
    }
    if(RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_y>=RJZ_polardemo_case_third_array_Fai[NUMONE]){
        RJZ_polardemo_case_third_data++;
    }
    if(RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_z>=RJZ_polardemo_case_third_array_Fai[NUMZERO]){  //违背3-2
    // CHECK-MESSAGES: :[[@LINE-1]]:61: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_third_data++;
    }

    if(RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Fai==RJZ_polardemo_case_third_array_Fai[0]){
        RJZ_polardemo_case_third_data++;
    }
    else if(RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Psi==RJZ_polardemo_case_third_array_Fai[1]){
        RJZ_polardemo_case_third_data++;
    }
    else if(RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Fai==RJZ_polardemo_case_third_array_Fai[2]){  //违背3-3
    // CHECK-MESSAGES: :[[@LINE-1]]:68: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_third_data++;
    }

    if(RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Fai==RJZ_polardemo_case_third_array_Fai[0] & RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Fai==RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_x){
        RJZ_polardemo_case_third_data++;
    }
    if(RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Psi==RJZ_polardemo_case_third_array_Fai[1] & RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Psi==RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_y){
        RJZ_polardemo_case_third_data++;
    }
    if(RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Gam==RJZ_polardemo_case_third_array_Fai[2] & RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Gam==RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_x){  //违背3-4
    // CHECK-MESSAGES: :[[@LINE-1]]:158: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_third_data++;
    }

    if(RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Fai==RJZ_polardemo_case_third_array_Fai[0] | RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Fai==RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_x){
        RJZ_polardemo_case_third_data++;
    }
    if(RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Psi==RJZ_polardemo_case_third_array_Fai[1] | RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Psi==RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_y){
        RJZ_polardemo_case_third_data++;
    }
    if(RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Gam==RJZ_polardemo_case_third_array_Fai[2] | RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Gam==RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_x){  //违背3-5
    // CHECK-MESSAGES: :[[@LINE-1]]:158: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_third_data++;
    }

    /*
      条件语句左右侧比对不一致错误：左右侧为单一量（包括多重条件用“与、或”连接）
      RJZ-1-4-1-case4:左右侧含结构体数组元素
    */
    struct struct_fourth
    {
        int RJZ_polardemo_array_Fai[10];
        int RJZ_polardemo_a;
        int RJZ_polardemo_b;
        int RJZ_polardemo_c;
    };
    struct struct_fourth RJZ_polardemo_case_fourth_struct_Fai;
    int RJZ_polardemo_case_fourth_x=1,RJZ_polardemo_case_fourth_y=2,RJZ_polardemo_case_fourth_z=3;
    int RJZ_polardemo_case_fourth_data=0;
    int RJZ_polardemo_case_fourth_array_Fai[10];

    if(RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[0]<RJZ_polardemo_case_fourth_x){
        RJZ_polardemo_case_fourth_data++;
    }
    if(RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[1]<RJZ_polardemo_case_fourth_y){
        RJZ_polardemo_case_fourth_data++;
    }
    if(RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[2]<RJZ_polardemo_case_fourth_z){  //遵循
        RJZ_polardemo_case_fourth_data++;
    }

    if(RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[0]!=RJZ_polardemo_case_fourth_array_Fai[0]){
        RJZ_polardemo_case_fourth_data++;
    }
    else if(RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[1]!=RJZ_polardemo_case_fourth_array_Fai[1]){
        RJZ_polardemo_case_fourth_data++;
    }
    else if(RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[2]!=RJZ_polardemo_case_fourth_array_Fai[0]){  //违背4-1
    // CHECK-MESSAGES: :[[@LINE-1]]:78: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_fourth_data++;
    }

    if(RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[NUMZERO]!=RJZ_polardemo_case_fourth_array_Fai[0]){
        RJZ_polardemo_case_fourth_data++;
    }
    else if(RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[NUMONE]!=RJZ_polardemo_case_fourth_array_Fai[1]){
        RJZ_polardemo_case_fourth_data++;
    }
    else if(RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[NUMTWO]!=RJZ_polardemo_case_fourth_array_Fai[0]){  //违背4-2
    // CHECK-MESSAGES: :[[@LINE-1]]:83: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_fourth_data++;
    }

    if(RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[NUMZERO]>=RJZ_polardemo_case_fourth_x && RJZ_polardemo_case_fourth_x<=RJZ_polardemo_case_fourth_array_Fai[NUMZERO]){
        RJZ_polardemo_case_fourth_data++;
    }
    if(RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[NUMONE]>=RJZ_polardemo_case_fourth_y && RJZ_polardemo_case_fourth_y<=RJZ_polardemo_case_fourth_array_Fai[NUMZERO]){  //违背4-3
    // CHECK-MESSAGES: :[[@LINE-1]]:138: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_fourth_data++;
    }
    if(RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[NUMTWO]>=RJZ_polardemo_case_fourth_z && RJZ_polardemo_case_fourth_z<=RJZ_polardemo_case_fourth_array_Fai[NUMTWO]){
        RJZ_polardemo_case_fourth_data++;
    }

    if(RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[0]<RJZ_polardemo_case_fourth_x){
        if(RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[1]<RJZ_polardemo_case_fourth_y){
            if(RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[0]<RJZ_polardemo_case_fourth_z){  //违背4-4
            // CHECK-MESSAGES: :[[@LINE-1]]:80: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
                RJZ_polardemo_case_fourth_data++;
            }
        }
    }

    if(RJZ_polardemo_case_fourth_x>RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[0]){
        RJZ_polardemo_case_fourth_data++;
    }
    else if(RJZ_polardemo_case_fourth_y>RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[1]){
        RJZ_polardemo_case_fourth_data++;
    }
    else if(RJZ_polardemo_case_fourth_z>RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[1]){  //违背4-5
    // CHECK-MESSAGES: :[[@LINE-1]]:41: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_fourth_data++;
    }


    /*
      条件语句左右侧比对不一致错误：左右侧为单一量（包括多重条件用“与、或”连接）
      RJZ-1-4-1-case5:左右侧含嵌套结构体的普通元素
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
    int RJZ_polardemo_case_fifth_x=1,RJZ_polardemo_case_fifth_y=2,RJZ_polardemo_case_fifth_z=3;
    int RJZ_polardemo_case_fifth_data=0;
    int RJZ_polardemo_case_fifth_array_Fai[10];

    if(struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1<=RJZ_polardemo_case_fifth_x){
        RJZ_polardemo_case_fifth_data++;
    }
    if(struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_2<=RJZ_polardemo_case_fifth_y){
        RJZ_polardemo_case_fifth_data++;
    }
    if(struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_3<=RJZ_polardemo_case_fifth_z){  //遵循
        RJZ_polardemo_case_fifth_data++;
    }

    if(struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1<=RJZ_polardemo_case_fifth_array_Fai[0]){
        RJZ_polardemo_case_fifth_data++;
    }
    if(struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_2<=RJZ_polardemo_case_fifth_array_Fai[1]){
        RJZ_polardemo_case_fifth_data++;
    }
    if(struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1<=RJZ_polardemo_case_fifth_array_Fai[2]){  //违背5-1
    // CHECK-MESSAGES: :[[@LINE-1]]:73: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_fifth_data++;
    }

    if(struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1<=RJZ_polardemo_case_fifth_x){
        RJZ_polardemo_case_fifth_data++;
    }
    else if(struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_2<=RJZ_polardemo_case_fifth_y){
        RJZ_polardemo_case_fifth_data++;
    }
    else if(struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1<=RJZ_polardemo_case_fifth_z){  //违背5-2
    // CHECK-MESSAGES: :[[@LINE-1]]:78: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_fifth_data++;
    }

    if(struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1<=RJZ_polardemo_case_fifth_x && struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1<=RJZ_polardemo_case_fifth_array_Fai[0] && struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1>struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_F){
        RJZ_polardemo_case_fifth_data++;
    }
    if(struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_2<=RJZ_polardemo_case_fifth_y && struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_2<=RJZ_polardemo_case_fifth_array_Fai[1] && struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_2>struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_P){
        RJZ_polardemo_case_fifth_data++;
    }
    if(struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_3<=RJZ_polardemo_case_fifth_z && struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_3<=RJZ_polardemo_case_fifth_array_Fai[2] && struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1>struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_G){  //违背5-3
    // CHECK-MESSAGES: :[[@LINE-1]]:273: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_fifth_data++;
    }

    if(struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1>RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[0]){
        RJZ_polardemo_case_fifth_data++;
    }
    if(struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_2>RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[1]){
        RJZ_polardemo_case_fifth_data++;
    }
    if(struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_3>RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[0]){  //违背5-4
    // CHECK-MESSAGES: :[[@LINE-1]]:72: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_fifth_data++;
    }

    if(struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1>RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Psi){  //违背5-5
    // CHECK-MESSAGES: :[[@LINE-1]]:72: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_fifth_data++;
    }
    if(struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_2>RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Psi){
        RJZ_polardemo_case_fifth_data++;
    }
    if(struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_3>RJZ_polardemo_case_third_struct_Fai.RJZ_polardemo_Gam){
        RJZ_polardemo_case_fifth_data++;
    }


    /*
      条件语句左右侧比对不一致错误：左右侧为单一量（包括多重条件用“与、或”连接）
      RJZ-1-4-1-case6:左右侧含嵌套结构体的数组元素
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
    int RJZ_polardemo_case_sixth_x=1,RJZ_polardemo_case_sixth_y=2,RJZ_polardemo_case_sixth_z=3;
    int RJZ_polardemo_case_sixth_data=0;
    int RJZ_polardemo_case_sixth_array_Fai[10];

    if(struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[0]<RJZ_polardemo_case_sixth_x){
        RJZ_polardemo_case_sixth_data++;
    }
    if(struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[1]<RJZ_polardemo_case_sixth_y){
        RJZ_polardemo_case_sixth_data++;
    }
    if(struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[2]<RJZ_polardemo_case_sixth_z){  //遵循
        RJZ_polardemo_case_sixth_data++;
    }

    if(struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[0]<RJZ_polardemo_case_sixth_array_Fai[0]){
        RJZ_polardemo_case_sixth_data++;
    }
    if(struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[1]<RJZ_polardemo_case_sixth_array_Fai[1]){
        RJZ_polardemo_case_sixth_data++;
    }
    if(struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[2]<RJZ_polardemo_case_sixth_array_Fai[1]){  //违背6-1
    // CHECK-MESSAGES: :[[@LINE-1]]:98: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_sixth_data++;
    }

    if(struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[0]<struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1){
        RJZ_polardemo_case_sixth_data++;
    }
    if(struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[1]<struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_2){
        RJZ_polardemo_case_sixth_data++;
    }
    if(struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[2]<struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1){  //违背6-2
    // CHECK-MESSAGES: :[[@LINE-1]]:98: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_sixth_data++;
    }

    if(struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[0]<struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1){
        RJZ_polardemo_case_sixth_data++;
    }
    if(struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[1]<struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_2){
        RJZ_polardemo_case_sixth_data++;
    }
    if(struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[2]<struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1){  //违背6-3
    // CHECK-MESSAGES: :[[@LINE-1]]:98: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_sixth_data++;
    }

    if(struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[0]<RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[0]){
        RJZ_polardemo_case_sixth_data++;
    }
    if(struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[1]<RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[1]){
        RJZ_polardemo_case_sixth_data++;
    }
    if(struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[2]<RJZ_polardemo_case_fourth_struct_Fai.RJZ_polardemo_array_Fai[0]){  //违背6-4
    // CHECK-MESSAGES: :[[@LINE-1]]:98: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_sixth_data++;
    }

    if(struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[0]<RJZ_polardemo_case_sixth_x || struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[0]<RJZ_polardemo_case_sixth_array_Fai[0]){
        RJZ_polardemo_case_sixth_data++;
    }
    if(struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[1]<RJZ_polardemo_case_sixth_y || struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[1]<RJZ_polardemo_case_sixth_array_Fai[1]){
        RJZ_polardemo_case_sixth_data++;
    }
    if(struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[2]<RJZ_polardemo_case_sixth_z || struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[2]<RJZ_polardemo_case_sixth_array_Fai[0]){  //违背6-5
    // CHECK-MESSAGES: :[[@LINE-1]]:218: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_sixth_data++;
    }


    /*
      条件语句左右侧比对不一致错误：左右侧为单一量（包括多重条件用“与、或”连接）
      RJZ-1-4-1-case7:左右侧含主结构体为数组型，嵌套普通子结构体的元素（元素为普通变量或数组元素）
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
    int RJZ_polardemo_case_seventh_x=1,RJZ_polardemo_case_seventh_y=2,RJZ_polardemo_case_seventh_z=3;
    int RJZ_polardemo_case_seventh_data=0;
    int RJZ_polardemo_case_seventh_array_Fai[10];

    if(struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_1<struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_F){
        RJZ_polardemo_case_seventh_data++;
    }
    if(struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_2<struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_P){
        RJZ_polardemo_case_seventh_data++;
    }
    if(struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_3<struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_G){  //遵循
        RJZ_polardemo_case_seventh_data++;
    }

    if(struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_1<RJZ_polardemo_case_seventh_array_Fai[NUMZERO]){
        RJZ_polardemo_case_seventh_data++;
    }
    if(struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_2<RJZ_polardemo_case_seventh_array_Fai[NUMONE]){
        RJZ_polardemo_case_seventh_data++;
    }
    if(struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_3<RJZ_polardemo_case_seventh_array_Fai[NUMZERO]){  //违背7-1
    // CHECK-MESSAGES: :[[@LINE-1]]:80: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_seventh_data++;
    }

    if(struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[NUMZERO]<RJZ_polardemo_case_seventh_array_Fai[NUMZERO]){
        RJZ_polardemo_case_seventh_data++;
    }
    else if(struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[NUMZERO]<RJZ_polardemo_case_seventh_array_Fai[NUMZERO]){
        RJZ_polardemo_case_seventh_data++;
    }
    if(struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[0]<RJZ_polardemo_case_seventh_array_Fai[NUMTWO]){  //违背7-2
    // CHECK-MESSAGES: :[[@LINE-1]]:91: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_seventh_data++;
    }

    if(struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[NUMZERO]<struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMZERO]){
        RJZ_polardemo_case_seventh_data++;
    }
    if(struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[NUMONE]<struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMONE]){
        RJZ_polardemo_case_seventh_data++;
    }
    if(struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[NUMZERO]<struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[NUMTWO]){  //违背7-3
    // CHECK-MESSAGES: :[[@LINE-1]]:97: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_seventh_data++;
    }

    if(struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[NUMZERO]<struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1){
        RJZ_polardemo_case_seventh_data++;
    }
    if(struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[NUMONE]<struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_2){
        RJZ_polardemo_case_seventh_data++;
    }
    if(struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[NUMTWO]<struct_fifth_Psi_variable.struct_fifth_variable.RJZ_polardemo_1){  //违背7-4
    // CHECK-MESSAGES: :[[@LINE-1]]:96: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_seventh_data++;
    }

    if(struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[NUMZERO]<RJZ_polardemo_case_seventh_array_Fai[NUMZERO] | struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_F<RJZ_polardemo_case_seventh_x){
        RJZ_polardemo_case_seventh_data++;
    }
    if(struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[NUMONE]<RJZ_polardemo_case_seventh_array_Fai[NUMONE] | struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_P<RJZ_polardemo_case_seventh_y){
        RJZ_polardemo_case_seventh_data++;
    }
    if(struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[NUMTWO]<RJZ_polardemo_case_seventh_array_Fai[NUMTWO] | struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_F<RJZ_polardemo_case_seventh_z){  //违背7-5
    // CHECK-MESSAGES: :[[@LINE-1]]:215: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_seventh_data++;
    }


    /*
      条件语句左右侧比对不一致错误：左右侧为单一量（包括多重条件用“与、或”连接）
      RJZ-1-4-1-case8:左右侧含主结构体，嵌套数组型子结构体的元素（元素为普通变量或数组元素）
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
    int RJZ_polardemo_case_eighth_x=1,RJZ_polardemo_case_eighth_y=2,RJZ_polardemo_case_eighth_z=3;
    int RJZ_polardemo_case_eighth_data=0;
    int RJZ_polardemo_case_eighth_array_Fai[10];

    if(struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_a>=RJZ_polardemo_case_eighth_x){
        RJZ_polardemo_case_eighth_data++;
    }
    if(struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_b>=RJZ_polardemo_case_eighth_y){
        RJZ_polardemo_case_eighth_data++;
    }
    if(struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_c>=RJZ_polardemo_case_eighth_z){  //遵循
        RJZ_polardemo_case_eighth_data++;
    }

    if(struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_a>=RJZ_polardemo_case_eighth_array_Fai[0]){
        RJZ_polardemo_case_eighth_data++;
    }
    if(struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_b>=RJZ_polardemo_case_eighth_array_Fai[1]){
        RJZ_polardemo_case_eighth_data++;
    }
    if(struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_c>=RJZ_polardemo_case_eighth_array_Fai[0]){  //违背8-1
    // CHECK-MESSAGES: :[[@LINE-1]]:79: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_eighth_data++;
    }

    if(struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_a>=struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[NUMTWO]){  //违背8-2
    // CHECK-MESSAGES: :[[@LINE-1]]:79: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_eighth_data++;
    }
    if(struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_b>=struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[NUMONE]){
        RJZ_polardemo_case_eighth_data++;
    }
    if(struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_c>=struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[NUMTWO]){
        RJZ_polardemo_case_eighth_data++;
    }

    if(struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_a>=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[0]){
        RJZ_polardemo_case_eighth_data++;
    }
    if(struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_b>=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[1]){
        RJZ_polardemo_case_eighth_data++;
    }
    if(struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_b>=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[2]){  //违背8-3
    // CHECK-MESSAGES: :[[@LINE-1]]:79: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_eighth_data++;
    }

    if(struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_array_Fai[0]>=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[0]){
        RJZ_polardemo_case_eighth_data++;
    }
    if(struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_array_Fai[1]>=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[1]){
        RJZ_polardemo_case_eighth_data++;
    }
    if(struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_array_Fai[2]>=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[0]){  //违背8-4
    // CHECK-MESSAGES: :[[@LINE-1]]:90: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_eighth_data++;
    }

    if(struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_a>=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[0] && struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_a>=RJZ_polardemo_case_eighth_x){
        RJZ_polardemo_case_eighth_data++;
    }
    if(struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_b>=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[1] && struct_eighth_Psi_variable.struct_eighth_Fai_array[1].RJZ_polardemo_b>=RJZ_polardemo_case_eighth_y){
        RJZ_polardemo_case_eighth_data++;
    }
    if(struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_a>=struct_sixth_Psi_variable.struct_sixth_Fai_variable.RJZ_polardemo_case_sixth_array_Fai[2] && struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_a>=RJZ_polardemo_case_eighth_z){  //违背8-5
    // CHECK-MESSAGES: :[[@LINE-1]]:79: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
    // CHECK-MESSAGES: :[[@LINE-2]]:243: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_eighth_data++;
    }


    /*
      条件语句左右侧比对不一致错误：左右侧为单一量（包括多重条件用“与、或”连接）
      RJZ-1-4-1-case9:左右侧含主结构体为数组型，嵌套数组型子结构体的元素（元素为普通变量或数组元素）
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
    int RJZ_polardemo_case_ninth_x=1,RJZ_polardemo_case_ninth_y=2,RJZ_polardemo_case_ninth_z=3;
    int RJZ_polardemo_case_ninth_data=0;
    int RJZ_polardemo_case_ninth_array_Fai[10];

    if(struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_F<RJZ_polardemo_case_ninth_x){
        RJZ_polardemo_case_ninth_data++;
    }
    if(struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_P<RJZ_polardemo_case_ninth_y){
        RJZ_polardemo_case_ninth_data++;
    }
    if(struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_G<RJZ_polardemo_case_ninth_z){  //遵循
        RJZ_polardemo_case_ninth_data++;
    }

    if(struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[0]<RJZ_polardemo_case_ninth_x){
        RJZ_polardemo_case_ninth_data++;
    }
    if(struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[1]<RJZ_polardemo_case_ninth_y){
        RJZ_polardemo_case_ninth_data++;
    }
    if(struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[2]<RJZ_polardemo_case_ninth_x){  //违背9-1
    // CHECK-MESSAGES: :[[@LINE-1]]:87: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_ninth_data++;
    }

    if(struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[0]<RJZ_polardemo_case_ninth_array_Fai[NUMZERO]){
        RJZ_polardemo_case_ninth_data++;
    }
    if(struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[1]<RJZ_polardemo_case_ninth_array_Fai[NUMONE]){
        RJZ_polardemo_case_ninth_data++;
    }
    if(struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[2]<RJZ_polardemo_case_ninth_array_Fai[NUMZERO]){  //违背9-2
    // CHECK-MESSAGES: :[[@LINE-1]]:87: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_ninth_data++;
    }

    if(struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[0]<struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_a){
        RJZ_polardemo_case_ninth_data++;  
    }
    if(struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[1]<struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_b){
        RJZ_polardemo_case_ninth_data++;  
    }
    if(struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[2]<struct_eighth_Psi_variable.struct_eighth_Fai_array[0].RJZ_polardemo_a){  //违背9-3
    // CHECK-MESSAGES: :[[@LINE-1]]:87: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_ninth_data++;  
    }

    if(struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[0]<struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[0]){
        RJZ_polardemo_case_ninth_data++;  
    }
    else if(struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[1]<struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[1]){
        RJZ_polardemo_case_ninth_data++;  
    }
    else if(struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[2]<struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[0]){  //违背9-4
    // CHECK-MESSAGES: :[[@LINE-1]]:92: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_ninth_data++;  
    }

    if(struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[0]<struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[0] || RJZ_polardemo_case_ninth_x>=struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_y){  //违背9-5
    // CHECK-MESSAGES: :[[@LINE-1]]:201: warning: 左值与右值不匹配 [bz12-variable-name-in-if-analysis]
        RJZ_polardemo_case_ninth_data++;  
    }
    if(struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[1]<struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[1] || RJZ_polardemo_case_ninth_y>=struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_y){
        RJZ_polardemo_case_ninth_data++;  
    }
    if(struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_array_Fai[2]<struct_seventh_Psi_array[0].struct_seventh_Fai_variable.RJZ_polardemo_array_Fai[2] || RJZ_polardemo_case_ninth_z>=struct_ninth_Psi_array[0].struct_ninth_Fai_array[0].RJZ_polardemo_z){
        RJZ_polardemo_case_ninth_data++;  
    }
    
  return (0);
}

