/******************************************************************************
* Copyright (c) 2012,上海好玩研发中心
* All rights reserved.
* 
* 文件名称：LogUtil.cpp
* 摘    要：游戏日志输出及异常堆栈捕捉
* 
* 当前版本：1.0
* 作    者：sunfei
* 建立日期：2007-10-22
**********************************************************************************/
#include "StdAfx.h"
#include ".\logutil.h"
#include <io.h>
#include <time.h>
#include <dbghelp.h>
#define LOG_HEADER	"--------------------------------------------------------\n"\
					"                     LOG        START\n"\
					"--------------------------------------------------------\n\n"
#define LOG_END		"\n--------------------------------------------------------\n"\
					"                     LOG         END\n"\
					"--------------------------------------------------------\n"
CLogFile* GetExceptFile();
CLogFile* GetDumpFile();
CLogFile* GetLogFile();
CLogFile* GetDBLogFile();
BOOL IsValidLog(int nLogType, int nLevel);

BOOL IsLogClient(UINT nCharaID, int nLevel);
BOOL NeedShowScreenLog();
void PrintUIMsg(const char* sMsgText, int nMsgLength);
CRITICAL_SECTION g_csPDBLock;

CLogFile::CLogFile()
{
	__try
	{
		memset(m_sDirectory, 0, sizeof(m_sDirectory));
		memset(m_sFilePrefix, 0, sizeof(m_sFilePrefix));
		memset(m_sFileExtension, 0, sizeof(m_sFileExtension));
		memset(m_sCurFileName, 0, sizeof(m_sCurFileName));
		memset(&m_splitPolicy, 0, sizeof(m_splitPolicy));
		memset(m_sLogBuffer, 0, sizeof(m_sLogBuffer));
		memset(&m_tmInitTime, 0, sizeof(m_tmInitTime));
		m_nNextFileID = 1;
		m_nCurFileSize = 0;
		m_lastSplitTime = 0;
		m_pLogFile = NULL;
		InitializeCriticalSection(&m_csLogLock);
	}
	__except(EXCEPTION_EXECUTE_HANDLER){}
}

CLogFile::~CLogFile()
{
	__try
	{
		DeleteCriticalSection(&m_csLogLock);
		if(m_pLogFile)
		{
			if(stricmp(m_sFileExtension, "txt")==0||stricmp(m_sFileExtension, "log")==0)
			{
				fwrite(LOG_END, sizeof(LOG_END)-1, 1, m_pLogFile);
				m_nCurFileSize += sizeof(LOG_END)-1;
			}
			fclose(m_pLogFile);
			m_pLogFile = NULL;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){}
}

//初始化Log对象
BOOL CLogFile::Initialize(const char* sDirectory, const char* sFilePrefix, const char* sFileExt, const LogSplitPolicy& policy)
{
	__try
	{
		_snprintf(m_sDirectory, SIZEOF_STR(m_sDirectory), sDirectory);
		_snprintf(m_sFilePrefix, SIZEOF_STR(m_sFilePrefix), sFilePrefix);
		_snprintf(m_sFileExtension, SIZEOF_STR(m_sFileExtension), sFileExt);
		memcpy(&m_splitPolicy, &policy, sizeof(m_splitPolicy));
		memset(&m_lastSplitTime, 0, sizeof(m_lastSplitTime));
		m_nNextFileID = 1;
		m_nCurFileSize = 0;
		//create log directory
		CreateDirectory(sDirectory, NULL);
		if((GetFileAttributes(sDirectory)&FILE_ATTRIBUTE_DIRECTORY)!=FILE_ATTRIBUTE_DIRECTORY)
		{
			return FALSE;
		}

		if(m_pLogFile)
		{
			if(stricmp(m_sFileExtension, "txt")==0||stricmp(m_sFileExtension, "log")==0)
			{
				fwrite(LOG_END, sizeof(LOG_END)-1, 1, m_pLogFile);
				m_nCurFileSize += sizeof(LOG_END)-1;
			}
			fclose(m_pLogFile);
			m_pLogFile = NULL;
			m_nCurFileSize = 0;

		}

		GetLocalTime(&m_tmInitTime);

		switch(m_splitPolicy.nSplitType)
		{
		case logsplit_bysize:
			if(m_splitPolicy.nSetting<=0)
			{
				m_splitPolicy.nSetting = 0x200000;//byte
			}
			else
			{
				m_splitPolicy.nSetting <<= 20;//byte
			}
			break;
		case logsplit_bytime:
			if(m_splitPolicy.nSetting<=0)
			{
				m_splitPolicy.nSetting = 6;//Hours
			}
			else if(m_splitPolicy.nSetting>=24)
			{
				m_splitPolicy.nSetting = 24;//Hours
			}
			m_splitPolicy.nSetting *=3600;//Seconds
			break;
		default:
			m_splitPolicy.nSetting = 0;
			OpenLogFile();
			break;
		}

		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER){}
	return FALSE;
}

//获取当前Log文件句柄
HANDLE	CLogFile::GetFileHandle()
{
	__try
	{
		if(m_pLogFile)
			return (HANDLE)_get_osfhandle(_fileno(m_pLogFile));
	}
	__except(EXCEPTION_EXECUTE_HANDLER){}
	return NULL;
}

void CLogFile::ShiftFile()
{
	__try
	{
		if(!m_pLogFile)
		{
			if( strlen(m_sFilePrefix) )
				OpenLogFile();
		}
		else
		{
			switch(m_splitPolicy.nSplitType)
			{
			case logsplit_bysize:
				if(m_nCurFileSize>=m_splitPolicy.nSetting)
				{
					OpenLogFile();
				}
				break;
			case logsplit_bytime:
				{
					time_t tCutTime = time(NULL);				
					if(tCutTime - m_lastSplitTime>=m_splitPolicy.nSetting || m_nCurFileSize>=0x400000)
					{
						OpenLogFile();
					}
				}
				break;
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){}
}

BOOL CLogFile::OpenLogFile()
{
	__try
	{
		if(m_pLogFile)
		{
			if(stricmp(m_sFileExtension, "txt")==0||stricmp(m_sFileExtension, "log")==0)
			{
				fwrite(LOG_END, sizeof(LOG_END)-1, 1, m_pLogFile);
				m_nCurFileSize += sizeof(LOG_END)-1;
			}
			fclose(m_pLogFile);
			m_pLogFile = NULL;
			m_nCurFileSize = 0;
		}
		switch(m_splitPolicy.nSplitType)
		{
		case logsplit_none:
			_snprintf(m_sCurFileName, SIZEOF_STR(m_sCurFileName), "%s-%04u%02u%02u-%02u%02u%02u.%s"
				, m_sFilePrefix
				, m_tmInitTime.wYear, m_tmInitTime.wMonth, m_tmInitTime.wDay, m_tmInitTime.wHour, m_tmInitTime.wMinute, m_tmInitTime.wSecond
				, m_sFileExtension
				);
			m_lastSplitTime = time(NULL);
			break;
		case logsplit_bysize:
		case logsplit_bytime:
			_snprintf(m_sCurFileName, SIZEOF_STR(m_sCurFileName), "%s-%04u%02u%02u-%02u%02u%02u-%d.%s"
				, m_sFilePrefix
				, m_tmInitTime.wYear, m_tmInitTime.wMonth, m_tmInitTime.wDay, m_tmInitTime.wHour, m_tmInitTime.wMinute, m_tmInitTime.wSecond
				, m_nNextFileID++
				, m_sFileExtension
				);
			m_lastSplitTime = time(NULL);
			break;
		}
		static char sLogPath[MAX_PATH] = "";
		if(strlen(m_sDirectory))
		{
			_snprintf(sLogPath, SIZEOF_STR(sLogPath), "%s\\%s", m_sDirectory, m_sCurFileName);
		}
		else
		{
			_snprintf(sLogPath, SIZEOF_STR(sLogPath), ".\\%s", m_sCurFileName);
		}

		if(stricmp(m_sFileExtension, "txt")==0||stricmp(m_sFileExtension, "log")==0)
		{
			m_pLogFile = fopen(sLogPath, "w");
			if(m_pLogFile)
			{
				fwrite(LOG_HEADER, sizeof(LOG_HEADER)-1, 1, m_pLogFile);
				m_nCurFileSize += sizeof(LOG_HEADER)-1;
			}
		}
		else
			m_pLogFile = fopen(sLogPath, "wb");

		return m_pLogFile!=NULL;
	}
	__except(EXCEPTION_EXECUTE_HANDLER){}
	return FALSE;
}


//输出log
void CLogFile::OutputLog(const char* sType, int nLevel, const char* sFmt, va_list va_ptr)
{
	__try
	{
		EnterCriticalSection(&m_csLogLock);

		ShiftFile();

		static SYSTEMTIME stm = {0};
		GetLocalTime(&stm);
		int nLogLen = _snprintf(m_sLogBuffer, SIZEOF_STR(m_sLogBuffer), "%u-%02u-%02u %02u:%02u:%02u.%03u [%s][%d]\t"
			, stm.wYear, stm.wMonth, stm.wDay, stm.wHour, stm.wMinute, stm.wSecond, stm.wMilliseconds
			, sType, nLevel);
		nLogLen += _vsnprintf(m_sLogBuffer+nLogLen, SIZEOF_STR(m_sLogBuffer)-nLogLen, sFmt, va_ptr);
		if(NeedShowScreenLog())
		{
			//CControlPanel::GetInstance()->PushMsg(m_sLogBuffer);
			//CControlPanel::GetInstance()->PushMsg(CON_MSG_LOG, m_sLogBuffer, nLogLen+1);
			PrintUIMsg(m_sLogBuffer, nLogLen);
		}
		nLogLen += _snprintf(m_sLogBuffer+nLogLen, SIZEOF_STR(m_sLogBuffer)-nLogLen, "\n");


		if(m_pLogFile)
		{
			 fwrite(m_sLogBuffer, nLogLen, 1, m_pLogFile);
			 m_nCurFileSize += nLogLen;

	#ifdef _DEBUG
			OutputDebugString(m_sLogBuffer);
	#endif
		}

		LeaveCriticalSection(&m_csLogLock);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LeaveCriticalSection(&m_csLogLock);
	}
}

void CLogFile::OutputLog(const char* sType, int nLevel, CException* pExc, const char* sFmt, ...)
{
	__try
	{
		EnterCriticalSection(&m_csLogLock);

		va_list va_ptr;
		va_start( va_ptr, sFmt );

		ShiftFile();

		static SYSTEMTIME stm = {0};
		GetLocalTime(&stm);
		int nLogLen = _snprintf(m_sLogBuffer, SIZEOF_STR(m_sLogBuffer), "%u-%02u-%02u %02u:%02u:%02u.%03u [%s][%d]\t"
			, stm.wYear, stm.wMonth, stm.wDay, stm.wHour, stm.wMinute, stm.wSecond, stm.wMilliseconds
			, sType, nLevel);
		nLogLen += _vsnprintf(m_sLogBuffer+nLogLen, SIZEOF_STR(m_sLogBuffer)-nLogLen, sFmt, va_ptr);
		va_end( va_ptr );

		pExc->GetErrorMessage(m_sLogBuffer+nLogLen,  SIZEOF_STR(m_sLogBuffer)-nLogLen);
		nLogLen+=strlen(m_sLogBuffer+nLogLen);
		if(NeedShowScreenLog())
		{
			//CControlPanel::GetInstance()->PushMsg(m_sLogBuffer);
			//CControlPanel::GetInstance()->PushMsg(CON_MSG_LOG, m_sLogBuffer, nLogLen+1);
			PrintUIMsg(m_sLogBuffer, nLogLen);
		}
		nLogLen += _snprintf(m_sLogBuffer+nLogLen, SIZEOF_STR(m_sLogBuffer)-nLogLen, "\n");
		if(m_pLogFile)
		{
			fwrite(m_sLogBuffer, nLogLen, 1, m_pLogFile);
			 m_nCurFileSize += nLogLen;

	#ifdef _DEBUG
			OutputDebugString(m_sLogBuffer);
	#endif
		}
		LeaveCriticalSection(&m_csLogLock);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LeaveCriticalSection(&m_csLogLock);
	}
}

BOOL InitExceptionHandle(char* sPdbPath)
{
	__try
	{
		InitializeCriticalSection(&g_csPDBLock);
		SetUnhandledExceptionFilter(ExceptLog);
		SymSetOptions(SymGetOptions() | SYMOPT_UNDNAME | SYMOPT_LOAD_LINES);
		HANDLE hProcess = GetCurrentProcess();
		return SymInitialize(hProcess, sPdbPath, TRUE);
	}
	__except(EXCEPTION_EXECUTE_HANDLER){}
	return FALSE;
}

void ReleaseExceptionHandle()
{
	__try
	{
		DeleteCriticalSection(&g_csPDBLock);
	}
	__except(EXCEPTION_EXECUTE_HANDLER){}
}

void WINAPI TraceCallStack(PCONTEXT pCxt, DWORD dwLevel, CLogFile* pLogFile)
{
	__try
	{

		if(!pCxt || !pLogFile) return;
		if (dwLevel < 2)
			dwLevel = 2;

		HANDLE hProcess = GetCurrentProcess();

		STACKFRAME stFrame = {0};
		const DWORD dwMachine = IMAGE_FILE_MACHINE_I386 ;
		stFrame.AddrPC.Offset    = pCxt->Eip;
		stFrame.AddrPC.Mode = AddrModeFlat;
		stFrame.AddrFrame.Offset = pCxt->Ebp;
		stFrame.AddrFrame.Mode   = AddrModeFlat;

		IMAGEHLP_MODULE ihModule = {sizeof(IMAGEHLP_MODULE)};
		IMAGEHLP_LINE	ihLine = {sizeof(IMAGEHLP_LINE)};
		static BYTE byInfo[512] = {0};
		PSYMBOL_INFO pSymInfo = (PSYMBOL_INFO)byInfo;
		pSymInfo->SizeOfStruct = sizeof(SYMBOL_INFO);
		pSymInfo->MaxNameLen = sizeof(byInfo) - sizeof(SYMBOL_INFO) + 1;
		static char sAddrDesc[1024] = "";
		int nDescLen = _snprintf(sAddrDesc, SIZEOF_STR(sAddrDesc), "call stack:");
		//fwrite(sAddrDesc, nDescLen, 1, pf);
		if(pLogFile)pLogFile->OutputLog("exception", 1, sAddrDesc);


		for ( DWORD i = 0 ; i <= dwLevel ; i++ )
		{
			if (StackWalk(dwMachine,hProcess,hProcess, &stFrame, pCxt, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL))
			{
				nDescLen = 0;
				PFPO_DATA pFpo = (PFPO_DATA)stFrame.FuncTableEntry;
				// Also check that the address is not zero.  Sometimes StackWalk returns TRUE with a frame of zero.
				if (0 != stFrame.AddrPC.Offset)
				{
					DWORD dwPlacement = 0;
					DWORD64 dwPlacement64 = 0;
					DWORD* pParams = stFrame.Params;
					DWORD dwAddr = stFrame.AddrPC.Offset;

					//module info
					if(SymGetModuleInfo(hProcess, dwAddr, &ihModule))
					{
						nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "0x%08X:0x%08X %s"
							, ihModule.BaseOfImage
							, dwAddr - ihModule.BaseOfImage
							, ihModule.ImageName);
					}
					else
					{
						nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "0x%08X <unknown module>"
							, dwAddr);
					}

					//line info
					if(SymGetLineFromAddr(hProcess, dwAddr, &dwPlacement, &ihLine))
					{
						nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "[placement:%u][%s:%u]\t"
							, dwPlacement
							, ihLine.FileName
							, ihLine.LineNumber
							);
					}
					else
					{
						nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "\t");
					}

					//sym info
					if(SymFromAddr(hProcess, dwAddr, &dwPlacement64, pSymInfo))
					{
						nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "function=%s(%u, %u, %u, %u)=(0x%08X,0x%08X,0x%08X,0x%08X), symflag=0x%08X{"
							, pSymInfo->Name
							, pParams[0], pParams[1], pParams[2], pParams[3]
							, pParams[0], pParams[1], pParams[2], pParams[3]
							, pSymInfo->Flags
							);
						if(pSymInfo->Flags)
						{
							if(pSymInfo->Flags & SYMFLAG_CONSTANT)
							{
								nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "(constant, value=%I64u)"
									, pSymInfo->Value);
							}
							if(pSymInfo->Flags & SYMFLAG_EXPORT)
							{
								nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "(export_table)");
							}
							if(pSymInfo->Flags & SYMFLAG_FORWARDER)
							{
								nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "(forwarder)");
							}
							if(pSymInfo->Flags & SYMFLAG_FRAMEREL)
							{
								nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "(frame_relative)");
							}
							if(pSymInfo->Flags & SYMFLAG_FUNCTION)
							{
								nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "(function)");
							}
							if(pSymInfo->Flags & SYMFLAG_LOCAL)
							{
								nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "(local_variable)");
							}
							if(pSymInfo->Flags & SYMFLAG_PARAMETER)
							{
								nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "(parameter)");
							}
							if(pSymInfo->Flags & SYMFLAG_REGISTER)
							{
								nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "(register, register=%u)"
									, pSymInfo->Register
									);
							}
							if(pSymInfo->Flags & SYMFLAG_REGREL)
							{
								nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "(register_relative)");
							}
							if(pSymInfo->Flags & SYMFLAG_THUNK)
							{
								nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "(thunk)");
							}
							if(pSymInfo->Flags & SYMFLAG_TLSREL)
							{
								nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "(tls_relative)");
							}
							if(pSymInfo->Flags & SYMFLAG_VALUEPRESENT)
							{
								nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "(valuepresent, value=%I64u)"
									, pSymInfo->Value
									);
							}
							if(pSymInfo->Flags & SYMFLAG_VIRTUAL)
							{
								nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "(virtual_symbol, addr=0x%016X)"
									, pSymInfo->Value
									);
							}
							nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "}"
								);
						}
						else
						{
							nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "}, value=%I64u"
								, pSymInfo->Value
								);
						}

						//nDescLen += _snprintf(sAddrDesc+nDescLen,SIZEOF_STR(sAddrDesc)-nDescLen, "\n");
						if(pLogFile)pLogFile->OutputLog("exception", 1, sAddrDesc);

					}
				}
			}
			else if(dwLevel>1)
			{
				break;
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){}
}

void WINAPI DumpException(EXCEPTION_POINTERS* pe, HANDLE hDumpFile)
{
	__try
	{
		if(hDumpFile==NULL || hDumpFile == INVALID_HANDLE_VALUE )
			return;

		static BYTE bySteam[2048] = {0};
		MINIDUMP_EXCEPTION_INFORMATION mdExceptionInfo = {0};
		mdExceptionInfo.ClientPointers = FALSE;
		mdExceptionInfo.ExceptionPointers = pe;
		mdExceptionInfo.ThreadId = GetCurrentThreadId();

		MINIDUMP_USER_STREAM_INFORMATION mdStreamInfo = {0};
		mdStreamInfo.UserStreamCount = 3;
		mdStreamInfo.UserStreamArray = (PMINIDUMP_USER_STREAM)bySteam;
		ASSERT(sizeof(bySteam)>=sizeof(MINIDUMP_USER_STREAM)*mdStreamInfo.UserStreamCount);
		PBYTE pStream = bySteam+sizeof(MINIDUMP_USER_STREAM)*mdStreamInfo.UserStreamCount;

		mdStreamInfo.UserStreamArray[0].Type = ThreadExListStream;
		PMINIDUMP_THREAD_EX_LIST pThreadList = (PMINIDUMP_THREAD_EX_LIST)pStream;
		memset(pThreadList, 0, sizeof(MINIDUMP_THREAD_EX_LIST));
		pThreadList->NumberOfThreads = 10;
		mdStreamInfo.UserStreamArray[0].Buffer = pThreadList;
		mdStreamInfo.UserStreamArray[0].BufferSize = sizeof(MINIDUMP_THREAD_EX_LIST)+sizeof(MINIDUMP_THREAD_EX)*pThreadList->NumberOfThreads;
		pStream += sizeof(MINIDUMP_THREAD_EX_LIST)+sizeof(MINIDUMP_THREAD_EX)*pThreadList->NumberOfThreads;

		mdStreamInfo.UserStreamArray[1].Type = MemoryListStream;
		PMINIDUMP_MEMORY_LIST pMemList = (PMINIDUMP_MEMORY_LIST)pStream;
		memset(pMemList, 0, sizeof(MINIDUMP_MEMORY_LIST));
		pMemList->NumberOfMemoryRanges = 5;
		mdStreamInfo.UserStreamArray[1].Buffer = pMemList;
		mdStreamInfo.UserStreamArray[1].BufferSize = sizeof(MINIDUMP_MEMORY_LIST)+sizeof(MINIDUMP_MEMORY_DESCRIPTOR)*pMemList->NumberOfMemoryRanges;
		pStream += sizeof(MINIDUMP_MEMORY_LIST)+sizeof(MINIDUMP_MEMORY_DESCRIPTOR)*pMemList->NumberOfMemoryRanges;

		mdStreamInfo.UserStreamArray[2].Type = ExceptionStream;
		PMINIDUMP_EXCEPTION_STREAM pExceptStream = (PMINIDUMP_EXCEPTION_STREAM)pStream;
		pExceptStream->ThreadId = GetCurrentThreadId();
		mdStreamInfo.UserStreamArray[2].Buffer = pExceptStream;
		mdStreamInfo.UserStreamArray[2].BufferSize = sizeof(MINIDUMP_EXCEPTION_STREAM);
		pStream += sizeof(MINIDUMP_EXCEPTION_STREAM);

		MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &mdExceptionInfo, &mdStreamInfo, NULL);
		ASSERT(pStream<=bySteam+sizeof(bySteam));
	}
	__except(EXCEPTION_EXECUTE_HANDLER){}

}

long WINAPI ExceptLog(EXCEPTION_POINTERS* pe)
{
	__try
	{
		EnterCriticalSection(&g_csPDBLock);
		PEXCEPTION_RECORD pExc = pe->ExceptionRecord;
		PCONTEXT pCxt = pe->ContextRecord;
		long nRet = EXCEPTION_EXECUTE_HANDLER;
		static char sExceptionDesc[1024] = "";
		bool bNeedDump = true;
		//expction information
		int nDescLen = _snprintf(sExceptionDesc, SIZEOF_STR(sExceptionDesc), "excepton is catched:\n"
			"------------------------------------------\n"
			//"catchpos=%s:%d\n"		
			//"exception="
			//, sRaiseFileName, nRaiseLine
			);

		switch(pExc->ExceptionCode)
		{
		case EXCEPTION_ACCESS_VIOLATION:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_ACCESS_VIOLATION(0x%08X)"
				, pExc->ExceptionCode);
			if(pExc->NumberParameters >1 && pExc->ExceptionInformation )
			{
				nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, " attempted to %s addr:0x%08X"
					, pExc->ExceptionInformation[0]==1 ? "write" : "read", pExc->ExceptionInformation[1]);
			}
			break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_ARRAY_BOUNDS_EXCEEDED(0x%08X)"
				, pExc->ExceptionCode);
			break;
		case EXCEPTION_BREAKPOINT:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_BREAKPOINT(0x%08X)"
				, pExc->ExceptionCode);
			break;
		case EXCEPTION_DATATYPE_MISALIGNMENT:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_DATATYPE_MISALIGNMENT(0x%08X)"
				, pExc->ExceptionCode);
			break;
		case EXCEPTION_FLT_DENORMAL_OPERAND:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_FLT_DENORMAL_OPERAND(0x%08X)"
				, pExc->ExceptionCode);
			break;
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_FLT_DIVIDE_BY_ZERO(0x%08X)"
				, pExc->ExceptionCode);
			break;
		case EXCEPTION_FLT_INEXACT_RESULT:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_FLT_INEXACT_RESULT(0x%08X)"
				, pExc->ExceptionCode);
			break;
		case EXCEPTION_FLT_INVALID_OPERATION:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_FLT_INVALID_OPERATION(0x%08X)"
				, pExc->ExceptionCode);
			break;
		case EXCEPTION_FLT_OVERFLOW:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_FLT_OVERFLOW(0x%08X)"
				, pExc->ExceptionCode);
			break;
		case EXCEPTION_FLT_STACK_CHECK:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_FLT_STACK_CHECK(0x%08X)"
				, pExc->ExceptionCode);
			break;
		case EXCEPTION_FLT_UNDERFLOW:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_FLT_UNDERFLOW(0x%08X)"
				, pExc->ExceptionCode);
			break;
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_ILLEGAL_INSTRUCTION(0x%08X)"
				, pExc->ExceptionCode);
			break;
		case EXCEPTION_IN_PAGE_ERROR:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_IN_PAGE_ERROR(0x%08X)"
				, pExc->ExceptionCode);
			break;
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_INT_DIVIDE_BY_ZERO(0x%08X)"
				, pExc->ExceptionCode);
			break;
		case EXCEPTION_INT_OVERFLOW:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_INT_OVERFLOW(0x%08X)"
				, pExc->ExceptionCode);
			break;
		case EXCEPTION_INVALID_DISPOSITION:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_INVALID_DISPOSITION(0x%08X)"
				, pExc->ExceptionCode);
			break;
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_NONCONTINUABLE_EXCEPTION(0x%08X)"
				, pExc->ExceptionCode);
			break;
		case EXCEPTION_PRIV_INSTRUCTION:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_PRIV_INSTRUCTION(0x%08X)"
				, pExc->ExceptionCode);
			break;
		case EXCEPTION_SINGLE_STEP:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_SINGLE_STEP(0x%08X)"
				, pExc->ExceptionCode);
			break;
		case EXCEPTION_STACK_OVERFLOW:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "EXCEPTION_STACK_OVERFLOW(0x%08X)"
				, pExc->ExceptionCode);
			nRet = EXCEPTION_CONTINUE_EXECUTION;
			break;
		default:
			nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "Unknow(0x%08X)"
				, pExc->ExceptionCode);
			break;
		}
		//nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "\nfunction=%s\n\n", sRaiseFun);
		nDescLen += _snprintf(sExceptionDesc+nDescLen,SIZEOF_STR(sExceptionDesc)-nDescLen, "\n\n");

		CLogFile* pLogFile = GetExceptFile();
		if(pLogFile)
		{
			pLogFile->OutputLog("exception", 1, sExceptionDesc);
			//call stack
			TraceCallStack(pCxt, 50, pLogFile);
			nDescLen = _snprintf(sExceptionDesc, SIZEOF_STR(sExceptionDesc), "\n------------------------------------------\n\n");
			pLogFile->OutputLog("exception", 1, sExceptionDesc);
			pLogFile->FlushLog();

			//dump memory
			CLogFile* pDmpfile = GetDumpFile();
			static LogSplitPolicy policy = {0};

			if(bNeedDump && pDmpfile &&
				pDmpfile->Initialize(pLogFile->GetDirectory(), "ExceptDump", "dmp", policy))
			{
				DumpException(pe, pDmpfile->GetFileHandle());
			}
		}

		LeaveCriticalSection(&g_csPDBLock);

		return nRet;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LeaveCriticalSection(&g_csPDBLock);
	}
	return EXCEPTION_EXECUTE_HANDLER;
}

void WINAPI ExceptLog( const char* sRaiseFun, const char* sRaiseFileName, long nRaiseLine, const char* sFmt, ...)
{
	__try
	{
		va_list va_ptr;
		va_start( va_ptr, sFmt );
		CLogFile* pLogFile = GetLogFile();
		if(pLogFile)
		{
			char tempstr[200] = {0};
			sprintf( tempstr, "EXCEPT] [function:%s,File: %s,line=%d",sRaiseFun,sRaiseFileName,nRaiseLine);
			pLogFile->OutputLog(tempstr, 1, sFmt, va_ptr);
			pLogFile->FlushLog();
		}
		va_end( va_ptr );
	}
	__except(EXCEPTION_EXECUTE_HANDLER)	{}
}

void WINAPI ExceptLog( const char* sRaiseFun, const char* sRaiseFileName, long nRaiseLine, CException* pe)
{
	__try
	{
		CLogFile* pLogFile = GetLogFile();
		if(pLogFile)
		{
			pLogFile->OutputLog("EXCEPT", 1, pe, "file=%s : %d, function=%s, mfc_exception=", sRaiseFileName, nRaiseLine, sRaiseFun);
			pLogFile->FlushLog();
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)	{}
}

void WINAPI DebugLog( int nLevel, const char* sFmt, ...)
{
	__try
	{
		if( IsValidLog(LogType_Debug, nLevel) )
		{
			va_list va_ptr;
			va_start( va_ptr, sFmt );
			CLogFile* pLogFile = GetLogFile();
			if(pLogFile)
			{
				pLogFile->OutputLog("DEGUG", nLevel, sFmt, va_ptr);
			}
			va_end( va_ptr );
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)	{}
}

void WINAPI TraceLog( int nLevel, const char* sFmt, ... )
{
	__try
	{
		if( IsValidLog(LogType_Trace, nLevel) )
		{
			va_list va_ptr;
			va_start( va_ptr, sFmt );
			CLogFile* pLogFile = GetLogFile();
			if(pLogFile)
			{
				pLogFile->OutputLog("TRACE", nLevel, sFmt, va_ptr);
			}
			va_end( va_ptr );
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){}
}

void WINAPI ErrorLog( int nLevel, const char* sFmt, ... )
{
	__try
	{
		if( IsValidLog(LogType_Error, nLevel) )
		{
			va_list va_ptr;
			va_start( va_ptr, sFmt );
			CLogFile* pLogFile = GetLogFile();
			if(pLogFile)
			{
				pLogFile->OutputLog("ERROR", nLevel, sFmt, va_ptr);
			}
			va_end( va_ptr );
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){}
}

void WINAPI Log( const char* sFmt, ... )
{
	__try
	{
		
			va_list va_ptr;
			va_start( va_ptr, sFmt );
			CLogFile* pLogFile = GetLogFile();
			if(pLogFile)
			{
				pLogFile->OutputLog("Info", 1, sFmt, va_ptr);
			}
			va_end( va_ptr );
		
	}
	__except(EXCEPTION_EXECUTE_HANDLER){}
}

void WINAPI DBLog( int nLevel, const char* sFmt, ... )
{
	__try
	{
		if(IsValidLog(LogType_DB, nLevel))
		{
			va_list va_ptr;
			va_start( va_ptr, sFmt );
			CLogFile* pLogFile = GetDBLogFile();
			if(pLogFile)
			{
				pLogFile->OutputLog("DB", nLevel, sFmt, va_ptr);
			}
			va_end( va_ptr );
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){}
}

void WINAPI ClientLog(UINT nCharaID, int nLevel, const char* sFmt, ... )
{
	__try
	{
		if( IsLogClient(nCharaID, nLevel))
		{
			va_list va_ptr;
			va_start( va_ptr, sFmt );
			CLogFile* pLogFile = GetLogFile();
			if(pLogFile)
			{
				pLogFile->OutputLog("CLIENT", nLevel, sFmt, va_ptr);
			}
			va_end( va_ptr );
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){}
}

void WINAPI ScreenLog(const char* sFmt, ... )
{
	__try
	{
		static char sLocalLogBuf[1024] = "";
		va_list va_ptr;
		va_start( va_ptr, sFmt );
		int nLen = _vsnprintf(sLocalLogBuf, SIZEOF_STR(sLocalLogBuf), sFmt, va_ptr);
		va_end( va_ptr );

		PrintUIMsg(sLocalLogBuf, nLen);
		//CControlPanel::GetInstance()->PushMsg(CON_MSG_LOG, sLocalLogBuf, nLen+1);
	}
	__except(EXCEPTION_EXECUTE_HANDLER){}
}
