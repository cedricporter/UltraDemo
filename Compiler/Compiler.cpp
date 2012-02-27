// Compiler.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Compiler.h"
#include "ETController.h"

// 获取通用寄存器
COMPILER_API unsigned long *get_reg(ETCompiler::ETController* ctrl)
{
    return ctrl->Getreg();
}

// 获取调试寄存器
COMPILER_API unsigned long *get_dreg(ETCompiler::ETController* ctrl)
{
    return ctrl->Getdreg();
}

// 返回的是变量的地址，如果是指针的话记得要以二级指针来解释
COMPILER_API ETCompiler::PropertiesInsertItem* get_first_item(ETCompiler::ETController* ctrl)
{
    return ctrl->GetFirstItem();
}

COMPILER_API ETCompiler::PropertiesInsertItem* get_next_item(ETCompiler::ETController* ctrl)
{
    return ctrl->GetNextItem();
}


/************************************************************************/
/* 解释器相关导出函数                                                    */
/************************************************************************/

// 创建解释器
COMPILER_API ETCompiler::ETController* create_controller()
{
    return new ETCompiler::ETController;
}

// 销毁解释器
COMPILER_API void destroy_controller(ETCompiler::ETController* ctrl)
{
    delete ctrl;
}

// 初始化解释器
//@param code 源代码
COMPILER_API int initial_machine(ETCompiler::ETController* ctrl, const char* code, char* error_message)
{
    int ret = ctrl->Initialiaze_Machine(code, error_message);

    return ret;
}

// 单步执行
COMPILER_API int step(ETCompiler::ETController* ctrl)
{
    int ret = ctrl->Step();
  /*  if (!ret)
        MessageBox(0, "Fuck", "f", 0);*/

    return ret;
    //return ctrl->Step();
}

// 获取当前行号
COMPILER_API int get_current_line(ETCompiler::ETController* ctrl)
{
    return ctrl->GetCurrentLine();
}

COMPILER_API const int get_output(ETCompiler::ETController* ctrl, char* output)
{
    strcpy(output, ctrl->GetOutput());
    return 0;
}


// 编译
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

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 Compiler.h
CCompiler::CCompiler()
{
	return;
}
