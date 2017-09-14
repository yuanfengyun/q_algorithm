/********************************************************************
created:	pinorr
file base:	HuPaiMJ.h
author:		pinorr	Q 505971450
purpose:	麻将胡牌提示算法(第3版)
*********************************************************************/

#ifndef __HU_PAI_MJ2_H__
#define __HU_PAI_MJ2_H__
#include <set>
#include <string>
#include <vector>
#include <windows.h>
#include <iostream>
#include <unordered_map>  

using namespace std;

//麻将颜色（种类）定义
enum enColorMJ
{
	enColorMJ_WAN = 0,  //万
	enColorMJ_TONG,     //筒
	enColorMJ_TIAO,     //条
	enColorMJ_FenZi,    //风、字 牌
	enColorMJ_Max,
};

#define MAX_TOTAL_TYPE					34
#define MAX_VAL_NUM						9
#define MAX_KEY_NUM						(MAX_VAL_NUM+1)		//9+赖子
#define MAX_NAI_NUM						4					//赖子最大个数
#define BIT_VAL_NUM						3					//一个值占的bit数
#define BIT_VAL_FLAG					0x07				//

inline int getKeyByIndex(BYTE byIndex[MAX_KEY_NUM], BYTE byNum = MAX_KEY_NUM)
{
	int nKey = 0;
	for (int i = 0; i < byNum; ++i)
		nKey |= (int)(byIndex[i] & BIT_VAL_FLAG) << (BIT_VAL_NUM*i);
	return nKey;
}
inline bool isValidKey(int llVal)
{
	BYTE byIndex[MAX_KEY_NUM] = {};
	for (int i = 0; i < MAX_KEY_NUM; ++i)
		byIndex[i] = (llVal >> (BIT_VAL_NUM*i))&BIT_VAL_FLAG;

	if (byIndex[9] > MAX_NAI_NUM)	return false;
	int nNum = 0;
	for (int i = 0; i < MAX_KEY_NUM; ++i)
	{
		nNum += byIndex[i];
		if (byIndex[i] > 4 || nNum > 14)	//
			return false;
	}
	return nNum > 0;
}
inline BYTE getNumByKey(int llVal, BYTE byNum = MAX_KEY_NUM)
{
	BYTE byIndex[MAX_KEY_NUM] = {};
	for (int i = 0; i < MAX_KEY_NUM; ++i)
		byIndex[i] = (llVal >> (BIT_VAL_NUM*i))&BIT_VAL_FLAG;

	BYTE nNum = 0;
	for (int i = 0; i < byNum; ++i)
		nNum += byIndex[i];
	return nNum;
}
inline void addMap(unordered_map<int, BYTE> mapTemp[], int llVal)
{
	BYTE nNum  = getNumByKey(llVal, MAX_VAL_NUM);
	BYTE byNum = (llVal >> (BIT_VAL_NUM * 9))&BIT_VAL_FLAG;
	int  val = (llVal & 0x7FFFFFF);
	unordered_map<int, BYTE>::iterator iter = mapTemp[nNum].find(val);
	if (iter != mapTemp[nNum].end())
		iter->second = min(byNum, iter->second);
	else
		mapTemp[nNum][val] = byNum;
}

set<int>					g_setSingle;		//单个顺子+刻子		50个
set<int>					g_setSingleFZ;		//单个顺子+刻子		22个
set<int>					g_setSingleJiang;	//单个将			19个
set<int>					g_setSingleJiangFZ;	//单个将			15个

unordered_map<int, BYTE>			g_mapHuAll[15];
unordered_map<int, BYTE>			g_mapHuAllFZ[15];

class CHuPaiMJ
{
private:
	static void TrainSingle()
	{
		BYTE byTemp[MAX_KEY_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 };
		g_setSingle.insert(getKeyByIndex(byTemp));
		g_setSingleFZ.insert(getKeyByIndex(byTemp));
		// 1.1 刻子
		for (int i = 0; i < MAX_VAL_NUM; ++i)
		{
			memset(byTemp, 0, MAX_KEY_NUM);
			for (int n = 0; n < 3; ++n)
			{
				byTemp[i] = 3 - n;	byTemp[9] = n;
				g_setSingle.insert(getKeyByIndex(byTemp));
				if (i < 7)	//风、字牌最多7张
					g_setSingleFZ.insert(getKeyByIndex(byTemp));
			}
		}
		// 1.2 顺子 没赖子
		for (int i = 0; i < MAX_VAL_NUM - 2; ++i)
		{
			memset(byTemp, 0, MAX_KEY_NUM);
			byTemp[i] = 1;	byTemp[i + 1] = 1;	byTemp[i + 2] = 1;
			g_setSingle.insert(getKeyByIndex(byTemp));
		}
		// 1.3 顺子 1个赖子 (2个赖子时也就是刻子)
		for (int i = 0; i < MAX_VAL_NUM - 2; ++i)
		{				
			for (int n = 0; n < 3; ++n)
			{
				memset(byTemp, 0, MAX_KEY_NUM);
				byTemp[i] = 1;	byTemp[i + 1] = 1;	byTemp[i + 2] = 1;
				byTemp[i + n] = 0;	byTemp[9] = 1;
				g_setSingle.insert(getKeyByIndex(byTemp));
			}			
		}
		// 2.1 将牌
		memset(byTemp, 0, MAX_KEY_NUM);
		byTemp[9] = 2;
		g_setSingleJiang.insert(getKeyByIndex(byTemp));
		g_setSingleJiangFZ.insert(getKeyByIndex(byTemp));
		for (int i = 0; i < MAX_VAL_NUM; ++i)
		{
			memset(byTemp, 0, MAX_KEY_NUM);
			for (int n = 0; n < 2; ++n)
			{
				byTemp[i] = 2 - n;	byTemp[9] = n;
				g_setSingleJiang.insert(getKeyByIndex(byTemp));
				if (i < 7)	//风、字牌最多7张
					g_setSingleJiangFZ.insert(getKeyByIndex(byTemp));
			}
		}
	};

	static void TrainAllComb(const set<int> &setSingle, unordered_map<int, BYTE> mapOut[])
	{
		int nAll = 0;
		int nSingle[100] = {};
		set<int>::iterator iter = setSingle.begin();
		for (; iter != setSingle.end(); ++iter)
			nSingle[nAll++] = *iter;

		for (int i1 = 0; i1 < nAll; ++i1)
		{
			addMap(mapOut, nSingle[i1]);
			for (int i2 = i1; i2 < nAll; ++i2)
			{
				int nTemp = nSingle[i1] + nSingle[i2];
				if (!isValidKey(nTemp))	continue;
				addMap(mapOut, nTemp);
				for (int i3 = i2; i3 < nAll; ++i3)
				{
					int nTemp = nSingle[i1] + nSingle[i2] + nSingle[i3];
					if (!isValidKey(nTemp))	continue;
					addMap(mapOut, nTemp);
					for (int i4 = i3; i4 < nAll; ++i4)
					{
						int nTemp = nSingle[i1] + nSingle[i2] + nSingle[i3] + nSingle[i4];
						if (!isValidKey(nTemp))	continue;
						addMap(mapOut, nTemp);
					}
				}
			}
		}
	}

	static void TrainAllComb_Jiang(const set<int> &setSingle, unordered_map<int, BYTE> mapOut[])
	{
		int nAll = 0;
		int nSingle[100] = {};

		set<int>::iterator iter = setSingle.begin();
		for (; iter != setSingle.end(); ++iter)
			nSingle[nAll++] = *iter;

		unordered_map<int, BYTE> mapTemp[15];
		for (int j = 0; j < 15; ++j)
			mapTemp[j] = mapOut[j];

		for (int i = 0; i < nAll; ++i)
		{
			for (int j = 0; j < 15; ++j)
			{
				addMap(mapOut, nSingle[i]);
				unordered_map<int, BYTE>::iterator iter_u = mapTemp[j].begin();
				for (; iter_u != mapTemp[j].end(); ++iter_u)
				{
					int nTemp = nSingle[i] + iter_u->first + (int(iter_u->second & BIT_VAL_FLAG) << 27);
					if (isValidKey(nTemp))
						addMap(mapOut, nTemp);
				}
			}
		}
	}

	static bool CheckCanHuSingle(BYTE byType, BYTE byCards[], BYTE &byNaiNum, BYTE byNaiMax)
	{
		bool bSuc = false;
		int	 nVal = getKeyByIndex(byCards, MAX_VAL_NUM);
		if (byType == enColorMJ_FenZi)
			nVal &= 0x1FFFFF;

		BYTE nNum = getNumByKey(nVal, MAX_VAL_NUM);
		if (byType == enColorMJ_FenZi)
		{
			unordered_map<int, BYTE>::iterator iterFind = g_mapHuAllFZ[nNum].find(nVal);
			if (iterFind != g_mapHuAllFZ[nNum].end())
			{
				byNaiNum = iterFind->second;
				if (iterFind->second <= byNaiMax)
					return true;
			}
		}
		else
		{
			unordered_map<int, BYTE>::iterator iterFind = g_mapHuAll[nNum].find(nVal);
			if (iterFind != g_mapHuAll[nNum].end())
			{
				byNaiNum = iterFind->second;
				if (iterFind->second <= byNaiMax)
					return true;
			}
		}
		byNaiNum = 0;
		return false;
	}

public:
	static void TrainAll()
	{
		if (g_setSingle.empty())
		{
			DWORD dwFlag = GetTickCount();
			TrainSingle();
			TrainAllComb(g_setSingle, g_mapHuAll);
			TrainAllComb(g_setSingleFZ, g_mapHuAllFZ);
			TrainAllComb_Jiang(g_setSingleJiang, g_mapHuAll);
			TrainAllComb_Jiang(g_setSingleJiangFZ, g_mapHuAllFZ);
			cout << "train cost:" << GetTickCount()-dwFlag << "ms" << endl;
		}	
		else
			cout << "already trained!" << endl;
	}	

	static bool CheckCanHu(BYTE byCardSrc[], BYTE byNaiIndex)
	{
		if (g_setSingle.empty())
			TrainAll();

		BYTE byCards[MAX_TOTAL_TYPE];
		memcpy(byCards, byCardSrc, MAX_TOTAL_TYPE);
		int nNaiZiNum = 0;
		if (byNaiIndex < MAX_TOTAL_TYPE)
		{
			nNaiZiNum = byCards[byNaiIndex];
			byCards[byNaiIndex] = 0;
		}

		BYTE byJiangNum = 0;
		BYTE nNaiTry[enColorMJ_Max] = {};
		for (int cor = 0; cor < enColorMJ_Max; ++cor)
		{
			int nMax = (cor == enColorMJ_FenZi) ? 7 : 9;
			int nCardAll = 0;
			for (int i = 0; i < nMax; ++i)
				nCardAll += byCards[9 * cor + i];

			if (nCardAll == 0) continue;
			if (!CheckCanHuSingle(cor, byCards + 9 * cor, nNaiTry[cor], nNaiZiNum))
				return false;

			nNaiZiNum -= nNaiTry[cor];
			byJiangNum += ((nCardAll + nNaiTry[cor]) % 3 == 2);
			if (byJiangNum > nNaiZiNum + 1)
				return false;
		}
		return byJiangNum > 0 || nNaiZiNum >= 2;
	}
};

#endif //__HU_PAI_MJ2_H__
