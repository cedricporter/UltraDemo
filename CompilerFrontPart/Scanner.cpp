/*
** 作者：华亮
** 说明：词法器实现
**
*/
#include "stdafx.h"
#include "Scanner.h"
#include <cstdio>
#include <algorithm>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#if SCANNER_REPORT
#define PRINT_TOKEN() 	PrintToken();
#else
#define PRINT_TOKEN()
#endif

namespace ETCompiler
{

	/*
	** 名字：ctor
	** 说明：
	**
	*/
	TinyScanner::TinyScanner() 
		:
	m_bFileOpened( 0 ),
		m_pFileReader( NULL ),
		m_pFSM( NULL )
		//m_otherHandlerCB( this, &Scanner::otherHandler ),
		//m_numHandlerCB( this, &Scanner::numHandler ),
		//m_assignHanderCB( this, &Scanner::assignHandler ),
		//m_charGetterCB( this, &Scanner::GetChar ),
		//m_errorHandlerCB( this, &Scanner::errorHandler ),
		//m_commentHandlerCB( this, &Scanner::commentHandler ),
		//m_whitespaceHandlerCB( this, &Scanner::whitespaceHandler ),
		//m_IDHandlerCB( this, &Scanner::IDHandler )
	{
		m_pFileReader = new FileReader;
		m_pFSM = new StateMachine;	

		AddHandlers();
		AddReservedWord();
	}

	/*
	** 名字：dtor
	** 说明：
	**
	*/
	TinyScanner::~TinyScanner()
	{
		SAFE_DELETE( m_pFSM );
		SAFE_DELETE( m_pFileReader );

		std::for_each( m_cbList.begin(), m_cbList.end(), [&]( HandlerFunctorType* pFunc ) { delete pFunc; } );

		for ( auto iter = m_reservedWords.begin(); iter != m_reservedWords.end(); ++iter )
		{
			if ( iter->second->m_Tag != Tag::BASIC )
			{
				delete iter->second;
			}
		}
		m_reservedWords.clear();

		//for ( auto iter = m_TokenList.begin(); iter != m_TokenList.end(); ++iter )
		//{
		//	delete *iter;
		//}
		//m_TokenList.clear();

	}

	/*
	** 名字：Scanner::AddReservedWord()
	** 说明：添加保留字
	**
	*/
	void TinyScanner::AddReservedWord()
	{
		reserve( new Word( _T("if"), Tag::IF ) );
		//reserve( new Word( _T("then"), Tag::THEN ) );
		reserve( new Word( _T("else"), Tag::ELSE ) );
		//reserve( new Word( _T("end"), Tag::END ) );

		reserve( new Word( _T("do"), Tag::DO ) );
		reserve( new Word( _T("while"), Tag::WHILE ) );
		reserve( new Word( _T("for"), Tag::FOR ) );

		reserve( new Word( _T("break"), Tag::BREAK ) );
		reserve( new Word( _T("continue"), Tag::CONTINUE ) );
		reserve( new Word( _T("return"), Tag::RETURN ) );

		reserve( new Word( _T("read"), Tag::READ ) );
		reserve( new Word( _T("write"), Tag::WRITE ) );

		reserve( new Word( _T("struct"), Tag::STRUCT ) );

		reserve( new Word( _T("new"), Tag::NEW ) );


		//reserve( new Word( _T("repeat"), Tag::REPEAT ) );
		//reserve( new Word( _T("until"), Tag::UNTIL ) );

		//reserve( new Word( _T("read"), Tag::READ ) );
		//reserve( new Word( _T("write"), Tag::WRITE ) );

		reserve( Type::Int );
		reserve( Type::Bool );
		reserve( new Word( _T("string"), Tag::STRING ) );

		reserve( new Word( _T("true"), Tag::TRUETYPE ) );
		reserve( new Word( _T("false"), Tag::FALSETYPE ) );

		//reserve( new Word( _T("and"), Tag::AND ) );
		//reserve( new Word( _T("or"), Tag::OR ) );
		//reserve( new Word( _T("not"), Tag::NOT ) );
	}

	/*
	** 名字：Scanner::AddHandlers()
	** 说明：为自动机添加回调函数
	**
	*/
	int TinyScanner::AddHandlers()
	{
		LPVOID param = NULL;
		HandlerFunctorType* functor;
		auto InsertCB = [&]() { functor = new HandlerFunctorType; m_cbList.push_back( functor );};

		// 处理其他情况的处理函数
		InsertCB();
		m_pFSM->AddOtherHandler( &( *functor = [=]()->int { return otherHandler( param ); }) );

		// 读取当前字符的处理函数
		InsertCB();
		m_pFSM->AddLookAheadCB( &( *functor = [=]()->int { return m_pFileReader->GetChar(); }) );

		// 遇到错误可被调用
		InsertCB();
		m_pFSM->AddErrorCB( &( *functor = [=]()->int { return errorHandler( param ); }) );

		//// 添加遇到“{”注释会调用的回调函数
		//InsertCB();
		//m_pFSM->AddHandler( '{', &( *functor = [=]()->int { return commentHandler( param ); }) );

		// 添加遇到“'”注释会调用的回调函数
		InsertCB();
		m_pFSM->AddHandler( '\'', &( *functor = [=]()->int { return strHandler( param ); }) );

		// 添加遇到数字会调用的回调函数
		InsertCB();
		*functor = [=]()->int { return numHandler( param ); };
		for ( char c = '0'; c <= '9'; c++ )
		{
			m_pFSM->AddHandler( c, functor );
		}

		// 添加遇到字母会调用的回调函数
		InsertCB();
		*functor = [=]()->int { return IDHandler( param ); };
		for ( char c = 'A'; c <= 'Z'; c++ )
		{
			m_pFSM->AddHandler( c, functor );
		}
		for ( char c = 'a'; c <= 'z'; c++ )
		{
			m_pFSM->AddHandler( c, functor );
		}

		// 添加遇到空白会调用的回调函数
		InsertCB();
		*functor = [=]()->int { return whitespaceHandler( param ); };
		m_pFSM->AddHandler( '\n', functor );
		m_pFSM->AddHandler( '\t', functor );
		m_pFSM->AddHandler( '\r', functor );
		m_pFSM->AddHandler( ' ', functor );

		return 0;
	}

	/*
	** 名字：Scanner::GetToken()
	** 说明：从文件流中取Token
	**
	*/
	Token* TinyScanner::GetToken()
	{
		assert( m_bFileOpened ); 				// 断言文件已经打开
		ResetToken();								// 重设当前Token

		try 
		{ 
			m_pFSM->Run();						// 启动状态机识别	
		}							
		catch ( int e )
		{ 
			if ( e == EOF )
			{
				delete m_currentToken;
				m_currentToken = new Token( Tag::ENDFILE ); 
			}
		}	

		// 设置Token行号
		m_currentToken->m_locateLine = m_pFileReader->GetLineno();

		//m_TokenList.push_back( m_currentToken );

		PRINT_TOKEN();

		return m_currentToken;					// 返回状态机处理完的Token
	}

	/*
	** 名字：Scanner::otherHandler( LPVOID param )
	** 说明：用于处理其他状态
	**
	*/
	int TinyScanner::otherHandler( LPVOID param )
	{
		char peek = m_pFileReader->GetNextChar();
		m_tokenString = peek;
	
		switch ( peek )
		{	
		case '&':
			peek = m_pFileReader->GetNextChar();
			if ( peek == '&' )
			{
				m_currentToken = new Token( Tag::AND ); m_tokenString += '&';
			}
			else
			{
				m_currentToken = new Token( Tag::REFERENCE );
				m_pFileReader->Prev();
			}
			break;
		case '|':
			peek = m_pFileReader->GetNextChar();
			if ( peek == '|' )
			{
				m_currentToken = new Token( Tag::OR );
				m_tokenString += '|';
			}
			break;
		case '%':
			m_currentToken = new Token( Tag::MOD );

			break;
		case '?':
			m_currentToken = new Token( Tag::QUESTIONMARK );
			break;	
		case '!':
			peek = m_pFileReader->GetNextChar();
			if ( peek == '=' )
			{
				m_currentToken = new Token( Tag::NE ); m_tokenString += '=';
			}
			else
			{
				m_currentToken = new Token( Tag::NOT );
				m_pFileReader->Prev();
			}
			break;
		case '.':
			m_currentToken = new Token( Tag::POINT );
			break;	
		case '[':
			m_currentToken = new Token( Tag::LSQUBRCK );
			break;	
		case ']':
			m_currentToken = new Token( Tag::RSQUBRCK );
			break;
		case '{':
			m_currentToken = new Token( Tag::LBRACE );
			break;
		case '}':
			m_currentToken = new Token( Tag::RBRACE );
			break;
		case ';':
			m_currentToken = new Token( Tag::SEMI );
			break;	
		case '(':
			m_currentToken = new Token( Tag::LPAREN );
			break;
		case ')':
			m_currentToken = new Token( Tag::RPAREN );
			break;
		case ',':
			m_currentToken = new Token( Tag::COMMA );
			break;
		case '=':
			peek = m_pFileReader->GetNextChar();
			if ( peek == '=' )
			{
				m_currentToken = new Token( Tag::EQ ); m_tokenString += '=';
			}
			else
			{
				m_currentToken = new Token( Tag::ASSIGN );
				m_pFileReader->Prev();
			}
			break;
		case ':':
			m_currentToken = new Token( Tag::COLON );
			break;
			//case ':':
			//	if ( m_pFileReader->GetNextChar() == '=' )
			//	{ m_currentToken = new Token( Tag::ASSIGN ); m_tokenString += '='; }
			//	else
			//	{ errorHandler( param ); m_pFileReader->Prev(); }
			//	break;
		case '+':
			peek = m_pFileReader->GetNextChar();
			if ( peek == '+' )
			{
				m_currentToken = new Token( Tag::INC ); m_tokenString += '+';
			}
			else if ( peek == '=' )
			{
				m_currentToken = new Token( Tag::PLUSASSIGN ); m_tokenString += '=';
			}
			else
			{
				m_currentToken = new Token( Tag::PLUS );
				m_pFileReader->Prev();
			}
			break;
		case '-':
			peek = m_pFileReader->GetNextChar();
			if ( peek == '>' )
			{ 
				m_currentToken = new Token( Tag::ARROW ); m_tokenString += '>'; 
			}
			else if ( peek == '=' )
			{

				m_currentToken = new Token( Tag::MINUSASSIGN ); m_tokenString += '=';
			}
			else if ( peek == '-' )
			{

				m_currentToken = new Token( Tag::DEC ); m_tokenString += '-';
			}
			else 
			{ 
				m_currentToken = new Token( Tag::MINUS );
				m_pFileReader->Prev();
			}
			break;
		case '*':
			peek = m_pFileReader->GetNextChar();
			if ( peek == '=' )
			{ 
				m_currentToken = new Token( Tag::MULASSIGN ); m_tokenString += '='; 
			}
			else
			{
				m_currentToken = new Token( Tag::TIMES );
				m_pFileReader->Prev();
			}
			break;
		case '/':
			peek = m_pFileReader->GetNextChar();
			if ( peek == '=' )
			{ 
				m_currentToken = new Token( Tag::DIVASSIGN ); m_tokenString += '='; 
			}
			else if ( peek == '/' )
			{
				while ( peek != '\n' )
				{
					peek = m_pFileReader->GetNextChar();
				}
				//peek = m_pFileReader->GetNextChar();
				//otherHandler( NULL );
				m_tokenString.clear();
				return whitespaceHandler( param );
			}
			else
			{
				m_currentToken = new Token( Tag::OVER );
				m_pFileReader->Prev();
			}
			break;
		case '<':
			peek = m_pFileReader->GetNextChar();
			if ( peek == '=' )
			{ 
				m_currentToken = new Token( Tag::LE ); 
				m_tokenString += '='; 
			}
			else 
			{ 
				m_currentToken = new Token( Tag::LT );
				m_pFileReader->Prev();
			}
			break;
		case '>':
			peek = m_pFileReader->GetNextChar();
			if ( peek == '=' )
			{
				m_currentToken = new Token( Tag::GE ); m_tokenString += '='; 
			}
			else
			{
				m_currentToken = new Token( Tag::GT );
				m_pFileReader->Prev();
			}
			break;
		default:
			errorHandler(param);
			m_pFileReader->Prev();
			break;
		}



		return END;
	}

	/*
	** 名字：Scanner::commentHandler( LPVOID param )
	** 说明：处理注释
	**
	*/
	int TinyScanner::commentHandler( LPVOID param )
	{
		int commentStartLine = m_pFileReader->GetLineno();
		try
		{
			while ( m_pFileReader->GetNextChar() != '}' );
		}
		catch ( int e )
		{
			if ( e == EOF )
			{
				m_currentToken = new Error( commentStartLine, _T("注释未匹配") + m_tokenString );
				errorHandler( (LPVOID)m_currentToken );
				throw e;
			}
		}

		return CONTINUE;
	}

	/*
	** 名字：Scanner::whitespaceHandler( LPVOID param )
	** 说明：处理空白
	**
	*/
	int TinyScanner::whitespaceHandler( LPVOID param )
	{
		char peek;
		do  
		{
			peek = m_pFileReader->GetNextChar();
		} while ( peek == '\t' || peek == '\n' || peek == ' ' || peek == '\r' );
		m_pFileReader->Prev();

		return CONTINUE;
	}

	/*
	** 名字：Scanner::numHandler( LPVOID param )
	** 说明：处理数字
	**
	*/
	int TinyScanner::numHandler( LPVOID param )
	{
		char peek = m_pFileReader->GetNextChar();
		while ( isdigit( peek ) )
		{
			m_tokenString += peek;
			peek = m_pFileReader->GetNextChar();
		} 
		m_pFileReader->Prev();

		m_currentToken = new Num( atoi( m_tokenString.c_str() ) );

		return END;
	}


	/*
	** 名字：Scanner::IDHandler( LPVOID param )
	** 说明：处理标识符和关键字
	**
	*/
	int TinyScanner::IDHandler( LPVOID param )
	{
		char peek = m_pFileReader->GetNextChar();
		while ( isalnum( peek ) )
		{
			m_tokenString += peek;
			peek = m_pFileReader->GetNextChar();
		} 
		m_pFileReader->Prev();

		if ( m_complexTypeMap.find( m_tokenString ) != m_complexTypeMap.end() )
		{
			m_currentToken = m_complexTypeMap[ m_tokenString ];	// 组合类型
		}
		else if ( m_reservedWords.find( m_tokenString ) != m_reservedWords.end() )
		{
			Word* word = m_reservedWords[ m_tokenString ];
			if ( word->m_Tag == Tag::BASIC )	// 如果是类型的话
			{
				m_currentToken = (Type*)m_reservedWords[ m_tokenString ];	// 类型不需要复制很多个，只有一个即可。
			}
			else	// 如果仅是一般保留字
			{
				m_currentToken = new Word( *m_reservedWords[ m_tokenString ] );
			}
		}
		else
		{
			m_currentToken = new Word( m_tokenString, Tag::ID );
		}

		return END;
	}

	/*
	** 名字：Scanner::strHandler( LPVOID param )
	** 说明：处理字符串
	**
	*/
	int TinyScanner::strHandler( LPVOID param )
	{
		m_pFileReader->Next();	// eat '
		char peek = m_pFileReader->GetNextChar();
		while ( true )
		{
			m_tokenString += peek;
			peek = m_pFileReader->GetNextChar();

			if ( peek == '\'' ) 
			{
				m_currentToken = new Word( m_tokenString, Tag::STRING );
				break;	
			}
			else if ( peek == '\\' )
			{	
				char peek = m_pFileReader->GetNextChar();
				if ( peek == '\'' )
				{ 
				}
				else
				{
					m_tokenString += '\\';
				}
			}
			else if ( peek == '\n' )
			{
				m_currentToken = new Error( m_pFileReader->GetLineno() - 1, _T("字符串未匹配") + m_tokenString );
				break;
			}
		} 

		return END;
	}

	/*
	** 名字：Scanner::errorHandler( LPVOID )
	** 说明：处理错误情况，会打印错误行号
	**
	*/
	int TinyScanner::errorHandler( LPVOID param )
	{
		std::size_t lineno = m_pFileReader->GetLineno();
		if ( ! param )
		{
			m_currentToken = new Error( lineno, m_tokenString );
		}

		std::cout << _T("Line at:") << dynamic_cast<Error*>(m_currentToken)->m_lineno << " " 
			<< dynamic_cast<Error*>(m_currentToken)->m_errorToken << std::endl;

		return END;
	}


	/*
	** 名字：Scanner::PrintToken()
	** 说明：打印当前Token
	**
	*/
	void TinyScanner::PrintToken()
	{
		assert( m_currentToken );

		CETString typeStr;
		switch ( m_currentToken->m_Tag )
		{
		case Tag::ID:
			typeStr = _T("ID");
			break;
		case Tag::NUM:
			typeStr = _T("NUM");
			break;
		case Tag::STRING:
			typeStr = _T("STR");
			break;
		case Tag::ENDFILE:
			return;
		default:
			typeStr = _T("SYM");
			break;
		}

		if ( m_reservedWords.find( m_tokenString ) != m_reservedWords.end() )
		{
			typeStr = _T("KEY");
		}

		std::cout << "( " << typeStr << ", " << m_tokenString << " )";
	}

	void TinyScanner::reserveType( Token* type )
	{
		m_complexTypeMap[ ((Word*)type)->m_lexeme ] = (Word*)type;
	}

}