#pragma once

#include <algorithm>
#include <stdio.h>
#include <string>
#include <string.h>
#include <time.h>

using namespace std;

#pragma pack(1)

#define INVALID_VAL						0xFF			//锟角凤拷锟斤拷锟斤拷
#define MAX_TOTAL_TYPE					34				//锟介将锟斤拷锟斤拷锟斤拷
#define MIN_HU_HUAN_CUN					5				//锟斤拷小锟斤拷锟芥长锟斤拷

enum enPlayMode
{
	enPlayMode_ZhuangFen,		//庄锟斤拷锟?
	enPlayMode_7Dui,			//锟缴猴拷锟竭讹拷
	enPlayMode_NaiZi,			//锟斤拷锟斤拷锟斤拷锟斤拷
	enPlayMode_Max,
};

//锟斤拷锟斤拷锟斤拷示锟斤拷锟?
enum enCountFlag
{
	enCountFlag_7Dui = 0x0001,
	enCountFlag_KeZiFirst = 0x0002,
	enCountFlag_13Lan = 0x0004,
};

//一锟斤拷锟斤拷锟斤拷锟斤拷
enum BLOCK_TYPE_HU
{
	BLOCK_22 = 1,				    //锟斤拷锟斤拷
	BLOCK_234,						//顺锟斤拷 锟斤拷锟斤拷 锟斤拷锟斤拷 锟结构
	BLOCK_222,						//锟斤拷锟斤拷 锟斤拷锟斤拷 锟斤拷锟斤拷 锟结构, 3锟斤拷锟斤拷同
	BLOCK_2222_DIAN,   				//锟斤拷锟斤拷--锟斤拷锟?4锟斤拷锟斤拷同
	BLOCK_2222_BU,   				//锟斤拷锟斤拷--锟斤拷锟斤拷,4锟斤拷锟斤拷同
	BLOCK_2222_AN,					//锟斤拷锟斤拷--锟斤拷锟斤拷,4锟斤拷锟斤拷同	
	BLOCK_MAX,
};


#define MAX_VAL_NUM						9
#define MAX_KEY_NUM						(MAX_VAL_NUM+1)		//9+锟斤拷锟斤拷
//锟介将锟斤拷色锟斤拷锟斤拷锟洁）锟斤拷锟斤拷
enum enColorMJ
{
	enColorMJ_WAN = 0,  //锟斤拷
	enColorMJ_TONG,     //筒
	enColorMJ_TIAO,     //锟斤拷
	enColorMJ_FenZi,    //锟界、锟斤拷 锟斤拷
	enColorMJ_Max,
};
//////////////////////////////////////////////////////////////
//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷值
static const BYTE s_HuCardType[] =
{
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,				//锟斤拷
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,				//锟斤拷
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,				//锟斤拷
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37                            //锟斤拷锟斤拷锟斤拷锟斤拷锟叫凤拷锟斤拷
};

//////////////////////////////////////////////////////////////
//锟斤拷锟斤拷值锟矫碉拷锟斤拷锟斤拷值
inline BYTE getIndexByVal(BYTE byCard)
{
	BYTE cor = byCard / 0x10;
	BYTE val = byCard % 0x10;

	int  nMaxIndex = cor == 3 ? 7 : 9;
	BYTE byIndex = INVALID_VAL;

	if (val > 0 && val <= 9 && cor <= 3)
		byIndex = (cor) * 9 + val - 1;

	return byIndex;
}
//锟斤拷锟斤拷锟斤拷值锟矫碉拷锟斤拷值
inline BYTE getValByIndex(BYTE byIndex)
{
	if (byIndex < MAX_TOTAL_TYPE)
	{
		return s_HuCardType[byIndex];
	}
	return INVALID_VAL;
}
//锟斤拷锟角凤拷锟角合凤拷锟斤拷值
inline bool IsValidCard(BYTE bCard)
{
	return getIndexByVal(bCard) != INVALID_VAL;
}

struct stColorData
{
	BYTE	byCorType;					// enColorMJ
	BYTE	byNum;						// 锟斤拷锟斤拷锟斤拷
	BYTE	byCount[MAX_KEY_NUM];		// 统锟斤拷锟斤拷锟斤拷

	stColorData() { memset(this, 0, sizeof(*this)); }
	stColorData(BYTE byType, BYTE byVal[MAX_KEY_NUM]) :byCorType(byType), byNum(0)
	{
		memcpy(byCount, byVal, MAX_KEY_NUM);
		for (int i = 0; i<MAX_KEY_NUM; ++i)
			byNum += byCount[i];
	}
};

struct stCardData
{
	BYTE	byCardNum[MAX_TOTAL_TYPE];			// 统锟斤拷锟斤拷锟斤拷
	BYTE	byNum;								// 锟斤拷锟斤拷
	BYTE	byHuCard;							// 锟斤拷锟斤拷锟斤拷锟斤拷锟狡ｏ拷锟斤拷锟斤拷锟斤拷途锟斤拷

	stCardData()
	{
		memset(this, 0, sizeof(*this));
	}
	stCardData(BYTE byCards[], BYTE num) :byNum(num)
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
		for (int i = 0; i<MAX_TOTAL_TYPE; ++i)
		{
			if (byCardNum[i] > 0 && byCardNum[i] + byIndex <= num)
			{
				memset(byCardOut + byIndex, getValByIndex(i), byCardNum[i]);
				byIndex += byCardNum[i];
			}
		}
		if (bAddHuCard && byHuCard != 0 && byIndex<num)
			byCardOut[byIndex] = byHuCard;
	}
};

#pragma pack(1)
