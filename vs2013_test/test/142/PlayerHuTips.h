/********************************************************************
	created:	pino
	file base:	CPlayerHuTips
	author:		pino	
	purpose:	麻将胡牌提示算法
*********************************************************************/

#ifndef __CPLAYER_HUTIPS_H__
#define __CPLAYER_HUTIPS_H__
#include <map>
#include <string>
#include <vector>
#include <list>

#include "Define.h"
#include "CMD_Mj.h"
#include "HuTipsMJ.h"


using namespace std;

class CPlayerHuTips
{
public:
	CPlayerHuTips(CHuTipsMJ* pAlgor);
	~CPlayerHuTips();

public:
	//游戏开始清理
	void GameBeginClear();
	//初始化数据
	void initData(std::list<BYTE> &handMjList, std::vector<MJCPGNode> &vecMJCPGList);
	
	//设置赖子牌
	void SetNaiZiCard(BYTE byCard)	{m_byNaiZiCard = byCard;}
	BYTE GetNaiZiCard()				{return m_byNaiZiCard;}
	
	//检测是否能听
	bool CheckMyCanTing_14(bool bAllResult = true);	
	bool CheckMyCanTing_13(stCardData &stData, int num = 0);
	bool CheckMyCanTing_13_HuTips(stCardData &stData, stHuData stHuInfo[10]);
	bool JustTryDianHu(stCardData stData, BYTE byMJCard, BYTE byNaiziCard, vector<stNodeMJ> &vecNodeOut);	
	//到得胡牌提示信息
	vector<stHuTips> CheckMyCanTing_HuTips(stCardData &stCardShow, bool bUseCun=true);

	//检测门清
	bool IsMENQING();
	//检查是否包含指定特殊字符
	bool isHaveSpecialCard(stCardData &stData);
	//检查花色种类
	BYTE getCardColorNum(stCardData &stData, BYTE byNaiZiCard=0);
	//检查是否清一色
	bool isQingYiSe(stCardData &stData, BYTE byNaiZiCard=0);
	//检查是否凑一色
	bool isChouYiSe(stCardData &stData, BYTE byNaiZiCard=0);
	//检查是否有一条龙
	bool isHaveYiLong(stCardData &stData);
	//检查是否是飘胡(碰碰胡)
	bool isPiaoHu(vector<stNodeMJ> vctTemp);
	//检查是否是站立（门清）
	bool isZhanLi();
	//得到对对胡的类型
	int  getDuiDuiHuType(vector<stNodeMJ> vctTemp);
	//得到夹胡类型
	int  getJiaHuType(vector<stNodeMJ> &vctNodeOut, BYTE byCard);
	bool isJiaHu();	
	//牌型分析
	int getCardsTypeFlag(BYTE byCard, vector<stNodeMJ> &vctTemp, bool isZhiMo);
	//得到当前牌型分
	int getPerByHuFlag(int nHuFlag);
	//得到当前牌型字符串
	std::string getStringByData(stCardData &stData);
	//刷新剩余牌数量
	void refreshLeftCardsNum(stHuTips &stHu, stCardData &stCardShow);

public:
	map<stKey, stAnswer>		m_mapHuanCun;

private:
	stCardData				m_stCardData;		//手牌数据
	std::vector<stNodeMJ>	m_vecNodeMJ;		//吃碰杠列表
	vector<BYTE>			m_vctUnlockCards;	//听时不锁牌列表
	

	CHuTipsMJ*				m_pAlgorithm;
	BYTE					m_byNaiZiCard;		//赖子牌
};

#endif  //__CPLAYER_H__