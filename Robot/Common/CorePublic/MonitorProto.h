#pragma once
#pragma pack(push ,1)

struct MoHeader
{
	WORD	length;	//协议数据长度
	byte	type;	//协议标识, eMoMessageType
	byte	ret;	//返回值, eMoProtoResult
};

struct MOPKG_CONFIG_ACK
{
	char			sListenIP[20];
	WORD			nListenPort;
	unsigned int	nMaxPlayers;
};

struct MOPKG_UPDATE_ACK
{
	UINT			nCurPlayers;	//当前玩家数量
	float			fCycleTime;		//响应时间
	float			fTcpRecvFlow;	//接受的tcp流量
	float			fTcpSendFlow;	//发送的tcp流量
	float			fUdpRecvFlow;	//接受的udp流量
	float			fUdpSendFlow;	//发送的udp流量
};

//监控服务返回的错误码
enum eMoProtoResult
{
	mo_ret_sucess,	//操作成功
};

//监控服务器的协议类型
enum eMoMessageType
{
	MO_MESSAGE_START,

	MOID_REQ_CONFIG_INFO= 10,	//请求对方服务器的配置信息， MoHeader
	MOID_ACK_CONFIG_INFO= 11,	//返回对方服务器的配置信息， MoHeader + MOPKG_CONFIG_ACK
	MOID_REQ_UPDATE_DATA= 12,	//请求对方服务器的动态信息， MoHeader
	MOID_ACK_UPDATE_DATA= 13,	//返回对方服务器的动态信息， MoHeader + MOPKG_UPDATE_ACK

	MO_MESSAGE_END,
};

#pragma pack(pop)
