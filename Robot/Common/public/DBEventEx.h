/******************************************************************************
* Copyright (c) 2012,上海好玩研发中心
* All rights reserved.
* 
* 文件名称：DBEventEx.h
* 摘    要：游戏DB事件
* 
* 当前版本：1.0
* 作    者：buffer001
* 建立日期：2006年09月18日
**********************************************************************************/
#pragma once


class CDBAutoTransaction
{
protected:
	ISDMySqlConn *m_pDBConnection;
public:
	CDBAutoTransaction(ISDMySqlConn *pDBConnection)
	{
		m_pDBConnection=pDBConnection;
		m_pDBConnection->ExecuteSql("Start Transaction");
	}
	~CDBAutoTransaction()
	{
		if(m_pDBConnection)
			m_pDBConnection->ExecuteSql("Rollback");
	}
	void Commit()
	{
		if(m_pDBConnection)
		{
			m_pDBConnection->ExecuteSql("Commit");
			m_pDBConnection=NULL;
		}
	}
};

class CDBEventEx : public ISDDBEventEx
{
public:
	CDBEventEx();
	CDBEventEx(const char* pPTID);
	virtual ~CDBEventEx(void);

	//取得DBIndex
	int  SDAPI GetDBSn(void)
	{
		SS_TRY_BEGIN_;
		return 0;
		SS_TRY_END_();
		return 0;
	}
	
    //事件对象释放
    void SDAPI Release(void)
	{
		SS_TRY_BEGIN_;
		SS_TRY_END_();
	}

protected:
	string					m_strPTID;				//PTID
	ISDMySqlRecordSet*		m_pRecordSet;			//访问结果集
	bool					m_bQueryRes;			//访问结果
};

