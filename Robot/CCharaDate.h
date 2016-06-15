#pragma once

#include "Common.h"

class CCharaDate
{
public:
	CCharaDate(){}
	~CCharaDate(){}

	void   SetCardData(const char* buf);
	size_t    GetCharaDataSize();

	const char*			GetPTID()const;
	__int64				GetiDigitalPTID()const;
	const char*			GetszCharName()const;

	const unsigned int GetIntegral()const;
	void  SetIntegral(unsigned int gral);
	const unsigned int GetRollCall()const;
	void  SetRollCall(unsigned int roll);
	const unsigned int GetIntegralLadder()const;
	void  SetIntegralLadder(unsigned int ladder);
	const unsigned int GetHoodleRoll()const;
	void  SetHoodleRoll(unsigned int roll);
	const unsigned int GetPileIntegral()const;
	void  SetPileIntegral(unsigned int gral);
private:
	CHARACTER_INFO  m_CharaInfo;
};