#pragma once


class xPacket
{
public:
	xPacket()
	{
		m_pBuf = NULL;
		m_iSize = 0;
		m_iMaxSize = 0;
		m_bBuildInBuffer = FALSE;
	}

	xPacket( int nSize )
	{
		m_pBuf = NULL;
		m_iSize = 0;
		m_iMaxSize = 0;
		m_bBuildInBuffer = FALSE;
		create( nSize );
	}

	xPacket( char * pbuf, int nSize )
	{
		m_pBuf = NULL;
		m_iSize = 0;
		m_iMaxSize = 0;
		m_bBuildInBuffer = FALSE;
		create( pbuf, nSize );
	}

	virtual ~xPacket(void)
	{
		destroy();
	}

	BOOL	create( char * pbuf, int nSize )
	{
		destroy();
		m_bBuildInBuffer = FALSE;
		m_pBuf = pbuf;
		m_iMaxSize = nSize;
		return TRUE;
	}

	BOOL	create( int nSize )
	{
		destroy();
		if( nSize > 0 )
		{
			m_bBuildInBuffer = TRUE;
			m_pBuf = new char[nSize];
			m_iMaxSize = nSize;
		}
		return TRUE;
	}

	BOOL notcreated()
	{
		return (m_pBuf == NULL);
	}
	//BOOL	recreate( char * pbuf, int nSize )
	//{
	//	destroy();
	//	return create( pbuf, nSize );
	//}

	//BOOL	recreate( int nSize )
	//{
	//	destroy();
	//	return create( nSize );
	//}

	VOID	destroy()
	{
		if( m_bBuildInBuffer )
			delete []m_pBuf;
		m_pBuf = NULL;
		m_iMaxSize = 0;
		m_iSize = 0;
		m_bBuildInBuffer = FALSE;
	}

	BOOL	push( const char * pszString )
	{
		return push( (LPVOID)pszString, (int)strlen( pszString ) );
	}

	BOOL	push( LPCVOID lpData, int iDatasize )
	{
		if( m_iMaxSize - m_iSize < iDatasize )return FALSE;
		memcpy( m_pBuf + m_iSize, lpData, iDatasize );
		m_iSize += iDatasize;
		return TRUE;
	}
	BOOL	push_const( UINT Data, int iDatasize )
	{
		if( m_iMaxSize - m_iSize < iDatasize )return FALSE;
		memcpy( m_pBuf + m_iSize, &Data, iDatasize );
		m_iSize += iDatasize;
		return TRUE;
	}
	BOOL	push_const( ULONG64 Data, int iDatasize )
	{
		if( m_iMaxSize - m_iSize < iDatasize )return FALSE;
		memcpy( m_pBuf + m_iSize, &Data, iDatasize );
		m_iSize += iDatasize;
		return TRUE;
	}
	BOOL	pop( LPVOID lpData, int iDatasize )
	{
		if( m_iSize < iDatasize )return FALSE;
		memcpy( lpData, m_pBuf + m_iSize, iDatasize );
		//		m_iSize -= iDatasize;
		return free( iDatasize );
	}
	BOOL	peek(int& iStartPos, LPVOID lpData, int iDatasize )
	{
		if( m_iSize-iStartPos < iDatasize )return FALSE;
		memcpy( lpData, m_pBuf+iStartPos, iDatasize );
		iStartPos+=iDatasize;
		//		m_iSize -= iDatasize;
		return TRUE;
	}
	BOOL	free( int iDatasize )
	{
		if( m_iSize < iDatasize )return FALSE;
		m_iSize -= iDatasize;
		if( m_iSize != 0 )
			memmove( m_pBuf, m_pBuf + iDatasize, m_iSize );
		return TRUE;
	}
	VOID	clear()
	{
		m_iSize = 0;
	}

	int	getsize(){return m_iSize;}
	const char * getbuf(){ return m_pBuf;}
	int	getmaxsize(){return m_iMaxSize;}
	int	getfreesize(){return m_iMaxSize - m_iSize;}
	void	setsize( int nSize ){ m_iSize = nSize;}
private:

	char * m_pBuf;
	int	m_iSize;
	int	m_iMaxSize;
	BOOL	m_bBuildInBuffer;
};