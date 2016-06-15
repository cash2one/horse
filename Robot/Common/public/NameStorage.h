#pragma once
#include <vector>
#include <map>
#include <string>

template < class T ,bool IsDynamic=false,bool NoCase=false>
class CNameStorage
{
protected:
	struct StorageNode
	{
		UINT ID;
		std::string ObjectName;
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

	std::map<std::string,StorageNode *> m_NameMap;
public:
	CNameStorage()
	{
		m_pObjectBuff=NULL;
		m_pFreeList=NULL;
		m_pObjectListHead=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;
		m_ObjectBuffSize=0;		
	}
	~CNameStorage()
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
		Clear();		
		SAFE_DELETE_ARRAY(m_pObjectBuff);
		m_pFreeList=NULL;
		m_pObjectListHead=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;
		m_ObjectBuffSize=0;		
		m_NameMap.clear();
	}
	void Clear()
	{
		for(UINT i=0;i<m_ObjectBuffSize;i++)
		{
			m_pObjectBuff[i].ID=i+1;			
			i==0?m_pObjectBuff[i].pPrev=NULL:m_pObjectBuff[i].pPrev=&(m_pObjectBuff[i-1]);
			i==m_ObjectBuffSize-1?m_pObjectBuff[i].pNext=NULL:m_pObjectBuff[i].pNext=&(m_pObjectBuff[i+1]);
			m_pObjectBuff[i].IsFree=true;
			SAFE_DELETE(m_pObjectBuff[i].pObject);
		}
		m_pFreeList=m_pObjectBuff;
		m_pObjectListHead=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;	
	}
	UINT NewObject(T** ppObject,LPCTSTR Name)
	{
		StorageNode * pNode=NewNode(Name);
		if(pNode)
		{	
			InsertNodeAfter(pNode,m_pObjectListTail);			
			*ppObject=pNode->pObject;
			return pNode->ID;
		}
		*ppObject=NULL;
		return 0;
	}	

	UINT AddObject(T& Object,LPCTSTR Name)
	{
		UINT ID;
		T * pObject;
		ID=NewObject(&pObject,Name);
		if(ID)
		{
			*pObject=Object;
			return ID;
		}
		return 0;
	}
	LPVOID GetObjectPos(UINT ID)
	{
		if(ID>0&&ID<=m_ObjectBuffSize)
		{
			ID--;
			if(!m_pObjectBuff[ID].IsFree)
				return &(m_pObjectBuff[ID]);
		}
		return NULL;
	}
	T* GetObject(LPVOID Pos)
	{
		StorageNode * pNode=(StorageNode *)Pos;	
		if(pNode)
			return pNode->pObject;
		else
			return NULL;
	}	
	T* GetObject(UINT ID)
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
	LPVOID GetObjectPos(LPCTSTR Name)
	{
		std::map<std::string,StorageNode *>::iterator	itr;

		std::string ObjName=Name;
		if(NoCase)
			strupr((char *)ObjName.c_str());
		itr=m_NameMap.find(ObjName);
		if(itr!=m_NameMap.end())
		{					
			return itr->second;
		}
		return NULL;
	}
	T* GetObject(LPCTSTR Name)
	{
		StorageNode * pNode=(StorageNode *)GetObjectPos(Name);	
		if(pNode)
			return pNode->pObject;
		else
			return NULL;
	}	
	bool DeleteObject(LPCTSTR Name)
	{
		StorageNode * pNode=(StorageNode *)GetObjectPos(Name);
		if(pNode)
		{
			DeleteNode(pNode);
			return true;
		}
		return false;;
	}	
	UINT GetObjectCount()
	{
		return m_ObjectCount;
	}

	LPVOID GetFirstObjectPos()
	{
		return m_pObjectListHead;
	}

	LPVOID GetLastObjectPos()
	{
		return m_pObjectListTail;
	}

	T* GetNext(LPVOID& Pos)
	{
		if(Pos)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			Pos=pNode->pNext;
			return pNode->pObject;
		}
		return NULL;
	}

	T* GetPrev(LPVOID& Pos)
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
			InsertNodeBefore(pNode,Before);
		}
	}
	BOOL MoveAfterTo(LPVOID Pos,LPVOID After)
	{
		if(Pos&&Pos!=After&&m_ObjectCount>1)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			StorageNode * pAfter=(StorageNode *)After;
			PickNode(pNode);
			InsertNodeAfter(pNode,After);
		}
	}

protected:
	StorageNode * NewNode(LPCTSTR Name)
	{
		if(m_pFreeList)
		{			
			StorageNode * pNode;

			
			pNode=m_pFreeList;
			m_pFreeList=pNode->pNext;
			if(m_pFreeList)
				m_pFreeList->pPrev=NULL;

			pNode->ObjectName=Name;
			if(NoCase)
				strupr((char *)pNode->ObjectName.c_str());
			m_NameMap[pNode->ObjectName]=pNode;

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
		m_NameMap.erase(pNode->ObjectName);
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
