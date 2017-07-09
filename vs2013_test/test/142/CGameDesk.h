/********************************************************************
created:	2016/10/11
created:	8:22:2016   14:33
filename: 	CGameDesk.h
author:		jimy.zhu

purpose:	游戏动态库142，长沙红中
*********************************************************************/
#ifndef CGameDesk_h__
#define CGameDesk_h__

#include <vector>
#include "CMD_Mj.h"
#include <list>

using namespace std;

#define IDT_USER_CUT				    1L	   			   //断线定时器 ID 预留十个

//定时器 ID(20-50之间)
//#define TIME_WAITUSER_AGREE             20                 //预留十个ID 20 - 29

#define TIME_PLAYER_1					21			       //玩家1出牌或操作计时器
#define TIME_PLAYER_2					22			       //玩家2出牌或操作计时器
#define TIME_PLAYER_3					23			       //玩家3出牌或操作计时器
#define TIME_PLAYER_4					24			       //玩家4出牌或操作计时器

#define TIME_WAIT_QIANGZHUANG			30			       //等待抢庄定时器
#define TIME_CHECK_END_MONEY 		    31			       //检测钱是否够
#define TIME_GAME_BASE_TIME 		    32			       //游戏中基础定时器
#define TIME_GAME_AUTO_HU 				33			       //自动胡延时

#define TIME_ONE_COUNT					37			       //每秒计时器
#define TIME_SEND_CARD_FINISH			38			       //客户端发牌时间

#define TIME_NOT_ENOUGH_PLAYER			42			       //等待人满计时器
#define TIME_MAX_ID						50			       //最大

#define TIME_MUL_D                      10000L          
#define TIME_SPACE					    100L   		    ///游戏 ID 间隔
#define TIME_START_ID				    1L		    	///定时器开始 ID

//游戏结束标志定义
#define GF_NORMAL						10				//游戏正常结束
#define GF_SAFE							11				//游戏安全结束

class CGameDesk : public IGameDesk
{
private:
	/// 以下为私有成员，各种游戏通用
	IGameMainManager	*m_pGameManager;				///< 房间管理指针
	int                 m_nRoomLevel;                   ///< 房间等级
	int                 m_nRoomID;                      ///< 房间号
	int					m_nDeskIndex;					///< 桌子号
	bool				m_bPlaying;						///< 游戏是否正在进行中
	int					m_nGameStation;					///< 游戏进行状态，由各游戏自行指定    
	
	UINT				m_dwBeginTime;					///< 本局游戏开始时间

	uint64_t            m_un64RoomLevelMinMoney;        ///< 最小金币限制

	ManageInfoStruct    m_ManageInfoStruct;             ///< 房间配置

	//状态
private:
	/// 发送游戏状态，用于刚进入桌及断线重连
	bool SendGameStation(BYTE bDeskStation, UINT uIndex, void * pStationData, UINT uSize);

	/// 等待同意时下发状态
	int GetStationWhileWaitAgree(int nDeskStation, bool bIsWatch, UINT uIndex);

	/// 等待掷骰子时下发状态
	int GetStationWhileWaitTouShaiZi(int nDeskStation, bool bIsWatch, UINT uIndex);

	/// 等待发牌时下发状态
	int GetStationWhileWaitSendCard(int nDeskStation, bool bIsWatch, UINT uIndex);

	/// 游戏中时下发状态
	int GetStationWhileWaitPlaying(int nDeskStation, bool bIsWatch, UINT uIndex);

	/// 结算状态
	int GetStationWhileGameEnd(int nDeskStation, bool bIsWatch, UINT uIndex);


	//函数定义
public:
	//构造函数
	CGameDesk(); 

	//析构函数
	virtual ~CGameDesk();

	//重载函数
public:
	//-------------------------------------------------------------------------
	// 从IGameDesk重载
	//-------------------------------------------------------------------------
	/// 初始化桌子
	virtual int InitialDesk(unsigned int unRoomID, int nDeskNo, int nMaxPeople, IGameMainManager* pIMainManager, unsigned int unExchangeRate);

	// 内部做释放
	virtual void Release(){}

	//查询版本号
	virtual int GetVersion(char* pVersion);

	/// 获取游戏状态，并发送到客户端
	virtual int OnGetGameStation(int nDeskStation, UserInfoForGame_t& userInfo);

	/// 玩家重入
	virtual int UserReCome(int nDeskStation, UserInfoForGame_t& userInfo);

	/// 定时器时间到
	virtual int OnGameTimer(unsigned int unTimerID,void* pData);

	/// 玩家断线
	virtual int UserNetCut(int nDeskStation, UserInfoForGame_t& userInfo);

	/// 玩家入坐到某位置
	virtual int UserSitDesk(int nDeskStation, UserInfoForGame_t& userInfo);

	/// 玩家离桌
	virtual int UserLeftDesk(int nDeskStation, UserInfoForGame_t& userInfo);

	///更新玩家信息
	virtual int UpdateUserInfo(int nDeskStation, UserInfoForGame_t& userInfo);

	/// 玩家同意游戏
	virtual int UserAgreeGame(int nDeskStation, UserInfoForGame_t& userInfo);

	/// 游戏是否在进行中
	virtual bool IsPlayingByGameStation(void);

	/// 某桌号玩家是否在游戏中
	virtual bool IsPlayGame(int bDeskStation);

	// 玩家是否可以桌下(棋牌游戏中，某些游戏正在玩的时候是不可以坐下的)
	virtual bool CanSit2Desk();

	//是否为棋牌游戏
	virtual bool IsChessCard();

	/// 获取当前游戏状态
	virtual int GetCurGameStation(){return m_nGameStation;}

	///单人游戏用户游戏中强退
	virtual int ForceQuit() ;

	// 房间奖池变化通知
	virtual void AwardPoolChangedNotify(){}

	// 处理proxy消息，来自服务器
	virtual int HandleProxyMsg(unsigned int unMsgID,void * pMsgHead ,const char* pMsgParaBuf, const int nLen);

	// 处理客户端消息，来自Lotus
	virtual int HandleClientMsg(unsigned int unUin,int nDeskStation, void * pMsgHead ,const char* pMsgParaBuf, const int nLen);

	//获取房间号
	virtual int GetRoomID(){return m_nRoomID;}

	//更新桌子游戏实时算法数据信息
	//(usAlgorithmType, 表示算法类型，这个参考(GameDef.h)枚举: E_ALGORITHM_TYPE)
	//(usModifyFlag,表示算法没有修改，这个参考(GameDef.h)枚举: E_ALGORITHM_MODIFY_TYPE)
	virtual int UpdateDeskRealtimeData(void *pDeskRealTimeData, unsigned short usAlgorithmType, unsigned short usModifyFlag){ return 0; }

	//设置GM控制信息，usGmType参考(GameDef.h)枚举:E_GM_TYPE
	virtual int SetGameMangerInfo(void *pGameManagerData, unsigned short usGmType);

	//-------------------------------------------------------------------------
	//游戏开始
	virtual bool GameBegin(BYTE bBeginFlag);
	//游戏结束
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	//用来改变用户断线条件的函数
	virtual bool CanNetCut(BYTE bDeskStation);

	//重载纯虚函数
public:

	//重置游戏状态
	virtual bool ReSetGameState(BYTE bLastStation);

	//处理函数
private:

	// 设置定时器
	bool SetGameTimer(int nTimeID, unsigned int unMSDelay);

	// 删除定时器
	int  KillGameTimer(int nTimeID);

	//殺所有計時器
	void KillAllTimer();

	//获取下一个玩家位置
	BYTE GetNextDeskStation(BYTE bDeskStation);

	int LogMsg(const char* szBuf, int len);

	//桌子号
	int GetTableID() { return m_nDeskIndex; }

	// 发送客户端数据
	int SendGameData(BYTE bDeskStation, UINT nSubCmdID, const BYTE* pMsgBuf, const int nMsgParaLen);

	// 发送观察数据
	int SendWatchData(BYTE bDeskStation, UINT nSubCmdID, const BYTE* pMsgBuf, const int nMsgParaLen);

	// 发送桌子广播数据
	int SendDeskBroadCast(UINT nSubCmdID, const BYTE* pMsgBuf, const int nMsgParaLen);

	//踢出玩家
	int  DoUserLeftDeskOp(int nDeskStation,int iCode);

	//检测玩家钱是否够,如果不够，就踢出
	int  CheckUserMoneyLeftGame();

	//通知玩家断线
	void NotifyUserNetCut(int nDeskStation,bool bNetCut);

	//获取玩家
	PlayerInfo* GetPlayerInfo(int iSeatNo);

	//获取总人数
	int GetTotalPlayerCount();

	//获取总同意人数
	int GetTotalAgreeCount();

	//获取总在线人数
	int GetTotalOnlineCount();

	//重置人数
	int ResetPlayerCount(int iCount);

	//重置开房卡房间数据 
	int ResetCardRoomConfigInfo();

	//定时器
private:
	//倒计时定时器
	int OnDjsBaseTime();
	int OnProcDjsOnTime();

	//抓鸟到时间
	int OnTimeZhuaNiao();

	//游戏结束
	int OnTimeGameEnd();

	//设置倒计时
	void SetDjs(E_DJS_GAME_PLAYING  eGameDjsType,int tmGameDjsTimes,int iGameDjsStation);

	//获取倒计时剩余时间(秒)
	int  GetLeftDjsSec();

	//消息
private:
	//用户动作消息
	int OnMsgUserDoAction(int iDeskStation,CMD_C_USER_DO_ACTION* pMsg);

	//托管
	bool OnUserTuoGuan(WORD wChairID, byte IsTuoGuan);

	//出牌
	int OnMsgProcAction_OutCard(int iDeskStation,CMD_C_USER_DO_ACTION* pMsg);

	//吃牌
	int OnMsgProcAction_EatCard(int iDeskStation,CMD_C_USER_DO_ACTION* pMsg);

	//碰牌
	int OnMsgProcAction_PengCard(int iDeskStation,CMD_C_USER_DO_ACTION* pMsg);

	//杠--点杠
	int OnMsgProcAction_DianGang(int iDeskStation,CMD_C_USER_DO_ACTION* pMsg);

	//杠--补杠
	int OnMsgProcAction_BuGang(int iDeskStation,CMD_C_USER_DO_ACTION* pMsg);

	//杠--暗杠
	int OnMsgProcAction_AnGang(int iDeskStation,CMD_C_USER_DO_ACTION* pMsg);

	//胡--自摸
	int OnMsgProcAction_ZiMoHu(int iDeskStation,CMD_C_USER_DO_ACTION* pMsg);

	//胡--点炮胡
	int OnMsgProcAction_DianPaoHu(int iDeskStation,CMD_C_USER_DO_ACTION* pMsg);

	//过(放弃)
	int OnMsgProcAction_GiveUp(int iDeskStation,CMD_C_USER_DO_ACTION* pMsg);

	//抓鸟
	int OnMsgProcAction_ZhuaNiao();

	//结算
	int OnMsgProcAction_JieSuan(bool bLiuJu);

	//游戏逻辑
private:

	//删除麻将墩牌
	//int DeleteMJDCard(std::list<BYTE>& lsOutDeleteMj,int iCount=1,bool bFront=true);
	int DeleteMJDCard(bool bZhuaNiao, std::list<BYTE>& lsOutDeleteMj,int iCount/*=1*/,bool bFront/*=true*/);
	//投骰子
	void OnCastDice();

	//发牌
	void OnFaPai();

	//摸牌
	int  OnTouchCard(int iDeskStation,bool bFront);

	//是否是下一个位置
	bool IsNextStation(int iCurStation,int iSecondStation);

	//获取下一个位置
	int  GetNextStation(int iCurStation);

	//判断是否能进行弃牌
	bool IsCanGiveUp(DWORD dwEnableAction);

	//获取当前操作位置
	int  GetCurOperateStation()                      { return m_iCurOperateStation; }
	void SetCurOperateStation(int iCurOperateStation);

	E_CUR_SEAT_OP_STATUS GetCurSeatOpStatus()                     { return m_iCurSeatOpStatus;   }
	void SetCurSeatOpStatus(E_CUR_SEAT_OP_STATUS iCurSeatOpStatus){ m_iCurSeatOpStatus = iCurSeatOpStatus; }

	//桌面麻将箭头显示位置
	BYTE GetCurMjShowSeat()                          { return m_byCurMjShowSeat; }
	void SetCurMjShowSeat(BYTE byCurMjShowSeat)      { m_byCurMjShowSeat = byCurMjShowSeat;}

	//检测当前玩家能进行的操作
	bool CheckCurPlayerEnableOperates(bool bNeedCheckZiMoHu=true);

	//检测其他玩家是否能对出的牌进行操作
	bool CheckOutCardOtherEnableOperates(BYTE byOutMJCard,BYTE byOutMJSeatNo,bool bOnlyCheckDianHu,bool bCheckQiangGangHu);
	//检测并且执行等待的操作
	int  CheckDoWaitOperate(int iNotifyWatiOpStation=-1);

	//获取没有操作的最大权限相对值
	int  GetMaxNotOperateRightIndex(int& iOutMaxDeskStation,DWORD& dwOutMaxOneAction);

	//获取已经操作的最大权限相对值
	int  GetMaxHasOperateRightIndex(int& iOutMaxDeskStation,DWORD& dwOutMaxOneAction);

	//清除玩家低的权限
	int  ClearAllPlayerLowQuanXian(DWORD dwOneAction);

	//获取胡牌玩家数
	int  GetHuPlayerCount();

	//转到下一个玩家操作
	int  TurnToNextStation();

	//转到指定玩家
	int  TurnToAimStation(int iDeskStation,bool bNeedTouchCard,bool bTouchFront);

	//通知客户端操作成功
	int  NotifyClientOperatesSuccess(int iDeskStation,DWORD dwAction,const std::vector<BYTE>& vecCardList,int iNotifyStation=-1);

	//通知客户端操作失败
	int  NotifyClientOperatesFail(int iNotifyStation,int iCode);

	//通知调整玩家当前显示
	int  NotifyAdjustPlayerEnableAction(int iDeskStation,DWORD dwOneAction);

	//计算分数
	void CalcGameEndScore(CardRoomScoreStatement& CardRoomInfo);

	//设置下一个庄位置
	void SetNextNtStation(int iDeskStation);

	//设置赖子牌值
	BYTE GetCfgLaiziCard()                   { return m_byCfgLaiziCard; }
	void SetCfgLaiziCard(BYTE byCfgLaiziCard){ m_byCfgLaiziCard = byCfgLaiziCard; }

	//发送能胡的牌
	int  NotifyEnableHuCards(int iNotifyStation);

	////检查是否能杠
	bool GetIsEnableGang();
	
	//广播托管消息
	void BroadcastUserTuoGuanMsg();

	//玩家操作计时器
	int OnPlayerOpTimer(int nTimeID);

	//判断是否金币场
	bool IsGoldRoom();

	//因玩家未入座或未准备，解散房间
	void KickUserOutTable(int nDeskStation, int nReason);

	//玩家金币场游戏币不足时，重新计算
	int ReCalcGameEndScore(double lfPercent[]);
	//桌子配置
private:
	int                      m_iCfgZhuaNiaoCount;    //抓鸟个数配置
	bool                     m_bCfgCanDianPaoHu;     //能否点炮胡
	bool                     m_bCfgZhuangXianScore;  //是否是庄闲算分
	bool                     m_bCfgGuoHouBuGang;	 //是否是过后补杠
	bool                     m_bCfgXianPengHouGang;	 //是否是先碰后杠
	bool                     m_bCfgHuQiDui;          //是否支持七小队
	bool                     m_bCfgHongZhong;        //红中赖子
	BYTE                     m_byCfgLaiziCard;       //赖子牌值
	byte					 m_byAddNiaoCount;		 //无赖子胡牌加鸟数

	//游戏的一些变量
private:

	std::vector<PlayerInfo*> m_vecPlayer;      //玩家列表
	int					     m_iNtStation;     //庄家位置
	int                      m_dwNtUserID;     //庄家用户ID

	BYTE                     m_byAllCardList[MAX_MJ_TOTAL_COUTN]; //所有的牌
	int                      m_iTotalMjCardCount;                 //总麻将数
	int                      m_iLeftMjCount;   //剩余牌个数
	int                      m_iStartIndex;    //麻将墩起始位置
	int                      m_iEndIndex;      //麻将墩结束位置

	BYTE                     m_byStartSeat;    //开始发牌位置
	BYTE					 m_byStartDun;     //开始发牌墩


	BYTE                     m_byDicePoint[2]; //获取骰子数

	int                      m_iTotalRound;    //总局数
	int                      m_iCurRound;      //当前第几局

	int                      m_iCurOperateStation;    //当前操作位置
	E_CUR_SEAT_OP_STATUS     m_iCurSeatOpStatus;      //当前座位操作状态

	BYTE                     m_byLastOutMjSetaNo; //最后出牌的玩家
	BYTE					 m_byLastOutMj;       //最后出牌的麻将
	BYTE                     m_byCurMjShowSeat;   //当前桌面麻将箭头显示玩家,255表示没有箭头指向

	//倒计时
	E_DJS_GAME_PLAYING       m_eGameDjsType;      //倒计时类型
	time_t                   m_tmGameDjsStartMS;  //倒计时开始时间(毫秒)
	int                      m_tmGameDjsTimes;    //倒计时定时时间(秒)
	int                      m_iGameDjsStation;   //倒计时指向位置 , -1表示不指向

	int                      m_iNextNtStation;    //下一次庄的位置
	int						 m_nFinalMoPaiChair;  //最后一个摸牌玩家

	T_CardRoomConfig         m_CardRoomConfig;    //开房卡信息

	//int						 m_nSaveAutoHuChairID;//保存自动胡的玩家

	//结算相关数据 
	CMD_S_GameEnd            m_GameEndInfo;
	//对应的分数 用于 保存数据
	int                      m_arrSaveDetailCount[GAME_PLAYER][E_CARD_GAME_SCORE_TYPE_MAX];  //各个类型对应的个数,下标参考 E_CARD_GAME_SCORE_TYPE
	__int64                  m_arrSaveDetailScore[GAME_PLAYER][E_CARD_GAME_SCORE_TYPE_MAX];  //各个类型对应的分数,下标参考 E_CARD_GAME_SCORE_TYPE

	CHuTipsMJ				 m_playerHuTips;	  //胡牌提示算法类
	int						 m_nPlayMode;		  //玩法模式

	//金币场配置
private:
	int								m_nRoomCardPlaform; 
	INT64							m_llBaseMoney;
	INT64							m_llLessTakeMoney;
	int								m_nTax;

private:
	int								m_nTempCount;							//校准断线重连时间用
	int								m_nOperateTime;							//操作等待时间
	int								m_nReadyTime;							//准备时间
	int								m_nWaitForFullTime;						//配桌时等待玩家人弃，若不齐，解散房间

private:
	bool							m_bPlayerTuoGuan[GAME_PLAYER];			//托管
	int								m_nPlayerTimeOutCount[GAME_PLAYER];		//超时次数
	int								m_nTimeOutLimit;						//超时次数超过后自动变成托管

	bool							m_bPlayerTimerRunning[GAME_PLAYER];		//是否启动了玩家计时器
};

extern "C"
{
	IGameDesk*  CreateNewGameDesk();
}

#endif

/******************************************************************************************************/
