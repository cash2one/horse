#pragma once

#include "CircleQueue.h"

#define DEFAULT_TRANS_QUEUE		2800
#define CONNECTION_TEST_TIME	20000

class CDBTransationWorkThread :
	public CEasyThread
{
protected:
	IDBConnection *						m_pConnection;
	CEasyString							m_ConnectString;
	CircleQueue<CDBTansaction *>	m_TransQueue;
	CircleQueue<CDBTansaction *>	m_FinishTransQueue;
	CEasyTimer							m_ConnectionTestTimer;
	HANDLE		m_WaitEvent;
	DECLARE_CLASS_INFO_STATIC(CDBTransationWorkThread)
public:
	CDBTransationWorkThread(void);
	~CDBTransationWorkThread(void);

	void Destory();

	bool Init(IDBConnection * pConnection,LPCTSTR ConnectStr,int QueueSize=DEFAULT_TRANS_QUEUE);
	bool AddTransaction(CDBTansaction * pDBTansaction);
	CDBTansaction *PopFinishTransaction();

	UINT GetQueueLen()
	{
		return m_TransQueue.GetObjectCount();
	}
	UINT GetFinishQueueLen()
	{
		return m_FinishTransQueue.GetObjectCount();
	}

protected:
	virtual BOOL OnStart();	
	virtual BOOL OnRun();
	virtual void OnTerminate();
};

