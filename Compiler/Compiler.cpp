// Compiler.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Compiler.h"
#include "Parser.h" 
#include "ETMachine.h"
#include "SymbolTable.h"
#include "Imm2asm.h"



// 这是导出变量的一个示例
COMPILER_API int nCompiler=0;

// 这是导出函数的一个示例。

// 编译
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

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 Compiler.h
CCompiler::CCompiler()
{
	return;
}
