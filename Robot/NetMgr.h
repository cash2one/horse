#pragma once

#include "MsgDef.h"
#include "GameSocket.h"
#include "TcpEncryption.h"
#include "Log.h"
#include "Common.h"
#include <string>

#define CC_SAFE_DELETE(p)            do { if(p) { delete (p); (p) = 0; } } while(0)
#define CC_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = 0; } } while(0)

class GameRoot;
class NetMngr
{
public:
	NetMngr();
	NetMngr(GameRoot* root);
	virtual ~NetMngr();
	//
public:
	//bool NetSend();
	inline void SetMsgHeader(unsigned short wCmd, unsigned short wRetCode, int nReserved = 0)
	{
		m_msgHeader.wLength		=	sizeof(m_msgHeader);
		m_msgHeader.wCmd		=	wCmd;                //连接协议
		m_msgHeader.wRetCode	=	wRetCode;            //返回成功还是失败，0是成功，1是失败
		m_msgHeader.nReserved	=	nReserved;           //保留字段

		m_strCliPacket.clear();
		m_strCliPacket.append((char*)&m_msgHeader, sizeof(m_msgHeader));		
	}
	//
	//设置包数据长度
	inline void SetMsgLength()
	{
		MSGHEADER* pMsgHeader = (MSGHEADER*)m_strCliPacket.c_str();
		pMsgHeader->wLength = (unsigned short)m_strCliPacket.size();
	}
	//
	void MainLoop(float dt);
	bool OnMessage(const char* buf,int nLen);
	//
	//bool Initialize(const char* szIP,int nPort,PMessageCallback pOnMessageCallback);
	bool Initialize(const char* szIP,int nPort,PMessageCallbackRoot pOnMessageCallback);
	void DisConnect();//主动断开当前连接
	void	SendClientMsg(bool bIsIndirect = true);
	bool    isCannet();
	void    setIsSendHert(bool bl){isSendHert = false;}
	/////////////////////网络消息/////////////////////////////////////////////////////
	void NoticeHeartTest();

#include "MsgSendDeclare.inl"
	//////////////////////////////////////////////////////////////////////////
private:
	CTcpEncryption  * m_pTcpEncryption;
	CGameSocket* m_pClientSocket;
	char m_RemoteIP[16];//ipv4
	int m_nPort;//

	MSGHEADER	m_msgHeader;
	std::string m_strCliPacket;
	PMessageCallbackRoot m_pOnMessageCallback;//网络消息接收回调函数
	time_t m_LastHeartTime;
	bool  isSendHert;

	GameRoot* mGameRoot;
	CLog* mLog;

	bool m_bInit;
	char g_sEncryptPack[64*1024];
};