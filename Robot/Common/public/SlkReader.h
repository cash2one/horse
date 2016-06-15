/******************************************************************************/
/**
*   \file SlkReader.h
*   <pre>
*	Copyright (c) 2005,上海好玩研发中心
*	All rights reserved.
*
*	文件名称：SlkReader.h
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
#ifndef _SLK_SLKREADER_H__
#define _SLK_SLKREADER_H__
//#include "types.h"

#include <vector>

//////////////////////////////////////////////////////////////////////////
#define SLK_EOF	0xFFFFFFFF
#define SLK_ERR 0xFFFFFFFF

#define MAX_SLK_NUM_STRING_LEN	64
#define MAX_SLK_COL_NAME_LEN	128
#define MAX_SLK_VALUE_STRING	2048

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
typedef signed char			sInt8;
typedef unsigned char		uInt8;
typedef signed short		sInt16;
typedef unsigned short		uInt16;
typedef signed int			sInt32;
typedef unsigned long		uInt32;
typedef __int64				sInt64;
typedef unsigned __int64	uInt64;

typedef float				flt32;		// 32-bit IEEE floating point.
typedef double				flt64;		// 64-bit IEEE double.

#ifndef NULL
#define NULL 0
#endif

//////////////////////////////////////////////////////////////////////////
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) if( NULL != (p) ) { delete (p); (p) = NULL; };
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY( p ) if( (p) ) { delete [](p); (p) = NULL; };
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE( p ) if( (p) ) { p->Release(); (p) = NULL; };
#endif
//

/////////////////////////////////////////////////////////////////////////


class IFileReader
{
public:
	virtual					~IFileReader(){};

	virtual bool			OpenFile( const char* pFileName ) = 0;
	virtual void			Close() = 0;

	virtual size_t			Read( void* pBuffer, uInt32 pSizeToRead ) = 0;	
	virtual bool			Seek( uInt32 pFinalPosition, sInt32 pRelativeMovement = SEEK_SET ) = 0;

	virtual uInt32			GetSize() const = 0;

};

//////////////////////////////////////////////////////////////////////////
extern	bool CreateFileReader( IFileReader** pReader );	

//////////////////////////////////////////////////////////////////////////
struct TSLKValue
{
	friend class CFileReadSLK;
	friend struct TSLKRow;
public:
	TSLKValue();
	~TSLKValue();
	TSLKValue(const TSLKValue& slkValue);
	
	sInt32		ToInt32() const;
	flt64		ToFlt64() const;
	const char*	ToField() const;

	operator int();
	operator unsigned int();
	operator unsigned long();
	operator unsigned short();
	operator double();
	operator float();
	operator bool();
	operator const char *();

protected:
	sInt32	intValue;
	double	floatValue;
	// NOTE:这个字符串指针由你来分配，我来释放。
	char*	stringValue;
};


//////////////////////////////////////////////////////////////////////////
struct TSLKColumn
{
	char name[MAX_SLK_COL_NAME_LEN]; 	
};

//////////////////////////////////////////////////////////////////////////
struct TSLKRow
{
	friend class CFileReadSLK;
public:
	TSLKRow();
	~TSLKRow();

	const TSLKValue* GetValue( uInt32 pColumn ) const;
	const bool SetValue( uInt32 pColumn, TSLKValue* pValue );
private:
	TSLKValue*	value;
};

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
class CFileReadSLK
{
public:
						CFileReadSLK();
						~CFileReadSLK();

	// Slow. you must call at initialize time.
	// 同一时刻一个 CFileReadSLK 只能对应一个文件。
	// 因为每调用一次 LoadFromFile 就要打开一次文件，所以比较慢。
	bool				LoadFromFile( const char* pFileName );
	// 关闭 Slk 文件，并且清空 buffer.
	void				Close();

	// 返回行和列的个数。
	uInt32				GetRowCount() const{ return m_row; };
	uInt32				GetColCount() const{ return m_col; };

	// access straight. 没有边界检查。.
	const TSLKValue*	GetValue( uInt32 pRow, uInt32 pCol ) const;
	TSLKValue*			GetValue( uInt32 pRow, uInt32 pCol );

	// 遍历取值。如果返回值为 SLK_EOF 则文件结束。.
	uInt32				GetFirstRow();
	uInt32				GetNextRow();
	uInt32				GetLastRow();

	//const TSLKValue* GetData( const char* pColName );
	// NOTE:返回的值生命期和 CFileReadSLK 相同。
	TSLKValue*			GetData( const char* pColName );
	const TSLKValue*	GetData( uInt32 pColumn ) const;	
	TSLKValue*			GetData( uInt32 pColumn );	

	int GetDataInteger(const char* pColName,int DefualtValue);
	double GetDataDouble(const char* pColName,double DefualtValue);
	LPCTSTR GetDataString(const char* pColName,LPCTSTR DefualtValue);

	const char * GetColName(uInt32 pColumn);

private:
	char*	FetchKnownKeyValue( char* pData, bool& pIsEnd );
	char*	EvalFKeyValue( char* pData, bool& pIsEnd );
	char*	EvalCKeyValue( char* pData, bool& pIsEnd );
	char*	EvalUnknowKey( char* pData, bool& pIsEnd );
	void	EvalKeyLine( char* pData );
	bool	GetInfoFromMemory( void* pMemory, uInt32 pSize );
	uInt32	GetColByName( const char* pName );
	

private:
	TSLKColumn*	m_column;
	TSLKRow*	m_data;
	uInt32		m_curPos;

	uInt32		m_row;
	uInt32		m_col;

	uInt32		m_curRow;
	uInt32		m_curCol;

	struct _TSLKValue
	{
		uInt32		row;
		uInt32		col;
		TSLKValue	value;
	};

	std::vector< _TSLKValue* >	_TSLKValueArray;
};




#endif	// _SLK_SLKREADER_H__


