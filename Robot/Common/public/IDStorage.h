#pragma once
#include <vector>
#include <string>


template < class T ,bool IsDynamic=false>
class CIDStorage
{
protected:
	struct StorageNode
	{
		UINT ID;
		StorageNode * pPrev;
		StorageNode * pNext;
		bool IsFree;		
		T * pObject;
	};
	StorageNode * m_pObjectBuff;
	StorageNode * m_pFreeList;
	StorageNode * m_pObjectListHead;
	StorageNode * m_pObjectListTail;
	UINT m_ObjectCount;
	UINT m_ObjectBuffSize;	
public:
	CIDStorage()
	{
		m_pObjectBuff=NULL;
		m_pFreeList=NULL;
		m_pObjectListHead=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;
		m_ObjectBuffSize=0;		
	}
	~CIDStorage()
	{
		Destory();
	}
	bool Create(UINT Size)
	{
		Clear();
		m_ObjectBuffSize=Size;
		m_pObjectBuff=new StorageNode[m_ObjectBuffSize];
		for(UINT i=0;i<m_ObjectBuffSize;i++)
		{
			m_pObjectBuff[i].ID=i+1;			
			i==0?m_pObjectBuff[i].pPrev=NULL:m_pObjectBuff[i].pPrev=&(m_pObjectBuff[i-1]);
			i==m_ObjectBuffSize-1?m_pObjectBuff[i].pNext=NULL:m_pObjectBuff[i].pNext=&(m_pObjectBuff[i+1]);
			m_pObjectBuff[i].IsFree=true;
			if(IsDynamic)
				m_pObjectBuff[i].pObject=NULL;
			else
				m_pObjectBuff[i].pObject=new T;
		}
		m_pFreeList=m_pObjectBuff;
		m_pObjectListHead=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;		
		return true;

	}
	void Destory()
	{
		for(UINT i=0;i<m_ObjectBuffSize;i++)
		{		
			SAFE_DELETE(m_pObjectBuff[i].pObject);
		}
		SAFE_DELETE_ARRAY(m_pObjectBuff);
		m_pFreeList=NULL;
		m_pObjectListHead=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;
		m_ObjectBuffSize=0;		
	}
	void Clear()
	{
		for(UINT i=0;i<m_ObjectBuffSize;i++)
		{
			m_pObjectBuff[i].ID=i+1;			
			i==0?m_pObjectBuff[i].pPrev=NULL:m_pObjectBuff[i].pPrev=&(m_pObjectBuff[i-1]);
			i==m_ObjectBuffSize-1?m_pObjectBuff[i].pNext=NULL:m_pObjectBuff[i].pNext=&(m_pObjectBuff[i+1]);
			m_pObjectBuff[i].IsFree=true;
			if(IsDynamic)
				SAFE_DELETE(m_pObjectBuff[i].pObject);
		}
		m_pFreeList=m_pObjectBuff;
		m_pObjectListHead=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;	
	}
	UINT NewObject(T** ppObject)
	{
		StorageNode * pNode=NewNode();
		if(pNode)
		{				
			InsertNodeAfter(pNode,m_pObjectListTail);			
			*ppObject=pNode->pObject;
			return pNode->ID;
		}
		*ppObject=NULL;
		return 0;
	}	

	UINT AddObject(T& Object)
	{
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
	LPVOID GetObjectPos(UINT ID) const
	{
		if(ID>0&&ID<=m_ObjectBuffSize)
		{
			ID--;
			if(!m_pObjectBuff[ID].IsFree)
				return &(m_pObjectBuff[ID]);
		}
		return NULL;
	}
	T* GetObject(LPVOID Pos) const
	{
		StorageNode * pNode=(StorageNode *)Pos;	
		if(Pos)
			return pNode->pObject;
		else
			return NULL;
	}	
	T* GetObject(UINT ID) const
	{		
		return GetObject(GetObjectPos(ID));
	}
	bool DeleteObject(UINT ID)
	{
		StorageNode * pNode=(StorageNode *)GetObjectPos(ID);
		if(pNode)
		{
			DeleteNode(pNode);
			return true;
		}
		return false;;
	}	

	UINT GetObjectCount() const
	{
		return m_ObjectCount;
	}

	LPVOID GetFirstObjectPos() const
	{
		return m_pObjectListHead;
	}

	LPVOID GetLastObjectPos() const
	{
		return m_pObjectListTail;
	}

	T* GetNext(LPVOID& Pos) const
	{
		if(Pos)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			Pos=pNode->pNext;
			return pNode->pObject;
		}
		return NULL;
	}

	T* GetPrev(LPVOID& Pos) const
	{
		if(Pos)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			Pos=pNode->pPrev;
			return pNode->pObject;
		}
		return NULL;
	}
	BOOL MoveBeforeTo(LPVOID Pos,LPVOID Before)
	{
		if(Pos&&Pos!=Before&&m_ObjectCount>1)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			StorageNode * pBefore=(StorageNode *)Before;
			PickNode(pNode);
			InsertNodeBefore(pNode,pBefore);
			return TRUE;
		}
		return FALSE;
	}
	BOOL MoveAfterTo(LPVOID Pos,LPVOID After)
	{
		if(Pos&&Pos!=After&&m_ObjectCount>1)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			StorageNode * pAfter=(StorageNode *)After;
			PickNode(pNode);
			InsertNodeAfter(pNode,pAfter);
			return TRUE;
		}
		return FALSE;
	}

protected:
	StorageNode * NewNode()
	{
		if(m_pFreeList)
		{			
			StorageNode * pNode;

			pNode=m_pFreeList;
			m_pFreeList=pNode->pNext;
			if(m_pFreeList)
				m_pFreeList->pPrev=NULL;
			pNode->pPrev=NULL;
			pNode->pNext=NULL;			
			pNode->IsFree=false;	
			if(IsDynamic)
				pNode->pObject=new T;
			m_ObjectCount++;
			return pNode;
		}
		return NULL;
	}
	void InsertNodeBefore(StorageNode * pNode,StorageNode * pBefore)
	{
		if(pBefore==NULL)
			pBefore=m_pObjectListHead;
		if(pBefore)
		{
			pNode->pPrev=pBefore->pPrev;
			pNode->pNext=pBefore;
			if(pBefore->pPrev)
				pBefore->pPrev->pNext=pNode;
			pBefore->pPrev=pNode;
		}
		else
		{
			m_pObjectListHead=pNode;
			m_pObjectListTail=pNode;
		}
		if(pBefore==m_pObjectListHead)
		{
			m_pObjectListHead=pNode;
		}
	}
	void InsertNodeAfter(StorageNode * pNode,StorageNode * pAfter)
	{
		if(pAfter==NULL)
			pAfter=m_pObjectListTail;
		if(pAfter)
		{
			pNode->pPrev=pAfter;
			pNode->pNext=pAfter->pNext;
			if(pAfter->pNext)
				pAfter->pNext->pPrev=pNode;
			pAfter->pNext=pNode;
		}
		else
		{
			m_pObjectListHead=pNode;
			m_pObjectListTail=pNode;
		}
		if(pAfter==m_pObjectListTail)
		{
			m_pObjectListTail=pNode;
		}
	}
	void PickNode(StorageNode * pNode)
	{
		if(pNode->pPrev)
			pNode->pPrev->pNext=pNode->pNext;
		if(pNode->pNext)
			pNode->pNext->pPrev=pNode->pPrev;
		if(pNode==m_pObjectListHead)
			m_pObjectListHead=pNode->pNext;
		if(pNode==m_pObjectListTail)
			m_pObjectListTail=pNode->pPrev;
		pNode->pPrev=NULL;
		pNode->pNext=NULL; 
	}
	void DeleteNode(StorageNode * pNode)
	{	
		PickNode(pNode);
		pNode->IsFree=true;		
		pNode->pPrev=NULL;
		pNode->pNext=m_pFreeList;
		if(IsDynamic)
			SAFE_DELETE(pNode->pObject);
		if(m_pFreeList)
			m_pFreeList->pPrev=pNode;
		m_pFreeList=pNode;
		m_ObjectCount--;
	}

};