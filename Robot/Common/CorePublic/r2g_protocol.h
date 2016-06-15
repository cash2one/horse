#pragma once

#define STONE_RECORD_ITEM_ID 44016
#pragma pack(push ,1)
typedef struct tagG2RMsgHeader
{
	WORD length;
	BYTE	type;	//Э���ʶ
	BYTE	ret;	//����ֵ
	int			nReserved;
}G2RMsgHeader;

typedef struct tagNotStoneRecordInfo
{
	UINT	nStoneID;
	UINT	nRecordID;
	uint32	EndTime;
	uint32	StoreTime;
}NotStoneRecordInfo;

enum G2RErrorCode
{
	eG2RSucceed = 0,
	eG2RDecItemFailed,//��ȡ����ʧ��
	eG2RStoneNotFound,//δ�ҵ����ֱ�ʯ
};

enum G2RProtocol
{
	R2G_NOT_UPLOAD_STONE_RECORD_COMPLETE,//�ϴ���ʯ¼�����
	G2R_ACK_UPLOAD_STONE_RECORD_COMPLETE,

	G2R_NOT_GAMESERVER,//֪ͨ�������GS

	R2G_NOT_CONFIGURATION,//֪ͨ���ã�IP�ͼ����˿�
};
#pragma pack(pop)