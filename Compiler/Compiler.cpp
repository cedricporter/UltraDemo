// Compiler.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "Compiler.h"
#include "Parser.h" 
#include "ETMachine.h"
#include "SymbolTable.h"
#include "Imm2asm.h"



// ���ǵ���������һ��ʾ��
COMPILER_API int nCompiler=0;

// ���ǵ���������һ��ʾ����

// ����
COMPILER_API int compile(const wchar_t* code)
{
    using namespace ETCompiler;

    ETMachine machine;

	CParser parser;

	//IParser& parser = myParser;
	parser.LoadFromFile( "SAMPLE.hpp" );
	parser.Parse( true );

	std::cout << parser.GetImmCode();

	std::ofstream out( "code.txt" );
	out << parser.GetImmCode() << std::endl;

	Imm2asm i2a;
	std::string asmcode = i2a.TranslateToAsm( parser.GetImmCode(), parser.GetTables() );


	std::cout << asmcode << std::endl;


	machine.LoadCode( asmcode );
	machine.Reset();

	while ( machine.Step() ) ;

	std::cout << machine.GetOutputLog();
    
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� Compiler.h
CCompiler::CCompiler()
{
	return;
}
