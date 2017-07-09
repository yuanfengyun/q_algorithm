#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义
#pragma pack(1)

#define GAME_RULE_SUIJIZHUANG		1//随机庄
#define GAME_RULE_QIANGZHUANG		2//抢庄
#define GAME_RULE_NOT_WAITE			3
#define GAME_RULE_FANGZHU_ZHUANG	4//房主庄
#define GAME_RULE_NO_ADD_SCORE		5
#define GAME_RULE_NO_ZHUANG			6//无庄
#define GAME_RULE_LUNLIU_ZHUANG     7//轮流庄

#define GAME_RULE_DIBEI				8//1分起步
#define	GAME_RULE_GAOBEI			9//10分起步

#define GAME_RULE_10LUN				10
#define GAME_RULE_20LUN				11

#define GAME_RULE_30LIM				12//30封顶
#define GAME_RULE_50LIM				13//50封顶
#define GAME_RULE_80LIM				14//80封顶
#define GAME_RULE_100LIM			15//100封顶

#define GAME_RULE_1DIZHU			16//底注1分
#define GAME_RULE_3DIZHU			17
#define GAME_RULE_5DIZHU			18

#define GAME_RULE_3BEIDI			19//三倍底
#define GAME_RULE_6PLAYER			20//6人房间

#define KIND_ID							20									//游戏 I D
#define GAME_PLAYER						8									//游戏人数
#define GAME_NAME						TEXT("炸金花")						//游戏名字
#define MAX_COUNT						3									//最大数目
#define MAX_TURN_NUM					8									//最大轮数

#define VERSION_SERVER					PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT					PROCESS_VERSION(6,0,3)				//程序版本

//结束原因
#define GER_NO_PLAYER					0x10								//没有玩家

//游戏状态
#define GS_TK_FREE						GAME_STATUS_FREE					//等待开始
#define GS_TK_BANKER					GAME_STATUS_PLAY					//叫庄状态
#define GS_TK_SCORE						GAME_STATUS_PLAY+1					//下注状态
#define GS_TK_PLAYING					GAME_STATUS_PLAY+2					//游戏进行


#define  CALL_BANKER_SUIJI    1
#define  CALL_BANKER_QIANGZHUANG
#define CALL_BANKER_FANGZHU

#define SERVER_LEN						32 

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START				100									//游戏开始
#define SUB_S_ADD_SCORE					101									//加注结果
#define SUB_S_PLAYER_EXIT				102									//用户强退
#define SUB_S_SEND_CARD					103									//发牌消息
#define SUB_S_GAME_END					104									//游戏结束
#define SUB_S_OPEN_CARD					105									//用户摊牌
#define SUB_S_CALL_BANKER				106									//用户叫庄
#define SUB_S_ALL_CARD					107									//发牌消息
#define SUB_S_AMDIN_COMMAND				108									//系统控制


#define SUB_S_GUODI						109									//锅底消息
#define SUB_S_TOTLE_SCORE				110									//每个玩家下注总分数，通知客户端
#define SUB_S_KANPAI					111									//看牌
#define SUB_S_QIPAI						112									//弃牌
#define	SUB_S_BAI						113									//败

#define SUB_S_GET_SCORE					114									//获取玩家分数


#define SUB_S_CALL_BANKER_NOTICE		200									//叫庄通知
#define SUB_S_ADD_SCORE_NOTICE			201									//加注通知


#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#define myatoi      atoi
#define myatoi64    _atoi64
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#define myatoi      _wtoi
#define myatoi64	_wtoi64
#endif

struct CMD_S_GuoDi
{
	LONGLONG						lGuodi;	//桌面底分数
	LONGLONG						lGeng;	//跟的底分数

	CMD_S_GuoDi()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct CMD_S_SCORE
{
	WORD								wUserCharID;
	LONGLONG							lScore;								//当前玩家下的注数
	LONGLONG							lCurScore;							//当前暗注分数
	WORD								wCurrentUserID;						//新的当前玩家
	bool								bNeedSound;							//是否需要声音。比牌的时候用到
	WORD								wTurn;								//轮
	CMD_S_SCORE()
	{
		bNeedSound = true;
	}
};

struct CMD_S_KanPai
{
	WORD								wUserCharID;
};

struct CMD_S_QiPai
{
	WORD								wUserCharID;
	WORD								wCurrentUserID;
};

struct CMD_S_BiPai
{
	WORD								wDoUserID;				//发起比牌人索引
	WORD								wCompUserID;			//被比牌人索引
	WORD								wLoseUserID;			//输家索引
};

//每个用户下注总数
struct CMS_S_Totle_Score
{
	LONGLONG						lTotle[GAME_PLAYER];
	LONGLONG						lLeft[GAME_PLAYER];			//剩余多少分能下注
	WORD							nLeftTurn[GAME_PLAYER];		//剩余轮数
};

//游戏状态
struct CMD_S_StatusFree
{
	LONGLONG							lCellScore;							//基础积分

	//历史积分
	LONGLONG							lTurnScore[GAME_PLAYER];			//积分信息
	LONGLONG							lCollectScore[GAME_PLAYER];			//积分信息
	TCHAR								szGameRoomName[SERVER_LEN];			//房间名称
};

//游戏状态
struct CMD_S_StatusCall
{
	WORD								wCallBanker;						//叫庄用户
	BYTE                                cbDynamicJoin;                      //动态加入 
	BYTE                                cbPlayStatus[GAME_PLAYER];          //用户状态
	BYTE								bCallStatus[GAME_PLAYER];						//叫庄状态

	//扑克信息
	BYTE								cbHandCardData[GAME_PLAYER][MAX_COUNT];//桌面扑克
	//历史积分
	LONGLONG							lTurnScore[GAME_PLAYER];			//积分信息
	LONGLONG							lCollectScore[GAME_PLAYER];			//积分信息
};

//游戏状态
struct CMD_S_StatusScore
{
	//下注信息
	BYTE								cbPlayStatus[GAME_PLAYER];          //用户状态
	BYTE								cbDynamicJoin;                      //动态加入
	LONGLONG							lTableScore[GAME_PLAYER];			//下注数目
	WORD								wBankerUser;						//庄家用户

	//扑克信息
	BYTE								cbHandCardData[GAME_PLAYER][MAX_COUNT];//桌面扑克
	//历史积分
	LONGLONG							lTurnScore[GAME_PLAYER];			//积分信息
	LONGLONG							lCollectScore[GAME_PLAYER];		//积分信息
	TCHAR								szGameRoomName[SERVER_LEN];		//房间名称
};

//游戏状态
struct CMD_S_StatusPlay
{
	//状态信息
	BYTE                                cbPlayStatus[GAME_PLAYER];          //用户状态(是否有人)
	WORD								wUserStatus[GAME_PLAYER];			//暗注/看牌/弃牌/输牌
	LONGLONG							lScore[GAME_PLAYER];				//下注数目
	LONGLONG							lLeft[GAME_PLAYER];					//每个玩家离限注还有多少筹码可以下
	WORD								wBankerUser;						//庄家用户
	WORD								wCurrentUser;						//当前轮到的玩家
	LONGLONG							lCurScore;							//当前所叫分数

	WORD								lUserChouMa[GAME_PLAYER][16];
	WORD								wXiaZhuCount[GAME_PLAYER];

	//扑克信息
	BYTE								cbHandCardData[GAME_PLAYER][MAX_COUNT];//手牌扑克
};

//用户叫庄
struct CMD_S_CallBanker
{
	WORD								wCallChairID;						//叫庄用户
	BYTE								cbCallDouble;						//倍数
};

//游戏开始
struct CMD_S_GameStart
{
	//下注信息
	WORD								wBankerUser;						//庄家用户
	WORD								wCurrentUser;
//	bool								bQiangPlayer[GAME_PLAYER];			//抢庄玩家
	WORD								wCurrentScore;							//当前注大小
};

//用户下注
struct CMD_S_AddScore
{
	WORD								wAddScoreUser;						//加注用户
	LONGLONG							lAddScoreCount;						//加注数目
	bool								bAn;								//是否为暗注
};

//游戏结束
struct CMD_S_GameEnd
{
	LONGLONG							lGameTax[GAME_PLAYER];				//游戏税收
	LONGLONG							lGameScore[GAME_PLAYER];			//游戏得分
	BYTE								cbCardData[GAME_PLAYER][MAX_COUNT];	//用户扑克
	bool								bBiPaiUser[GAME_PLAYER][GAME_PLAYER];//互相比牌玩家
};

//发牌数据包
struct CMD_S_SendCard
{
	BYTE								cbCardData[GAME_PLAYER][MAX_COUNT];	//用户扑克
};

//发牌数据包
struct CMD_S_AllCard
{
	bool								bAICount[GAME_PLAYER];
	BYTE								cbPlayStatus[GAME_PLAYER];			//游戏状态
	BYTE								cbCardData[GAME_PLAYER][MAX_COUNT];	//用户扑克
	bool                            bIsQiang;                         //是否是抢庄
};

//用户退出
struct CMD_S_PlayerExit
{
	WORD								wPlayerID;							//退出用户
};

//用户摊牌
struct CMD_S_Open_Card
{
	WORD								wPlayerID;							//摊牌用户
	BYTE								bOpen;								//摊牌标志
	//BYTE								bType;								//用户牌的类型
	//BYTE								bPoint;								//用户牌的点数
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构
#define SUB_C_CALL_BANKER				1									//用户叫庄
#define SUB_C_ADD_SCORE					2									//用户加注
#define SUB_C_OPEN_CARD					3									//用户摊牌
#define SUB_C_COMP_CARD					4									//用户比牌

#define	SUB_C_GEN_SCORE					6									//跟住
#define SUB_C_JIA_SCORE					7									//加注
#define	SUB_C_KANPAI					8									//看牌
#define	SUB_C_QIPAI						9									//弃牌
#define	SUB_C_BIPAI						10									//比牌

#define SUB_C_HAVE_SCORE				11									//玩家分数

struct CMD_C_HaveScore
{
	LONGLONG lHaveScore[GAME_PLAYER];
};

struct CMD_C_GuoDi
{
	WORD								wUserCharID;
	LONGLONG							lGuodiScore;
};

struct CMD_C_Gen
{
	WORD								wUserCharID;
	LONGLONG							lScore;
};

struct CMD_C_JiaZhu
{
	WORD								wUserCharID;
	LONGLONG							lScore;
	bool								bAn;								//是否为暗注
};

struct CMD_C_KanPai
{
	WORD								wUserCharID;
};

struct CMD_C_QiPai
{
	WORD								wUserCharID;
};

struct CMD_C_BiPai
{
	WORD								wUserCharID;
	WORD								toCompUserChairID;
	LONGLONG							lScore;
};


//用户叫庄
struct CMD_C_CallBanker
{
	BYTE								bBanker;							//做庄标志
};

//终端类型
struct CMD_C_SPECIAL_CLIENT_REPORT        
{
	WORD                                wUserChairID;                       //用户方位
};

//用户加注
struct CMD_C_AddScore
{
	bool								bAn;								//是否为暗注
	LONGLONG							lScore;								//加注数目
};

//用户摊牌
struct CMD_C_OxCard
{
	BYTE								bOX;								//牛牛标志
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//控制定义
#define SUB_C_AMDIN_COMMAND			5									//管理员命令

#define RQ_OPTION_CANCLE		1		//取消
#define RQ_OPTION_QUERYING		2		//查询
#define RQ_OPTION_SETING		3		//设置

#define CHEAT_TYPE_LOST		0		//输
#define CHEAT_TYPE_WIN		1		//赢

struct CMD_C_AdminReq
{
	BYTE cbReqType;							//请求类型
	BYTE cbCheatCount;						//控制次数
	BYTE cbCheatType;						//控制类型
	DWORD dwGameID;							//玩家标识
};

#pragma pack()
#endif
