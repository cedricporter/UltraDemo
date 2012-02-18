/*
** 作者：华亮
**
**
*/

#ifndef _MACRODEF_H
#define _MACRODEF_H

#include "Def.h"
#include "TClassFactory.h"


	//RegisterHelper<CETObject> RegisterHelp##classname##Interface( "et", NULL );		// 将自己的类名和创建这个类名的对象的函数指针
	//( #classname, classname::Create##classname##Interface );							// 通过RegisterHelper的构造函数添加到工厂中






//#define DECLARE_FACTORYOBJECT( className ) \
//public: \
//	static CETObject* CreateObject() \
//	{ return new className(); } \
//	static bool  RegisterObject( CFString utName ) \
//	{ \
//		className *pObj = (className*)CreateObject(); \
//		ClassFactory *pFactory = ClassFactory::GetInstance(); \
//		pFactory->RegisterObject( utName, pObj ); \
//		return true; \
//	} \
//private: \
//	static CFString m_class##className;
//
//#define REGISTER_FACTORYOBJECT( className ) \
//	bool b##className = className::RegisterObject( #className ); \
//	CFString className::m_class##className = #className;





#endif