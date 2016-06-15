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
	�ܵ�������ܵ���һ�˷��͵�����
	!param[in] wBusinessID WORD   ���ݵ�ҵ��ID
	!param[in] pData const char* ������ָ��
	!param[in] nLen int   ����������
	*/

	void SDAPI OnRecv(WORD wBusinessID, const char *pData, int nLen);

	/*!func
	����ص�����
	!param[in] wBusinessID WORD   ҵ��ID
	!param[in] nErrCode int �������
	!remarks nErrCodeΪ0��ʾ�ɹ��������ش�����
	*/
	void SDAPI OnReport(WORD wBusinessID, int nErrCode);
private:
	UINT				m_nServerID;
	ISDPipe*			m_pPipe;
	char				m_sAckData[1000];
};
