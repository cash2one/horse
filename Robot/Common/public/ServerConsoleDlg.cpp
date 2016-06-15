// AuthenServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ServerConsoleDlg.h"
#include ".\serverconsoledlg.h"




//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


#define PANEL_TIMER_ID		87832

extern char * g_ProgramName;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CServerConsoleDlg 对话框



CServerConsoleDlg::CServerConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerConsoleDlg::IDD, pParent)
	, m_ClientCount(_T("0"))
	, m_CycleTime(_T("0"))
	, m_TCPRecv(_T(""))
	, m_TCPSend(_T(""))
	, m_UDPRecv(_T(""))
	, m_UCPSend(_T(""))
{	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edMsgWnd);
	DDX_Text(pDX, IDC_ST_CLIENT_COUNT, m_ClientCount);
	DDX_Text(pDX, IDC_ST_CYCLE_TIME, m_CycleTime);
	DDX_Text(pDX, IDC_ST_TCP_RECV, m_TCPRecv);
	DDX_Text(pDX, IDC_ST_TCP_SEND, m_TCPSend);
	DDX_Text(pDX, IDC_ST_UDP_RECV, m_UDPRecv);
	DDX_Text(pDX, IDC_ST_UDP_SEND, m_UCPSend);
	DDX_Text(pDX, IDC_EDIT_COMMAND, m_Command);
}

BEGIN_MESSAGE_MAP(CServerConsoleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_USER_LOG_MSG, OnConsoleMsg)
	//}}AFX_MSG_MA
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_EXEC_COMMAND, OnBnClickedExecCommand)
END_MESSAGE_MAP()


// CServerConsoleDlg 消息处理程序

BOOL CServerConsoleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//初始化版本信息
	ShowVersion();
	
	

	CControlPanel::GetInstance()->SetHwnd(GetSafeHwnd());
	

	SetTimer(PANEL_TIMER_ID,SERVER_INFO_COUNT_TIME,NULL);
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CServerConsoleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerConsoleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CServerConsoleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServerConsoleDlg::OnOK()
{
	SS_TRY_BEGIN_;
	SS_TRY_END_();
}

void CServerConsoleDlg::OnCancel()
{
	SS_TRY_BEGIN_;
	int nRetCode = MessageBox("Are you sure to close the server?", "Warning", MB_YESNO);
	if(nRetCode == IDYES)
	{		
		CDialog::OnCancel();
	}
	SS_TRY_END_();
}

//取得版本
void CServerConsoleDlg::ShowVersion()
{
	SS_TRY_BEGIN;
	//取得版本号
	CString strGSPathName;
	GetModuleFileName(NULL,strGSPathName.GetBuffer(255),255);
	strGSPathName.ReleaseBuffer();
	DWORD dwHandle = -1;
	DWORD dwSize = GetFileVersionInfoSize(strGSPathName.GetBuffer(0), &dwHandle );
	LPVOID lpData = new BYTE[dwSize];
	GetFileVersionInfo( strGSPathName.GetBuffer(0), dwHandle, dwSize, lpData );	
	UINT uiLen = dwSize;	
	

	//取得版本号	
// 	LPVOID lpVersion = NULL;
// 	VerQueryValue(
// 			lpData, 
// 			TEXT( "\\StringFileInfo\\080403A8\\ProductVersion" ), 
// 			&lpVersion,
// 			&uiLen );
	CString Version;
	CString Caption;
	CString ProgramName;

	ProgramName=g_ProgramName;
	DWORD ver = GAME_VERSION;
	Version.Format("%d.%d.%d.%d",(ver & 0xff000000)>>24,(ver & 0xff0000)>>16,(ver & 0xff00)>>8,(ver & 0xff));
	Caption.Format("[%s V%s] StartTime:%s",ProgramName,Version,CTime::GetCurrentTime().Format("%m-%d %H:%M"));

	SetWindowText(Caption);

	delete [] lpData;
	SS_TRY_END();
}

LRESULT CServerConsoleDlg::OnConsoleMsg(WPARAM wParam, LPARAM lParam)
{
	SS_TRY_BEGIN_;
	PANEL_MSG * pMsg=(PANEL_MSG *)lParam;

	OnConsoleMsg(pMsg);

	CControlPanel::GetInstance()->ReleaseMsg(pMsg->ID);

	SS_TRY_END_();
	return 0;
}

void CServerConsoleDlg::OnConsoleMsg(PANEL_MSG * pMsg)
{
	SS_TRY_BEGIN_;
	switch(pMsg->MsgType)
	{
	case CON_MSG_LOG:
		OnLogMsg(pMsg->Msg);
		break;
	}
	SS_TRY_END_();
}

void CServerConsoleDlg::OnLogMsg(LPCTSTR szLogMsg)
{
	SS_TRY_BEGIN;

	int s1,s2;
	int sm1,sm2;
	int SelLine;
	int EndLine;

	m_edMsgWnd.SetRedraw(false);

	m_edMsgWnd.GetSel(sm1,sm2);
	m_edMsgWnd.SetSel(0,-1);
	m_edMsgWnd.GetSel(s1,s2);
	SelLine=m_edMsgWnd.LineFromChar(sm1);
	EndLine=m_edMsgWnd.GetLineCount()-1;
	if(s2>MAX_CONTROL_PANEL_MSG_LEN)
	{
		m_edMsgWnd.SetSel(0,-1);
		m_edMsgWnd.Clear();
		s2=0;
	}
	m_edMsgWnd.SetSel(s2,s2);
	m_edMsgWnd.ReplaceSel(CTime::GetCurrentTime().Format("%H:%M:%S :"));


	m_edMsgWnd.SetSel(0,-1);
	m_edMsgWnd.GetSel(s1,s2);
	m_edMsgWnd.SetSel(s2,s2);
	m_edMsgWnd.ReplaceSel(szLogMsg);


	m_edMsgWnd.SetSel(0,-1);
	m_edMsgWnd.GetSel(s1,s2);
	m_edMsgWnd.SetSel(s2,s2);	

	m_edMsgWnd.ReplaceSel("\r\n");



	m_edMsgWnd.SetRedraw(true);	


	if(SelLine==EndLine)
		m_edMsgWnd.LineScroll(m_edMsgWnd.GetLineCount());
	else
		m_edMsgWnd.SetSel(sm1,sm2);
	SS_TRY_END();
}

void CServerConsoleDlg::OnTimer(UINT nIDEvent)
{
	SS_TRY_BEGIN_;
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent==PANEL_TIMER_ID)
	{
		SERVER_INFO ServerInfo;
		CControlPanel::GetInstance()->GetServerInfo(ServerInfo);
		UpdateData(true);
		m_ClientCount.Format("%d",ServerInfo.ClientCount);
		m_CycleTime.Format("%06.6g",ServerInfo.CycleTime);
		m_TCPRecv.Format("%06.6g",ServerInfo.TCPRecvFlow);
		m_TCPSend.Format("%06.6g",ServerInfo.TCPSendFlow);
		m_UDPRecv.Format("%06.6g",ServerInfo.UDPRecvFlow);
		m_UCPSend.Format("%06.6g",ServerInfo.UDPSendFlow);
		UpdateData(false);
	}

	CDialog::OnTimer(nIDEvent);
	SS_TRY_END_();
}

void CServerConsoleDlg::OnBnClickedExecCommand()
{
	// TODO: 在此添加控件通知处理程序代码
	SS_TRY_BEGIN_;
	UpdateData(true);
	m_Command.Trim();
	if(!m_Command.IsEmpty())
		OnCommand(m_Command);
	m_Command.Empty();
	UpdateData(false);
	SS_TRY_END_();
}

void CServerConsoleDlg::OnCommand(LPCTSTR szCommand)
{
	SS_TRY_BEGIN_;
	CControlPanel::GetInstance()->PushCommand(szCommand);
	SS_TRY_END_();
}