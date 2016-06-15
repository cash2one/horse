
#include "StdAfx.h"
#include ".\MemoryPool.h"
void* MemoryPool::GetMemory()
{
	if(m_freeNum > 0 && m_freeNum <= m_totalNum)
	{
		--m_freeNum;
		unsigned int* pInt =(unsigned int*)(m_pfreeStack[m_freeNum]-sizeof(unsigned int));
		if( *pInt != 0)
		{
			return NULL;
		}
		++(*pInt);
		memset(m_pfreeStack[m_freeNum],0,m_OldObjectSize);
		return m_pfreeStack[m_freeNum];
	}
	else 
	{
		return NULL;
	}
}

bool MemoryPool::FreeMemory( void * pObject)
{
	if( m_pObjects <= pObject 
		&& pObject < m_pObjects + m_totalSize
		&& ((char*)pObject - (char*)m_pObjects)%m_objectSize == 0)
	{
		unsigned int* pInt =(unsigned int*)((char*)pObject-sizeof(unsigned int));
		if( *pInt != 1)
		{
			return false;
		}
		--(*pInt);
		memset(pObject,0,m_OldObjectSize);
		m_pfreeStack[m_freeNum] = (char*)pObject;
		++m_freeNum;
		if( m_freeNum > m_totalNum)
		{
			return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}


MemoryPool::MemoryPool(unsigned int count,unsigned int objectSize)
	:m_freeNum(count),m_totalNum(count),m_OldObjectSize(objectSize),m_PagesNum(0)
{
	m_NewObjectSize = objectSize+sizeof(unsigned int);
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
	lpvResult = VirtualAlloc(NULL,m_totalSize+4*m_dwPageSize,MEM_RESERVE,PAGE_NOACCESS);
	LPVOID	lpMem = (char*)lpvResult + 2*m_dwPageSize;
	for( int i = 0; i < count; ++i)
	{
		VirtualAlloc((char*)lpMem+i*m_objectSize, m_PagesNum*m_dwPageSize,MEM_COMMIT,PAGE_READWRITE);
		//²âÊÔÓÃ
		*((char*)lpMem+i*m_objectSize) = 0;
	}
	
	m_pObjects = (char*)lpMem + (m_PagesNum*m_dwPageSize) - objectSize;
	
	m_pfreeStack = new char*[m_totalNum];
	for(unsigned int i = 0 ; i < m_totalNum ; ++i)
	{
		m_pfreeStack[i] = &(m_pObjects[i*m_objectSize]);
		memset( m_pfreeStack[i], 0, objectSize);
		memset( m_pfreeStack[i]-sizeof(unsigned int), 0, sizeof(unsigned int));
	}

}

bool MemoryPool::CheckObjectPoint( void * pObject) const
{
	if( m_pObjects <= pObject 
		&& pObject < m_pObjects + m_totalSize
		&& ((char*)pObject - (char*)m_pObjects)%m_objectSize == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

MemoryPool::~MemoryPool(void)
{
	if(lpvResult)
	{
		LPVOID	lpMem = (char*)lpvResult + 2*m_dwPageSize;
		BOOL ret = FALSE;
		for( int i = 0; i < m_totalNum; ++i)
		{
			ret = VirtualFree((char*)lpMem+i*m_objectSize, m_PagesNum*m_dwPageSize,MEM_DECOMMIT);
			if( ! ret)
			{
				#ifdef _DEBUG
					OutputDebugString("MEM_DECOMMIT error");
				#endif
			}
		}
		
		ret = VirtualFree(lpvResult, 0,MEM_RELEASE);
		if( ! ret)
		{
			#ifdef _DEBUG
				OutputDebugString("MEM_RELEASE error");
			#endif
		}
		#ifdef _DEBUG
		OutputDebugString("MemoryPool::~MemoryPool");
		#endif

	}
	m_freeNum = 0;
	m_totalNum = 0;
	if(m_pfreeStack)
	{
		delete [] m_pfreeStack;
	}
}

