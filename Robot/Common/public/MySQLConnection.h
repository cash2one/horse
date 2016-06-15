#pragma once
#include "dbinterfaces.h"




typedef MYSQL * MYSQL_HANDLE;
typedef MYSQL_RES * MYSQL_RES_HANDLE;

class CMySQLDatabase;

class CMySQLConnection :
	public IDBConnection
{
private:
	MYSQL	m_Mysql;
protected:
	MYSQL_HANDLE		m_MySQLHandle;
	//CMySQLDatabase *	m_pDatabase;
	
	DECLARE_CLASS_INFO(CMySQLConnection)
public:
	CMySQLConnection(void);
	virtual ~CMySQLConnection(void);

	int Init();
	virtual void Destory();

	virtual int Connect(LPCTSTR ConnectStr);
	virtual int Disconnect();
	virtual BOOL IsConnected();
	virtual int ExecuteSQL(LPCSTR SQLStr,int StrLen=0);		
	virtual int GetResults(IDBRecordSet * pDBRecordset);
	virtual int NextResults();	
	virtual int EnableTransaction(BOOL IsEnable);
	virtual int Commit();
	virtual int RollBack();

	virtual UINT GetLastDatabaseErrorCode();
	virtual LPCTSTR GetLastDatabaseErrorString();

	virtual int TranslateString(LPCTSTR szSource,int SrcLen,LPTSTR szTarget,int MaxLen, bool isBlob = false);

	void ProcessErrorMsg(LPCTSTR Msg);
	virtual int GetMySqlInsertID(unsigned long long& lID);	
protected:
	//int FetchStaticResult(CMySQLStaticRecordSet * pDBRecordset);
	int FetchResult(CMySQLRecordSet * pDBRecordset);
	DWORD FetchConnectFlags(LPCTSTR FlagsStr);
};

