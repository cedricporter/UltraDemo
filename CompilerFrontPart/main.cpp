/*
** 作者：华亮
** 说明：测试用cpp
**
*/
#include "Scanner.h"
#include "Parser.h"
//#include "VirtualMachine.h"
#include "SymbolTable.h"
#include <fstream>
#include "Imm2asm.h"
#include "ETMachine.h"

using namespace ETCompiler;


  
int main()
{
	

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

	//long* a = (long*)(machine.m_stack.m_stack + MaxStackSize - 4);
	//long* b = (long*)(machine.m_stack.m_stack + MaxStackSize - 8);

	//std::cout << *a << " " << *b;

	return 0;
}