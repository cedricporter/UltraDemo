#include "ETController.h"

ETCompiler::ETController::ETController()
{
    m_pParser = new CParser;

    /************************************************************************/
    /* ע�������                                                            */
    /************************************************************************/

    // ע�ắ��
    auto regist = [&]( IDebugger* debug, std::function< LPVOID(LPVOID) > pfunc )
    {
        debug->SetCallBack( pfunc );				// Ϊ���������ûص�����
        m_machine.AddDebugger( debug );			// ��������ע�ᵽET������
    };


	//// �����ڴ���Ϣ��ʾ
	//pMemViewDebugger = new MemoryViewDebugger;
	//auto func4 = [&]( LPVOID param )->LPVOID
	//{
	//	switch ( (int)param )
	//	{
	//	case 0:
	//		return m_pParser;	// �����﷨��
	//	case 1:
	//		return &m_pMemSpy;	// �����ڴ��ش���
	//	}

	//	return NULL;
	//};
	//regist( pMemViewDebugger, func4 );
}

int ETCompiler::ETController::Initialiaze_Machine( const char* code )
{
    m_pParser->Reset();
    m_pParser->LoadFromString(code);
    m_pParser->Parse(true);
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