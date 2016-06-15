#include "StdAfx.h"
#include ".\mysqlconnection.h"


IMPLEMENT_CLASS_INFO(CMySQLConnection,IDBConnection);

CMySQLConnection::CMySQLConnection(void)
{
	m_MySQLHandle=NULL;
	//m_pDatabase=NULL;
}

CMySQLConnection::~CMySQLConnection(void)
{
	Destory();
}

int CMySQLConnection::Init()
{
	//m_pDatabase=pDatabase;
	return DBERR_SUCCEED;
}

void CMySQLConnection::Destory()
{
	Disconnect();
	//m_pDatabase=NULL;
}

int CMySQLConnection::Connect(LPCTSTR ConnectStr)
{
	Disconnect();
	m_MySQLHandle=mysql_init(&m_Mysql);
	if(m_MySQLHandle==NULL)
		return DBERR_MYSQL_MYSQLINITFAIL;
	//mysql_options(m_MySQLHandle,MYSQL_SET_CHARSET_NAME,"gb2312");
	CSettingFile StrAnalyzer;
	StrAnalyzer.Load(ConnectStr,';',0);
	const char * Host=StrAnalyzer.GetString(NULL,"Server","");
	const char * User=StrAnalyzer.GetString(NULL,"UID","");
	const char * Password=StrAnalyzer.GetString(NULL,"PWD","");
	const char * FlagsStr=StrAnalyzer.GetString(NULL,"Flag","");
	const char * CharSetStr=StrAnalyzer.GetString(NULL,"CharSet","");
	if(Password[0]==0)
		Password=NULL;
	const char * DB=StrAnalyzer.GetString(NULL,"DB","");
	if(DB[0]==0)
		DB=NULL;
	const char * UnixSocket=StrAnalyzer.GetString(NULL,"UnixSocket","");
	if(UnixSocket[0]==0)
		UnixSocket=NULL;
	int Port=StrAnalyzer.GetInteger(NULL,"Port",0);
	DWORD Flags=FetchConnectFlags(FlagsStr);
	if(mysql_real_connect(m_MySQLHandle,Host,User,Password,DB,Port,UnixSocket,
		Flags)==NULL)
	{
		ProcessErrorMsg("连接失败\r\n");
		return DBERR_MYSQL_CONNECTFAIL;
	}
	if(CharSetStr[0])
	{
		if(mysql_set_character_set(m_MySQLHandle,CharSetStr) )
		{
			ProcessErrorMsg("设置字符集失败\r\n");
			return DBERR_CHARSET;
		}
	}

	return DBERR_SUCCEED;
}

int CMySQLConnection::Disconnect()
{
	if(m_MySQLHandle)
	{	
		mysql_close(m_MySQLHandle);
		m_MySQLHandle=NULL;
	}
	return DBERR_SUCCEED;
}
BOOL CMySQLConnection::IsConnected()
{
	if(m_MySQLHandle==NULL)
		return FALSE;
	return mysql_ping(m_MySQLHandle)==0;	
}

int CMySQLConnection::ExecuteSQL(LPCSTR SQLStr,int StrLen)
{
	if(m_MySQLHandle==NULL)
		return DBERR_MYSQL_WANTCONNECT;
	if(!SQLStr)
	{
		return DBERR_INVALID_PARAM;
	}
	
	if(StrLen==0)
		StrLen=(int)strlen(SQLStr);
	//if(mysql_field_count(m_MySQLHandle))
	//{
	//	mysql_next_result(m_MySQLHandle);
	//	MYSQL_RES* result = mysql_store_result(m_MySQLHandle);
	//	if(result)
	//	{
	//		mysql_free_result(result);
	//	}
	//}
	if(mysql_real_query(m_MySQLHandle,SQLStr,StrLen))
	{
		ProcessErrorMsg("Execute SQL failed\r\n");
		return DBERR_EXE_SQL_FAIL;
	}

	return DBERR_SUCCEED;
}
//
int CMySQLConnection::GetMySqlInsertID(unsigned long long& lID)
{
	//
	lID = 0;
	if(m_MySQLHandle==NULL)
		return DBERR_MYSQL_WANTCONNECT;
	//
	lID = mysql_insert_id(m_MySQLHandle);
	return DBERR_SUCCEED;
}

int CMySQLConnection::GetResults(IDBRecordSet * pDBRecordset)
{
	if(!pDBRecordset)
	{
		return DBERR_INVALID_PARAM;
	}
	//if(pDBRecordset->IsKindOf(GET_CLASS_INFO(CMySQLStaticRecordSet)))
	//{
	//	return FetchStaticResult((CMySQLStaticRecordSet *)pDBRecordset);		
	//}
	pDBRecordset->Destory();
	if(pDBRecordset->IsKindOf(GET_CLASS_INFO(CMySQLRecordSet)))
	{
		return FetchResult((CMySQLRecordSet *)pDBRecordset);		
	}
	return DBERR_INVALID_PARAM;
	
}

BOOL CMySQLConnection::NextResults()
{
	if(!mysql_more_results(m_MySQLHandle))
	{
		return DBERR_NO_MORE_RESULTS;
	}
	int RetCode=mysql_next_result(m_MySQLHandle);
	if(RetCode>0)
	{
		ProcessErrorMsg("Execute SQL failed\r\n");
		return DBERR_EXE_SQL_FAIL;
	}
	return DBERR_SUCCEED;
}



int CMySQLConnection::EnableTransaction(BOOL IsEnable)
{
	if(m_MySQLHandle==NULL)
		return DBERR_MYSQL_WANTCONNECT;
	if(mysql_autocommit(m_MySQLHandle,!IsEnable))
	{
		ProcessErrorMsg("设置事务失败\r\n");
		return DBERR_MYSQL_ENABLETRANSACTIONFAIL;
	}
	return DBERR_SUCCEED;
}
int CMySQLConnection::Commit()
{
	if(m_MySQLHandle==NULL)
		return DBERR_MYSQL_WANTCONNECT;
	if(mysql_commit(m_MySQLHandle))
	{
		ProcessErrorMsg("提交事务失败\r\n");
		return DBERR_MYSQL_COMMITFAIL;
	}
	return DBERR_SUCCEED;
}
int CMySQLConnection::RollBack()
{
	if(m_MySQLHandle==NULL)
		return DBERR_MYSQL_WANTCONNECT;
	if(mysql_rollback(m_MySQLHandle))
	{
		ProcessErrorMsg("回滚事务失败\r\n");
		return DBERR_MYSQL_ROLLBACKFAIL;
	}
	return DBERR_SUCCEED;
}

UINT CMySQLConnection::GetLastDatabaseErrorCode()
{
	return mysql_errno(m_MySQLHandle);
}

LPCTSTR CMySQLConnection::GetLastDatabaseErrorString()
{
	return mysql_error(m_MySQLHandle);
}

int CMySQLConnection::TranslateString(LPCTSTR szSource,int SrcLen,LPTSTR szTarget,int MaxLen,bool isBlob)
{
	if( SrcLen * 2 +1 > MaxLen )
	{
		ASSERT(FALSE);
		return -1;
	}
#if (VER_LANGUAGE == 4)  //俄罗斯版 
	if(!isBlob)
	{
		int nLen = MultiByteToWideChar(1251,0,szSource, -1, NULL,0);
		wchar_t* wSource = new wchar_t[nLen+1];
		nLen = MultiByteToWideChar(1251, 0, szSource,-1, wSource, nLen);
		nLen = WideCharToMultiByte(936, 0, wSource,-1, NULL, 0,0,0);
		char* gbSource = new char[nLen+1];
		nLen = WideCharToMultiByte(936, 0, wSource,-1, gbSource, nLen,0,0);
		char* szTempTarget = new char[2*nLen+1];
		nLen = mysql_real_escape_string(m_MySQLHandle,szTempTarget,gbSource,nLen);
		if(szTempTarget[nLen-2] == '\\' && szTempTarget[nLen-1] == '0')
		{
			nLen -=2;
			szTempTarget[nLen]  = 0;
		}
		nLen = min((SrcLen * 2 +1), nLen);
		_tcsncpy(szTarget,szTempTarget, nLen);
		szTarget[nLen] = 0;
		delete []wSource;
		delete []gbSource;
		delete []szTempTarget;
		return nLen;
	}
#endif

	return (int)mysql_real_escape_string(m_MySQLHandle,szTarget,szSource,SrcLen);
}

void CMySQLConnection::ProcessErrorMsg(LPCTSTR Msg)
{
	PrintDBError(0xff,"%s errno=%d %s\r\n",Msg,mysql_errno(m_MySQLHandle),mysql_error(m_MySQLHandle));
}

//int CMySQLConnection::FetchStaticResult(CMySQLStaticRecordSet * pDBRecordset)
//{
//	CEasyBuffer ResultBuff;
//	
//
//	if(m_MySQLHandle==NULL)
//		return DBERR_MYSQL_WANTCONNECT;
//	MYSQL_RES_HANDLE hResults=mysql_store_result(m_MySQLHandle);
//	if(hResults==NULL)
//	{
//		if(mysql_errno(m_MySQLHandle)==0)
//			return DBERR_NO_RECORDS;
//		else
//		{
//			ProcessErrorMsg("提取结果集失败\r\n");
//			return DBERR_MYSQL_FETCHRESULTFAIL;
//		}
//	}	
//	
//	//计算数据集大小
//	int ColNum=mysql_num_fields(hResults);
//
//	if(ColNum<=0)
//		return DBERR_NO_RECORDS;
//
//	int RowNum=(int)mysql_num_rows(hResults);
//	if(RowNum<=0)
//		return DBERR_NO_RECORDS;
//
//	int DataSize=sizeof(int)*2+sizeof(DB_COLUMN_INFO)*ColNum;
//
//	
//	for(int i=0;i<RowNum;i++)
//	{	
//		mysql_data_seek(hResults,i);
//		MYSQL_ROW RowData=mysql_fetch_row(hResults);
//		ULONG * ValueLen=mysql_fetch_lengths(hResults);		
//		for(int j=0;j<ColNum;j++)
//		{
//			DataSize+=sizeof(int)+ValueLen[j]+1;
//		}
//	}
//
//	DataSize+=1024;
//	ResultBuff.Create(DataSize);
//
//	//获取结果集列信息
//	ResultBuff.PushBack(&ColNum,sizeof(int));
//	int RecordLineLen=0;	
//	if(ResultBuff.GetFreeSize()<(int)sizeof(DB_COLUMN_INFO)*ColNum)
//		return DBERR_BUFFER_OVERFLOW;	
//
//	DB_COLUMN_INFO * pColInfos=(DB_COLUMN_INFO *)ResultBuff.GetFreeBuffer();
//	ZeroMemory(pColInfos,sizeof(DB_COLUMN_INFO)*ColNum);
//	ResultBuff.SetUsedSize(ResultBuff.GetUsedSize()+sizeof(DB_COLUMN_INFO)*ColNum);
//	MYSQL_FIELD * pFields=mysql_fetch_fields(hResults);
//	for(int i=0;i<ColNum;i++)
//	{		
//		strncpy(pColInfos[i].Name,pFields[i].name,MAX_COLUMN_NAME);
//		pColInfos[i].Name[MAX_COLUMN_NAME-1]=0;
//		pColInfos[i].Type=pFields[i].type;
//		pColInfos[i].Size=pFields[i].length;
//		pColInfos[i].DigitSize=pFields[i].decimals;			
//		
//	}
//	
//
//	ResultBuff.PushBack(&RowNum,sizeof(int));
//	for(int i=0;i<RowNum;i++)
//	{	
//		mysql_data_seek(hResults,i);
//		MYSQL_ROW RowData=mysql_fetch_row(hResults);
//		ULONG * ValueLen=mysql_fetch_lengths(hResults);
//		for(int j=0;j<ColNum;j++)
//		{
//			if(RowData[j])
//			{			
//				ResultBuff.PushConstBack(UINT(ValueLen[j]),sizeof(ULONG));
//				ResultBuff.PushBack(RowData[j],ValueLen[j]);
//			}
//			else
//			{
//				ResultBuff.PushBack(UINT(0),sizeof(ULONG));				
//			}
//		}
//	}
//	mysql_free_result(hResults);
//	return pDBRecordset->SetRecordSetData(ResultBuff.GetBuffer(),ResultBuff.GetUsedSize());
//	
//}

int CMySQLConnection::FetchResult(CMySQLRecordSet * pDBRecordset)
{
	if(m_MySQLHandle==NULL)
		return DBERR_MYSQL_WANTCONNECT;
	MYSQL_RES_HANDLE hResults=mysql_store_result(m_MySQLHandle);
	if(hResults==NULL)
	{
		if(mysql_errno(m_MySQLHandle)==0)
			return DBERR_NO_RECORDS;
		else
		{
			//ProcessErrorMsg("提取结果集失败\r\n");
			return DBERR_FETCH_RESULT_FAIL;
		}
	}	
	my_ulonglong num = mysql_affected_rows(m_MySQLHandle);
	
	if( num == 0)
	{
		mysql_free_result(hResults);
		return DBERR_SUCCEED;
	}
	else if( (my_ulonglong)-1 == num)
	{
		mysql_free_result(hResults);
		hResults = NULL;
		return DBERR_FETCH_ROWS_NUM;
	}
	else if( num < 99999)
	{
		return pDBRecordset->Init(hResults);
	}
	else
	{
		mysql_free_result(hResults);
		ASSERT( false );
		return DBERR_FETCH_ROWS_NUM;
	}
}

DWORD CMySQLConnection::FetchConnectFlags(LPCTSTR FlagsStr)
{
	DWORD Flags=0;
	CStringSplitter Splitter(FlagsStr,'|');
	for(int i=0;i<(int)Splitter.GetCount();i++)
	{
		CEasyString Flag=Splitter[i];
		Flag.Trim(' ');
		if(Flag.CompareNoCase("CLIENT_COMPRESS")==0)
		{
			Flags|=CLIENT_COMPRESS;
		}
		else if(Flag.CompareNoCase("CLIENT_FOUND_ROWS")==0)
		{
			Flags|=CLIENT_FOUND_ROWS;
		}
		else if(Flag.CompareNoCase("CLIENT_IGNORE_SPACE")==0)
		{
			Flags|=CLIENT_IGNORE_SPACE;
		}
		else if(Flag.CompareNoCase("CLIENT_INTERACTIVE")==0)
		{
			Flags|=CLIENT_INTERACTIVE;
		}
		else if(Flag.CompareNoCase("CLIENT_LOCAL_FILES")==0)
		{
			Flags|=CLIENT_LOCAL_FILES;
		}
		else if(Flag.CompareNoCase("CLIENT_MULTI_STATEMENTS")==0)
		{
			Flags|=CLIENT_MULTI_STATEMENTS;
		}
		else if(Flag.CompareNoCase("CLIENT_MULTI_RESULTS")==0)
		{
			Flags|=CLIENT_MULTI_RESULTS;
		}
		else if(Flag.CompareNoCase("CLIENT_NO_SCHEMA")==0)
		{
			Flags|=CLIENT_NO_SCHEMA;
		}
		else if(Flag.CompareNoCase("CLIENT_ODBC")==0)
		{
			Flags|=CLIENT_ODBC;
		}
		else if(Flag.CompareNoCase("CLIENT_SSL")==0)
		{
			Flags|=CLIENT_SSL;
		}

	}
	return Flags;
}

