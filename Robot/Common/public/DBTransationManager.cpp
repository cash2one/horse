#include "StdAfx.h"
#include ".\dbtransationmanager.h"

IMPLEMENT_CLASS_INFO_STATIC(CDBTransationManager,CNameObject);
CDBTransationManager::CDBTransationManager(void)
{
	SS_TRY_BEGIN;
	m_pDatabase=NULL;
	SS_TRY_END();
}

CDBTransationManager::~CDBTransationManager(void)
{
	SS_TRY_BEGIN;
	Destory();
	SS_TRY_END();
}

void CDBTransationManager::Destory()
{
	SS_TRY_BEGIN_;
	for(int i=0;i<(int)m_WorkThreads.size();i++)
	{	
		if(m_WorkThreads[i].m_pDbThread)
		{
			m_WorkThreads[i].m_pDbThread->SafeTerminate(DEFAULT_THREAD_TERMINATE_TIME);
		}
	}	
	m_WorkThreads.clear();
	SS_TRY_END_();
}

bool CDBTransationManager::Init(IDatabase * pDatabase, const DbMap* dbMap,uint32 threadNum)
{
	SS_TRY_BEGIN_;
	if(pDatabase==NULL)
		return false;
	Destory();
	if( NULL == m_pDatabase )
	{
		m_pDatabase=pDatabase;
	}
	else
	{
		ErrorLog(2, "m_pDatabase != NULL");
		return false;
	}

	memcpy( m_DbMap,  dbMap, sizeof(m_DbMap) );
	m_WorkThreads.resize( threadNum );
	return true;
	SS_TRY_END_();
	return false;
}

bool CDBTransationManager::AddDBThread(uint32 dbID, const char* szConnectStr,int QueueSize)
{
	SS_TRY_BEGIN;
	IDBConnection * pConnection=m_pDatabase->CreateConnection();
	CDBTransationWorkThread * pThread = NULL;
	if(pConnection->Connect(szConnectStr)==DBERR_SUCCEED)
	{
		pThread=new CDBTransationWorkThread;
		TraceLog( 1, "CDBTransationWorkThread* = 0x%08x", pThread);
		if ( NULL == pThread ) return false;
		if(!pThread->Init(pConnection,szConnectStr,QueueSize))
		{
			SAFE_RELEASE(pThread);
			return false;
		}
		DbConnectThread dbConnectThread;
		dbConnectThread.m_pDbThread = pThread;
		dbConnectThread.m_DbID = dbID;
		strncpy_0(dbConnectThread.szConnectStr,  szConnectStr, sizeof(dbConnectThread.szConnectStr) );
		
		m_WorkThreads[dbID] = dbConnectThread;
		dbConnectThread.m_pDbThread = NULL;
	}
	else
	{
		SAFE_RELEASE(pConnection);
		return false;
	}
	
	
	return true;
	SS_TRY_END();
	return false;
}

CDBTransationWorkThread * CDBTransationManager::AllocWorkThread(uint64 DigitalPTID)
{
	SS_TRY_BEGIN_;
	uint32 hashValue = 0;
	if(DigitalPTID > 0)
	{
		hashValue = DigitalPTID%(DB_MAX_HASH);//1-10
		if( hashValue == 0 )
		{
			hashValue = DB_MAX_HASH;
		}
	}
	else
	{
		hashValue = 0; //0
	}

	if(m_DbMap[hashValue].hashValue ==  hashValue)
	{
		int dbID = m_DbMap[hashValue].m_DbID;
		
		
		if( dbID < m_WorkThreads.size() ) 
		{
			return m_WorkThreads[dbID].m_pDbThread;
		}
		else
		{
			return NULL;
		}
		
	}
	else
	{
		ErrorLog(2, "AllocDBTread error m_DbMap[hashValue].hashValue%d != %d",m_DbMap[hashValue].hashValue,hashValue);
	}
	SS_TRY_END_();

	return NULL;
}

bool CDBTransationManager::AddTransaction( CDBTansaction * pDBTansaction, uint64 DigitalPTID)
{
	SS_TRY_BEGIN_;

	//TODO claus for debug
	if(DigitalPTID > 1)
		DigitalPTID = 1;

	CDBTransationWorkThread * pThread=AllocWorkThread(DigitalPTID);
	if(pThread)
	{
		return pThread->AddTransaction(pDBTansaction);
	}
	else
	{
		ErrorLog( 3, "in AddTransaction AllocWorkThread [%I64u]" ,DigitalPTID );
	}
	SS_TRY_END_();
	return false;
}



int CDBTransationManager::Update(int ProcessLimit)
{
	SS_TRY_BEGIN_;
	int ProcessCount=0;
	while(ProcessCount<ProcessLimit)
	{
		int Count=0;
		for(int i=0;i<(int)m_WorkThreads.size();i++)
		{
			if ( m_WorkThreads[i].m_pDbThread )
			{
				CDBTansaction * pDBTansaction=m_WorkThreads[i].m_pDbThread->PopFinishTransaction();
				if(pDBTansaction)
				{
					pDBTansaction->OnFinish();
					SAFE_RELEASE(pDBTansaction);
					ProcessCount++;
					Count++;
				}
			}
		}
		if(Count==0)
			break;
	}
	
	return ProcessCount;
	SS_TRY_END_();
	return 0;
}

bool CDBTransationManager::IsIdle()
{
	SS_TRY_BEGIN_;
	int Len=0;
	for(int i=0;i<(int)m_WorkThreads.size();i++)
	{
		if(m_WorkThreads[i].m_pDbThread) 
		{
			Len+=m_WorkThreads[i].m_pDbThread->GetQueueLen()+m_WorkThreads[i].m_pDbThread->GetFinishQueueLen();
		}
	}
	return Len==0;
	SS_TRY_END_();
	return false;
}
