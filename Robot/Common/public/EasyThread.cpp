#include "StdAfx.h"
#include ".\easythread.h"

#include <process.h>

IMPLEMENT_CLASS_INFO(CEasyThread,CNameObject);

CEasyThread::CEasyThread():CNameObject()
{
	m_ThreadID=0;	
	m_hThread=NULL;	
	m_IsSuspended=FALSE;
	m_IsWorking=FALSE;
	m_IsTerminate=FALSE;
}

CEasyThread::~CEasyThread()
{
	SafeTerminate(DEFAULT_THREAD_TERMINATE_TIME);	
}


BOOL CEasyThread::Start(BOOL IsSuspended)
{
	unsigned int ThreadID;
	int Flag = 0;	

	if(IsWorking())
		return FALSE;

	if (IsSuspended)
	{
		Flag = CREATE_SUSPENDED;
		m_IsSuspended = TRUE;
	}
	else
		m_IsSuspended = FALSE;

	m_IsWorking=TRUE;
	m_IsTerminate=FALSE;

	HANDLE hThread = (HANDLE)_beginthreadex(
		NULL,
		0,
		CEasyThread::ThreadProc,
		(void*)this,
		Flag,
		(UINT *)&ThreadID);
	if (hThread == INVALID_HANDLE_VALUE)
		return FALSE;
	m_ThreadID = ThreadID;
	m_hThread = hThread;
	
	return TRUE;
}


BOOL CEasyThread::Resume()
{
	if (ResumeThread(m_hThread) != (DWORD)-1)
	{
		m_IsSuspended = FALSE;
		return TRUE;
	}
	else
		return FALSE;
}


BOOL CEasyThread::Suspend()
{
	if (SuspendThread(m_hThread) != (DWORD)-1)
	{
		m_IsSuspended = TRUE;
		return TRUE;
	}
	else
		return FALSE;
}


void CEasyThread::Terminate()
{
	::InterlockedExchange((LONG *)&m_IsTerminate,TRUE);
}


void CEasyThread::ForceTerminate()
{
	TerminateThread(m_hThread,0);
}

void CEasyThread::SafeTerminate(DWORD Milliseconds)
{
	Terminate();
	if(!WaitForTerminate(Milliseconds))
		ForceTerminate();
}

BOOL CEasyThread::WaitForTerminate(DWORD Milliseconds)
{
	if(Milliseconds==INFINITE)
	{	
		while(IsWorking())
		{
			Sleep(10);
		}
		return TRUE;
	}
	else
	{		
		CEasyTimer Timer;
		Timer.SetTimeOut(Milliseconds);
		while(!Timer.IsTimeOut())
		{			
			if(!IsWorking())
				return TRUE;
			Sleep(10);
		}
		return FALSE;
	}
}

BOOL CEasyThread::OnStart()
{
	return TRUE;
}
void CEasyThread::Execute()
{
	while((!IsTerminate())&&(OnRun()))
	{
	}	
}
BOOL CEasyThread::OnRun()
{
	return TRUE;
}
void CEasyThread::OnTerminate()
{
	
}


UINT WINAPI CEasyThread::ThreadProc(LPVOID pParam)
{
	CEasyThread * pThread=(CEasyThread *)pParam;
	if(!pThread->IsTerminate())
	{		
		if(pThread->OnStart())
		{
			pThread->Execute();
		}
	}
	pThread->OnTerminate();
	InterlockedExchange((LONG *)&(pThread->m_IsWorking),FALSE);
	pThread->m_ThreadID=0;	
	pThread->m_hThread=NULL;
	_endthread();	
	return 0;
}