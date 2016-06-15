/******************************************************************************
* Copyright (c) 2012,�Ϻ������з�����
* All rights reserved.
* 
* �ļ����ƣ�LogUtil.h
* ժ    Ҫ����Ϸ��־������쳣��ջ��׽
* 
* ��ǰ�汾��1.0
* ��    �ߣ�sunfei
* �������ڣ�2007-10-22
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
	logsplit_none,		//������log�ļ�
	logsplit_bysize,	//�����ļ���С�����ļ�(MB)
	logsplit_bytime,	//����ʱ������ļ�(Hour)
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
	//��ʼ��Log����
	BOOL Initialize(const char* sDirectory, const char* sFilePrefix, const char* sFileExt, const LogSplitPolicy& policy);
	//�ļ�����Ŀ¼
	const char* GetDirectory(){	return m_sDirectory;}
	//��ȡ��ǰLog�ļ���
	const char* GetFileName(){return m_sCurFileName;}
	//��ȡ��ǰLog�ļ����
	HANDLE	GetFileHandle();

	//���log
	void OutputLog(const char* sType, int nLevel, const char* sFmt, va_list va_ptr);
	//���log
	void OutputLog(const char* sType, int nLevel, const char* sFmt, ...);
	//���log
	void OutputLog(const char* sType, int nLevel, CException* pExc, const char* sFmt, ...);

	void FlushLog();
private:
	void ShiftFile();
	BOOL OpenLogFile();
private:
	char	m_sDirectory[128];	//log�ļ�Ŀ¼
	char	m_sFilePrefix[32];	//log�ļ���ǰ׺
	char	m_sFileExtension[8];	//log�ļ���չ��
	char	m_sCurFileName[128];	//��ǰlog�ļ���
	LogSplitPolicy m_splitPolicy;	//log�ļ����ѹ���
	char	m_sLogBuffer[LOGUTIL_BUF_LEN];	//log���ݻ�����
	int		m_nNextFileID;		//��һ��log�ļ���ID
	long	m_nCurFileSize;		//��ǰlog�ļ��Ĵ�С
	time_t	m_lastSplitTime;	//�ϴη���Log�ļ���ʱ��
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

