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

	UINT			nCommunityUserID;									//����ID,PHP��
	
	ULONG64			nIdoleDigitalID;									//ż�������ID

	BYTE			nVIP;
	UINT			nClubID;
	//UINT			nClubFlagID;
	short			nTitle1;
	short			nTitle2;
	UINT			nSingCount;											//�ݳ�����
	UINT			nPKCount;											//PK����
	UINT			nPKWinCount;										//PK��ʤ����	
	UINT			nPKGuessCount;										//�μ�PK���´���
	UINT			nPKGuessWinCount;									//�μ�PK���»�ʤ����
};

//key-value�ṹ
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
	WORD			wCurCount;						//��ǰ����
	WORD			wMaleCount;						//��������
	DWORD			dwCrowded;						//��������
};

//��ط�������Э������
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
