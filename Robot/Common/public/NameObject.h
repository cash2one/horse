#pragma once

class CNameObject;

typedef CNameObject * (*OBJECT_CREATE_FN)();



struct CLASS_INFO
{
	LPCTSTR				ClassName;
	CLASS_INFO *		pParentClassInfo;
	OBJECT_CREATE_FN	pObjectCreateFn;
	bool operator==(const CLASS_INFO& ClassInfo)
	{
		return strcmp(ClassName,ClassInfo.ClassName)==0;
	}
	bool operator!=(const CLASS_INFO& ClassInfo)
	{
		return strcmp(ClassName,ClassInfo.ClassName)!=0;
	}
	bool operator==(LPCTSTR Name)
	{
		return strcmp(ClassName,Name)==0;
	}
	bool operator!=(LPCTSTR Name)
	{
		return strcmp(ClassName,Name)!=0;
	}
	bool IsKindOf(const CLASS_INFO& ClassInfo)
	{
		if(*this==ClassInfo)
			return true;
		if(pParentClassInfo)
			return pParentClassInfo->IsKindOf(ClassInfo);
		return false;
	}
	bool IsKindOf(LPCTSTR Name)
	{
		if(*this==Name)
			return true;
		if(pParentClassInfo)
			return pParentClassInfo->IsKindOf(Name);
		return false;
	}
	bool IsKindOfFast(const CLASS_INFO& ClassInfo)
	{
		if(this==&ClassInfo)
			return true;
		if(pParentClassInfo)
			return pParentClassInfo->IsKindOfFast(ClassInfo);
		return false;
	}
	CNameObject * CreateObject()
	{
		if(pObjectCreateFn)
			return (*pObjectCreateFn)();
		else
			return NULL;
	}
};

class CClassInfoRegister
{
public:
	CClassInfoRegister(LPCTSTR ClassName,CLASS_INFO * pClassInfo);
	~CClassInfoRegister();
};

#define DECLARE_CLASS_INFO(ClassName) \
protected:\
	static CLASS_INFO	m_##ClassNameClassInfo;\
	static CClassInfoRegister m_##ClassNameClassInfoRegister;\
public:\
	virtual CLASS_INFO& GetClassInfo()\
	{\
		return GetThisClassInfo();\
	}\
	static CNameObject * CreateObject();\
	static CLASS_INFO& GetThisClassInfo();\
	
	

#define GET_CLASS_INFO(ClassName) (ClassName::GetThisClassInfo())
#define GET_CLASS_INFO_BY_NAME(ClassName) (*ClassName::GetClassInfo(ClassName))

#define IMPLEMENT_CLASS_INFO(ClassName,ParentClassName) \
	CLASS_INFO	ClassName::m_##ClassNameClassInfo={#ClassName,&GET_CLASS_INFO(ParentClassName),ClassName::CreateObject};\
	CClassInfoRegister	ClassName::m_##ClassNameClassInfoRegister(#ClassName,&ClassName::m_##ClassNameClassInfo);\
	CNameObject * ClassName::CreateObject()\
	{\
		return (ParentClassName *) new ClassName();\
	}\
	CLASS_INFO& ClassName::GetThisClassInfo()\
	{\
		return m_##ClassNameClassInfo;\
	}

#define DECLARE_CLASS_INFO_STATIC(ClassName) \
public:\
	static unsigned int m_iObjectCount;\
protected:\
	static CLASS_INFO	m_##ClassNameClassInfo;\
public:\
	virtual CLASS_INFO& GetClassInfo()\
	{\
		return GetThisClassInfo();\
	}\
	static CLASS_INFO& GetThisClassInfo();\


#define IMPLEMENT_CLASS_INFO_STATIC(ClassName,ParentClassName) \
	unsigned int ClassName::m_iObjectCount = 0;\
	CLASS_INFO	ClassName::m_##ClassNameClassInfo={#ClassName,&GET_CLASS_INFO(ParentClassName),NULL};\
	CLASS_INFO& ClassName::GetThisClassInfo()\
	{\
		return m_##ClassNameClassInfo;\
	}






class CUSOFile;


class CNameObject //: public CObject
{
protected:
	string			m_Name;
	UINT			m_ID;	
	volatile long	m_UseRef;
	LPVOID			m_pParam;

	static CLASS_INFO	m_CNameObjectClassInfo;	
	static std::map<std::string,CLASS_INFO *> * m_pAllClassInfo;
	static int m_AllClassCount;
	static CClassInfoRegister m_CNameObjectClassInfoRegister;

	friend class CClassInfoRegister;
public:
	static CLASS_INFO& GetThisClassInfo();
	static CNameObject * CreateObject();
	static CLASS_INFO * GetClassInfo(LPCTSTR ClassName);
	static int GetAllClassCount();
	virtual CLASS_INFO& GetClassInfo()
	{
		return GetThisClassInfo();;
	}
	inline bool IsKindOf(const CLASS_INFO& ClassInfo)
	{
		return GetClassInfo().IsKindOf(ClassInfo);
	}
	inline bool IsKindOf(LPCTSTR Name)
	{
		return GetClassInfo().IsKindOf(Name);
	}
	inline bool IsClass(const CLASS_INFO& ClassInfo)
	{
		return GetClassInfo()==ClassInfo;
	}
	inline bool IsClass(LPCTSTR Name)
	{
		return GetClassInfo()==Name;
	}
	inline bool IsKindOfFast(const CLASS_INFO& ClassInfo)
	{
		return GetClassInfo().IsKindOfFast(ClassInfo);
	}
	inline LPCTSTR GetClassName()
	{
		return GetClassInfo().ClassName;
	}

	CNameObject()
	{
		m_ID=0;
		m_UseRef=1;		
		m_pParam=NULL;
	}

	virtual ~CNameObject()
	{

	}
	virtual bool Initialize()
	{
		return true;
	}
	virtual void Destory()
	{
	}
	void AddUseRef()
	{
		InterlockedIncrement(&m_UseRef);
	}
	void DecUseRef()
	{
		InterlockedDecrement(&m_UseRef);
	}
	long GetUseRef()
	{
		return m_UseRef;
	}
	virtual void Release()
	{
		DecUseRef();
		if(GetUseRef()<=0)
			delete this;
	}	

	void SetName(LPCTSTR Name)
	{
		m_Name=Name;
	}

	const LPCTSTR GetName()
	{
		return m_Name.c_str();
	}	

	void SetID(UINT ID)
	{m_ID=ID;}

	UINT GetID()
	{return m_ID;}	
	

	void SetParam(LPVOID pParam)
	{
		m_pParam=pParam;
	}

	LPVOID GetParam()
	{
		return m_pParam;
	}
};