#include "StdAfx.h"



CServerThread::CServerThread()
{
	SS_TRY_BEGIN;
	m_unMiliSecUpdateCycle = 0;
	m_unCycleCount = 0;
	m_unFreshCycleCount = 0;
	m_TCPRecvBytes=0;
	m_TCPSendBytes=0;
	m_UDPRecvBytes=0;
	m_UDPSendBytes=0;
	m_unNowTime = time(NULL);
	m_unNowMiliTime = m_unLastFreshTime = GetTickCount();
	m_nValidTraceLevel =1;// -1;
	m_nValidDebugLevel =1;// -1;
	m_nValidErrorLevel = 1;
	m_nValidDBLogLevel = 2;
	memset(&m_serverStat, 0, sizeof(m_serverStat));

	m_nValidClientID = 0;
	m_nValidClientLevel = -1;
	
	m_pLogFile = NULL;
	m_pExceptFile = NULL;
	m_pDumpFile = NULL;
	m_pDBLogFile = NULL;
	m_bEnableScreenLog = false;

	SS_TRY_END();
}

CServerThread::~CServerThread(void)
{
	SS_TRY_BEGIN;
	SAFE_DELETE(m_pDBLogFile);
	SAFE_DELETE(m_pLogFile);
	SAFE_DELETE(m_pExceptFile);
	SAFE_DELETE(m_pDumpFile);

	ReleaseExceptionHandle();

	SS_TRY_END();
}

const SERVER_INFO* CServerThread::GetCurStatistic() const
{
	SS_TRY_BEGIN_;
	return &m_serverStat;
	SS_TRY_END_();
	return NULL;
}

void CServerThread::Execute()
{
	SS_TRY_BEGIN_;
	DWORD Time=GetNowMiliTime();
	while((!IsTerminate())&&(OnRun()))
	{
		if(GetNowMiliTime()-Time>1000)
		{
			FlushLogs();
			Time = GetNowMiliTime();
		}
	}	
	OnBeginTerminate();
	Time = timeGetTime();
	while(GetTimeToTime(Time,timeGetTime())<SERVER_ENDING_TIME&&OnTerminating())
	{
	}
	
	SS_TRY_END_();
}


BOOL CServerThread::OnStart()
{
	SS_TRY_BEGIN_;
	m_unMiliSecUpdateCycle = 0;
	m_unCycleCount = 0;
	m_unFreshCycleCount = 0;
	m_TCPRecvBytes=0;
	m_TCPSendBytes=0;
	m_UDPRecvBytes=0;
	m_UDPSendBytes=0;
	m_unNowTime = time(NULL);
	m_unNowMiliTime = m_unLastFreshTime = GetTickCount();
	memset(&m_serverStat, 0, sizeof(m_serverStat));
	TraceLog(5, "Server starts successfully");
	SS_TRY_END_();
	return TRUE;
}

void CServerThread::OnBeginTerminate()
{
}

void CServerThread::OnTerminate()
{
	SS_TRY_BEGIN_;	
	TraceLog(5, "Sever closed");
	SS_TRY_END_();
}

BOOL CServerThread::OnRun()
{
	SS_TRY_BEGIN_;		
	//计算服务器循环时间
	++m_unMiliSecUpdateCycle;
	++m_unCycleCount;
	++m_unFreshCycleCount;
	//if(m_unMiliSecUpdateCycle >= 20)
	//{
		m_unNowMiliTime = GetTickCount();
		//m_unMiliSecUpdateCycle = 0;
	//}

	if( m_unCycleCount >= 40 )
	{
		m_unNowTime = time(NULL);
		m_unCycleCount = 0;
			//执行控制台命令
		PANEL_MSG * pCommand=CControlPanel::GetInstance()->GetCommand();
		if(pCommand)
		{
			ExecCommand(pCommand->Msg);
			CControlPanel::GetInstance()->ReleaseCommand(pCommand->ID);
		}

	}
	
	uint32 tempMiliTime =m_unNowMiliTime - m_unLastFreshTime;
	if(tempMiliTime >=  SERVER_INFO_COUNT_TIME && m_unFreshCycleCount > 0)
	{	
		m_unLastFreshTime = m_unNowMiliTime;
		m_serverStat.ClientCount=GetClientCount();
		m_serverStat.CycleTime=(float)tempMiliTime/m_unFreshCycleCount;
		m_serverStat.TCPRecvFlow=(float)m_TCPRecvBytes*(1000.0f/1024/SERVER_INFO_COUNT_TIME);
		m_serverStat.TCPSendFlow=(float)m_TCPSendBytes*(1000.0f/1024/SERVER_INFO_COUNT_TIME);
		m_serverStat.UDPRecvFlow=(float)m_UDPRecvBytes*(1000.0f/1024/SERVER_INFO_COUNT_TIME);
		m_serverStat.UDPSendFlow=(float)m_UDPSendBytes*(1000.0f/1024/SERVER_INFO_COUNT_TIME);
		CControlPanel::GetInstance()->SetServerInfo(m_serverStat);
		/*
		LogServerInfo("CycleTime=%06.6g,TCPRecvFlow=%06.6g,TCPSendFlow=%06.6g,UDPRecvFlow=%06.6g,UDPSendFlow=%06.6g,ClientCount=%04d",
			ServerInfo.CycleTime,
			ServerInfo.TCPRecvFlow,
			ServerInfo.TCPSendFlow,
			ServerInfo.UDPRecvFlow,
			ServerInfo.UDPSendFlow,
			ServerInfo.ClientCount);
		*/
		m_unFreshCycleCount = 0;
		m_TCPRecvBytes=0;
		m_TCPSendBytes=0;
		m_UDPRecvBytes=0;
		m_UDPSendBytes=0;
	}


	return TRUE;
	SS_TRY_END_();
	return TRUE;
}

BOOL CServerThread::OnTerminating()
{
	return FALSE;
}

//执行控制台命令
void CServerThread::ExecCommand(LPCTSTR Command)
{
}

int CServerThread::GetClientCount()
{
	return 0;
}

BOOL CServerThread::InitLogFiles(char* sPdbPath, UINT nVilidLogTypes)
{
	try
	{
		if(!InitExceptionHandle(sPdbPath))
			return FALSE;

		LogSplitPolicy policy = {0};
		m_pDumpFile = new CLogFile;
		if(!m_pDumpFile ||  !m_pDumpFile->Initialize(".\\Except", "ExceptDump", "dmp", policy))
		{
			OutputDebugString("Initialize DmpLogFile Failed!");
			return FALSE;
		}

		policy.nSplitType = logsplit_bysize;
		policy.nSetting = 3;
		m_pExceptFile = new CLogFile;
		if(!m_pExceptFile ||  !m_pExceptFile->Initialize(".\\Except", "Exception", "log", policy))
		{
			OutputDebugString("Initialize ExceptLogFile Failed!");
			return FALSE;
		}

		policy.nSplitType = logsplit_bytime;
		policy.nSetting = 6;
		m_pLogFile = new CLogFile;
		if(!m_pLogFile ||  !m_pLogFile->Initialize(".\\Log", "Log", "log", policy))
		{
			OutputDebugString("Initialize LogFile Failed!");
			return FALSE;
		}

		if(nVilidLogTypes & LogType_DB)
		{
			m_pDBLogFile = new CLogFile;
			if(!m_pDBLogFile || !m_pDBLogFile->Initialize(".\\Log", "DBAccess", "log", policy))
			{
				OutputDebugString("Initialize DBLogFile Failed!");
				return FALSE;
			}
		}

		return TRUE;
	}
	catch(...){}
	return FALSE;
}

void CServerThread::FlushLogs()
{
	if(m_pLogFile)m_pLogFile->FlushLog();
	if(m_pExceptFile)m_pExceptFile->FlushLog();
	if(m_pDumpFile)m_pDumpFile->FlushLog();
	if(m_pDBLogFile)m_pDBLogFile->FlushLog();
}
