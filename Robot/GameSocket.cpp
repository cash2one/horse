#include "stdafx.h"
#include "GameSocket.h"
#include <iostream>

#pragma comment(lib,"ws2_32")

using namespace std;

CGameSocket::CGameSocket()
{ 
	// ��ʼ��
	memset(m_bufOutput, 0, sizeof(m_bufOutput));
	memset(m_bufInput, 0, sizeof(m_bufInput));
}

void CGameSocket::closeSocket()
{
	closesocket(m_sockClient);
// 	WSACleanup();
}

bool CGameSocket::Create(const char* pszServerIP, int nServerPort, int nBlockSec, bool bKeepAlive /*= FALSE*/)
{
	// ������
	if(pszServerIP == 0 || strlen(pszServerIP) > 15) {
		return false;
	}

	WSADATA wsaData;
	WORD version = MAKEWORD(2, 2);
	int ret = WSAStartup(version, &wsaData);//win sock start up
	if (ret != 0) {
		return false;
	}

	// �������׽���
	m_sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_sockClient == INVALID_SOCKET) {
		closeSocket();
		return false;
	}

	// ����SOCKETΪKEEPALIVE
	if(true)
	//if(bKeepAlive)
	{
		int		optval=1;
		if(setsockopt(m_sockClient, SOL_SOCKET, SO_KEEPALIVE, (char *) &optval, sizeof(optval)))
		{
			closeSocket();
			return false;
		}
	}

	DWORD nMode = 1;
	int nRes = ioctlsocket(m_sockClient, FIONBIO, &nMode);
	if (nRes == SOCKET_ERROR) {
		closeSocket();
		return false;
	}

	unsigned long serveraddr = inet_addr(pszServerIP);
	if(serveraddr == INADDR_NONE)	// ���IP��ַ��ʽ����
	{
		closeSocket();
		//
		//ExceptionExit(0x00000001);
		cout<<"IP��ַ��ʽ����"<<endl;
		return false;
	}

	sockaddr_in	addr_in;
	memset((void *)&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(nServerPort);
	addr_in.sin_addr.s_addr = serveraddr;

	if(connect(m_sockClient, (sockaddr *)&addr_in, sizeof(addr_in)) == SOCKET_ERROR) {
		if (hasError()) {
			closeSocket();
			//
			//ExceptionExit(0x00000002);
			//MsgBox("","����ʧ��");
			return false;
		}
		else	// WSAWOLDBLOCK
		{
			timeval timeout;
			timeout.tv_sec	= nBlockSec;
			timeout.tv_usec	= 0;
			fd_set writeset, exceptset;
			FD_ZERO(&writeset);
			FD_ZERO(&exceptset);
			FD_SET(m_sockClient, &writeset);
			FD_SET(m_sockClient, &exceptset);

			int ret = select(m_sockClient+1 /*FD_SETSIZE*/, NULL, &writeset, &exceptset, &timeout);
			if (ret == 0 || ret < 0) {
				closeSocket();
				//
				//MsgBox("","����ʧ��");
				//ExceptionExit(0x00000003);
				return false;
			} else	// ret > 0
			{
				ret = FD_ISSET(m_sockClient, &exceptset);
				if(ret)		// or (!FD_ISSET(m_sockClient, &writeset)
				{
					closeSocket();
					//
					//MsgBox("","������δ����");
					//ExceptionExit(0x00000004);
					return false;
				}
			}
		}
	}

	m_nInbufLen		= 0;
	m_nInbufStart	= 0;
	m_nOutbufLen	= 0;

	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 0;// 500;//���Źر�
	setsockopt(m_sockClient, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));

	return true;
}
//
bool CGameSocket::SendIndirectMsg(void* pBuf, int nSize)
{
	if(pBuf == 0 || nSize <= 0) 
		return false;

	if (m_sockClient == INVALID_SOCKET) 
		return false;
	// ����һ������
	int	outsize = 0;
	int nTotalSize = nSize;
	char* pChar = (char* )pBuf;
	while(nTotalSize > 0)
	{
		outsize = send(m_sockClient, pChar, nTotalSize, 0);
		if(outsize > 0) 
		{
			pChar+=outsize;
			nTotalSize -= outsize;

		} else 
		{
			if (hasError()) 
			{
				Reset();				
			}
			//
			return false;
		}
	}
	//
	return true;
}

bool CGameSocket::SendMsg(void* pBuf, int nSize)
{
	if(pBuf == 0 || nSize <= 0) {
		return false;
	}

	if (m_sockClient == INVALID_SOCKET) {
		return false;
	}

	// ���ͨѶ��Ϣ������
	int packsize = 0;
	packsize = nSize;

	// ���BUF���
	if(m_nOutbufLen + nSize > OUTBUFSIZE) {
		// ��������OUTBUF�е����ݣ������OUTBUF��
		Flush();
		if(m_nOutbufLen + nSize > OUTBUFSIZE) {
			// ������
			Reset();
			return false;
		}
	}
	// ������ӵ�BUFβ
	memmove(m_bufOutput + m_nOutbufLen, pBuf, nSize);
	m_nOutbufLen += nSize;
	return true;
}

bool CGameSocket::ReceiveMsg(void* pBuf, int& nSize)
{
	//������
	if(pBuf == NULL || nSize <= 0) {
		return false;
	}

	if (m_sockClient == INVALID_SOCKET) {
		return false;
	}

	// ����Ƿ���һ����Ϣ(С��2���޷���ȡ����Ϣ����)
	if(m_nInbufLen < 2) {
		//  ���û������ɹ�  ����   ���û��������ֱ�ӷ���
		if(!recvFromSock() || m_nInbufLen < 2) {		// ���m_nInbufLen������
			return false;
		}
	}

	// ����Ҫ��������Ϣ�Ĵ�С��һ����Ϣ����СΪ������Ϣ�ĵ�һ��16�ֽڣ�����Ϊ���λ�����������Ҫ�ֿ�����
	//int packsize = (unsigned char)m_bufInput[m_nInbufStart] +
	//	(unsigned char)m_bufInput[(m_nInbufStart + 1) % INBUFSIZE] * 256; // ע���ֽ��򣬸�λ+��λ
	//
	unsigned short* pPkgLen = (unsigned short*)&m_bufInput[m_nInbufStart]; 
	int packsize =int(*pPkgLen);

	// �����Ϣ���ߴ���� �ݶ����16k
	if (packsize <= 0 || packsize > _MAX_MSGSIZE) {
		m_nInbufLen = 0;		// ֱ�����INBUF
		m_nInbufStart = 0;
		return false;
	}

	// �����Ϣ�Ƿ�����(�����Ҫ��������Ϣ���ڴ�ʱ���������ݳ��ȣ���Ҫ�ٴ��������ʣ������)
	if (packsize > m_nInbufLen) {
		// ���û������ɹ�   ����    ��Ȼ�޷���ȡ�����������ݰ�  �򷵻أ�ֱ��ȡ��������
		if (!recvFromSock() || packsize > m_nInbufLen) {	// ���m_nInbufLen�Ѹ���
			return false;
		}
	}

	// ���Ƴ�һ����Ϣ
	if(m_nInbufStart + packsize > INBUFSIZE) {
		// ���һ����Ϣ�лؾ�����������ڻ��λ�������ͷβ��
		// �ȿ������λ�����ĩβ������
		int copylen = INBUFSIZE - m_nInbufStart;
		memmove(pBuf, m_bufInput + m_nInbufStart, copylen);

		// �ٿ������λ�����ͷ����ʣ�ಿ��
		memmove((unsigned char *)pBuf + copylen, m_bufInput, packsize - copylen);
		nSize = packsize;
	} else {
		// ��Ϣû�лؾ�����һ�ο�����ȥ
		memmove(pBuf, m_bufInput + m_nInbufStart, packsize);
		nSize = packsize;
	}

	// ���¼��㻷�λ�����ͷ��λ��
	m_nInbufStart = (m_nInbufStart + packsize) % INBUFSIZE;
	m_nInbufLen -= packsize;
	return	true;
}

bool CGameSocket::hasError()
{
	int err = WSAGetLastError();
	if(err != WSAEWOULDBLOCK) {
		return true;
	}

	return false;
}

// �������ж�ȡ�����ܶ�����ݣ�ʵ����������������ݵĵط�
bool CGameSocket::recvFromSock(void)
{
	if (m_nInbufLen >= INBUFSIZE || m_sockClient == INVALID_SOCKET) 
		return false;

	// ���յ�һ������
	int	savelen, savepos;			// ����Ҫ����ĳ��Ⱥ�λ��
	if(m_nInbufStart + m_nInbufLen < INBUFSIZE)		// INBUF�е�ʣ��ռ��л���
		savelen = INBUFSIZE - (m_nInbufStart + m_nInbufLen);		// �󲿿ռ䳤�ȣ����������ݵĳ���
	else 
		savelen = INBUFSIZE - m_nInbufLen;

	// ���������ݵ�ĩβ
	savepos = (m_nInbufStart + m_nInbufLen) % INBUFSIZE;
	//CHECKF(savepos + savelen <= INBUFSIZE);
	int inlen = recv(m_sockClient, m_bufInput + savepos, savelen, 0);
	if(inlen > 0) 
	{
		// �н��յ�����
		m_nInbufLen += inlen;

		if (m_nInbufLen > INBUFSIZE) 
			return false;

		// ���յڶ�������(һ�ν���û����ɣ����յڶ�������)
		if(inlen == savelen && m_nInbufLen < INBUFSIZE) 
		{
			int savelen2 = INBUFSIZE - m_nInbufLen;
			int savepos2 = (m_nInbufStart + m_nInbufLen) % INBUFSIZE;
			//			CHECKF(savepos + savelen <= INBUFSIZE);
			inlen = recv(m_sockClient, m_bufInput + savepos2, savelen2, 0);
			if(inlen > 0) 
			{
				m_nInbufLen += inlen;
				if (m_nInbufLen > INBUFSIZE) 
				{
					return false;
				}	
			} 
			else if(inlen == 0) 
			{
				Reset();
				return false;
			} 
			else 
			{
				// �����ѶϿ����ߴ��󣨰���������
				if (hasError()) 
				{
					Reset();
					return false;
				}
			}
		}
	} 
	else if(inlen == 0) 
	{
		Reset();
		return false;
	} 
	else 
	{
		// �����ѶϿ����ߴ��󣨰���������
		if (hasError()) 
		{
			Reset();
			return false;
		}
	}

	return true;
}

bool CGameSocket::Flush(void)		//? ��� OUTBUF > SENDBUF ����Ҫ���SEND����
{
	if (m_sockClient == INVALID_SOCKET) {
		return false;
	}

	if(m_nOutbufLen <= 0) {
		return true;
	}

	// ����һ������
	int	outsize;
	outsize = send(m_sockClient, m_bufOutput, m_nOutbufLen, 0);
	if(outsize > 0) {
		// ɾ���ѷ��͵Ĳ���
		if(m_nOutbufLen - outsize > 0) {
			memmove(m_bufOutput, m_bufOutput + outsize, m_nOutbufLen - outsize);
		}

		m_nOutbufLen -= outsize;

		if (m_nOutbufLen < 0) {
			return false;
		}
	} else {
		if (hasError()) {
			Reset();
			return false;
		}
	}

	return true;
}

bool CGameSocket::Check(void)
{
	// ���״̬
	if (m_sockClient == INVALID_SOCKET) {
		return false;
	}

	char buf[1];
	int	ret = recv(m_sockClient, buf, 1, MSG_PEEK);
	if(ret == 0) {
		Reset();
		return false;
	} else if(ret < 0) {
		if (hasError()) {
			Reset();
			return false;
		} else {	// ����
			return true;
		}
	} else {	// ������
		return true;
	}

	return true;
}

void CGameSocket::Reset(void)
{
	// �ر�
	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 500;
	int ret = setsockopt(m_sockClient, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));

	closeSocket();

	m_sockClient = INVALID_SOCKET;
	m_nInbufLen = 0;
	m_nInbufStart = 0;
	m_nOutbufLen = 0;

	memset(m_bufOutput, 0, sizeof(m_bufOutput));
	memset(m_bufInput, 0, sizeof(m_bufInput));	
}
//
void CGameSocket::ExceptionExit(int nCode)
{
	char szMsg[100]={0};
	sprintf(szMsg,"Network error!code=[0x%08X],game will be closed.",nCode);
	cout<<"ExceptionExit:"<<szMsg<<endl;
}