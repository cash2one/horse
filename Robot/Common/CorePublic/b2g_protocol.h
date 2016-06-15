#pragma once

#define MAKE_PROTO_VERSION(v1,v2,v3,v4)   ((v1<<24)|(v2<<16)|(v3<<8)|(v4))
#define ENCODER_VERSION	MAKE_PROTO_VERSION(0,0,0,3)
#define CHAR_NAME_LEN	24
#define PTID_LEN 24
#define MAX_ROOM_NAME	48
#define Appraise_Max    5
#define MAX_RIP_NOTICEBUF_SIZE	512
struct b2gProtoHeader
{
	WORD	length;	//协议数据长度
	BYTE	type;	//协议标识, b2gProtoType
	BYTE	ret;	//返回值, b2gProtoResult
};

enum b2gProtoResult
{
	b2gProtoResult_Success,			//操作成功
	b2gProtoResult_PackFormatErr,	//请求包格式错误
	b2gProtoResult_GameSvrDisconnected,	//游戏服务器断开连接了，请求失败
	b2gProtoResult_SendCmdError,	//发送请求命令出错
	b2gProtoResult_AudioVersion,	//音频服务器版本不匹配
	b2gProtoResult_EncoderVersion,	//解码协议版本不匹配
	b2gProtoResult_RoomNotExist,	//需要订阅的房间不存在
	b2gProtoResult_InvalidSingerIdx,//需要订阅的歌手索引不正确
	b2gProtoResult_PracticeRoom,	//不能订阅练歌房的音频
	b2gProtoResult_MemoryLack,		//内存不足，请求的操作失败
	b2gProtoResult_HasSubscribed,	//警告：此歌手的音频已经订阅过了
	b2gProtoResult_NotSubscribeRoom,//警告：没有订阅过此房间的音频
	b2gProtoResult_NotSubscribeSinger,//警告：没有订阅过此歌手的音频
};


struct SubscribePP
{
	UINT	nRoomID;
	BYTE	nReserved;
};

struct ssCharInfo
{
	UINT64  nDigitalID;
	UINT	nCharID;
	UINT	nClubFlagID;
	char	sCharName[CHAR_NAME_LEN];
	char	sPTID[PTID_LEN];
	BYTE	nSex;	
};

struct ssRoomInfo
{
	char	sRoomName[MAX_ROOM_NAME];
	UINT	nRoomID;
};

enum b2gRoomRegResult
{
	b2gRoomReg_Success,				//操作成功
	b2gRoomReg_RoomTypeError,		//房间类型错误
	b2gRoomReg_RoomDontExist,		//房间不存在
	b2gRoomReg_RoomRegAlready,		//房间不存在
};

struct ripRoomRegister
{
	UINT	nRoomID;
	char	sRoomName[MAX_ROOM_NAME];
	BYTE	Ret;
};

struct SSUserRipNoticeInfo
{
	UINT nPlyerID;
	char sNoticeMsg[MAX_RIP_NOTICEBUF_SIZE];
};

//歌手结算数据
struct ssSingSquare
{
	UINT64	SingerDigitalID;
	//***********************歌手结算*****************************//
	int		Score;							//歌唱分数
	int		ItemScore;						//道具得分
	int		Combo;							//获得的Combo数(有Combo的占总句数的多少)
	int		MaxCombo;						//最大Combo数
	BYTE	SquareType;						//结算类型
	BYTE	JudgeType;						//胜负判定
	BYTE	TotalAppraises;					//总体评价
	BYTE	Appraises[Appraise_Max];		//评价累计
	WORD	AverageGivedScore;				//平均给分
	float	VipGiveScorePercent;			//评委系数
	UINT	TotalScore;						//总得分
};

enum b2gProtocolType
{
	E2G_REQ_SUBSCRIBE_PP=0x10,	//直播服务器向游戏服务器 订阅房间中的音频流
	/*
		struct
		{
			UINT	audio_version;	//音频服务器协议版本号
			UINT	encoder_version;
			struct	SubscribePP;
		}
	*/

	G2E_ACK_SUBSCRIBE_PP,	//游戏服务器向直播服务器 返回订阅结果
	/*
		struct SubscribePP;
	*/

	E2G_REQ_UNSUBSCRIBE_PP=0x16,	//直播服务器向游戏服务器  取消订阅房间中的音频流
	/*
		struct	SubscribePP;
	*/
	G2E_ACK_UNSUBSCRIBE_PP,	//游戏服务器向直播服务器  返回取消订阅房间中的音频流
	/*
		struct	SubscribePP;
	*/

	B2GE_NOT_UPDATE_SERVER_LINK=0x18,	//直播服务器向游戏服务器，直播服务器向编码服务器  通知编码服务器断开，或者游戏服务器断开
	/*
		struct
		{
			BYTE	nServerType;	//enum SERVER_TYPE   gs <---90--- bs ---10---> es
			bool	bIsConnected;	//true=连接上了，false=断开了
		}
	*/


	G2E_NOT_START_SINGING=0x20,	//游戏服务器向直播服务器 通知玩家开始演唱
	/*
		struct
		{
			struct		SubscribePP;
			ssCharInfo	Singer;
			bool		bIsReplay;
			int			nSongID;
			BYTE		nSongNameLength;
			char*		sSongName;
		}
	*/

	G2E_NOT_FINISH_SINGING,	//游戏服务器向直播服务器 通知玩家结束演唱
	/*
		struct
		{
			struct	SubscribePP;
		}
	*/

	G2E_NOT_DELETE_ROOM,		//游戏服务器向直播服务器  通知房间被删除
	/*
		struct
		{
			UINT	nRoomID;
		}
	*/
	G2E_NOT_UPDATE_ROOMNAME=0x26,	//游戏服务器向直播服务器 通知更新房间名称
	/*
		struct
		{
			UINT	nRoomID;		
			BYTE	nRoomNameLength;
			char*	sRoomName;
		}
	*/
	G2E_NOT_UPDATE_EMCEE=0x28,	//游戏服务器向直播服务器 通知更新主持人信息
	/*
		struct
		{
			UINT		nRoomID;
			ssCharInfo	Emcee;
		}
	*/
	G2E_NOT_UPDATE_COMMENT=0x29,	//游戏服务器向直播服务器 通知更新麦序点评者信息
	/*
	struct
	{
		UINT		nRoomID;		
		ssCharInfo	Comment;
	}
	*/
	B2E_NOT_AUDIO_INFO=0x30,	//广播服务器向直播服务器 通知更新主持人信息
	/*
		struct
		{
			struct	SubscribePP;
			UINT	nPlayerID1;//reserved
			UINT	nPlayerID2;//reserved
			struct	AudioInfoDesc;
			BYTE	pAudioHead[];
			BYTE	pAudioData[];
			BYTE	pMusicData[];
		}
	*/
	G2E_NOT_START_RECORD,		//游戏服务器通知直播服务器 开始录制演唱文件
	/*
		struct	SubscribePP;
	*/

	G2E_NOT_CLOSE_RECORD,		//游戏服务器通知直播服务器 关闭正在录制的演唱文件
	/*
	struct
	{
		struct	SubscribePP;
		bool    bSaveRecord;//是否需要保持已经录制的文件
	}
	*/

	G2E_NOT_ROOM_CHAT,      //房间内聊天信息
	/*
	struct
	{
		struct		SubscribePP;
		ssCharInfo	ChatSender;
		WORD		nChatLength;
		char*		sChatMsg;
	}
	*/

	G2E_NOT_USE_ITEM,
	/*
	struct
	{
		struct		SubscribePP;
		ssCharInfo	User;
		ssCharInfo	Target;

		UINT		ItemTypeID;
		BYTE		nItemNameLength;
		char*		sItemName;
	}
	*/
	G2E_NOT_SING_SQUARE,
	/*
	struct
	{
		struct		SubscribePP;
		struct		ssSingSquare;
	}
	*/
	E2G_GET_ROOM_LIST=0x60,
	/*
		None
	*/
	G2E_ACK_ROOM_LIST,
	/*
		(multiple)
		struct ssRoomInfo
		{
			char	sRoomName[MAX_ROOM_NAME];
			UINT	nRoomID;
		};
		
	*/

	E2G_USER_NOTICE = 0x70,	//用户提示	
	/*
	(single)
	struct SSUserRipNoticeInfo
	{
	UINT nPlyerID;
	char sNoticeMsg[MAX_RIP_NOTICEBUF_SIZE];
	};
	*/
};