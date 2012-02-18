/*
** 作者：华亮
** 说明：
**
*/

#include "stdafx.h"
#include "Token.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace ETCompiler
{

	//////////////////////////////////////////////////////////////////////////
	//
	// Class Type
	// 

	Type* Type::Int = new Type( "int", Tag::BASIC, 4 );
	Type* Type::Float = new Type( "float", Tag::BASIC, 8 );
	Type* Type::Char = new Type( "char", Tag::BASIC, 1 );
	Type* Type::Bool = new Type( "bool", Tag::BASIC, 1 );

}