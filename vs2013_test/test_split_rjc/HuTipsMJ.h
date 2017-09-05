#ifndef __MJ_HUTIPS_H__
#define __MJ_HUTIPS_H__

#include <map>
#include "Define.h"
#include "DefineHuTip.h"

/********************************************************************
	created:	pino
	file base:	CHuTipsMJ
	author:		pino	
	purpose:	麻将基础逻辑算法
*********************************************************************/

class CHuTipsMJ
{
public:	
	CHuTipsMJ():m_nPlayMode(0){}

	//初始化
	void init();	
	//从链表中删除一段数据
	bool Remove(stCardData &stData,BYTE* byDelList, BYTE iNum);
	//随机所有的牌
	BYTE RandAllCard(BYTE iCard[], BYTE iCardCount);
	//是否位非法的ai牌数据
	bool IsInvalideIndex(BYTE bAICard); 
	//得到游戏数据的花色
	BYTE GetCardCor(BYTE bGameCard);
	//得到麻将牌的牌点
	BYTE GetCardVal(BYTE bGameCard);
	
	//-----------------------------------------------------------
	//----------------麻将逻辑算法-------------------------------
	//投骰子
	BYTE GetDiceVal();

	//检测是否可碰
	bool CheckCanPeng(stCardData &stData, BYTE byPengCard);  

	//检测是能够 明杠
	bool CheckCanVisbleGang(stCardData &stData,BYTE byGangCard);

	//检测是能够 暗杠
	vector<stNodeMJ> CheckSelfHiddenGang(stCardData &stData);

	//检测是能够 特殊杠
	vector<stNodeMJ> CheckSelfGangSpecial(stCardData &stData);

	//获取特殊杠结构
	bool getGangNodeSpecial(BYTE byCards[3], stNodeMJ &outNode);
	bool getGangNodeSpecial_4(BYTE byCards[4], stNodeMJ &outNode);

	//
	bool getSpecialNode(stCardData &stData, BYTE byCardVal[], BYTE num, stNodeMJ &stNodeOut);
	
	//将某张牌进行补杠
	bool CheckBuGang(std::vector<stNodeMJ> &vctNodeSave, BYTE byCurHandPai);
	//将某张牌进行特殊杠补杠
	bool CheckBuGang_Spe(std::vector<stNodeMJ> &vctNodeSave, BYTE byCurHandPai);

    //检测手牌能否补杠
	vector<BYTE> CheckSelfBuGang(stCardData stData, std::vector<stNodeMJ> &vctNodeSave);
	//检测手牌能否特殊杠补杠
	vector<BYTE> CheckSelfBuGang_Spe(stCardData stData, std::vector<stNodeMJ> &vctNodeSave);
	
	//点炮胡
	//bool CheckWin_Dian(stCardData stData, std::vector<stNodeMJ>& vctNodeOut,BYTE byOutCard,BYTE byNaiziCard, map<stKey, stAnswer> &mapHuanCun);
	bool CheckWin_Dian(stCardData &stData, BYTE byNaiziCard);

	//胡-七小队
	bool CheckWin_7XiaoDui(stCardData stData, std::vector<stNodeMJ>& vctNodeOut, BYTE byOutCard, BYTE byNaiziCard);

	//自摸胡-普通胡
	bool CheckWin_ZiMo_Normal(stCardData stData,std::vector<stNodeMJ>& vctNodeOut, BYTE byNaiziCard);

	//
	bool CanWin(stCardData &stData, stAnswer& stResult, BYTE byIndexNaiZi=INVALID_VAL, int nFlag=0);
	bool CanWin2(stCardData &stData, BYTE byIndexNaiZi);

	// 无赖子时，分颜色计算胡牌
	bool CanWin_Do(stCardData &stData);
	bool ColorMatch(stColorData &stData, int nFlag=0);
	// 有赖子时，分颜色计算胡牌
	bool CanWin_Do_Nai(stCardData &stData, BYTE byIndexNaiZi = INVALID_VAL);
	bool CanWin_Do_Nai2(stCardData &stData, stAnswer& stResult, BYTE byIndexNaiZi, map<stKey, stAnswer> &mapHuanCun);
	bool ColorMatch2(stColorData &stData, int nFlag=0, BYTE byCardNaiZi = INVALID_VAL, BYTE byNumNaiZi = 0);
  
	// 检测手中是否可以把某张牌打出来，及手上必须有这张牌才能出，否则失败
	bool CheckCanPlay(stCardData &stData, BYTE byCard);   

	vector<BYTE> getCardList(stNodeMJ &stNode);
	// 设置玩法模式 enPlayMode
	void setPlayMode(BYTE byMode);
	// 获取是否有某模式
	bool isHaveMode(BYTE byFlag);
	
private:   
	BYTE			m_nPlayMode;	// enPlayMode
	map<int, BYTE>	m_mapGangSp;	// 特殊杠
};

#endif