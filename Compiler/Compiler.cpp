// Compiler.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "Compiler.h"
#include "ETController.h"

// ��ȡͨ�üĴ���
COMPILER_API unsigned long *get_reg(ETCompiler::ETController* ctrl)
{
    return ctrl->Getreg();
}

// ��ȡ���ԼĴ���
COMPILER_API unsigned long *get_dreg(ETCompiler::ETController* ctrl)
{
    return ctrl->Getdreg();
}





/************************************************************************/
/* ��������ص�������                                                    */
/************************************************************************/

// ����������
COMPILER_API ETCompiler::ETController* create_controller()
{
    return new ETCompiler::ETController;
}

// ���ٽ�����
COMPILER_API void destroy_controller(ETCompiler::ETController* ctrl)
{
    delete ctrl;
}

// ��ʼ��������
//@param code Դ����
COMPILER_API int initial_machine(ETCompiler::ETController* ctrl, const char* code, char* error_message)
{
    int ret = ctrl->Initialiaze_Machine(code, error_message);

    return ret;
}

// ����ִ��
COMPILER_API bool step(ETCompiler::ETController* ctrl)
{
    bool ret = ctrl->Step();
    if (!ret)
        MessageBox(0, "Fuck", "f", 0);

    return ret;
    //return ctrl->Step();
}

// ��ȡ��ǰ�к�
COMPILER_API int get_current_line(ETCompiler::ETController* ctrl)
{
    return ctrl->GetCurrentLine();
}


// ����
COMPILER_API int compile(const wchar_t* code, char* outResult)
{
    using namespace ETCompiler;

    ETMachine machine;

	CParser parser;

    std::string outString;

	//IParser& parser = myParser;
	parser.LoadFromFile( "SAMPLE.hpp" );
	parser.Parse( true );

    //std::cout << parser.GetImmCode();
	outString += parser.GetImmCode();

	std::ofstream out( "code.txt" );
	out << parser.GetImmCode() << std::endl;

	Imm2asm i2a;
	std::string asmcode = i2a.TranslateToAsm( parser.GetImmCode(), parser.GetTables() );


	outString += asmcode;
    outString += "\n";


	machine.LoadCode( asmcode );
	machine.Reset();

	while ( machine.Step() ) ;

	outString += machine.GetOutputLog();

    strcpy(outResult, outString.c_str());

    return 0;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� Compiler.h
CCompiler::CCompiler()
{
	return;
}
