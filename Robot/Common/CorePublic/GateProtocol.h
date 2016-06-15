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

struct CLIENT_ADDRESS_INFO
{
	ULONG64			ClientID;	
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

struct GROUP_MEMBER_INFO
{
	GROUP_MEMBER_INFO()
	{
		ZeroMemory(this,sizeof(*this));
	}
	DWORD	GateID;
	UINT	GroupID;
	ULONG64	ClientID;	
	ULONG64	SrcClientID;
	BYTE	Announcer;
};

enum AUDIO_ANNOUNCER
{
	AUDIO_ANNOUNCER_NONE,
	AUDIO_ANNOUNCER_FULL,
	AUDIO_ANNOUNCER_SLIENT,
};

enum GATE_OBJECT_TYPE
{
	GATE_OBJ_NONE,
	GATE_OBJ_CLIENT,
	GATE_OBJ_GROUP,
	GATE_OBJ_MAX,
};

struct GROUP_MSG_HEADER
{
	UINT	GroupID;
	ULONG64	SenderID;
	ULONG64	ExceptClientID;	
};

struct GROUP_MSG
{
	GROUP_MSG_HEADER Header;
	char szData[1];
};


//////////////////////////////////////////////////////////////////////////////////
struct CLIENT_SEND_INFO
{
	UINT			TransID;
	MSG_SEND_PARAM	SendParam;
};

enum GATE_SS_MSGS
{
	GMSG_SS_GATE_SERVER_INFO,
	//GATE_SERVER_INFO ServerInfo
	GMSG_SS_LOAD_INFO,
	//DWORD 当前负载
	GMSG_SS_UDP_MSG_PACKET,
	//请求发送消息报
	//struct  
	//{
	//	WORD				ClientCount;				//客户端数量
	//	CLIENT_ADDRESS_INFO	ClientID[ClientCount];		//客户端地址
	//	char				Data[];						//要转发的数据
	//};
	GMSG_SS_CREATE_GROUP,					//建立组
	//ULONG64 ClientID
	GMSG_SS_CREATE_GROUP_RESULT,
	//ULONG64 ClientID;UINT 组ID  =0为失败

	GMSG_SS_DEL_GROUP,						//删除组
	//ULONG64 ClientID;UINT 组ID
	GMSG_SS_DEL_GROUP_RESULT,
	//ULONG64 ClientID;UINT 组ID;BYTE Result=0为成功

	GMSG_SS_ADD_CLIENT,						//添加客户端
	//ULONG64 ClientID
	GMSG_SS_ADD_CLIENT_RESULT,
	//ULONG64 ClientID,BYTE Result=0为成功

	GMSG_SS_DEL_CLIENT,						//删除客户端
	//ULONG64 ClientID
	GMSG_SS_DEL_CLIENT_RESULT,
	//ULONG64 ClientID,BYTE Result=0为成功

	GMSG_SS_ADD_GROUP_MEMBER,				//添加组成员
	//GROUP_MEMBER_INFO Info
	GMSG_SS_ADD_GROUP_MEMBER_RESULT,	
	//GROUP_MEMBER_INFO Info,BYTE Result==0为成功 =1为添加失败 =2为组不存在

	GMSG_SS_DEL_GROUP_MEMBER,				//删除组成员
	//GROUP_MEMBER_INFO Info
	GMSG_SS_DEL_GROUP_MEMBER_RESULT,
	//GROUP_MEMBER_INFO Info,BYTE Result==0为成功 =1为删除失败 =2为组不存在	

	GMSG_SS_UDP_GROUP_MSG_PACKET,			//发送组消息
	//struct  
	//{	
	//	GROUP_MSG_HEADER	Header;			
	//	char				Data[];						//要转发的数据
	//};

	GMSG_SS_UDP_GROUP_MSG_PACKET_EX,				//请求转发组消息
	//struct  
	//{	
	//	GROUP_MSG_HEADER	Header;			
	//	char				Data[];						//要转发的数据
	//};

	GMSG_SS_UDP_GROUP_MSG_PACKET_EX_RESULT,
	//ULONG64	SenderID;BYTE Result=0成功

	
	GMSG_SS_CHECK_OBJECT,					//检查某个服务器对象的状态
	//struct
	//{	
	//	UNLONG64	SessionID;
	//	BYTE		ObjectType;
	//	union			
	//	{
	//		UNLONG64	ClientID;
	//		UINT		GroupID;
	//	};
	//	
	//};
	GMSG_SS_CHECK_OBJECT_RESULT,
	//struct
	//{	
	//	BYTE		Result;				//=0存在，=1不存在
	//	UNLONG64	SessionID;
	//	BYTE		ObjectType;
	//	union			
	//	{
	//		UNLONG64	ClientID;
	//		UINT		GroupID;
	//	};
	//	
	//};

/////////////////////////////////////////////////////////////////////////////////////////
	GMSG_SS_UDP_BROADCAST,
	//请求群发消息报
	//struct  
	//{
	//	WORD				ClientCount;				//客户端数量
	//	CLIENT_SEND_INFO	TransID[ClientCount];		//客户端TransID
	//	char				Data[];						//要转发的数据
	//};

	GMSG_SS_MAX,
};

