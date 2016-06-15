#pragma once

#define WM_USER_LOG_MSG				WM_USER+1001
#define WM_USER_CLIENT_NUM			WM_USER+1002

#define MAX_CONSOLE_MSG_LEN			1024

enum CONSOLE_MSG_TYPE
{
	CON_MSG_LOG,
};

struct PANEL_MSG
{
	UINT	ID;
	UINT	MsgType;
	char	Msg[MAX_CONSOLE_MSG_LEN];
};

struct SERVER_INFO
{
	UINT	ClientCount;
	float	CycleTime;
	float	TCPRecvFlow;
	float	TCPSendFlow;
	float	UDPRecvFlow;
	float	UDPSendFlow;

};

class CControlPanel :
	public CStaticObject<CControlPanel>
{
protected:
	HWND								m_hWnd;
	CThreadSafeIDStorage<PANEL_MSG>		m_MsgPool;
	CThreadSafeIDStorage<PANEL_MSG>		m_CommandPool;
	SERVER_INFO							m_ServerInfo;
	CEasyCriticalSection				m_CriticalSection;
	
public:
	CControlPanel(void);
	~CControlPanel(void);

	void SetHwnd(HWND hWnd)
	{
		m_hWnd=hWnd;
	}

	BOOL PushMsg(LPCTSTR szMsg);
	BOOL PushMsg(UINT Type,LPCVOID pData,int Len);
	BOOL ReleaseMsg(UINT ID);

	void SetClientNum(int Num);

	void SendWndMsg(UINT MsgID,WPARAM wParam,LPARAM lParam);

	BOOL PushCommand(LPCTSTR szCommand);
	PANEL_MSG * GetCommand();
	BOOL ReleaseCommand(UINT ID);

	void SetServerInfo(SERVER_INFO& ServerInfo);
	void GetServerInfo(SERVER_INFO& ServerInfo);
};
