#pragma once

#define SERVER_ENDING_TIME	3*60*1000

class CServerThread : public CEasyThread
{
protected:
	uint32						m_unMiliSecUpdateCycle;
	uint32						m_unCycleCount;
	uint32						m_unFreshCycleCount;
	UINT						m_TCPRecvBytes;
	UINT						m_TCPSendBytes;
	UINT						m_UDPRecvBytes;
	UINT						m_UDPSendBytes;
	uint32					m_unNowTime;
	uint32					m_unLastFreshTime;

	uint32					m_unNowMiliTime;
	SERVER_INFO				m_serverStat;//服务器统计信息
protected:
	// log relative
	int							m_nValidTraceLevel;
	int							m_nValidDebugLevel;
	int							m_nValidErrorLevel;
	int							m_nValidDBLogLevel;

	UINT						m_nValidClientID;
	int							m_nValidClientLevel;
	
	bool						m_bEnableScreenLog;

	CLogFile*					m_pLogFile;
	CLogFile*					m_pExceptFile;
	CLogFile*					m_pDumpFile;
	CLogFile*					m_pDBLogFile;

public:
	CServerThread();
	virtual ~CServerThread(void);
	
public:	
	void AddTCPRecvBytes(int Count)
	{
		m_TCPRecvBytes+=Count;
	}
	void AddTCPSendBytes(int Count)
	{
		m_TCPSendBytes+=Count;
	}
	void AddUDPRecvBytes(int Count)
	{
		m_UDPRecvBytes+=Count;
	}
	void AddUDPSendBytes(int Count)
	{
		m_UDPSendBytes+=Count;
	}

	inline uint32 GetNowTime() const
	{
		return m_unNowTime;
	}

	inline uint32 GetNowMiliTime() const
	{
		return m_unNowMiliTime;
	}
	
	//log relatvie
	BOOL InitLogFiles(char* sPdbPath, UINT nVilidLogTypes);
	BOOL IsValidLog(int nLogType, int nLevel);
	
	BOOL IsLogClient(UINT nCharaID, int nLevel);
	BOOL NeedShowScreenLog(){return m_bEnableScreenLog;}
	CLogFile* GetLogFile(){ return m_pLogFile;}
	CLogFile* GetExceptFile(){ return m_pExceptFile;}
	CLogFile* GetDumpFile(){ return m_pDumpFile;}
	CLogFile* GetDBLogFile(){ return m_pDBLogFile;}
	void FlushLogs();
	const SERVER_INFO* GetCurStatistic() const;
protected:

	virtual void Execute();
	
	virtual BOOL OnRun();
	virtual BOOL OnTerminating();

	virtual BOOL OnStart();
	virtual void OnBeginTerminate();
	virtual void OnTerminate();

	//执行控制台命令
	virtual void ExecCommand(LPCTSTR Command);

	virtual int GetClientCount();
};

inline BOOL CServerThread::IsValidLog(int nLogType, int nLevel)
{
	BOOL bValid = FALSE;
	switch(nLogType)
	{
	case LogType_Debug:
		bValid = m_nValidDebugLevel>0 && nLevel>=m_nValidDebugLevel;
		break;
	case LogType_Trace:
		bValid = m_nValidTraceLevel>0 && nLevel>=m_nValidTraceLevel;
		break;
	case LogType_DB:
		bValid = m_nValidDBLogLevel>0 && nLevel>=m_nValidDBLogLevel;
		break;
	case LogType_Error:
		bValid = m_nValidErrorLevel>0 && nLevel>=m_nValidErrorLevel;
		break;
	case LogType_Except:
		bValid = TRUE;
		break;
	}
	return bValid;
}

inline BOOL CServerThread::IsLogClient(UINT nCharaID, int nLevel)
{
	return m_nValidClientID==nCharaID && m_nValidDebugLevel>0 && nLevel>=m_nValidClientLevel;
}
