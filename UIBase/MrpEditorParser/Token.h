/*
** 作者：华亮
** 说明：Token
**
*/

#pragma once

#include "def.h"


/*
** 名字：Tag
** 说明：标记Token的类型
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
** 名字：Token
** 说明：所有Token的基类
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
** 名字：Error
** 说明：记录错误token的行号和名字
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
** 名字：Num
** 说明：整数Token
**
*/
class Num : public Token
{
public:
	int m_Val;
	Num( int v ) : Token( Tag::NUM ), m_Val( v ) {}
};


/*
** 名字：Word
** 说明：ID或关键字
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
** 名字：Type
** 说明：类型
**
*/
class Type : public Word
{
public:
	int m_width;
	Type( const CETString& s, Tag::TagEnum t, int w ) : m_width( w ), Word( s, t ) {}

};

