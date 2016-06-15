#include "StdAfx.h"
#include ".\dbtransationworkthread.h"



IMPLEMENT_CLASS_INFO_STATIC(CDBTransationWorkThread,CEasyThread);
CDBTransationWorkThread::CDBTransationWorkThread(void)
{
	SS_TRY_BEGIN;
	m_pConnection=NULL;
	m_WaitEvent = NULL;
	SS_TRY_END();
}

CDBTransationWorkThread::~CDBTransationWorkThread(void)
{
	SS_TRY_BEGIN;

	Destory();
	SS_TRY_END();
}

void CDBTransationWorkThread::Destory()
{
	SS_TRY_BEGIN_;

	CDBTansaction ** ppDBTansaction=NULL;
	while(ppDBTansaction = m_TransQueue.PopOne() )
	{
		SAFE_RELEASE(*ppDBTansaction);
	}
	while(ppDBTansaction = m_FinishTransQueue.PopOne() )
	{
		SAFE_RELEASE(*ppDBTansaction);
	}
	SAFE_RELEASE(m_pConnection);
	SS_TRY_END_();
}

bool CDBTransationWorkThread::Init(IDBConnection * pConnection,LPCTSTR ConnectStr,int QueueSize)
{
	SS_TRY_BEGIN_;
	if(pConnection==NULL)
		return false;

	Destory();
	m_WaitEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	if(NULL == m_WaitEvent)
	{
		return false;
	}
	m_ConnectString=ConnectStr;
	m_pConnection=pConnection;
	m_pConnection->EnableTransaction(true);
	m_TransQueue.InitQueue(QueueSize);
	m_FinishTransQueue.InitQueue(QueueSize);
	m_ConnectionTestTimer.SaveTime();

	return Start();
	SS_TRY_END_();
	return false;
}

bool CDBTransationWorkThread::AddTransaction(CDBTansaction * pDBTansaction)
{
	SS_TRY_BEGIN_;
#ifdef _DEBUG
	if( m_FinishTransQueue.GetObjectCount() >= 10 )
	{
		ErrorLog( 3, "TooMany In FinishQUEUE: queue size=[%u] at AddTransaction",
				
				m_FinishTransQueue.GetObjectCount());
	}

	if( m_TransQueue.GetObjectCount() >= 10 )
	{
		ErrorLog( 3, "TooMany In ExeQUEUE: queue size=[%u] at AddTransaction",
			
			m_TransQueue.GetObjectCount());
	}
#endif
	
	if( NULL == pDBTansaction)
	{
		return false;
	}

	int ret = 0;
	GetLocalTime(&(pDBTansaction->m_AddToManagerTime));
	ret = m_TransQueue.PushOne(&pDBTansaction);

	if( ret == 1 )
	{
		SetEvent(m_WaitEvent);
	}
	else if(ret == 0)
	{
		ErrorLog( 3, "m_TransQueue.size = %u",m_TransQueue.GetObjectCount() );
	}
	return ret;
	SS_TRY_END_();

	return false;
}

CDBTansaction * CDBTransationWorkThread::PopFinishTransaction()
{
	SS_TRY_BEGIN_;
	CDBTansaction ** ppDBTansaction=m_FinishTransQueue.PopOne();
	if( ppDBTansaction )
	{
		return *ppDBTansaction;
	}
	SS_TRY_END_();
	return NULL;
}

BOOL CDBTransationWorkThread::OnStart()
{	
	SS_TRY_BEGIN_;
	return TRUE;
	SS_TRY_END_();
	return FALSE;
}
BOOL CDBTransationWorkThread::OnRun()
{
	SS_TRY_BEGIN_;
	
	
#ifdef _DEBUG	
	if( m_FinishTransQueue.GetObjectCount() >= 10 )
#else
	if( m_FinishTransQueue.GetObjectCount() >= 1200 )
#endif
	{
		ErrorLog( 3, "TooMany In FinishQUEUE: queue size=[%u]",
				
				m_FinishTransQueue.GetObjectCount());
	}
#ifdef _DEBUG	
	if( m_TransQueue.GetObjectCount() >= 10 )
#else
	if( m_TransQueue.GetObjectCount() >= 1200 )
#endif
	{
		ErrorLog( 3, "TooMany In ExeQUEUE: queue size=[%u]",
			
			m_TransQueue.GetObjectCount());
	}


	CDBTansaction * pDBTansaction = NULL;
	CDBTansaction ** ppDBTansaction=m_TransQueue.PopOne();
	if( ppDBTansaction )
	{
		pDBTansaction = *ppDBTansaction;
	}
	if(pDBTansaction)
	{
		SYSTEMTIME systm;
		GetLocalTime(&systm);

		uint64 exeMiliSec = (uint64)systm.wMinute* 60*1000 + (uint64)systm.wSecond*1000 + (uint64)systm.wMilliseconds
					- (
					(uint64)pDBTansaction->m_AddToManagerTime.wMinute* 60*1000 + (uint64)pDBTansaction->m_AddToManagerTime.wSecond*1000 + (uint64)pDBTansaction->m_AddToManagerTime.wMilliseconds
					);
		if( exeMiliSec > 250 )
		{
			ErrorLog( 3, "TooLong Time In QUEUE: [%s] InQueueTime = [%I64u]ms,queue size=[%u]",
				pDBTansaction->GetClassName(),
				exeMiliSec,
				
				m_TransQueue.GetObjectCount());
		}
		bool Ret=pDBTansaction->OnExecute(m_pConnection);
		if(Ret)
			m_pConnection->Commit();
		else
			m_pConnection->RollBack();

		SYSTEMTIME tempsystm;
		GetLocalTime(&tempsystm);

		exeMiliSec = (uint64)tempsystm.wMinute* 60*1000 + (uint64)tempsystm.wSecond*1000 + (uint64)tempsystm.wMilliseconds
						- (
						(uint64)systm.wMinute* 60*1000 + (uint64)systm.wSecond*1000 + (uint64)systm.wMilliseconds
						);
		if( exeMiliSec >= 250)
		{
			ErrorLog( 3, "Too Long Execute Time:[%s] ExeuteTime = [%I64u]ms,queue size=[%u]",
				pDBTansaction->GetClassName(),
				exeMiliSec,
				
				m_TransQueue.GetObjectCount());
		}

		pDBTansaction->SetResult(Ret);

		m_FinishTransQueue.PushOne(&pDBTansaction);
		
		pDBTansaction = NULL;
	
	}
	else
	{
		
		WaitForSingleObject( m_WaitEvent,2500);
		//Sleep(1);
	}
	if(m_ConnectionTestTimer.IsTimeOut(CONNECTION_TEST_TIME))
	{
		if(!m_pConnection->IsConnected())
		{
			ErrorLog( 3,"[%s]\n数据库连接已断开,重新连接", (LPCTSTR)m_ConnectString);
			DBLog(3, "[%s]\n数据库连接已断开,重新连接", (LPCTSTR)m_ConnectString);
			int nRetCode = m_pConnection->Connect((LPCTSTR)m_ConnectString);
			if(nRetCode!=DBERR_SUCCEED)
			{
				ErrorLog( 3,"[%s]\n数据库重新连接失败, retcode=%d, errorcode=%d, desc=%s"
					,(LPCTSTR)m_ConnectString, nRetCode, m_pConnection->GetLastDatabaseErrorCode(), m_pConnection->GetLastDatabaseErrorString());
			}
		}
		m_ConnectionTestTimer.SaveTime();
	}
	return TRUE;
	SS_TRY_END_();
	return FALSE;
}
void CDBTransationWorkThread::OnTerminate()
{
	SS_TRY_BEGIN_;
	SS_TRY_END_();
}

