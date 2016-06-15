/******************************************************************************
* Copyright (c) 2012,上海好玩研发中心
* All rights reserved.
* 
* 文件名称：LoadDll.h
* 摘    要：游戏服务器的Dll加载和释放
* 
* 当前版本：1.0
* 作    者：buffer001
* 建立日期：2006年03月14日
**********************************************************************************/
#pragma once

enum SD_MODULE
{
	SDM_NET=1,
	SDM_DB=(1<<1),
	SDM_PIPE=(1<<2),
	SDM_TIMER=(1<<3),
	SDM_PT=(1<<4),
};

typedef bool (APIENTRY *UploadReplayAuthFuc)(char* /*data*/, const unsigned char* /*ptid*/, unsigned long /*systime*/, unsigned long /*ip*/, unsigned char /*serverArea*/,const char* /*key = 0*/);

class CLoadDll
{
protected:
	CDBErrHandle		m_DBErrHandler;

	//加载Net的Dll接口
	HINSTANCE			m_hInsNet;
	ISDNetModule*		m_pNet;

	//加载DB的Dll接口
	HINSTANCE			m_hInsDB;
	ISDDBExModule*		m_pDB;

	//加载Pipe的接口
	HINSTANCE			m_hInsPipe;
	ISDPipeModule*		m_pPipe;

	//加载Timer的Dll接口
	HINSTANCE			m_hInsTimer;
	ISDTimerModule*		m_pTimer;

	//SDPT
	//HINSTANCE			m_hInsPT;
	//ISDPTModule*		m_pPT;

	//上传replay验证DLL
	HINSTANCE			m_hInsRep;
	UploadReplayAuthFuc m_pAuthFuc;

public:
	CLoadDll(void);
	virtual ~CLoadDll(void);

	//加载所有的Dll
	bool LoadDlls(DWORD LoadModules);

public:
	//取得网络接口
	ISDNetModule* GetNetInterface()
	{
		return m_pNet;
	}
	//取得DB接口
	ISDDBExModule* GetDBInterface()
	{
		return m_pDB;
	}
	//取得Pipe接口
	ISDPipeModule* GetPipeInterface()
	{
		return m_pPipe;
	}
	//取得Timer接口
	ISDTimerModule* GetTimerInterface()
	{
		return m_pTimer;
	}

// 	ISDPTModule* GetPTInterface()
// 	{
// 		return m_pPT;
// 	}

protected:
	////初始化SDError
	//bool InitSDError();

	//加载Net的Dll接口
	bool LoadNetInterface();
	//加载DB的Dll接口
	bool LoadDBInterface();
	//加载Pipe的Dll接口
	bool LoadPipeInterface();
	//加载Timer的Dll接口
	bool LoadTimerInterface();
	//加载PT的Dll接口
	bool LoadPTInterface();
};
