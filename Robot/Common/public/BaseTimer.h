/******************************************************************************
* Copyright (c) 2003,�Ϻ������з�����
* All rights reserved.
* 
* �ļ����ƣ�BaseTimer.h
* ժ    Ҫ����Ϸ������ʱ�ӻ���
* 
* ��ǰ�汾��1.0
* ��    �ߣ�buffer001
* �������ڣ�2006��11��16��
**********************************************************************************/
#pragma once

class CBaseTimer : public ISDTimer
{
public:
	CBaseTimer(int nTimerID, int nElapseTime);
	virtual ~CBaseTimer(void);

	UINT GetTimerID()
	{
		SS_TRY_BEGIN_;
		return m_nTimerID;
		SS_TRY_END_();
		return 0;
	}

	int GetElapseTime()
	{
		SS_TRY_BEGIN_;
		return m_nElapseTime;
		SS_TRY_END_();
		return 60000;
	}

protected:
	UINT		m_nTimerID;		//ʱ��ID
	int			m_nElapseTime;	//ʱ����
};