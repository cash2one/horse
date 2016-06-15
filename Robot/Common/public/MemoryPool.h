#pragma once

class MemoryPool
{
	unsigned int m_NewObjectSize;
	unsigned int m_OldObjectSize;
	unsigned int m_freeNum;
	unsigned int m_totalNum;
	unsigned int m_objectSize;

	unsigned int m_totalSize;
	
	char** m_pfreeStack;
	char* m_pObjects;

	
	DWORD m_dwPageSize;
	int		m_PagesNum;
	
public:
	LPVOID lpvResult;
	void* GetMemory();
	bool FreeMemory( void * pObject);

	bool CheckObjectPoint( void * pObject) const;

	MemoryPool(unsigned int count,unsigned int objectSize);
	~MemoryPool(void);
};
