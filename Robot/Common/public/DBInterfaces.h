#pragma once

#define VER_LANGUAGE 0
//数据类型
#include "DBTypes.h"



class IDatabase;
class IDBConnection;
class IDBRecordSet;
class IDBValue;

class IDatabase
	:public CNameObject
{
protected:
	DECLARE_CLASS_INFO_STATIC(IDatabase)
public:
	IDatabase();	
	virtual ~IDatabase();

	virtual IDBConnection * CreateConnection()=0;
	virtual int DeleteConnection(IDBConnection * pDBConnection)=0;	
};

class IDBConnection
	:public CNameObject
{
protected:
	DECLARE_CLASS_INFO_STATIC(IDBConnection)
public:
	IDBConnection();
	virtual ~IDBConnection();

	virtual int Connect(LPCTSTR ConnectStr)=0;
	virtual int Disconnect()=0;
	virtual BOOL IsConnected()=0;
	virtual int ExecuteSQL(LPCSTR SQLStr,int StrLen=0)=0;	
	virtual int GetResults(IDBRecordSet * pDBRecordset)=0;
	virtual int NextResults()=0;	
	virtual int EnableTransaction(BOOL IsEnable)=0;
	virtual int Commit()=0;
	virtual int RollBack()=0;	
	virtual UINT GetLastDatabaseErrorCode()=0;
	virtual LPCTSTR GetLastDatabaseErrorString()=0;
	virtual int TranslateString(LPCTSTR szSource,int SrcLen,LPTSTR szTarget,int MaxLen,bool isBlob = false)=0;
	virtual int GetMySqlInsertID(unsigned long long& lID)=0;
};

class IDBRecordSet
	:public CNameObject
{
protected:
	DECLARE_CLASS_INFO_STATIC(IDBRecordSet)
public:
	IDBRecordSet();
	virtual ~IDBRecordSet();


	virtual int GetRecordCount()=0;
	virtual int GetColumnCount()=0;
	virtual LPCTSTR GetColumnName(int Index)=0;
	virtual int GetIndexByColumnName(LPCTSTR Name)=0;
	virtual DB_COLUMN_INFO * GetColumnInfo(int Index)=0;

	virtual int GetParamCount()=0;
	virtual IDBValue& GetParam(int Index)=0;

	virtual IDBValue& GetField(int Index)=0;
	virtual IDBValue& GetField(LPCTSTR Name)=0;
	virtual int MoveFirst()=0;
	virtual int MoveLast()=0;
	virtual int MoveNext()=0;
	virtual int MovePrevious()=0;
	virtual int MoveTo(int Index)=0;	
	virtual bool IsEOF()=0;
	virtual bool IsBOF()=0;

};

class IDBValue
	:public CNameObject
{
protected:
	DECLARE_CLASS_INFO_STATIC(IDBValue)
public:
	IDBValue();
	virtual ~IDBValue();

	virtual DB_COLUMN_INFO * GetTypeInfo()=0;
	virtual int	GetLength()=0;
	virtual bool IsNull()=0;

	virtual operator bool()=0;	
	virtual operator char()=0;
	virtual operator unsigned char()=0;
	virtual operator short()=0;
	virtual operator unsigned short()=0;	
	virtual operator int()=0;
	virtual operator unsigned int()=0;
	virtual operator long()=0;
	virtual operator unsigned long()=0;
	virtual operator __int64()=0;
	virtual operator unsigned __int64()=0;
	virtual operator float()=0;
	virtual operator double()=0;
	virtual operator const char *()=0;	
	virtual operator const WCHAR *()=0;	
	virtual operator SYSTEMTIME()=0;
	virtual operator FILETIME()=0;
	virtual operator DB_DATE()=0;
	virtual operator DB_TIME()=0;
	virtual operator DB_TIMESTAMP()=0;
	virtual operator DB_GUID()=0;	
	virtual operator const void *()=0;	

};



