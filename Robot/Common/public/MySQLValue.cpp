#include "StdAfx.h"
#include ".\mysqlvalue.h"



IMPLEMENT_CLASS_INFO(CMySQLValue,IDBValue);

CMySQLValue::CMySQLValue(void):IDBValue()
{
	ZeroMemory(&m_ValueInfo,sizeof(m_ValueInfo));	
	m_ValueInfo.Type=-1;
	m_pData=NULL;
	m_DataSize=0;
}

CMySQLValue::CMySQLValue(const CMySQLValue& Value)
{
	ZeroMemory(&m_ValueInfo,sizeof(m_ValueInfo));	
	m_ValueInfo.Type=Value.m_ValueInfo.Type; //claus: -1 will have bug ;
	m_pData=NULL;
	m_DataSize=0;
	SetValue(m_ValueInfo,Value.m_pData,Value.m_DataSize);
}

CMySQLValue::~CMySQLValue(void)
{
	SAFE_DELETE_ARRAY(m_pData);
	m_DataSize=0;
	ZeroMemory(&m_ValueInfo,sizeof(m_ValueInfo));
}

void CMySQLValue::SetValue(DB_COLUMN_INFO& ValueInfo,LPVOID pData,int DataSize)
{
	if(pData==NULL)
		return;
	SAFE_DELETE_ARRAY(m_pData);
	m_ValueInfo=ValueInfo;	
	if(pData)
	{
		m_DataSize=DataSize;
		m_pData=new char[DataSize+2];
		if( m_pData )
		{
			memcpy(m_pData,pData,DataSize);	
			m_pData[DataSize]=0;
			m_pData[DataSize+1]=0;
		}
	}
}

DB_COLUMN_INFO * CMySQLValue::GetTypeInfo()
{
	return &m_ValueInfo;
}

int	CMySQLValue::GetLength()
{
	return m_DataSize;
}

bool CMySQLValue::IsNull()
{
	return m_pData==NULL;
}


CMySQLValue::operator bool()
{
	bool Default=false;
	switch(m_ValueInfo.Type)
	{	
	case MYSQL_TYPE_TINY:		
	case MYSQL_TYPE_SHORT:		
	case MYSQL_TYPE_LONG:
	case MYSQL_TYPE_INT24:		
	case MYSQL_TYPE_LONGLONG:	
		Default=atoi(m_pData)!=0;
		break;
	case MYSQL_TYPE_DECIMAL:	
	case MYSQL_TYPE_FLOAT:		
	case MYSQL_TYPE_DOUBLE:		
	case MYSQL_TYPE_NEWDECIMAL:	
		Default=atof(m_pData)!=0;
		break;	
	case MYSQL_TYPE_BIT:
		Default=BitToUINT(m_pData,m_DataSize-1)!=0;
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_YEAR:
	case MYSQL_TYPE_SET:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_GEOMETRY:
	case MYSQL_TYPE_BLOB:
		break;	
	}	
	return Default;
}
CMySQLValue::operator char()
{
	char Default=0;
	switch(m_ValueInfo.Type)
	{	
	case MYSQL_TYPE_TINY:		
	case MYSQL_TYPE_SHORT:		
	case MYSQL_TYPE_LONG:
	case MYSQL_TYPE_INT24:		
	case MYSQL_TYPE_LONGLONG:	
		Default=(char)atoi(m_pData);
		break;
	case MYSQL_TYPE_DECIMAL:	
	case MYSQL_TYPE_FLOAT:		
	case MYSQL_TYPE_DOUBLE:				
	case MYSQL_TYPE_NEWDECIMAL:	
		Default=(char)atof(m_pData);
		break;
	case MYSQL_TYPE_BIT:
		Default=(char)BitToUINT(m_pData,m_DataSize-1);
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_YEAR:
	case MYSQL_TYPE_SET:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_GEOMETRY:
	case MYSQL_TYPE_BLOB:
		break;	
	}	
	return Default;
}
CMySQLValue::operator unsigned char()
{
	unsigned char Default=0;
	switch(m_ValueInfo.Type)
	{	
	case MYSQL_TYPE_TINY:		
	case MYSQL_TYPE_SHORT:		
	case MYSQL_TYPE_LONG:
	case MYSQL_TYPE_INT24:		
	case MYSQL_TYPE_LONGLONG:	
		Default=(unsigned char)atoi(m_pData);
		break;
	case MYSQL_TYPE_DECIMAL:	
	case MYSQL_TYPE_FLOAT:		
	case MYSQL_TYPE_DOUBLE:				
	case MYSQL_TYPE_NEWDECIMAL:	
		Default=(unsigned char)atof(m_pData);
		break;
	case MYSQL_TYPE_BIT:
		Default=(unsigned char)BitToUINT(m_pData,m_DataSize-1);
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_YEAR:
	case MYSQL_TYPE_SET:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_GEOMETRY:
	case MYSQL_TYPE_BLOB:
		break;	
	}	
	return Default;
}
CMySQLValue::operator short()
{
	short Default=0;
	switch(m_ValueInfo.Type)
	{	
	case MYSQL_TYPE_TINY:		
	case MYSQL_TYPE_SHORT:		
	case MYSQL_TYPE_LONG:
	case MYSQL_TYPE_INT24:		
	case MYSQL_TYPE_LONGLONG:	
		Default=(short)atoi(m_pData);
		break;
	case MYSQL_TYPE_DECIMAL:	
	case MYSQL_TYPE_FLOAT:		
	case MYSQL_TYPE_DOUBLE:				
	case MYSQL_TYPE_NEWDECIMAL:	
		Default=(short)atof(m_pData);
		break;
	case MYSQL_TYPE_BIT:
		Default=(short)BitToUINT(m_pData,m_DataSize-1);
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_YEAR:
	case MYSQL_TYPE_SET:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_GEOMETRY:
	case MYSQL_TYPE_BLOB:
		break;	
	}	
	return Default;
}
CMySQLValue::operator unsigned short()
{
	unsigned short Default=0;
	switch(m_ValueInfo.Type)
	{	
	case MYSQL_TYPE_TINY:		
	case MYSQL_TYPE_SHORT:		
	case MYSQL_TYPE_LONG:
	case MYSQL_TYPE_INT24:		
	case MYSQL_TYPE_LONGLONG:	
		Default=(unsigned short)atoi(m_pData);
		break;
	case MYSQL_TYPE_DECIMAL:	
	case MYSQL_TYPE_FLOAT:		
	case MYSQL_TYPE_DOUBLE:				
	case MYSQL_TYPE_NEWDECIMAL:	
		Default=(unsigned short)atof(m_pData);
		break;
	case MYSQL_TYPE_BIT:
		Default=(unsigned short)BitToUINT(m_pData,m_DataSize-1);
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_YEAR:
	case MYSQL_TYPE_SET:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_GEOMETRY:
	case MYSQL_TYPE_BLOB:
		break;	
	}	
	return Default;
}
CMySQLValue::operator int()
{
	int Default=0;
	switch(m_ValueInfo.Type)
	{	
	case MYSQL_TYPE_TINY:		
	case MYSQL_TYPE_SHORT:		
	case MYSQL_TYPE_LONG:
	case MYSQL_TYPE_INT24:		
	case MYSQL_TYPE_LONGLONG:	
		Default=(int)atoi(m_pData);
		break;
	case MYSQL_TYPE_DECIMAL:	
	case MYSQL_TYPE_FLOAT:		
	case MYSQL_TYPE_DOUBLE:				
	case MYSQL_TYPE_NEWDECIMAL:	
		Default=(int)atof(m_pData);
		break;
	case MYSQL_TYPE_BIT:
		Default=(int)BitToUINT(m_pData,m_DataSize-1);
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_YEAR:
	case MYSQL_TYPE_SET:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_GEOMETRY:
	case MYSQL_TYPE_BLOB:
		break;	
	}	
	return Default;
}
CMySQLValue::operator unsigned int()
{
	unsigned int Default=0;
	switch(m_ValueInfo.Type)
	{	
	case MYSQL_TYPE_TINY:		
	case MYSQL_TYPE_SHORT:		
	case MYSQL_TYPE_LONG:
	case MYSQL_TYPE_INT24:		
	case MYSQL_TYPE_LONGLONG:	
		Default=(unsigned int)atoi(m_pData);
		break;
	case MYSQL_TYPE_DECIMAL:	
	case MYSQL_TYPE_FLOAT:		
	case MYSQL_TYPE_DOUBLE:				
	case MYSQL_TYPE_NEWDECIMAL:	
		Default=(unsigned int)atof(m_pData);
		break;
	case MYSQL_TYPE_BIT:
		Default=(unsigned int)BitToUINT(m_pData,m_DataSize-1);
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_YEAR:
	case MYSQL_TYPE_SET:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_GEOMETRY:
	case MYSQL_TYPE_BLOB:
		break;	
	}	
	return Default;
}
CMySQLValue::operator long()
{
	long Default=0;
	switch(m_ValueInfo.Type)
	{	
	case MYSQL_TYPE_TINY:		
	case MYSQL_TYPE_SHORT:		
	case MYSQL_TYPE_LONG:
	case MYSQL_TYPE_INT24:		
	case MYSQL_TYPE_LONGLONG:	
		Default=(long)atoi(m_pData);
		break;
	case MYSQL_TYPE_DECIMAL:	
	case MYSQL_TYPE_FLOAT:		
	case MYSQL_TYPE_DOUBLE:				
	case MYSQL_TYPE_NEWDECIMAL:	
		Default=(long)atof(m_pData);
		break;
	case MYSQL_TYPE_BIT:
		Default=(long)BitToUINT(m_pData,m_DataSize-1);
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_YEAR:
	case MYSQL_TYPE_SET:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_GEOMETRY:
	case MYSQL_TYPE_BLOB:
		break;	
	}	
	return Default;
}
CMySQLValue::operator unsigned long()
{
	unsigned long Default=0;
	switch(m_ValueInfo.Type)
	{	
	case MYSQL_TYPE_TINY:		
	case MYSQL_TYPE_SHORT:		
	case MYSQL_TYPE_LONG:
	case MYSQL_TYPE_INT24:		
	case MYSQL_TYPE_LONGLONG:	
		Default=(unsigned long)atoi(m_pData);
		break;
	case MYSQL_TYPE_DECIMAL:	
	case MYSQL_TYPE_FLOAT:		
	case MYSQL_TYPE_DOUBLE:				
	case MYSQL_TYPE_NEWDECIMAL:	
		Default=(unsigned long)atof(m_pData);
		break;
	case MYSQL_TYPE_BIT:
		Default=(unsigned long)BitToUINT(m_pData,m_DataSize-1);
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_YEAR:
	case MYSQL_TYPE_SET:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_GEOMETRY:
	case MYSQL_TYPE_BLOB:
		break;	
	}	
	return Default;
}
CMySQLValue::operator __int64()
{
	__int64 Default=0;
	switch(m_ValueInfo.Type)
	{	
	case MYSQL_TYPE_TINY:		
	case MYSQL_TYPE_SHORT:		
	case MYSQL_TYPE_LONG:
	case MYSQL_TYPE_INT24:		
	case MYSQL_TYPE_LONGLONG:	
		Default=(__int64)_atoi64(m_pData);
		break;
	case MYSQL_TYPE_DECIMAL:	
	case MYSQL_TYPE_FLOAT:		
	case MYSQL_TYPE_DOUBLE:				
	case MYSQL_TYPE_NEWDECIMAL:	
		Default=(__int64)atof(m_pData);
		break;
	case MYSQL_TYPE_BIT:
		Default=(__int64)BitToUINT(m_pData,m_DataSize-1);
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_YEAR:
	case MYSQL_TYPE_SET:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_GEOMETRY:
	case MYSQL_TYPE_BLOB:
		break;	
	}	
	return Default;
}
CMySQLValue::operator unsigned __int64()
{
	unsigned __int64 Default=0;
	switch(m_ValueInfo.Type)
	{	
	case MYSQL_TYPE_TINY:		
	case MYSQL_TYPE_SHORT:		
	case MYSQL_TYPE_LONG:
	case MYSQL_TYPE_INT24:		
	case MYSQL_TYPE_LONGLONG:	
		Default=(unsigned __int64)_atoi64(m_pData);
		break;
	case MYSQL_TYPE_DECIMAL:	
	case MYSQL_TYPE_FLOAT:		
	case MYSQL_TYPE_DOUBLE:				
	case MYSQL_TYPE_NEWDECIMAL:	
		Default=(unsigned __int64)atof(m_pData);
		break;
	case MYSQL_TYPE_BIT:
		Default=(unsigned __int64)BitToUINT(m_pData,m_DataSize-1);
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_YEAR:
	case MYSQL_TYPE_SET:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_GEOMETRY:
	case MYSQL_TYPE_BLOB:
		break;	
	}	
	return Default;
}
CMySQLValue::operator float()
{
	float Default=0.0f;
	switch(m_ValueInfo.Type)
	{	
	case MYSQL_TYPE_TINY:		
	case MYSQL_TYPE_SHORT:		
	case MYSQL_TYPE_LONG:
	case MYSQL_TYPE_INT24:
		Default=(float)atoi(m_pData);
		break;
	case MYSQL_TYPE_LONGLONG:	
		Default=(float)_atoi64(m_pData);
		break;
	case MYSQL_TYPE_DECIMAL:	
	case MYSQL_TYPE_FLOAT:		
	case MYSQL_TYPE_DOUBLE:				
	case MYSQL_TYPE_NEWDECIMAL:	
		Default=(float)atof(m_pData);
		break;
	case MYSQL_TYPE_BIT:
		Default=(float)BitToUINT(m_pData,m_DataSize-1);
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_YEAR:
	case MYSQL_TYPE_SET:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_GEOMETRY:
	case MYSQL_TYPE_BLOB:
		break;	
	}	
	return Default;
}
CMySQLValue::operator double()
{
	double Default=0.0f;
	switch(m_ValueInfo.Type)
	{	
	case MYSQL_TYPE_TINY:		
	case MYSQL_TYPE_SHORT:		
	case MYSQL_TYPE_LONG:
	case MYSQL_TYPE_INT24:
		Default=(double)atoi(m_pData);
		break;
	case MYSQL_TYPE_LONGLONG:	
		Default=(double)_atoi64(m_pData);
		break;
	case MYSQL_TYPE_DECIMAL:	
	case MYSQL_TYPE_FLOAT:		
	case MYSQL_TYPE_DOUBLE:				
	case MYSQL_TYPE_NEWDECIMAL:	
		Default=(double)atof(m_pData);
		break;
	case MYSQL_TYPE_BIT:
		Default=(double)BitToUINT(m_pData,m_DataSize-1);
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_YEAR:
	case MYSQL_TYPE_SET:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_GEOMETRY:
	case MYSQL_TYPE_BLOB:
		break;	
	}	
	return Default;
}
CMySQLValue::operator const char *()
{
	const char * Default="NULL";
	if(m_pData)
	{
#if (VER_LANGUAGE == 4)  //¶íÂÞË¹°æ 
		int nLen = MultiByteToWideChar(936,0,m_pData, -1, NULL,0);
		wchar_t* wString = new wchar_t[nLen+1];
		nLen = MultiByteToWideChar(936, 0, m_pData,-1, wString, nLen);
		nLen = WideCharToMultiByte(1251, 0, wString,-1, NULL, 0,0,0);
		nLen = WideCharToMultiByte(1251, 0, wString,-1, m_pData, nLen,0,0);
		*(m_pData+nLen) = '\0';
		m_DataSize = nLen;
		delete []wString;
#endif
		Default=(const char *)m_pData;	
	}
	return Default;
}
CMySQLValue::operator const WCHAR *()
{
	WCHAR Null[5]={'N','U','L','L',0};
	const WCHAR * Default=Null;
	if(m_pData)
		Default=(const WCHAR *)m_pData;
	return Default;
}

CMySQLValue::operator SYSTEMTIME()
{
	SYSTEMTIME_EX Default;	
	switch(m_ValueInfo.Type)
	{		
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATETIME:
		sscanf(m_pData,"%hd-%hd-%hd %hd:%hd:%hd",
			&Default.wYear,&Default.wMonth,&Default.wDay,
			&Default.wHour,&Default.wMinute,&Default.wSecond);
		break;
	case MYSQL_TYPE_DATE:
		sscanf(m_pData,"%hd-%hd-%hd",
			&Default.wYear,&Default.wMonth,&Default.wDay);
		break;
	case MYSQL_TYPE_TIME:
		sscanf(m_pData,"%hd:%hd:%hd",
			&Default.wHour,&Default.wMinute,&Default.wSecond);
		break;
	case MYSQL_TYPE_YEAR:
		Default.wYear=atoi(m_pData);
		break;	
	}	
	return Default;
}
CMySQLValue::operator FILETIME()
{
	FILETIME_EX Default;
	switch(m_ValueInfo.Type)
	{		
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATETIME:
		{
			SYSTEMTIME_EX SystemTime;
			sscanf(m_pData,"%hd-%hd-%hd %hd:%hd:%hd",
				&SystemTime.wYear,&SystemTime.wMonth,&SystemTime.wDay,
				&SystemTime.wHour,&SystemTime.wMinute,&SystemTime.wSecond);
			Default=SystemTime;
		}
		break;
	case MYSQL_TYPE_DATE:
		{
			SYSTEMTIME_EX SystemTime;
			sscanf(m_pData,"%hd-%hd-%hd",
				&SystemTime.wYear,&SystemTime.wMonth,&SystemTime.wDay);
			Default=SystemTime;
		}
		break;
	case MYSQL_TYPE_TIME:
		{
			SYSTEMTIME_EX SystemTime;
			sscanf(m_pData,"%hd:%hd:%hd",
				&SystemTime.wHour,&SystemTime.wMinute,&SystemTime.wSecond);
			Default=SystemTime;
		}
		break;
	case MYSQL_TYPE_YEAR:
		{
			SYSTEMTIME_EX SystemTime;
			SystemTime.wYear=atoi(m_pData);
			Default=SystemTime;
		}
		break;	
	}	
	return Default;
}
CMySQLValue::operator DB_DATE()
{
	DB_DATE Default;	
	WORD temp;
	switch(m_ValueInfo.Type)
	{		
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATETIME:
		sscanf(m_pData,"%hd-%hd-%hd %hd:%hd:%hd",
			&Default.year,&Default.month,&Default.day,
			&temp,&temp,&temp);
		break;
	case MYSQL_TYPE_DATE:
		sscanf(m_pData,"%hd-%hd-%hd",
			&Default.year,&Default.month,&Default.day);
		break;
	case MYSQL_TYPE_TIME:		
		break;
	case MYSQL_TYPE_YEAR:
		Default.year=atoi(m_pData);
		break;	
	}	
	return Default;
}
CMySQLValue::operator DB_TIME()
{
	DB_TIME Default;
	WORD temp;
	switch(m_ValueInfo.Type)
	{		
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATETIME:
		sscanf(m_pData,"%hd-%hd-%hd %hd:%hd:%hd",
			&temp,&temp,&temp,
			&Default.hour,&Default.minute,&Default.second);
		break;
	case MYSQL_TYPE_DATE:		
		break;
	case MYSQL_TYPE_TIME:
		sscanf(m_pData,"%hd:%hd:%hd",
			&Default.hour,&Default.minute,&Default.second);
		break;
	case MYSQL_TYPE_YEAR:		
		break;	
	}	
	return Default;
}
CMySQLValue::operator DB_TIMESTAMP()
{
	DB_TIMESTAMP Default;
	switch(m_ValueInfo.Type)
	{		
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATETIME:
		sscanf(m_pData,"%hd-%hd-%hd %hd:%hd:%hd.&d",
			&Default.year,&Default.month,&Default.day,
			&Default.hour,&Default.minute,&Default.second,
			&Default.fraction);
		break;
	case MYSQL_TYPE_DATE:
		sscanf(m_pData,"%hd-%hd-%hd",
			&Default.year,&Default.month,&Default.day);
		break;
	case MYSQL_TYPE_TIME:
		sscanf(m_pData,"%hd:%hd:%hd",
			&Default.hour,&Default.minute,&Default.second);
		break;
	case MYSQL_TYPE_YEAR:
		Default.year=atoi(m_pData);
		break;	
	}	
	return Default;
}
CMySQLValue::operator DB_GUID()
{
	DB_GUID Default;	
	return Default;
}
CMySQLValue::operator const void *()
{
	const void * Default=m_pData;	
	return Default;
}


int CMySQLValue::MySQLTypeToDBType(int MySQLType)
{
	switch(MySQLType)
	{	
	case MYSQL_TYPE_TINY:		
		return DB_TYPE_BYTE;
	case MYSQL_TYPE_SHORT:		
		return DB_TYPE_SMALLINT;
	case MYSQL_TYPE_LONG:
	case MYSQL_TYPE_INT24:		
		return DB_TYPE_INTEGER;
	case MYSQL_TYPE_LONGLONG:		
		return DB_TYPE_BIGINT;
	case MYSQL_TYPE_DECIMAL:
	case MYSQL_TYPE_NEWDECIMAL:

	case MYSQL_TYPE_FLOAT:		
		return DB_TYPE_FLOAT;
	case MYSQL_TYPE_DOUBLE:		
		return DB_TYPE_FLOAT;
	case MYSQL_TYPE_BIT:		
		return DB_TYPE_BIGINT;
	case MYSQL_TYPE_TIMESTAMP:
		return DB_TYPE_TIMESTAMP;
	case MYSQL_TYPE_DATE:
		return DB_TYPE_DATE;
	case MYSQL_TYPE_TIME:
		return DB_TYPE_TIME;
	case MYSQL_TYPE_DATETIME:
		return DB_TYPE_TIMESTAMP;
	case MYSQL_TYPE_YEAR:
		return DB_TYPE_SMALLINT;
	case MYSQL_TYPE_SET:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_VAR_STRING:
		return DB_TYPE_CHAR;
	case MYSQL_TYPE_GEOMETRY:
	case MYSQL_TYPE_BLOB:
		return DB_TYPE_BINARY;			
	default:
		return DB_TYPE_UNKNOW;
	}
}

