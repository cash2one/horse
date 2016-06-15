/******************************************************************************
* 
* �ļ����ƣ�ProtocolData.h
* ժ    Ҫ����Ϸ���������ݺ�Э��
* 
* ��ǰ�汾��1.0
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

#define MAX_LEVEL					101							//���ȼ�
#define MAX_LUCK_LEVEL				10

#define LADDER_TOTAL_NUM			500
#define LADDER_TOP100_NUM			100

#define MAX_EXT_CHARAPROPERTY_LEN   512

#define MAX_ADMIN_B					2
#define MAX_ADMIN_C					3
#define MAX_ADMIN_D					4

#define MAX_LADDER_TIMES			4							//���а�������
#define	PLAYERPAGESIZE				10							//���ҳ���С

//��ɫID
typedef UINT	CharaID;
//������ID
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
//���ߴ�����
typedef enum EMSDisConnectCode
{
	eMSDisConnectMax,
}MSDISCONNECTCODE;

//��������
extern const char* g_pszDisconnectCodeDesc[];

inline LPCTSTR GetDisconnectCodeDesc(int Code)
{
	if(Code>=0&&Code<eMSDisConnectMax)
		return g_pszDisconnectCodeDesc[Code];
	else
	{
		static char Msg[64];
		_snprintf(Msg,SIZEOF_STR(Msg), "δ֪����[%d]",Code);
		return Msg;
	}
}
	
//����������
extern const char* g_pszRetCodeDesc[];

inline LPCTSTR GetMsgRetCodeDesc(int Code)
{
	if(Code>=0&&Code<eMSRetMax)
		return g_pszRetCodeDesc[Code];
	else
	{
		static char Msg[64];
		_snprintf(Msg, SIZEOF_STR(Msg),"δ֪����[%d]",Code);
		return Msg;
	}
}

//�������ͻ��˽���������ͷ
struct MSGHEADER
{
	WORD	wLength;											//�������ĳ���,������ͷ
	WORD	wCmd;												//����
	WORD	wRetCode;											//����״̬
	int		nReserved;											//�����ֶ�
};

typedef struct tagSSMsg
{
	MSGHEADER	Header;
	char		szData[1];
}SSMSG;

//////////////////////card struct//////////////////////////////////////
struct CARD_MAGIC_INFO
{
	WORD		wMagicID;		//����ID
	BYTE		byLvl;//	
};
////
struct MONSTER_CARD_BASE_INFO{
	WORD		wCardID;				//����ID
	DWORD		dwCurrExp;			//���Ƶ�ǰ����ֵ
	BYTE		byJob;				//����ְҵ 0-��ʿ 1-�̿� 2-���� 3-���� 4-���� 5-����
	BYTE		byRace;				//��������
	
	WORD		wLevel;				//�ȼ�	
	//
	WORD		wHP;				//HP
	WORD		wMaxHP;				//
	
	WORD		wDC;				//����������
	WORD		wDC1;				//����������
	//
	WORD		wAC;				//�������
	WORD		wAC1;				//�������

	WORD		wMAC;				//����ħ��
	WORD		wMAC1;				//����ħ��
	//
	BYTE		byColor;			//����Ʒ����ɫ
};

//�������ݿ���
struct MONSTER_CARD_INFO:public MONSTER_CARD_BASE_INFO
{	
	DWORD		dwUniqueIdx;//�Լ���������	Ψһ����
	bool		bIsLock;//�����Ƿ�����
	//tech
	CARD_MAGIC_INFO  stCardMagicInfo[4];//ս������
};

//�ڴ�����

////////////////////////////////����/////////////////////////////////////////////
// struct MAIN_DUPMAP_INFO{
// 	WORD wMainID;//�ؿ���id
// 	WORD wImageIdx;//��ͼ����
// };
// //
// struct SUB_DUPMAP_INFO{
// 	WORD wSubID;//�ؿ���id
// 	WORD wImageIdx;//��ͼ����
// };
typedef struct FinishDupmapInfo{//�Ѿ���ɵĸ�����Ϣ
	WORD wSubID;//������id
	WORD wRepeat;//�ظ�����
}FINISH_DUPMAP_INFO,*PFINISH_DUPMAP_INFO;
//
//typedef vector<FINISH_DUPMAP_INFO>  VC_FINISH_DUPMAP_INFO;
////////////////////////////////////////////////////////////////////////////
//��Ҫ��פ�ڴ��ȫ����ҵ���Ϣ
typedef struct PlayerLineupInfo{//��Ҳ�����Ϣ
	MONSTER_CARD_INFO stFaCardInfo[4];//��ҷ���
	MONSTER_CARD_INFO stMonCardInfo[6];//��ҳ������ֿ�
	MONSTER_CARD_INFO stHelpCardInfo[3];//���Ԯ����
}PLAYER_LINEUP_INFO,*PPLAYER_LINEUP_INFO;

typedef struct GLOBALE_PLAYER_DATA {
	WORD wEnergy;//��ҵ���ֵ
	time_t tmLastAddEnergyTime;//���ӵ���ֵ��ʱ��
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
	char			szCharName[MAX_CHAR_NAME];					//��ɫ����		
	BYTE			bySex;										//�Ա�(0:��; 1:Ů	
};

//������Ϣ
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
	char					szPTID[MAX_PTID];					//ƽ̨ID
	UINT64					iDigitalPTID;                       //����ID
	char					szCharName[MAX_CHAR_NAME];			//��ɫ����

	unsigned int            score;								//��ǰ����
	unsigned int            allscore;							//�ۼƻ���
	unsigned int			point;								//��ȯ
	unsigned int            exchpoint;		                    //����һ�ȯ
	unsigned int 			rank;								//��������	

	unsigned int            viptime;                            //����������¼���� VIPʣ��ʱ��
	unsigned short			pkgcapacity;						//�����������

	unsigned int 			nReserv;							//����
	unsigned int			createtime;							//����ʱ��
	//unsigned int			lastlogintime;						//�����¼ʱ��

	BaseCharaInfo()
	{		
		//memset(this,0,sizeof(BaseCharaInfo));
	}
};

struct CHARACTER_INFO
	:public BaseCharaInfo
{
	unsigned int			clubid;					                        //����ID
	short					title1;											//�ƺ�1
	short					title2;											//�ƺ�2		
	unsigned char			difficulty;										//�Ѷȵȼ�
	unsigned char			shopdiscount;									//�����ۿ�,�ȵ�
	unsigned short			wReserv	;										//����
	unsigned int			btnstatus;										//������button����

	CHARACTER_INFO()
		:clubid(0)
	{
		memset(this,0,sizeof(CHARACTER_INFO));
	}
};

// ����״̬
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
// ��ɫӵ�еĵ�����Ϣ
struct PlayerItemInfo
{
	//unsigned short	index;						//�ڰ����е�λ��
	unsigned int		instID;						//����ʵ��ID
	unsigned int 		id;							//����ID
	unsigned short		num;						//��������
	unsigned short		slot;						//װ��λ��
	unsigned int		duration;					//������ֵ:ʣ�������ʣ��ʱ��
	ItemStatus			status;						//����״̬
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

	vector<PlayerItemInfo>	ItemList;						//�û����߱�	
	map<WORD,vector<FINISH_DUPMAP_INFO> >  DupmapList;		//����Ѿ���ɵĹؿ���Ӧ�ĸ�����Ϣ
	CFriendList					FriendList;					//������Ϣ	

	ULONG64						uCurRight;					//��ǰȨ��
	UINT						nPing;						//��ǰping	
	WORD						nGoldLevel;					//
	uint32						uLoginTime;					//���ε�½ʱ��
	WORD						wMaxPkgCount;				//��������������
	uint32						uLastItemIndex;				//���Ƶ����ڴ�����

	CHARACTEREXTPRO            CharaPropertyExt;			//�������ڲ�ʹ�õĿ���չ������

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
//������״̬
typedef enum EServerStatus
{
	eServerNotAvailable	=	0,									//������������
	eServerIdle,												//����������
	eServerModerate,											//����������
	//eServerCrowded,											//������ӵ��
	eServerFull,												//��������Ա
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


//��Ϸ������Ϣ
struct GameOverInfo
{
	int		LeftTime;		//ʣ��ʱ�䣬unit: millisecond�� <0ʱ��ʾ����ڶ���GameOver״̬
	int		ItemEnergy;		//�յ�������
};

//�����̵����

#define VIP_ITEM_FLAG		   0x00000001
#define ITEM_HOT_BUY_FLAG      0x00000002
#define ITEM_RECOMMEND_FLAG    0x00000004
#define NEW_ITEM_FLAG		   0x00000008
#define ONE_YUAN_FLAG		   0x00000010

enum ShoppingStatus 
{
	SHOPPING_INI			= 0,
	SHOPPING_WAITING		= 1,    //�����н����ڽ���
	SHOPPING_REQBUYED       = 2,
};


struct STCommodity
{
    unsigned int m_iCommodityID;
    
    unsigned int m_iCommoditySubID;
    
    unsigned short m_iSaleDateLimit; //ÿ���������ơ�0�������ơ� >0��ʾÿ�յ�����������00:00~23:59
    unsigned char m_byDiscount; //�����ۿۣ�0~100
    unsigned char m_byVipDiscount; //��Ա�ۿ�,0~100
    
   //�Ƿ��������,��������Ҹ���
    unsigned int m_iAddGameTicket;

    unsigned int m_unAttribute; //��Ʒ���ԣ���һλ���Ϊ1���ʾ�ǻ�Աר����Ʒ��һ����Ҳ��ܹ���Ϊ0��ʾһ����Ʒ������λ�д���һ����չ��������
								//VIP_ITEM_FLAG,ITEM_HOT_BUY_FLAG,ITEM_RECOMMEND_FLAG λ��
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
	unsigned int m_iCommodityID;   //����ĵ���ID 
	unsigned int m_iCommoditySubID; //������ߵ���ID(Ϊ�Ժ�Ԥ��)
	unsigned int m_iNum;           //�����������
	short		 m_nValidDuration;	//������ߵ���Ч��
	char		 m_cPayInType; //֧����ʽ 1����ȯ 2����Ϸ�����  0��Ĭ��ֵ ��Ч
	
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

//ϵͳ֪ͨ����
enum NoticeType
{
	Notice_Admin,		//ϵͳ����Ա��������Ϣ
	Notice_Ladder,		//�������а���Ϣ

	Notice_System,		//ϵͳ����
};

//
struct tagServerList
{
	unsigned int     dwServerID; //������id ѡ��ʱcheck
	unsigned char    ServerName[32];//���������� Լ15������
};
//////////////////////////////////////////////////////////////////////////

#pragma pack(pop)