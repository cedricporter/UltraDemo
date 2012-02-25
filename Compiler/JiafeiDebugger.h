#pragma once
#include "Debugger.h"
#include <stack>

namespace ETCompiler
{
	/*
	** ���֣�MemoryViewDebugger
	** ˵���������ڴ���Ϣ��ȡ�ĵ�����
	**
	*/
	class MemoryViewDebugger : public IDebugger
	{
		int flag;
	    std::stack < int > m_funcId;
		std::list< PropertiesInsertItem > listComp;
	public:
		void Debug( VCPU* pCPU, LPVOID param );

	};




	/*
	** ���֣�
	** ˵����
	**
	*/
	class WatchDogDebugger : public IDebugger
	{
		int flag;
	    std::stack < int > m_funcId;
		std::list< PropertiesInsertItem > listComp;
	public:
		void Debug( VCPU* pCPU, LPVOID param );

	};
}