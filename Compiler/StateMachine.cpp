/*
** 作者：华亮
** 说明：简易的自动机，有待改进。
**
*/

#include "stdafx.h"
#include "StateMachine.h"

namespace ETCompiler
{



/*
** 名字：StateMachine::Run()
** 说明：启动自动机
**
*/
int StateMachine::Run()
{
	CALLBACK_SCANNER* handler;
	do 
	{
		handler = NULL; 
		char peek = (*m_pLookAheadCB)(); // 读取当前字符
		if ( m_callbackMap.find( peek ) != m_callbackMap.end() ) // 在映射表里查找是否有处理函数
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