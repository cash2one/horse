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
#include "StdAfx.h"

CBaseTimer::CBaseTimer(int nTimerID, int nElapseTime)
{
	SS_TRY_BEGIN_;
	m_nTimerID		=	nTimerID;
	m_nElapseTime	=	nElapseTime;
	SS_TRY_END_();
}

CBaseTimer::~CBaseTimer()
{
	SS_TRY_BEGIN_;
	SS_TRY_END_();
}