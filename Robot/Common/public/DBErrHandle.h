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
#pragma once
class CDBErrHandle : public ISDDBErrHandler
{
public:
	CDBErrHandle(void);
	virtual ~CDBErrHandle(void);

	void SDAPI OnError(int nErrCode, int nDbSn);
};
