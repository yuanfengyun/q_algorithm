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
	for (int i = 0; i<MAX_KEY_NUM; ++i)
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

inline bool isValidKey(int llVal)
{
	BYTE byIndex[MAX_KEY_NUM] = {};
	getIndexByKey2(llVal, byIndex);
	if (byIndex[9] > MAX_NAI_NUM)
		return false;

	int nNum = 0;
	for (int i = 0; i<MAX_KEY_NUM; ++i)
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

set<int>				g_setSingle;		//单个顺子+刻子		50个
set<int>				g_setSingleFZ;		//单个顺子+刻子		22个
set<int>				g_setSingleJiang;	//单个将			19个
set<int>				g_setSingleJiangFZ;	//单个将			15个

unordered_set<int>		g_usetHuAll[15];			//
unordered_set<int>		g_usetHuAllFZ[15];			// 风、字牌


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
		for (int i = 0; i<MAX_VAL_NUM; ++i)
		{
			memset(byTemp, 0, MAX_KEY_NUM);
			for (int n = 0; n<3; ++n)
			{
				byTemp[i] = 3 - n;
				byTemp[9] = n;
				g_setSingle.insert(getKey2ByIndex(byTemp));
				if (i < 7)	//风、字牌最多7张
					g_setSingleFZ.insert(getKey2ByIndex(byTemp));
			}
		}
		// 1.2 顺子 没赖子
		for (int i = 0; i<MAX_VAL_NUM - 2; ++i)
		{
			memset(byTemp, 0, MAX_KEY_NUM);
			byTemp[i] = 1;
			byTemp[i + 1] = 1;
			byTemp[i + 2] = 1;
			g_setSingle.insert(getKey2ByIndex(byTemp));
		}
		// 1.2 顺子 1个赖子 (2个赖子时也就是刻子)
		for (int i = 0; i<MAX_VAL_NUM - 2; ++i)
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
		for (int i = 0; i<MAX_VAL_NUM; ++i)
		{
			memset(byTemp, 0, MAX_KEY_NUM);
			for (int n = 0; n<2; ++n)
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

		for (int i1 = 0; i1<nAllFZ; ++i1)
		{
			g_usetHuAllFZ[getNumByKey(nSingleFZ[i1])].insert(nSingleFZ[i1]);
			for (int i2 = i1; i2<nAllFZ; ++i2)
			{
				int nTemp = nSingleFZ[i1] + nSingleFZ[i2];
				if (!isValidKey(nTemp))
					continue;
				g_usetHuAllFZ[getNumByKey(nTemp)].insert(nTemp);
				for (int i3 = i2; i3<nAllFZ; ++i3)
				{
					int nTemp = nSingleFZ[i1] + nSingleFZ[i2] + nSingleFZ[i3];
					if (!isValidKey(nTemp))
						continue;
					g_usetHuAllFZ[getNumByKey(nTemp)].insert(nTemp);
					for (int i4 = i3; i4<nAllFZ; ++i4)
					{
						int nTemp = nSingleFZ[i1] + nSingleFZ[i2] + nSingleFZ[i3] + nSingleFZ[i4];
						if (!isValidKey(nTemp))
							continue;
						g_usetHuAllFZ[getNumByKey(nTemp)].insert(nTemp);
					}
				}
			}
		}

		for (int i = 0; i<nAllJiangFZ; ++i)
		{
			for (int j = 0; j < 15; ++j)
			{
				if (j % 3 != 0)
					continue;

				g_usetHuAllFZ[getNumByKey(nSingleJiangKe[i])].insert(nSingleJiangKe[i]);
				unordered_set<int>::iterator iter_u = g_usetHuAllFZ[j].begin();
				for (; iter_u != g_usetHuAllFZ[j].end(); ++iter_u)
				{
					int nTemp = nSingleJiangKe[i] + *iter_u;
					if (isValidKey(nTemp))
						g_usetHuAllFZ[getNumByKey(nTemp)].insert(nTemp);
				}
			}
		}
		cout << (int)sizeof(*(g_setSingleFZ.begin())) << endl;
		// 		cout << "pino:" << dec << g_setSingleFZ.size() << endl;
		// 		cout << "pino:" << dec << g_usetHuAllFZ.size() << endl;
		// 		cout << "pino:" << dec << g_usetHuAllJiangFZ.size() << endl;

		for (int i1 = 0; i1<nAll; ++i1)
		{
			g_usetHuAll[getNumByKey(nSingle[i1])].insert(nSingle[i1]);
			for (int i2 = i1; i2<nAll; ++i2)
			{
				int nTemp = nSingle[i1] + nSingle[i2];
				if (!isValidKey(nTemp))
					continue;
				g_usetHuAll[getNumByKey(nTemp)].insert(nTemp);
				for (int i3 = i2; i3<nAll; ++i3)
				{
					int nTemp = nSingle[i1] + nSingle[i2] + nSingle[i3];
					if (!isValidKey(nTemp))
						continue;
					g_usetHuAll[getNumByKey(nTemp)].insert(nTemp);
					for (int i4 = i3; i4<nAll; ++i4)
					{
						int nTemp = nSingle[i1] + nSingle[i2] + nSingle[i3] + nSingle[i4];
						if (!isValidKey(nTemp))
							continue;
						g_usetHuAll[getNumByKey(nTemp)].insert(nTemp);
					}
				}
			}
		}

		for (int i = 0; i<nAllJiang; ++i)
		{
			for (int j = 0; j < 15; ++j)
			{
				if (j % 3 != 0)
					continue;

				g_usetHuAll[getNumByKey(nSingleJiang[i])].insert(nSingleJiang[i]);
				unordered_set<int>::iterator iter_u = g_usetHuAll[j].begin();
				for (; iter_u != g_usetHuAll[j].end(); ++iter_u)
				{
					int nTemp = nSingleJiang[i] + *iter_u;
					if (isValidKey(nTemp))
						g_usetHuAll[getNumByKey(nTemp)].insert(nTemp);
				}
			}
		}
		cout << endl;
		cout << "pino:" << dec << g_setSingle.size() << endl;
		//cout << "pino:" << dec << g_usetHuAll.size() << endl;
		//cout << "pino:" << dec << g_usetHuAllJiang.size() << endl;
	}

	static bool CheckCanHuSingle(stColorData &stColor, BYTE &byNaiNum, BYTE byNaiMax)
	{
		if (stColor.byNum == 0)
			return true;

		bool bSuc = false;
		int nVal = getKey2ByIndex(stColor.byCount);
		for (byNaiNum = 0; byNaiNum <= byNaiMax; ++byNaiNum)
		{
			BYTE byAll = stColor.byNum + byNaiNum;
			BYTE byLeftNum = byAll % 3;
			if (byLeftNum == 1)
				continue;

			int nKey = nVal + ((int)(byNaiNum & BIT_VAL_FLAG) << (BIT_VAL_NUM * 9));

			if (stColor.byCorType == enColorMJ_FenZi)
				bSuc = g_usetHuAllFZ[byAll].find(nKey) != g_usetHuAllFZ[byAll].end();
			else
				bSuc = g_usetHuAll[byAll].find(nKey) != g_usetHuAll[byAll].end();

			if (bSuc)
				return true;
		}
		byNaiNum = 0;
		return false;
	}

	static bool CheckCanHu(stCardData &stData, BYTE byNaiIndex)
	{
		if (stData.byNum % 3 != 2)
			return false;

		int nNaiZiNum = 0;
		if (byNaiIndex != INVALID_VAL)
		{
			nNaiZiNum = stData.byCardNum[byNaiIndex];
			stData.byCardNum[byNaiIndex] = 0;
			stData.byNum -= nNaiZiNum;
		}

		stColorData stColorTemp[enColorMJ_Max];
		for (int cor = 0; cor < enColorMJ_Max; ++cor)
		{
			stColorTemp[cor].byCorType = cor;
			int nAll = (cor == enColorMJ_FenZi) ? 7 : MAX_VAL_NUM;
			if (cor*MAX_VAL_NUM + nAll <= MAX_TOTAL_TYPE)
			{
				memcpy(stColorTemp[cor].byCount, stData.byCardNum + cor*MAX_VAL_NUM, nAll);
				for (int i = 0; i < nAll; ++i)
					stColorTemp[cor].byNum += stColorTemp[cor].byCount[i];
			}
		}

		BYTE byJiangNum = 0;
		BYTE nNaiTry[4] = {};
		for (int cor = 0; cor < enColorMJ_Max; ++cor)
		{
			if (stColorTemp[cor].byNum == 0)
				continue;

			if (!CheckCanHuSingle(stColorTemp[cor], nNaiTry[cor], nNaiZiNum))
				return false;

			nNaiZiNum -= nNaiTry[cor];
			byJiangNum += (stColorTemp[cor].byNum + nNaiTry[cor]) == 2;
			if (byJiangNum > nNaiZiNum + 1)
				return false;
		}
		return true;
	}
};

#endif //__CPLAYER_H__