#pragma once

#include "communityproto.h"

#pragma pack(push ,1)

struct CmmCharOnline
{
	UINT			nCharID;
	BYTE			nRoomType;
	UINT			nRoomID;
	char			sRoomName[ROOM_NAME_LEN];

	UINT			nCurServerID;
	char            sCurServerName[SERVER_NAME_LEN];
};

struct CmmCharaInfo
{
	UINT			nArea;
	ULONG64			nDigitalID;
	char			sPTID[PTID_LEN];

	UINT			nCharaID;
	char			sCharaName[CHAR_NAME_LEN];
};

struct CmmDetailChar : public CmmCharaInfo
{
	BYTE			nSex;
	BYTE			nIsOnline;
	CmmCharOnline	OnlineDetail;

	WORD			nSingLevel;
	WORD			nSingExp;
	WORD			nCommentLevel;
	WORD			nCommentExp;

	UINT			nCrowded;
	char			sIntroduce[CHAR_INTRODUCE_LEN];

	UINT			nCommunityUserID;									//社区ID,PHP用
	
	ULONG64			nIdoleDigitalID;									//偶像的数字ID

	BYTE			nVIP;
	UINT			nClubID;
	//UINT			nClubFlagID;
	short			nTitle1;
	short			nTitle2;
	UINT			nSingCount;											//演唱次数
	UINT			nPKCount;											//PK次数
	UINT			nPKWinCount;										//PK获胜次数	
	UINT			nPKGuessCount;										//参加PK竞猜次数
	UINT			nPKGuessWinCount;									//参加PK竞猜获胜次数
};

//key-value结构
typedef struct kv_token_s {
	char* key;
	char* value;
	size_t nValueBytes;
} kv_token;


struct dbconnstr
{
	string name;
	string connstr;
};

struct CmmRoomInfo
{
	UINT			nRoomID;
	char			sRoomName[ROOM_NAME_LEN];
	WORD			wCurCount;						//当前人数
	WORD			wMaleCount;						//男性人数
	DWORD			dwCrowded;						//房间人气
};

//监控服务器的协议类型
enum eCenterTransferType
{
	CENTER_TRANS_MESSAGE_START,

	CENTER_TRANS_NOTIFY_COMMUNITYID,
	CENTER_TRANS_NOTIFY_ONLINE_INFO,
	CENTER_TRANS_NOTIFY_OFFLINE,

	CENTER_TRANS_REQGS_CHARDETAIL,
	CENTER_TRANS_ACKCM_CHARDETAIL,

	CENTER_TRANS_REQGS_ROOMINFO,
	CENTER_TRANS_ACKCM_ROOMINFO,

	CENTER_TRANS_MESSAGE_END,
};

#pragma pack(pop)
