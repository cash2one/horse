/******************************************************************************
* Copyright (c) 2012,上海好玩研发中心
* All rights reserved.
* 
* 文件名称：LoadDll.cpp
* 摘    要：游戏服务器的Dll加载和释放
* 
* 当前版本：1.0
* 作    者：buffer001
* 建立日期：2006年03月14日
**********************************************************************************/
#include "StdAfx.h"

#ifdef _DEBUG

#define NET_MODULE_DLL		"SDNetD.dll"
#define DB_MODULE_DLL		"SDDBD.dll"
#define PIPE_MODULE_DLL		"SDPipeD.dll"
#define TIMER_MODULE_DLL	"SDTimerD.dll"
#define PT_MODULE_DLL		"SDPT.dll"
#else
#define NET_MODULE_DLL		"SDNet.dll"
#define DB_MODULE_DLL		"SDDB.dll"
#define PIPE_MODULE_DLL		"SDPipe.dll"
#define TIMER_MODULE_DLL	"SDTimer.dll"
#define PT_MODULE_DLL		"SDPT.dll"
#endif

CLoadDll::CLoadDll(void)
{
	SS_TRY_BEGIN;
	m_hInsNet		=	NULL;
	m_pNet			=	NULL;
	m_hInsDB		=	NULL;
	m_pDB			=	NULL;
	m_hInsPipe		=	NULL;
	m_pPipe			=	NULL;
	m_hInsTimer		=	NULL;
	m_pTimer		=	NULL;
// 	m_hInsPT		=	NULL;
// 	m_pPT			=	NULL;
	m_hInsRep		=	NULL;
	m_pAuthFuc		=	NULL;
	SS_TRY_END();
}

CLoadDll::~CLoadDll(void)
{
	SS_TRY_BEGIN;
	if(m_pTimer)
	{
		m_pTimer->Release();
		if(m_hInsTimer)
		{
			FreeLibrary(m_hInsTimer);
			m_hInsTimer	= NULL;
		}
		m_pTimer = NULL;
	}

	if(m_pPipe)
	{
		m_pPipe->Release();
		m_pPipe	=	NULL;
		if(m_hInsPipe)
		{
			FreeLibrary(m_hInsPipe);
			m_hInsPipe	=	NULL;
		}
	}

	if(m_pDB)
	{
		m_pDB->Release();
		if(m_hInsDB)
		{
			FreeLibrary(m_hInsDB);
			m_hInsDB	=	NULL;
		}
		m_pDB = NULL;
	}

	if(m_pNet)
	{
		m_pNet->Release();
		if(m_hInsNet)
		{
			FreeLibrary(m_hInsNet);
			m_hInsNet	=	NULL;
		}
		m_pNet	=	NULL;
	}

// 	if(m_pPT)
// 	{
// 		m_pPT->Release();
// 		if(m_hInsPT)
// 		{
// 			FreeLibrary(m_hInsPT);
// 			m_hInsNet	=	NULL;
// 		}
// 		m_pPT	=	NULL;
// 	}

	if(m_hInsRep)
	{
		FreeLibrary(m_hInsRep);
		m_hInsRep = NULL;
		m_pAuthFuc = NULL;
	}
	//SDErrorInstance()->Release();
	SS_TRY_END();
}

//加载所有的Dll
bool CLoadDll::LoadDlls(DWORD LoadModules)
{
	SS_TRY_BEGIN_;
	//if(!InitSDError())
	//	return false;

	if(LoadModules&SDM_NET)
	{
		if(!LoadNetInterface())
			return false;
	}
	if(LoadModules&SDM_DB)
	{
		if(!LoadDBInterface())
			return false;
	}
	if(LoadModules&SDM_PIPE)
	{
		if(!LoadPipeInterface())
			return false;
	}
	if(LoadModules&SDM_TIMER)
	{
		if(!LoadTimerInterface())
			return false;
	}
	if(LoadModules&SDM_PT)
	{
		if(!LoadPTInterface())
			return false;
	}

	return true;	
	SS_TRY_END_();
	return false;
}

#include <strsafe.h>

void ErrorExit(LPTSTR lpszFunction) 
{ 
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
		(lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
	StringCchPrintf((LPTSTR)lpDisplayBuf, 
		LocalSize(lpDisplayBuf),
		TEXT("%s failed with error %d: %s"), 
		lpszFunction, dw, lpMsgBuf); 
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	//ExitProcess(dw); 
}

//加载Net的Dll接口
bool CLoadDll::LoadNetInterface()
{
	SS_TRY_BEGIN_;
	m_hInsNet = LoadLibrary(NET_MODULE_DLL);
	if(m_hInsNet == NULL)
	{
		ErrorExit(TEXT("LoadNetInterface"));

		//MessageBox(NULL, NET_MODULE_DLL, "DLL file can't be found", 0);
		return false;
	}

	PFN_SDNetGetModule pNet = (PFN_SDNetGetModule)GetProcAddress(m_hInsNet, "SDNetGetModule");
	if(pNet == NULL)
	{
		MessageBox(NULL, NET_MODULE_DLL, "Find interface failed in DLL file", 0);
		return false;
	}

	m_pNet = pNet(&SDNETMODULE_VERSION);
	if(m_pNet == NULL)
	{
		MessageBox(NULL, NET_MODULE_DLL, "Invoke DLL interface failed", 0);
		return false;
	}

	return true;
	SS_TRY_END_();
	return false;
}

//加载DB的Dll接口
bool CLoadDll::LoadDBInterface()
{
	SS_TRY_BEGIN_;

	m_hInsDB = LoadLibrary(DB_MODULE_DLL);
	if(m_hInsDB == NULL)
	{
		MessageBox(NULL, DB_MODULE_DLL, "DLL file can't be found", 0);
		return false;
	}

	PFN_SDDBExGetModule pDB = (PFN_SDDBExGetModule)GetProcAddress(m_hInsDB, "SDDBExGetModule");
	if(pDB == NULL)
	{
		MessageBox(NULL, DB_MODULE_DLL, "Find interface failed in DLL file", 0);
		return false;
	}

	m_pDB = pDB(&m_DBErrHandler, &SDDB_VERSION);
	if(m_pDB == NULL)
	{
		MessageBox(NULL, DB_MODULE_DLL, "Invoke DLL interface failed", 0);
		return false;
	}

	return true;
	SS_TRY_END_();
	return false;
}

//加载Pipe的Dll接口
bool CLoadDll::LoadPipeInterface()
{
	SS_TRY_BEGIN_;

	m_hInsPipe = LoadLibrary(PIPE_MODULE_DLL);
	if(m_hInsPipe == NULL)
	{
		MessageBox(NULL, PIPE_MODULE_DLL, "DLL file can't be found", 0);
		return false;
	}

	PFN_SDPipeGetModule pPipe = (PFN_SDPipeGetModule)GetProcAddress(m_hInsPipe, "SDPipeGetModule");
	if(pPipe == NULL)
	{
		MessageBox(NULL, PIPE_MODULE_DLL, "Find interface failed in DLL file", 0);
		return false;
	}

	m_pPipe = pPipe(&SDPIPE_VERSION);
	if(m_pPipe == NULL)
	{
		MessageBox(NULL, PIPE_MODULE_DLL, "Invoke DLL interface failed", 0);
		return false;
	}

	return true;
	SS_TRY_END_();
	return false;
}

//加载Timer的Dll接口
bool CLoadDll::LoadTimerInterface()
{
	SS_TRY_BEGIN_;

	m_hInsTimer = LoadLibrary(TIMER_MODULE_DLL);
	if(m_hInsTimer == NULL)
	{
		MessageBox(NULL, TIMER_MODULE_DLL, "DLL file can't be found", 0);
		return false;
	}

	PFN_SDTimerGetModule pTimer = (PFN_SDTimerGetModule)GetProcAddress(m_hInsTimer, "SDTimerGetModule");
	if(pTimer == NULL)
	{
		MessageBox(NULL, TIMER_MODULE_DLL, "Find interface failed in DLL file", 0);
		return false;
	}

	m_pTimer = pTimer(&SDTIMER_VERSION);
	if(m_pTimer == NULL)
	{
		MessageBox(NULL, TIMER_MODULE_DLL, "Invoke DLL interface failed", 0);
		return false;
	}

	return true;
	SS_TRY_END_();
	return false;
}

bool CLoadDll::LoadPTInterface()
{
	SS_TRY_BEGIN_;

// 	m_hInsPT = LoadLibrary(PT_MODULE_DLL);
// 	if(m_hInsPT == NULL)
// 	{
// 		MessageBox(NULL, PT_MODULE_DLL, "DLL file can't be found", 0);
// 		return false;
// 	}
// 
// 	PFN_SDPTGetModule pGetInterFace = (PFN_SDPTGetModule)GetProcAddress(m_hInsPT, "SDPTGetModule");
// 	if(pGetInterFace == NULL)
// 	{
// 		MessageBox(NULL, PT_MODULE_DLL, "Find interface failed in DLL file", 0);
// 		return false;
// 	}

// 	m_pPT = pGetInterFace(&SD_PT_VERSION);
// 	if(m_pPT == NULL)
// 	{
// 		MessageBox(NULL, PT_MODULE_DLL, "Invoke DLL interface failed", 0);
// 		return false;
// 	}

	return true;
	SS_TRY_END_();
	return false;
}

