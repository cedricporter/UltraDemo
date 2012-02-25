// Compiler.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "Compiler.h"
#include "Parser.h" 
#include "ETMachine.h"
#include "SymbolTable.h"
#include "Imm2asm.h"
#include <string>



// ���ǵ���������һ��ʾ��
COMPILER_API int nCompiler=0;

// ���ǵ���������һ��ʾ����

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
