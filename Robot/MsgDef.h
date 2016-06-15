#pragma once

#include "msg_def.h"

class NetMngr;
typedef bool (NetMngr::*PMessageCallback)(const char* buf,int nLen);//typedef void (CCObject::*SEL_SCHEDULE)(float);
typedef bool (*PMessageCallbackRoot)(const char* buf,int nLen);

#pragma pack(push ,1)

struct MSGHEADER
{
	unsigned short	wLength;											//整个包的长度,包括包头 win下的WORD
	unsigned short	wCmd;												//命令
	unsigned short	wRetCode;											//返回状态
	int				nReserved;											//保留字段
};

typedef struct tagSSMsg
{
	MSGHEADER	Header;
	char		szData[1];
}SSMSG;

struct tagServerList
{
	unsigned int     dwServerID;		//服务器id 选组时check;
	unsigned char    ServerName[32];	//服务器名称 约15个汉字
};

#pragma pack(pop)