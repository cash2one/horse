/******************************************************************
* Copyright (c) 2006,上海盛大网络
* All rights reserved.
* 
* 文件名称：a2g_protocol.h
* 摘    要：音频服务器与游戏服务器间协议
* 
* 当前版本：1.0
* 作    者：sunwanxun
* 建立日期：2007-7-18
********************************************************************/

#pragma once

struct ProtoHeader
{
	WORD	length;	//协议数据长度
	byte	type;	//协议标识, ProtoType
	byte	ret;	//返回值, eProtoResult
};

enum eProtoResult
{
	eProtoResult_Success,			//操作成功
	eProtoResult_WarnSuc,			//操作虽成功，但不应该有此情况发生
	eProtoResult_TooSmallGroup,		//操作失败
	eProtoResult_HasGroupService,	//此音频服务器已经为此房间挂接过一个组
	eProtoResult_ConnectIsFull,		//连接的玩家已经到达上限
	eProtoResult_ServerIsLocked,	//服务器被锁定
	eProtoResult_GroupIsFull,		//音频组已满
	eProtoResult_ClientNotFound,	//玩家未找到
	eProtoResult_GroupNotFound,		//组未找到

	//UDP错误
	eProtoResult_IllegalAddress,	//非法的发送地址
	eProtoResult_IllegalToken,		//非法的Token
	eProtoResult_IllegalFormat,		//非法的音频包格式
	eProtoResult_UnauthAudio,		//未授权的音频数据类型
	eProtoResult_InvalidAudioType,	//无效音频类型
	eProtoResult_EmptyAudioPacket,	//玩家发送空音频数据包
};

enum ProtoType
{
	G2A_REQ_RESETSERVER = 0x00,	//请求重置音频服务器
	A2G_ACK_RESETSERVER,		//回复重置音频服务器
	
	G2A_REQ_RESETGROUP = 0x02,	//请求重置音频组
	//struct
	//{
	//	RoomID nRoomID;
	//};
	A2G_ACK_RESETGROUP,			//回复重置音频组
	//struct
	//{
	//	RoomID nRoomID;
	//};
	
	G2A_REQ_GETCONFIG = 0x04,	//请求音频服务器配置信息
	A2G_ACK_GETCONFIG,			//回复音频服务器配置信息
	//struct AudioSvrConfig
	
	G2A_REQ_CREATEGROUP = 0x06,	//请求创建音频组
	//struct
	//{
	//	RoomID nRoomID;
	//};
	A2G_ACK_CREATEGROUP,		//回复创建音频组
	//struct
	//{
	//	RoomID nRoomID;
	//};

	
	G2A_REQ_CLOSEGROUP = 0x08,	//请求关闭音频组
	//struct
	//{
	//	RoomID nRoomID;
	//};
	A2G_ACK_CLOSEGROUP,			//回复关闭音频组
	//struct
	//{
	//	RoomID nRoomID;
	//};
	
	G2A_REQ_ADDCLIENT = 0x0A,	//请求将玩家加入音频组
	//struct
	//{
	//	RoomID nRoomID;
	//	[AudioClient]
	//};
	A2G_ACK_ADDCLIENT,			//回复将玩家加入音频组
	//struct
	//{
	//	RoomID nRoomID;
	//	CharaID nCharaID;
	//};

	G2A_REQ_DELCLIENT = 0x0C,	//请求删除玩家
	//struct
	//{
	//	RoomID nRoomID;
	//	CharaID nCharaID
	//};
	A2G_ACK_DELCLIENT,			//回复删除玩家
	//struct
	//{
	//	RoomID nRoomID;
	//	CharaID nCharaID
	//};
	
	G2A_REQ_UPDATECLIENT = 0x0E,	//请求更新玩家状态
	//struct AudioClient;
	A2G_ACK_UPDATECLIENT,			//回复更新玩家状态
	//struct
	//{
	//	RoomID nRoomID;
	//	CharaID nCharaID
	//};

	A2G_REQ_SENDCLIENTAUDIO = 0x20,	//请求游戏服务器转发玩家音频数据
	//struct
	//{
	//	RoomID	  nRoomID;
	//	MSGHEADER msg;
	//	AudioData data;
	//};
	G2A_ACK_SENDCLIENTAUDIO,	//回复游戏服务器转发玩家音频数据
	//struct
	//{
	//	RoomID	nRoomID;
	//	CharaID	nCharaID;
	//};

	G2A_REQ_SENDSERVERAUDIO = 0x30,	//请求音频服务器转发其他音频服务器发来的音频数据
	//struct
	//{
	//	RoomID	  nRoomID;
	//	MSGHEADER msg;
	//	AudioData data;
	//};
	A2G_ACK_SENDSERVERAUDIO,		//回复音频服务器转发其他音频服务器发来的音频数据
	//struct
	//{
	//	RoomID	nRoomID;
	//	CharaID	nCharaID;
	//};

	A2G_REP_CURSTATUS = 0x40,		//报告音频服务器状态
	//struct
	//{
	//	UINT nHoldenClients;
	//};
	A2G_REP_KEEPALIVE,				//报告音频服务器心跳
	A2G_REP_GROUPLIST,				//报告音频服务器上创建的音频组列表
	A2G_REP_ILLEGALAUDIO,			//收到玩家发送的非法音频数据
	//struct
	//{
	//	CharaID		nCharaID;
	//	RoomID		nRoomID;
	//	UINT		nSendIP;
	//	WORD		nSendPort;
	//};

	A2G_REP_DELCLIENT,				//报告删除了某个音频组中的玩家 -- 当游戏服务器与音频服务器之间的状态完全同步时，不会发出
	//struct
	//{
	//	short nCount;
	//	struct
	//	{
	//		RoomID nRoomID;
	//		CharaID nCharaID;
	//	}[nCount];
	//};

	G2A_NOT_CLEARBUFFER,			//通知印谱服务器清除音频数据缓冲
	//struct
	//{
	//	RoomID nRoomID;
	//};
	G2A_NOT_SETREPLAYER,			//请求设置播放麦序的原始演唱者
	//struct
	//{
	//	RoomID	nRoomID;
	//	short	nCount;
	//	CharaID	nReplayedSingerID[ nCount ];
	//};
};
