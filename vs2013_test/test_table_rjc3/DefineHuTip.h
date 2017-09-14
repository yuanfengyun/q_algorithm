#pragma once

#include <algorithm>
#include <stdio.h>
#include <string>
#include <string.h>
#include <time.h>

using namespace std;

#pragma pack(1)

#define INVALID_VAL						0xFF			//非法数据
#define MAX_TOTAL_TYPE					34				//麻将种类数
#define MIN_HU_HUAN_CUN					5				//最小缓存长度

enum enPlayMode
{
	enPlayMode_ZhuangFen,		//庄算分
	enPlayMode_7Dui,			//可胡七对
	enPlayMode_NaiZi,			//红中赖子
	enPlayMode_Max,
};

//胡牌提示相关
enum enCountFlag
{
	enCountFlag_7Dui		= 0x0001,
	enCountFlag_KeZiFirst	= 0x0002,
	enCountFlag_13Lan		= 0x0004,
};

//一组牌类型
enum BLOCK_TYPE_HU
{	
	BLOCK_22 = 1,				    //对子
	BLOCK_234,						//顺序 或者 吃牌 结构
	BLOCK_222,						//刻子 或者 碰子 结构, 3个相同
	BLOCK_2222_DIAN,   				//杠子--点杠,4个相同
	BLOCK_2222_BU,   				//杠子--补杠,4个相同
	BLOCK_2222_AN ,					//杠子--暗杠,4个相同	
	BLOCK_MAX,
};


#define MAX_VAL_NUM						9
#define MAX_KEY_NUM						(MAX_VAL_NUM+1)		//9+赖子
//麻将颜色（种类）定义
enum enColorMJ
{
	enColorMJ_WAN = 0,  //万
	enColorMJ_TONG,     //筒
	enColorMJ_TIAO,     //条
	enColorMJ_FenZi,    //风、字 牌
	enColorMJ_Max,
};
//////////////////////////////////////////////////////////////
//所有牌类型值
static const BYTE s_HuCardType[] = 
{
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,				//万
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,				//饼
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,				//条
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37                            //东南西北中发白
};

//////////////////////////////////////////////////////////////
//由牌值得到索引值
inline BYTE getIndexByVal(BYTE byCard)
{
	BYTE cor = byCard / 0x10;
	BYTE val = byCard % 0x10;

	int  nMaxIndex = cor==3 ? 7 : 9;
	BYTE byIndex = INVALID_VAL;

	if (val > 0 && val <= 9 && cor <= 3) 
		byIndex = (cor)*9 + val-1;
		
	return byIndex;	
}
//由索引值得到牌值
inline BYTE getValByIndex(BYTE byIndex)
{
	if (byIndex < MAX_TOTAL_TYPE)
	{
		return s_HuCardType[byIndex];
	}
	return INVALID_VAL;
}
//由是否是合法牌值
inline bool IsValidCard(BYTE bCard)
{	
	return getIndexByVal(bCard) != INVALID_VAL;
}

struct stColorData
{
	BYTE	byCorType;					// enColorMJ
	BYTE	byNum;						// 牌数量
	BYTE	byCount[MAX_KEY_NUM];		// 统计数据

	stColorData(){memset(this, 0, sizeof(*this));}	
	stColorData(BYTE byType, BYTE byVal[MAX_KEY_NUM]):byCorType(byType),byNum(0)
	{ 
		memcpy(byCount, byVal, MAX_KEY_NUM);
		for (int i=0; i<MAX_KEY_NUM; ++i)
			byNum += byCount[i];
	}
};

struct stCardData
{
	BYTE	byCardNum[MAX_TOTAL_TYPE];			// 统计数据
	BYTE	byNum;								// 数量
	BYTE	byHuCard;							// 胡的那张牌（特殊用途）

	stCardData()
	{
		memset(this, 0, sizeof(*this));
	}	
	stCardData(BYTE byCards[], BYTE num):byNum(num)
	{
		memcpy(byCardNum, byCards, MAX_TOTAL_TYPE);
	}

	void clear()
	{
		memset(this, 0, sizeof(*this));
	}
	void getCardsData(BYTE byCardOut[], BYTE num, bool bAddHuCard = false)
	{
		BYTE byIndex = 0;
		for (int i=0; i<MAX_TOTAL_TYPE; ++i)
		{
			if (byCardNum[i] > 0 && byCardNum[i]+byIndex <= num)
			{
				memset(byCardOut+byIndex, getValByIndex(i), byCardNum[i]);
				byIndex += byCardNum[i];
			}
		}
		if (bAddHuCard && byHuCard != 0 && byIndex<num)
			byCardOut[byIndex] = byHuCard;
	}	
};

#pragma pack(1)