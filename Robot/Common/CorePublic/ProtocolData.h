/******************************************************************************
* 
* 文件名称：ProtocolData.h
* 摘    要：游戏服务器数据和协议
* 
* 当前版本：1.0
**********************************************************************************/
#pragma once

#include "msg_def.h"

#pragma pack(push ,1)
#define MAKE_VERSION(v1,v2,v3,v4)   ((v1<<24)|(v2<<16)|(v3<<8)|(v4))

#ifndef SIZEOF_STR
#define SIZEOF_STR(str)	sizeof(str)-1
#endif//SIZEOF_STR

#define GAME_VERSION	MAKE_VERSION(0,0,0,2)
#define STAT_VERSION	MAKE_VERSION(0,1,0,1)

#define MAX_SERVER_NAME		32

#define MAX_INVALID_NAME_COUNT  10

#define CHAR_NAME_LIMIT 16
#define MAX_IPADDR_LEN 16

#define MAX_PTID		24

#define MAX_LOGIN_DATA_LEN	64
#define MAX_CHAT_LEN			100

#define MAX_LEVEL					101							//最大等级
#define MAX_LUCK_LEVEL				10

#define LADDER_TOTAL_NUM			500
#define LADDER_TOP100_NUM			100

#define MAX_EXT_CHARAPROPERTY_LEN   512

#define MAX_ADMIN_B					2
#define MAX_ADMIN_C					3
#define MAX_ADMIN_D					4

#define MAX_LADDER_TIMES			4							//排行榜保留次数
#define	PLAYERPAGESIZE				10							//玩家页面大小

//角色ID
typedef UINT	CharaID;
//服务器ID
typedef UINT	ServerID;

enum SERVER_TYPE
{
	SVR_TYPE_CENTER=1,
	SVR_TYPE_LOGIN=2,
	//SVR_TYPE_SHOP=3,	

	SVR_TYPE_GAME=10,
	SVR_TYPE_CLUB=21,
	
	SVR_TYPE_MONITOR=50,
	SVR_TYPE_STATISTIC = 55,
};
//断线错误码
typedef enum EMSDisConnectCode
{
	eMSDisConnectMax,
}MSDISCONNECTCODE;

//断线描述
extern const char* g_pszDisconnectCodeDesc[];

inline LPCTSTR GetDisconnectCodeDesc(int Code)
{
	if(Code>=0&&Code<eMSDisConnectMax)
		return g_pszDisconnectCodeDesc[Code];
	else
	{
		static char Msg[64];
		_snprintf(Msg,SIZEOF_STR(Msg), "未知错误[%d]",Code);
		return Msg;
	}
}
	
//返回码描述
extern const char* g_pszRetCodeDesc[];

inline LPCTSTR GetMsgRetCodeDesc(int Code)
{
	if(Code>=0&&Code<eMSRetMax)
		return g_pszRetCodeDesc[Code];
	else
	{
		static char Msg[64];
		_snprintf(Msg, SIZEOF_STR(Msg),"未知错误[%d]",Code);
		return Msg;
	}
}

//服务器客户端交互的数据头
struct MSGHEADER
{
	WORD	wLength;											//整个包的长度,包括包头
	WORD	wCmd;												//命令
	WORD	wRetCode;											//返回状态
	int		nReserved;											//保留字段
};

typedef struct tagSSMsg
{
	MSGHEADER	Header;
	char		szData[1];
}SSMSG;

//////////////////////card struct//////////////////////////////////////
struct CARD_MAGIC_INFO
{
	WORD		wMagicID;		//技能ID
	BYTE		byLvl;//	
};
////
struct MONSTER_CARD_BASE_INFO{
	WORD		wCardID;				//卡牌ID
	DWORD		dwCurrExp;			//卡牌当前经验值
	BYTE		byJob;				//卡牌职业 0-力士 1-刺客 2-射手 3-术者 4-萨满 5-符箓
	BYTE		byRace;				//卡牌种族
	
	WORD		wLevel;				//等级	
	//
	WORD		wHP;				//HP
	WORD		wMaxHP;				//
	
	WORD		wDC;				//基础攻击力
	WORD		wDC1;				//基础攻击力
	//
	WORD		wAC;				//基础物防
	WORD		wAC1;				//基础物防

	WORD		wMAC;				//基础魔防
	WORD		wMAC1;				//基础魔防
	//
	BYTE		byColor;			//卡牌品质颜色
};

//保存数据库用
struct MONSTER_CARD_INFO:public MONSTER_CARD_BASE_INFO
{	
	DWORD		dwUniqueIdx;//自己背包卡牌	唯一索引
	bool		bIsLock;//卡牌是否锁定
	//tech
	CARD_MAGIC_INFO  stCardMagicInfo[4];//战斗技能
};

//内存数据

////////////////////////////////副本/////////////////////////////////////////////
// struct MAIN_DUPMAP_INFO{
// 	WORD wMainID;//关卡主id
// 	WORD wImageIdx;//底图索引
// };
// //
// struct SUB_DUPMAP_INFO{
// 	WORD wSubID;//关卡主id
// 	WORD wImageIdx;//底图索引
// };
typedef struct FinishDupmapInfo{//已经完成的副本信息
	WORD wSubID;//副本子id
	WORD wRepeat;//重复次数
}FINISH_DUPMAP_INFO,*PFINISH_DUPMAP_INFO;
//
//typedef vector<FINISH_DUPMAP_INFO>  VC_FINISH_DUPMAP_INFO;
////////////////////////////////////////////////////////////////////////////
//需要常驻内存的全局玩家的信息
typedef struct PlayerLineupInfo{//玩家布阵信息
	MONSTER_CARD_INFO stFaCardInfo[4];//玩家法器
	MONSTER_CARD_INFO stMonCardInfo[6];//玩家出阵妖怪卡
	MONSTER_CARD_INFO stHelpCardInfo[3];//玩家援助卡
}PLAYER_LINEUP_INFO,*PPLAYER_LINEUP_INFO;

typedef struct GLOBALE_PLAYER_DATA {
	WORD wEnergy;//玩家道义值
	time_t tmLastAddEnergyTime;//最后加道义值的时间
	PLAYER_LINEUP_INFO stPlayerLineupInfo;
}GLOBALE_PLAYER_DATA_CFG,*PGLOBALE_PLAYER_DATA_CFG;

typedef map<uint64,GLOBALE_PLAYER_DATA_CFG> MAP_GLOBALE_PLAYER_DATA_CFG;
typedef map<string,PGLOBALE_PLAYER_DATA_CFG> MAP_PGLOBALE_PLAYER_DATA_CFG;
//////////////////////////////////////////

typedef vector<GLOBALE_LADDER_INFO_CFG> VC_GLOBALE_LADDER_INFO_CFG;
typedef map<ULONG64,UINT> MAP_LADDER_OF_PLAYER;

inline bool AllScoreCompare(const GLOBALE_LADDER_INFO_CFG& AllScoreInfo1,const  GLOBALE_LADDER_INFO_CFG& AllScoreInfo2)
{
	return AllScoreInfo1.uAllScore > AllScoreInfo2.uAllScore;
}
////////////////////////////////

struct CHAR_CREATE_INFO
{
	char			szCharName[MAX_CHAR_NAME];					//角色名称		
	BYTE			bySex;										//性别(0:男; 1:女	
};

//好友信息
struct FRIEND_INFO
{	
	UINT	CharID;
	char	CharName[MAX_CHAR_NAME];	
	BYTE	bySex;
	bool	IsOnline;
	
	char             szGameServerName[MAX_SERVER_NAME];


	FRIEND_INFO()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct FRIEND_INFO_EX:public FRIEND_INFO
{		
	char GroupName[MAX_CHAR_NAME];	
	static bool Comp(FRIEND_INFO_EX& f1,FRIEND_INFO_EX& f2)
	{
		return strcmp(f1.CharName,f2.CharName)>0;
	}
	
	FRIEND_INFO_EX()
	{
		memset(GroupName,0, sizeof(GroupName));
	}
};

//
typedef struct tagCharacterExtPro2
{
	BYTE	byAdd;	//
	char	Reserved512[MAX_EXT_CHARAPROPERTY_LEN];
}CHARACTEREXTPRO,*PCHARACTEREXTPRO;

typedef vector<FRIEND_INFO_EX>	CFriendList;
typedef map<string,CFriendList > CFriendGroupMap;

struct BaseCharaInfo
{	
	char					szPTID[MAX_PTID];					//平台ID
	UINT64					iDigitalPTID;                       //数字ID
	char					szCharName[MAX_CHAR_NAME];			//角色名称

	unsigned int            score;								//当前积分
	unsigned int            allscore;							//累计积分
	unsigned int			point;								//点券
	unsigned int            exchpoint;		                    //弹珠兑换券
	unsigned int 			rank;								//积分排名	

	unsigned int            viptime;                            //用作连续登录天数 VIP剩余时间
	unsigned short			pkgcapacity;						//包裹最大容量

	unsigned int 			nReserv;							//保留
	unsigned int			createtime;							//创建时间
	//unsigned int			lastlogintime;						//最近登录时间

	BaseCharaInfo()
	{		
		//memset(this,0,sizeof(BaseCharaInfo));
	}
};

struct CHARACTER_INFO
	:public BaseCharaInfo
{
	unsigned int			clubid;					                        //工会ID
	short					title1;											//称号1
	short					title2;											//称号2		
	unsigned char			difficulty;										//难度等级
	unsigned char			shopdiscount;									//购物折扣,等等
	unsigned short			wReserv	;										//保留
	unsigned int			btnstatus;										//主界面button控制

	CHARACTER_INFO()
		:clubid(0)
	{
		memset(this,0,sizeof(CHARACTER_INFO));
	}
};

// 道具状态
enum ItemStatus
{
	item_status_none = 0x00,
	item_status_equipped,
};

enum ItemNum
{
	item_num_max
};

enum ItemID
{
	item_id_max
};
// 角色拥有的道具信息
struct PlayerItemInfo
{
	//unsigned short	index;						//在包裹中的位置
	unsigned int		instID;						//道具实例ID
	unsigned int 		id;							//道具ID
	unsigned short		num;						//道具数量
	unsigned short		slot;						//装备位置
	unsigned int		duration;					//消耗数值:剩余次数、剩余时间
	ItemStatus			status;						//道具状态
};

inline bool operator == (const PlayerItemInfo& plyItemInfo,unsigned int instID)
{
	return plyItemInfo.instID == instID;
}

inline bool operator == (const PlayerItemInfo& plyItemInfo,ItemStatus status)
{
	return plyItemInfo.status == status;
}

inline bool operator == (const PlayerItemInfo& plyItemInfo,ItemNum num)
{
	return plyItemInfo.num == 0;
}

inline bool operator == (const PlayerItemInfo& plyItemInfo,ItemID itemid)
{
	return plyItemInfo.id == itemid;
}

struct TPlayerData
{
	CHARACTER_INFO				CharInfo;	

	vector<PlayerItemInfo>	ItemList;						//用户道具表	
	map<WORD,vector<FINISH_DUPMAP_INFO> >  DupmapList;		//玩家已经完成的关卡对应的副本信息
	CFriendList					FriendList;					//好友信息	

	ULONG64						uCurRight;					//当前权力
	UINT						nPing;						//当前ping	
	WORD						nGoldLevel;					//
	uint32						uLoginTime;					//本次登陆时间
	WORD						wMaxPkgCount;				//包裹最大道具数量
	uint32						uLastItemIndex;				//卡牌道具内存索引

	CHARACTEREXTPRO            CharaPropertyExt;			//服务器内部使用的可扩展的数据

	TPlayerData()
	{				
		uCurRight=0;
		nPing=0xFFFFFFFF;
		uLoginTime = 0;
		wMaxPkgCount = 1;

		nGoldLevel = 0;
		uLastItemIndex = 1;
		//
		ZeroMemory(&CharaPropertyExt,sizeof(CHARACTEREXTPRO));
	}
	void operator=(const TPlayerData& PlayerData)
	{
		CharInfo=PlayerData.CharInfo;

		ItemList.assign(PlayerData.ItemList.begin(),PlayerData.ItemList.end());		
		FriendList.assign(PlayerData.FriendList.begin(),PlayerData.FriendList.end());

		uCurRight=PlayerData.uCurRight;
		nPing=PlayerData.nPing;

		nGoldLevel = PlayerData.nGoldLevel;
		wMaxPkgCount=PlayerData.wMaxPkgCount;
		uLoginTime=PlayerData.uLoginTime;
		uLastItemIndex=PlayerData.uLastItemIndex;
		memcpy(&CharaPropertyExt,&PlayerData.CharaPropertyExt,sizeof(CHARACTEREXTPRO));
	}
};
//服务器状态
typedef enum EServerStatus
{
	eServerNotAvailable	=	0,									//服务器不可用
	eServerIdle,												//服务器空闲
	eServerModerate,											//服务器适中
	//eServerCrowded,											//服务器拥挤
	eServerFull,												//服务器满员
}SERVERSTATUS;

typedef struct tagServerInfo
{
	unsigned int     dwServerID;
	unsigned short   wPlayerCount;
	unsigned char    byServerState;
	unsigned int     dwListenIP;
	unsigned short   wListenPort;
	unsigned char    byOptLen;
	unsigned char    abyOpt[255];
}SERVERINFO;

struct PAGE_INFO
{
	BYTE	Type;
	BYTE	Param;  //totalPagesNum
	BYTE	Page;
	BYTE	PageLen;
};

union SIMPLE_DATA_TIME
{
	struct  
	{
		WORD Year;
		BYTE Month;
		BYTE Day;
	};
	struct  
	{
		BYTE Reserve;
		BYTE Hour;
		BYTE Minute;
		BYTE Second;
	};
};

struct LADDER_START_TIME
{
	BYTE hour;       // [0,23]
	BYTE minute;	 // [0,59]
};


//游戏结束信息
struct GameOverInfo
{
	int		LeftTime;		//剩余时间，unit: millisecond； <0时表示进入第二次GameOver状态
	int		ItemEnergy;		//收到的能量
};

//道具商店相关

#define VIP_ITEM_FLAG		   0x00000001
#define ITEM_HOT_BUY_FLAG      0x00000002
#define ITEM_RECOMMEND_FLAG    0x00000004
#define NEW_ITEM_FLAG		   0x00000008
#define ONE_YUAN_FLAG		   0x00000010

enum ShoppingStatus 
{
	SHOPPING_INI			= 0,
	SHOPPING_WAITING		= 1,    //自身有交易在进行
	SHOPPING_REQBUYED       = 2,
};


struct STCommodity
{
    unsigned int m_iCommodityID;
    
    unsigned int m_iCommoditySubID;
    
    unsigned short m_iSaleDateLimit; //每日销量限制。0不受限制。 >0表示每日的销售量，从00:00~23:59
    unsigned char m_byDiscount; //道具折扣，0~100
    unsigned char m_byVipDiscount; //会员折扣,0~100
    
   //是否送虚拟币,及送虚拟币个数
    unsigned int m_iAddGameTicket;

    unsigned int m_unAttribute; //商品属性，第一位如果为1则表示是会员专属商品，一般玩家不能够买，为0表示一般商品，其他位有待进一步扩展如热卖等
								//VIP_ITEM_FLAG,ITEM_HOT_BUY_FLAG,ITEM_RECOMMEND_FLAG 位或
	short m_nValidDurationOne;
	unsigned int m_iPointPriceOne;
    unsigned int m_iGamePriceOne;

	short m_nValidDurationTwo;
	unsigned int m_iPointPriceTwo;
	unsigned int m_iGamePriceTwo;

	short m_nValidDurationThree;
	unsigned int m_iPointPriceThree;
	unsigned int m_iGamePriceThree;

	short m_nValidDurationFour;
	unsigned int m_iPointPriceFour;
	unsigned int m_iGamePriceFour;

	short m_nValidDurationFive;
	unsigned int m_iPointPriceFive;
	unsigned int m_iGamePriceFive;
	
	STCommodity()
	{
		memset( this, 0, sizeof(*this));
	}
};

struct RequestBuy
{
	unsigned int m_iCommodityID;   //购买的道具ID 
	unsigned int m_iCommoditySubID; //购买道具的子ID(为以后预留)
	unsigned int m_iNum;           //购买道具数量
	short		 m_nValidDuration;	//购买道具的有效期
	char		 m_cPayInType; //支付方式 1：点券 2：游戏虚礼币  0：默认值 无效
	
	RequestBuy()
	{
		memset( this, 0, sizeof( *this) );
	}
};

struct Commodity
{
	unsigned int m_iCommodityID; 
	unsigned int m_iCommoditySubID;
};

//系统通知类型
enum NoticeType
{
	Notice_Admin,		//系统管理员发出的消息
	Notice_Ladder,		//进入排行榜消息

	Notice_System,		//系统公告
};

//
struct tagServerList
{
	unsigned int     dwServerID; //服务器id 选组时check
	unsigned char    ServerName[32];//服务器名称 约15个汉字
};
//////////////////////////////////////////////////////////////////////////

#pragma pack(pop)