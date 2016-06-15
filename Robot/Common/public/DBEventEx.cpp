/******************************************************************************
* Copyright (c) 2003,上海好玩研发中心
* All rights reserved.
* 
* 文件名称：DBEventEx.cpp
* 摘    要：游戏DB事件
* 
* 当前版本：1.0
* 作    者：buffer001
* 建立日期：2006年09月18日
**********************************************************************************/
#include "StdAfx.h"
#include "DBEventEx.h"

CDBEventEx::CDBEventEx()
{
	SS_TRY_BEGIN;
	m_pRecordSet	=	NULL;
	m_bQueryRes		=	false;		
	SS_TRY_END();
}

CDBEventEx::CDBEventEx(const char* pPTID)
{
	SS_TRY_BEGIN;
	m_strPTID		=	pPTID;
	m_pRecordSet	=	NULL;
	m_bQueryRes		=	false;		
	SS_TRY_END();
}

CDBEventEx::~CDBEventEx(void)
{
	SS_TRY_BEGIN;
	m_strPTID.clear();
	m_pRecordSet	=	NULL;
	m_bQueryRes		=	false;		
	SS_TRY_END();
}
