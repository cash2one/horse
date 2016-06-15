/******************************************************************************
* Copyright (c) 2012,�Ϻ������з�����
* All rights reserved.
* 
* �ļ����ƣ�DBEventEx.h
* ժ    Ҫ����ϷDB�¼�
* 
* ��ǰ�汾��1.0
* ��    �ߣ�buffer001
* �������ڣ�2006��09��18��
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

	//ȡ��DBIndex
	int  SDAPI GetDBSn(void)
	{
		SS_TRY_BEGIN_;
		return 0;
		SS_TRY_END_();
		return 0;
	}
	
    //�¼������ͷ�
    void SDAPI Release(void)
	{
		SS_TRY_BEGIN_;
		SS_TRY_END_();
	}

protected:
	string					m_strPTID;				//PTID
	ISDMySqlRecordSet*		m_pRecordSet;			//���ʽ����
	bool					m_bQueryRes;			//���ʽ��
};

