#include "stdafx.h"
#include "Common.h"
#include "CCharaDate.h"

void   CCharaDate::SetCardData(const char* buf)
{
	int size = GetCharaDataSize();
	memcpy(&m_CharaInfo,buf,size);
}
size_t    CCharaDate::GetCharaDataSize()
{
	size_t size = sizeof(CHARACTER_INFO);
	return size;
}

const char* CCharaDate::GetPTID()const
{
	return m_CharaInfo.szPTID;
}
INT64 CCharaDate::GetiDigitalPTID()const
{
	return m_CharaInfo.iDigitalPTID;
}
const char* CCharaDate::GetszCharName()const
{
	return m_CharaInfo.szCharName;
}
const unsigned int CCharaDate::GetIntegral()const
{
	return m_CharaInfo.mIntegral;
}
void  CCharaDate::SetIntegral(unsigned int gral)
{
	m_CharaInfo.mIntegral = gral;
}
const unsigned int CCharaDate::GetRollCall()const
{
	return m_CharaInfo.mRollCall;
}
void  CCharaDate::SetRollCall(unsigned int roll)
{
	m_CharaInfo.mRollCall = roll;
}
const unsigned int CCharaDate::GetIntegralLadder()const
{
	return m_CharaInfo.mIntegralLadder;
}
void  CCharaDate::SetIntegralLadder(unsigned int ladder)
{
	m_CharaInfo.mIntegralLadder = ladder;
}
const unsigned int CCharaDate::GetHoodleRoll()const
{
	return m_CharaInfo.mHoodleRoll;
}
void  CCharaDate::SetHoodleRoll(unsigned int roll)
{
	m_CharaInfo.mHoodleRoll = roll;
}
const unsigned int CCharaDate::GetPileIntegral()const
{
	return m_CharaInfo.mPileIntegral;
}
void  CCharaDate::SetPileIntegral(unsigned int gral)
{
	m_CharaInfo.mPileIntegral = gral;
}