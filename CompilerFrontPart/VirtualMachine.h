/*
** 作者：华亮
** 说明：
**
*/

#pragma once

#include "Parser.h"

#include <map>
#include <string>
#include <stack>
#include <list>

using namespace ETCompiler;


struct ValueNode;

typedef std::map< CETString, ValueNode >				StackElementType;
typedef std::list< std::map< CETString, ValueNode > >	StackType;


/*
** 名字：ValueNode
** 说明：封装所有类型
**
*/
struct ValueNode
{
	union 
	{
		int		iVal;
		double	fVal;
		char	cVal;
		bool	bVal;
		void*	pVal;
	} Val;

	Type* pType;

	ValueNode() : pType( NULL )
	{
		Val.fVal = 0; 
	}

	ValueNode( bool b )
	{
		Val.bVal = b;
		pType = Type::Bool;
	}

	ValueNode( int i )
	{
		Val.iVal = i;
		pType = Type::Int;
	}

	ValueNode( double f )
	{
		Val.fVal = f;
		pType = Type::Float;
	}
	
	ValueNode operator+( const ValueNode& node )
	{
		if ( this->pType->m_lexeme != node.pType->m_lexeme )	throw "type is different!";
		ValueNode retNode;
		retNode.pType = this->pType;

		if ( this->pType->m_lexeme == "int" ) retNode.Val.iVal = this->Val.iVal + node.Val.iVal;
		if ( this->pType->m_lexeme == "double" ) retNode.Val.fVal = this->Val.fVal + node.Val.fVal;
		if ( this->pType->m_lexeme == "char" ) retNode.Val.cVal = this->Val.cVal + node.Val.cVal;
		if ( this->pType->m_lexeme == "bool" ) retNode.Val.bVal = this->Val.bVal + node.Val.bVal;

		return retNode;
	}

	ValueNode operator-( const ValueNode& node )
	{
		if ( this->pType->m_lexeme != node.pType->m_lexeme )	throw "type is different!";
		ValueNode retNode;
		retNode.pType = this->pType;
		if ( this->pType->m_lexeme == "int" ) retNode.Val.iVal = this->Val.iVal - node.Val.iVal;
		if ( this->pType->m_lexeme == "double" ) retNode.Val.fVal = this->Val.fVal - node.Val.fVal;
		if ( this->pType->m_lexeme == "char" ) retNode.Val.cVal = this->Val.cVal - node.Val.cVal;
		if ( this->pType->m_lexeme == "bool" ) retNode.Val.bVal = this->Val.bVal - node.Val.bVal;
		return retNode;
	}

	ValueNode operator*( const ValueNode& node )
	{
		if ( this->pType->m_lexeme != node.pType->m_lexeme )	throw "type is different!";
		ValueNode retNode;
		retNode.pType = this->pType;
		if ( this->pType->m_lexeme == "int" ) retNode.Val.iVal = this->Val.iVal * node.Val.iVal;
		if ( this->pType->m_lexeme == "double" ) retNode.Val.fVal = this->Val.fVal * node.Val.fVal;
		if ( this->pType->m_lexeme == "char" ) retNode.Val.cVal = this->Val.cVal * node.Val.cVal;
		if ( this->pType->m_lexeme == "bool" ) retNode.Val.bVal = this->Val.bVal * node.Val.bVal;
		return retNode;
	}

	ValueNode operator/( const ValueNode& node )
	{
		if ( this->pType->m_lexeme != node.pType->m_lexeme )	throw "type is different!";
		ValueNode retNode;
		retNode.pType = this->pType;
		if ( this->pType->m_lexeme == "int" ) retNode.Val.iVal = this->Val.iVal / node.Val.iVal;
		if ( this->pType->m_lexeme == "double" ) retNode.Val.fVal = this->Val.fVal / node.Val.fVal;
		if ( this->pType->m_lexeme == "char" ) retNode.Val.cVal = this->Val.cVal / node.Val.cVal;
		if ( this->pType->m_lexeme == "bool" ) retNode.Val.bVal = this->Val.bVal / node.Val.bVal;
		return retNode;
	}

	bool operator==( const ValueNode& node )
	{
		if ( this->pType->m_lexeme != node.pType->m_lexeme )	throw "type is different!";
		if ( this->pType->m_lexeme == "int" ) return this->Val.iVal == node.Val.iVal;
		if ( this->pType->m_lexeme == "double" ) return this->Val.fVal == node.Val.fVal;
		if ( this->pType->m_lexeme == "char" ) return this->Val.cVal == node.Val.cVal;
		if ( this->pType->m_lexeme == "bool" ) return this->Val.bVal == node.Val.bVal;

		// if a pointer
		return false;
	}
	
	bool operator!=( const ValueNode& node )
	{
		if ( this->pType->m_lexeme != node.pType->m_lexeme )	throw "type is different!";
		if ( this->pType->m_lexeme == "int" ) return this->Val.iVal != node.Val.iVal;
		if ( this->pType->m_lexeme == "double" ) return this->Val.fVal != node.Val.fVal;
		if ( this->pType->m_lexeme == "char" ) return this->Val.cVal != node.Val.cVal;
		if ( this->pType->m_lexeme == "bool" ) return this->Val.bVal != node.Val.bVal;

		// if a pointer
		return false;
	}

	bool operator>( const ValueNode& node )
	{
		if ( this->pType->m_lexeme != node.pType->m_lexeme )	throw "type is different!";
		if ( this->pType->m_lexeme == "int" ) return this->Val.iVal > node.Val.iVal;
		if ( this->pType->m_lexeme == "double" ) return this->Val.fVal > node.Val.fVal;
		if ( this->pType->m_lexeme == "char" ) return this->Val.cVal > node.Val.cVal;
		if ( this->pType->m_lexeme == "bool" ) return this->Val.bVal > node.Val.bVal;

		// if a pointer
		return false;
	}

	bool operator>=( const ValueNode& node )
	{
		if ( this->pType->m_lexeme != node.pType->m_lexeme )	throw "type is different!";
		if ( this->pType->m_lexeme == "int" ) return this->Val.iVal >= node.Val.iVal;
		if ( this->pType->m_lexeme == "double" ) return this->Val.fVal >= node.Val.fVal;
		if ( this->pType->m_lexeme == "char" ) return this->Val.cVal >= node.Val.cVal;
		if ( this->pType->m_lexeme == "bool" ) return this->Val.bVal >= node.Val.bVal;

		// if a pointer
		return false;
	}

	bool operator<( const ValueNode& node )
	{
		if ( this->pType->m_lexeme != node.pType->m_lexeme )	throw "type is different!";
		if ( this->pType->m_lexeme == "int" ) return this->Val.iVal < node.Val.iVal;
		if ( this->pType->m_lexeme == "double" ) return this->Val.fVal < node.Val.fVal;
		if ( this->pType->m_lexeme == "char" ) return this->Val.cVal < node.Val.cVal;
		if ( this->pType->m_lexeme == "bool" ) return this->Val.bVal < node.Val.bVal;

		// if a pointer
		return false;
	}

	bool operator<=( const ValueNode& node )
	{
		if ( this->pType->m_lexeme != node.pType->m_lexeme )	throw "type is different!";
		if ( this->pType->m_lexeme == "int" ) return this->Val.iVal <= node.Val.iVal;
		if ( this->pType->m_lexeme == "double" ) return this->Val.fVal <= node.Val.fVal;
		if ( this->pType->m_lexeme == "char" ) return this->Val.cVal <= node.Val.cVal;
		if ( this->pType->m_lexeme == "bool" ) return this->Val.bVal <= node.Val.bVal;

		// if a pointer
		return false;
	}

};

class VirtualMachine
{
public:
	typedef std::stack< StmtSeq* >							EnvStackType;

private:
	EnvStackType		m_EnvStack;			// 运行时栈
	Parser				m_parser;			// 语法器
	StmtSeq*			m_currentNode;		// 当前执行到的节点
	StackType			m_Stack;			// 运行时栈


public:
	VirtualMachine();

	void LoadFromString( const CETString& codeString );

	void Compile();

	bool Step();

	bool Run();

	bool IsReady() const
	{
		return !!m_currentNode;
	}

	const StackType& GetSymbols() const
	{
		return m_Stack;
	}

private:

	void Pop();

	bool RecoverEnvironment();

	void RunWhileStmt( Stmt* s1, Stmt* s2 );

	void RunIfElseStmt( Stmt* s2, Stmt* s1 );

	void Push( Stmt* s2 );

	void RunIfStmt( Stmt* s2, Stmt* s1 );

	void RunSetStmt( Stmt* s1, Stmt* s2 );

	void RunDeclareStmt( Stmt* s1, Stmt* s2 );

	void RunBreakStmt( Stmt* s1 );

	void Reset();

	ValueNode CalcExp( Expr* node );

};
