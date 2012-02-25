#include "stdafx.h"
#include "Imm2asm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace ETCompiler
{
	Imm2asm::Imm2asm() : m_bNewStmt( true ), m_regBaseNum( 0 ), m_label( -1 ), m_lineno( 0 ), m_pTables( NULL ), m_pCurTable( NULL )
	{
		m_jumpMap.resize( SizeofJumpMap, 0 );
	}

	void Imm2asm::TranslateLine( std::string& line )
	{
		TokenList tokenList = str2Tok(line);

		if ( tokenList.size() > 0 )
		{
			_translateLine(tokenList);
		}
	}

	std::string Imm2asm::ExpandCode( const std::string& imm )
	{
		std::string buffer = imm;
		std::string line;

		std::string loader 
			= 
			"entry ETMachineEntry\n"
			"call main\n"
			"halt\n";

		buffer.insert( 0, loader );

		std::size_t iter = 0;
		while ( iter < buffer.size() )
		{
			if ( buffer[ iter ] != '\n' ) line += buffer[ iter ];

			if ( buffer[ iter ] == '\n' )
			{
				TokenList tokList = str2Tok(line);
				if ( line != "" && tokList[ 0 ] == "entry" )
				{
					std::string insert 
						= 
						"push ebp\n"
						"mov ebp, esp\n"
						"sub esp, esp, ";

					int esp_offset = 0;

					Table* pTable = m_pTables->GetTable( tokList[ 1 ] );
					if ( pTable )
					{
						esp_offset = pTable->GetTableSize() * 2;
					}
					insert += itoa( esp_offset ) + '\n';

					buffer.insert( iter + 1, insert );
					//break;
				}

				line = "";
			}

			iter++;
		}

		return buffer;
	}

	ETCompiler::TokenList Imm2asm::str2Tok( std::string &line )
	{
		std::string token;

		TokenList tokenList;
		for ( std::size_t i = 0; i < line.size(); i++ )
		{
			if ( isalnum( line[ i ] ) )
			{
				token += line[ i ];
			}
			else
			{
				switch ( line[ i ] )
				{
				case '+':
				case '-':
				case '*':
				case '&':
				case '/':
				case '=':
				case '>':
				case '<':
				case '!':
				case '#':
				case '.':
					token += line[ i ];
					break;
				case '[':
					while ( line[ i ] != ']')	token += line[ i++ ];
					token += line[ i ];
					break;
				default:
					if ( token != "" )
					{
						tokenList.push_back( token );
						token.clear();
					}
				}
			}
		}

		if ( token != "" )
		{
			tokenList.push_back( token );
			token.clear();
		}

		return tokenList;
	}


	std::string Imm2asm::temp2reg( const std::string& temp )
	{
		char str[ 32 ];
		if ( temp.size() > 1 && temp[ 0 ] == '#' && temp[ 1 ] == 't' && isdigit( temp[ 2 ] ) )	// 临时变量
		{
			int num = combinationToInt( temp.substr( 1 ) );

			sprintf_s( str, "r%d", num - m_regBaseNum );

			return str;
		}
		else if ( temp[ 0 ] == 'r' && isdigit( temp[ 1 ] ) )
		{
			return temp;
		}
		else if ( isalpha( temp[ 0 ] ) ) // 变量
		{
			Symbol* pSym = NULL;
			std::string reg;
			std::string::size_type iter;
			int offset = 0;
			if ( ( iter = temp.find( '[' ) ) != std::string::npos )	// 数组访问符
			{
				std::string var = temp.substr( 0, iter );
				std::string index = temp.substr( iter + 1, temp.size() - iter - 2 );
				reg = temp2reg( index );
				pSym = m_pCurTable->GetSymbol( var );
				offset = pSym->offset;
				//offset = -offset;

				if ( pSym->type->typeKind == PointerKind )	// 如果是指针的话
				{
					//offset = -offset;

					if ( offset < 0 )
					{
						sprintf_s( str, "[[ebp+%d]+%s]", -(long)(offset - sizeof( dword )), reg.c_str() );	// 参数
					}
					else 
					{
						sprintf_s( str, "[[ebp-%d]+%s]", offset, reg.c_str() );	// 局部变量
					}
					return str;
				}
			}
			else
			{
				// 结构体访问符
				auto iter = temp.find( '.' );
				if ( (iter = temp.find( '.' )) != std::string::npos )
				{
					pSym = m_pCurTable->GetSymbol( temp.substr( 0, iter ) );
					offset = pSym->offset;

					if ( pSym->type->structDescription )
					{
						pSym = pSym->type->structDescription->GetSymbol( temp.substr( iter + 1 ) );
						offset -= pSym->offset;

					}
					else throw "Error in temp2reg! About struct access";
				}
				else if ( (iter = temp.find( "->")) != std::string::npos )
				{
					pSym = m_pCurTable->GetSymbol( temp.substr( 0, iter ) );
					offset = pSym->offset;

					if ( pSym->type->next->structDescription )
					{
						pSym = pSym->type->next->structDescription->GetSymbol( temp.substr( iter + 2 ) );
						int structOffset = pSym->offset;

						if ( offset >= 0 )
						{
							sprintf_s( str, "[[ebp-%d]+%d]", offset, structOffset );	// 局部变量
						}
						else
						{
							sprintf_s( str, "[[ebp+%d]+%d]", -(long)(offset - sizeof( dword )), structOffset );	// 局部变量
						}

						return str;
					}
					else throw "Error in temp2reg! About struct access";
				}
				else
				{
					pSym = m_pCurTable->GetSymbol( temp );
					offset = pSym->offset;
				}
			}

			if ( offset >= 0 )
			{
				sprintf_s( str, "[ebp-%d]", offset );	// 局部变量
			}
			else
			{
				sprintf_s( str, "[ebp+%d]", -(long)(offset - sizeof( dword )) );	// 参数，保存时为负偏移，而参数在栈的高地址，所以改为正
			}

			std::string result = str;
			if ( reg.size() > 0 )
			{
				result.insert( result.size() - 1, "+" + reg );
			}

			return result;
		}

		return temp;
	}

	void Imm2asm::LocateLabel( const std::string& imm )
	{
		std::string line;
		std::string::const_iterator iter = imm.begin();			
		std::string curEntry;

		while ( iter != imm.end() )
		{
			line += *iter;

			if ( *iter == '\n' )
			{
				TokenList tokenList = str2Tok(line);

				if ( tokenList.size() >= 1 )
				{
					if ( tokenList[ 0 ] == "Label" )
					{
						m_label = combinationToInt( tokenList[ 1 ] );
					}
					else if ( tokenList[ 0 ] == "entry" )
					{
						curEntry = tokenList[ 1 ];
					}
					else
					{
						if ( m_label != -1 )
						{
							m_jumpMap[ m_label ] = m_lineno;
							m_label = -1;
						}
						if ( curEntry.size() > 0 )
						{
							m_entryMap[ curEntry ] = m_lineno;
							curEntry.clear();
						}
						m_lineno++;
					}
				}

				line = "";
			}

			iter++;
		}

		m_jumpMap[ 0 ] = m_lineno;

		m_lineno = 0;
		m_label = -1;
	}

	void Imm2asm::_translateLine( TokenList& tokenList )
	{
		std::string code;

		SetRegBaseNum(tokenList);		

		if ( tokenList[ 0 ] == "if" )
		{
			code = _if(tokenList);
		}
		else if ( tokenList[ 0 ] == "iffalse" )
		{
			code = _iffalse(tokenList );
		}
		else if ( tokenList[ 0 ] == "goto" )
		{
			code = _goto( tokenList );
		}
		else if ( tokenList[ 0 ] == "Lineno" )
		{
			code = "mov d0, " + tokenList[ 1 ];
			m_bNewStmt = true;
		}
		else if ( tokenList[ 0 ] == "Label" )
		{
			m_label = combinationToInt( tokenList[ 1 ] );
		}
		else if ( tokenList[ 0 ] == "entry" )
		{
			//code = tokenList[ 0 ] + " " + tokenList[ 1 ];
			m_pCurTable = m_pTables->GetTable( tokenList[ 1 ] );
		}
		else if ( tokenList[ 0 ] == "arg" )
		{
			code = tokenList[ 0 ] + " " + temp2reg( tokenList[ 1 ] );
		}
		else if ( tokenList[ 0 ] == "call" )
		{
			code = tokenList[ 0 ] + " " + itoa( m_entryMap[ tokenList[ 1 ] ] );
		}
		else if ( tokenList[ 0 ] == "halt" )
		{
			code = tokenList[ 0 ];
		}
		else if ( tokenList[ 0 ] == "push" )
		{
			code = tokenList[ 0 ] + " " + tokenList[ 1 ];
		}
		else if ( tokenList[ 0 ] == "pop" )
		{
			code = tokenList[ 0 ] + " " + temp2reg( tokenList[ 1 ] );
		}
		else if ( tokenList[ 0 ] == "mov") 
		{
			code = tokenList[ 0 ] + " " + tokenList[ 1 ] + ", " + tokenList[ 2 ];
		}
		else if ( tokenList[ 0 ] == "read" )
		{
			code = "in " + temp2reg( tokenList[ 1 ] );
		}
		else if ( tokenList[ 0 ] == "write" )
		{
			code = "out " + temp2reg( tokenList[ 1 ] );
		}
		else if ( tokenList.size() >= 3 && tokenList[ 1 ] == "=" )
		{
			code = GenAssign( tokenList );
		}
		else
		{
			//std::cout << "error!!!!!!!!!\n";
			for ( auto iter = tokenList.begin(); iter != tokenList.end(); iter++ )
			{
				code += *iter + " ";
			}
		}

		if ( code != "" )	
		{
			AddToASM(code);
		}
	}

	std::string Imm2asm::_goto( TokenList& tokenList )
	{
		std::string code = "jmp ";

		int target = m_jumpMap[ combinationToInt( tokenList[ 1 ] ) ];
		//int offset = target - m_lineno;
		char offStr[ 5 ];
		_itoa_s( target, offStr, 10 );
		code += offStr;	

		return code;
	}

	std::string Imm2asm::_if( TokenList& tokenList )
	{
		std::string code;
		std::string& op = tokenList[ 2 ];
		if ( op == "=" )
		{
			code = "je ";
		}
		else if ( op == "!=" )
		{
			code = "jne ";
		}
		else if ( op == ">" )
		{
			code = "jgt ";
		}
		else if ( op == ">=" )
		{
			code = "jge ";
		}
		else if ( op == "<" )
		{
			code = "jl ";
		}
		else if ( op == "<=" )
		{
			code = "jlt ";
		}

		code += temp2reg( tokenList[ 1 ] ) + ", ";
		code += temp2reg( tokenList[ 3 ] ) + ", ";
		int target = m_jumpMap[ combinationToInt( tokenList[ 5 ] ) ];
		//int offset = target - m_lineno;

		char offStr[ 5 ];
		_itoa_s( target, offStr, 10 );
		code += offStr;

		return code;
	}

	std::string Imm2asm::_iffalse( TokenList& tokenList )
	{
		std::string code;
		std::string& op = tokenList[ 2 ];
		if ( op == "=" )
		{
			code = "jne ";
		}
		else if ( op == "!=" )
		{
			code = "jeq ";
		}
		else if ( op == ">" )
		{
			code = "jle ";
		}
		else if ( op == ">=" )
		{
			code = "jlt ";
		}
		else if ( op == "<" )
		{
			code = "jge ";
		}
		else if ( op == "<=" )
		{
			code = "jgt ";
		}

		code += temp2reg( tokenList[ 1 ] ) + ", ";
		code += temp2reg( tokenList[ 3 ] ) + ", ";
		int target = m_jumpMap[ combinationToInt( tokenList[ 5 ] ) ];
		//int offset = target - m_lineno;

		char offStr[ 5 ];
		_itoa_s( target, offStr, 10 );
		code += offStr;	

		return code;
	}

	std::string Imm2asm::GenAssign( TokenList& tokenList )
	{
		std::string code;
		if ( tokenList[ 1 ] == "=" ) // 赋值语句
		{
			if ( tokenList.size() == 3 )	// 直接赋值
			{
				code = "mov ";
				code += temp2reg( tokenList[ 0 ] ) + ", ";
				code += temp2reg( tokenList[ 2 ] );
			}
			else if ( tokenList.size() == 4 )	// 单目运算符
			{
				if ( tokenList[ 2 ] == "*" )
				{
					code = "mov ";
					code += temp2reg( tokenList[ 0 ] ) + ", ";
					code += '[' + temp2reg( tokenList[ 3 ] ) + ']';
				}
				else if ( tokenList[ 2 ] == "&" )
				{
					code = "lea ";
					code += temp2reg( tokenList[ 0 ] ) + ", ";
					code += temp2reg( tokenList[ 3 ] );
				}
			}
			else	// 双目运算
			{
				switch ( tokenList[ 3 ][ 0 ] )
				{
				case '+':
					code = "add ";
					break;
				case '-':
					code = "sub ";
					break;
				case '*':
					code = "mul ";
					break;
				case '/':
					code = "div ";
					break;
				}

				code += temp2reg( tokenList[ 0 ] ) + ", ";
				code += temp2reg( tokenList[ 2 ] ) + ", ";	
				code += temp2reg( tokenList[ 4 ] );
			}
		}	

		return code;
	}

	void Imm2asm::SetRegBaseNum( TokenList& tokenList )
	{
		if ( m_bNewStmt ) // 新语句，重新分配寄存器
		{
			for ( auto iter = tokenList.begin(); iter != tokenList.end(); iter++ )
			{
				if ( iter->size() > 2 && iter->at( 0 ) == '#'  && iter->at( 1 ) == 't' && isdigit( iter->at( 2 ) ) )
				{
					m_regBaseNum = combinationToInt( (*iter).substr( 1 ) );
					break;
				}
			}
			m_bNewStmt = false;
		}
	}

	void Imm2asm::AddToASM( const std::string& code )
	{
		m_asmcode += code + '\n';

		//if ( m_label != -1 )
		//{
		//	m_jumpMap[ m_label ] = m_lineno;

		//	m_label = -1;
		//}

		++m_lineno;
	}

	int Imm2asm::combinationToInt( const std::string& num )
	{
		const char* numstr = num.c_str() + 1;
		return atoi( numstr );
	}

	std::string Imm2asm::TranslateToAsm( const std::string& imm, const Environment& tables )
	{
		m_pTables = (Environment*)&tables;

		std::string buffer = ExpandCode(imm);		// 为entry添加活动保存指令

		//std::cout << std::endl << buffer << std::endl;

		LocateLabel( buffer );

		std::string line;
		std::size_t iter = 0;

		while ( iter < buffer.size() )
		{
			line += buffer[ iter ];

			if ( buffer[ iter ] == '\n' )
			{
				TranslateLine(line);

				line = "";
			}

			iter++;
		}

		return m_asmcode;
	}

	



}