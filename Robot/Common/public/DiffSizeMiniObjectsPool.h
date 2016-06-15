#pragma once

class DiffSizeMiniObjectsPool
{
	unsigned int m_NewObjectSize;
	unsigned int m_OldObjectSize;
	unsigned int m_freeNum;
	unsigned int m_totalNum;
	unsigned int m_objectSize;

	unsigned int m_totalSize;
	
	char** m_pfreeStack;
	char* m_pObjects;

	LPVOID lpvResult;
	DWORD m_dwPageSize;
	int		m_PagesNum;
	
public:
	void* GetMemory(unsigned int nSize);
	bool FreeMemory( void * pObject);

	DiffSizeMiniObjectsPool(unsigned int count,unsigned int objectSize);
	~DiffSizeMiniObjectsPool(void);
};
