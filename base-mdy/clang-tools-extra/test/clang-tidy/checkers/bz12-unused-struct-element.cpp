#include <stdio.h>
#include <string.h>
//******************************应能够识别常用类型名******************************//
typedef double 			REAL64;
typedef float 			REAL32;
typedef	unsigned char		BYTE;
typedef unsigned int    	UINT32;
typedef signed int      	INT32;
typedef unsigned short int   	WORD;

//******************************应能够识别宏定义******************************//

#define NUMZERO       0
#define NUMONE        1 
#define NUMTWO        2
#define TOTALNUM      3
//******************************子结构体1定义******************************//
typedef struct  RJZ_pol_demo_childone
{
REAL64 cox;
REAL64 coy;
REAL64 coz;
REAL64 co[TOTALNUM];
REAL64 ct[TOTALNUM];
}ST_CHILD_ONE_STR;


//******************************子结构体2定义******************************//
typedef struct  RJZ_pol_demo_childtwo
{
REAL64 cox;
REAL64 coy;
REAL64 coz;
REAL64 co[TOTALNUM];
REAL64 ct[TOTALNUM];
}ST_CHILD_TWO_STR;

//******************************复杂结构体（含数组元素）******************************//
typedef struct  RJZ_pol_demo
{
REAL64  dname_a[TOTALNUM];
REAL64  dname_b[3];
REAL64  dname_c[TOTALNUM][3];
INT32   dname_d[3][TOTALNUM];
INT32   aa;
INT32   bb;


ST_CHILD_ONE_STR child_a;
ST_CHILD_ONE_STR child_b;

ST_CHILD_TWO_STR child2_a[3];
ST_CHILD_TWO_STR child2_b[NUMTWO+1];


}ST_RJZ_TOTAL_DEM;


//******************************简单变量、数组定义******************************//
REAL32  RJZ_polardemo_1 = 0.0;
REAL32  RJZ_polardemo_2 = 0.0;
REAL32  RJZ_polardemo_3 = 0.0;

REAL32  RJZ_polardemo_x = 0.0;
REAL32  RJZ_polardemo_y = 0.0;
REAL32  RJZ_polardemo_z = 0.0;

REAL32  RJZ_polardemo[3] = {0.0,0.0,0.0};
REAL32  RJZ_polardata[3] = {0.0,0.0,0.0};
REAL32  RJZ_polardemodouble[3][3] = {{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0}};	

REAL32  RJZ_polardemo_Fai = 0.0;
REAL32  RJZ_polardemo_Psi = 0.0;
REAL32  RJZ_polardemo_Gam = 0.0;

//******************************结构体与数组型结构体的定义******************************//


int main()
{
//RJZ-1-8-1-case1,未初始化内容为整个结构体

    //遵循示例RJZ-1-8-1-case1-Y-1
    ST_RJZ_TOTAL_DEM rjz_case1_Y_1;
    memset(&rjz_case1_Y_1, 1, sizeof(rjz_case1_Y_1));

    //遵循示例RJZ-1-8-1-case1-Y-2
    ST_RJZ_TOTAL_DEM rjz_case1_Y_2;
    memcpy(&rjz_case1_Y_2, &rjz_case1_Y_1, sizeof(ST_RJZ_TOTAL_DEM));

    //遵循示例RJZ-1-8-1-case1-Y-3
    ST_RJZ_TOTAL_DEM rjz_case1_Y_3 = rjz_case1_Y_1;

    //遵循示例RJZ-1-8-1-case1-Y-4     
    ST_RJZ_TOTAL_DEM rjz_case1_Y_4;
    rjz_case1_Y_4 = rjz_case1_Y_1;

    //违背示例RJZ-1-8-1-case1-N-1     
    ST_RJZ_TOTAL_DEM rjz_case1_N_1;

//RJZ-1-8-1-case2,未初始化内容为结构体子变量

    //遵循示例RJZ-1-8-1-case2-Y-1
    ST_RJZ_TOTAL_DEM rjz_test_1;
    memset(&rjz_test_1.aa, 1, sizeof(rjz_test_1.aa));
    
    //遵循示例RJZ-1-8-1-case2-Y-2
    ST_RJZ_TOTAL_DEM rjz_test_2;
    memcpy(&rjz_test_2.aa, &rjz_test_1.aa, sizeof(rjz_test_2.aa));

    //遵循示例RJZ-1-8-1-case2-Y-3
    ST_RJZ_TOTAL_DEM rjz_test_2;
    rjz_test_2.bb = rjz_test_1.aa;
    
    //违背示例RJZ-1-8-1-case2-N-1
    //rjz_test_1.bb未初始化

//RJZ-1-8-1-case3,未初始化内容为结构体子数组元素

    //遵循示例RJZ-1-8-1-case3-Y-1
    memset(&rjz_test_1.dname_a, 1, sizeof(rjz_test_1.dname_a));
    
    //遵循示例RJZ-1-8-1-case3-Y-2
    memcpy(&rjz_test_2.dname_a, &rjz_test_1.dname_a, sizeof(rjz_test_2.dname_a));

    //遵循示例RJZ-1-8-1-case3-Y-3
    for(int i=0; i < TOTALNUM; i++)
    {
        rjz_test_2.dname_b[i] = rjz_test_1.dname_b[i];
    }
 
    //违背示例RJZ-1-8-1-case3-N-1
    //rjz_test_1.dname_b未初始化

    //遵循示例RJZ-1-8-1-case3-Y-4
    memset(&rjz_test_1.dname_c, 1, sizeof(rjz_test_1.dname_c));
    
    //遵循示例RJZ-1-8-1-case3-Y-5
    memcpy(&rjz_test_2.dname_c, &rjz_test_1.dname_c, sizeof(rjz_test_2.dname_c));

    //遵循示例RJZ-1-8-1-case3-Y-6
    for(int i=0; i < TOTALNUM; i++)
    {
        for(int j=0; j < TOTALNUM; j++)
        {
            rjz_test_2.dname_d[i][j] = rjz_test_1.dname_c[i][j];
        }
    }
 
    //违背示例RJZ-1-8-1-case3-N-2
    //rjz_test_1.dname_d未初始化

//RJZ-1-8-1-case4,未初始化内容为结构体子结构体元素
    
    //遵循示例RJZ-1-8-1-case4-Y-1
    memset(&rjz_test_1.child_a, 1, sizeof(rjz_test_1.child_a));
    
    //遵循示例RJZ-1-8-1-case4-Y-2
    memcpy(&rjz_test_2.child_a, &rjz_test_1.child_a, sizeof(rjz_test_2.child_a));

    //遵循示例RJZ-1-8-1-case4-Y-3
    rjz_test_2.child_b = rjz_test_1.child_a;
 
    //违背示例RJZ-1-8-1-case4-N-1
    //rjz_test_1.child_b未初始化

    //遵循示例RJZ-1-8-1-case4-Y-4
    memset(&rjz_test_1.child2_a, 1, sizeof(rjz_test_1.child2_a));
    
    //遵循示例RJZ-1-8-1-case4-Y-5
    memcpy(&rjz_test_2.child2_a, &rjz_test_1.child2_a, sizeof(rjz_test_2.child2_a));

    //遵循示例RJZ-1-8-1-case4-Y-6
    for(int i=0; i < TOTALNUM; i++)
    {
        rjz_test_2.child2_b[i] = rjz_test_1.child2_a[i];
    }
 
    //违背示例RJZ-1-8-1-case4-N-1
    //rjz_test_1.child2_b未初始化
}
