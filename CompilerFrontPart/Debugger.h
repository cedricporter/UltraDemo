/*
** ���ߣ�����
** ˵����
**
*/

#pragma once
#include "def.h"


namespace ETCompiler
{
	typedef std::function< LPVOID( LPVOID param ) >	IDebuggerCallBackType;

	class VCPU;


	/*
	** ���֣�IDebugger
	** ˵�����������ӿ�
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
	** ���֣�LineInfoDebugger
	** ˵������ע����Ϣ�ĵ�����
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
	** ���֣�LogDebugger
	** ˵������ע�����Ϣ�ĵ�����
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
	** ���֣�BreakPointDebugger
	** ˵��������ϵ���Ϣ�ĵ�����
	**
	*/
	class BreakPointDebugger : public IDebugger
	{
	public:
		void Debug( VCPU* pCPU, LPVOID param );

	};




}
