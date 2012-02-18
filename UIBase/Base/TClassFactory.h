/*
** ���ߣ�����
** ˵���������๤��ģ��
**
*/

#ifndef CLASSFACTORY_H
#define CLASSFACTORY_H

#ifdef _UNICODE
	#define CETString std::wstring	
#else
	#define CETString std::string
#endif

#include <map>
#include <string>
#include <list>
#include "MacroDef.h"


/*
** ���֣�TFactory
** ˵�����๤����ģ��
**
*/
template < typename Interface >
class TFactory
{
public:
	typedef typename Interface* (*LP_FUNC_CREATEOBJECT)();					// ������Ķ���ĺ���ָ��
	typedef std::map< CETString, LP_FUNC_CREATEOBJECT > MAPTYPE;			// �����봴���������ָ���ӳ�������
	typedef std::pair< CETString, LP_FUNC_CREATEOBJECT > PAIRTYPE;			// ӳ���Pair

public:
	Interface* CreateObject( const CETString& classname );	// ��������������Ķ��� 
	void RegisterClass( const CETString& classname, LP_FUNC_CREATEOBJECT pFunCreateObject );	// ע���ൽ������
	static TFactory* GetInstance();									// ��ȡ����ʵ��

private:	// ֻ�ܴ�GetInstance�л�ȡʵ��������ģʽ
	TFactory() {}
	TFactory( const TFactory& other ){}
	TFactory& operator=( const TFactory& other ){} 

private:
	MAPTYPE m_ObjectCreatorMap;				// �����봴���������ָ���ӳ��
	static TFactory* m_pFactoryInstance;	// ����ʵ��
};



template < typename Interface >
TFactory< Interface >* TFactory< Interface >::m_pFactoryInstance = NULL;


// ��������������Ķ��� 
template < typename Interface >
Interface* TFactory< Interface >::CreateObject( const CETString& classname )
{
	assert( m_pFactoryInstance );

	Interface* object = NULL;
	LP_FUNC_CREATEOBJECT pFunCreate;
	if ( m_ObjectCreatorMap.find( classname ) != m_ObjectCreatorMap.end() )
	{
		pFunCreate = m_ObjectCreatorMap[ classname ];
		object = (*pFunCreate)();
	}
	return object;
}

// ע���ൽ������
template < typename Interface >
void TFactory< Interface >::RegisterClass( const CETString& classname, typename LP_FUNC_CREATEOBJECT pFunCreateObject )
{
	assert( m_pFactoryInstance );
	m_ObjectCreatorMap.insert( PAIRTYPE( classname, pFunCreateObject ) );
}

// ��ȡ����ʵ��
template < typename Interface >
TFactory< Interface >* TFactory< Interface >::GetInstance()
{
	if ( m_pFactoryInstance == NULL )
	{
		m_pFactoryInstance = new TFactory;
	}

	return m_pFactoryInstance;
}


//////////////////////////////////////////////////////////////////////////



/*
** ���֣�RegisterHelper
** ˵�����ڶ������ʱ��Э������ע�ᵽ�๤����
**
*/
template < typename Interface >
class RegisterHelper
{
public:
	RegisterHelper( const CETString& classname, typename TFactory< Interface >::LP_FUNC_CREATEOBJECT pFunCreateObject )
	{
		TFactory< Interface >* pFactory = TFactory< Interface >::GetInstance();
		pFactory->RegisterClass( classname, pFunCreateObject );
	}
};



//////////////////////////////////////////////////////////////////////////

/*
** ���֣�DECLARE_REGISTER_FACTORY
** ˵���������.h�е����ڲ���������ĺ꣬���ϣ�����ܹ�ͨ�����ִ�����
**      �����ͨ����Ӵ˺���ע���Լ����๤����
*/
#define DECLARE_REGISTER_FACTORY( classname, Interface ) \
public: \
	static Interface* Create##classname##Interface(); \
private: \
	friend void RegisterClass( const CETString& classname ); 


/*
** ���֣�IMPLEMENT_REGISTER_FACTORY
** ˵���������.cpp�������DECLARE_REGISTER_FACTORY�Ķ���
**		RegisterHelper��������ע�ᵽ������
*/
#define IMPLEMENT_REGISTER_FACTORY( classname, Interface ) \
	Interface* classname::Create##classname##Interface() { return new classname; }	\
	RegisterHelper<Interface> RegisterHelp##classname##Interface( TEXT(#classname), classname::Create##classname##Interface ); 






#endif