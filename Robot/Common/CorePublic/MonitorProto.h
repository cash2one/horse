#pragma once
#pragma pack(push ,1)

struct MoHeader
{
	WORD	length;	//Э�����ݳ���
	byte	type;	//Э���ʶ, eMoMessageType
	byte	ret;	//����ֵ, eMoProtoResult
};

struct MOPKG_CONFIG_ACK
{
	char			sListenIP[20];
	WORD			nListenPort;
	unsigned int	nMaxPlayers;
};

struct MOPKG_UPDATE_ACK
{
	UINT			nCurPlayers;	//��ǰ�������
	float			fCycleTime;		//��Ӧʱ��
	float			fTcpRecvFlow;	//���ܵ�tcp����
	float			fTcpSendFlow;	//���͵�tcp����
	float			fUdpRecvFlow;	//���ܵ�udp����
	float			fUdpSendFlow;	//���͵�udp����
};

//��ط��񷵻صĴ�����
enum eMoProtoResult
{
	mo_ret_sucess,	//�����ɹ�
};

//��ط�������Э������
enum eMoMessageType
{
	MO_MESSAGE_START,

	MOID_REQ_CONFIG_INFO= 10,	//����Է���������������Ϣ�� MoHeader
	MOID_ACK_CONFIG_INFO= 11,	//���ضԷ���������������Ϣ�� MoHeader + MOPKG_CONFIG_ACK
	MOID_REQ_UPDATE_DATA= 12,	//����Է��������Ķ�̬��Ϣ�� MoHeader
	MOID_ACK_UPDATE_DATA= 13,	//���ضԷ��������Ķ�̬��Ϣ�� MoHeader + MOPKG_UPDATE_ACK

	MO_MESSAGE_END,
};

#pragma pack(pop)
