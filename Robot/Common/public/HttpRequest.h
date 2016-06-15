#pragma once

#include ".\DiffSizeMiniObjectsPool.h"
#include "curl/curl.h"

class HttpRequest :	public CNameObject
{
protected:
	bool		m_IsSucceed;
	DECLARE_CLASS_INFO_STATIC(HttpRequest)

public:

// 	static DiffSizeMiniObjectsPool m_TransMemPool;
// 	void * operator new(size_t nSize)
// 	{
// 		if(  nSize > 4*1024-48 )
// 		{
// 			ErrorLog( 5, "New TransMemPool Fail nSize=[%I64u]",nSize);
// 			return NULL;
// 		}
// 		void * pVoid = m_TransMemPool.GetMemory(nSize);
// 		if( ! pVoid )
// 		{
// 			ErrorLog( 5, "m_TransMemPool.GetMemory() Fail");
// 		}
// 		return pVoid;
// 	}
// 
// 	void operator delete(void * pObject)
// 	{
// 		if( ! m_TransMemPool.FreeMemory(pObject) )
// 		{
// 			ErrorLog( 5, "m_TransMemPool.FreeMemory Fail[0x%08X]", pObject);
// 		}
// 	}

	SYSTEMTIME m_AddToManagerTime;
	
	string url;
	string buffer;
	CURLcode curlcode;

	HttpRequest(void);
	~HttpRequest(void);

	virtual bool OnExecute(IDBConnection * pConnection);
	virtual void OnFinish();

	void SetResult(bool IsSucceed,CURLcode code)
	{
		m_IsSucceed = IsSucceed;
		curlcode = code;
	}
};