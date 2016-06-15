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
	//DWORD ��ǰ����
	GMSG_SS_UDP_MSG_PACKET,
	//��������Ϣ��
	//struct  
	//{
	//	WORD				ClientCount;				//�ͻ�������
	//	CLIENT_ADDRESS_INFO	ClientID[ClientCount];		//�ͻ��˵�ַ
	//	char				Data[];						//Ҫת��������
	//};
	GMSG_SS_CREATE_GROUP,					//������
	//ULONG64 ClientID
	GMSG_SS_CREATE_GROUP_RESULT,
	//ULONG64 ClientID;UINT ��ID  =0Ϊʧ��

	GMSG_SS_DEL_GROUP,						//ɾ����
	//ULONG64 ClientID;UINT ��ID
	GMSG_SS_DEL_GROUP_RESULT,
	//ULONG64 ClientID;UINT ��ID;BYTE Result=0Ϊ�ɹ�

	GMSG_SS_ADD_CLIENT,						//��ӿͻ���
	//ULONG64 ClientID
	GMSG_SS_ADD_CLIENT_RESULT,
	//ULONG64 ClientID,BYTE Result=0Ϊ�ɹ�

	GMSG_SS_DEL_CLIENT,						//ɾ���ͻ���
	//ULONG64 ClientID
	GMSG_SS_DEL_CLIENT_RESULT,
	//ULONG64 ClientID,BYTE Result=0Ϊ�ɹ�

	GMSG_SS_ADD_GROUP_MEMBER,				//������Ա
	//GROUP_MEMBER_INFO Info
	GMSG_SS_ADD_GROUP_MEMBER_RESULT,	
	//GROUP_MEMBER_INFO Info,BYTE Result==0Ϊ�ɹ� =1Ϊ���ʧ�� =2Ϊ�鲻����

	GMSG_SS_DEL_GROUP_MEMBER,				//ɾ�����Ա
	//GROUP_MEMBER_INFO Info
	GMSG_SS_DEL_GROUP_MEMBER_RESULT,
	//GROUP_MEMBER_INFO Info,BYTE Result==0Ϊ�ɹ� =1Ϊɾ��ʧ�� =2Ϊ�鲻����	

	GMSG_SS_UDP_GROUP_MSG_PACKET,			//��������Ϣ
	//struct  
	//{	
	//	GROUP_MSG_HEADER	Header;			
	//	char				Data[];						//Ҫת��������
	//};

	GMSG_SS_UDP_GROUP_MSG_PACKET_EX,				//����ת������Ϣ
	//struct  
	//{	
	//	GROUP_MSG_HEADER	Header;			
	//	char				Data[];						//Ҫת��������
	//};

	GMSG_SS_UDP_GROUP_MSG_PACKET_EX_RESULT,
	//ULONG64	SenderID;BYTE Result=0�ɹ�

	
	GMSG_SS_CHECK_OBJECT,					//���ĳ�������������״̬
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
	//	BYTE		Result;				//=0���ڣ�=1������
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
	//����Ⱥ����Ϣ��
	//struct  
	//{
	//	WORD				ClientCount;				//�ͻ�������
	//	CLIENT_SEND_INFO	TransID[ClientCount];		//�ͻ���TransID
	//	char				Data[];						//Ҫת��������
	//};

	GMSG_SS_MAX,
};

