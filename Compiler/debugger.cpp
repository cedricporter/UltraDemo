/*
** 作者：华亮
** 说明：
**
*/


#include "stdafx.h"
#include "Debugger.h"
#include "Parser.h"
#include "ETMachine.h"
#include <fstream>
#include <string>
#include "MessageDefinition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace ETCompiler
{

	//////////////////////////////////////////////////////////////////////////
	//
	// Class LineInfoDebugger
	// 
	void LineInfoDebugger::Debug( VCPU* pCPU, LPVOID param )
	{
		if ( m_currentLine != pCPU->Getdreg()[ 0 ] )
		{
			m_currentLine = pCPU->Getdreg()[ 0 ];
			m_callBack( (LPVOID)( m_currentLine ) );
		}
	}


	//////////////////////////////////////////////////////////////////////////
	//
	// Class LogDebugger
	// 
	void LogDebugger::Debug( VCPU* pCPU, LPVOID param )
	{
		if ( ! pCPU->GetLog().empty() )
		{
			m_log += pCPU->GetLog();
			pCPU->ClearLog();

			m_callBack( NULL );
		}
	}

	void LogDebugger::ClearLog()
	{ 
		m_log.clear(); 
	//	m_callBack( NULL ); 
	}


	//////////////////////////////////////////////////////////////////////////
	//
	// Class BreakPointDebugger
	// 
	void BreakPointDebugger::Debug( VCPU* pCPU, LPVOID param )
	{
		static int lastlineno = -1;
		if ( lastlineno != pCPU->Getdreg()[ 0 ] )
		{
			m_callBack( (LPVOID)( pCPU->Getdreg()[ 0 ] ) );	
			lastlineno = pCPU->Getdreg()[ 0 ];
		}
	}


}