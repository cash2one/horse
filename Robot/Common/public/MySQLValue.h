#pragma once
#include "dbinterfaces.h"




class CMySQLValue :
	public IDBValue
{
protected:
	DB_COLUMN_INFO	m_ValueInfo;
	char *			m_pData;	
	int				m_DataSize;

	DECLARE_CLASS_INFO(CMySQLValue)
public:
	CMySQLValue(void);
	CMySQLValue(const CMySQLValue& Value);
	virtual ~CMySQLValue(void);

	void SetValue(DB_COLUMN_INFO& ValueInfo,LPVOID pData,int DataSize);

	virtual DB_COLUMN_INFO * GetTypeInfo();	
	virtual int	GetLength();
	virtual bool IsNull();

	virtual operator bool();	
	virtual operator char();
	virtual operator unsigned char();
	virtual operator short();
	virtual operator unsigned short();
	virtual operator int();
	virtual operator unsigned int();
	virtual operator long();
	virtual operator unsigned long();
	virtual operator __int64();
	virtual operator unsigned __int64();
	virtual operator float();
	virtual operator double();
	virtual operator const char *();	
	virtual operator const WCHAR *();	
	virtual operator SYSTEMTIME();
	virtual operator FILETIME();
	virtual operator DB_DATE();
	virtual operator DB_TIME();
	virtual operator DB_TIMESTAMP();
	virtual operator DB_GUID();	
	virtual operator const void *();

protected:
	int MySQLTypeToDBType(int MySQLType);

	
};

