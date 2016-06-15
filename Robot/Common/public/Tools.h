#pragma once
#include <set>
union SERVER_ID
{
	DWORD		ServerID;
	SD_PIPEID	PipeID;
};

inline int GetRand(int Min,int Max)
{
	return rand()%(Max-Min+1)+Min;
}

////拷贝字符串，自动添\0
//inline char * strncpy_0(char *strDest, const char *strSource, size_t count)
//{
//	char * ret=strncpy(strDest,strSource,count-1);
//	strDest[count-1]=0;
//	return ret;
//}
//
//记录日志

inline size_t strnlen(const char * pStr,size_t BufferLen)
{
	size_t Len=0;
	if(pStr)
	{
		while(pStr[Len] &&BufferLen)
		{
			Len++;
			BufferLen--;
		}
	}

	return Len;
}
/*
inline void Log(const char* format, ...)
{
//	SS_SS_TRY_BEGIN;
//	static char pszMsg[10240];
//	va_list args;
//
//	va_start(args, format);
//	_vsnprintf(pszMsg, sizeof(pszMsg), format, args);
//	va_end(args);
//
//	SDErrorInstance()->Log(pszMsg, "Log");	
//	CControlPanel::GetInstance()->PushMsg(pszMsg);
//#ifdef _DEBUG
//	OutputDebugString(pszMsg);
//	OutputDebugString("\r\n");
//#endif
//	SS_TRY_END_INFO("CProcessThread::Log", THROWEXCEPTION);
//	
}

inline void LogDebug(const char* format, ...)
{
//#ifdef _DEBUG
//	SS_SS_TRY_BEGIN;
//
//	static char pszMsg[10240];
//	va_list args;
//
//	va_start(args, format);
//	_vsnprintf(pszMsg, sizeof(pszMsg), format, args);
//	va_end(args);
//
//	SDErrorInstance()->Log(pszMsg, "LogDebug");
//	CControlPanel::GetInstance()->PushMsg(pszMsg);
//	OutputDebugString(pszMsg);
//	OutputDebugString("\r\n");
//	SS_TRY_END_INFO("CProcessThread::LogDebug", THROWEXCEPTION);
//#endif
}


inline void LogUDP(const char* format, ...)
{
//#ifdef UDP_LOG
//	SS_SS_TRY_BEGIN;
//	static char pszMsg[10240];
//	va_list args;
//
//	va_start(args, format);
//	_vsnprintf(pszMsg, sizeof(pszMsg), format, args);
//	va_end(args);
//
//	SDErrorInstance()->Log(pszMsg, "LogUDP");		
//
//	SS_TRY_END_INFO("CProcessThread::Log", THROWEXCEPTION);
//#endif
}

inline void LogUDPDebug(const char* format, ...)
{
//	SS_SS_TRY_BEGIN;
//
//	static char pszMsg[10240];
//	va_list args;
//
//	va_start(args, format);
//	_vsnprintf(pszMsg, sizeof(pszMsg), format, args);
//	va_end(args);
//
//	SDErrorInstance()->Log(pszMsg, "LogUDPDebug");	
//
//	SS_TRY_END_INFO("CProcessThread::LogDebug", THROWEXCEPTION);
}

inline void LogServerInfo(const char* format, ...)
{

	//SS_SS_TRY_BEGIN;

	//static char pszMsg[10240];
	//va_list args;

	//va_start(args, format);
	//_vsnprintf(pszMsg, sizeof(pszMsg), format, args);
	//va_end(args);

	//SDErrorInstance()->Log(pszMsg, "LogServerInfo");	

	//SS_TRY_END_INFO("CProcessThread::LogDebug", THROWEXCEPTION);

}

inline void LogModule(const char* Module,const char* format, ...)
{

	//SS_SS_TRY_BEGIN;

	//static char pszMsg[10240];
	//va_list args;

	//va_start(args, format);
	//_vsnprintf(pszMsg, sizeof(pszMsg), format, args);
	//va_end(args);

	//SDErrorInstance()->Log(pszMsg, Module);	

	//SS_TRY_END_INFO("CProcessThread::LogModule", THROWEXCEPTION);

}

inline void PrintScriptLog(const char* format, ...)
{
	//SS_SS_TRY_BEGIN;

	//static char pszMsg[10240];
	//va_list args;

	//va_start(args, format);
	//_vsnprintf(pszMsg, sizeof(pszMsg), format, args);
	//va_end(args);

	//SDErrorInstance()->Log(pszMsg, "LogScript");
	//CControlPanel::GetInstance()->PushMsg(pszMsg);
	//
	//SS_TRY_END_INFO("CProcessThread::PrintScriptLog", THROWEXCEPTION);
}
*/
#include "..\CorePublic\ProtocolData.h"

//消息头定义
extern		MSGHEADER	g_msgHeader;

//消息包
extern		string		g_strCliPacket;

//设置消息头
inline void SetMsgHeader(WORD wCmd, WORD wRetCode, int nReserved = 0)
{
	SS_TRY_BEGIN_;
	g_msgHeader.wLength		=	sizeof(g_msgHeader);
	g_msgHeader.wCmd		=	wCmd;
	g_msgHeader.wRetCode	=	wRetCode;
	g_msgHeader.nReserved	=	nReserved;

	g_strCliPacket.clear();
	g_strCliPacket.append((char*)&g_msgHeader, sizeof(g_msgHeader));
	SS_TRY_END_();
}
//设置包数据长度
inline void SetMsgLength()
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)g_strCliPacket.c_str();
	pMsgHeader->wLength = (WORD)g_strCliPacket.size();
}

template<typename T>
void ClearOverLapBit(T& n1, T& n2)
{
	n1 ^= n2;
	n2 &= n1;
	n1 ^= n2;
}
uint32	LocalTimeStringToTime(const char* pszLocalTime);

//字符串变为大写
extern void StrUP(string& strSrc);

bool AnsiStrnicmp(const char* pSrcString, const char* pDesString);

struct str_nocase_less
{
	bool operator()(const string& str1, const string& str2) const
	{
		return _mbsicmp((unsigned char*)str1.c_str(),(unsigned char*)str2.c_str())<0;
	}
};

struct WordCompare
	: public less<string>
{	// functor for operator<
	bool operator()(const string& _Left, const string& _Right) const
	{	// apply operator< to operands
		return AnsiStrnicmp(_Left.c_str(), _Right.c_str());
	}
};
typedef set<string, WordCompare> NoCaseWordSet;

extern bool FetchCharInfoEx(TPlayerData* pPlayerData,IDBConnection * pConnection,IDBRecordSet* pRecordSet);
extern bool PackCharInfo(const TPlayerData* pPlayerData,IDBConnection * pConnection);

LPCTSTR GetServerIDString(DWORD ServerID);
int TrimNameString(char* sName, int nMaxLen);
int ClearBlankChar(char* sStr, int nStrLen);
int TrimString(char* sStr, int nLength);
bool SystemTimeToTimet(const SYSTEMTIME& systime, time_t& tim);
bool ConvertTimet(char* sTimeStr, time_t& tim);
void TimetToFileTime( time_t t, LPFILETIME pft );
void TimetToSystemTime(time_t t, SYSTEMTIME& systime);
void TimetToString(time_t t, string& sResultTime);
bool KeyWordFiltrate( char* pszInMsg,int len, const NoCaseWordSet& setBlackWords);
