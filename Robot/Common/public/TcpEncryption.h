#pragma once
#include "arc4.h"

class CTcpEncryption
{
public:
	CTcpEncryption(void);
	~CTcpEncryption(void);
	void InitEncrypt(int nSkipLen=sizeof(WORD));
	void EncryptPack(const char* sPacket, char* sResult, int nPackLen);
	void DecryptPack(char* sPacket, int nPackLen);
private:
	arc4_context	m_encontext;
	arc4_context	m_decontext;
	int				m_nSkipLen;
};
