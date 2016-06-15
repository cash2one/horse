#include "StdAfx.h"
#include ".\mysqldatabase.h"



IMPLEMENT_CLASS_INFO(CMySQLDatabase,IDatabase);

CMySQLDatabase::CMySQLDatabase(void):IDatabase()
{
	mysql_library_init( -1 ,NULL,NULL);
}

CMySQLDatabase::~CMySQLDatabase(void)
{
	mysql_library_end();
}

bool CMySQLDatabase::IsThreadSafe()
{
	return mysql_thread_safe();
}

IDBConnection * CMySQLDatabase::CreateConnection()
{
	CMySQLConnection * pConnection=new CMySQLConnection();	
	if(pConnection->Init()==DBERR_SUCCEED)
		return pConnection;
	pConnection->Release();	
	return NULL;
}

int CMySQLDatabase::DeleteConnection(IDBConnection * pDBConnection)
{
	if(pDBConnection)
	{
		pDBConnection->Release();
		return DBERR_SUCCEED;
	}
	return DBERR_INVALID_CONNECTION;
}
