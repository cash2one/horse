#include "StdAfx.h"
#include ".\tcpencryption.h"

CTcpEncryption::CTcpEncryption(void)
{
	memset(&m_encontext, 0, sizeof(m_encontext));
	memset(&m_decontext, 0, sizeof(m_decontext));
	m_nSkipLen = 0;
}

CTcpEncryption::~CTcpEncryption(void)
{
}

void CTcpEncryption::InitEncrypt(int nSkipLen)
{
	unsigned char vInitCrypto[8] = {0};
	arc4_setup( &m_encontext, (unsigned char *) vInitCrypto, 8 );
	arc4_crypt( &m_encontext, vInitCrypto, 8);
	arc4_setup( &m_encontext, (unsigned char *) vInitCrypto, 8 );
	memcpy(&m_decontext, &m_encontext, sizeof(m_encontext));
	m_nSkipLen = nSkipLen;
}

void CTcpEncryption::EncryptPack(const char* sPacket, char* sResult, int nPackLen)
{
	if(m_nSkipLen)
		memcpy(sResult, sPacket, m_nSkipLen);

	arc4_crypt( &m_encontext, (unsigned char *)sPacket+m_nSkipLen, nPackLen-m_nSkipLen, (unsigned char *)sResult+m_nSkipLen);
}

void CTcpEncryption::DecryptPack(char* sPacket, int nPackLen)
{
	arc4_crypt( &m_decontext, (unsigned char *)sPacket+m_nSkipLen, nPackLen-m_nSkipLen);
}