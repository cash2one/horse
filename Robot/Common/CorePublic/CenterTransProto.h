#pragma once
/*
#pragma pack(push ,1)
#define CHAR_NAME_LEN	24
#define PTID_LEN		24
#define CHAR_INTRODUCE_LEN	48
#define TRANS_ID_LEN	33
#define ROOM_NAME_LEN	48

struct CmmCharOnline
{
	UINT			nCharID;
	BYTE			nRoomType;
	UINT			nRoomID;
	char			sRoomName[64];
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

	WORD			nSingLevel;
	WORD			nSingExp;
	WORD			nCommentLevel;
	WORD			nCommentExp;

	UINT			nCrowded;
	UINT			nCommunityUserID;
	char			sIntroduce[CHAR_INTRODUCE_LEN];

	BYTE			nVIP;
	UINT			nClubID;
	UINT			nClubFlagID;
	short			nTitle1;
	short			nTitle2;
	UINT			nSingCount;											//演唱次数
	UINT			nPKCount;											//PK次数
	UINT			nPKWinCount;										//PK获胜次数	
	UINT			nPKGuessCount;										//参加PK竞猜次数
	UINT			nPKGuessWinCount;									//参加PK竞猜获胜次数
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
*/