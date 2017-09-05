#include "HuTipsMJ.h"
#include <algorithm>
#include <string.h>

using namespace std;

//初始化
void CHuTipsMJ::init()
{
	vector<BYTE> vctTemp;
	m_mapGangSp.clear();	
}

bool CHuTipsMJ::Remove(stCardData &stData, BYTE* byDelList, BYTE iNum)
{
	if (byDelList == NULL || iNum > stData.byNum)
	{
		return false;
	}
	return stData.DelCard(byDelList, iNum);
}

BYTE CHuTipsMJ::RandAllCard(BYTE iCard[], BYTE iCardCount)
{
	for (int i=0; i<iCardCount; ++i)
	{
		BYTE randIndex = rand()%(iCardCount-i);
		if (randIndex != 0)
		{
			BYTE byTemp = iCard[randIndex + i];
			iCard[randIndex + i] = iCard[i];
			iCard[i] = byTemp;
		}
	}
	return iCardCount;
}

bool CHuTipsMJ::IsInvalideIndex(BYTE byIndex)
{
	return byIndex >= MAX_TOTAL_TYPE; 
}

BYTE CHuTipsMJ::GetCardCor(BYTE byCardVal)
{	
	return (byCardVal/0x10);
}
BYTE CHuTipsMJ::GetCardVal(BYTE byCardVal)
{
	return (byCardVal & 0x0F);
}

//----------------------------------------------------------------------
//-------游戏基本逻辑算法-------------------

//投骰子
BYTE CHuTipsMJ::GetDiceVal()
{
	return rand()%6+1;
}

bool CHuTipsMJ::CheckCanPeng(stCardData &stData, BYTE byPengCard)
{
	BYTE byIndex = getIndexByVal(byPengCard);
	if (byIndex >= MAX_TOTAL_TYPE)
	{
		return false;
	}
	return stData.byCardCount[byIndex] >= 2;
}

//检测是否能够明杠
bool CHuTipsMJ::CheckCanVisbleGang(stCardData &stData, BYTE byGangCard)
{
	BYTE byIndex = getIndexByVal(byGangCard);
	if (byIndex >= MAX_TOTAL_TYPE)
	{
		return false;
	}
	return stData.byCardCount[byIndex] >= 3;
}

//检测是否能够暗杠
vector<stNodeMJ> CHuTipsMJ::CheckSelfHiddenGang(stCardData &stData)
{
	vector<stNodeMJ> vctNodeOut;
	for (int i=0; i<MAX_TOTAL_TYPE; ++i)
	{
		if (stData.byCardCount[i] == 4)
		{
			stNodeMJ stNode;
			stNode.byType = BLOCK_2222_AN;
			stNode.addCardInfo(getValByIndex(i), 4);
			vctNodeOut.push_back(stNode);
		}
	}	
	return vctNodeOut;
}

//检测是能够 特殊杠
vector<stNodeMJ> CHuTipsMJ::CheckSelfGangSpecial(stCardData &stData)
{
	vector<stNodeMJ> vctNodeOut;
	return vctNodeOut;
}

//获取特殊杠结构
bool CHuTipsMJ::getGangNodeSpecial(BYTE byCards[3], stNodeMJ &outNode)
{
	vector<BYTE> vctHave;
	for (int i=0; i<3; ++i)
		vctHave.push_back(byCards[i]);

	map<int, BYTE>::iterator iterFind = m_mapGangSp.find(getKey(vctHave));
	if (iterFind != m_mapGangSp.end())
	{
		memset(&outNode, 0, sizeof(stNodeMJ));
		outNode.byType = iterFind->second;
		outNode.addCardInfo(vctHave[0], 1);
		outNode.addCardInfo(vctHave[1], 1);
		outNode.addCardInfo(vctHave[2], 1);		
		return true;
	}
	return false;
}

bool CHuTipsMJ::getGangNodeSpecial_4(BYTE byCards[4], stNodeMJ &outNode)
{
	vector<BYTE> vctHave;
	for (int i=0; i<4; ++i)
		vctHave.push_back(byCards[i]);

	map<int, BYTE>::iterator iterFind = m_mapGangSp.find(getKey(vctHave));
	if (iterFind != m_mapGangSp.end())
	{
		memset(&outNode, 0, sizeof(stNodeMJ));
		outNode.byType = iterFind->second;
		outNode.addCardInfo(vctHave[0], 1);
		outNode.addCardInfo(vctHave[1], 1);
		outNode.addCardInfo(vctHave[2], 1);	
		outNode.addCardInfo(vctHave[3], 1);
		return true;
	}
	return false;
}

// bAddSameOne:true 同类型的牌只存一个 false 每组牌都存
bool CHuTipsMJ::getSpecialNode(stCardData &stData, BYTE byCardVal[], BYTE num, stNodeMJ &stNodeOut)
{	
	return false;
}

//将某张牌进行补杠
bool CHuTipsMJ::CheckBuGang(std::vector<stNodeMJ> &vctNodeSave, BYTE byCurHandPai)
{
	if (!IsValidCard(byCurHandPai) || vctNodeSave.empty())    
		return false;

	int iCount = vctNodeSave.size();
	for (int i=0; i<iCount; i++)
	{
		if (vctNodeSave[i].byType == BLOCK_222 && vctNodeSave[i].isHaveCard(byCurHandPai))		// pino
		{
			vctNodeSave[i].byType = BLOCK_2222_BU;
			vctNodeSave[i].addCardInfo(byCurHandPai, 1);
			return true;			
		}
	}	
	return false;
}


//将某张牌进行补杠
bool CHuTipsMJ::CheckBuGang_Spe(std::vector<stNodeMJ> &vctNodeSave, BYTE byCurHandPai)
{	
	return false;
}

// 检测手上的牌可补杠的牌的列表
vector<BYTE> CHuTipsMJ::CheckSelfBuGang(stCardData stData, std::vector<stNodeMJ> &vctNodeSave)
{			
	vector<BYTE> vctGang;	
	for (int i=0; i<MAX_TOTAL_TYPE; ++i)
	{		
		if (stData.byCardCount[i] > 0)
		{
			BYTE byCardVal = getValByIndex(i);
			if (CheckBuGang(vctNodeSave, byCardVal))
			{
				vctGang.push_back(byCardVal);
			}
		}
	}	
	return vctGang;
}

//检测手牌能否特殊杠补杠
vector<BYTE> CHuTipsMJ::CheckSelfBuGang_Spe(stCardData stData, std::vector<stNodeMJ> &vctNodeSave)
{
	vector<BYTE> vctGang;	
	for (int i=0; i<MAX_TOTAL_TYPE; ++i)
	{		
		if (stData.byCardCount[i] > 0)
		{
			BYTE byCardVal = getValByIndex(i);
			if (CheckBuGang_Spe(vctNodeSave, byCardVal))
			{
				vctGang.push_back(byCardVal);
			}
		}
	}	
	return vctGang;
}

//点炮胡
//bool CHuTipsMJ::CheckWin_Dian(stCardData stData, std::vector<stNodeMJ>& vctNodeOut, BYTE byOutCard, BYTE byCardNaiZi, map<stKey, stAnswer> &mapHuanCun)
bool CHuTipsMJ::CheckWin_Dian(stCardData &stData, BYTE byCardNaiZi)
{	
 	BYTE byNaiZi = getIndexByVal(byCardNaiZi);

	return CanWin2(stData, byNaiZi);
}

//胡-七小队
bool CHuTipsMJ::CheckWin_7XiaoDui(stCardData stData, std::vector<stNodeMJ>& vctNodeOut, BYTE byOutCard, BYTE byNaiziCard)
{
	return false;
}

//自摸胡-普通胡
bool CHuTipsMJ::CheckWin_ZiMo_Normal(stCardData stData, std::vector<stNodeMJ>& vctNodeOut, BYTE byNaiziCard)
{	
	BYTE byNaiZi = getIndexByVal(byNaiziCard);
	stAnswer stResult;
	bool bSuc = CanWin(stData, stResult, byNaiZi);
	if (bSuc)
		stResult.getVctNode(vctNodeOut);

	return bSuc;
}

bool CHuTipsMJ::CanWin2(stCardData &stData, BYTE byIndexNaiZi)
{
// 	BYTE nNaiZi = 0;
// 	if (byIndexNaiZi < MAX_TOTAL_TYPE)
// 		nNaiZi = stData.byCardCount[byIndexNaiZi];

// 	if (stData.byNum == 0)
// 		return true;

	BYTE nNaiZi = stData.byCardCount[byIndexNaiZi];
	return CanWin_Do_Nai(stData, byIndexNaiZi);
}

bool CHuTipsMJ::CanWin(stCardData &stData, stAnswer& stResult, BYTE byIndexNaiZi/*INVALID_VAL*/, int nFlag/*0*/)
{	
	BYTE nNaiZi = 0;
	if (byIndexNaiZi < MAX_TOTAL_TYPE)	
		nNaiZi = stData.byCardCount[byIndexNaiZi];	
	
	if (stData.byNum == 0)	
		return true;

	if (nNaiZi == 0 && stResult.nNum == 0)
		return CanWin_Do(stData);
	else
		return CanWin_Do_Nai(stData, byIndexNaiZi);

	return false;
}

bool CHuTipsMJ::CanWin_Do(stCardData &stData)
{
	for (int cor=0; cor<enColorMJ_Max; ++cor)
	{
		stColorData stColorTemp;
		stColorTemp.byCorType = cor;
		int nAll = (cor == enColorMJ_FenZi) ? 7 : MAX_VAL_NUM;
		if (cor*MAX_VAL_NUM + nAll <= MAX_TOTAL_TYPE)
		{
			memcpy(stColorTemp.byCount, stData.byCardCount+cor*MAX_VAL_NUM, nAll);		
			for (int i=0; i<nAll; ++i)
				stColorTemp.byNum += stColorTemp.byCount[i];

			if (ColorMatch2(stColorTemp) == false)
			{
				return false;
			}
		}		
	}
	return true;
}

bool CHuTipsMJ::ColorMatch(stColorData &stData, int nFlag/*0*/)
{
	if (stData.byNum == 0) 
		return true;

	int nLeft = stData.byNum%3;
	if (nLeft == 1)
		return false;

	int nAll = (stData.byCorType == enColorMJ_FenZi) ? 7 : MAX_VAL_NUM;
	if (nLeft == 2)	// 有将
	{
		for (int i=0; i<nAll; ++i)
		{
			if (stData.byCount[i] == 2)
			{				
				stData.byCount[i] -= 2;
				stData.byNum -= 2;
				if (ColorMatch(stData))
					return true;
				stData.byCount[i] += 2;
				stData.byNum += 2;
			}
		}
		for (int i=0; i<nAll; ++i)
		{
			if (stData.byCount[i] > 2)
			{				
				stData.byCount[i] -= 2;
				stData.byNum -= 2;
				if (ColorMatch(stData))
					return true;

				stData.byCount[i] += 2;
				stData.byNum += 2;
			}
		}
	}
	int index = nFlag%MAX_COUNT_FLAG_NUM;
	int nLev  = nFlag/MAX_COUNT_FLAG_NUM;
	if (nLev < enBlockLev_222)
		index = 0;
	if (nLev <= enBlockLev_222)
	{
		int i = index;
		for (; i<nAll; ++i)
		{
			if (stData.byCount[i] >= 3)
			{				
				stData.byCount[i] -= 3;
				stData.byNum -= 3;
				if (ColorMatch(stData, enBlockLev_222*MAX_COUNT_FLAG_NUM+i+1))
					return true;

				stData.byCount[i] += 3;
				stData.byNum += 3;
			}
		}
	}		
//	if (stData.byCorType != enColorMJ_FenZi )
	if (stData.byCorType != enColorMJ_FenZi && nLev <= enBlockLev_234)
	{		
		if (nLev < enBlockLev_234)
			index = 0;

		for (int m=0; m<index; ++m)
			if (stData.byCount[m] > 0) return false;

		int i = index;
		for (; i<nAll-2; ++i)
		{
			if (stData.byCount[i] > 0 && stData.byCount[i+1] > 0 && stData.byCount[i+2] > 0)
			{				
				stData.byCount[i]	-= 1;
				stData.byCount[i+1] -= 1;
				stData.byCount[i+2] -= 1;
				stData.byNum -= 3;
				if (ColorMatch(stData, enBlockLev_234*MAX_COUNT_FLAG_NUM+i))
					return true;

				stData.byCount[i]	+= 1;
				stData.byCount[i+1] += 1;
				stData.byCount[i+2] += 1;
				stData.byNum += 3;
				return false;
			}
		}
	}	
	return false;
}

bool CHuTipsMJ::CanWin_Do_Nai(stCardData &stData, BYTE byIndexNaiZi/*INVALID_VAL*/)
{
	if (stData.byNum % 3 != 2)
		return false;

	int nNaiZiNum = 0;
	if (byIndexNaiZi < MAX_TOTAL_TYPE)
	{
		nNaiZiNum = stData.byCardCount[byIndexNaiZi];
		stData.byCardCount[byIndexNaiZi] = 0;
		stData.byNum -= nNaiZiNum;
	}

	int nNaiZiMax[enColorMJ_Max] = {4, 4, 4, 4};
	stColorData stColorTemp[enColorMJ_Max];
	for (int cor=0; cor<enColorMJ_Max; ++cor)
	{		
		stColorTemp[cor].byCorType = cor;
		int nAll = (cor == enColorMJ_FenZi) ? 7 : MAX_VAL_NUM;
		if (cor*MAX_VAL_NUM + nAll <= MAX_TOTAL_TYPE)
		{
			memcpy(stColorTemp[cor].byCount, stData.byCardCount+cor*MAX_VAL_NUM, nAll);
			for (int i=0; i<nAll; ++i)
				stColorTemp[cor].byNum += stColorTemp[cor].byCount[i];			
		}
		if (stColorTemp[cor].byNum == 0)
			nNaiZiMax[cor] = 0;
	}
	
	BYTE byLeftNum[enColorMJ_Max] = {};
	for (int i1=0; i1<=min(nNaiZiMax[0], nNaiZiNum); ++i1)
	{		
		byLeftNum[enColorMJ_WAN] = (stColorTemp[0].byNum+i1)%3;
		if (byLeftNum[enColorMJ_WAN] == 1) continue;
		for (int i2=0; i2<=min(nNaiZiMax[1], nNaiZiNum-i1); ++i2)
		{
			byLeftNum[enColorMJ_TONG] = (stColorTemp[1].byNum+i2)%3;
			if (byLeftNum[enColorMJ_TONG] == 1) continue;
			for (int i3=0; i3<=min(nNaiZiMax[2], nNaiZiNum-i1-i2); ++i3)
			{
				byLeftNum[enColorMJ_TIAO] = (stColorTemp[2].byNum+i3)%3;
				if (byLeftNum[enColorMJ_TIAO] == 1) continue;
				int i4 = nNaiZiNum-i1-i2-i3;
				if (i4 <= nNaiZiMax[3])
				{
					byLeftNum[enColorMJ_FenZi] = (stColorTemp[3].byNum+i4)%3;
					if (byLeftNum[enColorMJ_FenZi] == 1 || byLeftNum[0]+byLeftNum[1]+byLeftNum[2]+byLeftNum[3] != 2) continue;

					stColorData stTemp[enColorMJ_Max];
					memcpy(stTemp, stColorTemp, sizeof(stTemp));					
					
					if (stColorTemp[0].byNum>0 && !ColorMatch2(stTemp[0], 0, getValByIndex(byIndexNaiZi), i1))
						continue;
					if (stColorTemp[1].byNum>0 && !ColorMatch2(stTemp[1], 0, getValByIndex(byIndexNaiZi), i2))
						continue;
					if (stColorTemp[2].byNum>0 && !ColorMatch2(stTemp[2], 0, getValByIndex(byIndexNaiZi), i3))
						continue;
					if (stColorTemp[3].byNum>0 && !ColorMatch2(stTemp[3], 0, getValByIndex(byIndexNaiZi), i4))
						continue;

					return true;
				}
			}			
		}
	}	
	return false;
}

bool CHuTipsMJ::CanWin_Do_Nai2(stCardData &stData, stAnswer& stResult, BYTE byIndexNaiZi, map<stKey, stAnswer> &mapHuanCun)
{
	if (stData.byNum % 3 != 2)
		return false;

	int nNaiZiNum = 0;
	if (byIndexNaiZi < MAX_TOTAL_TYPE)
	{
		nNaiZiNum = stData.byCardCount[byIndexNaiZi];
		stData.byCardCount[byIndexNaiZi] = 0;
		stData.byNum -= nNaiZiNum;
	}

	int nNaiZiMax[enColorMJ_Max] = {4, 4, 4, 4};
	stColorData stColorTemp[enColorMJ_Max];
	for (int cor=0; cor<enColorMJ_Max; ++cor)
	{		
		stColorTemp[cor].byCorType = cor;
		int nAll = (cor == enColorMJ_FenZi) ? 7 : MAX_VAL_NUM;
		if (cor*MAX_VAL_NUM + nAll <= MAX_TOTAL_TYPE)
		{
			memcpy(stColorTemp[cor].byCount, stData.byCardCount+cor*MAX_VAL_NUM, nAll);
			for (int i=0; i<nAll; ++i)
				stColorTemp[cor].byNum += stColorTemp[cor].byCount[i];			
		}
		if (stColorTemp[cor].byNum == 0)
			nNaiZiMax[cor] = 0;
	}

	for (int i1=0; i1<=min(nNaiZiMax[0], nNaiZiNum); ++i1)
	{		
		for (int i2=0; i2<=min(nNaiZiMax[1], nNaiZiNum-i1); ++i2)
		{
			for (int i3=0; i3<=min(nNaiZiMax[2], nNaiZiNum-i1-i2); ++i3)
			{
				int i4 = min(nNaiZiMax[3], nNaiZiNum-i1-i2-i3);
				if (i1+i2+i3+i4 == nNaiZiNum)
				{
					stAnswer stResultTemp[4];
					stColorData stTemp[enColorMJ_Max];
					memcpy(stTemp, stColorTemp, sizeof(stTemp));
					if ((stColorTemp[0].byNum+i1)%3 == 1 || (stColorTemp[1].byNum+i2)%3 == 1 ||
						(stColorTemp[2].byNum+i3)%3 == 1 || (stColorTemp[3].byNum+i4)%3 == 1 )
					{
						continue;
					}
					stKey strKey;
					if (stColorTemp[0].byNum > 0)
					{						
// 						if (stColorTemp[0].byNum + i1 >= MIN_HU_HUAN_CUN)
// 						{
// 							strKey = stColorTemp[0].getStringKey(byIndexNaiZi, i1);
// 							map<stKey, stAnswer>::iterator iter = mapHuanCun.find(strKey);
// 							if (iter != mapHuanCun.end())
// 								stResultTemp[0] = iter->second;
// 						}
						if (stResultTemp[0].nNum == 0)
						{
							if (!ColorMatch2(stTemp[0], 0, getValByIndex(byIndexNaiZi), i1))
								continue;
// 							else if (stColorTemp[0].byNum + i1 >= MIN_HU_HUAN_CUN)
// 								mapHuanCun[strKey] = stResultTemp[0];
						}
					}
					if (stColorTemp[1].byNum > 0)
					{						
// 						if (stColorTemp[1].byNum + i2 >= MIN_HU_HUAN_CUN)
// 						{
// 							strKey = stColorTemp[1].getStringKey(byIndexNaiZi, i2);
// 							map<stKey, stAnswer>::iterator iter = mapHuanCun.find(strKey);
// 							if (iter != mapHuanCun.end())
// 								stResultTemp[1] = iter->second;
// 						}
						if (stResultTemp[1].nNum == 0)
						{
							if (!ColorMatch2(stTemp[1], 0, getValByIndex(byIndexNaiZi), i2))
								continue;
// 							else if (stColorTemp[1].byNum + i2 >= MIN_HU_HUAN_CUN)
// 								mapHuanCun[strKey] = stResultTemp[1];
						}
					}
					if (stColorTemp[2].byNum > 0)
					{						
// 						if (stColorTemp[2].byNum + i3 >= MIN_HU_HUAN_CUN)
// 						{
// 							strKey = stColorTemp[2].getStringKey(byIndexNaiZi, i3);
// 							map<stKey, stAnswer>::iterator iter = mapHuanCun.find(strKey);
// 							if (iter != mapHuanCun.end())
// 								stResultTemp[2] = iter->second;
// 						}
						if (stResultTemp[2].nNum == 0)
						{
							if (!ColorMatch2(stTemp[2], 0, getValByIndex(byIndexNaiZi), i3))
								continue;
// 							else if (stColorTemp[2].byNum + i3 >= MIN_HU_HUAN_CUN)
// 								mapHuanCun[strKey] = stResultTemp[2];
						}
					}
					if (stColorTemp[3].byNum > 0)
					{						
// 						if (stColorTemp[3].byNum + i4 >= MIN_HU_HUAN_CUN)
// 						{
// 							strKey = stColorTemp[3].getStringKey(byIndexNaiZi, i4);
// 							map<stKey, stAnswer>::iterator iter = mapHuanCun.find(strKey);
// 							if (iter != mapHuanCun.end())
// 								stResultTemp[3] = iter->second;
// 						}
						if (stResultTemp[3].nNum == 0)
						{
							if (!ColorMatch2(stTemp[3], 0, getValByIndex(byIndexNaiZi), i4))
								continue;
// 							else if (stColorTemp[3].byNum + i4 >= MIN_HU_HUAN_CUN)
// 								mapHuanCun[strKey] = stResultTemp[3];
						}
					}
					stResult.addNode(stResultTemp[0]);
					stResult.addNode(stResultTemp[1]);
					stResult.addNode(stResultTemp[2]);
					stResult.addNode(stResultTemp[3]);
					return true;
				}
			}			
		}
	}	
	return false;
}

bool CHuTipsMJ::ColorMatch2(stColorData &stData, int nFlag/*0*/, BYTE byNaiIndex/*INVALID_VAL*/, BYTE byNumNaiZi/*0*/)
{
	if (byNaiIndex == INVALID_VAL)
		byNumNaiZi = 0;

	int nJiangNum = (stData.byNum + byNumNaiZi) % 3;
	if (nJiangNum == 1)
		return false;

	if (stData.byNum + byNumNaiZi == 0)
		return true;

	int nAll = (stData.byCorType == enColorMJ_FenZi) ? 7 : MAX_VAL_NUM;
	if (nJiangNum == 2)	// 有将
	{
		if (byNumNaiZi >= 2)
		{
			stNodeMJ stNode;
			stNode.byType = BLOCK_22;			
			stNode.addCardInfo(byNaiIndex, 2);
			if (ColorMatch2(stData, 0, byNaiIndex, byNumNaiZi-2))
				return true;
		}
		if (byNumNaiZi >= 1)
		{
			for (int i=0; i<nAll; ++i)
			{
				if (stData.byCount[i] >= 1)
				{					
					stData.byCount[i] -= 1;
					stData.byNum -= 1;
					if (ColorMatch2(stData, 0, byNaiIndex, byNumNaiZi-1))
						return true;
					stData.byCount[i] += 1;
					stData.byNum += 1;
				}
			}
		}		
		for (int i=0; i<nAll; ++i)
		{			
			if (stData.byCount[i] == 2)
			{
				stData.byCount[i] -= 2;
				stData.byNum -= 2;
				if (ColorMatch2(stData, 0, byNaiIndex, byNumNaiZi))
					return true;
				stData.byCount[i] += 2;
				stData.byNum += 2;
			}
		}
		for (int i=0; i<nAll; ++i)
		{
			if (stData.byCount[i] > 2)
			{
				stData.byCount[i] -= 2;
				stData.byNum -= 2;
				if (ColorMatch2(stData, 0, byNaiIndex, byNumNaiZi))
					return true;
				stData.byCount[i] += 2;
				stData.byNum += 2;
			}
		}
		return false;
	}
	int i = nFlag%MAX_COUNT_FLAG_NUM;
	int nLev  = nFlag/MAX_COUNT_FLAG_NUM;	
	if (nLev <= enBlockLev_222)
	{
		if (nLev < enBlockLev_222)
			i = 0;

		for (; i<nAll; ++i)
		{
			if (stData.byCount[i] + byNumNaiZi >= 3)
			{
				BYTE nCardsNeed = min((BYTE)3, stData.byCount[i]);
				BYTE nNaiZiNeed = 3 - nCardsNeed;
				stData.byCount[i]	-= nCardsNeed;
				stData.byNum		-= nCardsNeed;
				if (ColorMatch2(stData, enBlockLev_222*MAX_COUNT_FLAG_NUM+i+1, byNaiIndex, byNumNaiZi-nNaiZiNeed))
					return true;
				stData.byCount[i]	+= nCardsNeed;
				stData.byNum		+= nCardsNeed;
			}
		}
	}		

	BYTE byLineNum = 0;
	if (stData.byCorType != enColorMJ_FenZi && nLev <= enBlockLev_234)
	{		
		if (nLev < enBlockLev_234)
			i = 0;

		for (int m=0; m<i; ++m)
			if (stData.byCount[m] > 0) return false;

		for (; i<nAll-2; ++i)
		{		
			int nFlag = 0;
			byLineNum = 0;
			if (stData.byCount[i] > 0)	
			{
				nFlag |= 1;
				++byLineNum;
			}
			if (stData.byCount[i+1] > 0)
			{
				nFlag |= 2;
				++byLineNum;
			}
			if (stData.byCount[i+2] > 0)
			{
				nFlag |= 4;
				++byLineNum;
			}
			if (byLineNum + byNumNaiZi >= 3)
			{
				BYTE nNaiZiNeed = 3 - byLineNum;
				if (nFlag & 1)	stData.byCount[i] -= 1;
				if (nFlag & 2)	stData.byCount[i+1] -= 1;
				if (nFlag & 4)	stData.byCount[i+2] -= 1;
				stData.byNum -= byLineNum;

				if (ColorMatch2(stData, enBlockLev_234*MAX_COUNT_FLAG_NUM+i, byNaiIndex, byNumNaiZi-nNaiZiNeed))
					return true;

				if (nFlag & 1)	stData.byCount[i]	+= 1;
				if (nFlag & 2)	stData.byCount[i+1] += 1;
				if (nFlag & 4)	stData.byCount[i+2] += 1;
				stData.byNum += byLineNum;
				if (nNaiZiNeed == 0)
					return false;
			}
		}		
	}	
	return false;
}

bool CHuTipsMJ::CheckCanPlay(stCardData &stData,BYTE byCard)
{
	BYTE byIndex = getIndexByVal(byCard);
	if (byIndex < MAX_TOTAL_TYPE)
	{
		if (stData.byCardCount[byIndex] > 0)
		{
			return true;
		}
	}	
	return false;
}

vector<BYTE> CHuTipsMJ::getCardList(stNodeMJ &stNode)
{
	vector<BYTE> vctCards;	
	return vctCards;
}

void CHuTipsMJ::setPlayMode(BYTE byMode) 
{
	m_nPlayMode = byMode;
	init();
}

// 获取是否有某模式
bool CHuTipsMJ::isHaveMode(BYTE byFlag)
{
	return (m_nPlayMode&byFlag) == byFlag;
}
