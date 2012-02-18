/*
** 作者：华亮
** 说明：抽象类工厂模板
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
** 名字：TFactory
** 说明：类工厂的模板
**
*/
template < typename Interface >
class TFactory
{
public:
	typedef typename Interface* (*LP_FUNC_CREATEOBJECT)();					// 创建类的对象的函数指针
	typedef std::map< CETString, LP_FUNC_CREATEOBJECT > MAPTYPE;			// 类名与创建类对象函数指针的映射的类型
	typedef std::pair< CETString, LP_FUNC_CREATEOBJECT > PAIRTYPE;			// 映射的Pair

public:
	Interface* CreateObject( const CETString& classname );	// 根据类名创建类的对象 
	void RegisterClass( const CETString& classname, LP_FUNC_CREATEOBJECT pFunCreateObject );	// 注册类到工厂中
	static TFactory* GetInstance();									// 获取工厂实例

private:	// 只能从GetInstance中获取实例，单件模式
	TFactory() {}
	TFactory( const TFactory& other ){}
	TFactory& operator=( const TFactory& other ){} 

private:
	MAPTYPE m_ObjectCreatorMap;				// 类名与创建类对象函数指针的映射
	static TFactory* m_pFactoryInstance;	// 工厂实例
};



template < typename Interface >
TFactory< Interface >* TFactory< Interface >::m_pFactoryInstance = NULL;


// 根据类名创建类的对象 
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

// 注册类到工厂中
template < typename Interface >
void TFactory< Interface >::RegisterClass( const CETString& classname, typename LP_FUNC_CREATEOBJECT pFunCreateObject )
{
	assert( m_pFactoryInstance );
	m_ObjectCreatorMap.insert( PAIRTYPE( classname, pFunCreateObject ) );
}

// 获取工厂实例
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
** 名字：RegisterHelper
** 说明：在定义类的时候协助将类注册到类工厂中
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
** 名字：DECLARE_REGISTER_FACTORY
** 说明：添加在.h中的类内部帮助定义的宏，如果希望类能够通过名字创建，
**      则可以通过添加此宏来注册自己到类工厂。
*/
#define DECLARE_REGISTER_FACTORY( classname, Interface ) \
public: \
	static Interface* Create##classname##Interface(); \
private: \
	friend void RegisterClass( const CETString& classname ); 


/*
** 名字：IMPLEMENT_REGISTER_FACTORY
** 说明：添加在.cpp中来完成DECLARE_REGISTER_FACTORY的定义
**		RegisterHelper帮助将类注册到工厂里
*/
#define IMPLEMENT_REGISTER_FACTORY( classname, Interface ) \
	Interface* classname::Create##classname##Interface() { return new classname; }	\
	RegisterHelper<Interface> RegisterHelp##classname##Interface( TEXT(#classname), classname::Create##classname##Interface ); 






#endif