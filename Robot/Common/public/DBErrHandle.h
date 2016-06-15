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
#pragma once
class CDBErrHandle : public ISDDBErrHandler
{
public:
	CDBErrHandle(void);
	virtual ~CDBErrHandle(void);

	void SDAPI OnError(int nErrCode, int nDbSn);
};
