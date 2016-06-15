#include "StdAfx.h"
#include "HttpRequest.h"
#include ".\dbtansaction.h"

// DiffSizeMiniObjectsPool HttpRequest::m_TransMemPool(10000,4*1024-48);

IMPLEMENT_CLASS_INFO_STATIC(HttpRequest,CNameObject);
HttpRequest::HttpRequest(void)
{
	m_IsSucceed=false;
	++m_iObjectCount;
	memset(&m_AddToManagerTime, 0, sizeof(m_AddToManagerTime));
}

HttpRequest::~HttpRequest(void)
{

}

bool HttpRequest::OnExecute(IDBConnection * pConnection)
{
	return false;
}

void HttpRequest::OnFinish()
{
	if(!m_IsSucceed)
	{
		ScreenLog("======= Http fail:%ld =======",(int)curlcode);
		TraceLog(3,"======= Http fail:%ld =======",(int)curlcode);
	}
}

