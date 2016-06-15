/******************************************************************************
* Copyright (c) 2012,上海好玩研发中心
* All rights reserved.
* 
* 文件名称：LogUtil.h
* 摘    要：游戏日志输出及异常堆栈捕捉
* 
* 当前版本：1.0
* 作    者：sunfei
* 建立日期：2007-10-22
**********************************************************************************/
#pragma once
#define LOGUTIL_BUF_LEN		10240

BOOL InitExceptionHandle(char* sPdbPath);
void ReleaseExceptionHandle();

void WINAPI ScreenLog(const char* sFmt, ... );

void WINAPI ClientLog(UINT nCharaID, int nLevel, const char* sFmt, ... );
void WINAPI DBLog( int nLevel, const char* sFmt, ... );
void WINAPI DebugLog( int nLevel, const char* sFmt, ...);
void WINAPI TraceLog( int nLevel, const char* sFmt, ... );
void WINAPI ErrorLog( int nLevel, const char* sFmt, ... );
void WINAPI Log( const char* sFmt, ... );
void WINAPI ExceptLog( const char* sRaiseFun, const char* sRaiseFileName, long nRaiseLine, const char* sFmt, ...);
void WINAPI ExceptLog( const char* sRaiseFun, const char* sRaiseFileName, long nRaiseLine, CException* pe);
long WINAPI ExceptLog(EXCEPTION_POINTERS* pe);

enum LogType
{
	LogType_None	= 0x00,
	LogType_DB		= 0x01,
	LogType_Debug	= 0x02,
	LogType_Trace	= 0x04,
	LogType_Error	= 0x08,
	LogType_Except	= 0x10,
};

enum LogSplitType
{
	logsplit_none,		//不分裂log文件
	logsplit_bysize,	//按照文件大小分裂文件(MB)
	logsplit_bytime,	//按照时间分裂文件(Hour)
};

struct LogSplitPolicy
{
	long nSplitType;	//enum LogSplitType
	long nSetting;		//
};

class CLogFile 
{
public:
	CLogFile();
	virtual ~CLogFile();

public:
	//初始化Log对象
	BOOL Initialize(const char* sDirectory, const char* sFilePrefix, const char* sFileExt, const LogSplitPolicy& policy);
	//文件所在目录
	const char* GetDirectory(){	return m_sDirectory;}
	//获取当前Log文件名
	const char* GetFileName(){return m_sCurFileName;}
	//获取当前Log文件句柄
	HANDLE	GetFileHandle();

	//输出log
	void OutputLog(const char* sType, int nLevel, const char* sFmt, va_list va_ptr);
	//输出log
	void OutputLog(const char* sType, int nLevel, const char* sFmt, ...);
	//输出log
	void OutputLog(const char* sType, int nLevel, CException* pExc, const char* sFmt, ...);

	void FlushLog();
private:
	void ShiftFile();
	BOOL OpenLogFile();
private:
	char	m_sDirectory[128];	//log文件目录
	char	m_sFilePrefix[32];	//log文件名前缀
	char	m_sFileExtension[8];	//log文件扩展名
	char	m_sCurFileName[128];	//当前log文件名
	LogSplitPolicy m_splitPolicy;	//log文件分裂规则
	char	m_sLogBuffer[LOGUTIL_BUF_LEN];	//log内容缓冲区
	int		m_nNextFileID;		//下一个log文件的ID
	long	m_nCurFileSize;		//当前log文件的大小
	time_t	m_lastSplitTime;	//上次分裂Log文件的时间
	SYSTEMTIME m_tmInitTime;
	CRITICAL_SECTION m_csLogLock;
	FILE*	m_pLogFile;
};

#ifndef SIZEOF_STR
#define SIZEOF_STR(str) sizeof(str)-1
#endif //SIZEOF_STR


inline void CLogFile::OutputLog(const char* sType, int nLevel, const char* sFmt, ...)
{
	va_list va_ptr;
	va_start( va_ptr, sFmt );
	OutputLog(sType, nLevel, sFmt, va_ptr);
	va_end( va_ptr );
}

inline void CLogFile::FlushLog()
{
	EnterCriticalSection(&m_csLogLock);
	if(m_pLogFile)
	{
		fflush(m_pLogFile);
	}
	LeaveCriticalSection(&m_csLogLock);
}

#define SS_TRY_BEGIN	try {
#define SS_TRY_END()		}	\
    catch(exception* pe) \
    {\
		ExceptLog(__FUNCTION__, __FILE__, __LINE__, "std::except detail=%s", pe->what()); \
    }\
	catch(CException* pMfcE)\
	{\
		ExceptLog(__FUNCTION__, __FILE__, __LINE__, pMfcE); \
	}

#define SS_TRY_BEGIN_	__try {
#define SS_TRY_END_()		}	\
	__except(ExceptLog(GetExceptionInformation())) \
	{\
		ExceptLog(__FUNCTION__, __FILE__, __LINE__, "Exception_");\
	}

