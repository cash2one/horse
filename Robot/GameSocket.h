#pragma once

#include <WinSock2.h>
#include <windows.h>

#define _MAX_MSGSIZE	16 * 1024		// 暂定一个消息最大为16k
#define BLOCKSECONDS	30				// INIT函数阻塞时间
#define INBUFSIZE		(64*1024)		//?	具体尺寸根据剖面报告调整  接收数据的缓存
#define OUTBUFSIZE		(8*1024)		//? 具体尺寸根据剖面报告调整。 发送数据的缓存，当不超过8K时，FLUSH只需要SEND一次

class CGameSocket
{
public:
	CGameSocket(void);
	bool	Create(const char* pszServerIP, int nServerPort, int nBlockSec = BLOCKSECONDS, bool bKeepAlive = false);
	bool	SendMsg(void* pBuf, int nSize);
	bool	SendIndirectMsg(void* pBuf, int nSize);//直接发送到网络上 ,不放入缓存中
	bool	ReceiveMsg(void* pBuf, int& nSize);
	bool	Flush(void);
	bool	Check(void);
	void	Reset(void);
	SOCKET	GetSocket(void) const { return m_sockClient; }
	//
	void ExceptionExit(int nCode);//异常退出

private:
	bool	recvFromSock(void);		// 从网络中读取尽可能多的数据
	bool    hasError();			// 是否发生错误，注意，异步模式未完成非错误
	void    closeSocket();

	SOCKET	m_sockClient;

	// 发送数据缓冲
	char	m_bufOutput[OUTBUFSIZE];	//? 可优化为指针数组
	int		m_nOutbufLen;

	// 环形缓冲区
	char	m_bufInput[INBUFSIZE];
	int		m_nInbufLen;
	int		m_nInbufStart;				// INBUF使用循环式队列，该变量为队列起点，0 - (SIZE-1)
};