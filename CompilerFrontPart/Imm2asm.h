/*
** 作者：华亮
** 说明：
**
*/

/*
指令列表：
add souce, lhs, rhs
call label
div
in
jeq
jge
jgt
jle
jlt lhs, rhs, label
jmp
jne
lea
leave
mov
mul
out
st a, reg
sub
*/

#pragma once
#include <string>
#include <vector>
#include "Def.h"
#include "SymbolTable.h"

namespace ETCompiler
{
	typedef std::vector< std::string >	TokenList;
	
	/*
	** 名字：Imm2asm
	** 说明：将中间代码转换为汇编代码，需要符号表
	**		扫描3遍：	
	**				第一遍：展开代码
	**				第二遍：计算Label
	**				第三遍：计算偏移，翻译为汇编
	**
	*/
	class Imm2asm
	{
		typedef std::map< std::string, int > StrIntMap;
		bool				m_bNewStmt;		// 标记位，用于标记它是否是新语句以重新分配寄存器
		Environment*		m_pTables;		// 符号表
		int					m_label;		// 最后一个Label
		int					m_lineno;		// 当前汇编代码的行号，从0开始
		int					m_regBaseNum;	// 寄存器起始基数，用于抵消临时变量后边的数字
		std::string			m_asmcode;		// 转换成的汇编代码
		std::vector< int >	m_jumpMap;		// Label和实际代码行号的映射
		StrIntMap			m_entryMap;
		Table*				m_pCurTable;
		static const int SizeofJumpMap = 500;

	public:
		Imm2asm();
		// 将中间代码转换为ET机器的汇编代码
		std::string TranslateToAsm( const std::string& imm, const Environment& tables );
		std::string ExpandCode( const std::string& imm );

	private:
		void TranslateLine( std::string& line );
		// 将一行中间代码拆分为一个个Token
		TokenList str2Tok( std::string &line );
		// 将一个字符串的第二位以及后面的数字转为数字类型，第一位通常用于标记
		int combinationToInt( const std::string& num );
		// 为临时变量分配寄存器
		std::string temp2reg( const std::string& temp );	
		// 计算Label实际位置
		void LocateLabel( const std::string& imm );
		// 翻译一行
		void _translateLine( TokenList& tokenList );
		std::string _if( TokenList& tokenList );
		std::string _iffalse( TokenList& tokenList );
		std::string _goto( TokenList& tokenList );
		// 生成赋值语句
		std::string GenAssign( TokenList& tokenList );
		// 设置寄存器基数，为分配寄存器提供必要条件
		void SetRegBaseNum( TokenList& tokenList );
		// 添加code到汇编代码
		void AddToASM( const std::string& code );
	};

	

	


}