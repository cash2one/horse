#pragma once

struct GATE_MSG_HEADER
{
	WORD	wLength;											//�������ĳ���,������ͷ
	WORD	wCmd;												//����
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
	//DWORD ��ǰ����
	
	GMSG_SS_ACCEPT_CLIENT,
	//֪ͨGate���ܿͻ���
	//GATE_CLIENT_INFO AcceptInfo	
	GMSG_SS_CLIENT_ENTER,
	//�ͻ��˽��������
	//GATE_CLIENT_INFO ClientInfo;
	GMSG_SS_CLIENT_LEAVE,
	//�ͻ��˽��������
	//GATE_CLIENT_LEAVE_INFO LeaveInfo;
	GMSG_SS_SET_BROADCAST_ANNOUNCER,
	//���ÿͻ��˵Ĺ㲥Ȩ��
	//struct  
	//{
	//	UINT Channel;
	//	UINT TransID;
	//	BYTE Announcer;
	//};
	GMSG_SS_SET_BROADCAST_ACCEPT,
	//���ÿͻ��˹㲥���ն���
	//struct  
	//{
	//	UINT Channel;
	//	UINT TransID;
	//	UINT AcceptCharID;
	//};
	GMSG_SS_ADD_BROADCAST_MEMBER,
	//��ӹ㲥��Ա
	//struct  
	//{
	//	UINT Channel;
	//	WORD TargetCount;
	//	UINT TargetTransID[TargetCount];
	//	WORD MemberCount;
	//	BROADCAST_MEMBER_INFO MemberInfo[MemberCount];
	//};

	GMSG_SS_DEL_BROADCAST_MEMBER,
	//ɾ���㲥��Ա
	//struct  
	//{
	//	UINT Channel;
	//	WORD TargetCount;
	//	UINT TargetTransID[TargetCount];
	//	BROADCAST_MEMBER_INFO MemberInfo[MemberCount];
	//};
	GMSG_SS_DEL_ALL_BROADCAST_MEMBER,
	//ɾ�����й㲥��Ա
	//struct  
	//{
	//	UINT Channel;	
	//	UINT TargetTransID;
	//};
	
	GMSG_SS_UDP_MSG_BROADCAST,
	//����Ⱥ����Ϣ��
	//struct  
	//{
	//	WORD				ClientCount;				//�ͻ�������
	//	CLIENT_SEND_INFO	TransID[ClientCount];		//�ͻ���TransID
	//	char				Data[];						//Ҫת��������
	//};
	GMSG_SS_TCP_MSG_PACKET,
	//�ͻ�����Ϣ��
	//struct  
	//{		
	//	UINT TransID;									//�ͻ���TransID
	//	char Data[];									//Ҫת��������
	//};
	GMSG_SS_CLIENT_PING,
	//֪ͨĳ���ͻ��˵�PING
	//struct
	//{
	//	UINT TransID;
	//	UINT Ping;
	//};
	GMSG_SS_MAX,
};

