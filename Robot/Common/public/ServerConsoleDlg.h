// AuthenServerDlg.h : 头文件
//

#pragma once

#include "PublicResource.h"


// CServerConsoleDlg 对话框
class CServerConsoleDlg : public CDialog
{
// 构造
public:
	CServerConsoleDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SERVER_CONSOLE };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnConsoleMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	void OnOK();
	void OnCancel();
	//取得版本
	void ShowVersion();

protected:
	virtual void OnCommand(LPCTSTR szCommand);
	virtual void OnConsoleMsg(PANEL_MSG * pMsg);
	virtual void OnLogMsg(LPCTSTR szLogMsg);
public:
	CEdit m_edMsgWnd;
	void OnTimer(UINT nIDEvent);
	CString m_ClientCount;
	CString m_CycleTime;
	CString m_TCPRecv;
	CString m_TCPSend;
	CString m_UDPRecv;
	CString m_UCPSend;
	CString	m_Command;
	void OnBnClickedExecCommand();
};
