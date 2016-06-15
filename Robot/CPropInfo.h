#ifndef _PROP_INFO_H_
#define _PROP_INFO_H_
#pragma once

#include "Common.h"

class CPropInfo
{
public:
	CPropInfo(){}
	~CPropInfo(){}
	size_t    GetPropDataSize();

	void   SetPropData(const char* buf);
	//void	UpdatePropData(PropInfo  &pif);

	unsigned int GetId();
	void SetID(unsigned int id);

	unsigned int GetPropId();
	void SetPropID(unsigned int id);

	unsigned short GetPropImageID();
	void SetPropImageID(unsigned short imageID);

	unsigned char  GetPropNum();
	void  SetPropNum(unsigned char num);

	unsigned int   GetPropTime();
	void  SetPropTime(unsigned int time);

	const char*    GetPropName();
	void SetPropName(const char* str);

	const char*    GetPropDepict();
	void  SetPropDepict(const char* str);

	unsigned char  GetPropType();
	void SetPropType(unsigned char m_Type);

	unsigned int   GetPropDress();
	void SetPropDress(unsigned int money);

	unsigned char  GetIsDress();
	void SetIsDress(unsigned char  dress);
private:
	PropInfo  m_PropInfo;
};
#endif