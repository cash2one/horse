/******************************************************************************
* Copyright (c) 2003,�Ϻ������з�����
* All rights reserved.
* 
* �ļ����ƣ�DBErrHandler.h
* ժ    Ҫ����ɫ�������Ĵ�����
* 
* ��ǰ�汾��1.0
* ��    �ߣ�buffer001
* �������ڣ�2006��03��14��
**********************************************************************************/
#include "StdAfx.h"
#include ".\dberrhandle.h"

CDBErrHandle::CDBErrHandle(void)
{
	SS_TRY_BEGIN_;
	SS_TRY_END_();
}

CDBErrHandle::~CDBErrHandle(void)
{
	SS_TRY_BEGIN_;
	SS_TRY_END_();
}

void SDAPI CDBErrHandle::OnError(int nErrCode, int nDbSn)
{
	SS_TRY_BEGIN_;
	FILE* fp = fopen("DBError.log", "at");
	if(fp)
	{
		char pszBuf[100];
		_snprintf(pszBuf, SIZEOF_STR(pszBuf), "DB��������[%d]", nErrCode);
		fputs(pszBuf, fp);
	}
	fclose(fp);
	SS_TRY_END_();
}