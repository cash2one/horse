/******************************************************************************
* Copyright (c) 2003,上海好玩研发中心
* All rights reserved.
* 
* 文件名称：DBErrHandler.h
* 摘    要：角色服务器的错误处理
* 
* 当前版本：1.0
* 作    者：buffer001
* 建立日期：2006年03月14日
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
		_snprintf(pszBuf, SIZEOF_STR(pszBuf), "DB操作错误[%d]", nErrCode);
		fputs(pszBuf, fp);
	}
	fclose(fp);
	SS_TRY_END_();
}