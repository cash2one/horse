#pragma once

#include "curl/curl.h"
#include "HttpRequest.h"
#include "CircleQueue.h"

#define DEFAULT_TRANS_QUEUE		2800
#define CONNECTION_TEST_TIME	20000

class HttpClient :
	public CEasyThread
{
protected:
	CircleQueue<HttpRequest *>		m_ProcessQueue;
	CircleQueue<HttpRequest *>		m_FinishQueue;
	HANDLE							m_WaitEvent;

	CURLcode						m_CurlCode;
	CURL*							m_pCurl;

	DECLARE_CLASS_INFO_STATIC(HttpClient)

public:
	HttpClient(void);
	~HttpClient(void);

	static HttpClient& Instance();
	void Destory();

	bool Init(int QueueSize = DEFAULT_TRANS_QUEUE);
	bool AddHttpRequest(HttpRequest*);
	HttpRequest *PopFinishHttpRequest();

	bool Execute(HttpRequest*);
	int Update(int);

	UINT GetQueueLen()
	{
		return m_ProcessQueue.GetObjectCount();
	}

	UINT GetFinishQueueLen()
	{
		return m_FinishQueue.GetObjectCount();
	}

protected:
	virtual BOOL OnStart();	
	virtual BOOL OnRun();
	virtual void OnTerminate();
};