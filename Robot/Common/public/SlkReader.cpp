/******************************************************************************/
/**
*   \file SlkReader.cpp
*   <pre>
*	Copyright (c) 2005,上海好玩研发中心
*	All rights reserved.
*
*	文件名称：SlkReader.cpp
*	摘    要：
*
*	当前版本：1.0
*	作    者：赵文生
*	电子邮件：ZhaoWenSheng@yanfa.com
*   建立日期：2005-12-15   10:50
*	完成日期：
*   </pre>
*/
/******************************************************************************/
#include "stdafx.h"
//#include "IFileReader.h"
#include "SlkReader.h"
#include "cassert"

///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class CFileReader	: public IFileReader
{
public:
	CFileReader();
	~CFileReader();

	bool	OpenFile( const char* pFileName );
	void	Close();

	size_t	Read( void* pBuffer, uInt32 pSizeToRead );	
	bool	Seek( uInt32 pFinalPosition, sInt32 pRelativeMovement = SEEK_SET );

	virtual uInt32	GetSize() const { return m_size; };

protected:
	FILE*	m_file;
	uInt32	m_size;
};

//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
CFileReader::CFileReader()
//------------------------------------------------------------------------
{
	m_size		= 0;
	m_file		= NULL;
}

//------------------------------------------------------------------------
CFileReader::~CFileReader()
//------------------------------------------------------------------------
{
	Close();	
}

//------------------------------------------------------------------------
bool CFileReader::OpenFile( const char* pFileName )
//------------------------------------------------------------------------
{
	assert( NULL == m_file );
	if ( NULL == pFileName ) 
	{
		return false; 
	}
	char fileName[MAX_PATH];
	_snprintf( fileName, SIZEOF_STR(fileName), "%s", pFileName );
	m_file = fopen( fileName, "rb");
	if ( NULL != m_file )
	{
		// get FileSize
		fseek( m_file, 0, SEEK_END );
		m_size = ftell( m_file );
		fseek( m_file, 0, SEEK_SET );
		return true;
	}
	return false;
}

//------------------------------------------------------------------------
void CFileReader::Close()
//------------------------------------------------------------------------
{
	if ( NULL != m_file)
	{
		fclose( m_file);
	}
	m_size		= 0;
	m_file		= NULL;
}

//------------------------------------------------------------------------
size_t CFileReader::Read( void* pBuffer, uInt32 pSizeToRead )	
//------------------------------------------------------------------------
{
	if( m_file )
		return fread( pBuffer, 1, pSizeToRead, m_file );
	else 
		return 0;
}

//------------------------------------------------------------------------
bool CFileReader::Seek( uInt32 pFinalPosition, sInt32 pRelativeMovement )
//------------------------------------------------------------------------
{
	if( m_file )
		return (fseek( m_file, pFinalPosition, pRelativeMovement ) == 0);
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
bool CreateFileReader( IFileReader** pReader )	
//------------------------------------------------------------------------
{
	IFileReader* fileReader = NULL;
	fileReader = new CFileReader();
	if( fileReader == NULL )
		return false;
	*pReader = fileReader;
	return true;
}

//////////////////////////////////////////////////////////////////////

namespace
{
	const char* sNullString = "";
};

#define MAX_NUMBERSTRING_LENGTH		64

//------------------------------------------------------------------------
TSLKValue::TSLKValue()
//------------------------------------------------------------------------
{
	intValue	= 0;
	floatValue	= 0.0f;
	stringValue	= NULL;
}
//------------------------------------------------------------------------
TSLKValue::~TSLKValue()
//------------------------------------------------------------------------
{
	//if( NULL != stringValue )
	//{
	//	delete stringValue;
	//	stringValue = NULL;
	//}
	SAFE_DELETE_ARRAY(stringValue);
}

TSLKValue::TSLKValue(const TSLKValue& slkValue)
{
	intValue = slkValue.intValue;
	floatValue = slkValue.floatValue;
	stringValue = NULL;
	
	if(NULL != slkValue.stringValue)
	{
		size_t len = strlen( slkValue.stringValue );
		stringValue = new char[len+2];
		memcpy( stringValue, slkValue.stringValue, len+1 );
	}
}
//------------------------------------------------------------------------
sInt32	TSLKValue::ToInt32() const
//------------------------------------------------------------------------
{
	return intValue;
}

//------------------------------------------------------------------------
flt64 TSLKValue::ToFlt64() const
//------------------------------------------------------------------------
{
	return floatValue;
}

//------------------------------------------------------------------------
const char* TSLKValue::ToField() const
//------------------------------------------------------------------------
{
	if( NULL != stringValue )
	{
		return stringValue;
	}else
	{
		return sNullString;
	}
}

TSLKValue::operator int()
{
	return ToInt32();
}

TSLKValue::operator unsigned int()
{
	return (unsigned int)ToInt32();
}

TSLKValue::operator unsigned long()
{
	return (unsigned long)ToInt32();
}

TSLKValue::operator unsigned short()
{
	return (unsigned short)ToInt32();
}

TSLKValue::operator double()
{
	return ToFlt64();
}

TSLKValue::operator float()
{
	return (float)ToFlt64();
}

TSLKValue::operator bool()
{
	return ToInt32() > 0;
}

TSLKValue::operator const char *()
{
	return ToField();
}
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
TSLKRow::TSLKRow()
//------------------------------------------------------------------------
{
	value = NULL;
}

//------------------------------------------------------------------------
TSLKRow::~TSLKRow()
//------------------------------------------------------------------------
{
	SAFE_DELETE_ARRAY( value );
}

//------------------------------------------------------------------------
const TSLKValue* TSLKRow::GetValue( uInt32 pColumn ) const
//------------------------------------------------------------------------
{
	return &( value[pColumn] );
}

//------------------------------------------------------------------------
const bool TSLKRow::SetValue( uInt32 pColumn, TSLKValue* pValue )
//------------------------------------------------------------------------
{
	value[pColumn].intValue		= pValue->intValue;
	value[pColumn].floatValue	= pValue->floatValue;
	if( NULL != pValue->stringValue )
	{
		size_t len = strlen( pValue->stringValue );
		SAFE_DELETE_ARRAY( value[pColumn].stringValue );
		value[pColumn].stringValue = new char[len+2];

		string str = pValue->stringValue;
		int npos = (int)str.find("\\n");//解释转义字符
		const char* cs1p = "\n";
		while(npos != -1)
		{
			str = str.replace(npos, 2 ,cs1p);
			npos = (int)str.find("\\n", npos);
		}

		memcpy( value[pColumn].stringValue, str.c_str()/*pValue->stringValue*/,str.length()+1 );
	}else
	{
		SAFE_DELETE_ARRAY( value[pColumn].stringValue );
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------
CFileReadSLK::CFileReadSLK()
//------------------------------------------------------------------------
{
	m_column	= NULL;
	m_data		= NULL;

	m_row		= 0;
	m_col		= 0;

	m_curRow	= 0;
	m_curCol	= 0;

	m_curPos	= 0;
}

//------------------------------------------------------------------------
CFileReadSLK::~CFileReadSLK()
//------------------------------------------------------------------------
{
	Close();
}

//------------------------------------------------------------------------
bool CFileReadSLK::LoadFromFile( const char* pFileName )
//------------------------------------------------------------------------
{
	assert( NULL == m_data );
	assert( NULL == m_column );

	//OutputDebugString(pFileName);
	//OutputDebugString("\n==================================\n");
	IFileReader* fileReader = NULL;
	if( !CreateFileReader( &fileReader ) )
		return false;

	bool isLoadOk = true;
	for( unsigned int i = 0; i < _TSLKValueArray.size(); i++ )
	{
		_TSLKValue* p = _TSLKValueArray[i];
		SAFE_DELETE( p );
	}
	_TSLKValueArray.clear();
	char*	buffer = NULL;
	uInt32	bufferSize = 0;
	if(fileReader)
	{
		if( !(fileReader->OpenFile( pFileName )) )
		{
			isLoadOk = false;
			goto OPEN_FILE_FAIL;
		}
		bufferSize = fileReader->GetSize();
		assert( bufferSize > 0 );
		buffer = new char[bufferSize+1];
		memset( buffer, 0, (bufferSize + 1) * sizeof(char) );
		char tmpBuffer[1024];
		memset( tmpBuffer, 0, 1024 * sizeof(char) );

		char* ptrBuffer = buffer;
		size_t size = fileReader->Read( (void*)tmpBuffer, 1024 );
		assert( "出现异常错误！" && size <= 1024 );
		size_t count = 0;
		while( size > 0 )		
		{
			memcpy( ptrBuffer, tmpBuffer, size );
			ptrBuffer	+= size;
			count		+= size;
			memset( tmpBuffer, 0, 1024 * sizeof(char) );
			size = fileReader->Read( tmpBuffer, 1024 );
			assert( "出现异常错误！" && size <= 1024 );
		}
		assert( "文件大小错误！" && bufferSize == count );
		if( bufferSize != count )
			isLoadOk = false;
		else if( !GetInfoFromMemory( buffer, bufferSize ) )
			isLoadOk = false;

		SAFE_DELETE_ARRAY( buffer );
	}

OPEN_FILE_FAIL:
	if(fileReader)
		fileReader->Close();
	SAFE_DELETE( fileReader );

	for( unsigned int i = 0; i < _TSLKValueArray.size(); i++ )
	{
		_TSLKValue* p = _TSLKValueArray[i];
		SAFE_DELETE( p );
	}
	_TSLKValueArray.clear();

	return isLoadOk;
}

//------------------------------------------------------------------------
static bool IsNumberChar( char pChar )
//------------------------------------------------------------------------
{

	if( (pChar >= '0' && pChar <= '9') || ('.' == pChar) || ('-' == pChar) || ('+' == pChar) )
	{
		return true;
	}
	else
		return false;
}

//------------------------------------------------------------------------
static bool IsReturnChar( char pChar )
//------------------------------------------------------------------------
{
	if( '\n' == pChar || '\r' == pChar )
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////////
enum emSlkKeyType
{
	ESKY_Unkown = 0,
	ESKY_F,
	ESKY_C,
	ESKY_End,
	ESKY_DataEnd
};

//------------------------------------------------------------------------
static emSlkKeyType GetLineKeyType( char* pString )
//------------------------------------------------------------------------
{
	if( '\0' == *pString )
		return ESKY_DataEnd;

	if( 'F' == *pString )
	{
		if( ';' == *(pString + 1) )
			return ESKY_F;
	}
	if( 'C' == *pString )
	{
		if( ';' == *(pString + 1) )
			return ESKY_C;
	}
	if( 'E' == *pString )
	{
		return ESKY_End;
	}
	return ESKY_Unkown;
}

//////////////////////////////////////////////////////////////////////////
enum emSlkValueType
{
	ESVT_Unkown = 0,
	ESVT_X,
	ESVT_Y,
	ESVT_K,
	ESVT_A,//批注内容
};

//------------------------------------------------------------------------
static emSlkValueType GetValueType( char pChar )
//------------------------------------------------------------------------
{
	if('X' == pChar) 
		return ESVT_X;

	if('Y' == pChar)
		return ESVT_Y;

	if( 'K' == pChar )
		return ESVT_K;

	if('A' == pChar)
		return ESVT_A;

	return ESVT_Unkown;
}

//------------------------------------------------------------------------
static char* FetchToNextLine( char* pData, bool& pIsEnd )
//------------------------------------------------------------------------
{
	char* ptrData = pData;

	for( ;; )
	{
		if( '\n' == *ptrData )
		{
			ptrData++;
			return ptrData;
		}
		if( '\0' == *ptrData )
		{
			pIsEnd = true;
			return ptrData;
		}
		ptrData++;
	}
}

//------------------------------------------------------------------------
static char* GetSLKNumValue( char* pData, char* pOutValue, bool& pIsEnd )
//------------------------------------------------------------------------
{
	char* ptrValue = pOutValue;
	char* ptrData  = pData;
	int len = 0;
	for(;;)
	{
		if( '\n' == *ptrData++ ) 
			return ptrData;

		if('\0' == *ptrData) 
		{
			pIsEnd = true;
			return ptrData;
		}

		if( ';' == *ptrData)
		{
			return ptrData;
		}

		if( !IsNumberChar( *ptrData ) )
			return ptrData;

		if( len < MAX_SLK_NUM_STRING_LEN-1 )
			*(ptrValue++) = *(ptrData);
		else
		{
			assert( "数字长度错误！" && false );
		}
		len++;
	}

	return NULL;
}

//------------------------------------------------------------------------
static char* GetSLKStringValue( char* pData, char* pOutValue, bool& pIsEnd )
//------------------------------------------------------------------------
{
	char* ptrValue = pOutValue;
	char* ptrData  = pData;
	ptrData++;
	int len = 0;
	//进入字符串
	for(;;)
	{
		if('\n' == *ptrData++)
		{
			if( '"' == *(ptrValue-1) )
				*(ptrValue-1) = '\0';
			return ptrData;
		}
		if('\0' == *ptrData)
		{
			pIsEnd = true;
			return ptrData;
		}

		if( len < MAX_SLK_VALUE_STRING-1 )
		{
			if( !IsReturnChar(*ptrData) )
			{
				*(ptrValue++) = *(ptrData);
			}
		}
		else
		{
			assert( "SLK文件出错！" && false );
			// ("<<<ERRROR>>>[GetSLKStringValue] \n" );
		}
		len++;
	}

	return NULL;
}

//------------------------------------------------------------------------
char* CFileReadSLK::FetchKnownKeyValue( char* pData, bool& pIsEnd )
//------------------------------------------------------------------------
{
	char* ptrData = pData;
	++ptrData;
	if( ';' != *ptrData)
	{
		pIsEnd = true;
		return ptrData;
	}
	for(;;) 
	{
		if('\n' == *ptrData++ )	
		{
			return ptrData;
		}

		if('\0' == *ptrData)
		{
			pIsEnd = true;
			return ptrData;
		}
	
		emSlkValueType valueType = GetValueType( *ptrData );
	
		switch( valueType ) 
		{
		case ESVT_X:
			{
				char strValue[MAX_SLK_NUM_STRING_LEN];
				memset( strValue, 0, sizeof(char)*MAX_SLK_NUM_STRING_LEN );
				ptrData = GetSLKNumValue( ptrData, strValue, pIsEnd );
				if( 0 < strlen(strValue) && strlen(strValue) < 11)
				{
					m_curCol = atoi( strValue ) -1;
					if(m_curCol > 300)
					{
						m_curCol = 0;
					}
					if( m_curCol < 0 )
					{
						m_curCol = 0;
						//ZM::Log::global().log( "<<<ERROR>>> Error SLK File." );
					}
				}
				else
				{
					ErrorLog(3,"---------slkerrr-----------");
				}
			}
			break;
		case ESVT_Y:
			{
				char strValue[MAX_SLK_NUM_STRING_LEN];
				memset( strValue, 0, sizeof(char)*MAX_SLK_NUM_STRING_LEN );
				ptrData = GetSLKNumValue( ptrData, strValue, pIsEnd );
				if( 0 < strlen(strValue) && strlen(strValue) < 11)
				{
					m_curRow = atoi( strValue ) - 1;

					if(m_curRow > 5000)
					{
						m_curRow = 0;
					}
					if( m_curRow < 0 )
					{
						m_curRow = 0;
						//ZM::Log::global().log( "<<<ERROR>>> Error SLK File." );
					}
				//OutputDebugString("\n");
				}
				else
				{
					ErrorLog(3,"1111111111---------slkerrr-----------");
				}
			}
			break;
		case ESVT_K:
			{
				if( '"' != *(ptrData+1) )
				{
					if( IsNumberChar( *(ptrData+1) ) )
					{
						char strValue[MAX_SLK_NUM_STRING_LEN];
						memset( strValue, 0, sizeof(char)*MAX_SLK_NUM_STRING_LEN );
						ptrData = GetSLKNumValue( ptrData, strValue, pIsEnd );

						m_row = max( m_curRow+1, m_row );
						m_col = max( m_curCol+1, m_col );	

						_TSLKValue* value = new _TSLKValue;
						value->col	= m_curCol;
						value->row	= m_curRow;
						value->value.intValue	= atoi( strValue );
						value->value.floatValue = atof( strValue );
						size_t len = strlen( strValue );
						if( MAX_SLK_NUM_STRING_LEN <= len )
						{
							ErrorLog(3,"222222222---------slkerrr-----------");
						}
						value->value.stringValue= new char[len+2];
						memset( value->value.stringValue, 0, (len+2)*sizeof(char) );
						strcpy( value->value.stringValue, strValue );

						_TSLKValueArray.push_back( value );

						//OutputDebugString(strValue);
						//OutputDebugString("\t");
						//ZM::Log::global().log( "[%d,%d](%f)\n", m_curRow, m_curCol, atof( strValue ) );
					}
				}
				else
				{
					char strValue[MAX_SLK_VALUE_STRING];
					memset( strValue, 0, sizeof(char)*MAX_SLK_VALUE_STRING );
					ptrData = GetSLKStringValue( ptrData, strValue, pIsEnd );

					m_row = max( m_curRow+1, m_row );
					m_col = max( m_curCol+1, m_col );	

					_TSLKValue* value = new _TSLKValue;
					value->col	= m_curCol;
					value->row	= m_curRow;
					value->value.intValue	= atoi( strValue );
					value->value.floatValue = atof( strValue );
					size_t len = strlen( strValue );
					if( MAX_SLK_VALUE_STRING <= len )
					{
						ErrorLog(3,"3333---------slkerrr-----------");
					}
					value->value.stringValue= new char[len+2];
					memset( value->value.stringValue, 0, (len+2)*sizeof(char) );
					strcpy( value->value.stringValue, strValue );

					_TSLKValueArray.push_back( value );

					//OutputDebugString(strValue);
					//OutputDebugString("\t");
					//ZM::Log::global().log( "[%d,%d](%s)\n", m_curRow, m_curCol, strValue );
				}
			}
			break;
		case ESVT_A:
			while('\n'!= *ptrData && '\0'!= *ptrData)ptrData++;
			break;
		case ESVT_Unkown:
			break;
		}
	}
}


//------------------------------------------------------------------------
char* CFileReadSLK::EvalFKeyValue( char* pData, bool& pIsEnd )
//------------------------------------------------------------------------
{
	return FetchKnownKeyValue( pData, pIsEnd );
}

//------------------------------------------------------------------------
char* CFileReadSLK::EvalCKeyValue( char* pData, bool& pIsEnd )
//------------------------------------------------------------------------
{
	return FetchKnownKeyValue( pData, pIsEnd );
}

//------------------------------------------------------------------------
char* CFileReadSLK::EvalUnknowKey( char* pData, bool& pIsEnd )
//------------------------------------------------------------------------
{
	return FetchToNextLine( pData, pIsEnd );
}


////------------------------------------------------------------------------
//char* CFileReadSLK::EvalKeyLine( char* pData, bool& pIsEnd )
////------------------------------------------------------------------------
//// OLD.
//{
//	//assert( strlen( pData ) > 0 );
//	emSlkKeyType type = GetLineKeyType( pData );
//	switch( type ) 
//	{
//	case ESKY_Unkown:
//		pData = EvalUnknowKey( pData, pIsEnd );
//		break;
//	case ESKY_F:
//		pData = EvalFKeyValue( pData, pIsEnd );
//		break;
//	case ESKY_C:
//		pData = EvalCKeyValue( pData, pIsEnd );
//		break;
//	case ESKY_End:
//		pIsEnd	= true;
//		//ZM::Log::global().log( "---- Read SLK File End ---- \n");
//		break;
//	case ESKY_DataEnd:
//		pIsEnd	= true;
//		break;
//	}
//
//	//ZM::Log::global().log( "[EvalKeyLine]%d \n", (int)type );
//
//	if( !pIsEnd )
//		return EvalKeyLine( pData, pIsEnd );
//	else
//		return pData;
//}

//------------------------------------------------------------------------
void CFileReadSLK::EvalKeyLine( char* pData )
//------------------------------------------------------------------------
{
	//NiOutputDebugString( "CFileReadSLK::EvalKeyLine===================\n" );
	bool isEnd = false;
	for(;;)
	{
		emSlkKeyType type = GetLineKeyType( pData );
		switch( type ) 
		{
		case ESKY_Unkown:
			pData = EvalUnknowKey( pData, isEnd );
			break;
		case ESKY_F:
			pData = EvalFKeyValue( pData, isEnd );
			break;
		case ESKY_C:
			pData = EvalCKeyValue( pData, isEnd );
			break;
		case ESKY_End:
		case ESKY_DataEnd:
			isEnd	= true;
			break;
		}
		if( isEnd )
			break;	
	}
	//NiOutputDebugString( "CFileReadSLK::EvalKeyLine--------------------\n" );
}

//------------------------------------------------------------------------
bool CFileReadSLK::GetInfoFromMemory( void* pMemory, uInt32 pSize )
//------------------------------------------------------------------------
{
	if( NULL == pMemory )
		return false;

	char* buffer	= (char*)pMemory;
	m_curRow		= 0;
	m_curCol		= 0;
	m_col			= 0;
	m_row			= 0;

	EvalKeyLine( buffer );
	//OutputDebugString("\n");

	if( m_col > 0 && m_row > 0  )
	{
		m_row = m_row - 1;
		uInt32 i;
		m_column = new TSLKColumn[m_col];
		for( i = 0; i < m_col; i++ )
		{
			memset( m_column[i].name, 0, MAX_SLK_COL_NAME_LEN*sizeof(char) );
			_snprintf( m_column[i].name, SIZEOF_STR(m_column[i].name), "Column%03d", i );
		}

		m_data	= new TSLKRow[m_row];
		for( i = 0; i < (m_row); i++ )
		{
			m_data[i].value = new TSLKValue[m_col];
		}
		for( i = 0; i < _TSLKValueArray.size(); i++ )
		{
			_TSLKValue* p = _TSLKValueArray[i];
			if( p->row < 1 )
			{
				if( NULL != (p->value.stringValue) )
				{
					size_t len = strlen(p->value.stringValue);
					assert( "列名称太长！（128字符以内）" && MAX_SLK_COL_NAME_LEN > len );
					memset( m_column[p->col].name, 0, MAX_SLK_COL_NAME_LEN*sizeof(char) );
					memcpy( m_column[p->col].name, p->value.stringValue, min((MAX_SLK_COL_NAME_LEN-1),len) );
				}
			}else
				m_data[(p->row-1)].SetValue( p->col, &(p->value) );
		}
	}

	return true;
}

//------------------------------------------------------------------------
void CFileReadSLK::Close()
//------------------------------------------------------------------------
{
	SAFE_DELETE_ARRAY( m_column );
	if( NULL != m_data )
	{
		for( uInt32 i = 0; i < (m_row); i++ )
		{
			SAFE_DELETE_ARRAY( m_data[i].value );
		}
	}
	SAFE_DELETE_ARRAY( m_data );

	m_row		= 0;
	m_col		= 0;
	m_curRow	= 0;
	m_curCol	= 0;
	m_curPos	= 0;
}

//------------------------------------------------------------------------
const TSLKValue* CFileReadSLK::GetValue( uInt32 pRow, uInt32 pCol ) const
//------------------------------------------------------------------------
{
	if( ( pRow < m_row ) && ( pCol < m_col ) )
		return m_data[pRow].GetValue( pCol );
	else
		return NULL;
}

//------------------------------------------------------------------------
TSLKValue* CFileReadSLK::GetValue( uInt32 pRow, uInt32 pCol ) 
//------------------------------------------------------------------------
{
	if( ( pRow < m_row ) && ( pCol < m_col ) )
		return (TSLKValue*)(m_data[pRow].GetValue( pCol ));
	else
		return NULL;
}

//------------------------------------------------------------------------
uInt32	CFileReadSLK::GetFirstRow()
//------------------------------------------------------------------------
{
	if( 0 == m_row )
		return SLK_EOF;
	else
	{
		m_curPos = 0;
		return m_curPos;
	}
}

//------------------------------------------------------------------------
uInt32 CFileReadSLK::GetNextRow()
//------------------------------------------------------------------------
{
	if( m_curPos < m_row - 1 )
	{
		m_curPos++;
		return m_curPos;
	}else
		return SLK_EOF;
}

//------------------------------------------------------------------------
uInt32	CFileReadSLK::GetLastRow()
//------------------------------------------------------------------------
{
	if( m_row > 0 )
	{
		m_curPos = m_row - 1;
		return m_curPos;
	}else
	{
		m_curPos = 0;
		return SLK_EOF;
	}
}

//------------------------------------------------------------------------
TSLKValue* CFileReadSLK::GetData( const char* pColName ) 	
//------------------------------------------------------------------------
{
	uInt32 id = this->GetColByName( pColName );
	if( SLK_ERR != id )
		return GetValue( m_curPos, id );
	else
		return NULL;
}

//------------------------------------------------------------------------
const TSLKValue* CFileReadSLK::GetData( uInt32 pColumn ) const	
//------------------------------------------------------------------------
{
	return GetValue( m_curPos, pColumn );
}

//------------------------------------------------------------------------
TSLKValue* CFileReadSLK::GetData( uInt32 pColumn ) 	
//------------------------------------------------------------------------
{
	return GetValue( m_curPos, pColumn );
}

int CFileReadSLK::GetDataInteger(const char* pColName,int DefualtValue)
{
	TSLKValue* pValue=GetData(pColName);
	if(pValue)
		return pValue->ToInt32();
	else
		return DefualtValue;
}

double CFileReadSLK::GetDataDouble(const char* pColName,double DefualtValue)
{
	TSLKValue* pValue=GetData(pColName);
	if(pValue)
		return pValue->ToFlt64();
	else
		return DefualtValue;
}

LPCTSTR CFileReadSLK::GetDataString(const char* pColName,LPCTSTR DefualtValue)
{
	TSLKValue* pValue=GetData(pColName);
	if(pValue)
		return pValue->ToField();
	else
		return DefualtValue;
}

//------------------------------------------------------------------------
uInt32 CFileReadSLK::GetColByName( const char* pName )
//------------------------------------------------------------------------
{
	if( NULL == m_column )
		return SLK_ERR;

	for( uInt32 i = 0; i < m_col; i++ )
	{
		if( strcmp( m_column[i].name, pName ) == 0 )
		{
			return i;
		}
	}
	return SLK_ERR;
}

const char * CFileReadSLK::GetColName(uInt32 pColumn)
{
	if(pColumn<m_col)
		return m_column[pColumn].name;
	else
		return NULL;
}
