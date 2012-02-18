/*
** 作者：华亮
** 说明：
**
*/

#pragma once

#include "SymbolTable.h"
#include "Debugger.h"
#include <vector>
#include <iostream>

namespace ETCompiler
{
	typedef std::vector< std::string >	TokenList;


	const static std::size_t MaxStackSize = /*0x100000*/100000;	// 1MB


	/*
	** 名字：RuntimeStack
	** 说明：运行时栈
	**
	*/
	class RuntimeStack
	{
	public:
		uchar_pointer	m_stack;

		std::stack< Table* > m_retStack;

		uchar_pointer GetStack();

		RuntimeStack();

		void Reset();

		~RuntimeStack();
	};
	

	/*
	** 名字：Instruction
	** 说明：一条指令
	**
	*/
	struct Instruction
	{
		typedef TokenList InstructionElemVector;

		InstructionElemVector	m_elements;
	};

	typedef std::vector< Instruction > InstructionVector;


	/*
	** 名字：VMemory
	** 说明：模拟的内存，里面存放指令
	**
	*/
	class VMemory
	{
	public:
		InstructionVector	m_instructions;		// 指令向量

		// 将一行中间代码拆分为一个个Token
		TokenList str2Tok( std::string &line );

		// 加载指令
		void LoadCode( const std::string& code );		

		void Reset(); 

		const InstructionVector& GetInstructions() const
		{
			return m_instructions;
		}
	};

	class VCPU;

	typedef std::function< void( VCPU* ) >	DebugCallBackType;

	/*
	** 名字：VCPU
	** 说明：虚拟CPU
	**
	*/
	class VCPU
	{
	private:
		dword	ebp;		// 基指针
		dword	esp;		// 栈指针，定义为指向栈顶元素的首地址
		dword	eip;		// 指令指针

		dword*	reg;		// 通用寄存器
		double* freg;		// 浮点寄存器
		dword*	dreg;		// 调试寄存器		

		dword	temp[ 5 ];	// 临时存储区域	

		Instruction*	IR;	// 指令寄存器

		DebugCallBackType	m_debug_callback;	// 调试回调函数

		uchar_pointer				m_stack;			// 指向栈的指针
		uchar_pointer				m_endOfStack;		// 栈终点，用于检查错误
		const InstructionVector*	m_pInstructions;	// 指令内存

		static const dword STACK_OFFSET = sizeof( dword );	// 机器字长 
		static const dword m_Size = 1024;

		friend class Debugger;
		std::string		m_log;
				
		bool m_bRun;

	public:

		VCPU();

		~VCPU();

		dword_pointer Getebp() { return (dword_pointer)ebp; }	// 获取寄存器ebp里面的值
		dword_pointer Getesp() { return (dword_pointer)esp; }	// 获取寄存器esp里面的值
		dword Geteip() { return eip; }							// 获取eip

		dword_pointer Getdreg() { return dreg; }				// 获取调试寄存器
		dword_pointer Getreg() { return reg; }					// 获取通用寄存器

		const std::string& GetLog() { return m_log; }			// 获取输出日志
		void ClearLog() { m_log.clear(); }						// 清空输出日志

		Instruction* GetIR() { return IR; }						// 获取指令寄存器中的指令

		void SetDebugCallback( const DebugCallBackType& cb );	// 设置调试回调，这个在每执行一条指令会调用一次
		
		void LoadStack( uchar_pointer pStack );					// 获取栈指针
		
		void LoadInstructions( const InstructionVector* instuct );	// 装载指令
		
		bool DecodeExecute();		// 译码&执行一条指令

		bool Fetch();				// 取指令

		void Check();				// 每执行一条指令后的安全检查
		
		void Reset();				// 重置虚拟机

	private:
		void Execute();				// 执行一条指令
		
		/************************************************************************/
		/* 指令处理函数                                                          */
		/************************************************************************/
		void leave();

		void _call( TokenList& insVector );

		void jump( TokenList& insVector );

		void pop( TokenList& insVector );

		void push( TokenList& insVector );

		void _conditionJmp( TokenList& insVector );

		dword GetAddress( const std::string& name );

		void _operator( TokenList& insVector );
		
		void _mov( TokenList& insVector );

		void _push( dword word );

		void _pop( dword& word );	

		dword_pointer GetConst( const std::string& item );

		dword_pointer GetVar( const std::string& item );

		uchar_pointer GetFromEbpOffset( const std::string &item, uchar_pointer pointer );

		dword_pointer GetReg( const std::string& item );
		
		// 将一个字符串的第二位以及后面的数字转为数字类型，第一位通常用于标记
		int combinationToInt( const std::string& num );
	};

	
	/*
	** 名字：ETMachine
	** 说明：虚拟机
	**
	*/
	class ETMachine
	{
	private:
		int m_curLine;
	public:

		VCPU					m_cpu;
		VMemory					m_memory;
		RuntimeStack			m_stack;
		//IDebugger*		m_pDebugger;
		std::list< IDebugger* >	m_debuggers;
		DebugCallBackType		m_debuggerCallback;

	public:
		ETMachine();

		~ETMachine()
		{
			//SAFE_DELETE( m_pDebugger );
		}

		// 装载汇编代码
		void LoadCode( const std::string& code );

		// 单步执行
		bool Step();

		// 重置虚拟机
		void Reset();

		// 获取执行到的行数
		int GetCurLine();

		// 添加调试器
		void AddDebugger( IDebugger* pDebugger );

		// 获取数据结果
		const std::string& GetOutputLog()
		{
			return m_cpu.GetLog();
		}
	};



}