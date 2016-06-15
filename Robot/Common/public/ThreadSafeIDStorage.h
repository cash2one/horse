#pragma once
#include <vector>
#include <string>


template < class T>
class CThreadSafeIDStorage
{
protected:
	struct StorageNode
	{
		UINT ID;
		StorageNode * pPrev;
		StorageNode * pNext;
		bool IsFree;		
		T Object;
	};
	typedef StorageNode * LPSTORAGENODE;
	volatile LPSTORAGENODE	m_pObjectBuff;
	volatile LPSTORAGENODE	m_pFreeList;
	volatile LPSTORAGENODE	m_pObjectList;
	volatile LPSTORAGENODE	m_pObjectListTail;
	volatile UINT			m_ObjectCount;
	volatile UINT			m_ObjectBuffSize;	
	CEasyCriticalSection	m_EasyCriticalSection;	
public:
	CThreadSafeIDStorage()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		m_pObjectBuff=NULL;
		m_pFreeList=NULL;
		m_pObjectList=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;
		m_ObjectBuffSize=0;		
	}
	~CThreadSafeIDStorage()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		Destory();
	}
	bool Create(UINT Size)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		Clear();
		m_ObjectBuffSize=Size;
		m_pObjectBuff=new StorageNode[m_ObjectBuffSize];
		for(UINT i=0;i<m_ObjectBuffSize;i++)
		{
			m_pObjectBuff[i].ID=i+1;			
			i==0?m_pObjectBuff[i].pPrev=NULL:m_pObjectBuff[i].pPrev=&(m_pObjectBuff[i-1]);
			i==m_ObjectBuffSize-1?m_pObjectBuff[i].pNext=NULL:m_pObjectBuff[i].pNext=&(m_pObjectBuff[i+1]);
			m_pObjectBuff[i].IsFree=true;
		}
		m_pFreeList=m_pObjectBuff;
		m_pObjectList=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;		
		return true;

	}
	void Destory()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		SAFE_DELETE_ARRAY(m_pObjectBuff);
		m_pFreeList=NULL;
		m_pObjectList=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;
		m_ObjectBuffSize=0;		
	}
	void Clear()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		for(UINT i=0;i<m_ObjectBuffSize;i++)
		{
			m_pObjectBuff[i].ID=i+1;			
			i==0?m_pObjectBuff[i].pPrev=NULL:m_pObjectBuff[i].pPrev=&(m_pObjectBuff[i-1]);
			i==m_ObjectBuffSize-1?m_pObjectBuff[i].pNext=NULL:m_pObjectBuff[i].pNext=&(m_pObjectBuff[i+1]);
			m_pObjectBuff[i].IsFree=true;
		}
		m_pFreeList=m_pObjectBuff;
		m_pObjectList=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;	
	}
	UINT NewObject(T** ppObject)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		if(m_pFreeList)
		{			
			StorageNode * pNode;

			pNode=m_pFreeList;
			m_pFreeList=pNode->pNext;
			if(m_pFreeList)
				m_pFreeList->pPrev=NULL;
			pNode->pPrev=NULL;
			pNode->pNext=m_pObjectList;
			if(m_pObjectList)
				m_pObjectList->pPrev=pNode;
			else
				m_pObjectListTail=pNode;
			m_pObjectList=pNode;
			pNode->IsFree=false;
			if(ppObject)
				*ppObject=&(pNode->Object);				
			m_ObjectCount++;
			return pNode->ID;
		}
		*ppObject=NULL;
		return 0;
	}	

	UINT AddObject(T& Object)	{
		
		CAutoLock Lock(m_EasyCriticalSection);
		UINT ID;
		T * pObject;
		ID=NewObject(&pObject);
		if(ID)
		{
			*pObject=Object;
			return ID;
		}
		return 0;
	}
	T* GetObject(UINT ID)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		if(ID>0&&ID<=m_ObjectBuffSize)
		{
			ID--;
			if(!m_pObjectBuff[ID].IsFree)
			{
				return &(m_pObjectBuff[ID].Object);
			}
		}
		return NULL;
	}	
	T* GetObjectDirect(UINT ID)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		if(ID>0&&ID<=m_ObjectBuffSize)
		{
			ID--;
			return &(m_pObjectBuff[ID].Object);
		}
		return NULL;
	}	
	bool DeleteObject(UINT ID)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		if(ID>0&&ID<=m_ObjectBuffSize)
		{
			ID--;
			if(!m_pObjectBuff[ID].IsFree)
			{
				DeleteNode(m_pObjectBuff+ID);
				return true;
			}
		}
		return false;;
	}	

	UINT GetObjectCount()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return m_ObjectCount;
	}

	LPVOID GetFirstObjectPos()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return m_pObjectList;
	}

	LPVOID GetLastObjectPos()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return m_pObjectListTail;
	}

	T* GetNext(LPVOID& Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		if(Pos)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			Pos=pNode->pNext;
			return &(pNode->Object);
		}
		return NULL;
	}

	T* GetPrev(LPVOID& Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		if(Pos)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			Pos=pNode->pPrev;
			return &(pNode->Object);
		}
		return NULL;
	}

protected:
	void DeleteNode(StorageNode * pNode)
	{	
		pNode->IsFree=true;		
		if(pNode->pPrev)
		{
			pNode->pPrev->pNext=pNode->pNext;
			if(pNode->pNext)
				pNode->pNext->pPrev=pNode->pPrev;
			else
				m_pObjectListTail=pNode->pPrev;
		}
		else
		{
			m_pObjectList=pNode->pNext;
			if(m_pObjectList)
				m_pObjectList->pPrev=NULL;
			else
				m_pObjectListTail=NULL;
		}
		pNode->pPrev=NULL;
		pNode->pNext=m_pFreeList;
		if(m_pFreeList)
			m_pFreeList->pPrev=pNode;
		m_pFreeList=pNode;
		m_ObjectCount--;
	}

};