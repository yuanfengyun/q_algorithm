/********************************************************************
created:	pinorr
file base:	HuPaiMJ.h
author:		pinorr	Q 505971450
purpose:	麻将胡牌提示算法(第2版)
*********************************************************************/

#ifndef __HU_PAI_MJ_H__
#define __HU_PAI_MJ_H__
#include <map>
#include <set>
#include <unordered_set>
#include <string>
#include <vector>
#include <list>
#include <windows.h>
#include <iostream>
#include <hash_map>  
#include "DefineHuTip.h"

using namespace std;

#define MAX_KEY_NUM						(MAX_VAL_NUM+1)		//9+赖子
#define MAX_NAI_NUM						4					//赖子最大个数
#define BIT_VAL_NUM						3					//一个值占的bit数
#define BIT_VAL_FLAG					0x07				//

struct stCorOut
{
	BYTE byCorType;					//
	bool byNum;						// 总张数
	BYTE byValNum[MAX_KEY_NUM];		//
};

inline void getIndexByKey2(int llVal, BYTE byIndex[MAX_KEY_NUM])
{
	for (int i = 0; i < MAX_KEY_NUM; ++i)
	{
		byIndex[i] = (llVal >> (BIT_VAL_NUM*i))&BIT_VAL_FLAG;
	}
}

inline int getKey2ByIndex(BYTE byIndex[MAX_KEY_NUM])
{
	int nKey = 0;
	for (int i = 0; i<MAX_KEY_NUM; ++i)
		nKey |= (int)(byIndex[i] & BIT_VAL_FLAG) << (BIT_VAL_NUM*i);

	return nKey;
}
inline int getKeyByIndex(BYTE byIndex[MAX_VAL_NUM])
{
	int nKey = 0;
	for (int i = 0; i < MAX_VAL_NUM; ++i)
		nKey |= (int)(byIndex[i] & BIT_VAL_FLAG) << (BIT_VAL_NUM*i);

	return nKey;
}

inline bool isValidKey(int llVal)
{
	BYTE byIndex[MAX_KEY_NUM] = {};
	getIndexByKey2(llVal, byIndex);
	if (byIndex[9] > MAX_NAI_NUM)
		return false;

	int nNum = 0;
	for (int i = 0; i < MAX_KEY_NUM; ++i)
	{
		nNum += byIndex[i];
		if (byIndex[i] > 4 || nNum > 14)	//
			return false;
	}
	return nNum > 0;
}
inline BYTE getNumByKey(int llVal)
{
	BYTE byIndex[MAX_KEY_NUM] = {};
	getIndexByKey2(llVal, byIndex);
	BYTE nNum = 0;
	for (int i = 0; i < MAX_KEY_NUM; ++i)
		nNum += byIndex[i];

	return nNum;
}
inline BYTE getNumByKey2(int llVal)
{
	BYTE byIndex[MAX_KEY_NUM] = {};
	getIndexByKey2(llVal, byIndex);
	BYTE nNum = 0;
	for (int i = 0; i < MAX_VAL_NUM; ++i)
		nNum += byIndex[i];

	return nNum;
}
inline BYTE getNaiByKey(int llVal)
{
	return (llVal >> (BIT_VAL_NUM * 9))&BIT_VAL_FLAG;
}
inline int getKeyNoNai(int llVal)
{
	return llVal & 0x7FFFFFF;
}
inline void addMap(hash_map<int, BYTE> mapTemp[], int llVal)
{
	BYTE nNum = getNumByKey2(llVal);
	BYTE byNum = getNaiByKey(llVal);
	int  val = getKeyNoNai(llVal);
	hash_map<int, BYTE>::iterator iter = mapTemp[nNum].find(val);
	if (iter != mapTemp[nNum].end())
		iter->second = min(byNum, iter->second);
	else
		mapTemp[nNum][val] = byNum;
}

set<int>				g_setSingle;		//单个顺子+刻子		50个
set<int>				g_setSingleFZ;		//单个顺子+刻子		22个
set<int>				g_setSingleJiang;	//单个将			19个
set<int>				g_setSingleJiangFZ;	//单个将			15个

// unordered_set<int>		g_usetHuAll[15];			// 
// unordered_set<int>		g_usetHuAllFZ[15];			// 风、字牌

hash_map<int, BYTE>			g_mapHuAll[15];
hash_map<int, BYTE>			g_mapHuAllFZ[15];


class CHuPaiMJ
{
public:
	CHuPaiMJ() {};
	~CHuPaiMJ() {};

public:
	static void TrainSingle()			// 1.三张单独组合
	{
		BYTE byTemp[MAX_KEY_NUM] = {};
		byTemp[9] = 3;
		g_setSingle.insert(getKey2ByIndex(byTemp));
		g_setSingleFZ.insert(getKey2ByIndex(byTemp));
		// 1.1 刻子
		for (int i = 0; i < MAX_VAL_NUM; ++i)
		{
			memset(byTemp, 0, MAX_KEY_NUM);
			for (int n = 0; n < 3; ++n)
			{
				byTemp[i] = 3 - n;
				byTemp[9] = n;
				g_setSingle.insert(getKey2ByIndex(byTemp));
				if (i < 7)	//风、字牌最多7张
					g_setSingleFZ.insert(getKey2ByIndex(byTemp));
			}
		}
		// 1.2 顺子 没赖子
		for (int i = 0; i < MAX_VAL_NUM - 2; ++i)
		{
			memset(byTemp, 0, MAX_KEY_NUM);
			byTemp[i] = 1;
			byTemp[i + 1] = 1;
			byTemp[i + 2] = 1;
			g_setSingle.insert(getKey2ByIndex(byTemp));
		}
		// 1.2 顺子 1个赖子 (2个赖子时也就是刻子)
		for (int i = 0; i < MAX_VAL_NUM - 2; ++i)
		{
			memset(byTemp, 0, MAX_KEY_NUM);
			byTemp[i] = 1;
			byTemp[i + 1] = 1;
			byTemp[9] = 1;
			g_setSingle.insert(getKey2ByIndex(byTemp));
			memset(byTemp, 0, MAX_KEY_NUM);
			byTemp[9] = 1;
			byTemp[i + 1] = 1;
			byTemp[i + 2] = 1;
			g_setSingle.insert(getKey2ByIndex(byTemp));
			memset(byTemp, 0, MAX_KEY_NUM);
			byTemp[i] = 1;
			byTemp[9] = 1;
			byTemp[i + 2] = 1;
			g_setSingle.insert(getKey2ByIndex(byTemp));
		}
		// 1.3 将牌
		memset(byTemp, 0, MAX_KEY_NUM);
		byTemp[9] = 2;
		g_setSingleJiang.insert(getKey2ByIndex(byTemp));
		g_setSingleJiangFZ.insert(getKey2ByIndex(byTemp));
		for (int i = 0; i < MAX_VAL_NUM; ++i)
		{
			memset(byTemp, 0, MAX_KEY_NUM);
			for (int n = 0; n < 2; ++n)
			{
				byTemp[i] = 2 - n;
				byTemp[9] = n;
				g_setSingleJiang.insert(getKey2ByIndex(byTemp));
				if (i < 7)	//风、字牌最多7张
					g_setSingleJiangFZ.insert(getKey2ByIndex(byTemp));
			}
		}
	};
	static void TrainAll()
	{
		TrainSingle();

		int nAll = 0;
		int nAllFZ = 0;
		int nSingle[100] = {};
		int nSingleFZ[50] = {};

		set<int>::iterator iter = g_setSingle.begin();
		for (; iter != g_setSingle.end(); ++iter)
			nSingle[nAll++] = *iter;
		iter = g_setSingleFZ.begin();
		for (; iter != g_setSingleFZ.end(); ++iter)
			nSingleFZ[nAllFZ++] = *iter;

		int nAllJiang = 0;
		int nAllJiangFZ = 0;
		int nSingleJiang[20] = {};
		int nSingleJiangKe[20] = {};

		iter = g_setSingleJiang.begin();
		for (; iter != g_setSingleJiang.end(); ++iter)
			nSingleJiang[nAllJiang++] = *iter;
		iter = g_setSingleJiangFZ.begin();
		for (; iter != g_setSingleJiangFZ.end(); ++iter)
			nSingleJiangKe[nAllJiangFZ++] = *iter;

		BYTE nNum;
		int nKey;
		for (int i1 = 0; i1 < nAllFZ; ++i1)
		{
			nNum = getNumByKey2(nSingleFZ[i1]);
			nKey = getKeyNoNai(nSingleFZ[i1]);
			addMap(g_mapHuAllFZ, nSingleFZ[i1]);
			for (int i2 = i1; i2 < nAllFZ; ++i2)
			{
				int nTemp = nSingleFZ[i1] + nSingleFZ[i2];
				if (!isValidKey(nTemp))
					continue;
				addMap(g_mapHuAllFZ, nTemp);
				for (int i3 = i2; i3 < nAllFZ; ++i3)
				{
					int nTemp = nSingleFZ[i1] + nSingleFZ[i2] + nSingleFZ[i3];
					if (!isValidKey(nTemp))
						continue;
					addMap(g_mapHuAllFZ, nTemp);
					for (int i4 = i3; i4 < nAllFZ; ++i4)
					{
						int nTemp = nSingleFZ[i1] + nSingleFZ[i2] + nSingleFZ[i3] + nSingleFZ[i4];
						if (!isValidKey(nTemp))
							continue;
						addMap(g_mapHuAllFZ, nTemp);
					}
				}
			}
		}

		hash_map<int, BYTE> mapTempFZ[15];
		for (int j = 0; j < 15; ++j)
			mapTempFZ[j] = g_mapHuAllFZ[j];

		for (int i = 0; i < nAllJiangFZ; ++i)
		{
			for (int j = 0; j < 15; ++j)
			{
				addMap(g_mapHuAllFZ, nSingleJiangKe[i]);
				hash_map<int, BYTE>::iterator iter_u = mapTempFZ[j].begin();
				for (; iter_u != mapTempFZ[j].end(); ++iter_u)
				{
					int nTemp = nSingleJiangKe[i] + iter_u->first + (int(iter_u->second & BIT_VAL_FLAG) << 27);
					if (isValidKey(nTemp))
						addMap(g_mapHuAllFZ, nTemp);
				}
			}
		}
		cout << (int)sizeof(*(g_setSingleFZ.begin())) << endl;

		for (int i1 = 0; i1 < nAll; ++i1)
		{
			addMap(g_mapHuAll, nSingle[i1]);
			for (int i2 = i1; i2 < nAll; ++i2)
			{
				int nTemp = nSingle[i1] + nSingle[i2];
				if (!isValidKey(nTemp))
					continue;
				addMap(g_mapHuAll, nTemp);
				for (int i3 = i2; i3 < nAll; ++i3)
				{
					int nTemp = nSingle[i1] + nSingle[i2] + nSingle[i3];
					if (!isValidKey(nTemp))
						continue;
					addMap(g_mapHuAll, nTemp);
					for (int i4 = i3; i4 < nAll; ++i4)
					{
						int nTemp = nSingle[i1] + nSingle[i2] + nSingle[i3] + nSingle[i4];
						if (!isValidKey(nTemp))
							continue;
						addMap(g_mapHuAll, nTemp);
					}
				}
			}
		}

		hash_map<int, BYTE> mapTemp[15];
		for (int j = 0; j < 15; ++j)
			mapTemp[j] = g_mapHuAll[j];

		for (int i = 0; i < nAllJiang; ++i)
		{
			for (int j = 0; j < 15; ++j)
			{
				addMap(g_mapHuAll, nSingleJiang[i]);
				hash_map<int, BYTE>::iterator iter_u = mapTemp[j].begin();
				for (; iter_u != mapTemp[j].end(); ++iter_u)
				{
					int nTemp = nSingleJiang[i] + iter_u->first + (int(iter_u->second & BIT_VAL_FLAG) << 27);
					if (isValidKey(nTemp))
						addMap(g_mapHuAll, nTemp);
				}
			}
		}
		cout << endl;
		cout << "pino:" << dec << g_setSingle.size() << endl;
		//cout << "pino:" << dec << g_usetHuAll.size() << endl;
		//cout << "pino:" << dec << g_usetHuAllJiang.size() << endl;
	}

	static bool CheckCanHuSingle(BYTE byType, BYTE byCards[10], BYTE &byNaiNum, BYTE byNaiMax)
	{
		bool	bSuc = false;
		int		nVal = getKeyByIndex(byCards);

		if (byType == enColorMJ_FenZi) 
			nVal &= 0x1FFFFF;

		BYTE	nNum = getNumByKey2(nVal);
		if (byType == enColorMJ_FenZi)
		{
			hash_map<int, BYTE>::iterator iterFind = g_mapHuAllFZ[nNum].find(nVal);
			if (iterFind != g_mapHuAllFZ[nNum].end())
			{
				if (iterFind->second <= byNaiMax)
				{
					byNaiNum = iterFind->second;
					return true;
				}
			}
		}
		else
		{
			hash_map<int, BYTE>::iterator iterFind = g_mapHuAll[nNum].find(nVal);
			if (iterFind != g_mapHuAll[nNum].end())
			{
				if (iterFind->second <= byNaiMax)
				{
					byNaiNum = iterFind->second;
					return true;
				}
			}
		}

		byNaiNum = 0;
		return false;
	}

	static bool CheckCanHu(BYTE byCardSrc[], BYTE byNaiIndex)
	{
		BYTE byCards[MAX_TOTAL_TYPE];
		memcpy(byCards, byCardSrc, MAX_TOTAL_TYPE);
		int nNaiZiNum = 0;
		if (byNaiIndex < INVALID_VAL)
		{
			nNaiZiNum = byCards[byNaiIndex];
			byCards[byNaiIndex] = 0;
		}

		BYTE byJiangNum = 0;
		BYTE nNaiTry[enColorMJ_Max] = {};
// 		BYTE byTemp[MAX_TOTAL_TYPE];
// 		memcpy(byTemp, byCards, MAX_TOTAL_TYPE);
		for (int cor = 0; cor < enColorMJ_Max; ++cor)
		{
			int nCardAll = 0;
			for (int i = 0; i < 9; ++i)
			{
				if (9 * cor + i < MAX_TOTAL_TYPE)
					nCardAll += byCards[9 * cor + i];
			}
			if (nCardAll == 0) continue;

			if (!CheckCanHuSingle(cor, byCards + 9 * cor, nNaiTry[cor], nNaiZiNum))
				return false;

			nNaiZiNum -= nNaiTry[cor];
			byJiangNum += ((nCardAll + nNaiTry[cor])%3 == 2);
			if (byJiangNum > nNaiZiNum + 1)
				return false;
		}
		return byJiangNum > 0 || nNaiZiNum >= 2;
	}
};

#endif //__CPLAYER_H__