/*
** ���ߣ�����
**
**
*/

#ifndef _MACRODEF_H
#define _MACRODEF_H

#include "Def.h"
#include "TClassFactory.h"


	//RegisterHelper<CETObject> RegisterHelp##classname##Interface( "et", NULL );		// ���Լ��������ʹ�����������Ķ���ĺ���ָ��
	//( #classname, classname::Create##classname##Interface );							// ͨ��RegisterHelper�Ĺ��캯����ӵ�������






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