#include "StdAfx.h"
#include ".\DiffSizeMiniObjectsPool.h"

void* DiffSizeMiniObjectsPool::GetMemory(unsigned int nSize)
{
	if(m_freeNum > 0 )
	{
		--m_freeNum;
		char* pObjectAddr = m_pfreeStack[m_freeNum]+m_PagesNum*m_dwPageSize-nSize;
		char* pInt =(char*)(pObjectAddr-sizeof(unsigned int));
		if( *pInt != 0)
		{
			return NULL;
		}
		++(*pInt);
		unsigned short* pObjectSize = (unsigned short*)(pObjectAddr-sizeof(unsigned short));
		*pObjectSize = nSize;
		unsigned int* pflag = (unsigned int*)(pObjectAddr-2*sizeof(unsigned int));
		*pflag = 0xFFFFFFFF;
		return pObjectAddr;
	}
	else 
	{
		return NULL;
	}
}

bool DiffSizeMiniObjectsPool::FreeMemory( void * pObject)
{
	if( m_pObjects < pObject 
		&& pObject < m_pObjects + m_totalSize
		)
	{
		unsigned int* pflag = (unsigned int*)((char*)pObject-2*sizeof(unsigned int));
		if( *pflag != 0xFFFFFFFF)
		{
			return false;
		}
		*pflag = 0;
		char* pInt =(char*)((char*)pObject-sizeof(unsigned int));
		if( *pInt != 1)
		{
			return false;
		}
		--(*pInt);

		unsigned short* pObjectSize = (unsigned short*)((char*)pObject-sizeof(unsigned short));
		
		memset(pObject,0,(*pObjectSize));
		m_pfreeStack[m_freeNum] = (char*)pObject - m_PagesNum*m_dwPageSize + (*pObjectSize);
		(*pObjectSize) = 0;
		++m_freeNum;

		return true;
	}
	else
	{
		return false;
	}
}


DiffSizeMiniObjectsPool::DiffSizeMiniObjectsPool(unsigned int count,unsigned int objectSize)
	:m_freeNum(count),m_totalNum(count),m_OldObjectSize(objectSize),m_PagesNum(0)
{
	m_NewObjectSize = objectSize+2*sizeof(unsigned int);
	SYSTEM_INFO sSysInfo;         // useful information about the system

	GetSystemInfo(&sSysInfo);     // populate the system information structure

	m_dwPageSize = sSysInfo.dwPageSize;
	
	int last = m_NewObjectSize%m_dwPageSize;
	if( last != 0)
	{
		m_PagesNum = m_NewObjectSize/m_dwPageSize+1;
	}
	else
	{
		m_PagesNum = m_NewObjectSize/m_dwPageSize;
	}
	m_objectSize = m_dwPageSize*(m_PagesNum +1);
	
	m_totalSize = count*m_objectSize;
	lpvResult = VirtualAlloc(NULL,m_totalSize+m_dwPageSize,MEM_RESERVE,PAGE_NOACCESS);
	LPVOID	lpMem = (char*)lpvResult + m_dwPageSize;
	for( int i = 0; i < count; ++i)
	{
		VirtualAlloc((char*)lpMem+i*m_objectSize, m_PagesNum*m_dwPageSize,MEM_COMMIT,PAGE_READWRITE);
		
	}
	
	m_pObjects = (char*)lpMem;
	
	m_pfreeStack = new char*[m_totalNum];
	for(unsigned int i = 0 ; i < m_totalNum ; ++i)
	{
		m_pfreeStack[i] = &(m_pObjects[i*m_objectSize]);
		memset( m_pfreeStack[i], 0, m_PagesNum*m_dwPageSize);

	}

}

DiffSizeMiniObjectsPool::~DiffSizeMiniObjectsPool(void)
{
	if(lpvResult)
	{
		LPVOID	lpMem = (char*)lpvResult + m_dwPageSize;
		for( int i = 0; i < m_totalNum; ++i)
		{
			VirtualFree((char*)lpMem+i*m_objectSize, m_PagesNum*m_dwPageSize,MEM_DECOMMIT);
		}
		
		VirtualFree(lpvResult, 0,MEM_RELEASE);
	}
	m_freeNum = 0;
	m_totalNum = 0;
	if(m_pfreeStack)
	{
		delete [] m_pfreeStack;
	}
}

