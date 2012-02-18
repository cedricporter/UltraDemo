/*
** ���ߣ�����
** ˵����Token
**
*/

#pragma once

#include "def.h"


/*
** ���֣�Tag
** ˵�������Token������
**
*/
class Tag
{
public:
	enum TagEnum
	{
		INT = 256,
		BOOL,
		CHAR,
		STRING,
		AND,
		OR,
		NOT,
		ASSIGN,
		WHILE,
		DO,
		IF,
		THEN,
		ELSE,
		END,
		REPEAT,
		UNTIL,
		READ,
		WRITE,
		NUM,
		LONGNUM,
		DOUBLE,
		FLOAT,
		EQ,
		NE,
		GE,
		LE,
		GT,
		LT,
		TRUETYPE,
		FALSETYPE,
		ID,
		PLUS,
		MINUS,
		TIMES,
		OVER,
		LPAREN,
		RPAREN,
		LBRACE,
		RBRACE,
		COMMA,
		SEMI,
		ERRORTYPE,
		ENDFILE
	};
};


/*
** ���֣�Token
** ˵��������Token�Ļ���
**
*/
class Token
{
public:
	int m_Tag;
	Token( Tag::TagEnum t ) : m_Tag( t ) {}
	virtual ~Token() {}
};


/*
** ���֣�Error
** ˵������¼����token���кź�����
**
*/
class Error : public Token
{
public:
	Error( int lineno = 0, const CETString& tokenString = _T("") ) 
		: Token( Tag::ERRORTYPE ), m_errorToken( tokenString ), m_lineno( lineno )
	{}

	CETString m_errorToken;
	int m_lineno;
};


/*
** ���֣�Num
** ˵��������Token
**
*/
class Num : public Token
{
public:
	int m_Val;
	Num( int v ) : Token( Tag::NUM ), m_Val( v ) {}
};


/*
** ���֣�Word
** ˵����ID��ؼ���
**
*/
class Word : public Token
{
public:
	CETString m_lexeme;
	explicit Word( const CETString& str, Tag::TagEnum tag ) : m_lexeme( str ), Token( tag ) {}

	//static const Word and, or, eq, ne, le, ge, True, False;
};


/*
** ���֣�Type
** ˵��������
**
*/
class Type : public Word
{
public:
	int m_width;
	Type( const CETString& s, Tag::TagEnum t, int w ) : m_width( w ), Word( s, t ) {}

};

