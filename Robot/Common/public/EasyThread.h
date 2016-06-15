#pragma once
#include "nameobject.h"

#define DEFAULT_THREAD_TERMINATE_TIME		30000

class CEasyThread :
	public CNameObject
{
protected:
	UINT			m_ThreadID;	
	HANDLE			m_hThread;	
	BOOL			m_IsSuspended;
	volatile BOOL	m_IsWorking;
	volatile BOOL	m_IsTerminate;

	DECLARE_CLASS_INFO(CEasyThread)
public:
	CEasyThread();
	virtual ~CEasyThread();

	///启动线程
	BOOL Start(BOOL IsSuspended = FALSE);

	///继续线程
	BOOL Resume();

	/// 挂起线程
	BOOL Suspend();

	///终止线程
	void Terminate();

	//强制结束
	void ForceTerminate();
	void SafeTerminate(DWORD Milliseconds=DEFAULT_THREAD_TERMINATE_TIME);

	//线程是否被结束了
	BOOL IsTerminate();

	//线程是否被挂起
	BOOL IsSuspend();

	///线程是否在工作
	BOOL IsWorking();

	///等待线程退出
	BOOL WaitForTerminate(DWORD Milliseconds=INFINITE);


	HANDLE GetThreadHandle();
	DWORD GetThreadID();
protected:
	virtual BOOL OnStart();
	virtual void Execute();
	virtual BOOL OnRun();
	virtual void OnTerminate();

	///线程例程
	static UINT WINAPI ThreadProc(LPVOID pParam);
};

inline BOOL CEasyThread::IsTerminate()
{
	return m_IsTerminate;
}
inline BOOL CEasyThread::IsSuspend()
{
	return m_IsSuspended;
}
inline BOOL CEasyThread::IsWorking()
{
	return m_IsWorking;
}

inline HANDLE CEasyThread::GetThreadHandle()
{
	return m_hThread;
}

inline DWORD CEasyThread::GetThreadID()
{
	return m_ThreadID;
}