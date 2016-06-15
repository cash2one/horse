/******************************************************************************
* Copyright (c) 2003,�Ϻ������з�����
* All rights reserved.
* 
* �ļ����ƣ�DBEventEx.cpp
* ժ    Ҫ����ϷDB�¼�
* 
* ��ǰ�汾��1.0
* ��    �ߣ�buffer001
* �������ڣ�2006��09��18��
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
