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