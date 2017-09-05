/********************************************************************
	created:	pino
	file base:	CPlayerHuTips2
	author:		pino	
	purpose:	麻将胡牌提示算法(第2版)
*********************************************************************/

#ifndef __CPLAYER_HUTIPS2_H__
#define __CPLAYER_HUTIPS2_H__
#include <map>
#include <set>
#include <unordered_set>
#include <string>
#include <vector>
#include <list>
#include <windows.h>
#include <iostream>

using namespace std;

#define MAX_VAL_NUM						9					//一种颜色9个值
#define MAX_NAI_NUM						4					//赖子最大个数
#define MAX_KEY_NUM						(MAX_VAL_NUM+1)		//9+赖子
#define BIT_VAL_NUM						4					//一个值占的bit数

#define INVALID_VAL						0xFF			//非法数据
#define MAX_ALL_TYPE					34				//麻将种类数

//麻将颜色（种类）定义
enum enCorMJ
{
	enCorMJ_0,		//万
	enCorMJ_1,		//筒
	enCorMJ_2,		//条
	enCorMJ_3,		//风、字 牌
	enCorMJ_Max,
};

inline void getIndexByKey2(long long llVal, BYTE byIndex[MAX_KEY_NUM])
{
	for (int i=0; i<MAX_KEY_NUM; ++i)
	{
		byIndex[i] = (llVal>>(BIT_VAL_NUM*i))&0x0F;
	}
}

inline long long getKey2(BYTE byIndex[MAX_KEY_NUM])
{
	long long nKey = 0;
	for (int i=0; i<MAX_KEY_NUM; ++i)
		nKey |= (long long)(byIndex[i]&0x0F)<<(BIT_VAL_NUM*i);

//	cout<<hex<<nKey<<endl;
// 	BYTE byIndex2[MAX_KEY_NUM] = {};
// 	getIndexByKey2(nKey, byIndex2);
	return nKey;
}

inline bool isValidKey(long long llVal)
{
	BYTE byIndex[MAX_KEY_NUM] = {};
	getIndexByKey2(llVal, byIndex);
	if (byIndex[9] > MAX_NAI_NUM)
		return false;

	int nNum = 0;
	for (int i=0; i<MAX_KEY_NUM; ++i)
	{
		nNum += byIndex[i];
		if (byIndex[i] > 4 || nNum > 12)	//不带将，所以最大牌数为12
			return false;		
	}
	return nNum > 0;
}

struct stKey2
{
	long long nKey;

	stKey2():nKey(0){}
	stKey2(BYTE byIndex[MAX_KEY_NUM]):nKey(0)
	{
		for (int i=0; i<MAX_KEY_NUM; ++i)
			nKey |= (byIndex[i]&0x0F)<<(BIT_VAL_NUM*i);
	}
	stKey2(const stKey2 &t){nKey = t.nKey;}

	bool operator==(const stKey2 &t) const
	{		
		return nKey == t.nKey;
	}	

	stKey2 &operator += (const stKey2 &t){
		nKey += t.nKey;
		return *this;
	} 
};

struct stColorData2
{
	BYTE	byCorType;						// enCorMJ
	BYTE    byNum;
	BYTE	byValNum[MAX_KEY_NUM];			// 

	stColorData2(){ memset(this, 0, sizeof(*this)); }
	stColorData2(BYTE byType, BYTE byVal[MAX_KEY_NUM]):byCorType(byType),byNum(0)
	{ 
		memcpy(byValNum, byVal, MAX_KEY_NUM);
		for (int i=0; i<MAX_KEY_NUM; ++i)
			byNum += byValNum[i];
	}
};

struct stCardData2
{
	BYTE	byCardNum[MAX_ALL_TYPE];			// 统计数据
	BYTE	byNum;								// 数量

	stCardData2()
	{
		memset(this, 0, sizeof(*this));
	}
	stCardData2(BYTE byCards[], BYTE num):byNum(num)
	{
		memcpy(byCardNum, byCards, MAX_ALL_TYPE);
	}
};

class CPlayerHuTips2
{
public:
	CPlayerHuTips2(){};
	~CPlayerHuTips2(){};

public:	
	//设置赖子牌
	void SetNaiZiCard(BYTE byCard)	{m_byNaiIndex = byCard;}
	BYTE GetNaiZiCard()				{return m_byNaiIndex;}

	bool CheckCanHuSingle(stColorData2 &stColor, BYTE byNaiNum)
	{
		stColor.byValNum[9] = byNaiNum;
		BYTE byLeftNum = (stColor.byNum+byNaiNum)%3;
		if (byLeftNum == 1) 
			return false;

		if (stColor.byNum == 0)
			return true;
		
		if (byLeftNum == 0)
		{
			if (stColor.byCorType == enCorMJ_3)
				return m_usetHuAllKe.find(getKey2(stColor.byValNum)) != m_usetHuAllKe.end();
			else
				return m_usetHuAll.find(getKey2(stColor.byValNum)) != m_usetHuAll.end();
		}
		else
		{
			if (byNaiNum >= 2 && CheckCanHuSingle(stColor, byNaiNum-2))
			{
				return true;
			}
			else if (byNaiNum >= 1)
			{
				for (int i=0; i<MAX_VAL_NUM; ++i)
				{					
					if (stColor.byValNum[i] > 0)
					{
						stColor.byValNum[i] -= 1;
						stColor.byNum		-= 1;
						if (CheckCanHuSingle(stColor, byNaiNum-1))
							return true;
						stColor.byValNum[i] += 1;
						stColor.byNum		+= 1;
					}
				}
			}
			for (int i=0; i<MAX_VAL_NUM; ++i)
			{					
				if (stColor.byValNum[i] >= 2)
				{
					stColor.byValNum[i] -= 2;
					stColor.byNum		-= 2;
					if (CheckCanHuSingle(stColor, byNaiNum))
						return true;
					stColor.byValNum[i] += 2;
					stColor.byNum		+= 2;
				}
			}		
		}
		return false;
	}
	
	bool CheckCanHu(stCardData2 &stData, BYTE byNaiIndex)
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

		int nNaiZiMax[enCorMJ_Max] = {4, 4, 4, 4};
		stColorData2 stColorTemp[enCorMJ_Max];
		for (int cor=0; cor<enCorMJ_Max; ++cor)
		{		
			stColorTemp[cor].byCorType = cor;
			int nAll = (cor == enCorMJ_3) ? 7 : MAX_VAL_NUM;
			if (cor*MAX_VAL_NUM + nAll <= MAX_ALL_TYPE)
			{				
				memcpy(stColorTemp[cor].byValNum, stData.byCardNum+cor*MAX_VAL_NUM, nAll);
				for (int i=0; i<nAll; ++i)
					stColorTemp[cor].byNum += stColorTemp[cor].byValNum[i];			
			}
			if (stColorTemp[cor].byNum == 0)
				nNaiZiMax[cor] = 0;
		}

		BYTE byLeftNum[enCorMJ_Max] = {};
		for (int i1=0; i1<=min(nNaiZiMax[0], nNaiZiNum); ++i1)
		{		
			byLeftNum[enCorMJ_0] = (stColorTemp[0].byNum+i1)%3;
			if (byLeftNum[enCorMJ_0] == 1) continue;
			for (int i2=0; i2<=min(nNaiZiMax[1], nNaiZiNum-i1); ++i2)
			{
				byLeftNum[enCorMJ_1] = (stColorTemp[1].byNum+i2)%3;
				if (byLeftNum[enCorMJ_1] == 1) continue;
				for (int i3=0; i3<=min(nNaiZiMax[2], nNaiZiNum-i1-i2); ++i3)
				{
					byLeftNum[enCorMJ_2] = (stColorTemp[2].byNum+i3)%3;
					if (byLeftNum[enCorMJ_2] == 1) continue;
					int i4 = nNaiZiNum-i1-i2-i3;
					if (i4 <= nNaiZiMax[3])
					{
						byLeftNum[enCorMJ_3] = (stColorTemp[3].byNum+i4)%3;
						if (byLeftNum[enCorMJ_3] == 1 || byLeftNum[0]+byLeftNum[1]+byLeftNum[2]+byLeftNum[3] != 2) continue;

						stColorData2 stTemp[enCorMJ_Max];
						memcpy(stTemp, stColorTemp, sizeof(stTemp));					

						if (stColorTemp[0].byNum>0 && !CheckCanHuSingle(stTemp[0], i1))
							continue;
						if (stColorTemp[1].byNum>0 && !CheckCanHuSingle(stTemp[1], i2))
							continue;
						if (stColorTemp[2].byNum>0 && !CheckCanHuSingle(stTemp[2], i3))
							continue;
						if (stColorTemp[3].byNum>0 && !CheckCanHuSingle(stTemp[3], i4))
							continue;

						return true;
					}
				}			
			}
		}	
		return false;
	};

	void TrainSingle()			// 1.三张单独组合
	{
		BYTE byTemp[MAX_KEY_NUM] = {};
		byTemp[9] = 3;
		long long ll = getKey2(byTemp);
		m_setSingle.insert(getKey2(byTemp));
		m_setSingleKe.insert(getKey2(byTemp));
		// 1.1 刻子
		for (int i=0; i<MAX_VAL_NUM; ++i)
		{
			memset(byTemp, 0, MAX_KEY_NUM);
			for (int n=0; n<3; ++n)
			{
				byTemp[i] = 3-n;
				byTemp[9] = n;
				m_setSingle.insert(getKey2(byTemp));
				if (i < 7)	//风、字牌最多7张
					m_setSingleKe.insert(getKey2(byTemp));
			}
		}
		// 1.2 顺子 没赖子
		for (int i=0; i<MAX_VAL_NUM-2; ++i)
		{
			memset(byTemp, 0, MAX_KEY_NUM);	
			byTemp[i]	= 1;
			byTemp[i+1]	= 1;
			byTemp[i+2]	= 1;
			m_setSingle.insert(getKey2(byTemp));					
		}
		// 1.2 顺子 1个赖子 (2个赖子时也就是刻子)
		for (int i=0; i<MAX_VAL_NUM-2; ++i)
		{
			memset(byTemp, 0, MAX_KEY_NUM);	
			byTemp[i]	= 1;
			byTemp[i+1]	= 1;
			byTemp[9]	= 1;
			m_setSingle.insert(getKey2(byTemp));	
			memset(byTemp, 0, MAX_KEY_NUM);	
			byTemp[9]	= 1;
			byTemp[i+1]	= 1;
			byTemp[i+2]	= 1;			
			m_setSingle.insert(getKey2(byTemp));
			memset(byTemp, 0, MAX_KEY_NUM);	
			byTemp[i]	= 1;
			byTemp[9]	= 1;
			byTemp[i+2]	= 1;
			m_setSingle.insert(getKey2(byTemp));
		}
	};
	void TrainAll()
	{
		TrainSingle();

		int nAll	= 0;
		int nAllKe	= 0;
		long long nSingle[100]	= {};
		long long nSingleKe[50] = {};
		set<long long>::iterator iter = m_setSingle.begin();
		for (; iter != m_setSingle.end(); ++iter)
			nSingle[nAll++] = *iter;

		iter = m_setSingleKe.begin();
		for (; iter != m_setSingleKe.end(); ++iter)
			nSingleKe[nAllKe++] = *iter;

		for (int i1=0; i1<nAllKe; ++i1)
		{
			m_usetHuAllKe.insert(nSingleKe[i1]);
			for (int i2=i1; i2<nAllKe; ++i2)
			{								
				if (!isValidKey(nSingleKe[i1] + nSingleKe[i2])) 
					continue;
				//cout<<hex<<nSingleKe[i1] + nSingleKe[i2]<<'\t';
				m_usetHuAllKe.insert(nSingleKe[i1] + nSingleKe[i2]);
				for (int i3=i2; i3<nAllKe; ++i3)
				{					
					if (!isValidKey(nSingleKe[i1] + nSingleKe[i2] + nSingleKe[i3])) 
						continue;
					//cout<<hex<<nSingleKe[i1] + nSingleKe[i2] + nSingleKe[i3]<<'\t';
					m_usetHuAllKe.insert(nSingleKe[i1] + nSingleKe[i2] + nSingleKe[i3]);
					for (int i4=i3; i4<nAllKe; ++i4)
					{						
						if (!isValidKey(nSingleKe[i1] + nSingleKe[i2] + nSingleKe[i3] + nSingleKe[i4])) 
							continue;
						//cout<<hex<<nSingleKe[i1] + nSingleKe[i2] + nSingleKe[i3] + nSingleKe[i4]<<'\t';
						m_usetHuAllKe.insert(nSingleKe[i1] + nSingleKe[i2] + nSingleKe[i3] + nSingleKe[i4]);
					}
				}
			}
		}
// 		cout<<endl;
// 		cout<<"pino:"<<dec<<m_setSingleKe.size()<<endl;
 //		cout<<"pino:"<<dec<<m_usetHuAllKe.size()<<endl;
				
		for (int i1=0; i1<nAll; ++i1)
		{
			m_usetHuAll.insert(nSingle[i1]);
			for (int i2=i1; i2<nAll; ++i2)
			{								
				if (!isValidKey(nSingle[i1] + nSingle[i2])) 
					continue;
				//cout<<hex<<nSingle[i1] + nSingle[i2]<<'\t';
				m_usetHuAll.insert(nSingle[i1] + nSingle[i2]);
				for (int i3=i2; i3<nAll; ++i3)
				{					
					if (!isValidKey(nSingle[i1] + nSingle[i2] + nSingle[i3])) 
						continue;
					//cout<<hex<<nSingle[i1] + nSingle[i2] + nSingle[i3]<<'\t';
					m_usetHuAll.insert(nSingle[i1] + nSingle[i2] + nSingle[i3]);
					for (int i4=i3; i4<nAll; ++i4)
					{						
						if (!isValidKey(nSingle[i1] + nSingle[i2] + nSingle[i3] + nSingle[i4])) 
							continue;
						//cout<<hex<<nSingle[i1] + nSingle[i2] + nSingle[i3] + nSingle[i4]<<'\t';
						m_usetHuAll.insert(nSingle[i1] + nSingle[i2] + nSingle[i3] + nSingle[i4]);
					}
				}
			}
		}
// 		cout<<endl;
// 		cout<<"pino:"<<dec<<m_setSingle.size()<<endl;
// 		cout<<"pino:"<<dec<<m_usetHuAll.size()<<endl;
	}
	
private:	
	BYTE						m_byNaiIndex;		//赖子牌

	set<long long>				m_setSingle;		//单个顺子+刻子		50个
	set<long long>				m_setSingleKe;		//单个刻子			22个

	unordered_set<long long>	m_usetHuAll;		//
	unordered_set<long long>	m_usetHuAllKe;		//
};

#endif  //__CPLAYER_H__