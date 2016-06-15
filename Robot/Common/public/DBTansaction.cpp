#include "StdAfx.h"
#include ".\dbtansaction.h"

DiffSizeMiniObjectsPool CDBTansaction::m_TransMemPool(10000,4*1024-48);

IMPLEMENT_CLASS_INFO_STATIC(CDBTansaction,CNameObject);
CDBTansaction::CDBTansaction(void)
{
	m_IsSucceed=false;
	++m_iObjectCount;
	memset(&m_AddToManagerTime, 0, sizeof(m_AddToManagerTime));
}

CDBTansaction::~CDBTansaction(void)
{
	
}

bool CDBTansaction::OnExecute(IDBConnection * pConnection)
{
	return false;
}

void CDBTansaction::OnFinish()
{
}

