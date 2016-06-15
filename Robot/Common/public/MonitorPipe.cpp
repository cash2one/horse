#include "StdAfx.h"
#include "monitorpipe.h"
#include "../CorePublic/MonitorProto.h"

extern void GetServBasicConfig(MOPKG_CONFIG_ACK* pConfig);
extern void GetServCurStatistic(MOPKG_UPDATE_ACK* pData);

CMonitorPipe::CMonitorPipe(void)
{
	SS_TRY_BEGIN;
	m_nServerID = 0;
	m_pPipe = NULL;
	memset(m_sAckData, 0, sizeof(m_sAckData));
	SS_TRY_END();
}

CMonitorPipe::~CMonitorPipe(void)
{
	SS_TRY_BEGIN;
	m_pPipe = NULL;
	SS_TRY_END();
}

void CMonitorPipe::SetPipe(ISDPipe* pPipe)
{
	SS_TRY_BEGIN_;
	m_pPipe = pPipe;
	SD_PIPEID *pstID = (SD_PIPEID *)&m_nServerID;

	if(m_pPipe==NULL)
	{
		ErrorLog(5, "monitor pipe[%d-%d-%d-%d] disconnected"
			, pstID->byRegion, pstID->byReserved, pstID->byType, pstID->byIndex);
		ScreenLog("monitor pipe[%d-%d-%d-%d] disconnected"
			, pstID->byRegion, pstID->byReserved, pstID->byType, pstID->byIndex);
		return;
	}
	m_nServerID = m_pPipe->GetID();
	m_pPipe->SetSink(GM_BUSINESS_ID, this);
	ErrorLog(5, "monitor pipe[%d-%d-%d-%d] connected"
			, pstID->byRegion, pstID->byReserved, pstID->byType, pstID->byIndex);
	ScreenLog("monitor pipe[%d-%d-%d-%d] connected"
			, pstID->byRegion, pstID->byReserved, pstID->byType, pstID->byIndex);

	SS_TRY_END_();
}
/*!func
管道对象接受到另一端发送的数据
!param[in] wBusinessID WORD   数据的业务ID
!param[in] pData const char* 数据区指针
!param[in] nLen int   数据区长度
*/

void SDAPI CMonitorPipe::OnRecv(WORD wBusinessID, const char *pData, int nLen)
{
	SS_TRY_BEGIN_;
	if(nLen<sizeof(MoHeader) || reinterpret_cast<const MoHeader*>(pData)->length!=nLen)
		return ;

	ProcessPacket( reinterpret_cast<const MoHeader*>(pData), pData+sizeof(MoHeader), nLen-sizeof(MoHeader));
	SS_TRY_END_();
}

/*!func
出错回调函数
!param[in] wBusinessID WORD   业务ID
!param[in] nErrCode int 错误代码
!remarks nErrCode为0表示成功，出错返回错误码
*/
void SDAPI CMonitorPipe::OnReport(WORD wBusinessID, int nErrCode)
{
	SS_TRY_BEGIN_;
	if(wBusinessID == GM_BUSINESS_ID)
	{
		DWORD dwID=0 ;
		if(m_pPipe)
			dwID= m_pPipe->GetID();
		SD_PIPEID *pstID = (SD_PIPEID *)&dwID;
		ErrorLog(1, "Component error: monitor pipe[%d-%d-%d-%d] reports error[%d]", pstID->byRegion, pstID->byReserved, pstID->byType, pstID->byIndex, nErrCode);
	}
	SS_TRY_END_();
}

void CMonitorPipe::ProcessPacket(const MoHeader* pHeader, const char* pData, int nLen)
{
	SS_TRY_BEGIN_;
	switch(pHeader->type)
	{
	case MOID_REQ_CONFIG_INFO:
		{
			ASSERT(sizeof(MoHeader)+sizeof(MOPKG_CONFIG_ACK)<sizeof(m_sAckData));
			MoHeader* pHead = reinterpret_cast<MoHeader*>(m_sAckData);
			pHead->type = MOID_ACK_CONFIG_INFO;
			pHead->ret = mo_ret_sucess;
			pHead->length = sizeof(MoHeader)+sizeof(MOPKG_CONFIG_ACK);

			MOPKG_CONFIG_ACK* pData = reinterpret_cast<MOPKG_CONFIG_ACK*>(m_sAckData+sizeof(MoHeader));
			memset(pData, 0, sizeof(MOPKG_CONFIG_ACK));
			GetServBasicConfig(pData);

			m_pPipe->Send(GM_BUSINESS_ID, m_sAckData, pHead->length);
		}
		break;
	case MOID_REQ_UPDATE_DATA:
		{
			ASSERT(sizeof(MoHeader)+sizeof(MOPKG_UPDATE_ACK)<sizeof(m_sAckData));
			MoHeader* pHead = reinterpret_cast<MoHeader*>(m_sAckData);
			pHead->type = MOID_ACK_UPDATE_DATA;
			pHead->ret = mo_ret_sucess;
			pHead->length = sizeof(MoHeader);

			pHead->length += sizeof(MOPKG_UPDATE_ACK);
			MOPKG_UPDATE_ACK* pData = reinterpret_cast<MOPKG_UPDATE_ACK*>(m_sAckData+sizeof(MoHeader));
			memset(pData, 0, sizeof(MOPKG_UPDATE_ACK));
			GetServCurStatistic(pData);

			m_pPipe->Send(GM_BUSINESS_ID, m_sAckData, pHead->length);
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	SS_TRY_END_();
}
