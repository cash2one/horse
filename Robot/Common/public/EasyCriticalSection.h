#pragma once
#include "WinBase.h"

class CEasyCriticalSection
{
protected:
	CRITICAL_SECTION	m_critical_sec;
public:
	CEasyCriticalSection(void)
	{
		InitializeCriticalSection( &m_critical_sec );
	}
	CEasyCriticalSection(CEasyCriticalSection& Section)
	{
		m_critical_sec=Section.m_critical_sec;		
	}
	~CEasyCriticalSection(void)
	{
		DeleteCriticalSection( &m_critical_sec );
	}
	void Lock()
	{
		EnterCriticalSection( &m_critical_sec );
	}
	void Unlock()
	{
		LeaveCriticalSection( &m_critical_sec );
	}
	BOOL TryLock()
	{
#if(_WIN32_WINNT >= 0x0400)
		return TryEnterCriticalSection(&m_critical_sec );
#else
		return TRUE;
#endif
	}
};
