#pragma once

#define STONE_RECORD_ITEM_ID 44016
#pragma pack(push ,1)
typedef struct tagG2RMsgHeader
{
	WORD length;
	BYTE	type;	//协议标识
	BYTE	ret;	//返回值
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
	eG2RDecItemFailed,//扣取道具失败
	eG2RStoneNotFound,//未找到音乐宝石
};

enum G2RProtocol
{
	R2G_NOT_UPLOAD_STONE_RECORD_COMPLETE,//上传宝石录音完成
	G2R_ACK_UPLOAD_STONE_RECORD_COMPLETE,

	G2R_NOT_GAMESERVER,//通知玩家所在GS

	R2G_NOT_CONFIGURATION,//通知配置，IP和监听端口
};
#pragma pack(pop)