/*
** 作者：华亮
** 说明：
**
*/

#include "stdafx.h"
#include "ETMachine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace ETCompiler
{
	//////////////////////////////////////////////////////////////////////////
	//
	// Class VMemory
	// 
	void ETCompiler::VMemory::LoadCode( const std::string& code )
	{
		Reset();

		std::string line;
		std::string::const_iterator iter = code.begin();

		while ( iter != code.end() )
		{
			line += *iter;

			if ( *iter == '\n' )
			{
				//TranslateLine(line);
				TokenList tokList = str2Tok( line );

				Instruction ins;

				for ( auto iter = tokList.begin(); iter != tokList.end(); iter++ )
				{
					ins.m_elements.push_back( *iter );
				}

				m_instructions.push_back( ins );

				line = "";
			}

			iter++;
		}
	}

	void VMemory::Reset()
	{
		m_instructions.clear();
	}

	ETCompiler::TokenList VMemory::str2Tok( std::string &line )
	{
		std::string token;

		TokenList tokenList;
		for ( std::size_t i = 0; i < line.size(); i++ )
		{
			if ( isalnum( line[ i ] ) || line[ i ] == '[' || line[ i ] == ']' || line[ i ] == '+' || line[ i ] == '-' )
			{
				token += line[ i ];
			}
			else
			{
				if ( token != "" )
				{
					tokenList.push_back( token );
					token.clear();
				}
			}
		}

		return tokenList;
	}



	/*
	** 名字：RuntimeStack
	** 说明：
	**
	*/
	ETCompiler::uchar_pointer RuntimeStack::GetStack()
	{
		return m_stack;
	}

	RuntimeStack::RuntimeStack()
	{
		m_stack = new uchar[ MaxStackSize + 0xfff ];
		Reset();
	}

	void RuntimeStack::Reset()
	{
		memset( m_stack, 0, MaxStackSize + 0xfff );
	}

	RuntimeStack::~RuntimeStack()
	{
		delete [] m_stack;
	}



	//////////////////////////////////////////////////////////////////////////
	//
	// Class VCPU
	// 
	bool VCPU::DecodeExecute()
	{
		if ( m_bRun )
		{
			if ( eip >= m_pInstructions->size() )	Reset();

			if ( !Fetch() )  return false;

			Execute();

			Check();

			// 调试器回调
			m_debug_callback( this );

			if ( eip >= m_pInstructions->size() )	return false;
			return true;
		}
		return false;
	}

	void VCPU::Execute()
	{
		TokenList& insVector = IR->m_elements;

		static TokenList lastInst = insVector;

		std::string& first = insVector[ 0 ];

		if ( first == "mov" )
		{
			_mov(insVector);
		}
		else if ( first == "lea" )
		{
			dword target = NULL;		
			dword source = NULL;
			target |= (dword)GetReg( insVector[ 1 ] );				
			target |= (dword)GetVar( insVector[ 1 ] );

			source |= (dword)GetReg( insVector[ 2 ] );				
			source |= (dword)GetVar( insVector[ 2 ] );
			source |= (dword)GetConst( insVector[ 2 ] );

			dword_pointer t = (dword_pointer)target;
			dword_pointer s = (dword_pointer)source;

			*(dword_pointer)target = source;

			t = (dword_pointer)target;
			s = (dword_pointer)source;
		}
		else if ( first == "push" || first == "arg" )
		{
			push(insVector);
		}
		else if ( first == "pop" )
		{
			pop(insVector);
		}
		else if ( first == "add" || first == "sub" || first == "mul" || first == "div" )
		{
			_operator(insVector);
		}
		else if ( first == "jmp" )
		{
			jump(insVector);
		}
		else if ( first == "jeq" || first == "jlt" || first == "jle" || first == "jne" || first == "jgt" || first == "jge")
		{
			_conditionJmp(insVector);
		}	
		else if ( first == "call" )
		{
			_call(insVector);
		}
		else if ( first == "leave" )
		{
			leave();
		}
		else if ( first == "in" )
		{

		}
		else if ( first == "out" )
		{
			dword target = 0;

			target |= (dword)GetConst( insVector[ 1 ] );
			if ( target ) 
			{
				if ( *(dword_pointer)target == 0 )	m_log += "\n";					
				else if ( *(dword_pointer)target == 1 )	m_log += " ";
			}
			else
			{
				target = GetAddress( insVector[ 1 ] );

				char out[ 64 ];
				sprintf_s( out, "%d", *(dword_pointer)target );

				m_log += out;
			}

		}
		else if ( first == "halt" )
		{
			eip = m_pInstructions->size();	// 程序结束
			std::cout << "\nDone!!\n";
		}

		lastInst = insVector;
	}

	void VCPU::leave()
	{
		// 修正栈，恢复现场
		esp = ebp;
		_pop( ebp );
		_pop( eip );
	}

	void VCPU::_call( TokenList& insVector )
	{
		dword label = (dword)GetConst( insVector[ 1 ] );

		_push( eip );	// 保存PC

		eip = *(dword_pointer)label;
	}

	void VCPU::jump( TokenList& insVector )
	{
		dword label = (dword)GetConst( insVector[ 1 ] );

		eip = *(dword_pointer)label;
	}

	void VCPU::pop( TokenList& insVector )
	{
		dword target = GetAddress( insVector[ 1 ] );

		_pop( *(dword_pointer)target );
	}

	void VCPU::push( TokenList& insVector )
	{
		dword target = GetAddress( insVector[ 1 ] );

		_push( *(dword_pointer)target );
	}

	void VCPU::_conditionJmp( TokenList& insVector )
	{
		dword test1 = GetAddress( insVector[ 1 ] );		
		dword test2 = GetAddress( insVector[ 2 ] );
		dword label = GetAddress( insVector[ 3 ] );

		if ( insVector[ 0 ] == "jeq" && (long)*(dword_pointer)test1 == (long)*(dword_pointer)test2 ) eip = *(dword_pointer)label;
		if ( insVector[ 0 ] == "jlt" && (long)*(dword_pointer)test1 < (long)*(dword_pointer)test2 ) eip = *(dword_pointer)label;
		if ( insVector[ 0 ] == "jle" && (long)*(dword_pointer)test1 <= (long)*(dword_pointer)test2 ) eip = *(dword_pointer)label;
		if ( insVector[ 0 ] == "jne" && (long)*(dword_pointer)test1 != (long)*(dword_pointer)test2 ) eip = *(dword_pointer)label;
		if ( insVector[ 0 ] == "jgt" && (long)*(dword_pointer)test1 > (long)*(dword_pointer)test2 ) eip = *(dword_pointer)label;
		if ( insVector[ 0 ] == "jge" && (long)*(dword_pointer)test1 >= (long)*(dword_pointer)test2 ) eip = *(dword_pointer)label;
	}

	ETCompiler::dword VCPU::GetAddress( const std::string& name )
	{
		dword target = 0;
		target |= (dword)GetReg( name );				
		target |= (dword)GetVar( name );
		target |= (dword)GetConst( name );
		return target;
	}

	void VCPU::_operator( TokenList& insVector )
	{
		dword target = GetAddress( insVector[ 1 ] );		
		dword source1 = GetAddress( insVector[ 2 ] );	
		dword source2 = GetAddress( insVector[ 3 ] );	

		if ( insVector[ 0 ] == "add" ) *(dword_pointer)target = *(dword_pointer)source1 + *(dword_pointer)source2;
		else if ( insVector[ 0 ] == "sub" ) *(dword_pointer)target = *(dword_pointer)source1 - *(dword_pointer)source2;
		else if ( insVector[ 0 ] == "mul" ) *(dword_pointer)target = *(dword_pointer)source1 * *(dword_pointer)source2;
		else if ( insVector[ 0 ] == "div" ) *(dword_pointer)target = *(dword_pointer)source1 / *(dword_pointer)source2;
	}

	void VCPU::_mov( TokenList& insVector )
	{
		dword target = GetAddress( insVector[ 1 ] );		
		dword source = GetAddress( insVector[ 2 ] );	

		dword_pointer t = (dword_pointer)target;
		dword_pointer s = (dword_pointer)source;

		*(dword_pointer)target = *(dword_pointer)source;

		t = (dword_pointer)target;
		s = (dword_pointer)source;
	}

	void VCPU::_push( dword word )
	{
		esp -= STACK_OFFSET;
		*(dword*)(esp) = word;
	}

	void VCPU::_pop( dword& word )
	{
		word = *(dword*)(esp);
		esp += STACK_OFFSET;
	}

	ETCompiler::dword_pointer VCPU::GetConst( const std::string& item )
	{
		static int i = 0;
		dword_pointer pointer = NULL;
		if ( isdigit( item[ 0 ] ) )
		{
			temp[ i ] = atoi( item.c_str() );
			pointer = (dword_pointer)&temp[ i ];
			i = i > 3 ? 0 : i + 1;
		}	

		return pointer;
	}

	ETCompiler::dword_pointer VCPU::GetVar( const std::string& item )
	{
		uchar_pointer pointer = NULL;

		if ( item[ 0 ] == '[' )
		{
			if ( item[ 1 ] == 'e' )		// [ebp
			{
				pointer = GetFromEbpOffset(item, pointer);
			}
			else if ( item[ 1 ] == 'r' )
			{
				pointer = (uchar_pointer)GetReg( item.substr( 1, item.size() - 2 ) );
				pointer = (uchar_pointer)*(dword_pointer)pointer;
			}
			else if ( isdigit( item[ 1 ] ) )
			{
			}
			else if ( item[ 1 ] == '[' )	// [[ebp+X]-RX]
			{
				pointer = (uchar_pointer)GetReg( item.substr( 2, 3 ) );

				int firstRightSquare = item.find_first_of( ']' );
				int offset = atoi( item.substr( 5, firstRightSquare - 5 ).c_str() );

				// 第一次寻址
				pointer = ((uchar_pointer)*(dword_pointer)pointer) + offset;

				// 第二次寻址
				pointer = (uchar_pointer)*(dword_pointer)pointer;						

				// 取得最后的寄存器RX
				dword regTemp = GetAddress( item.substr( firstRightSquare + 1 + 1, item.size() - (firstRightSquare + 1) - 2 ) );
				dword_pointer look = (dword_pointer)regTemp;

				char c = item[ firstRightSquare + 1 ];

				// 加上RX的值
				if ( c == '-' )
				{
					pointer = pointer - *(dword_pointer)regTemp;
				}
				else if ( c == '+')
				{
					pointer = pointer + *(dword_pointer)regTemp;
				}
				else if ( c == ']' )
				{
					c = c;
				}

			}
		}	

		dword_pointer pt = (dword_pointer)pointer;

		return (dword_pointer)pointer;
	}

	ETCompiler::uchar_pointer VCPU::GetFromEbpOffset( const std::string &item, uchar_pointer pointer )
	{
		std::string reg = item.substr( 1, 3 );

		char num[ 10 ];
		int j = 0;
		std::size_t i;
		for ( i = 5; i < item.size() - 1 && isdigit( item[ i ] ); i++ )
		{
			num[ j++ ] = item[ i ];
		}
		num[ j ] = '\0';

		char op = item[ i ];
		i++;
		std::string rReg;
		while ( i < item.size() - 1 )
		{
			rReg += item[ i++ ];
		}

		dword target = ebp;
		if ( reg == "esp" )
		{
			target = esp;
		}
		else if ( reg == "eip" )
		{
			target = eip;
		}

		switch ( item[ 4 ] )
		{
		case '+':
			pointer = (uchar_pointer)(target + atoi( num ));
			break;
		case '-':
			pointer = (uchar_pointer)(target - atoi( num ));
			break;
		}

		if ( rReg.size() > 0 )
		{
			target = (dword)GetReg( rReg );
			if ( target )
			{
				if ( op == '+' )		pointer += *(dword_pointer)target;  
				else if ( op == '-' )	pointer -= *(dword_pointer)target;  

				dword_pointer p = (dword_pointer)pointer;
			}
		}	

		if ( (dword)pointer < (dword)m_stack || (dword)pointer > (dword)m_endOfStack )
		{
			MessageBox( NULL, "Read Error Memory!", "Error", MB_OK );
		}
		return pointer;
	}

	ETCompiler::dword_pointer VCPU::GetReg( const std::string& item )
	{
		dword_pointer pointer = NULL;
		if ( item[ 0 ] == 'r' ) // 通用寄存器
		{
			int r = combinationToInt( item );
			pointer = (dword_pointer)&reg[ r ];
		}	
		else if ( item[ 0 ] == 'd' )	// 调试寄存器
		{
			int r = combinationToInt( item );
			pointer = (dword_pointer)&dreg[ r ];
		}
		else if ( item == "ebp" )		// 栈帧指针
		{
			pointer = (dword_pointer)&ebp;
		}
		else if ( item == "esp" )		// 栈指针
		{
			pointer = (dword_pointer)&esp;
		}
		else if ( item == "eip" )		// 指令指针，充当PC
		{
			pointer = (dword_pointer)&eip;
		}

		return pointer;
	}

	int VCPU::combinationToInt( const std::string& num )
	{
		const char* numstr = num.c_str() + 1;
		return atoi( numstr );
	}

	void VCPU::Check()
	{
		// check
		if ( (dword)esp < (dword)m_stack || (dword)esp > (dword)&m_stack[ MaxStackSize ] )	// 栈溢出
		{	
			m_bRun = false;
		}
	}

	bool VCPU::Fetch()
	{
		if ( eip < m_pInstructions->size() )
		{
			IR = const_cast<Instruction*>(&(*m_pInstructions)[ eip ]);
			eip++;
			return true;
		}
		return false;
	}

	void VCPU::Reset()
	{
		m_bRun = true;

		dreg[ 0 ] = dreg[ 1 ] = 0;
		memset( dreg, 0, sizeof( dword ) * m_Size );

		ebp;
		esp;
		eip = 0;
	}

	VCPU::VCPU() : eip( 0 ), ebp( 0 ), esp( 0 ), IR( NULL ), m_bRun( false )
	{
		reg	= new dword[ m_Size ];
		freg = new double[ m_Size ];
		dreg = new dword[ m_Size ];

		dreg[ 0 ] = 0;
	}

	void VCPU::LoadStack( uchar_pointer pStack )
	{
		m_stack = pStack;
		ebp = esp = (dword)( m_stack + (MaxStackSize & 0xfffffff8) );		
		m_endOfStack = (uchar_pointer)ebp;
	}

	void VCPU::LoadInstructions( const InstructionVector* instuct )
	{
		m_pInstructions = instuct;
	}

	void VCPU::SetDebugCallback( const DebugCallBackType& cb )
	{
		m_debug_callback = cb;
	}

	VCPU::~VCPU()
	{
		delete [] reg;
		delete [] freg;
		delete [] dreg;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	// Class ETMachine
	// 
	ETMachine::ETMachine() : m_curLine( 0 )
	{
		//m_pDebugger = new TinyDebugger;							// 新建调试器，这里不好，待修改
		m_cpu.LoadStack( m_stack.GetStack() );					// 关联CPU和栈
		m_cpu.LoadInstructions( &m_memory.GetInstructions() );	// 关联CPU和内存的指令区域

		DebugCallBackType cb = [&]( VCPU* pCPU )				// CPU中的回调函数，每执行一条指令会调用一次
		{
			for ( auto iter = m_debuggers.begin(); iter != m_debuggers.end(); iter++ )
			{
				(*iter)->Debug( pCPU, NULL );
			}	
		};

		m_cpu.SetDebugCallback( cb );							// 设置CPU的回调函数
	}

	void ETMachine::LoadCode( const std::string& code )
	{
		m_memory.LoadCode( code );
	}

	bool ETMachine::Step()
	{
		bool bState = false;
		while ( m_curLine == m_cpu.Getdreg()[ 0 ] )
		{
			if ( ! (bState = m_cpu.DecodeExecute()) ) break;
		}

		m_curLine = m_cpu.Getdreg()[ 0 ];

		return bState;
	}

	void ETMachine::Reset()
	{
		m_curLine = 0;

		m_cpu.LoadStack( m_stack.GetStack() );					// 关联CPU和栈

		m_cpu.Reset();

		m_stack.Reset();
	}

	int ETMachine::GetCurLine()
	{
		return m_cpu.Getdreg()[ 0 ];
	}

	void ETMachine::AddDebugger( IDebugger* pDebugger )
	{
		m_debuggers.push_back( pDebugger );
	}




}
