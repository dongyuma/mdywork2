// RUN: %check_clang_tidy %s misrac2012-operator-inappropriate-type %t
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum enum_tag { ENUM_0, ENUM_1, ENUM_2 } enum_t;
static void R_10_1(void)
{
	char buff[128];
	bool bla = true;
	bool blb = false;
	char cha = 'a';
	int ia = 10;
	unsigned int uia = 1;
	int16_t sia = 2;
	int8_t i8a = 1;
	uint8_t ui8a = 1;
	enum_t ena = ENUM_0;
	enum_t enb = ENUM_1;
	float fa = 10.3;


	buff[bla] = 11; //违背1
	buff[cha] = 12; //违背2
	//buff[1.0] = 1;  //编译器支持
	buff[ia] = 13; //遵循
	buff[(int)10.0] = 13; //遵循

	ia = +bla; //违背3
	ia = -bla; //违背4
	ia = +cha; //违背5
	ia = -cha; //违背6
	ia = +ena; //违背7
	ia = -ena; //违背8
	ia = +uia; //遵循
	ia = -uia; //违背9
	ia = -ui8a; //违背10

	ia = bla + 10; //违背11
	bla += 11;     //违背12
	ia = ena - enb; //违背13 x2
	enb += ena;     //违背14 x2
	ui8a += uia;       //遵循
	ia -= i8a;       //遵循

	ia = bla * 10;  //违背15
	ia = cha * 14;  //违背16
	ia = ena * 15;  //违背17
	ui8a *= uia;      //遵循
	bla /= ia;      //违背18
	ia /= cha;      //违背19
	ia /= ena;      //违背20
	ia /= i8a;      //遵循
	ia %= bla;      //违背21
	ia %= cha;      //违背22
	ia %= ena;      //违背23
	ia %= i8a;      //遵循
	//ia = fa % 3;  //编译器支持
	//ia = 10 % fa; //编译器支持

	if (ia < bla || ia > blb || ia <= bla || ia >= blb) //违背24 x4
	{
	}
	if (ia < i8a || ui8a >= uia) //遵循
	{
	}

	if (!bla) //遵循
	{
	}
	if (!ia || !ui8a || !cha || !enb || !fa) //违背25 x5
	{
	}
	if (bla || blb && bla || (!bla && blb && blb)) //遵循
	{
	}
	if (ia && ui8a && cha && enb && fa) //违背26 x5
	{
	}
	if (uia - ui8a || ia) //违背27 x2
	{
	}
	if (ia - i8a == 0 || ia != 0) //遵循
	{
	}
	ia = -(bla && blb);     //违背 28

	ui8a >>= 3;  //遵循
	bla <<= 3; //违背29
	ia >>= 4; //违背30
	enb << 5; //违背31
	cha >> 2; //违背32
	//fa >> 2; //编译器支持

	ui8a << ui8a; //遵循
	ui8a >>= bla; //违背33
	ui8a <<= ia;  //违背34
	ui8a >> enb;  //违背35
	ui8a << cha;  //违背36
	//ui8a >> fa;   //编译器支持

	~0x3; //遵循
	~ui8a; //遵循
	~ia;   //违背37
	~bla;  //违背38
	~cha;  //违背39
	~ena;  //违背40
	//~fa;   //编译器支持

	0xF & 0x1;  //遵循
	ui8a &uia; //遵循
	bla &cha;  //违背41 x2
	enb &ia;   //违背42 x2
	//ui8a & fa; //编译器支持

	0xF | 0x1;  //遵循
	ui8a | uia; //遵循
	bla | cha;  //违背43 x2
	enb | ia;   //违背44 x2
	//ui8a | fa; //编译器支持

	0xF ^ 0x1;  //遵循
	ui8a ^uia; //遵循
	bla ^cha;  //违背45 x2
	enb ^ia;   //违背46 x2
	//ui8a ^ fa; //编译器支持

	ia = bla ? ui8a : cha; //遵循
	ia = ia == 3 ? ui8a : cha; //遵循
	ia = cha ? ui8a : cha; //违背47
	ia = ena ? ui8a : cha; //违背48
	ia = ia ? ui8a : cha;  //违背49
	ia = ui8a ? ui8a : cha; //违背50
	ia = fa ? ui8a : cha;   //违背51
}


void R_10_2(void)
{
	uint8_t u8a = 10;
	int8_t s8a = 11;
	char cha;
	int16_t s16a = 12;
	int32_t s32a;
	float fla = 10.3;
	enum { a1, a2, a3 } ena = a2;

	cha = '0' + u8a;  //遵循
	cha = s8a + '0';  //遵循
	s32a = cha - '0'; //遵循
	cha = '0' - s8a;  //遵循
	cha++;            //遵循

	//The following also break R.10.4
	s16a = s16a - 'a';  //违背52
	cha = '0' + fla;    //违背53
	cha = cha + ':';    //违背54
	cha = cha - ena;    //违背55 x2
}


void R_10_4(void)
{
	uint8_t u8a = 10;
	uint8_t u8b = 11;
	uint16_t u16b = 12;
	int8_t s8a = 13;
	char cha = 14;

	enum EA { a1, a2, a3 } ena = a2;
	enum EB { b1, b2, b3 } enb = b2;

	if (ena > a1)  //遵循
	{
		; //no action
	}
	if (b3 < enb || b3 > b1)  //遵循
	{
		; //no action
	}
	u16b = u8a + u16b;   //遵循

	cha += u8a;          //遵循

	s8a += u8a;          //违背56
	//also breaks R.10.3

	u8b = u8b + 2;      //违背57

	if (enb > a1)       //违背58
	{
		; // no action
	}
	if (ena == enb)    //违背59
	{
		; // no action
	}
	u8a += cha;        //遵循
}

void R_10_7(void)
{
	uint16_t u16a = 1;
	uint16_t u16b = 2;

	uint32_t u32a = 3;
	uint32_t u32b = 4;

	u32a = u32a * u16a + u16b;               //遵循
	u32a = (u32a * u16a) + u16b;             //遵循
	u32a = u32a * ((uint32_t)u16a + u16b);   //遵循
	u32a += (u32b + u16b);                   //遵循

	u32a = u32a * (u16a + u16b); //违背60
	u32a += (u16a + u16b);       //违背61
}