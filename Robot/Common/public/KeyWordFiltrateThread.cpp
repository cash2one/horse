#include "StdAfx.h"
#include ".\keywordfiltratethread.h"

IMPLEMENT_CLASS_INFO_STATIC(CKeyWordFiltrateThread, CEasyThread);
CKeyWordFiltrateThread::CKeyWordFiltrateThread(void)
	:m_KeyWordsWaitEvent(NULL)
{
	memset( m_szTaiwanOneWords, 0, sizeof(m_szTaiwanOneWords));
}

CKeyWordFiltrateThread::~CKeyWordFiltrateThread(void)
{
}

bool CKeyWordFiltrateThread::InitKeyWordsFiltrate(int QueueSize)
{
	SS_TRY_BEGIN_;
	
	m_KeyWordsWaitEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	if(NULL == m_KeyWordsWaitEvent)
	{
		return false;
	}
	//claus
	m_WordsTransQueue.InitQueue(QueueSize);
	m_WordsFinishTransQueue.InitQueue(QueueSize);

	return Start();
	SS_TRY_END_();
	return false;
}

bool CKeyWordFiltrateThread::SetWordsSet(const NoCaseWordSet& setBlackWords)
{
	SS_TRY_BEGIN_;
	m_Lock.Lock();
	m_setBlackWords = setBlackWords;
	m_Lock.Unlock();
	return true;
	SS_TRY_END_();
	return false;
}

//初始化执行一次
bool CKeyWordFiltrateThread::SetTaiWanOneWords(const char* pszTaiWanOneWords)
{
	SS_TRY_BEGIN_;
	m_Lock.Lock();
	strncpy_0( m_szTaiwanOneWords, pszTaiWanOneWords, sizeof(m_szTaiwanOneWords));
	m_Lock.Unlock();
	return true;
	SS_TRY_END_();
	return false;
}

bool CKeyWordFiltrateThread::AddTransaction(UINT64 nDigitalID, uint32 charID, UINT nClubFlagID, const char* pszCharName, const char* pszInMsg,int len)
{
	SS_TRY_BEGIN_;
	
	SaidWords	msg = {0};
	msg.nDigitalID = nDigitalID;
	msg.nClubFlagID = nClubFlagID;
	msg.charId = charID;
	strncpy_0( msg.szCharName, pszCharName, sizeof(msg.szCharName));
	
	msg.len = len;
	strncpy_0( msg.szInMsg, pszInMsg, sizeof(msg.szInMsg));

	int ret = m_WordsTransQueue.PushOne(&msg);

	if( ret == 1 )
	{
		SetEvent(m_KeyWordsWaitEvent);
	}
	else if(ret == 0)
	{
		ErrorLog( 3, "m_TransQueue.size = %u",m_WordsTransQueue.GetObjectCount() );
	}
	return ret;
	SS_TRY_END_();

	return false;
}

SaidWords * CKeyWordFiltrateThread::PopFinishTransaction()
{
	SS_TRY_BEGIN_;

	return m_WordsFinishTransQueue.PopOne();
	
	SS_TRY_END_();
	return NULL;
}

BOOL CKeyWordFiltrateThread::OnRun()
{
	SS_TRY_BEGIN_;
	
	SaidWords * pTansaction=m_WordsTransQueue.PopOne();
	if( pTansaction )
	{
		SYSTEMTIME	CurTime;
		GetLocalTime(&CurTime);
		char MsgBuffer[300] ={0};
		int nMsgLen = _snprintf(MsgBuffer, SIZEOF_STR(MsgBuffer), "<SD:%u><SN:%s><ST:%hu-%02hu-%02hu %02hu:%02hu:%02hu>",
			pTansaction->charId,
			pTansaction->szCharName,
			CurTime.wYear,CurTime.wMonth,CurTime.wDay,
			CurTime.wHour,CurTime.wMinute,CurTime.wSecond);
		strncat(MsgBuffer, "<BODY>", 6);
		int headStrLen = strlen(MsgBuffer);
		strcat( MsgBuffer, pTansaction->szInMsg);
		strncpy_0( pTansaction->szInMsg, MsgBuffer, sizeof(pTansaction->szInMsg));
		m_Lock.Lock();
		if( KeyWordFiltrate( pTansaction->szInMsg + headStrLen,pTansaction->len, m_setBlackWords ) )
		{		
		}
		m_Lock.Unlock();
		pTansaction->len = (int)strlen(pTansaction->szInMsg);
		m_WordsFinishTransQueue.PushOne(pTansaction);
	}
	else
	{
		WaitForSingleObject( m_KeyWordsWaitEvent, INFINITE);
	}

	return TRUE;
	SS_TRY_END_();
	return FALSE;
}