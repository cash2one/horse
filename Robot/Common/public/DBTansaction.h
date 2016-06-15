#pragma once

#include ".\DiffSizeMiniObjectsPool.h"

class CDBTansaction :
	public CNameObject
{
protected:
	
	bool		m_IsSucceed;
	
	DECLARE_CLASS_INFO_STATIC(CDBTansaction)
public:

	static DiffSizeMiniObjectsPool m_TransMemPool;
	void * operator new(size_t nSize)
	{
		if(  nSize > 4*1024-48 )
		{
			ErrorLog( 5, "New TransMemPool Fail nSize=[%I64u]",nSize);
			return NULL;
		}
		void * pVoid = m_TransMemPool.GetMemory(nSize);
		if( ! pVoid )
		{
			ErrorLog( 5, "m_TransMemPool.GetMemory() Fail");
		}
		return pVoid;
	}
	
	void operator delete(void * pObject)
	{
		if( ! m_TransMemPool.FreeMemory(pObject) )
		{
			ErrorLog( 5, "m_TransMemPool.FreeMemory Fail[0x%08X]", pObject);
		}
	}
	SYSTEMTIME m_AddToManagerTime;

	CDBTansaction(void);
	~CDBTansaction(void);

	virtual bool OnExecute(IDBConnection * pConnection);
	virtual void OnFinish();

	void SetResult(bool IsSucceed)
	{
		m_IsSucceed=IsSucceed;
	}
};

