#pragma once

template < class T>
class CircleQueue
{
	unsigned int m_nMaxSize;
	volatile int m_nReadPos;
	volatile int m_nWritePos;

	T* pBuffer;
public:
	CircleQueue()
		:m_nMaxSize(0)
		,m_nReadPos(0)
		,m_nWritePos(0)
		,pBuffer(NULL)
	{
		
	}
	~CircleQueue(void)
	{
		if( pBuffer )
		{
			delete[] pBuffer;
		}
	}

	int InitQueue(unsigned int nSize)
	{
		ASSERT( nSize < 0x7FFFF && nSize >= 8);
		m_nReadPos = 0;
		m_nWritePos = 0;
		m_nMaxSize= nSize;
		
		pBuffer = new T[nSize];

		return TRUE;
	}

	T* PopOne()
	{
		T* pItem = NULL;
		
		if(  m_nWritePos != m_nReadPos  )
		{
			pItem = &(pBuffer[m_nReadPos]);
			m_nReadPos = (m_nReadPos +1+m_nMaxSize) % m_nMaxSize;
		}
		
		return pItem;
	}

	int PushOne(const T* pItem)
	{
		int itemNum = m_nWritePos - m_nReadPos;
		if( itemNum <= -3 || (itemNum >= 0 && itemNum <= m_nMaxSize - 3))
		{
			pBuffer[m_nWritePos] = *pItem;

			m_nWritePos = (m_nWritePos +1+m_nMaxSize) % m_nMaxSize;
			if( itemNum == 0)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
		else
		{
			return 0;
		}
	}

	int GetObjectCount()
	{
		return (m_nWritePos +m_nMaxSize - m_nReadPos)%m_nMaxSize;
	}
};
