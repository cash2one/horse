#pragma once
#include "MMSystem.h"

#define MAXTIME	(DWORD(0xffffffff))

inline DWORD	GetTimeToTime(DWORD t1,DWORD t2 )
{
	return (t1<=t2?(t2-t1):(MAXTIME-t1+t2));
}

class CEasyTimer
{
public:
	CEasyTimer():m_dwSavedTime(0),m_dwTimeoutTime(0)
	{

	}
	CEasyTimer(const CEasyTimer& Timer)
	{
		m_dwSavedTime=Timer.m_dwSavedTime;
		m_dwTimeoutTime=Timer.m_dwTimeoutTime;
	}
	inline DWORD	GetTime()
	{
		return timeGetTime();
	}
	VOID	SaveTime()
	{
		m_dwSavedTime = timeGetTime();
	}
	BOOL	IsTimeOut( DWORD starttime, DWORD timeout )
	{
		DWORD	dwTime = timeGetTime();
		if( GetTimeToTime(starttime, dwTime) >= timeout )
		{
			return TRUE;
		}
		return FALSE;
	}
	BOOL	IsTimeOut( DWORD dwTimeOut ) const
	{
		DWORD	dwTime = timeGetTime();
		if( GetTimeToTime(m_dwSavedTime, dwTime)>= dwTimeOut )
		{
			return TRUE;
		}
		return FALSE;
	}
	VOID	SetTimeOut( DWORD	dwTimeOut)
	{
		m_dwSavedTime = timeGetTime();
		m_dwTimeoutTime = dwTimeOut;
	}
	VOID	SetNextTimeOut( DWORD dwAppendTime )
	{
		m_dwTimeoutTime += dwAppendTime;
	}
	BOOL	IsTimeOut() const
	{
		DWORD	dwTime = timeGetTime();
		if( GetTimeToTime(m_dwSavedTime, dwTime)>= m_dwTimeoutTime )
			return TRUE;
		return FALSE;
	}
	DWORD	GetLeftTime() const
	{
		DWORD dwTime = timeGetTime();
		dwTime = GetTimeToTime( m_dwSavedTime, dwTime );
		if( dwTime >= m_dwTimeoutTime )return 0;
		return (m_dwTimeoutTime - dwTime);
	}
	DWORD GetPastTime()
	{
		DWORD dwTime = timeGetTime();
		return GetTimeToTime( m_dwSavedTime, dwTime );
	}
	DWORD	GetTimeOut() const{ return m_dwTimeoutTime;}
	DWORD	GetSavedTime() const{ return m_dwSavedTime;}
	void SetSavedTime(DWORD dwTime)
	{
		m_dwSavedTime=dwTime;
	}
private:
	DWORD	m_dwSavedTime;
	DWORD	m_dwTimeoutTime;
};