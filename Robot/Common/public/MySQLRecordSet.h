#pragma once
#include "dbinterfaces.h"



class CMySQLConnection;
typedef MYSQL_RES * MYSQL_RES_HANDLE;

class CMySQLRecordSet :
	public IDBRecordSet
{
protected:
	//CMySQLConnection *				m_pDBConnection;
	MYSQL_RES_HANDLE				m_hResults;
	std::vector<DB_COLUMN_INFO>		m_pColumnInfos;
	std::vector<CMySQLValue>		m_RowBuffer;
	int								m_CurRow;
	CMySQLValue						m_EmptyValue;

	DECLARE_CLASS_INFO(CMySQLRecordSet)
public:
	CMySQLRecordSet(void);
	virtual ~CMySQLRecordSet(void);

	int Init(MYSQL_RES_HANDLE hResults);
	virtual void Destory();

	virtual int GetRecordCount();
	virtual int GetColumnCount();
	virtual LPCTSTR GetColumnName(int Index);
	virtual int GetIndexByColumnName(LPCTSTR Name);
	virtual DB_COLUMN_INFO * GetColumnInfo(int Index);

	virtual int GetParamCount();
	virtual IDBValue& GetParam(int Index);

	virtual IDBValue& GetField(int Index);
	virtual IDBValue& GetField(LPCTSTR Name);
	virtual int MoveFirst();
	virtual int MoveLast();
	virtual int MoveNext();
	virtual int MovePrevious();
	virtual int MoveTo(int Index);	
	virtual bool IsEOF();
	virtual bool IsBOF();
protected:
	int FetchRow();
};

