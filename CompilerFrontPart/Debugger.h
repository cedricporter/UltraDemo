/*
** 作者：华亮
** 说明：
**
*/

#pragma once
#include "def.h"


namespace ETCompiler
{
	typedef std::function< LPVOID( LPVOID param ) >	IDebuggerCallBackType;

	class VCPU;


	/*
	** 名字：IDebugger
	** 说明：调试器接口
	**
	*/
	class IDebugger
	{
	protected:
		IDebuggerCallBackType	m_callBack;

	public:
		virtual ~IDebugger() {}
		virtual void Debug( VCPU* pCPU, LPVOID param ) = 0;
		virtual void SetCallBack( const IDebuggerCallBackType& cb ) 
		{
			m_callBack = cb;
		}
	};
	

	/*
	** 名字：LineInfoDebugger
	** 说明：关注行信息的调试器
	**
	*/
	class LineInfoDebugger : public IDebugger
	{
	private:
		int m_currentLine;

	public:
		LineInfoDebugger() : m_currentLine( 0 ) {}

		void Debug( VCPU* pCPU, LPVOID param );

		int GetCurLine() { return m_currentLine; }
	};


	/*
	** 名字：LogDebugger
	** 说明：关注输出信息的调试器
	**
	*/
	class LogDebugger : public IDebugger
	{
	private:
		std::string				m_log;

	public:
		void Debug( VCPU* pCPU, LPVOID param );
			
		void ClearLog();

		const std::string& GetLog() { return m_log; }
	};



	/*
	** 名字：BreakPointDebugger
	** 说明：处理断点信息的调试器
	**
	*/
	class BreakPointDebugger : public IDebugger
	{
	public:
		void Debug( VCPU* pCPU, LPVOID param );

	};




}
