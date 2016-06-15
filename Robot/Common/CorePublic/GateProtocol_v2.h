#pragma once

struct GATE_MSG_HEADER
{
	WORD	wLength;											//整个包的长度,包括包头
	WORD	wCmd;												//命令
};

struct GATE_MSG
{
	GATE_MSG_HEADER Header;
	char			Data[1];
};


union	MSG_SEND_PARAM
{
	ULONG64	qwParam;
	DWORD	dwParam[2];
	WORD	wParam[4];
	BYTE	byParam[8];
};

struct CLIENT_SEND_INFO
{
	UINT			TransID;
	UINT			Channel;
	MSG_SEND_PARAM	SendParam;
};

struct GATE_SERVER_INFO
{
	DWORD	ServerID;
	DWORD	ListenIP;
	DWORD	TCPListenPort;	
	DWORD	UDPListenPort;
	DWORD	MaxClient;
};

enum AUDIO_ANNOUNCER
{
	AUDIO_ANNOUNCER_NONE,
	AUDIO_ANNOUNCER_FULL,
	AUDIO_ANNOUNCER_SLIENT,
};

struct GATE_CLIENT_INFO
{
	UINT	TransID;	
	ULONG64	AccountID;
	UINT	CharID;
	DWORD	SourceIP;
	WORD	SourceTCPPort;
	WORD	SourceUDPPort;
};

struct GATE_CLIENT_LEAVE_INFO
{
	GATE_CLIENT_INFO	ClientInfo;
	BYTE				Reason;
};

struct BROADCAST_MEMBER_INFO
{
	DWORD GateID;
	UINT TransID;
	UINT CharID;
};
enum BROADCAST_CHANNEL
{
	BCN_ROOM_SING,
	BCN_ROOM_EMCEE,
	BCN_MAX,
};


enum GATE_SS_MSGS
{
	GMSG_SS_GATE_SERVER_INFO,
	//GATE_SERVER_INFO ServerInfo
	GMSG_SS_LOAD_INFO,
	//DWORD 当前负载
	
	GMSG_SS_ACCEPT_CLIENT,
	//通知Gate接受客户端
	//GATE_CLIENT_INFO AcceptInfo	
	GMSG_SS_CLIENT_ENTER,
	//客户端进入服务器
	//GATE_CLIENT_INFO ClientInfo;
	GMSG_SS_CLIENT_LEAVE,
	//客户端进入服务器
	//GATE_CLIENT_LEAVE_INFO LeaveInfo;
	GMSG_SS_SET_BROADCAST_ANNOUNCER,
	//设置客户端的广播权限
	//struct  
	//{
	//	UINT Channel;
	//	UINT TransID;
	//	BYTE Announcer;
	//};
	GMSG_SS_SET_BROADCAST_ACCEPT,
	//设置客户端广播接收对象
	//struct  
	//{
	//	UINT Channel;
	//	UINT TransID;
	//	UINT AcceptCharID;
	//};
	GMSG_SS_ADD_BROADCAST_MEMBER,
	//添加广播成员
	//struct  
	//{
	//	UINT Channel;
	//	WORD TargetCount;
	//	UINT TargetTransID[TargetCount];
	//	WORD MemberCount;
	//	BROADCAST_MEMBER_INFO MemberInfo[MemberCount];
	//};

	GMSG_SS_DEL_BROADCAST_MEMBER,
	//删除广播成员
	//struct  
	//{
	//	UINT Channel;
	//	WORD TargetCount;
	//	UINT TargetTransID[TargetCount];
	//	BROADCAST_MEMBER_INFO MemberInfo[MemberCount];
	//};
	GMSG_SS_DEL_ALL_BROADCAST_MEMBER,
	//删除所有广播成员
	//struct  
	//{
	//	UINT Channel;	
	//	UINT TargetTransID;
	//};
	
	GMSG_SS_UDP_MSG_BROADCAST,
	//请求群发消息报
	//struct  
	//{
	//	WORD				ClientCount;				//客户端数量
	//	CLIENT_SEND_INFO	TransID[ClientCount];		//客户端TransID
	//	char				Data[];						//要转发的数据
	//};
	GMSG_SS_TCP_MSG_PACKET,
	//客户端消息包
	//struct  
	//{		
	//	UINT TransID;									//客户端TransID
	//	char Data[];									//要转发的数据
	//};
	GMSG_SS_CLIENT_PING,
	//通知某个客户端的PING
	//struct
	//{
	//	UINT TransID;
	//	UINT Ping;
	//};
	GMSG_SS_MAX,
};

