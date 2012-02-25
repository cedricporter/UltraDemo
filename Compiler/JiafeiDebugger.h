#pragma once
#include "Debugger.h"
#include <stack>

namespace ETCompiler
{
	/*
	** 名字：MemoryViewDebugger
	** 说明：负责内存信息获取的调试器
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
	** 名字：
	** 说明：
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