/*
** 作者：华亮
** 说明：自动机
**
*/

#pragma once

#include "def.h"
#include <map>
#include <hash_map>

namespace ETCompiler
{
	
	class TinyScanner;

	class StateMachine
	{
	public:
		typedef std::function< int() >						CALLBACK_SCANNER;
		//typedef std::map< char, CALLBACK_SCANNER* >			MAPTYPE;
		typedef std::hash_map< char, CALLBACK_SCANNER* >	MAPTYPE;	// 用哈希表来映射回调函数

	public:
		StateMachine() : m_pErrorCB( NULL ), m_pOtherCB( NULL ), m_pLookAheadCB( NULL ) {}

		int Run();

		int AddHandler( char key, CALLBACK_SCANNER* functor )	// 在映射表中添加Handler
		{ m_callbackMap[ key ] = functor; return 0; }

		int AddOtherHandler( CALLBACK_SCANNER* functor )		// 添加其他处理函数
		{ m_pOtherCB = functor; return 0; }

		int AddLookAheadCB( CALLBACK_SCANNER* functor )			// 添加读取当前字符的函数
		{ m_pLookAheadCB = functor; return 0; }

		int AddErrorCB( CALLBACK_SCANNER* functor )
		{ m_pErrorCB = functor; return 0; }

	private:
		CALLBACK_SCANNER*	m_pLookAheadCB;
		CALLBACK_SCANNER*	m_pOtherCB;			// 其他情况处理，在Map中找不到映射时调用
		CALLBACK_SCANNER*	m_pErrorCB;			// 异常情况处理，在Map中找不到映射时调用
		MAPTYPE				m_callbackMap;		// 回调函数映射表
	};


}