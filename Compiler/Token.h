/*
** 作者：华亮
** 说明：Token
**
*/

#pragma once

#include "def.h"

namespace ETCompiler
{


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
			AND,		// &&
			ARROW,		// ->
			ASSIGN,		// =
			BASIC,		// 基本类型
			BOOL,
			BREAK,		// break
			CASE,		// case
			CHAR,
			COLON,		// :
			COMMA,		// ,
			CONTINUE,	// continue
			DEC,		// --
			DIVASSIGN,	// /=
			DO,			// do
			DOUBLE,		// double
			ELSE,		// else
			END,
			ENDFILE,
			EQ,			// ==
			ERRORTYPE,
			FALSETYPE,	// false
			FLOAT,		// float
			FOR,		// for
			GE,			// >=
			GT,			// >
			ID,
			IF,			// if
			INC,		// ++
			INDEX,		// []
			INT,
			LBRACE,		// {
			LE,			// <=
			LONGNUM,	// long
			LPAREN,		// (
			LSQUBRCK,	// [
			LT,			// <
			MINUS,		// - 
			MINUSASSIGN,// -=
			MOD,		// %
			MODASSIGN,	// %=
			MULASSIGN,	// *=
			NEW,
			NE,			// !=
			NOT,		// !
			NUM,		// number
			OR,			// ||
			OVER,		// /
			PLUS,		// +
			PLUSASSIGN,	// +=
			POINT,		// .
			QUESTIONMARK,//?
			RBRACE,		// }
			READ,
			REFERENCE,	// &
			REPEAT,
			RETURN,		// return
			RPAREN,		// )
			RSQUBRCK,	// ]
			SEMI,		// ;
			STRING,
			STRUCT,		// struct
			SWITCH,		// SWITCH
			TEMP,		// 临时变量
			THEN,
			TIMES,		// *
			TRUETYPE,	// true
			UNION,		// union
			UNTIL,
			WHILE,		// while
			WRITE
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
		int m_locateLine;
		Token( Tag::TagEnum t ) : m_Tag( t ) {}
		virtual ~Token() {}

		virtual std::string ToString()
		{
			switch ( m_Tag )
			{
			case Tag::POINT:
				return ".";
			case Tag::ARROW:
				return "->";
			case Tag::PLUS:
				return "+";
			case  Tag::MINUS:
				return "-";
			case Tag::TIMES:
				return "*";
			case Tag::REFERENCE:
				return "&";
			case  Tag::OVER:
				return "/";
			case Tag::EQ:
			case Tag::ASSIGN:
				return "=";
			case Tag::NE:
				return "!=";
			case Tag::LT:
				return "<";
			case Tag::LE:
				return "<=";
			case Tag::GT:
				return ">";
			case Tag::GE:
				return ">=";
			case Tag::AND:
				return "&&";
			case Tag::OR:
				return "||";
			default:
				return ""; 
			}		
		}

		virtual Token* Clone() { return new Token( *this ); }
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

		Token* Clone()
		{
			return new Num( *this );
		}

		std::string ToString()
		{ 
			char val[ 20 ];
			sprintf_s( val, "%d", m_Val );
			return val;
		}
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
		Word( const CETString& str, Tag::TagEnum tag ) : m_lexeme( str ), Token( tag ) {}
		~Word() {}
		//static const Word and, or, eq, ne, le, ge, True, False;

		std::string ToString()
		{ 
			return m_lexeme;
		}

		Token* Clone()
		{
			return new Word( *this );
		}
	};


	class Pointer;
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

		Token* Clone()
		{
			return this;
		}

		// 判断是否为数字类型等价的类型
		static bool Numeric( Type* tp )
		{
			if ( tp == Type::Char || tp == Type::Int || tp == Type::Float || typeid( *tp ) == typeid( Pointer ) )	return true;

			return false;
		}

		// 返回最大类型
		static Type* MaxType( Type* tp1, Type* tp2 )
		{
			if ( tp1->m_Tag == Tag::STRUCT )	return tp1;
			else if ( tp2->m_Tag == Tag::STRUCT )	return tp2;

			if ( ! Numeric( tp1 ) || ! Numeric( tp2 ) )	return NULL;
			else if ( tp1 == Type::Float || tp2 == Type::Float )	return Float;
			else if ( typeid( *tp1 ) == typeid( Pointer ) )	return tp1;
			else if ( typeid( *tp2 ) == typeid( Pointer ) ) return tp2;
			else if ( tp1 == Type::Int || tp2 == Type::Int )	return Int;
			else	return Char;
		}

		static Type *Int, *Float, *Char, *Bool; 
	};





	/*
	** 名字：Array
	** 说明：数组类型
	**
	*/
	class Array : public Type
	{
	public:
		Type*	m_pElementType;
		int		m_size;
		Array( int sz, Type* p ) : Type( "[]", Tag::INDEX, sz * p->m_width ), m_size( sz ), m_pElementType( p )
		{
		}

		std::string ToString()
		{
			return "[" + itoa( m_size ) + "]" + m_pElementType->ToString();
		}

	};


	class Pointer : public Type
	{
	public:
		Type*	m_pElementType;
		int		m_size;

		Pointer( Type* p ) : Type( "*", Tag::TIMES, sizeof( long ) ), m_size( sizeof( long ) ), m_pElementType( p )
		{
		}

		std::string ToString()
		{
			return "*" + m_pElementType->ToString();
		}

	};
}