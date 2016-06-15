// AuthenServerDlg.h : ͷ�ļ�
//

#pragma once

#include "PublicResource.h"


// CServerConsoleDlg �Ի���
class CServerConsoleDlg : public CDialog
{
// ����
public:
	CServerConsoleDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERVER_CONSOLE };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnConsoleMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	void OnOK();
	void OnCancel();
	//ȡ�ð汾
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
