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

	///�����߳�
	BOOL Start(BOOL IsSuspended = FALSE);

	///�����߳�
	BOOL Resume();

	/// �����߳�
	BOOL Suspend();

	///��ֹ�߳�
	void Terminate();

	//ǿ�ƽ���
	void ForceTerminate();
	void SafeTerminate(DWORD Milliseconds=DEFAULT_THREAD_TERMINATE_TIME);

	//�߳��Ƿ񱻽�����
	BOOL IsTerminate();

	//�߳��Ƿ񱻹���
	BOOL IsSuspend();

	///�߳��Ƿ��ڹ���
	BOOL IsWorking();

	///�ȴ��߳��˳�
	BOOL WaitForTerminate(DWORD Milliseconds=INFINITE);


	HANDLE GetThreadHandle();
	DWORD GetThreadID();
protected:
	virtual BOOL OnStart();
	virtual void Execute();
	virtual BOOL OnRun();
	virtual void OnTerminate();

	///�߳�����
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