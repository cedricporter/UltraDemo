/*
** ���ߣ�����
** ˵�������׵��Զ������д��Ľ���
**
*/

#include "stdafx.h"
#include "StateMachine.h"

namespace ETCompiler
{



/*
** ���֣�StateMachine::Run()
** ˵���������Զ���
**
*/
int StateMachine::Run()
{
	CALLBACK_SCANNER* handler;
	do 
	{
		handler = NULL; 
		char peek = (*m_pLookAheadCB)(); // ��ȡ��ǰ�ַ�
		if ( m_callbackMap.find( peek ) != m_callbackMap.end() ) // ��ӳ���������Ƿ��д�����
		{
			handler = m_callbackMap[ (*m_pLookAheadCB)() ];
		}
		else
		{
			handler = m_pOtherCB;
		}
	} while ( ! (*handler)() );

	return 0;
}

}