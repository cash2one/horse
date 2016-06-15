/******************************************************************
* Copyright (c) 2006,�Ϻ�ʢ������
* All rights reserved.
* 
* �ļ����ƣ�a2g_protocol.h
* ժ    Ҫ����Ƶ����������Ϸ��������Э��
* 
* ��ǰ�汾��1.0
* ��    �ߣ�sunwanxun
* �������ڣ�2007-7-18
********************************************************************/

#pragma once

struct ProtoHeader
{
	WORD	length;	//Э�����ݳ���
	byte	type;	//Э���ʶ, ProtoType
	byte	ret;	//����ֵ, eProtoResult
};

enum eProtoResult
{
	eProtoResult_Success,			//�����ɹ�
	eProtoResult_WarnSuc,			//������ɹ�������Ӧ���д��������
	eProtoResult_TooSmallGroup,		//����ʧ��
	eProtoResult_HasGroupService,	//����Ƶ�������Ѿ�Ϊ�˷���ҽӹ�һ����
	eProtoResult_ConnectIsFull,		//���ӵ�����Ѿ���������
	eProtoResult_ServerIsLocked,	//������������
	eProtoResult_GroupIsFull,		//��Ƶ������
	eProtoResult_ClientNotFound,	//���δ�ҵ�
	eProtoResult_GroupNotFound,		//��δ�ҵ�

	//UDP����
	eProtoResult_IllegalAddress,	//�Ƿ��ķ��͵�ַ
	eProtoResult_IllegalToken,		//�Ƿ���Token
	eProtoResult_IllegalFormat,		//�Ƿ�����Ƶ����ʽ
	eProtoResult_UnauthAudio,		//δ��Ȩ����Ƶ��������
	eProtoResult_InvalidAudioType,	//��Ч��Ƶ����
	eProtoResult_EmptyAudioPacket,	//��ҷ��Ϳ���Ƶ���ݰ�
};

enum ProtoType
{
	G2A_REQ_RESETSERVER = 0x00,	//����������Ƶ������
	A2G_ACK_RESETSERVER,		//�ظ�������Ƶ������
	
	G2A_REQ_RESETGROUP = 0x02,	//����������Ƶ��
	//struct
	//{
	//	RoomID nRoomID;
	//};
	A2G_ACK_RESETGROUP,			//�ظ�������Ƶ��
	//struct
	//{
	//	RoomID nRoomID;
	//};
	
	G2A_REQ_GETCONFIG = 0x04,	//������Ƶ������������Ϣ
	A2G_ACK_GETCONFIG,			//�ظ���Ƶ������������Ϣ
	//struct AudioSvrConfig
	
	G2A_REQ_CREATEGROUP = 0x06,	//���󴴽���Ƶ��
	//struct
	//{
	//	RoomID nRoomID;
	//};
	A2G_ACK_CREATEGROUP,		//�ظ�������Ƶ��
	//struct
	//{
	//	RoomID nRoomID;
	//};

	
	G2A_REQ_CLOSEGROUP = 0x08,	//����ر���Ƶ��
	//struct
	//{
	//	RoomID nRoomID;
	//};
	A2G_ACK_CLOSEGROUP,			//�ظ��ر���Ƶ��
	//struct
	//{
	//	RoomID nRoomID;
	//};
	
	G2A_REQ_ADDCLIENT = 0x0A,	//������Ҽ�����Ƶ��
	//struct
	//{
	//	RoomID nRoomID;
	//	[AudioClient]
	//};
	A2G_ACK_ADDCLIENT,			//�ظ�����Ҽ�����Ƶ��
	//struct
	//{
	//	RoomID nRoomID;
	//	CharaID nCharaID;
	//};

	G2A_REQ_DELCLIENT = 0x0C,	//����ɾ�����
	//struct
	//{
	//	RoomID nRoomID;
	//	CharaID nCharaID
	//};
	A2G_ACK_DELCLIENT,			//�ظ�ɾ�����
	//struct
	//{
	//	RoomID nRoomID;
	//	CharaID nCharaID
	//};
	
	G2A_REQ_UPDATECLIENT = 0x0E,	//����������״̬
	//struct AudioClient;
	A2G_ACK_UPDATECLIENT,			//�ظ��������״̬
	//struct
	//{
	//	RoomID nRoomID;
	//	CharaID nCharaID
	//};

	A2G_REQ_SENDCLIENTAUDIO = 0x20,	//������Ϸ������ת�������Ƶ����
	//struct
	//{
	//	RoomID	  nRoomID;
	//	MSGHEADER msg;
	//	AudioData data;
	//};
	G2A_ACK_SENDCLIENTAUDIO,	//�ظ���Ϸ������ת�������Ƶ����
	//struct
	//{
	//	RoomID	nRoomID;
	//	CharaID	nCharaID;
	//};

	G2A_REQ_SENDSERVERAUDIO = 0x30,	//������Ƶ������ת��������Ƶ��������������Ƶ����
	//struct
	//{
	//	RoomID	  nRoomID;
	//	MSGHEADER msg;
	//	AudioData data;
	//};
	A2G_ACK_SENDSERVERAUDIO,		//�ظ���Ƶ������ת��������Ƶ��������������Ƶ����
	//struct
	//{
	//	RoomID	nRoomID;
	//	CharaID	nCharaID;
	//};

	A2G_REP_CURSTATUS = 0x40,		//������Ƶ������״̬
	//struct
	//{
	//	UINT nHoldenClients;
	//};
	A2G_REP_KEEPALIVE,				//������Ƶ����������
	A2G_REP_GROUPLIST,				//������Ƶ�������ϴ�������Ƶ���б�
	A2G_REP_ILLEGALAUDIO,			//�յ���ҷ��͵ķǷ���Ƶ����
	//struct
	//{
	//	CharaID		nCharaID;
	//	RoomID		nRoomID;
	//	UINT		nSendIP;
	//	WORD		nSendPort;
	//};

	A2G_REP_DELCLIENT,				//����ɾ����ĳ����Ƶ���е���� -- ����Ϸ����������Ƶ������֮���״̬��ȫͬ��ʱ�����ᷢ��
	//struct
	//{
	//	short nCount;
	//	struct
	//	{
	//		RoomID nRoomID;
	//		CharaID nCharaID;
	//	}[nCount];
	//};

	G2A_NOT_CLEARBUFFER,			//֪ͨӡ�׷����������Ƶ���ݻ���
	//struct
	//{
	//	RoomID nRoomID;
	//};
	G2A_NOT_SETREPLAYER,			//�������ò��������ԭʼ�ݳ���
	//struct
	//{
	//	RoomID	nRoomID;
	//	short	nCount;
	//	CharaID	nReplayedSingerID[ nCount ];
	//};
};
