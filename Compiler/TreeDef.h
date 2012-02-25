/*
** ���ߣ�����
** ˵����
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
	** ���֣�AssembyCodeGen
	** ˵������������м����
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
	** ���֣�TreeNode
	** ˵�����﷨���ڵ���ࡣ
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
	/*					���ʽ�ڵ㶨��                                       */
	/*																	    */
	/************************************************************************/


	/*
	** ���֣�Expr
	** ˵�������ʽ�ڵ�
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
			//delete m_pType;	// ����delete��Ϊ�������Ͳ�������µ�
		}

		virtual Expr* Gen() = 0;
		virtual Expr* Reduce() = 0;
		virtual void Jumping( int t, int f );
		virtual void EmitJumps( const std::string& test, int t, int f );

		virtual Expr* Clone();

		virtual std::string ToString();

	};


	

	/*
	** ���֣�Id
	** ˵�������𱣴���ʽ�еı�ʶ��
	**
	*/
	class Id : public Expr
	{
	private:
		int					m_defPointerCount;			// ָ�����
		std::list<Expr*>	m_defArrayDimension;		// ���������ά��
		int					m_offset;					// ��Ե�ַ
		std::list<Expr*>	m_curArrayOpExpr;			// ��ǰ��������������
		int					m_pointercount;
		bool				m_recycle;					// �Ƿ���ն��������

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
	** ���֣�Temp
	** ˵��������������ʱ����
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
	** ���֣�Operation
	** ˵����
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
	** ���֣�ArrayAccess
	** ˵�����������
	**
	*/
	class ArrayAccess : public Operation
	{
	public:
		Id		m_array;			
		Expr*	m_index;	// �±�
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
	** ���֣�CallExpr
	** ˵������������
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
	** ���֣�Arith
	** ˵������Ԫ������������Ӽ��˳���
	**		Tokenֵ������������������ʽ�������ߵĲ�������
	**
	*/
	class Arith : public Operation
	{
	public:
		Expr*	m_pExpr1;
		Expr*	m_pExpr2;
		bool	m_bRecycle;	// �Ƿ����������ڴ棬���Ǹ��ܾ�������⣬Ϊ�˽��Gen()�����⡣

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
	** ���֣�Ternary
	** ˵����? : ���ʽ
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
	** ���֣�Unary
	** ˵����һԪ�����������縺�š�
	**
	*/
	class Unary : public Operation
	{
	public:
		Expr*	m_pExpr;
		bool	m_bRecycle;	// �Ƿ����������ڴ棬���Ǹ��ܾ�������⣬Ϊ�˽��Gen()�����⡣

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
	** ���֣�Constant
	** ˵��������
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
	** ���֣�Logical
	** ˵�����߼�����������ڱ��̳С�
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
	** ���֣�OrOperator
	** ˵����������Ĳ�������
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
	** ���֣�AndOperator
	** ˵�����������������
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
	** ���֣�NotOperator
	** ˵�����������������
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
	** ���֣�Relation
	** ˵������ϵ��������> < >= <= ֮��Ĳ�������
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
	/*					���ڵ㶨��                                         */
	/*																	    */
	/************************************************************************/


	/*
	** ���֣�Stmt
	** ˵�������
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
	** ���֣�NullStmt
	** ˵��������䣬�����ڱȽϣ�����������
	**
	*/
	class NullStmt : public Stmt
	{
		void Gen( int b, int a, bool bASM ) {}
	};


	/*
	** ���֣�
	** ˵����
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
	** ���֣�IfStmt
	** ˵��������If���
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
	** ���֣�IfElseStmt
	** ˵��������if-else�ṹ�����
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
	** ���֣�WhileStmt
	** ˵����whileѭ��
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
	** ���֣�ForStmt
	** ˵����for���
	**
	*/
	class ForStmt : public Stmt
	{
	public:
		Stmt* m_pInitStmt;		// ��ʼ�����
		Expr* m_pLogicExpr;		// �������
		Stmt* m_pIterStmt;		// ÿ��ѭ���󶼻�ִ�е����
		Stmt* m_pStmt;			// for��������
	
		ForStmt( Stmt* pInit, Expr* pExpr, Stmt* pIterStmt, Stmt* pStmt )
			: m_pInitStmt( pInit ), m_pLogicExpr( pExpr ), m_pIterStmt( pIterStmt ), m_pStmt( pStmt )
		{
	
		}

		void Gen( int b, int a, bool bASM )
		{

		}
	
	};


	///*
	//** ���֣�SwitchStmt
	//** ˵�����Ⱥ���
	//**
	//*/
	//class SwitchStmt : public Stmt
	//{
	//public:
	//	Id	m_id;
	//	std::list< Num* > m_caseList;
	//};


	/*
	** ���֣�BreakStmt
	** ˵����break���
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
	** ���֣�ReturnStmt
	** ˵��������
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
	** ���֣�StmtSeq
	** ˵����������У����������ֵܹ�ϵ�����
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
	** ���֣�SetStmt
	** ˵������ֵ��䡣���Ϊ��ʶ�����ұ�Ϊ���ʽ
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
	** ���֣�DeclareStmt
	** ˵�������������ڵ�
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
	** ���֣�FuncDeclStmt
	** ˵�������������ڵ�
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