#include "iostream"
#include <windows.h>
#include "vector"
#include "map"
#include "list"
#include "set"
#include "algorithm"

//		#ifdef TTT
		#ifndef TTT

using namespace std;

//数值定义
#define MAX_WEAVE					7									//最大组合
#define MAX_INDEX					21									//最大索引
#define MAX_COUNT					21									//最大数目
#define MAX_COMMON_CARD_COUNT		20									//常用牌数量
#define MIN_HU_VAL					15									//最小胡息
#define MAX_TUN_VAL					16									//最大囤值

#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0xFF								//数值掩码

//规则类型
enum enPlayType
{
	enPlayType_NORMAL	= 0,		//一般
	enPlayType_XDD		= 1,		//小大搭
	enPlayType_27A		= 2,		//二七十
	enPlayType_123		= 3,		//一二三
	enPlayType_WANG		= 4,		//王
	enPlayType_max,
};

#define TY_MY		(1+(1<<enPlayType_WANG)+(1<<enPlayType_123)+(1<<enPlayType_27A)+(1<<enPlayType_XDD))
//#define TY_MY		(1+(1<<enPlayType_WANG)+(1<<enPlayType_123)+(1<<enPlayType_27A))
//#define TY_MY		(1+(1<<enPlayType_XDD)+(1<<enPlayType_123)+(1<<enPlayType_27A))
//#define TY_MY		(1+(1<<enPlayType_123)+(1<<enPlayType_27A))

bool isHavePlayType(enPlayType type)
{
	if (type >= enPlayType_max)
	{
		return false;
	}
	return (TY_MY&(1<<type)) != 0;
}

int getKey(char n1, char n2, char n3, char n4)
{
	vector<int> vctTemp;
	vctTemp.push_back(n1);
	vctTemp.push_back(n2);
	vctTemp.push_back(n3);
	vctTemp.push_back(n4);
	sort(vctTemp.begin(), vctTemp.end());
	return ((vctTemp[3]<<24) + (vctTemp[2]<<16) + (vctTemp[1]<<8) + vctTemp[0]);
}

void addMapVal(map<int, char> & mapList, int key, char val)
{
	map<int, char>::iterator iter = mapList.find(key);
	if (iter == mapList.end())	
		mapList[key] = val;	
	else if (iter->second < val)	
		mapList[key] = val;	
}

void getTiPaoList(map<int, char> & mapList)
{
	for (char val=1; val<=10; ++val)
	{
		int VAL = val+0x10;
		addMapVal(mapList, getKey(val,val,val,val), 9);
		addMapVal(mapList, getKey(VAL,VAL,VAL,VAL), 12);
		if (isHavePlayType(enPlayType_WANG))
		{
			addMapVal(mapList, getKey(0xFF,val,val,val), 9);
			addMapVal(mapList, getKey(0xFF,0xFF,val,val), 9);
			addMapVal(mapList, getKey(0xFF,0xFF,0xFF,val), 9);
			addMapVal(mapList, getKey(0xFF,VAL,VAL,VAL), 12);
			addMapVal(mapList, getKey(0xFF,0xFF,VAL,VAL), 12);
			addMapVal(mapList, getKey(0xFF,0xFF,0xFF,VAL), 12);		
			addMapVal(mapList, getKey(0xFF,0xFF,0xFF,0xFF), 12);	
		}		
	}
}

void getWeiPengList(map<int, char> & mapList)
{
	for (char val=1; val<=10; ++val)
	{
		int VAL = val+0x10;
		addMapVal(mapList, getKey(val,val,val,0), 3);
		addMapVal(mapList, getKey(VAL,VAL,VAL,0), 6);
		if (isHavePlayType(enPlayType_XDD))
		{
			addMapVal(mapList, getKey(VAL,val,val,0), 0);
			addMapVal(mapList, getKey(VAL,VAL,val,0), 0);
			if (isHavePlayType(enPlayType_WANG))
			{
				addMapVal(mapList, getKey(0xFF,VAL,val,0), 0);
			}
		}
		if (isHavePlayType(enPlayType_WANG))
		{
			addMapVal(mapList, getKey(0xFF,val,val,0), 3);
			addMapVal(mapList, getKey(0xFF,0xFF,val,0), 3);
			addMapVal(mapList, getKey(0xFF,VAL,VAL,0), 6);
			addMapVal(mapList, getKey(0xFF,0xFF,VAL,0), 6);
			addMapVal(mapList, getKey(0xFF,0xFF,0xFF,0), 6);
		}		
	}	
}

void get27AList(map<int, char> & mapList)
{
	if (isHavePlayType(enPlayType_27A))	
	{
		addMapVal(mapList, getKey(0x0A,0x07,0x02,0x00), 3);
		addMapVal(mapList, getKey(0x1A,0x17,0x12,0x00), 6);
		if (isHavePlayType(enPlayType_WANG))
		{
			addMapVal(mapList, getKey(0xFF,0x07,0x02,0x00), 3);
			addMapVal(mapList, getKey(0x0A,0xFF,0x02,0x00), 3);
			addMapVal(mapList, getKey(0x0A,0x07,0xFF,0x00), 3);
			addMapVal(mapList, getKey(0xFF,0xFF,0x02,0x00), 3);
			addMapVal(mapList, getKey(0x0A,0xFF,0xFF,0x00), 3);
			addMapVal(mapList, getKey(0xFF,0x07,0xFF,0x00), 3);

			addMapVal(mapList, getKey(0x1F,0x17,0x12,0x00), 6);
			addMapVal(mapList, getKey(0x1A,0x1F,0x12,0x00), 6);
			addMapVal(mapList, getKey(0x1A,0x17,0x1F,0x00), 6);
			addMapVal(mapList, getKey(0x1F,0x1F,0x12,0x00), 6);
			addMapVal(mapList, getKey(0x1A,0x1F,0x1F,0x00), 6);
			addMapVal(mapList, getKey(0x1F,0x17,0x1F,0x00), 6);
		}	
	}	
}

void getShunZhiList(map<int, char> & mapList)
{
	for (char val=1; val<=8; ++val)
	{
		int xi = 0;	
		if (isHavePlayType(enPlayType_123))		
			xi = (val==1) ? 3 : 0;	
				
		addMapVal(mapList, getKey(val,		val+1,		val+2,		0x00), xi);
		addMapVal(mapList, getKey(0x10+val,	0x10+val+1,	0x10+val+2,	0x00), xi*2);
		if (isHavePlayType(enPlayType_WANG))
		{
			addMapVal(mapList, getKey(0xFF,val+1,val+2,0x00), xi);
			addMapVal(mapList, getKey(val,0xFF,val+2,0x00), xi);
			addMapVal(mapList, getKey(val,val+1,0xFF,0x00), xi);
			addMapVal(mapList, getKey(val,0xFF,0xFF,0x00), xi);
			addMapVal(mapList, getKey(0xFF,val+1,0xFF,0x00), xi);
			addMapVal(mapList, getKey(0xFF,0xFF,val+2,0x00), xi);

			addMapVal(mapList, getKey(0xFF,0x10+val+1,0x10+val+2,0x00), xi*2);
			addMapVal(mapList, getKey(0x10+val,0xFF,0x10+val+2,0x00), xi*2);
			addMapVal(mapList, getKey(0x10+val,0x10+val+1,0xFF,0x00), xi*2);
			addMapVal(mapList, getKey(0x10+val,0xFF,0xFF,0x00), xi*2);
			addMapVal(mapList, getKey(0xFF,0x10+val+1,0xFF,0x00), xi*2);
			addMapVal(mapList, getKey(0xFF,0xFF,0x10+val+2,0x00), xi*2);
		}		
	}
}

map<int, char>	m_mapKey;
int getCardVal(int nVal)
{
	//四张牌时  只有一种情况 
	//	1.四张相同的或带王	
	//三张牌时 
	//	1.三张一样的或带王
	//  2.二七十
	//  3.大小搭
	//	4.连牌
	if (m_mapKey.empty())
	{
		getTiPaoList(m_mapKey);
		getWeiPengList(m_mapKey);
		get27AList(m_mapKey);
		getShunZhiList(m_mapKey);
	}	

	map<int, char>::iterator iter = m_mapKey.find(nVal);
	if (iter != m_mapKey.end())
	{
		return int(iter->second);
	}
	return -1;
}

int getCardVal(char val1, char val2, char val3, char val4 = 0x00)
{		
	int key = getKey(val1, val2, val3, val4);
	return getCardVal(key);
}

struct stHuKeyData
{
	bool bKey;
	int nHuKey;
	vector<char> vctIndex;

	stHuKeyData()
	{
		nHuKey = 0;
		bKey = false;
	}
	stHuKeyData(int key, char index1, char index2, char index3=-1, char index4=-1, bool bK = false)
	{
		nHuKey = key;
		bKey = bK;
		vctIndex.push_back(index1);
		vctIndex.push_back(index2);
		if (index3 >= 0)	
			vctIndex.push_back(index3);
		if (index4 >= 0)		
			vctIndex.push_back(index4);
	}
};


struct stCardData
{	
	stCardData(): nFlag(0), nLeft(0)
	{}

	void push(char val)
	{
		char n = getCardNumIndex(val);
		if (n >=0 && n < 21)
		{
			vctCardIndex[n].push_back(vctCard.size());
		}		
		vctCard.push_back(val);		
		nLeft++;
	}

	char getVal(char index)
	{
		return vctCard[index];
	}

	size_t size()
	{
		return vctCard.size();
	}

	char getLeft()
	{
		return nLeft;
	}

	vector<char> getLeftIndex()
	{
		vector<char> vctLeftIndex;
		int nSize = vctCard.size();
		for (size_t i=0; i<nSize; ++i)
		{
			if (!getFlag(i))
			{
				vctLeftIndex.push_back(i);
			}
		}
		return vctLeftIndex;
	}

	void setFlag(char index, bool bFlag)
	{
		if (index < vctCard.size())
		{
			if (bFlag)
			{
				if (getFlag(index) == false)	
				{
					char n = getCardNumIndex(vctCard[index]);
					if (n >= 0 && n < 21)
					{
						vector<char>::iterator pos = find(vctCardIndex[n].begin(),vctCardIndex[n].end(), index);
						if (pos != vctCardIndex[n].end())
							vctCardIndex[n].erase(pos);
					}
					--nLeft;
					nFlag |= (1<<index);
				}
			}
			else
			{
				if (getFlag(index))
				{					
					char n = getCardNumIndex(vctCard[index]);
					if (n >= 0 && n < 21)
					{
						vctCardIndex[n].push_back(index);
					}					
					++nLeft;
					nFlag &= ~(1<<index);
				}
			}
		}		
	}

	bool getFlag(char index)
	{
		return nFlag &(1<<index);
	}

	bool isFinised()
	{
		int nSize = vctCard.size();
		int nFlagAll = ((1<<(nSize))-1);
		return (nFlagAll & nFlag) == nFlagAll;
	}
	vector<char>& getcardsNumByVal(char val)
	{
		return vctCardIndex[getCardNumIndex(val)];
	}

private:
	char getCardNumIndex(char val)
	{
		if (val <= 0)		
			return 20;		
		else if (val <= 0x0A)	
			return val-1;		
		else if (val >= 0x10 && val <= 0x1A)
			return 10+val-0x11;

		return -1;
	}

private:
	vector<char>		vctCard;
	int					nFlag;	
	char				nLeft;
	vector<char>		vctCardIndex[21];	//21种牌
};

struct stAnswer
{
	char				nPer;		// 倍率
	char				nHuVal;
	vector<stHuKeyData> vctData;

	void push(stHuKeyData stKey, stCardData *pVctCard = NULL)
	{
		nHuVal += stKey.nHuKey;
		vctData.push_back(stKey);
		if (pVctCard != NULL)
		{
			vector<char> &vIndex = stKey.vctIndex;
			for (int i=0; i<vIndex.size(); ++i)
			{
				(*pVctCard).setFlag(vIndex[i], true);
			}
		}
	}

	void pop(stCardData *pVctCard = NULL)
	{
		int nSize = vctData.size();
		if (pVctCard != NULL && nSize > 0)
		{
			vector<char> &vIndex = vctData[nSize-1].vctIndex;
			for (int i=0; i<vIndex.size(); ++i)
			{
				(*pVctCard).setFlag(vIndex[i], false);
			}
		}
		nHuVal -= vctData[vctData.size()-1].nHuKey;
		vctData.pop_back();		
	}

	stAnswer()
	{
		nHuVal	= 0;
		nPer	= 1;
	}	
	void clear()
	{
		nHuVal	= 0;
		nPer	= 1;
		vctData.clear();
	}
	void setPer(char per)
	{
		nPer = per;
	}
	char nGetHuPer()
	{
		if (nHuVal < 15)		
			return 0;
		
		return ((nHuVal-15)/3+1)*nPer;
	}
};

bool g_bError = false;
void getCardHuKey(stCardData stData, stAnswer &answerData, stAnswer &resultMax)
{	
	// 1.预处理	
	//vector<stHuKeyData> &vctHuKeyData = answerData.vctData;		
	
	// 2.
	stCardData stLeftCard;
	vector<char> vctLeftIndex = stData.getLeftIndex();
	for (size_t i=0; i<vctLeftIndex.size(); ++i)
	{
		stLeftCard.push(vctLeftIndex[i]);
	}

	// 3.逻辑
	int nLeft = stLeftCard.size();	
	if (nLeft == 0)
	{		
		if (resultMax.nGetHuPer() < answerData.nGetHuPer())
		{
			resultMax = answerData;			
		}
	}
	else if (nLeft <= 1)
	{
		g_bError = true;
	}
	else if (nLeft == 3)
	{
		int nHuKey = getCardVal(stData.getVal(stLeftCard.getVal(0)), stData.getVal(stLeftCard.getVal(1)), stData.getVal(stLeftCard.getVal(2)));
		if (nHuKey >= 0)
		{			
			stAnswer answerTemp = answerData;	
			answerTemp.push(stHuKeyData(nHuKey, stLeftCard.getVal(0), stLeftCard.getVal(1), stLeftCard.getVal(2)));	
			if (resultMax.nGetHuPer() < answerTemp.nGetHuPer())
			{							
				resultMax = answerTemp;
			}
		}
	}
	else if (nLeft == 2)
	{		
		int nVal1 = stData.getVal(stLeftCard.getVal(0));
		int nVal2 = stData.getVal(stLeftCard.getVal(1));
		if ((nVal1 == 0xff || nVal2 == 0xff || nVal1 == nVal2) && resultMax.nGetHuPer() < answerData.nGetHuPer())		
		{			
			resultMax = answerData;	
			resultMax.push(stHuKeyData(0, stLeftCard.getVal(0), stLeftCard.getVal(1)));						
		}	
	}
	else	// 还有多个
	{		
		static char s_chrIndex[21] = {
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
			0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A,	0xff,
		};
		// 1.尝试12或9胡
		vector<char> vctKingIndex = stData.getcardsNumByVal(0xff);
		if (vctKingIndex.size() == 4)
		{	//【1.1】 四个王
			stHuKeyData sKeyData(12, vctKingIndex[0], vctKingIndex[1], vctKingIndex[2], vctKingIndex[3]);
			answerData.push(sKeyData, &stData);
			getCardHuKey(stData, answerData, resultMax);							
			answerData.pop(&stData);
		}
		if (vctKingIndex.size() >= 3)
		{	//【1.2】 三个王带一个
			for (int i=19; i>=0; --i)
			{
				vector<char> vctTemp = stData.getcardsNumByVal(s_chrIndex[i]);
				if (vctTemp.size() > 0 && vctTemp.size() < 3)
				{
					int nHuVal = i>=10 ? 12 : 9; 
					stHuKeyData sKeyData(nHuVal, vctKingIndex[0], vctKingIndex[1], vctKingIndex[2], vctTemp[0]);
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
			}
		}
		if (vctKingIndex.size() >= 2)
		{	//【1.2】 两个王带两个
			for (int i=19; i>=0; --i)
			{
				vector<char> vctTemp = stData.getcardsNumByVal(s_chrIndex[i]);
				if (vctTemp.size() == 2)
				{
					int nHuVal = i>=10 ? 12 : 9; 
					stHuKeyData sKeyData(nHuVal, vctKingIndex[0], vctKingIndex[1], vctTemp[0], vctTemp[1]);					
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
			}
		}
		if (vctKingIndex.size() >= 1)
		{	//【1.3】 一个王带三个
			for (int i=19; i>=0; --i)
			{
				vector<char> vctTemp = stData.getcardsNumByVal(s_chrIndex[i]);
				if (vctTemp.size() == 3)
				{
					int nHuVal = i>=10 ? 12 : 9;
					stHuKeyData sKeyData(nHuVal, vctKingIndex[0], vctTemp[0], vctTemp[1], vctTemp[2]);
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
			}
		}

		// 2 尝试6胡或3胡
		if (vctKingIndex.size() >= 3)
		{	//【2.1】 三个王
			stHuKeyData sKeyData(6, vctKingIndex[0], vctKingIndex[1], vctKingIndex[2]);
			answerData.push(sKeyData, &stData);
			getCardHuKey(stData, answerData, resultMax);							
			answerData.pop(&stData);
		}
		if (vctKingIndex.size() >= 2)
		{	//【2.2】 二个王带一个
			for (int i=19; i>=0; --i)
			{
				vector<char> vctTemp = stData.getcardsNumByVal(s_chrIndex[i]);
				if (vctTemp.size() > 0 && vctTemp.size() < 3)
				{
					int nHuVal = i>=10 ? 6 : 3; 
					stHuKeyData sKeyData(nHuVal, vctKingIndex[0], vctKingIndex[1], vctTemp[0]);
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
			}
		}
		if (vctKingIndex.size() >= 1)
		{	//【2.3】 一个王带两个
			for (int i=19; i>=0; --i)
			{
				vector<char> vctTemp = stData.getcardsNumByVal(s_chrIndex[i]);
				if (vctTemp.size() == 2)
				{
					int nHuVal = i>=10 ? 6 : 3; 
					stHuKeyData sKeyData(nHuVal, vctKingIndex[0], vctTemp[0], vctTemp[1]);					
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
			}
		}
		{	//【2.4】 三个的			
			for (int i=19; i>=0; --i)
			{
				vector<char> vctTemp = stData.getcardsNumByVal(s_chrIndex[i]);
				if (vctTemp.size() == 3)
				{
					int nHuVal = i>=10 ? 6 : 3; 
					stHuKeyData sKeyData(nHuVal, vctTemp[0], vctTemp[1], vctTemp[2]);					
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
			}
		}
		{	//【2.5】 一二三 或带王
			vector<char> vctTemp1 = stData.getcardsNumByVal(0x11);
			vector<char> vctTemp2 = stData.getcardsNumByVal(0x12);
			vector<char> vctTemp3 = stData.getcardsNumByVal(0x13);
			if (vctTemp1.size() > 0 && vctTemp2.size() > 0 && vctTemp3.size() > 0)
			{
				stHuKeyData sKeyData(6, vctTemp1[0], vctTemp2[0], vctTemp3[0]);			
				answerData.push(sKeyData, &stData);
				getCardHuKey(stData, answerData, resultMax);						
				answerData.pop(&stData);
			}
			if (vctKingIndex.size() >= 2)
			{
				if (vctTemp1.size() > 0)
				{
					stHuKeyData sKeyData(6, vctKingIndex[0], vctKingIndex[1], vctTemp1[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
				if (vctTemp2.size() > 0)
				{
					stHuKeyData sKeyData(6, vctKingIndex[0], vctKingIndex[1], vctTemp2[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
				if (vctTemp3.size() > 0)
				{
					stHuKeyData sKeyData(6, vctKingIndex[0], vctKingIndex[1], vctTemp3[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
			}
			if (vctKingIndex.size() >= 1)
			{
				if (vctTemp1.size() > 0 && vctTemp2.size() > 0)
				{
					stHuKeyData sKeyData(6, vctKingIndex[0], vctTemp1[0], vctTemp2[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
				if (vctTemp2.size() > 0 && vctTemp3.size() > 0)
				{
					stHuKeyData sKeyData(6, vctKingIndex[0], vctTemp2[0], vctTemp3[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
				if (vctTemp1.size() > 0 && vctTemp3.size() > 0)
				{
					stHuKeyData sKeyData(6, vctKingIndex[0], vctTemp1[0], vctTemp3[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
			}
		}
		{	//【2.6】 1 2 3 或带王
			vector<char> vctTemp1 = stData.getcardsNumByVal(0x01);
			vector<char> vctTemp2 = stData.getcardsNumByVal(0x02);
			vector<char> vctTemp3 = stData.getcardsNumByVal(0x03);
			if (vctTemp1.size() > 0 && vctTemp2.size() > 0 && vctTemp3.size() > 0)
			{
				stHuKeyData sKeyData(3, vctTemp1[0], vctTemp2[0], vctTemp3[0]);			
				answerData.push(sKeyData, &stData);
				getCardHuKey(stData, answerData, resultMax);						
				answerData.pop(&stData);
			}
			if (vctKingIndex.size() >= 2)
			{
				if (vctTemp1.size() > 0)
				{
					stHuKeyData sKeyData(3, vctKingIndex[0], vctKingIndex[1], vctTemp1[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
				if (vctTemp2.size() > 0)
				{
					stHuKeyData sKeyData(3, vctKingIndex[0], vctKingIndex[1], vctTemp2[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
				if (vctTemp3.size() > 0)
				{
					stHuKeyData sKeyData(3, vctKingIndex[0], vctKingIndex[1], vctTemp3[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
			}
			if (vctKingIndex.size() >= 1)
			{
				if (vctTemp1.size() > 0 && vctTemp2.size() > 0)
				{
					stHuKeyData sKeyData(3, vctKingIndex[0], vctTemp1[0], vctTemp2[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
				if (vctTemp2.size() > 0 && vctTemp3.size() > 0)
				{
					stHuKeyData sKeyData(3, vctKingIndex[0], vctTemp2[0], vctTemp3[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
				if (vctTemp1.size() > 0 && vctTemp3.size() > 0)
				{
					stHuKeyData sKeyData(3, vctKingIndex[0], vctTemp1[0], vctTemp3[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
			}
		}
		{	//【2.7】 二七十 或带王
			vector<char> vctTemp1 = stData.getcardsNumByVal(0x12);
			vector<char> vctTemp2 = stData.getcardsNumByVal(0x17);
			vector<char> vctTemp3 = stData.getcardsNumByVal(0x1A);
			if (vctTemp1.size() > 0 && vctTemp2.size() > 0 && vctTemp3.size() > 0)
			{
				stHuKeyData sKeyData(6, vctTemp1[0], vctTemp2[0], vctTemp3[0]);			
				answerData.push(sKeyData, &stData);
				getCardHuKey(stData, answerData, resultMax);						
				answerData.pop(&stData);
			}
			if (vctKingIndex.size() >= 2)
			{
				if (vctTemp1.size() > 0)
				{
					stHuKeyData sKeyData(6, vctKingIndex[0], vctKingIndex[1], vctTemp1[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
				if (vctTemp2.size() > 0)
				{
					stHuKeyData sKeyData(6, vctKingIndex[0], vctKingIndex[1], vctTemp2[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
				if (vctTemp3.size() > 0)
				{
					stHuKeyData sKeyData(6, vctKingIndex[0], vctKingIndex[1], vctTemp3[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
			}
			if (vctKingIndex.size() >= 1)
			{
				if (vctTemp1.size() > 0 && vctTemp2.size() > 0)
				{
					stHuKeyData sKeyData(6, vctKingIndex[0], vctTemp1[0], vctTemp2[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
				if (vctTemp2.size() > 0 && vctTemp3.size() > 0)
				{
					stHuKeyData sKeyData(6, vctKingIndex[0], vctTemp2[0], vctTemp3[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
				if (vctTemp1.size() > 0 && vctTemp3.size() > 0)
				{
					stHuKeyData sKeyData(6, vctKingIndex[0], vctTemp1[0], vctTemp3[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
			}
		}
		{	//【2.8】 2 7 10 或带王
			vector<char> vctTemp1 = stData.getcardsNumByVal(0x02);
			vector<char> vctTemp2 = stData.getcardsNumByVal(0x07);
			vector<char> vctTemp3 = stData.getcardsNumByVal(0x0A);
			if (vctTemp1.size() > 0 && vctTemp2.size() > 0 && vctTemp3.size() > 0)
			{
				stHuKeyData sKeyData(3, vctTemp1[0], vctTemp2[0], vctTemp3[0]);			
				answerData.push(sKeyData, &stData);
				getCardHuKey(stData, answerData, resultMax);						
				answerData.pop(&stData);
			}
			if (vctKingIndex.size() >= 2)
			{
				if (vctTemp1.size() > 0)
				{
					stHuKeyData sKeyData(3, vctKingIndex[0], vctKingIndex[1], vctTemp1[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
				if (vctTemp2.size() > 0)
				{
					stHuKeyData sKeyData(3, vctKingIndex[0], vctKingIndex[1], vctTemp2[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
				if (vctTemp3.size() > 0)
				{
					stHuKeyData sKeyData(3, vctKingIndex[0], vctKingIndex[1], vctTemp3[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
			}
			if (vctKingIndex.size() >= 1)
			{
				if (vctTemp1.size() > 0 && vctTemp2.size() > 0)
				{
					stHuKeyData sKeyData(3, vctKingIndex[0], vctTemp1[0], vctTemp2[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
				if (vctTemp2.size() > 0 && vctTemp3.size() > 0)
				{
					stHuKeyData sKeyData(3, vctKingIndex[0], vctTemp2[0], vctTemp3[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
				if (vctTemp1.size() > 0 && vctTemp3.size() > 0)
				{
					stHuKeyData sKeyData(3, vctKingIndex[0], vctTemp1[0], vctTemp3[0]);			
					answerData.push(sKeyData, &stData);
					getCardHuKey(stData, answerData, resultMax);						
					answerData.pop(&stData);
				}
			}
		}		

		if (answerData.nGetHuPer() <= resultMax.nGetHuPer() || answerData.nGetHuPer() < 1)
		{
			return;
		}

		map<int, stHuKeyData> mapHuKeyVal;
		map<int, stHuKeyData> mapHuKeyVal0;
		int nAll = stLeftCard.size();		
			
		for (int i=0; i<nAll; ++i)
		{				
			for (int j=i+1; j<nAll; ++j)
			{
				for (int k=j+1; k<nAll; ++k)
				{
					int key = getKey(stData.getVal(stLeftCard.getVal(i)), stData.getVal(stLeftCard.getVal(j)), stData.getVal(stLeftCard.getVal(k)), 0x00);
					int nHuKey = getCardVal(key);
					if (nHuKey > 0)										
					{
						mapHuKeyVal[key] = stHuKeyData(nHuKey, stLeftCard.getVal(i), stLeftCard.getVal(j), stLeftCard.getVal(k));
					}
					else if(nHuKey == 0)
					{
						mapHuKeyVal0[key] = stHuKeyData(0, stLeftCard.getVal(i), stLeftCard.getVal(j), stLeftCard.getVal(k));
					}
				}
			}
		}	

// 		if (!mapHuKeyVal.empty())
// 		{
// 			return;
// 		}

		map<int, stHuKeyData>::iterator iter = mapHuKeyVal0.begin();
		int nHuVal = resultMax.nGetHuPer();	
		for (; iter != mapHuKeyVal0.end(); ++iter)
		{
			answerData.push(iter->second, &stData);
			getCardHuKey(stData, answerData, resultMax);
			answerData.pop(&stData);
			if (resultMax.nGetHuPer() > nHuVal)
			{
				break;
			}
			if (g_bError) break;				
		}		
	}	
}

bool getCardHuKeyInit(vector<char> &vctCardIn, stAnswer &resultAnswer)
{	
	size_t nSizeCard = vctCardIn.size();
	if (nSizeCard == 0)
	{
		return false;
	}
	// 统计牌
	g_bError = false;

	char						nLastKey = vctCardIn[nSizeCard-1];
	stCardData					sCardData;
	stAnswer					sAnswer;		
	map<char, vector<char> >	mapKeyCount;

	for (size_t i=0; i<nSizeCard; ++i)
	{		
		sCardData.push(vctCardIn[i]);
		if (i != nSizeCard-1)	// 最后一张牌暂不统计
		{
			mapKeyCount[vctCardIn[i]].push_back(i);
		}		
	}
	int	nKingNum = mapKeyCount[0xff].size();	// 王牌个数
	map<char, vector<char> >	mapCard[5];
	// 优先拿出四个的
	map<char, vector<char>>::iterator iter = mapKeyCount.begin();
	for (; iter!=mapKeyCount.end(); ++iter)
	{
		vector<char> &vctIndex = iter->second;
		if (vctIndex.size() == 4 && iter->first != (char)0xff)
		{			
			int nHuKey = getCardVal(sCardData.getVal(vctIndex[0]), sCardData.getVal(vctIndex[1]), sCardData.getVal(vctIndex[2]), sCardData.getVal(vctIndex[3]));			
			sAnswer.push(stHuKeyData(nHuKey, vctIndex[0], vctIndex[1], vctIndex[2], vctIndex[3]), &sCardData);
		}
		mapCard[vctIndex.size()][iter->first] = iter->second;		
	}

	for (int i=min(nKingNum, 3); i>=0; --i)
	{
		stCardData sCardTemp = sCardData;
		stAnswer sAnswerTemp = sAnswer;		
		int nKingComb = nKingNum-i;		// 用于组合的牌中王的个数
		if (i == 0)
		{			
			if (MAX_TUN_VAL <= resultAnswer.nGetHuPer()) 
				continue;
			int nKing = nKingComb;
			map<char, vector<char>>::iterator iter = mapCard[3].find(nLastKey);
			if (iter != mapCard[3].end())
			{		
				vector<char> &vctIndex = iter->second;
				int nHuKey = getCardVal(nLastKey, nLastKey, nLastKey, nLastKey);				
				sAnswerTemp.push(stHuKeyData(nHuKey, vctIndex[0], vctIndex[1], vctIndex[2], nSizeCard-1), &sCardTemp);				
				mapCard[3].erase(iter);
				g_bError = false;
				getCardHuKey(sCardTemp, sAnswerTemp, resultAnswer);				
			}
		}
		else if (i == 1)
		{
			if (2*MAX_TUN_VAL <= resultAnswer.nGetHuPer()) 
				continue;
			vector<char> &vctIndex = mapKeyCount[0xff];
			sAnswerTemp.setPer(2);
			sAnswerTemp.push(stHuKeyData(0, vctIndex[0], nSizeCard-1, -1, -1, true), &sCardTemp);			
			g_bError = false;
			getCardHuKey(sCardTemp, sAnswerTemp, resultAnswer);			
		}
		else if (i == 2)
		{			
			if (4*MAX_TUN_VAL <= resultAnswer.nGetHuPer()) 
				continue;
			vector<char> &vctIndex = mapKeyCount[0xff];
			int nHuKey = getCardVal(0xff, 0xff, nLastKey, 0);
			sAnswerTemp.setPer(4);
			sAnswerTemp.push(stHuKeyData(nHuKey, vctIndex[0], vctIndex[1], nSizeCard-1, -1, true), &sCardTemp);
			g_bError = false;
			getCardHuKey(sCardTemp, sAnswerTemp, resultAnswer);			
		}
		else if (i == 3)
		{
			vector<char> &vctIndex = mapKeyCount[0xff];
			int nHuKey = getCardVal(0xff, 0xff, 0xff, nLastKey);
			sAnswerTemp.setPer(8);			
			sAnswerTemp.push(stHuKeyData(nHuKey, vctIndex[0], vctIndex[1], vctIndex[2], nSizeCard-1, true), &sCardTemp);
			g_bError = false;
			getCardHuKey(sCardTemp, sAnswerTemp, resultAnswer);			
		}	
		//*/
	}	
	if (MAX_TUN_VAL <= resultAnswer.nGetHuPer()) 
		return true;
	g_bError = false;
	getCardHuKey(sCardData, sAnswer, resultAnswer);	

	return resultAnswer.nGetHuPer() > 0;
}

void main()
{
	char cbCardData[] =
	{
		0x11,0x11,0x01,
		0x02,0x12,0x02,
		0x03,0x13,0x03,	
		0x07,0x17,0x07,
		0x08,0x08,0x1A,
		0x0A,0xFF,0xFF,
		0xFF,0xFF,0x11,
	};
	//*/
/*	char cbCardData[] =
	{
		0x01,0x11,0x12,
		0x12,0x12,0x12,
		0x03,0x13,0x03,
		0x17,0x17,0x17,
		0x08,0x18,0x08,
		0x09,0xff,0x09,
		0xff,0xff,0x17,
	};
	//*/
/*	char cbCardData[] =
	{
		0x11,0x11,0x01,
		0x12,0x12,0x02,
		0x13,0x13,0x03,
		0x14,0x04,0x04,
		0x15,0x05,0x05,
		0x16,0x06,0x06,
		0x17,0x07,0x07,
	};
	//*/
/*	char cbCardData[] =
	{
		0x11,0x11,0x01,
		0x12,0x12,0x02,
		0x13,0x13,0x03,
		0x17,0x17,0x07,
		0x1A,0x1A,0x0A,
	};
	//*/
/*	char cbCardData[] =
	{
		0x01,0x01,0x01,
		0x02,0x02,0x02,
		0x03,0x03,0x03,
		0x04,0x04,0x04,
		0x05,0x05,0x05,
		0x06,0x06,0x06,
		0x07,0x07,0x07,
	};
	//*/

	vector<char> vctTest;
	int nSize = sizeof(cbCardData);
	for (int i=0; i<nSize; ++i)
	{
		vctTest.push_back(cbCardData[i]);
	}
		
	int nVal = getCardVal(0x14, 0x12, 0x13);
	cout<<"val = "<<nVal<<endl;
	
	int nTick = GetTickCount();
	stAnswer sResult;
	for (int i=0; i<1; ++i)
	{
		sResult.clear();
		getCardHuKeyInit(vctTest, sResult);
	}	
	int nHuPer = max(0, (sResult.nHuVal-15)/3 + 1);
	cout<<"Result HuVal = "<<(int)sResult.nHuVal<<" ("<<nHuPer<<")"<<endl;
	cout<<"Result AllMoneyPer = "<<nHuPer<<"*"<<(int)sResult.nGetHuPer()/nHuPer<<" ("<<(int)sResult.nGetHuPer()<<")"<<endl;
	cout<<"ms = "<<GetTickCount() - nTick<<endl;

	char chrTemp[200];
	cout<<"/////////////////////////////////////"<<endl;
	int nAllKey = 0;
	for (size_t n=0; n<sResult.vctData.size(); ++n)
	{
		memset(chrTemp, 0, sizeof(chrTemp));
		stHuKeyData &stTemp = sResult.vctData[n];
		char nVal[4] = {};
		for (size_t i=0; i<4; ++i)
		{
			if (i < stTemp.vctIndex.size())
			{
				if (stTemp.vctIndex[i] >= 0 && stTemp.vctIndex[i] < (int)vctTest.size())
					nVal[i] = vctTest[stTemp.vctIndex[i]];
			}
		}
		sprintf(chrTemp, "val=%d \t data:\t %02x\t %02x\t %02x\t %02x\t key:%d \n", stTemp.nHuKey, (BYTE)nVal[0], (BYTE)nVal[1], (BYTE)nVal[2], (BYTE)nVal[3], stTemp.bKey);			
		cout<<chrTemp;
		if (nVal[0]!= 0 && nVal[0] == nVal[1] && nVal[2] == nVal[3] && nVal[3] == 0x00)
			nAllKey += 0;
		else 
			nAllKey += getCardVal(nVal[0], nVal[1], nVal[2], nVal[3]);
	}
	cout<<"huVal = "<<nAllKey<<endl;
	cout<<endl;
	//*/
	Sleep(100000);
}

#endif