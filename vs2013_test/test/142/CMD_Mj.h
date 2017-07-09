#ifndef CMD_SHOWHAND_HEAD_FILE
#define CMD_SHOWHAND_HEAD_FILE

#include <windows.h>

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//公共宏定义
#define GAME_NAME						"长沙红中"      //游戏名字
#define GAME_ID						    142             //游戏ID
#define GAME_PLAYER						4				//游戏人数

//版本定义
#define GAME_MAX_VER					1				//现在最高版本
#define GAME_LESS_VER					1				//现在最低版本
#define GAME_CHANGE_VER					0				//修改版本


//////////////////////////////////////////////////////////////////////////

#define	HAND_MJ_MAX						14              //手牌数最大值
#define OUTCARD_COUNT_MAX               30              //出牌最大值
#define MAX_ZHUANIAO_COUNT              10              //最大抓鸟数
#define MAX_MJ_TOTAL_COUTN              112             //麻将数最多

#define SCORE_MAX_COUNT                 20              //分数最大项


//游戏状态定义
#define GS_WAIT_SETGAME					0				//等待东家设置状态   GameStation_2
#define GS_WAIT_ARGEE					1				//等待同意设置       GameStation_2
#define GS_NT_TOUZEZI					20				//庄家掷骰子状态     GameStation_3
#define GS_SEND_CARD					21				//发牌状态           GameStation_4
#define GS_PLAY_GAME					22				//游戏中状态         GameStation_5
//#define GS_GAME_JIESUAN					23				//结算状态           GameStation_6
#define GS_WAIT_NEXT				    24				//等待下一盘开始     GameStation_6

#define MAX_TALK_LEN			        500				///最大聊天数据长度
#define GAME_NICK_LEN                   20              ///游戏昵称长度

#define  ASS_MAIN_GAME                  180  			///< 游戏专用消息主ID

#define  ASS_GM_AGREE_GAME				1			    ///< 同意游戏

///用户同意结构
struct MSG_GR_R_UserAgree
{
	BYTE								bDeskNO;		///游戏桌号
	BYTE								bDeskStation;	///位置号码
	BYTE								bAgreeGame;		///同意标志
};


#define ASS_GM_GAME_STATION				2		///游戏状态
/********************************************************************************/
//配置扩展枚举
enum E_LS_CONFIG_TYPE
{
	ECFGTYPE_LAIZICARD = 0, //赖子牌值
};

//胡牌主类型              v2.1
enum E_HU_MAIN_SERVER_CARD_TYPE
{
	EHU_MAIN_TYPE_NO  = 0              ,    //无
	EHU_MAIN_TYPE_PINGHU       = 0x0001,    //平胡
	EHU_MAIN_TYPE_PENPENHU     = 0x0002,    //碰碰胡
	EHU_MAIN_TYPE_QINYISE      = 0x0004,    //清一色
	EHU_MAIN_TYPE_QIXIAODUI    = 0x0008,    //七小对
	EHU_MAIN_TYPE_HAOQIDUI     = 0x0010,    //豪七对
	EHU_MAIN_TYPE_SHISANYAO    = 0x0020,    //十三幺

	EHU_MAIN_TYPE_QIDADUI      = 0x0040,    //七大对    v2.5
	EHU_MAIN_TYPE_CHAOHAOQIDUI = 0x0080,    //超豪七对  v2.5

	EHU_MAIN_TYPE_MAX
};

//游戏状态数据包	（ 等待东家设置状态 ）
struct GameStation_1
{
	BYTE				bStation;						//游戏状态
	BYTE				iVersion;						//游戏版本号
	BYTE				iVersion2;						//游戏版本号

	//配置信息  
	int                 iLsConfig[10];                  //配置列表，预留几个 ,看考E_LS_CONFIG_TYPE             

	//游戏信息
	BYTE				byBankerSeatNo;				    //庄家座位号
	int                 iTotalRound;                    //总游戏局数
	int                 iLeftRound;                     //剩余多少局
};

//游戏状态数据包	（ 等待同意设置 ）
struct GameStation_2
{
	BYTE				bStation;						//游戏状态
	BYTE				iVersion;						//游戏版本号
	BYTE				iVersion2;						//游戏版本号

	//配置信息
	int                 iLsConfig[10];                  //配置列表，预留几个 ,看考E_LS_CONFIG_TYPE   

	//游戏信息
	BYTE				byBankerSeatNo;				    //庄家座位号
	int                 iTotalRound;                    //总游戏局数
	int                 iLeftRound;                     //剩余多少局

	bool                bIsAgree[GAME_PLAYER];          //同意标记
};

//游戏状态数据包	（ 庄家掷骰子状态 ）
struct GameStation_3
{
	BYTE				bStation;						//游戏状态
	BYTE				iVersion;						//游戏版本号
	BYTE				iVersion2;						//游戏版本号

	//配置信息
	int                 iLsConfig[10];                  //配置列表，预留几个 ,看考E_LS_CONFIG_TYPE   

	//游戏信息
	BYTE				byBankerSeatNo;				    //庄家座位号
	int                 iTotalRound;                    //总游戏局数
	int                 iLeftRound;                     //剩余多少局

	BYTE                bySatrtIndex;                   //麻将墩起始位置
	BYTE                byEndIndex;                     //麻将墩结束位置
	int                 iLeftMjCount;                   //剩余麻将数
};

//游戏状态数据包	（ 发牌状态 ）
struct GameStation_4
{
	BYTE				bStation;						//游戏状态
	BYTE				iVersion;						//游戏版本号
	BYTE				iVersion2;						//游戏版本号

	//配置信息
	int                 iLsConfig[10];                  //配置列表，预留几个 ,看考E_LS_CONFIG_TYPE   

	//游戏信息
	BYTE				byBankerSeatNo;				    //庄家座位号
	int                 iTotalRound;                    //总游戏局数
	int                 iLeftRound;                     //剩余多少局

	BYTE                byDicePoint[2];                 //骰子点数
	BYTE                bySatrtIndex;                   //麻将墩起始位置
	BYTE                byEndIndex;                     //麻将墩结束位置
	int                 iLeftMjCount;                   //剩余麻将数
};

//游戏状态数据包	（ 游戏中状态 ）
struct GameStation_5
{	
	BYTE				bStation;						//游戏状态
	BYTE				iVersion;						//游戏版本号
	BYTE				iVersion2;						//游戏版本号

	//配置信息
	int                 iLsConfig[10];                  //配置列表，预留几个 ,看考E_LS_CONFIG_TYPE   

	//游戏信息
	BYTE				byBankerSeatNo;				    //庄家座位号
	int                 iTotalRound;                    //总游戏局数
	int                 iLeftRound;                     //剩余多少局

	BYTE                byDicePoint[2];                 //骰子点数
	BYTE                bySatrtIndex;                   //麻将墩起始位置
	BYTE                byEndIndex;                     //麻将墩结束位置
	int                 iLeftMjCount;                   //剩余麻将数

	//倒计时
	BYTE                bySeatNo;                       //定时器指向座位号
	int                 iDjsTimes;                      //定时器倒计时多少

	//牌
	BYTE                byPengCardList[GAME_PLAYER][4];    //碰牌列表
	BYTE                byMingGangCardList[GAME_PLAYER][4];//明杠牌列表
	BYTE                byAnGangCardList[GAME_PLAYER][4];  //暗杠牌列表
	BYTE                byChiList[GAME_PLAYER][4][3];      //吃牌列表

	BYTE                byOtherSeatMjCount[GAME_PLAYER];   //其他玩家手上牌数
	BYTE                byMyHandMjs[HAND_MJ_MAX];          //自己手上麻将

	BYTE                byOutCardList[MAX_MJ_TOTAL_COUTN]; //玩家出的牌
	BYTE                byCurMjShowSeat;                   //当前桌面麻将箭头显示玩家,255表示没有箭头指向
	BYTE                byCurOperateStation;               //当前出牌位置
	BYTE                byCurOpStatus;                     //当前出牌状态,0正在出牌中，1出牌完成，等待其他玩家操作(出的牌其他人吃碰杠)
};

#define ASS_GM_NORMAL_TALK				4									///普通聊天
///聊天结构 
struct MSG_GR_RS_NormalTalk
{
	COLORREF							crColor;							///字体颜色
	SHORT								iLength;							///信息长度
	LONG								dwSendID;							///用户 ID
	LONG								dwTargetID;							///目标 ID
	int									nDefaultIndex;						///=0，输入的内容，>0，选择的内容
	char								szMessage[MAX_TALK_LEN+1];			///聊天内容
};


#define ASS_NOTIFY_USER_NET_CUT  		5									///通知断线
///通知断线 
struct MSG_NOTIFY_USER_NET_CUT
{
	BYTE				 byNetCutSeatNo;				 //断线座位号
	bool                 bIsNetCut;                      //是否断线
};


//游戏开始
#define SUB_S_GAME_START				0x201									//游戏开始
struct CMD_S_GameStart
{
	BYTE				 byBankerSeatNo;				 //庄家座位号
	int                  iTotalRound;                    //总游戏局数
	int                  iLeftRound;                     //剩余多少局

	BYTE                 byDicePoint[2];                 //骰子点数
	BYTE                 byStartSeat;                    //从哪家开始抓牌
	BYTE                 byStartPos;                     //从哪墩开始抓牌
	int                  iLeftMjCount;                   //剩余麻将数
};

////投骰子
//#define SUB_S_GAME_START				0x202									//投骰子
//struct CMD_S_CASE_DICE
//{
//	BYTE byDicePoint[2];//骰子点数
//	BYTE byDiceStep;    //第几次掷骰子
//	BYTE byBankPos;     //庄家的位置
//	BYTE byStartSeat;  //从哪家开始抓牌
//	BYTE byStartPos;   //从哪墩开始抓牌
//};

//发送麻将
#define SUB_S_SEND_MJ					0x203							    //发牌
struct CMD_S_SendMj
{
	BYTE				  byMjs[HAND_MJ_MAX];				     //所发的麻将牌
	BYTE				  byMjCount;							 //麻将数目
	BYTE				  byBankerSeatNo;				         //庄家座位号

	BYTE                  bySatrtIndex;                          //麻将墩起始位置
	BYTE                  byEndIndex;                            //麻将墩结束位置
	int                   iLeftMjCount;                          //剩余麻将数
};

//用户摸牌
//服务器->客户端 广播包
#define SUB_S_TOUCH_MJ					0x204							    //用户摸牌
struct CMD_S_TouchMj
{
	BYTE				   byTouchMj;							   //摸牌麻将 其他玩家这个值为0
	BYTE				   byCurUserSeatNo;   					   //当前玩家位置
	BYTE                   bySatrtIndex;                           //麻将墩起始位置
	BYTE                   byEndIndex;                             //麻将墩结束位置
	int                    iLeftMjCount;                           //剩余麻将数
};

//玩家操作类型
enum E_ACTION_TYPE
{
	EACTION_OUTCARD   = 0x0001, //出牌
	EACTION_CHI_LEFT  = 0x0002, //吃牌--左吃(@**)
	EACTION_CHI_MID   = 0x0004, //吃牌--中吃(*@*)
	EACTION_CHI_RIGHT = 0x0008, //吃牌--右吃(**@)
	EACTION_PENG      = 0x0010, //碰牌
	EACTION_GANG_DIAN = 0x0020, //杠--点杠
	EACTION_GANG_BU   = 0x0040, //杠--补杠
	EACTION_GANG_AN   = 0x0080, //杠--暗杠
	EACTION_HU_ZIMO   = 0x0100, //胡--自摸
	EACTION_HU_DIAN   = 0x0200, //胡--点炮胡
	EACTION_GIVEUP    = 0x0400, //过(放弃)
};

//胡牌额外类型 天地胡     v2.1
enum E_HU_CARD_EXTENDS_TYPE  
{
	EHUEXTENDS_TYPE_NO   = 0           ,  //无
	EHUEXTENDS_TYPE_QIDUI	   = 0x0001,  //七对
	EHUEXTENDS_TYPE_GANG_HUA   = 0x0002,  //杠上花
	EHUEXTENDS_TYPE_SIHONGZHONG= 0x0004,  //四红中
	EHUEXTENDS_TYPE_GANG_PAO   = 0x0008,  //杠上炮
	EHUEXTENDS_TYPE_QIANGGANGHU= 0x0020,  //抢杠胡
	EHUEXTENDS_TYPE_TIAN       = 0x0040,  //天胡
	EHUEXTENDS_TYPE_DI         = 0x0080,  //地胡
	EHUEXTENDS_MEN_QING        = 0x0100,  //是否门清
};

//牌列表类型
enum E_CARD_LIST_TYPE
{
	ECARDLISTTYPE_CHI  = 0 , //吃
	ECARDLISTTYPE_PENG ,     //碰
	ECARDLISTTYPE_GANG_DIAN ,//杠--点杠
	ECARDLISTTYPE_GANG_BU ,  //杠--补杠
	ECARDLISTTYPE_GANG_AN ,  //杠--暗杠
};

//操作对应的牌列表
struct stOpCardList
{
	BYTE                 byCardltType;    //牌列表类型   参考E_CARD_LIST_TYPE
	BYTE	             byCard[4];       //能操作的动作对应的牌列表 , 如吃(2个一组)、暗杠、补杠等
};

//服务器通知玩家操作
//服务端->客户端 
#define SUB_S_NOTIFY_USER_OPERATE      0x205   				//通知玩家进行操作
struct CMD_S_NOTIFY_USER_OPERATE
{
	DWORD                dwEnableAction;  //能操作的动作  参考E_ACTION_TYPE 

	BYTE                 byOutMjSeatNo;   //目标座位号 , 即吃、碰、点杠、点炮胡 谁的牌
	BYTE                 byOutMj;         //目标座位出的麻将

	BYTE	             byCardNum;       //能操作的动作对应的牌数量
	stOpCardList         lsCardInfo[1];   //牌列表
};

//玩家进行操作
//客户端->服务端 
#define SUB_C_USER_DO_ACTION	        0x106   			//玩家进行操作
struct CMD_C_USER_DO_ACTION
{
	DWORD   dwAction;        //动作  参考E_ACTION_TYPE 

	BYTE	byCardNum;       //动作对应的牌数量
	BYTE	byCard[4];       //动作对应的牌列表 ,例如 出的牌、吃的牌、杠的牌、碰的牌
};

//玩家托管
//客户端->服务端 
#define SUB_C_USER_TUOGUAN	        0x108   			//托管
struct CMD_C_USER_TUOGUAN
{
	byte							byIsTuoGuan;							//是否托管
};

//玩家操作失败
//服务端->客户端 
#define SUB_S_USER_ACTION_R_FAIL	    0x206   			//玩家操作失败 返回
struct CMD_S_USER_ACTION_R_FAIL
{
	int iCode;            //错误码
};
//错误码:
#define ERR_ACTION_CODE_WAIT_OTHER_OPERATE      1           //等待其他玩家操作

//玩家操作成功,广播玩家操作动作结果
//服务端->客户端 广播包
#define SUB_S_USER_ACTION_R_SUCC	    0x207   			//玩家操作成功 返回
struct CMD_S_USER_ACTION_R_SUCC
{
	BYTE	bySeatNo;        //动作座位号
	DWORD   dwAction;        //动作  参考E_ACTION_TYPE 

	BYTE    byOutMjSetaNo;   //目标座位号 , 即吃、碰、点杠、点炮胡 谁的牌
	BYTE    byOutMj;         //目标座位出的麻将

	BYTE    bySatrtIndex;    //麻将墩起始位置
	BYTE    byEndIndex;      //麻将墩结束位置
	int     iLeftMjCount;    //剩余麻将数

	BYTE	byCardNum;       //动作对应的牌数量
	BYTE	byCard[4];       //动作对应的牌列表
};


//////////////////////////////////////////////////////////////////////////
//游戏结束
#define SUB_S_GAME_END           	    0x209   			//游戏结束

//计算分数类型
enum E_CALC_SCORE_TYPE
{
	ECSCORE_HU_ZIMO = 0 , //自摸胡
	ECSCORE_DIAN_PAO    , //点炮
	ECSCORE_JIE_PAO     , //接炮
	ECSCORE_DIAN_GANG   , //点杠
	ECSCORE_JIE_GANG    , //接杠
	ECSCORE_AN_GANG     , //暗杠
	ECSCORE_BU_GANG     , //补杠
	ECSCORE_ZHONG_NIAO  , //中鸟
	ECSCORE_BEI_QGANG   , //被抢杠
	ECSCORE_QGANG_HU    , //抢杠胡
};

struct stZhuaNiaoCard
{
	BYTE    byNiaoMj;              //抓鸟牌
	BYTE    byIsZhongNiao;         //是否中鸟  1是,0否
	BYTE    bySeatNo;              //中鸟位置
};

//一个输赢分数信息
struct stOneUserEndInfo
{
	int    iCurTotalPan;         //本局总潘数
	INT64  iCurTotalMoney;       //本局总输赢分数
	INT64  iCurGangMoney;        //本局杠多少分数    v2.2
	INT64  iCurHuMoney;          //本局胡多少分数    v2.2
	INT64  iCurNiaoMoney;        //本局中鸟多少分数  v2.2
	INT64  iTotalMoney;          //当前多少豆
	int    iTax;                 //扣税

	BYTE   byPengCardList[4];    //碰牌列表
	BYTE   byMingGangCardList[4];//明杠牌列表
	BYTE   byAnGangCardList[4];  //暗杠牌列表
	BYTE   byChiList[4][3];      //吃牌列表
	BYTE   byHandMjs[HAND_MJ_MAX];  //手上麻将 ,胡牌玩家胡的牌 是最后一张牌

	char   szUserNick[GAME_NICK_LEN];//用户昵称

	BYTE   byLsDetailCount[SCORE_MAX_COUNT]; //各个类型对应的个数,下标参考 E_CALC_SCORE_TYPE
	
	DWORD  dwHuExtendsType;      //胡牌额外类型--天地胡, 参考E_HU_CARD_EXTENDS_TYPE   v2.1
}; 

//一个输赢分数信息
struct stOneUserEndInfo_double
{
	double lfCurTotalMoney;       //本局总输赢分数
	double lfCurGangMoney;        //本局杠多少分数    v2.2
	double lfCurHuMoney;          //本局胡多少分数    v2.2
	double lfCurNiaoMoney;        //本局中鸟多少分数  v2.2
};

struct CMD_S_GameEnd
{
	BYTE                            byIsLiuJu;                //是否流局 1是,0否
	BYTE				            byBankerSeatNo;			  //庄家座位号
	BYTE                            byZhuaNiaoSeatNo;         //抓鸟座位号
	stOneUserEndInfo                userDetail[GAME_PLAYER];

	BYTE                            byZhuaNiaoCount;                //抓鸟麻将数
	stZhuaNiaoCard                  lsZhuaNiao[MAX_ZHUANIAO_COUNT]; //抓鸟牌列表 
};
//////////////////////////////////////////////////////////////////////////

//定时器
//服务端->客户端
#define SUB_S_NOTIFY_TIME	            0x210   		 	//定时器
struct CMD_S_NOTIFY_TIME
{
	BYTE                            bySeatNo;               //定时器指向座位号
	int                             iDjsTimes;              //定时器倒计时多少
};

//通知能胡的牌列表  v2.1
//服务端->客户端
#define SUB_S_NOTIFY_CAN_HU_CARDS       0x211   		 	//通知能胡的牌
struct CMD_S_NOTIFY_CAN_HU_CARDS
{
	BYTE                            byCanHuCards[28];       //能胡的牌列表
};

//其他玩家能对听牌杠后2张牌做出的操作
//服务器->客户端 广播包
#define SUB_S_NOTIFY_USER_TUOGUAN			0x217									//托管广播
struct CMD_S_NOTIFY_USER_TUOGUAN
{
	byte				byUserTuoGuan[GAME_PLAYER];			//
};

//游戏状态数据包	（ 结算状态  ）
struct GameStation_6
{
	BYTE				bStation;						//游戏状态
	BYTE				iVersion;						//游戏版本号
	BYTE				iVersion2;						//游戏版本号

	//配置信息  
	int                 iLsConfig[10];                  //配置列表，预留几个 ,看考E_LS_CONFIG_TYPE 

	//游戏信息
	BYTE				byBankerSeatNo;				    //庄家座位号
	int                 iTotalRound;                    //总游戏局数
	int                 iLeftRound;                     //剩余多少局

	bool                bIsAgree[GAME_PLAYER];          //同意标记

	CMD_S_GameEnd       sGameEndInfo;                   //结算信息
};

#pragma pack(1)

#endif