/*
** 作者：华亮
** 说明：
**
*/

#pragma once

#include "Token.h"
#include <list>
#include <stdio.h>
#include <stdarg.h>
#include <algorithm>
#include <string>

namespace ETCompiler
{
	class IParser;

	/*
	** 名字：AssembyCodeGen
	** 说明：用于输出中间代码
	**
	*/
	class AssembyCodeGen
	{
	public:
		static IParser* parser;
		static bool GenCode( const char* code );
	};

	class IScanner;

	class LinenoSupplier
	{
	public:
		static IScanner*	pScanner;
		static int GetLineno();
	};

	/*
	** 名字：TreeNode
	** 说明：语法树节点基类。
	**
	*/
	class TreeNode
	{ 
	public:
		int m_lineno;
		static int m_labels;

		TreeNode()
		{
			m_lineno = LinenoSupplier::GetLineno();
		}

		virtual ~TreeNode() {}

		int NewLabel()
		{
			return ++m_labels;
		}

		virtual void EmitLabel( int i )
		{
			char code[ 20 ];
			sprintf_s( code, "Label L%d\n", i );
			AssembyCodeGen::GenCode( code );
		}

		virtual void Emit( const std::string& s )
		{
			AssembyCodeGen::GenCode( ('\t' + s + "\n").c_str() );
		}

		virtual void Error( char* errorMessage )
		{
			char mess[ 50 ];
			sprintf_s( mess, "near line %d :  %s", m_lineno, errorMessage );
			throw mess;
		}
	};





	/************************************************************************/
	/*					                                                    */
	/*					表达式节点定义                                       */
	/*																	    */
	/************************************************************************/


	/*
	** 名字：Expr
	** 说明：表达式节点
	**
	*/
	class Expr : public TreeNode
	{
	public:
		Token*	m_pTok;
		Type*	m_pType;

		Expr( Token* tok, Type* p ) : m_pTok( tok ), m_pType( p ) {}
		virtual ~Expr()
		{
			SAFE_DELETE( m_pTok );
			//delete m_pType;	// 不必delete因为基本类型不会产生新的
		}

		virtual Expr* Gen() = 0;
		virtual Expr* Reduce() = 0;
		virtual void Jumping( int t, int f );
		virtual void EmitJumps( const std::string& test, int t, int f );

		virtual Expr* Clone();

		virtual std::string ToString();

	};


	

	/*
	** 名字：Id
	** 说明：负责保存表达式中的标识符
	**
	*/
	class Id : public Expr
	{
	private:
		int					m_defPointerCount;			// 指针个数
		std::list<Expr*>	m_defArrayDimension;		// 定义的数组维数
		int					m_offset;					// 相对地址
		std::list<Expr*>	m_curArrayOpExpr;			// 当前变量的数组修饰
		int					m_pointercount;
		bool				m_recycle;					// 是否回收定义的数据

	public:

		Id( Word* id, Type* p ) : Expr( id, p ), m_defPointerCount( 0 ), m_offset( 0 ), m_pointercount( 0 ), m_recycle( true )
		{
		}

		Id( const Id& id );

		~Id()
		{
			std::for_each( m_curArrayOpExpr.begin(), m_curArrayOpExpr.end(), []( Expr* pExpr )
			{
				delete pExpr;
			});

			if ( m_recycle )
			{
				std::for_each( m_defArrayDimension.begin(), m_defArrayDimension.end(), []( Expr* pExpr )
				{
					delete pExpr;
				});
			}
		}

		void SetDefPointerUnrefCount( int count );
		int GetPointerDef();

		int GetPointerCur();
		void SetCurPointerUnrefCount( int count );

		void AddArrayDef( Expr* e );
		std::list<Expr*>& GetArrayDef();

		void AddArrayExpr( Expr* e );
		std::list<Expr*>& GetArrayExpr();
	

		Expr* Clone();

		Expr* Gen() 
		{
			return Clone();
		}

		Expr* Reduce() 
		{
			return Clone();
		}


	};


	/*
	** 名字：Temp
	** 说明：用于生成临时变量
	**
	*/
	class Temp : public Expr
	{
	public:
		static int count;
		static bool bAsm;

		int number;

		Temp( Type* pType ) : Expr( new Word( "#t", Tag::TEMP ), pType ), number( 0 )
		{
			number = ++count;
		}

		std::string ToString()
		{
			char code[ 20 ];
			sprintf_s( code, "#t%d", number );
			return code;
		}

		Expr* Gen()
		{
			return NULL;
		}

		Expr* Reduce() 
		{
			return NULL;
		}

	};

	class ExprSeq : public Expr
	{
	public:
		Expr*	m_pExpr1;
		Expr*	m_pExpr2;

		ExprSeq( Expr* e1, Expr* e2 ) : m_pExpr1( e1 ), m_pExpr2( e2 ), Expr( new Token( Tag::SEMI), e1->m_pType )
		{
		}

		Expr* Gen();

		Expr* Reduce()
		{
			return Clone();
		}

		Expr* Clone()
		{
			return new ExprSeq( m_pExpr1->Clone(), m_pExpr2->Clone() );
		}

	};


	/*
	** 名字：Operation
	** 说明：
	**
	*/
	class Operation : public Expr
	{
	public:
		Operation( Token* tok, Type* tp ) : Expr( tok, tp ) {}
		virtual ~Operation()	{}

		Expr* Reduce()
		{
			//Expr* x = Gen();
			//Temp* t = new Temp( m_pType );
			//Emit( t->ToString() + " = " + x->ToString() );
			//delete x;
			//return t;
			return Gen();
		}
	};
		
	
	/*
	** 名字：ArrayAccess
	** 说明：数组访问
	**
	*/
	class ArrayAccess : public Operation
	{
	public:
		Id		m_array;			
		Expr*	m_index;	// 下标
		bool	m_bRecycle;

		ArrayAccess( const Id& id, Expr* i, Type* p, bool bRec = true ) 
			: m_array( id ), m_index( i ), Operation( new Word( "[]", Tag::INDEX ), p ), m_bRecycle( bRec )
		{
		}

		~ArrayAccess()
		{
			if ( m_bRecycle )
			{
				delete m_index;
			}
		}

		void Jumping( int t, int f )
		{
			Expr* x = Reduce();
			EmitJumps( x->ToString(), t, f );
			delete x;
		}

		Expr* Gen()
		{	
			return new ArrayAccess( m_array, m_index->Reduce(), m_pType );
		}

		std::string ToString()
		{
			return m_array.ToString() + "[" + m_index->ToString() + "]";
		}
	};
	

	/*
	** 名字：CallExpr
	** 说明：函数调用
	**
	*/
	class CallExpr : public Expr
	{
	public:
		Id					m_id;
		std::list< Expr* >	m_params;
		bool				m_bDeleted;

		CallExpr( const Id& id, bool bDel = true ) : Expr( NULL, NULL ), m_id( id ), m_bDeleted( bDel )
		{
		}


		~CallExpr()
		{
			if ( m_bDeleted )
			{
				for ( auto iter = m_params.begin(); iter != m_params.end(); iter++ )
				{
					delete *iter;
				}
			}
		}

		void AddParam( Expr* param )
		{
			m_params.push_back( param );
		}

		Expr* Gen()
		{
			return Reduce();
		}

		Expr* Reduce();
	};

	/*
	** 名字：Arith
	** 说明：二元操作符。例如加减乘除。
	**		Token值保存操作符，两个表达式保存两边的操作符。
	**
	*/
	class Arith : public Operation
	{
	public:
		Expr*	m_pExpr1;
		Expr*	m_pExpr2;
		bool	m_bRecycle;	// 是否由它回收内存，这是个很纠结的问题，为了解决Gen()的问题。

		Arith( Token* tok, Expr* e1, Expr* e2, bool bRecy = true );

		~Arith()
		{
			if ( m_bRecycle )
			{
				SAFE_DELETE( m_pExpr1 );
				SAFE_DELETE( m_pExpr2 );
			}
		}

		Expr* Gen();

		std::string ToString()
		{
			if ( m_pTok->m_Tag == Tag::POINT || m_pTok->m_Tag == Tag::ARROW )
			{
				return m_pExpr1->ToString() + m_pTok->ToString() + m_pExpr2->ToString();
			}
			return m_pExpr1->ToString() + " " + m_pTok->ToString() + " " + m_pExpr2->ToString();
		}

	};


	/*
	** 名字：Ternary
	** 说明：? : 表达式
	**
	*/
	class Ternary : public Operation
	{
	public:
		Expr* m_logicExpr;
		Expr* m_Expr1;
		Expr* m_Expr2;

		Ternary( Token* tok, Expr* logic, Expr* e1, Expr* e2 ) 
			: Operation( tok, NULL ), m_logicExpr( logic ), m_Expr1( e1 ), m_Expr2( e2 )
		{

		}

		Expr* Gen()
		{
			return NULL;
		}

		Expr* Reduce() 
		{
			return NULL;
		}
	};


	/*
	** 名字：Unary
	** 说明：一元操作符。例如负号。
	**
	*/
	class Unary : public Operation
	{
	public:
		Expr*	m_pExpr;
		bool	m_bRecycle;	// 是否由它回收内存，这是个很纠结的问题，为了解决Gen()的问题。

		Unary( Token* tok, Expr* exp, bool bre = true );

		~Unary()
		{
			if ( m_bRecycle ) SAFE_DELETE( m_pExpr );
		}

		Expr* Gen();

		std::string ToString()
		{
			return m_pTok->ToString() + " " + m_pExpr->ToString();
		}

	};




	/*
	** 名字：Constant
	** 说明：常量
	**
	*/
	class Constant : public Expr
	{
	public:
		Constant( Token* tok, Type* tp ) : Expr( tok, tp ) 
		{
		}

		Constant( int i ) : Expr( new Num( i ), Type::Int )
		{		
		}

		//Constant( const CETString& str ) : Expr( new )

		static Constant *True, *False;

		void Jumping( int t, int f );

		Expr* Clone();

		Expr* Gen()
		{
			return Clone();
		}

		Expr* Reduce() 
		{
			return Clone();
		}

	};


	/*
	** 名字：Logical
	** 说明：逻辑运算符。用于被继承。
	**
	*/
	class Logical : public Expr
	{
	public:
		Expr* m_pExpr1;
		Expr* m_pExpr2;

		Logical( Token* tok, Expr* e1, Expr* e2 ) : Expr( tok, NULL ), m_pExpr1( e1 ), m_pExpr2( e2 )
		{
			// type check

			m_pType = e1->m_pType;
		}

		virtual ~Logical()
		{
			SAFE_DELETE( m_pExpr1 );
			SAFE_DELETE( m_pExpr2 );
		}

		Expr* Gen();



	};


	/*
	** 名字：OrOperator
	** 说明：或运算的操作符。
	**
	*/
	class OrOperator : public Logical
	{
	public:
		OrOperator( Token* tok, Expr* e1, Expr* e2 ) : Logical( tok, e1, e2 )
		{

		}

		void Jumping( int t, int f );

		Expr* Clone();

		Expr* Reduce() 
		{
			return Clone();
		}


	};   


	/*
	** 名字：AndOperator
	** 说明：与运算操作符。
	**
	*/
	class AndOperator : public Logical
	{
	public:
		AndOperator( Token* tok, Expr* e1, Expr* e2 ) : Logical( tok, e1, e2 )
		{

		}

		void Jumping( int t, int f );

		Expr* Clone();

		Expr* Reduce() 
		{
			return Clone();
		}
	};


	/*
	** 名字：NotOperator
	** 说明：非运算操作符。
	**
	*/
	class NotOperator : public Logical
	{
	public:
		NotOperator( Token* tok, Expr* e ) : Logical( tok, e, NULL )
		{

		}

		void Jumping( int t, int f )
		{
			m_pExpr1->Jumping( f, t );
		}

		std::string ToString()
		{
			return m_pTok->ToString() + " " + m_pExpr1->ToString();
		}

		Expr* Clone();

		Expr* Reduce() 
		{
			return Clone();
		}
	};


	/*
	** 名字：Relation
	** 说明：关系操作符。> < >= <= 之类的操作符。
	**
	*/
	class Relation : public Logical
	{
	public:
		Relation( Token* tok, Expr* e1, Expr* e2 ) : Logical( tok, e1, e2 )
		{

		}

		void Jumping( int t, int f );

		Expr* Clone();

		Expr* Reduce() 
		{
			return Clone();
		}
	};



	//class Access : public Operation
	//{
	//
	//};







	/************************************************************************/
	/*					                                                    */
	/*					语句节点定义                                         */
	/*																	    */
	/************************************************************************/


	/*
	** 名字：Stmt
	** 说明：语句
	**
	*/
	class Stmt : public TreeNode
	{
	public:
		static Stmt* m_pEnclosing;
		static Stmt* m_pNullStmt;
		int m_after;

		Stmt() : m_after( 0 ) {} 
		virtual ~Stmt() {}

		virtual void Gen( int b, int a, bool bASM ) = 0;

		void GenMovLineno( int lineno );
	};


	/*
	** 名字：NullStmt
	** 说明：空语句，仅用于比较，可以无视它
	**
	*/
	class NullStmt : public Stmt
	{
		void Gen( int b, int a, bool bASM ) {}
	};


	/*
	** 名字：
	** 说明：
	**
	*/
	class CallStmt : public Stmt
	{
	public:
		std::list< Expr* >	m_args;
		Type*				m_pType;
		Id					m_id;

		CallStmt( const Id& id ) : m_id( id )
		{
		}

		//CallStmt( Type* type ) : m_pType( type ) {}

		void AddArg( Expr* expr )
		{
			m_args.push_back( expr );
		}

		void Gen( int b, int a, bool bASM );
	};

	class ReadStmt : public Stmt
	{
	public:
		Expr* m_pExpr;

		ReadStmt( Expr* pExpr ) : m_pExpr( pExpr )
		{
		}

		void Gen( int b, int a, bool bASM );

	};

	class WriteStmt : public Stmt
	{
	public:
		Expr* m_pExpr;

		WriteStmt( Expr* pExpr ) : m_pExpr( pExpr )
		{
		}

		void Gen( int b, int a, bool bASM );

	};


	/*
	** 名字：IfStmt
	** 说明：保存If语句
	**
	*/
	class IfStmt : public Stmt
	{
	public:
		Expr* m_pExpr;
		Stmt* m_pStmt;

		IfStmt( Expr* expr, Stmt* stmt ) : m_pExpr( expr ), m_pStmt( stmt ) {}
		~IfStmt()
		{
			SAFE_DELETE( m_pExpr );
			SAFE_DELETE( m_pStmt );
		}

		void Gen( int b, int a, bool bASM );
	};

	/*
	** 名字：IfElseStmt
	** 说明：保存if-else结构的语句
	**
	*/
	class IfElseStmt : public Stmt
	{
	public:
		Expr* m_pExpr;
		Stmt* m_pIfStmt;
		Stmt* m_pElseStmt;

		IfElseStmt( Expr* expr, Stmt* ifstmt, Stmt* elsestmt ) 
			: m_pExpr( expr ), m_pIfStmt( ifstmt ), m_pElseStmt( elsestmt )
		{
		}
		~IfElseStmt()
		{
			SAFE_DELETE( m_pExpr );
			SAFE_DELETE( m_pIfStmt );
			SAFE_DELETE( m_pElseStmt );
		}

		void Gen( int b, int a, bool bASM );
	};


	/*
	** 名字：WhileStmt
	** 说明：while循环
	**
	*/
	class WhileStmt : public Stmt
	{
	public:
		Expr* m_pExpr;
		Stmt* m_pStmt;

		WhileStmt() : m_pStmt( NULL ), m_pExpr( NULL ) {}
		~WhileStmt()
		{
			SAFE_DELETE( m_pExpr );
			SAFE_DELETE( m_pStmt );
		}

		void Init( Expr* expr, Stmt* stmt );

		void Gen( int b, int a, bool bASM );
	};

	
	/*
	** 名字：ForStmt
	** 说明：for语句
	**
	*/
	class ForStmt : public Stmt
	{
	public:
		Stmt* m_pInitStmt;		// 初始化语句
		Expr* m_pLogicExpr;		// 结束语句
		Stmt* m_pIterStmt;		// 每次循环后都会执行的语句
		Stmt* m_pStmt;			// for下面的语句
	
		ForStmt( Stmt* pInit, Expr* pExpr, Stmt* pIterStmt, Stmt* pStmt )
			: m_pInitStmt( pInit ), m_pLogicExpr( pExpr ), m_pIterStmt( pIterStmt ), m_pStmt( pStmt )
		{
	
		}

		void Gen( int b, int a, bool bASM )
		{

		}
	
	};


	///*
	//** 名字：SwitchStmt
	//** 说明：先忽略
	//**
	//*/
	//class SwitchStmt : public Stmt
	//{
	//public:
	//	Id	m_id;
	//	std::list< Num* > m_caseList;
	//};


	/*
	** 名字：BreakStmt
	** 说明：break语句
	**
	*/
	class BreakStmt : public Stmt
	{
	public:
		Stmt* m_pStmt;
		BreakStmt()
		{
			if ( Stmt::m_pEnclosing == NULL ) throw "unenclosed break";

			m_pStmt = Stmt::m_pEnclosing;
		}

		~BreakStmt()
		{
			SAFE_DELETE( m_pStmt );
		}

		void Gen( int b, int a, bool bASM );
	};


	/*
	** 名字：ReturnStmt
	** 说明：返回
	**
	*/
	class ReturnStmt : public Stmt
	{
	public:
		Expr*	m_pExpr;

		ReturnStmt( Expr* pExpr ) : m_pExpr( pExpr )
		{

		}

		void Gen( int b, int a, bool bASM );
	};


	/*
	** 名字：StmtSeq
	** 说明：语句序列，负责连接兄弟关系的语句
	**
	*/
	class StmtSeq : public Stmt
	{
	public:
		Stmt* m_pStmt1;
		Stmt* m_pStmt2;

		StmtSeq( Stmt* s1, Stmt* s2 ) : m_pStmt1( s1 ), m_pStmt2( s2 ) {}
		~StmtSeq()
		{
			SAFE_DELETE( m_pStmt1 );
			SAFE_DELETE( m_pStmt2 );
		}

		void Gen( int b, int a, bool bASM );

	};


	class ExprStmt : public Stmt
	{
	public:
		Expr*	m_pExpr;

		ExprStmt( Expr* expr ) : m_pExpr( expr )
		{

		}

		void Gen( int b, int a, bool bASM );


	};


	/*
	** 名字：SetStmt
	** 说明：赋值语句。左边为标识符，右边为表达式
	**
	*/
	class SetStmt : public Stmt
	{
	public:
		Id		m_id;
		Expr*	m_pExpr;

		SetStmt( const Id& id, Expr* expr ) : m_id( id ), m_pExpr( expr )
		{
		}

		~SetStmt()
		{
			SAFE_DELETE( m_pExpr );
		}

		void Gen( int b, int a, bool bASM )
		{
			if ( bASM ) GenMovLineno( m_lineno );

			Expr* pExpr = m_pExpr->Gen();
			Temp* temp = new Temp( m_pExpr->m_pType );

			Emit( temp->ToString() + " = " + pExpr->ToString() );
			Emit( m_id.ToString() + " = " + temp->ToString() );
			delete temp;
			delete pExpr;
		}
	};



	class SetElemStmt : public Stmt
	{
	public:
		Id		m_array;
		Expr*	m_pIndex;
		Expr*	m_pExpr;

		SetElemStmt( ArrayAccess* i, Expr* expr )
			: m_array( i->m_array ), m_pIndex( i->m_index ), m_pExpr( expr )
		{
			if ( check( i->m_pType, expr->m_pType ) == NULL )
			{
				Error( "type Error" );
			}
		}

		Type* check( Type* p1, Type* p2 )
		{
			if ( typeid( *p1 ) == typeid( Array ) || typeid( *p2 ) == typeid( Array) ) return NULL;
			else if ( p1 == p2 )	return p2;
			else if ( Type::Numeric( p1 ) && Type::Numeric( p2 ) )	return p2;
			
			return NULL;
		}

		void Gen( int b, int a, bool bASM )
		{
			if ( bASM ) GenMovLineno( m_lineno );

			Expr* p1 = m_pIndex->Reduce();
			std::string s1 = p1->ToString();
			Expr* p2 = m_pExpr->Reduce();
			std::string s2 = p2->ToString();

			Emit( m_array.ToString() + "[" + s1 + "] = " + s2 );

			delete p1;
			delete p2;
		}
	};



	/*
	** 名字：DeclareStmt
	** 说明：变量声明节点
	**
	*/
	class DeclareStmt : public Stmt
	{
	public:
		std::list< SetStmt* >	m_setStmtList;
		Type*					m_type;

		DeclareStmt( Type* type ) : m_type( type ) {}

		void AddSetStmt( SetStmt* pStmt )
		{
			m_setStmtList.push_back( pStmt );
		}

		void Gen( int b, int a, bool bASM ) {}
	};


	/*
	** 名字：FuncDeclStmt
	** 说明：函数声明节点
	**
	*/
	//class FuncDeclStmt : public Stmt
	//{
	//public:
	//	struct FunParamNode
	//	{
	//		Type		type;
	//		SetStmt*	pSet;
	//	};
	//
	//public:
	//	Type			m_type;
	//	Id				m_id;
	//
	//	std::list< FunParamNode > m_typeparamList;
	//
	//	//FuncDeclStmt() : m_type( ... ), m_id( ... )
	//	//{
	//	//}
	//
	//	void AddTypeSetStmt( const FunParamNode& paramNode )
	//	{
	//		m_typeparamList.push_back( paramNode );
	//	}
	//
	//};

}