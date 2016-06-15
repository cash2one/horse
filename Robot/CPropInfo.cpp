#include "stdafx.h"
#include "CPropInfo.h"

size_t    CPropInfo::GetPropDataSize()
{
	size_t size = sizeof(PropInfo);
	return size;
}
void   CPropInfo::SetPropData(const char* buf)
{
	int size = GetPropDataSize();
	memcpy(&m_PropInfo,buf,size);
}
//void	CPropInfo::UpdatePropData(PropInfo  &pif)
//{
	//memcpy(&m_PropInfo,)
//}
unsigned int CPropInfo::GetId()
{
	return m_PropInfo.mID;
}
void CPropInfo::SetID(unsigned int id)
{
	m_PropInfo.mID = id;
}
unsigned int CPropInfo::GetPropId()
{
	return m_PropInfo.mPropID;
}
void CPropInfo::SetPropID(unsigned int id)
{
	m_PropInfo.mPropID = id;
}
unsigned short CPropInfo::GetPropImageID()
{
	return m_PropInfo.mImageID;
}
void CPropInfo::SetPropImageID(unsigned short imageID)
{
	m_PropInfo.mImageID = imageID;
}
unsigned char  CPropInfo::GetPropNum()
{
	return m_PropInfo.mPropNum;
}
void  CPropInfo::SetPropNum(unsigned char num)
{
	m_PropInfo.mPropNum = num;
}
unsigned int   CPropInfo::GetPropTime()
{
	return m_PropInfo.mTime;
}
void  CPropInfo::SetPropTime(unsigned int time)
{
	m_PropInfo.mTime = time;
}
const char*    CPropInfo::GetPropName()
{
	return m_PropInfo.mPropName;
}
void CPropInfo::SetPropName(const char* str)
{
	sprintf(m_PropInfo.mPropName,"%s",str);
}
const char*    CPropInfo::GetPropDepict()
{
	return m_PropInfo.mPropDepict;
}
void  CPropInfo::SetPropDepict(const char* str)
{
	sprintf(m_PropInfo.mPropDepict,"%s",str);
}
unsigned char  CPropInfo::GetPropType()
{
	return m_PropInfo.mType;
}
void CPropInfo::SetPropType(unsigned char m_Type)
{
	m_PropInfo.mType = m_Type;
}
unsigned int   CPropInfo::GetPropDress()
{
	return m_PropInfo.mDress;
}
void CPropInfo::SetPropDress(unsigned int money)
{
	m_PropInfo.mDress = money;
}
unsigned char  CPropInfo::GetIsDress()
{
	return m_PropInfo.mIsDress;
}
void CPropInfo::SetIsDress(unsigned char  dress)
{
	m_PropInfo.mIsDress = dress;
}