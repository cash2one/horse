#pragma once

#include <WinSock2.h>
#include <windows.h>

#define _MAX_MSGSIZE	16 * 1024		// �ݶ�һ����Ϣ���Ϊ16k
#define BLOCKSECONDS	30				// INIT��������ʱ��
#define INBUFSIZE		(64*1024)		//?	����ߴ�������汨�����  �������ݵĻ���
#define OUTBUFSIZE		(8*1024)		//? ����ߴ�������汨������� �������ݵĻ��棬��������8Kʱ��FLUSHֻ��ҪSENDһ��

class CGameSocket
{
public:
	CGameSocket(void);
	bool	Create(const char* pszServerIP, int nServerPort, int nBlockSec = BLOCKSECONDS, bool bKeepAlive = false);
	bool	SendMsg(void* pBuf, int nSize);
	bool	SendIndirectMsg(void* pBuf, int nSize);//ֱ�ӷ��͵������� ,�����뻺����
	bool	ReceiveMsg(void* pBuf, int& nSize);
	bool	Flush(void);
	bool	Check(void);
	void	Reset(void);
	SOCKET	GetSocket(void) const { return m_sockClient; }
	//
	void ExceptionExit(int nCode);//�쳣�˳�

private:
	bool	recvFromSock(void);		// �������ж�ȡ�����ܶ������
	bool    hasError();			// �Ƿ�������ע�⣬�첽ģʽδ��ɷǴ���
	void    closeSocket();

	SOCKET	m_sockClient;

	// �������ݻ���
	char	m_bufOutput[OUTBUFSIZE];	//? ���Ż�Ϊָ������
	int		m_nOutbufLen;

	// ���λ�����
	char	m_bufInput[INBUFSIZE];
	int		m_nInbufLen;
	int		m_nInbufStart;				// INBUFʹ��ѭ��ʽ���У��ñ���Ϊ������㣬0 - (SIZE-1)
};