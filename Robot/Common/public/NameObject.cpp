#include "stdafx.h"
#include "NameObject.h"

CLASS_INFO	CNameObject::m_CNameObjectClassInfo={"CNameObject",NULL};

std::map<std::string,CLASS_INFO *> * CNameObject::m_pAllClassInfo=NULL;
int CNameObject::m_AllClassCount=0;

CClassInfoRegister CNameObject::m_CNameObjectClassInfoRegister("CNameObject",&m_CNameObjectClassInfo);

CClassInfoRegister::CClassInfoRegister(LPCTSTR ClassName,CLASS_INFO * pClassInfo)
{
	if(CNameObject::m_pAllClassInfo==NULL)
		CNameObject::m_pAllClassInfo=new std::map<std::string,CLASS_INFO *>;
	(*CNameObject::m_pAllClassInfo)[std::string(ClassName)]=pClassInfo;
	CNameObject::m_AllClassCount++;
}
CClassInfoRegister::~CClassInfoRegister()
{
	CNameObject::m_AllClassCount--;
	if(CNameObject::m_AllClassCount<=0)
		SAFE_DELETE(CNameObject::m_pAllClassInfo);
}

CNameObject * CNameObject::CreateObject()
{
	return new CNameObject();
}


CLASS_INFO& CNameObject::GetThisClassInfo()
{
	return m_CNameObjectClassInfo;
}

CLASS_INFO * CNameObject::GetClassInfo(LPCTSTR ClassName)
{
	return (*m_pAllClassInfo)[ClassName];
}

int CNameObject::GetAllClassCount()
{
	return m_AllClassCount;
}