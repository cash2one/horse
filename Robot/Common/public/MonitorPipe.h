#pragma once
struct MoHeader;

class CMonitorPipe :
	public ISDPipeSink
{
public:
	CMonitorPipe(void);
	virtual ~CMonitorPipe(void);
	void ProcessPacket(const MoHeader* pHeader, const char* pData, int nLen);
	void SetPipe(ISDPipe* pPipe);
	//------------------------------------------------------------------
	//							ISDPipeSink
	//------------------------------------------------------------------
	/*!func
	管道对象接受到另一端发送的数据
	!param[in] wBusinessID WORD   数据的业务ID
	!param[in] pData const char* 数据区指针
	!param[in] nLen int   数据区长度
	*/

	void SDAPI OnRecv(WORD wBusinessID, const char *pData, int nLen);

	/*!func
	出错回调函数
	!param[in] wBusinessID WORD   业务ID
	!param[in] nErrCode int 错误代码
	!remarks nErrCode为0表示成功，出错返回错误码
	*/
	void SDAPI OnReport(WORD wBusinessID, int nErrCode);
private:
	UINT				m_nServerID;
	ISDPipe*			m_pPipe;
	char				m_sAckData[1000];
};
