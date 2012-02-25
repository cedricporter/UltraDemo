/*
** ���ߣ�����
** ˵�����Զ���
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
		typedef std::hash_map< char, CALLBACK_SCANNER* >	MAPTYPE;	// �ù�ϣ����ӳ��ص�����

	public:
		StateMachine() : m_pErrorCB( NULL ), m_pOtherCB( NULL ), m_pLookAheadCB( NULL ) {}

		int Run();

		int AddHandler( char key, CALLBACK_SCANNER* functor )	// ��ӳ��������Handler
		{ m_callbackMap[ key ] = functor; return 0; }

		int AddOtherHandler( CALLBACK_SCANNER* functor )		// �������������
		{ m_pOtherCB = functor; return 0; }

		int AddLookAheadCB( CALLBACK_SCANNER* functor )			// ��Ӷ�ȡ��ǰ�ַ��ĺ���
		{ m_pLookAheadCB = functor; return 0; }

		int AddErrorCB( CALLBACK_SCANNER* functor )
		{ m_pErrorCB = functor; return 0; }

	private:
		CALLBACK_SCANNER*	m_pLookAheadCB;
		CALLBACK_SCANNER*	m_pOtherCB;			// �������������Map���Ҳ���ӳ��ʱ����
		CALLBACK_SCANNER*	m_pErrorCB;			// �쳣���������Map���Ҳ���ӳ��ʱ����
		MAPTYPE				m_callbackMap;		// �ص�����ӳ���
	};


}