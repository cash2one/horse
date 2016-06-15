#include "StdAfx.h"
#include "HttpClient.h"
#include ".\dbtransationworkthread.h"

IMPLEMENT_CLASS_INFO_STATIC(HttpClient,CEasyThread);
HttpClient::HttpClient(void)
: m_pCurl(NULL)
, m_WaitEvent(NULL)
{
	
}

HttpClient::~HttpClient(void)
{
	SS_TRY_BEGIN;
	Destory();
	SS_TRY_END();
}

HttpClient& HttpClient::Instance()
{
	static HttpClient client;
	return client;
}

void HttpClient::Destory()
{
	SS_TRY_BEGIN_;

	HttpRequest ** ppHttpRequest=NULL;
	while(ppHttpRequest = m_ProcessQueue.PopOne() )
	{
		SAFE_RELEASE(*ppHttpRequest);
	}
	while(ppHttpRequest = m_FinishQueue.PopOne() )
	{
		SAFE_RELEASE(*ppHttpRequest);
	}

	if(m_pCurl) 
	{
		curl_easy_cleanup(m_pCurl);
	    m_pCurl = NULL;
	}
	SS_TRY_END_();
}

bool HttpClient::Init(int QueueSize)
{
	SS_TRY_BEGIN_;
	Destory();
	m_WaitEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	if(NULL == m_WaitEvent)
	{
		return false;
	}
	m_ProcessQueue.InitQueue(QueueSize);
	m_FinishQueue.InitQueue(QueueSize);
	
	m_pCurl = curl_easy_init();

	return Start();
	SS_TRY_END_();
	return false;
}

bool HttpClient::AddHttpRequest(HttpRequest * httpRequest)
{
	SS_TRY_BEGIN_;
#ifdef _DEBUG
	if( m_FinishQueue.GetObjectCount() >= 10 )
	{
		ErrorLog( 3, "TooMany In FinishQUEUE: queue size=[%u] at AddHttpRequest",

			m_FinishQueue.GetObjectCount());
	}

	if( m_ProcessQueue.GetObjectCount() >= 10 )
	{
		ErrorLog( 3, "TooMany In ExeQUEUE: queue size=[%u] at AddHttpRequest",

			m_ProcessQueue.GetObjectCount());
	}
#endif

	if( NULL == httpRequest)
	{
		return false;
	}

	int ret = 0;
	ret = m_ProcessQueue.PushOne(&httpRequest);

	if( ret == 1 )
	{
		GetLocalTime(&(httpRequest->m_AddToManagerTime));
		SetEvent(m_WaitEvent);
	}
	else if(ret == 0)
	{
		ErrorLog( 3, "AddHttpRequest m_ProcessQueue.size = %u",m_ProcessQueue.GetObjectCount() );
	}
	return ret;
	SS_TRY_END_();

	return false;
}

HttpRequest * HttpClient::PopFinishHttpRequest()
{
	SS_TRY_BEGIN_;
	HttpRequest ** ppHttpRequest = m_FinishQueue.PopOne();
	if( ppHttpRequest )
	{
		return *ppHttpRequest;
	}
	SS_TRY_END_();
	return NULL;
}

BOOL HttpClient::OnStart()
{	
	SS_TRY_BEGIN_;
	return TRUE;
	SS_TRY_END_();
	return FALSE;
}

BOOL HttpClient::OnRun()
{
	SS_TRY_BEGIN_;

#ifdef _DEBUG	
	if( m_FinishQueue.GetObjectCount() >= 10 )
#else
	if( m_FinishQueue.GetObjectCount() >= 1200 )
#endif
	{
		ErrorLog( 3, "HttpClient::OnRun TooMany In FinishQUEUE: queue size=[%u]",

			m_FinishQueue.GetObjectCount());
	}
#ifdef _DEBUG	
	if( m_ProcessQueue.GetObjectCount() >= 10 )
#else
	if( m_ProcessQueue.GetObjectCount() >= 1200 )
#endif
	{
		ErrorLog( 3, "HttpClient::OnRun TooMany In ExeQUEUE: queue size=[%u]",

			m_ProcessQueue.GetObjectCount());
	}

	HttpRequest * pHttpRequest = NULL;
	HttpRequest ** ppHttpRequest=m_ProcessQueue.PopOne();
	if( ppHttpRequest )
	{
		pHttpRequest = *ppHttpRequest;
	}
	if(pHttpRequest)
	{
		SYSTEMTIME systm;
		GetLocalTime(&systm);

		uint64 exeMiliSec = 
			(uint64)systm.wMinute* 60*1000 + (uint64)systm.wSecond*1000 + (uint64)systm.wMilliseconds -
			((uint64)pHttpRequest->m_AddToManagerTime.wMinute* 60*1000 + (uint64)pHttpRequest->m_AddToManagerTime.wSecond*1000 + (uint64)pHttpRequest->m_AddToManagerTime.wMilliseconds
			);
		if( exeMiliSec > 250 )
		{
			ErrorLog( 3, "TooLong Time In QUEUE: [%s] InQueueTime = [%I64u]ms,queue size=[%u]",
				pHttpRequest->GetClassName(),
				exeMiliSec,
				m_ProcessQueue.GetObjectCount());
		}
	
		bool Ret = Execute(pHttpRequest);

		SYSTEMTIME tempsystm;
		GetLocalTime(&tempsystm);

		exeMiliSec =
			(uint64)tempsystm.wMinute* 60*1000 + (uint64)tempsystm.wSecond*1000 + (uint64)tempsystm.wMilliseconds -
			((uint64)systm.wMinute* 60*1000 + (uint64)systm.wSecond*1000 + (uint64)systm.wMilliseconds
			);
		if( exeMiliSec >= 250)
		{
			ErrorLog( 3, "Too Long Execute Time:[%s] ExeuteTime = [%I64u]ms,queue size=[%u]",
				pHttpRequest->GetClassName(),
				exeMiliSec,
				m_ProcessQueue.GetObjectCount());
		}

		pHttpRequest->SetResult(Ret,m_CurlCode);

		m_FinishQueue.PushOne(&pHttpRequest);

		pHttpRequest = NULL;

	}
	else
	{
		WaitForSingleObject( m_WaitEvent,2500);
		//Sleep(1);
	}

	return TRUE;
	SS_TRY_END_();
	return FALSE;
}

void HttpClient::OnTerminate()
{
	SS_TRY_BEGIN_;
	SS_TRY_END_();
}

size_t writeResponse(void *ptr, size_t size, size_t nmemb, void *stream)
{
	string *recvBuffer = (string*)stream;
	size_t sizes = size * nmemb;

	// add data to the end of recvBuffer
	// write data maybe called more than once in a single request
	//recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
	recvBuffer->append((char*)ptr,sizes);

	return sizes;
}

bool HttpClient::Execute(HttpRequest* pHttpRequest)
{
	if(m_pCurl == NULL) return false;

	m_CurlCode = curl_easy_setopt(m_pCurl,CURLOPT_URL,pHttpRequest->url.c_str());
	if(m_CurlCode != CURLE_OK) return false;

	m_CurlCode = curl_easy_setopt(m_pCurl,CURLOPT_FOLLOWLOCATION,1);
	if(m_CurlCode != CURLE_OK) return false;

	m_CurlCode = curl_easy_setopt(m_pCurl,CURLOPT_WRITEFUNCTION,writeResponse);
	if(m_CurlCode != CURLE_OK) return false;

	m_CurlCode = curl_easy_setopt(m_pCurl,CURLOPT_WRITEDATA,&pHttpRequest->buffer);
	if(m_CurlCode != CURLE_OK) return false;

	m_CurlCode =  curl_easy_perform(m_pCurl);
	return (CURLE_OK == m_CurlCode);
}

int HttpClient::Update(int ProcessLimit)
{
	SS_TRY_BEGIN_;
	int ProcessCount=0;
// 	while(ProcessCount<ProcessLimit)
// 	{
// 		int Count=0;
// 		for(int i=0;i<(int)m_WorkThreads.size();i++)
// 		{
// 			if ( m_WorkThreads[i].m_pDbThread )
			{
				HttpRequest * pHttpRequest= PopFinishHttpRequest();
				if(pHttpRequest)
				{
					pHttpRequest->OnFinish();
 					SAFE_RELEASE(pHttpRequest);
					ProcessCount++;
// 					Count++;
				}
			}
// 		}
// 		if(Count==0)
// 			break;
// 	}

	return ProcessCount;
	SS_TRY_END_();
	return 0;
}
