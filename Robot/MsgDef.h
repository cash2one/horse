#pragma once

#include "msg_def.h"

class NetMngr;
typedef bool (NetMngr::*PMessageCallback)(const char* buf,int nLen);//typedef void (CCObject::*SEL_SCHEDULE)(float);
typedef bool (*PMessageCallbackRoot)(const char* buf,int nLen);

#pragma pack(push ,1)

struct MSGHEADER
{
	unsigned short	wLength;											//�������ĳ���,������ͷ win�µ�WORD
	unsigned short	wCmd;												//����
	unsigned short	wRetCode;											//����״̬
	int				nReserved;											//�����ֶ�
};

typedef struct tagSSMsg
{
	MSGHEADER	Header;
	char		szData[1];
}SSMSG;

struct tagServerList
{
	unsigned int     dwServerID;		//������id ѡ��ʱcheck;
	unsigned char    ServerName[32];	//���������� Լ15������
};

#pragma pack(pop)