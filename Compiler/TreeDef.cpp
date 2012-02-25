/*
** 作者：华亮
** 说明：
**
*/

#include "stdafx.h"
#include "TreeDef.h"
#include "Scanner.h"
#include "parser.h"


namespace ETCompiler
{
	//////////////////////////////////////////////////////////////////////////
	//
	// Class AssembyCodeGen
	// 
	IParser* AssembyCodeGen::parser = NULL;
	bool AssembyCodeGen::GenCode( const char* code )
	{
		if ( parser )
		{
			parser->InsertCode( code );
			return true;
		}
		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	// Class Constant
	// 
	Constant* Constant::True = new Constant( new Word( "true", Tag::TRUETYPE ), Type::Bool );
	Constant* Constant::False = new Constant( new Word( "false", Tag::FALSETYPE ), Type::Bool );


	//////////////////////////////////////////////////////////////////////////
	//
	// Class Stmt
	// 
	Stmt* Stmt::m_pEnclosing = NULL;	// used for break
	Stmt* Stmt::m_pNullStmt = new NullStmt;


	//////////////////////////////////////////////////////////////////////////
	//
	// Class Temp
	// 
	int Temp::count = 0;
	bool Temp::bAsm = false;

	//////////////////////////////////////////////////////////////////////////
	//
	// Class TreeNode
	// 
	int TreeNode::m_labels = 0;

	//////////////////////////////////////////////////////////////////////////
	//
	// Class LinenoSupplier
	// 
	IScanner* LinenoSupplier::pScanner = NULL;
	int LinenoSupplier::GetLineno()
	{
		if ( pScanner )	return pScanner->GetLineno();
		return -1;
	}
	

	//////////////////////////////////////////////////////////////////////////
	//
	// Class Unary
	// 
	Expr* Unary::Gen()
	{
		if ( m_pTok->m_Tag == Tag::TIMES )
		{
			Expr* exp = m_pExpr->Reduce();
			Temp temp( exp->m_pType );
			Emit( temp.ToString() + " = " + exp->ToString() );

			Temp* t = new Temp( exp->m_pType );
			Emit( t->ToString() + " = " + m_pTok->ToString() + " " + temp.ToString() );

			delete exp;
			return t;
		}
		return new Unary( m_pTok->Clone(), m_pExpr->Reduce() );
	}

	Unary::Unary( Token* tok, Expr* exp, bool bre /*= true */ ) : Operation( tok, NULL ), m_pExpr( exp ), m_bRecycle( bre )
	{
		// max type
		m_pType = Type::MaxType( Type::Int, exp->m_pType );
		if ( m_pType == NULL )
		{
			Error( "type Error! in class Unary" );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class Expr
	// 
	void Expr::Jumping( int t, int f )
	{
		EmitJumps( m_pTok->ToString(), t, f );
	}

	void Expr::EmitJumps( const std::string& test, int t, int f )
	{
		char code[ 64 ];
		if ( t != -1 && f != -1 )
		{
			sprintf_s( code, "if %s goto L%d", test.c_str(), t );
			Emit( code );
			sprintf_s( code, "goto L%d", t );
			Emit( code );
		}
		else if ( t != -1 )
		{
			sprintf_s( code, "if %s goto L%d", test.c_str(), t );
			Emit( code );
		}
		else if ( f != -1 )
		{
			sprintf_s( code, "iffalse %s goto L%d", test.c_str(), f );
			Emit( code );
		}
	}

	Expr* Expr::Clone()
	{
		return NULL;
	}

	std::string Expr::ToString()
	{
		return m_pTok->ToString();
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class Id
	// 
	Id::Id( const Id& id ) : Expr( NULL, NULL ), m_recycle( false )
	{
		*this = id;

		m_pType = (Type*)id.m_pType->Clone();
		m_pTok = id.m_pTok->Clone();

		m_curArrayOpExpr.clear();
		for ( auto iter = id.m_curArrayOpExpr.begin(); iter != id.m_curArrayOpExpr.end(); iter++ )
		{
			m_curArrayOpExpr.push_back( (*iter)->Clone() );
		}
	}

	void Id::SetDefPointerUnrefCount( int count )
	{
		m_defPointerCount = count;
	}

	int Id::GetPointerDef()
	{
		return m_defPointerCount;
	}

	int Id::GetPointerCur()
	{
		return m_pointercount;
	}

	void Id::SetCurPointerUnrefCount( int count )
	{
		m_pointercount = count;
	}

	void Id::AddArrayDef( Expr* e )
	{
		m_defArrayDimension.push_back( e );
	}

	std::list<Expr*>& Id::GetArrayDef()
	{
		return m_defArrayDimension;
	}

	void Id::AddArrayExpr( Expr* e )
	{
		m_curArrayOpExpr.push_back( e );
	}

	std::list<Expr*>& Id::GetArrayExpr()
	{
		return m_curArrayOpExpr;
	}

	Expr* Id::Clone()
	{
		Id* pId = new Id( *this );
		pId->m_pType = (Type*)m_pType->Clone();
		pId->m_pTok = m_pTok->Clone();

		pId->m_curArrayOpExpr.clear();
		for ( auto iter = m_curArrayOpExpr.begin(); iter != m_curArrayOpExpr.end(); iter++ )
		{
			pId->m_curArrayOpExpr.push_back( (*iter)->Clone() );
		}

		return pId;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class ExprSeq
	// 
	Expr* ExprSeq::Gen()
	{
		Expr* p1 = m_pExpr1->Reduce();
		//Temp temp1( m_pExpr1->m_pType );
		//Emit( p1->ToString() );
		if ( m_pExpr2 )
		{
			Expr* p2 = m_pExpr2->Reduce();
			//Temp temp2( m_pExpr1->m_pType );
			//Emit(  p2->ToString() );
			delete p2;
		}

		return p1;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class CallExpr
	// 
	Expr* CallExpr::Reduce()
	{
		Emit( "push r0" );	

		for ( auto arg = m_params.rbegin(); arg != m_params.rend(); arg++ )
		{
			Expr* pExpr = (*arg)->Gen();
			Temp* temp = new Temp( (*arg)->m_pType );
			Emit( temp->ToString() + " = " + pExpr->ToString() );
			Emit( "arg " + temp->ToString() );

			delete temp;
			delete pExpr;
		}

		Emit( "call " + m_id.ToString() );

		for ( std::size_t i = 0; i < m_params.size(); i++ )
		{
			Emit( "pop r0" );
		}

		Temp* temp = new Temp( m_pType );
		Emit( "pop " + temp->ToString() );

		return temp;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class Arith
	// 
	Arith::Arith( Token* tok, Expr* e1, Expr* e2, bool bRecy /*= true */ ) : Operation( tok, NULL ), m_pExpr1( e1 ), m_pExpr2( e2 ), m_bRecycle( bRecy )
	{
		m_pType = Type::MaxType( m_pExpr1->m_pType, m_pExpr2->m_pType );

		if ( m_pType == NULL )		Error( "Type Error" );
	}

	Expr* Arith::Gen()
	{
		if ( typeid( *m_pExpr1 ) == typeid( ArrayAccess ) && m_pTok->m_Tag == Tag::ASSIGN )
		{
			Expr* p1 = dynamic_cast<ArrayAccess*>( m_pExpr1 )->m_index->Reduce();
			std::string s1 = p1->ToString();
			Expr* p2 = m_pExpr2->Reduce();
			std::string s2 = p2->ToString();

			Emit( dynamic_cast<ArrayAccess*>( m_pExpr1 )->m_array.ToString() + "[" + s1 + "] = " + s2 );

			Temp* temp = new Temp( m_pType );
			Emit( temp->ToString() + " = " + dynamic_cast<ArrayAccess*>( m_pExpr1 )->m_array.ToString() + "[" + s1 + "]" );

			delete p1;
			delete p2;

			return temp;
		}
		else if ( m_pTok->m_Tag == Tag::ASSIGN )
		{
			Arith* exp = new Arith( m_pTok->Clone(), m_pExpr1->Reduce(), m_pExpr2->Reduce(), false );
			Emit( exp->ToString() );
			Temp* temp = new Temp( m_pType );
			Emit( temp->ToString() + " " + m_pTok->ToString() + " " + m_pExpr1->ToString() );
			delete exp;
			return temp;
		}
		else if ( m_pTok->m_Tag == Tag::POINT || m_pTok->m_Tag == Tag::ARROW )
		{
			Arith* exp = new Arith( m_pTok->Clone(), m_pExpr1->Reduce(), m_pExpr2->Reduce(), false );
			return exp;
		}
		else
		{
			Expr* exp = new Arith( m_pTok->Clone(), m_pExpr1->Reduce(), m_pExpr2->Reduce(), false );
			Temp* temp = new Temp( m_pType );
			Emit( temp->ToString() + " = " + exp->ToString() );
			delete exp;
			return temp;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class Constant
	// 
	void Constant::Jumping( int t, int f )
	{
		char code[ 20 ];
		if ( m_pTok->m_Tag == Tag::TRUETYPE && t != 0 )
		{
			sprintf_s( code, "goto L%d", t );
			Emit( code );
		}
		else if ( m_pTok->m_Tag == Tag::FALSETYPE && f != 0 )
		{
			sprintf_s( code, "goto L%d", f );
			Emit( code );
		}
	}

	Expr* Constant::Clone()
	{
		Constant* pConst = new Constant( *this );
		pConst->m_pTok = m_pTok->Clone();
		pConst->m_pType = (Type*)m_pType->Clone();

		return pConst;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class Logical
	// 
	Expr* Logical::Gen()
	{
		int f = NewLabel();
		int a = NewLabel();

		Temp* temp = new Temp( m_pType );
		Jumping( -1, f );

		char code[ 30 ];

		Emit( temp->ToString() + " = true" );
		sprintf_s( code, "goto L%d", a );
		Emit( code );
		EmitLabel( f );
		sprintf_s( code, "goto L%d", a );
		EmitLabel( a );

		return temp;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class OrOperator
	// 
	void OrOperator::Jumping( int t, int f )
	{
		int label = t != -1 ? t : NewLabel();
		m_pExpr1->Jumping( label, -1 );
		m_pExpr2->Jumping( t, f );
		if ( t == -1 ) EmitLabel( label );
	}

	Expr* OrOperator::Clone()
	{
		OrOperator* pOR = new OrOperator( *this );
		pOR->m_pTok = m_pTok->Clone();
		pOR->m_pType = (Type*)m_pType->Clone();
		pOR->m_pExpr1 = m_pExpr1->Clone();
		pOR->m_pExpr2 = m_pExpr2->Clone();

		return pOR;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class AndOperator
	// 
	void AndOperator::Jumping( int t, int f )
	{
		int label = f != -1 ? f : NewLabel();
		m_pExpr1->Jumping( -1, label );
		m_pExpr2->Jumping( t, f );
		if ( f == -1 ) EmitLabel( label );
	}

	Expr* AndOperator::Clone()
	{
		AndOperator* pAnd = new AndOperator( *this );
		pAnd->m_pTok = m_pTok->Clone();
		pAnd->m_pType = (Type*)m_pType->Clone();
		pAnd->m_pExpr1 = m_pExpr1->Clone();
		pAnd->m_pExpr2 = m_pExpr2->Clone();

		return pAnd;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class NotOperator
	// 
	Expr* NotOperator::Clone()
	{
		NotOperator* pNot = new NotOperator( *this );
		pNot->m_pTok = m_pTok->Clone();
		pNot->m_pType = (Type*)m_pType->Clone();
		pNot->m_pExpr1 = m_pExpr1->Clone();
		pNot->m_pExpr2 = m_pExpr2->Clone();

		return pNot;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class Relation
	// 
	void Relation::Jumping( int t, int f )
	{
		Expr* ap = m_pExpr1->Reduce();
		Expr* bp = m_pExpr2->Reduce();

		std::string test = ap->ToString() + " " + m_pTok->ToString() + " " + bp->ToString();

		EmitJumps( test, t, f );
	}

	Expr* Relation::Clone()
	{
		Relation* pRel = new Relation( *this );

		pRel->m_pTok = m_pTok->Clone();
		pRel->m_pType = (Type*)m_pType->Clone();
		pRel->m_pExpr1 = m_pExpr1->Clone();
		pRel->m_pExpr2 = m_pExpr2->Clone();

		return pRel;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class Stmt
	// 
	void Stmt::GenMovLineno( int lineno )
	{
		char code[ 30 ];
		sprintf_s( code, "Lineno %d", lineno );
		Emit( code );
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class CallStmt
	// 
	void CallStmt::Gen( int b, int a, bool bASM )
	{
		if ( bASM ) GenMovLineno( m_lineno );

		for ( auto arg = m_args.rbegin(); arg != m_args.rend(); arg++ )
		{
			Expr* pExpr = (*arg)->Gen();
			Temp* temp = new Temp( (*arg)->m_pType );
			Emit( temp->ToString() + " = " + pExpr->ToString() );
			Emit( "arg " + temp->ToString() );

			delete temp;
			delete pExpr;
		}

		Emit( "call " + m_id.ToString() );

		for ( std::size_t i = 0; i < m_args.size(); i++ )
		{
			Emit( "pop r0" );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class ReadStmt
	// 
	void ReadStmt::Gen( int b, int a, bool bASM )
	{
		if ( bASM ) GenMovLineno( m_lineno );

		Expr* pExpr = m_pExpr->Gen();
		Temp temp( m_pExpr->m_pType );

		Emit( temp.ToString() + " = " + pExpr->ToString() );
		Emit( "read " + temp.ToString() );
		delete pExpr;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class WriteStmt
	// 
	void WriteStmt::Gen( int b, int a, bool bASM )
	{
		if ( bASM ) GenMovLineno( m_lineno );

		if ( typeid( *m_pExpr ) == typeid( Constant ) )
		{
			Emit( "write " + itoa( dynamic_cast<Num*>(dynamic_cast<Constant*>(m_pExpr)->m_pTok)->m_Val ) );
		}
		else
		{
			Expr* pExpr = m_pExpr->Gen();
			Temp temp( m_pExpr->m_pType );

			Emit( temp.ToString() + " = " + pExpr->ToString() );
			Emit( "write " + temp.ToString() );
			delete pExpr;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class IfStmt
	// 
	void IfStmt::Gen( int b, int a, bool bASM )
	{
		if ( bASM ) GenMovLineno( m_lineno );

		int label = NewLabel();
		m_pExpr->Jumping( -1, a );
		if ( m_pStmt ) 
		{			
			EmitLabel( label );
			m_pStmt->Gen( label, a, bASM );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class IfElseStmt
	// 
	void IfElseStmt::Gen( int b, int a, bool bASM )
	{
		if ( bASM ) GenMovLineno( m_lineno );

		int label1 = NewLabel();
		int label2 = NewLabel();

		m_pExpr->Jumping( -1, label2 );

		if ( m_pIfStmt )
		{
			EmitLabel( label1 );
			m_pIfStmt->Gen( label1, a, bASM );
		}

		char code[ 10 ];
		sprintf_s( code, "goto L%d", a );
		Emit( code );

		if ( m_pElseStmt ) 
		{
			EmitLabel( label2 );
			m_pElseStmt->Gen( label2, a, bASM );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class WhileStmt
	// 
	void WhileStmt::Init( Expr* expr, Stmt* stmt )
	{
		m_pExpr = expr;
		m_pStmt = stmt;
		// type check
	}

	void WhileStmt::Gen( int b, int a, bool bASM )
	{
		if ( bASM ) GenMovLineno( m_lineno );

		m_after = a;
		m_pExpr->Jumping( -1, a );			

		if ( m_pStmt )
		{
			int label = NewLabel();
			EmitLabel( label );
			m_pStmt->Gen( label, b, bASM );
		}

		char code[ 10 ];
		sprintf_s( code, "goto L%d", b );
		Emit( code );
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class BreakStmt
	// 
	void BreakStmt::Gen( int b, int a, bool bASM )
	{
		if ( bASM ) GenMovLineno( m_lineno );

		char code[ 10 ];
		sprintf_s( code, "goto L%d", m_pStmt->m_after );
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class ReturnStmt
	// 
	void ReturnStmt::Gen( int b, int a, bool bASM )
	{
		if ( bASM ) GenMovLineno( m_lineno );

		Expr* pExpr = m_pExpr->Gen();
		Temp* temp = new Temp( m_pExpr->m_pType );

		Emit( temp->ToString() + " = " + pExpr->ToString() );

		Emit( "leave " + temp->ToString() );

		delete temp;
		delete pExpr;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class StmtSeq
	// 
	void StmtSeq::Gen( int b, int a, bool bASM )
	{
		if ( m_pStmt1 == Stmt::m_pNullStmt && m_pStmt2 )	m_pStmt2->Gen( b, a, bASM );
		else if ( m_pStmt2 == Stmt::m_pNullStmt || m_pStmt2 == NULL && m_pStmt1 ) m_pStmt1->Gen( b, a, bASM );
		else if ( m_pStmt2 )
		{
			int label = NewLabel();
			m_pStmt1->Gen( b, label, bASM );
			EmitLabel( label );
			m_pStmt2->Gen( label, a, bASM );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Class ExprStmt
	// 
	void ExprStmt::Gen( int b, int a, bool bASM )
	{
		if ( bASM ) GenMovLineno( m_lineno );

		Expr* pExpr = m_pExpr->Gen();
		if ( pExpr && pExpr->ToString().find( "=" ) != std::string::npos )
		{
			Emit( pExpr->ToString() );

			delete pExpr;	
		}
	}

}