/*
** ���ߣ�����
** ˵�����ʷ���ʵ��
**
*/
#include "stdafx.h"
#include "Scanner.h"
#include <cstdio>
#include <algorithm>
#include <iostream>


#if SCANNER_REPORT
#define PRINT_TOKEN() 	PrintToken();
#else
#define PRINT_TOKEN()
#endif




/*
** ���֣�ctor
** ˵����
**
*/
Scanner::Scanner() 
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
** ���֣�dtor
** ˵����
**
*/
Scanner::~Scanner()
{
	SAFE_DELETE( m_pFSM );
	SAFE_DELETE( m_pFileReader );

	std::for_each( m_cbList.begin(), m_cbList.end(), [&]( HandlerFunctorType* pFunc ) { delete pFunc; } );
}

/*
** ���֣�Scanner::AddReservedWord()
** ˵������ӱ�����
**
*/
void Scanner::AddReservedWord()
{
	/*reserve( new Word( _T("if"), Tag::IF ) );
	reserve( new Word( _T("then"), Tag::THEN ) );
	reserve( new Word( _T("else"), Tag::ELSE ) );
	reserve( new Word( _T("end"), Tag::END ) );

	reserve( new Word( _T("do"), Tag::DO ) );
	reserve( new Word( _T("while"), Tag::WHILE ) );

	reserve( new Word( _T("repeat"), Tag::REPEAT ) );
	reserve( new Word( _T("until"), Tag::UNTIL ) );

	reserve( new Word( _T("read"), Tag::READ ) );
	reserve( new Word( _T("write"), Tag::WRITE ) );

	reserve( new Word( _T("int"), Tag::INT ) );
	reserve( new Word( _T("bool"), Tag::BOOL) );
	reserve( new Word( _T("string"), Tag::STRING ) );

	reserve( new Word( _T("true"), Tag::TRUETYPE ) );
	reserve( new Word( _T("false"), Tag::FALSETYPE ) );

	reserve( new Word( _T("and"), Tag::AND ) );
	reserve( new Word( _T("or"), Tag::OR ) );
	reserve( new Word( _T("not"), Tag::NOT ) );*/
}

/*
** ���֣�Scanner::AddHandlers()
** ˵����Ϊ�Զ�����ӻص�����
**
*/
int Scanner::AddHandlers()
{
	LPVOID param = NULL;
	HandlerFunctorType* functor;
	auto InsertCB = [&]() { functor = new HandlerFunctorType; m_cbList.push_back( functor );};

	// ������������Ĵ�����
	InsertCB();
	m_pFSM->AddOtherHandler( &( *functor = [=]()->int { return otherHandler( param ); }) );

	// ��ȡ��ǰ�ַ��Ĵ�����
	InsertCB();
	m_pFSM->AddLookAheadCB( &( *functor = [=]()->int { return m_pFileReader->GetChar(); }) );

	// ��������ɱ�����
	InsertCB();
	m_pFSM->AddErrorCB( &( *functor = [=]()->int { return errorHandler( param ); }) );

	// ���������;��ע�ͻ���õĻص�����
	InsertCB();
	m_pFSM->AddHandler( ';', &( *functor = [=]()->int { return commentHandler( param ); }) );

	// ���������'��ע�ͻ���õĻص�����
	InsertCB();
	m_pFSM->AddHandler( '\'', &( *functor = [=]()->int { return strHandler( param ); }) );

	// ����������ֻ���õĻص�����
	InsertCB();
	*functor = [=]()->int { return numHandler( param ); };
	for ( char c = '0'; c <= '9'; c++ )
	{
		m_pFSM->AddHandler( c, functor );
	}

	// ���������ĸ����õĻص�����
	InsertCB();
	*functor = [=]()->int { return IDHandler( param ); };
	for ( char c = 'A'; c <= 'z'; c++ )
	{
		m_pFSM->AddHandler( c, functor );
	}

	// ��������հ׻���õĻص�����
	InsertCB();
	*functor = [=]()->int { return whitespaceHandler( param ); };
	m_pFSM->AddHandler( '\n', functor );
	m_pFSM->AddHandler( '\t', functor );
	m_pFSM->AddHandler( '\r', functor );
	m_pFSM->AddHandler( ' ', functor );

	return 0;
}

/*
** ���֣�Scanner::GetToken()
** ˵�������ļ�����ȡToken
**
*/
Token* Scanner::GetToken()
{
	assert( m_bFileOpened ); 				// �����ļ��Ѿ���
	Reset();								// ���赱ǰToken

	try 
	{ 
		m_pFSM->Run();						// ����״̬��ʶ��	
	}							
	catch ( int e )
	{ 
		if ( e == EOF ) m_currentToken = new Token( Tag::ENDFILE ); 
	}	

	PRINT_TOKEN();

	return m_currentToken;					// ����״̬���������Token
}

/*
** ���֣�Scanner::otherHandler( LPVOID param )
** ˵�������ڴ�������״̬
**
*/
int Scanner::otherHandler( LPVOID param )
{
	char peek = m_pFileReader->GetNextChar();
	m_tokenString = peek;

	switch ( peek )
	{	
	case '{':
		m_currentToken = new Token( Tag::LBRACE );
		break;	
	case '}':
		m_currentToken = new Token( Tag::RBRACE );
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
		m_currentToken = new Token( Tag::EQ );
		break;
	case ':':
		if ( m_pFileReader->GetNextChar() == '=' )
		{ m_currentToken = new Token( Tag::ASSIGN ); m_tokenString += '='; }
		else
		{ errorHandler( param ); m_pFileReader->Prev(); }
		break;
	case '+':
		m_currentToken = new Token( Tag::PLUS );
		break;
	case '-':
		m_currentToken = new Token( Tag::MINUS );
		break;
	case '*':
		m_currentToken = new Token( Tag::TIMES );
		break;
	case '/':
		m_currentToken = new Token( Tag::OVER );
		break;
	case '<':
		if ( m_pFileReader->GetNextChar() == '=' )
		{ m_currentToken = new Token( Tag::LE ); m_tokenString += '='; }
		else 
		{ m_currentToken = new Token( Tag::LT ); }
		break;
	case '>':
		if ( m_pFileReader->GetNextChar() != '=' )
		{ m_currentToken = new Token( Tag::GE ); m_tokenString += '='; }
		else
		{ m_currentToken = new Token( Tag::GT ); }
		break;
	default:
		errorHandler(param);
		m_pFileReader->Prev();
		break;
	}



	return END;
}

/*
** ���֣�Scanner::commentHandler( LPVOID param )
** ˵��������ע��
**
*/
int Scanner::commentHandler( LPVOID param )
{
	int commentStartLine = m_pFileReader->GetLineno();
	try
	{
		while ( m_pFileReader->GetNextChar() != '\n' );
	}
	catch ( int e )
	{
		if ( e == EOF )
		{
			m_currentToken = new Error( commentStartLine, _T("ע��δƥ��") + m_tokenString );
			errorHandler( (LPVOID)m_currentToken );
			throw e;
		}
	}
	
	return CONTINUE;
}

/*
** ���֣�Scanner::whitespaceHandler( LPVOID param )
** ˵��������հ�
**
*/
int Scanner::whitespaceHandler( LPVOID param )
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
** ���֣�Scanner::numHandler( LPVOID param )
** ˵������������
**
*/
int Scanner::numHandler( LPVOID param )
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
** ���֣�Scanner::IDHandler( LPVOID param )
** ˵���������ʶ���͹ؼ���
**
*/
int Scanner::IDHandler( LPVOID param )
{
	char peek = m_pFileReader->GetNextChar();
	while ( isalnum( peek ) || peek == '_' )
	{
		m_tokenString += peek;
		peek = m_pFileReader->GetNextChar();
	} 
	m_pFileReader->Prev();

	if ( m_reservedWords.find( m_tokenString ) != m_reservedWords.end() )
	{
		m_currentToken = new Word( *m_reservedWords[ m_tokenString ] );
	}
	else
	{
		m_currentToken = new Word( m_tokenString, Tag::ID );
	}

	return END;
}

/*
** ���֣�Scanner::strHandler( LPVOID param )
** ˵���������ַ���
**
*/
int Scanner::strHandler( LPVOID param )
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
			m_currentToken = new Error( m_pFileReader->GetLineno() - 1, _T("�ַ���δƥ��") + m_tokenString );
			break;
		}
	} 
	
	return END;
}

/*
** ���֣�Scanner::errorHandler( LPVOID )
** ˵�������������������ӡ�����к�
**
*/
int Scanner::errorHandler( LPVOID param )
{
	std::size_t lineno = m_pFileReader->GetLineno();
	if ( ! param )
	{
		m_currentToken = new Error( lineno, m_tokenString );
	}

	std::cout << _T("Line at:") << dynamic_cast<Error*>(m_currentToken)->m_lineno << " " << dynamic_cast<Error*>(m_currentToken)->m_errorToken << std::endl;

	return END;
}


/*
** ���֣�Scanner::PrintToken()
** ˵������ӡ��ǰToken
**
*/
void Scanner::PrintToken()
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

