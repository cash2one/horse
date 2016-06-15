/******************************************************************************
* Copyright (c) 2012,�Ϻ������з�����
* All rights reserved.
* 
* �ļ����ƣ�LoadDll.h
* ժ    Ҫ����Ϸ��������Dll���غ��ͷ�
* 
* ��ǰ�汾��1.0
* ��    �ߣ�buffer001
* �������ڣ�2006��03��14��
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

	//����Net��Dll�ӿ�
	HINSTANCE			m_hInsNet;
	ISDNetModule*		m_pNet;

	//����DB��Dll�ӿ�
	HINSTANCE			m_hInsDB;
	ISDDBExModule*		m_pDB;

	//����Pipe�Ľӿ�
	HINSTANCE			m_hInsPipe;
	ISDPipeModule*		m_pPipe;

	//����Timer��Dll�ӿ�
	HINSTANCE			m_hInsTimer;
	ISDTimerModule*		m_pTimer;

	//SDPT
	//HINSTANCE			m_hInsPT;
	//ISDPTModule*		m_pPT;

	//�ϴ�replay��֤DLL
	HINSTANCE			m_hInsRep;
	UploadReplayAuthFuc m_pAuthFuc;

public:
	CLoadDll(void);
	virtual ~CLoadDll(void);

	//�������е�Dll
	bool LoadDlls(DWORD LoadModules);

public:
	//ȡ������ӿ�
	ISDNetModule* GetNetInterface()
	{
		return m_pNet;
	}
	//ȡ��DB�ӿ�
	ISDDBExModule* GetDBInterface()
	{
		return m_pDB;
	}
	//ȡ��Pipe�ӿ�
	ISDPipeModule* GetPipeInterface()
	{
		return m_pPipe;
	}
	//ȡ��Timer�ӿ�
	ISDTimerModule* GetTimerInterface()
	{
		return m_pTimer;
	}

// 	ISDPTModule* GetPTInterface()
// 	{
// 		return m_pPT;
// 	}

protected:
	////��ʼ��SDError
	//bool InitSDError();

	//����Net��Dll�ӿ�
	bool LoadNetInterface();
	//����DB��Dll�ӿ�
	bool LoadDBInterface();
	//����Pipe��Dll�ӿ�
	bool LoadPipeInterface();
	//����Timer��Dll�ӿ�
	bool LoadTimerInterface();
	//����PT��Dll�ӿ�
	bool LoadPTInterface();
};
