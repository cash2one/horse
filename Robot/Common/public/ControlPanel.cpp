#include "StdAfx.h"
#include ".\controlpanel.h"

CControlPanel::CControlPanel(void)
{
	m_MsgPool.Create(1024);
	m_CommandPool.Create(1024);
	ZeroMemory(&m_ServerInfo,sizeof(m_ServerInfo));
	m_hWnd=NULL;
}

CControlPanel::~CControlPanel(void)
{
	
}

BOOL CControlPanel::PushMsg(LPCTSTR szMsg)
{
	SS_TRY_BEGIN_;
	return PushMsg(CON_MSG_LOG,szMsg,(int)strlen(szMsg)+1);
	SS_TRY_END_();
	return FALSE;
}

BOOL CControlPanel::PushMsg(UINT Type,LPCVOID pData,int Len)
{
	SS_TRY_BEGIN_;
	if(m_hWnd)
	{
		PANEL_MSG * pMsg;
		UINT ID=m_MsgPool.NewObject(&pMsg);
		if(ID)
		{
			pMsg->ID=ID;
			pMsg->MsgType=Type;
			if(Len>MAX_CONSOLE_MSG_LEN)
				Len=MAX_CONSOLE_MSG_LEN;
			memcpy(pMsg->Msg,pData,Len);
			PostMessage(m_hWnd,WM_USER_LOG_MSG,0,(LPARAM)pMsg);
			return TRUE;
		}
	}
	SS_TRY_END_();
	return FALSE;
}

BOOL CControlPanel::ReleaseMsg(UINT ID)
{
	SS_TRY_BEGIN_;

	return m_MsgPool.DeleteObject(ID);

	SS_TRY_END_();
	return FALSE;
}

void CControlPanel::SetClientNum(int Num)
{
	SS_TRY_BEGIN_;

	PostMessage(m_hWnd,WM_USER_CLIENT_NUM,Num,0);

	SS_TRY_END_();
}

void CControlPanel::SendWndMsg(UINT MsgID,WPARAM wParam,LPARAM lParam)
{
	SS_TRY_BEGIN_;

	PostMessage(m_hWnd,MsgID,wParam,lParam);

	SS_TRY_END_();
}

BOOL CControlPanel::PushCommand(LPCTSTR szCommand)
{
	SS_TRY_BEGIN_;
	
	PANEL_MSG * pMsg;
	UINT ID=m_CommandPool.NewObject(&pMsg);
	if(ID)
	{
		pMsg->ID=ID;
		strncpy_0(pMsg->Msg,szCommand,MAX_CONSOLE_MSG_LEN);		
		return TRUE;
	}

	SS_TRY_END_();
	return FALSE;
}

PANEL_MSG * CControlPanel::GetCommand()
{
	SS_TRY_BEGIN_;
	LPVOID Pos=m_CommandPool.GetFirstObjectPos();

	if(Pos)
		return m_CommandPool.GetNext(Pos);
	else
		return NULL;
	SS_TRY_END_();
	return NULL;
}

BOOL CControlPanel::ReleaseCommand(UINT ID)
{
	SS_TRY_BEGIN_;

	return m_CommandPool.DeleteObject(ID);

	SS_TRY_END_();
	return FALSE;
}

void CControlPanel::SetServerInfo(SERVER_INFO& ServerInfo)
{
	SS_TRY_BEGIN;
	CAutoLock AutoLock(m_CriticalSection);
	m_ServerInfo=ServerInfo;
	SS_TRY_END();
}
void CControlPanel::GetServerInfo(SERVER_INFO& ServerInfo)
{
	SS_TRY_BEGIN;
	CAutoLock AutoLock(m_CriticalSection);
	ServerInfo=m_ServerInfo;
	SS_TRY_END();
}
void PrintUIMsg(const char* sMsgText, int nMsgLength)
{
	CControlPanel::GetInstance()->PushMsg(CON_MSG_LOG, sMsgText, nMsgLength+1);
}