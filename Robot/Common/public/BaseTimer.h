/******************************************************************************
* Copyright (c) 2003,上海好玩研发中心
* All rights reserved.
* 
* 文件名称：BaseTimer.h
* 摘    要：游戏服务器时钟基类
* 
* 当前版本：1.0
* 作    者：buffer001
* 建立日期：2006年11月16日
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
	UINT		m_nTimerID;		//时钟ID
	int			m_nElapseTime;	//时间间隔
};