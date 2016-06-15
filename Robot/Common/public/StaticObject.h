#pragma once

template <class T>
class CStaticObject
{
public:		
	static T * GetInstance()
	{
		//if( m_pInstance == NULL )
		//	m_pInstance = new T;
		return &m_Instance;
	}
protected:
	static T m_Instance;
};
template <class T>
T  CStaticObject<T>::m_Instance;