#include "def.h"
#include "ETController.h"
#include "JiafeiDebugger.h"

ETCompiler::ETController::ETController()
{
    m_pParser = new CParser;

    /************************************************************************/
    /* 注册调试器                                                            */
    /************************************************************************/

    // 注册函数
    auto regist = [&]( IDebugger* debug, std::function< LPVOID(LPVOID) > pfunc )
    {
        debug->SetCallBack( pfunc );				// 为调试器设置回调函数
        m_machine.AddDebugger( debug );			// 将调试器注册到ET机器中
    };


	// 负责内存信息显示
	m_memoryWatcher = new MemoryViewDebugger;
	auto func4 = [&]( LPVOID param )->LPVOID
	{
		switch ( (int)param )
		{
		case 0:
			return m_pParser;	// 返回语法器
		case 1:
			return this;	    // 返回控制器，用于内存变量保存下来给C#使用
		}

		return NULL;
	};
	regist( m_memoryWatcher, func4 );
}

int ETCompiler::ETController::Initialiaze_Machine( const char* code, char* error_message )
{
    m_pParser->Reset();
    m_pParser->LoadFromString(code);

    try
    {
        m_pParser->Parse(true);
    }
    catch (char* e)
    {
        strcpy(error_message, e);
        return m_pParser->GetCurrentLine();
    }

    m_machine.Reset();

    Imm2asm i2a;
    std::string asm_code; 
    asm_code = i2a.TranslateToAsm(m_pParser->GetImmCode(), m_pParser->GetTables());

    //MessageBox(0, asm_code.c_str(), "", 0);
    m_machine.LoadCode(asm_code);
    m_machine.Reset();

    return 0;
}

bool ETCompiler::ETController::Step()
{
    return m_machine.Step();
}

unsigned long * ETCompiler::ETController::Getdreg()
{
    return m_machine.m_cpu.Getdreg();
}

unsigned long * ETCompiler::ETController::Getreg()
{
    return m_machine.m_cpu.Getreg();
}

int ETCompiler::ETController::GetCurrentLine()
{
    return m_machine.GetCurLine();
}

ETCompiler::WatchPropertiesListType& ETCompiler::ETController::GetMemoryList()
{
    return m_memoryList;
}

ETCompiler::PropertiesInsertItem* ETCompiler::ETController::GetFirstItem()
{
    currentIter = m_memoryList.begin();
    if (currentIter == m_memoryList.end())
        return NULL;
    return &*currentIter;
}

ETCompiler::PropertiesInsertItem* ETCompiler::ETController::GetNextItem()
{
    ++currentIter;
    if (currentIter != m_memoryList.end())
    {
        return &*currentIter;
    }
    return NULL;
}

const char* ETCompiler::ETController::GetOutput()
{
    return m_machine.GetOutputLog().c_str();
}
