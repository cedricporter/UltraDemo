/*
** 作者：华亮，罗嘉飞
** 说明：
**
*/

#include "stdafx.h"
#include "Parser.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace ETCompiler
{

	void ETCompiler::CParser::Match( Tag::TagEnum tag )
	{
		if ( m_pToken->m_Tag == tag )
		{
			Move();
			// delete
		}
		else
		{
			Tag::TagEnum tellMeWhat = (Tag::TagEnum)m_pToken->m_Tag;
			// Error report
			char errorStr[ 50 ];
			sprintf_s( errorStr, "Match Error Token at lineno: %d", m_pToken->m_locateLine );
			throw errorStr;
		}
	}

	void CParser::Move()
	{
		m_pToken = m_pScanner->GetToken();
		m_tokenList.push_back( m_pToken );
	}

	CParser::~CParser()
	{
		//SAFE_DELETE( m_pScanner );

		Reset();
	}

	void ETCompiler::CParser::program( bool bASM )
	{
		m_pToken = m_pScanner->GetToken();

		m_pRoot = external_declaration( bASM );

		//// Print Syntax Tree
		//for ( auto iter = m_funStmtMap.begin(); iter != m_funStmtMap.end(); iter ++ )
		//{
		//	Log( iter->first );
		//	PrintTree( iter->second );
		//	//MessageBox( NULL, m_printTreeLog.c_str(), "Tree", MB_OK );
		//	//m_printTreeLog.clear();
		//}

		//std::ofstream out( "tree.txt" );
		//out << m_printTreeLog;
		//m_printTreeLog.clear();

		//for ( auto iter = m_funStmtMap.begin(); iter != m_funStmtMap.end(); iter ++ )
		//{
		//	AssembyCodeGen::GenCode( ("entry " + iter->first + '\n').c_str() );
		//	AssembyCodeGen::GenCode( ("mov d1, " + itoa( m_tableList.GetTable( iter->first )->GetID() ) + '\n').c_str() );

		//	int begin = iter->second->NewLabel();
		//	int after = iter->second->NewLabel();
		//	iter->second->EmitLabel( begin );
		//	iter->second->Gen( begin, after, bASM );
		//	iter->second->EmitLabel( after );

		//	delete iter->second;
		//}
		//m_funStmtMap.clear();
	}



	Stmt* CParser::external_declaration( bool bASM )
	{
		int func_id = 0;
		while ( 1 )
		{
			if ( m_pToken->m_Tag == Tag::BASIC )	
			{
				Type* tp = type();

				Token* pToken = m_pToken;
				Match( Tag::ID );
				Word* wordid = dynamic_cast<Word*>(pToken);
				Id* id = new Id( wordid, tp );
				m_env.Put( wordid->m_lexeme, id, true );

				if ( m_pToken->m_Tag == Tag::LPAREN )	// Func_definitions
				{
					m_tableList.AddTable( wordid->m_lexeme );
					m_tableList.GetTable( wordid->m_lexeme )->SetID( func_id++ );

					Match( Tag::LPAREN );
					param_type_list();
					Match( Tag::RPAREN );

					m_funStmtMap[ wordid->m_lexeme ] = (Stmt*)(m_pRoot = block());	// { ... }

					m_env.Clear();
				}
				else if ( m_pToken->m_Tag == Tag::COMMA || m_pToken->m_Tag == Tag::SEMI )// external_decl_stmt
				{
					DeclareStmt* pDecl = new DeclareStmt( tp );

					SetStmt* pSet = new SetStmt( *id, assignForDecl() );
					pDecl->AddSetStmt( pSet );
					InsertSymbol( id );

					while ( m_pToken->m_Tag == Tag::COMMA )	//  { “,” declarator }
					{
						Match( Tag::COMMA );

						declarator( pDecl, tp );
					}

					Match( Tag::SEMI );

					delete pDecl;

				}
				else throw "error in external_declaration";
			}
			else if ( m_pToken->m_Tag == Tag::STRUCT )
			{
				struct_specifier();
			}
			else
			{
				return NULL;
			}


		}			

		return NULL;
	}

	Stmt* CParser::block()
	{
		Match( Tag::LBRACE );		// {

		//decl_stmt();					// decls

		Stmt* pStmt = stmts();		// stmts

		Match( Tag::RBRACE );		// }

		return pStmt;
	}

	void CParser::argname( Type* type )
	{
		// pointer
		//int ptNum = 0;
		if ( m_pToken->m_Tag == Tag::TIMES )
		{
			//ptNum = pointer();
			type = pointers( type );
		}

		// ID
		Token* pToken = m_pToken;
		Match( Tag::ID );
		Word* wordid = dynamic_cast<Word*>(pToken);
		Id* id = new Id( wordid, type );
		//id->SetDefPointerUnrefCount( ptNum );
		m_env.Put( wordid->m_lexeme, id );

		// array
		if ( m_pToken->m_Tag == Tag::LSQUBRCK )
		{
			Type* tp = dimens( type );
			// 还需要保存type到链表中最后回收内存！！！！！！！！
			id->m_pType = tp;
			//arrayDeclarator( *id );
			InsertSymbol( id, false, dynamic_cast<Array*>(tp) );
		}
		else
		{
			InsertSymbol( id, true );
		}	
	}

	Stmt* CParser::param_type_list()
	{
		if ( m_pToken->m_Tag == Tag::RPAREN )	return NULL;

		argname( type() );

		while ( m_pToken->m_Tag == Tag::COMMA )
		{
			Match( Tag::COMMA );

			argname( type() );
		}

		return NULL;	// needed to be modified
	}

	ArrayAccess* CParser::offset( const Id& a )
	{
		Expr *i, *w, *t1, *t2, *loc;
		Type* type = a.m_pType;
		Match( Tag::LSQUBRCK );
		i = boolExpr();
		Match( Tag::RSQUBRCK );

		Array* arr = dynamic_cast<Array*>(type);
		if ( arr )
		{
			type = dynamic_cast<Array*>(type)->m_pElementType;
		}
		else if ( typeid( *type ) == typeid( Pointer ) ) 
		{
			type = dynamic_cast<Pointer*>( type )->m_pElementType;
			w = new Constant( type->m_width );

			t1 = new Arith( new Token( Tag::TIMES ), i, w );
			loc = t1;

			//t1 = new Constant( a.m_pType->m_width );
			//loc = new Arith( new Token( Tag::MINUS ), t1, loc );

			return new ArrayAccess( a, loc, type );
		}

		w = new Constant( type->m_width );
		t1 = new Arith( new Token( Tag::TIMES ), i, w );
		loc = t1;

		while ( m_pToken->m_Tag == Tag::LSQUBRCK )
		{
			Match( Tag::LSQUBRCK );
			i = boolExpr();
			Match( Tag::RSQUBRCK );

			type = dynamic_cast<Array*>(type)->m_pElementType;

			w = new Constant( type->m_width );

			t1 = new Arith( new Token( Tag::TIMES ), i, w );
			t2 = new Arith( new Token( Tag::PLUS ), loc, t1 );
			loc = t2;
		}

		//int offsetWidth = dynamic_cast<Array*>(a.m_pType)->m_size * dynamic_cast<Array*>( a.m_pType )->m_pElementType->m_width;
		//t1 = new Constant( offsetWidth );
		//loc = new Arith( new Token( Tag::MINUS ), t1, loc );

		return new ArrayAccess( a, loc, type );
	}

	Type* CParser::dimens( Type* p )
	{
		Match( Tag::LSQUBRCK );		//-> [					
		Token* tok = m_pToken;
		Match( Tag::NUM );
		Match( Tag::RSQUBRCK );		//-> ]		

		if ( m_pToken->m_Tag == Tag::LSQUBRCK )
		{
			p = dimens( p );
		}

		return new Array( dynamic_cast<Num*>(tok)->m_Val, p );
	}

	Stmt* CParser::stmt()
	{
		Expr* pExpr = NULL, *p1 = NULL, *p2 = NULL, *p3 = NULL;
		Stmt *s1 = NULL, *s2 = NULL, *s3 = NULL, *pSaveStmt = NULL;
		Id* id = NULL;
		int lineno = 0;
		while ( true )
		{
			switch ( m_pToken->m_Tag )
			{
			case Tag::SEMI:
				break;

			case Tag::IF:
				lineno = LinenoSupplier::GetLineno();
				Match( Tag::IF );		//-> if
				Match( Tag::LPAREN );	//-> (
				pExpr = boolExpr();		//-> exp
				Match( Tag::RPAREN );	//-> )

				s1 = compound_stmt();	//-> compound_stmt
				//	dynamic_cast<StmtSeq*>(s1)->m_pStmt2;

				if ( m_pToken->m_Tag != Tag::ELSE )	//-> else
				{
					s3 = new IfStmt( pExpr, s1 );
					dynamic_cast<IfStmt*>(s3)->m_lineno = lineno;	// 修正行号
					return s3;
				}

				Match( Tag::ELSE );		// else
				s2 = compound_stmt();

				s3 = new IfElseStmt( pExpr, s1, s2 );
				dynamic_cast<IfElseStmt*>(s3)->m_lineno = lineno;	// 修正行号
				return s3;

			case Tag::WHILE:
				pSaveStmt = Stmt::m_pEnclosing;
				s1 = new WhileStmt();
				Stmt::m_pEnclosing = s1;	// 保存while提供给break使用

				Match( Tag::WHILE );	// while
				Match( Tag::LPAREN );	// (
				pExpr = boolExpr();		// boolExpr
				Match( Tag::RPAREN );	// )

				dynamic_cast<WhileStmt*>(s1)->Init( pExpr, stmt() );

				Stmt::m_pEnclosing = pSaveStmt;
				return s1;

			case Tag::FOR:
				//pSaveStmt = Stmt::m_pEnclosing;
				//s1 = new ForStmt( NULL, NULL, NULL, NULL );
				//Stmt::m_pEnclosing = s1;	// 保存while提供给break使用

				//Match( Tag::FOR );		// for
				//Match( Tag::LPAREN );	// (
				//dynamic_cast<ForStmt*>(s1)->m_pInitStmt = stmt();		

				//dynamic_cast<ForStmt*>(s1)->m_pLogicExpr = boolExpr();		
				//Match( Tag::SEMI );
				//dynamic_cast<ForStmt*>(s1)->m_pIterStmt = boolExpr();	
				//Match( Tag::RPAREN );	// )

				//dynamic_cast<ForStmt*>(s1)->m_pStmt = block();

				//Stmt::m_pEnclosing = pSaveStmt;
				//return s1;

			case Tag::BREAK:
				Match( Tag::BREAK );
				Match( Tag::SEMI );
				return new BreakStmt;

			case Tag::RETURN:
				Match( Tag::RETURN );
				pExpr = boolExpr();
				s1 = new ReturnStmt( pExpr );
				Match( Tag::SEMI );
				return s1;
				// new return
				// need to be continue

			case Tag::LBRACE:
				return block();

				//case Tag::ID:		
				//	return assign();
			case Tag::STRUCT:
			case Tag::BASIC:
				delete decl_stmt();
				break;				

			case Tag::READ:
				Match( Tag::READ );

				s1 = new ReadStmt( boolExpr() );
				Match( Tag::SEMI );

				delete id;
				return s1;

			case Tag::WRITE:
				Match( Tag::WRITE );

				s1 = new WriteStmt( boolExpr() );
				Match( Tag::SEMI );

				delete id;
				return s1;

			default: 
				s1 = new ExprStmt( expr() );
				Match( Tag::SEMI );
				return s1;
			}
		}


		return NULL;
	}

	Stmt* CParser::stmts()
	{
		if ( m_pToken->m_Tag == Tag::RBRACE )	//-> }
		{
			return NULL;
		}
		else
		{
			try
			{
				Stmt* s1 = stmt();
				Stmt* s2 = stmts();
				return new StmtSeq( s1, s2 );		// 语句序列
			}
			catch ( char* e )
			{
				//MessageBoxA( NULL, e, "Error", MB_OK );
				throw e;
			}
		}

		return NULL;
	}

	Stmt* CParser::assign()
	{
		Stmt* pStmt = NULL;

		// pointer
		int ptNum = 0;
		if ( m_pToken->m_Tag == Tag::TIMES )
		{
			ptNum = pointer();
		}

		// ID
		Token* pToken = m_pToken;
		Match( Tag::ID );
		Word* wordid = dynamic_cast<Word*>(pToken);

		Id* oldId = m_env.GetId( wordid->m_lexeme );
		if ( oldId == NULL )
		{
			throw "non defined variable!";
		}
		Id* id = new Id( *oldId );
		id->GetArrayExpr().clear();

		// array
		if ( m_pToken->m_Tag == Tag::LSQUBRCK )		// [
		{
			//arrayDeclarator( *id );
			ArrayAccess* arrAcc = offset( *id );
			Match( Tag::ASSIGN );
			pStmt = new SetElemStmt( arrAcc, boolExpr() );
			arrAcc->m_bRecycle = false;
			delete arrAcc;
		}
		else if ( m_pToken->m_Tag == Tag::ASSIGN )		//-> = 
		{
			Move();
			pStmt = new SetStmt( *id, boolExpr() );	//-> expr
		}
		else if ( m_pToken->m_Tag == Tag::LPAREN )	//-> (
		{
			Match( Tag::LPAREN );	// -> (
			//CallExpr* pExpr = new CallExpr( Id( *id ) );
			//param_list( pExpr );
			CallStmt* pCall = new CallStmt( *id );
			CallExpr callExpr( m_fooId, false );
			param_list( &callExpr );

			for ( auto arg = callExpr.m_params.begin(); arg != callExpr.m_params.end(); arg++ )
			{
				pCall->AddArg( *arg );
			}				

			Match( Tag::RPAREN );	//-> )

			pStmt = pCall;
		}
		else throw "assign error";

		Match( Tag::SEMI );							//-> ;

		delete id;
		return pStmt;
	}

	Stmt* CParser::compound_stmt()
	{
		if ( m_pToken->m_Tag == Tag::LBRACE ) 
		{
			return block();
		}
		else
		{
			return new StmtSeq( stmt(), NULL );
		}
	}

	Type* CParser::pointers( Type* p )
	{
		Match( Tag::TIMES );	//-> *
		Token* tok = m_pToken;

		if ( m_pToken->m_Tag == Tag::TIMES )
		{
			p = pointers( p );
		}

		return new Pointer( p );
	}

	Type* CParser::type()
	{
		Type* p = dynamic_cast<Type*>(m_pToken);
		if ( p == NULL )	throw "Type Error!";
		Move();					// type

		return p;
	}

	void CParser::struct_specifier()
	{
		Match( Tag::STRUCT );	

		Token* pToken = m_pToken;
		Match( Tag::ID );
		Word* wordid = dynamic_cast<Word*>(pToken);
		m_tableList.AddTable( wordid->m_lexeme, true );	// 添加结构体表
		Type* type = new Type( wordid->m_lexeme, Tag::STRUCT, m_tableList.GetTable( wordid->m_lexeme )->GetTableSize() );
		m_pScanner->reserveType( type );

		SymbolType* symbolType = new SymbolType;
		symbolType->structDescription = m_tableList.GetTable( wordid->m_lexeme );
		symbolType->type_name = wordid->m_lexeme;
		symbolType->width = m_tableList.GetTable( wordid->m_lexeme )->GetTableSize();
		symbolType->typeKind = StructKind;
		m_tableList.InsertType( *symbolType );

		Match( Tag::LBRACE );

		while ( m_pToken->m_Tag == Tag::BASIC || m_pToken->m_Tag == Tag::STRUCT )
		{
			Stmt* pStmt = decl_stmt( true );
		}

		Match( Tag::RBRACE );
		Match( Tag::SEMI );

		type->m_width = m_tableList.GetTable( wordid->m_lexeme )->GetTableSize();
		m_tableList.GetType( wordid->m_lexeme )->width = m_tableList.GetTable( wordid->m_lexeme )->GetTableSize();
	}

	Expr* CParser::assignForDecl()
	{
		if ( m_pToken->m_Tag != Tag::ASSIGN )	return NULL;

		Match( Tag::ASSIGN );

		return expr();
	}

	void CParser::arrayDeclarator( Id& id )
	{
		Match( Tag::LSQUBRCK );
		id.AddArrayExpr( const_expr() );
		Match( Tag::RSQUBRCK );

		while ( m_pToken->m_Tag == Tag::LSQUBRCK )
		{
			Match( Tag::LSQUBRCK );
			id.AddArrayExpr( const_expr() );
			Match( Tag::RSQUBRCK );
		}
	}

	int CParser::pointer()
	{
		int count = 0;

		Match( Tag::TIMES ); //-> *
		count++;

		while ( m_pToken->m_Tag == Tag::TIMES )
		{
			Match( Tag::TIMES ); //-> *
			count++;
		}

		return count;
	}

	void CParser::InsertSymbol( Id* id, bool bParam /*= false*/, Array* type /*= NULL */ )
	{
		Symbol symbol;
		symbol.name = dynamic_cast<Word*>(id->m_pTok)->m_lexeme;

		if ( type != NULL )
		{
			SymbolType symbolType;
			symbolType.type_name = id->m_pType->ToString();
			symbolType.width = type->m_width;
			while ( true )
			{
				symbolType.type_name += "_" + itoa( type->m_size ); 
				if ( type->m_pElementType->m_Tag == Tag::BASIC || type->m_pElementType->m_Tag == Tag::STRUCT )	break;
				else
				{
					type = (Array*)type->m_pElementType;
				}						
			}
			symbolType.typeKind = ArrayKind;
			m_tableList.InsertType( symbolType );
			symbol.type = m_tableList.GetType( symbolType.type_name );
		}
		else
		{
			if ( typeid( *id->m_pType ) == typeid( Pointer ) )
			{
				Type* type = dynamic_cast<Pointer*>( id->m_pType );
				while ( typeid( *dynamic_cast<Pointer*>( type )->m_pElementType ) == typeid( Pointer ) )
				{
					type = dynamic_cast<Pointer*>( type )->m_pElementType;
				}

				SymbolType symbolType;
				symbolType.typeKind = PointerKind;
				symbolType.width = sizeof( long );
				symbolType.next = m_tableList.GetType( dynamic_cast<Pointer*>( type )->m_pElementType->m_lexeme );
				symbolType.type_name = dynamic_cast<Pointer*>( type )->m_pElementType->m_lexeme + "*";

				m_tableList.InsertType( symbolType );

				symbol.type = m_tableList.GetType( symbolType.type_name );
			}
			else 
			{
				symbol.type = m_tableList.GetType( id->m_pType->m_lexeme );
			}
		}

		if ( bParam )
		{
			m_tableList.InsertParam( symbol );
		}
		else
		{
			m_tableList.InsertSymbol( symbol );
		}
	}

	void CParser::declarator( DeclareStmt* pDecl, Type* tp, bool bGlobal /*= false */ )
	{
		// pointer
		//int ptNum = 0;
		if ( m_pToken->m_Tag == Tag::TIMES )
		{
			//ptNum = pointer();
			tp = pointers( tp );
		}

		// ID
		Token* pToken = m_pToken;
		Match( Tag::ID );
		Word* wordid = dynamic_cast<Word*>(pToken);
		Id* id = new Id( wordid, tp );
		//id->SetDefPointerUnrefCount( ptNum );
		m_env.Put( wordid->m_lexeme, id, bGlobal );

		// array
		if ( m_pToken->m_Tag == Tag::LSQUBRCK )
		{
			Type* type = dimens( tp );
			// 还需要保存type到链表中最后回收内存！！！！！！！！
			id->m_pType = type;
			//arrayDeclarator( *id );
			InsertSymbol( id, false, dynamic_cast<Array*>(type) );
		}
		else
		{
			InsertSymbol(id);
		}

		//if ( m_pToken->m_Tag == Tag::ASSIGN )
		//{
		//	Token* tok = m_pToken;

		//	Expr* e1 = assignment_expr();

		//	new Arith( tok, id, e1 );
		//}
		
		// assignForDecl
		//SetStmt* pSet = new SetStmt( *id, assignForDecl() );
		//pDecl->AddSetStmt( pSet );

	}

	void CParser::declarators( DeclareStmt* pDecl, Type* tp, bool bGlobal /*= false */ )
	{
		declarator( pDecl, tp, bGlobal );

		while ( m_pToken->m_Tag == Tag::COMMA )	//  { “,” declarator }
		{
			Match( Tag::COMMA );

			declarator( pDecl, tp, bGlobal );
		}
	}

	Stmt* CParser::decl_stmt( bool bGlobal /*= false */ )
	{
		if ( m_pToken->m_Tag != Tag::BASIC && m_pToken->m_Tag != Tag::STRUCT )	return NULL;	// <empty>

		Type* tp = type();				// -> type

		DeclareStmt* pDecl = new DeclareStmt( tp );

		declarators( pDecl, tp, bGlobal );		// -> declarators

		Match( Tag::SEMI );				// -> ;

		return pDecl;
	}

	Expr* CParser::boolExpr()
	{
		Expr* pExpr = join();						
		while ( m_pToken->m_Tag == Tag::OR )		//-> || 
		{
			Token* pTok = m_pToken;
			Match( Tag::OR );
			pExpr = new OrOperator( pTok, pExpr, join() );
		}
		return pExpr;
	}

	Expr* CParser::join()
	{
		Expr* pExpr = equality();
		while ( m_pToken->m_Tag == Tag::AND )		//-> &&
		{
			Token* pToken = m_pToken;
			Move();
			pExpr = new AndOperator( pToken, pExpr, equality() );
		}
		return pExpr;
	}

	Expr* CParser::equality()
	{
		Expr* pExpr = rel();
		while ( m_pToken->m_Tag == Tag::EQ || m_pToken->m_Tag == Tag::NE )	//-> == | !=
		{
			Token* pToken = m_pToken;
			Move();
			pExpr = new Relation( pToken, pExpr, rel() );
		}
		return pExpr;
	}

	Expr* CParser::rel()
	{
		Expr* pExpr = addictive_expr();
		Token* pToken = NULL;

		switch ( m_pToken->m_Tag )				//-> < <= > >= 
		{
		case Tag::LT:
		case Tag::LE:
		case Tag::GE:
		case Tag::GT:
			pToken = m_pToken;
			Move();
			return new Relation( pToken, pExpr, addictive_expr() );
			break;
		default:
			return pExpr;
		}
	}

	Expr* CParser::addictive_expr()
	{
		Expr* pExpr = term();
		while ( m_pToken->m_Tag == Tag::PLUS || m_pToken->m_Tag == Tag::MINUS )	//-> + | -
		{
			Token* pToken = m_pToken;
			Move();
			pExpr = new Arith( pToken, pExpr, term() );
		}
		return pExpr;
	}

	Expr* CParser::term()
	{
		Expr* pExpr = unary();
		while ( m_pToken->m_Tag == Tag::TIMES || m_pToken->m_Tag == Tag::OVER ) //-> * | /
		{
			Token* pToken = m_pToken;
			Move();
			pExpr = new Arith( pToken, pExpr, unary() );
		}
		return pExpr;
	}

	Expr* CParser::unary()
	{
		if ( m_pToken->m_Tag == Tag::MINUS )		//-> -
		{
			Move();
			return new Unary( new Token( Tag::MINUS ), unary() );
		}
		else if ( m_pToken->m_Tag == Tag::NOT )		//-> !
		{
			Token* pTok = m_pToken;
			Move();
			return new NotOperator( pTok, unary() );
		}
		else if ( m_pToken->m_Tag == Tag::TIMES )	//->	*
		{
			Move();
			return new Unary( new Token( Tag::TIMES ), unary() );
		}
		else if ( m_pToken->m_Tag == Tag::REFERENCE )	//-> &
		{
			Move();
			return new Unary( new Token( Tag::REFERENCE ), unary() );
		}
		else return factor();
	}

	Expr* CParser::factor()
	{
		Expr* pExpr = NULL;
		Type* tp = NULL;

		switch ( m_pToken->m_Tag )
		{
		case Tag::LPAREN:				//-> (
			Move();
			pExpr = boolExpr();			//-> expr
			Match( Tag::RPAREN );		//-> )
			return pExpr;

		case Tag::NUM:
			pExpr = new Constant( m_pToken, Type::Int );	//-> num
			Move();
			return pExpr;

		case Tag::DOUBLE:
		case Tag::FLOAT:
			pExpr = new Constant( m_pToken, Type::Float );	//-> doubleNum
			Move();
			return pExpr;

		case Tag::TRUETYPE:
			pExpr = new Constant( *Constant::True );	//-> true
			Move();
			return pExpr;

		case Tag::FALSETYPE:
			pExpr = new Constant( *Constant::False );	//-> false
			Move();
			return pExpr;

		case Tag::ID:
			pExpr = abstract_declarator();

			return pExpr;

		case Tag::NEW:
			Match( Tag::NEW );

			tp = type();
			if ( tp )
			{

			}


		default:
			//error
			break;
		}

		return NULL;
	}

	Expr* CParser::param_list( CallExpr* pCall )
	{
		Expr* pExpr = NULL;
		while ( m_pToken->m_Tag != Tag::RPAREN )	
		{
			pCall->AddParam ( boolExpr() );

			while ( m_pToken->m_Tag == Tag::COMMA )
			{
				Move();
				pCall->AddParam ( boolExpr() );
			}
		}
		return pExpr;
	}

	Expr* CParser::abstract_declarator()
	{
		Id* id = NULL;

		id = m_env.GetId( ((Word*)m_pToken)->m_lexeme );	//-> id
		if ( id == NULL )	
		{
			char errorMessage[ 32 ];
			sprintf_s( errorMessage, "Can't find symbol! At line: %d", m_pToken->m_locateLine );
			throw errorMessage;
		}
		Move();

		if ( m_pToken->m_Tag == Tag::LSQUBRCK )		// array
		{
			return offset( *id );
		}
		else if ( m_pToken->m_Tag == Tag::LPAREN )	// func call
		{
			Match( Tag::LPAREN );	// -> (
			CallExpr* pExpr = new CallExpr( Id( *id ) );
			param_list( pExpr );
			Match( Tag::RPAREN );	//-> )
			return pExpr;
		}
		else if ( m_pToken->m_Tag == Tag::ARROW || m_pToken->m_Tag == Tag::POINT )
		{
			Token *tok = m_pToken;
			Move();
			return new Arith( tok, new Id( *id ), abstract_declarator() );
			//return new StructAccessExpr( Id( *id ), abstract_declarator() );
		}
		else
		{
			return new Id( *id );
		}

		return NULL;
	}

	Expr* CParser::const_expr()
	{
		return conditional_expr();
	}

	Expr* CParser::conditional_expr()
	{
		Expr* pExpr = logical_expr();		

		if ( m_pToken->m_Tag == Tag::QUESTIONMARK )
		{
			Token* pToken = m_pToken;
			Match( Tag::QUESTIONMARK );

			Expr* lhs = expr();

			Match( Tag::COLON );

			Expr* rhs = conditional_expr();

			pExpr = new Ternary( pToken, pExpr, lhs, rhs );
		}

		return pExpr;
	}

	Expr* CParser::logical_expr()
	{
		return boolExpr();
	}

	Expr* CParser::expr()
	{
		if ( m_pToken->m_Tag != Tag::ID 
			&& m_pToken->m_Tag != Tag::INC 
			&& m_pToken->m_Tag != Tag::DEC
			&& !unary_op() )	
		{
			return NULL;
		}

		Expr *p2 = NULL;
		Expr *p1 = assignment_expr();
		if ( m_pToken->m_Tag == Tag::COMMA )
		{
			Match( Tag::COMMA );
			p2 = expr();
		}
		return new ExprSeq( p1, p2 );
	}

	Expr* CParser::assignment_expr()
	{
		Expr* lhs = NULL;
		if ( isUnaryExpr() )
		{
			lhs = unary_expr();
			if ( assignment_op() )
			{
				Token* assTok = m_pToken;
				Move();
				Expr* rhs = assignment_expr();		
				Arith* arith = new Arith( assTok, lhs, rhs );

				return arith;
			}

		}

		return lhs;
	}

	bool CParser::isUnaryExpr()
	{
		switch ( m_pToken->m_Tag )
		{
		case Tag::ID:
		case Tag::INC:
		case Tag::DEC:
		case Tag::REFERENCE:
		case Tag::TIMES:
		case Tag::MINUS:
		case Tag::PLUS:
		case Tag::NOT:
		case Tag::NUM:
			return true;
		default:
			return false;				
		}
	}

	bool CParser::assignment_op()
	{
		switch ( m_pToken->m_Tag )
		{
		case Tag::ASSIGN:
		case Tag::PLUSASSIGN:
		case Tag::MINUSASSIGN:
		case Tag::MODASSIGN:
		case Tag::MULASSIGN:
		case Tag::DIVASSIGN:
			return true;
		default:
			return false;
		}
	}

	bool CParser::unary_op()
	{
		switch ( m_pToken->m_Tag )
		{
		case Tag::PLUS:
		case Tag::MINUS:
		case Tag::REFERENCE:
		case Tag::TIMES:
		case Tag::NOT:
			return true;
		default:
			return false;
		}
	}

	Expr* CParser::unary_expr()
	{
		Expr* pExpr = NULL;
		Token* tok = m_pToken;
		switch ( m_pToken->m_Tag )
		{
		case Tag::ID:
		case Tag::NUM:
		case Tag::LPAREN:
			return boolExpr();
		case Tag::INC:
		case Tag::DEC:
			Move();
			pExpr = unary_expr();
			return new Unary( tok, pExpr );	
		case Tag::TIMES:
		case Tag::PLUS:
		case Tag::MINUS:
		case Tag::REFERENCE:
		case Tag::NOT:
			Move();
			pExpr = cast_expr();
			return new Unary( tok, pExpr );	
		default:
			throw "Unary Error";
		}
	}

	Expr* CParser::cast_expr()
	{
		if ( m_pToken->m_Tag == Tag::LPAREN )
		{
			Match( Tag::LPAREN );

			Match( Tag::RPAREN );
			cast_expr();

			return NULL;
		}

		return unary_expr();
	}

	void CParser::Reset()
	{
		m_pToken = NULL;
		m_pRoot = NULL;
		m_immCode.clear();
		m_tableList.Clear();
		m_pScanner->Reset();
		DeleteAllToken();

		for ( auto iter = m_funStmtMap.begin(); iter != m_funStmtMap.end(); iter ++ )
		{
			delete iter->second;
		}
		m_funStmtMap.clear();
	}

	void CParser::LoadFromString( const CETString& codeString )
	{
		Reset();
		m_pScanner->LoadFromString( codeString );
	}

	int CParser::LoadFromFile( const CETString& filename )
	{
		Reset();
		return m_pScanner->OpenFile( filename );
	}

	CParser::CParser() : m_fooId( new Word( "fooId", Tag::OVER ), Type::Int )
	{
		//LoadFromFile( "SAMPLE.hpp" );
		m_pScanner = new TinyScanner;

		LinenoSupplier::pScanner = m_pScanner;

		AssembyCodeGen::parser = this;			// 绑定自己到语法树节点的输出源
	}

	void CParser::Parse( int flag /*= 0 */ )
	{
		try
		{
			Reset();
			program( !!flag );
		}
		catch ( char* e )
		{
			//MessageBox( NULL, e, "Error", MB_OK );
			throw e;
		}
		
		
	}

	const Environment& CParser::GetTables()
	{
		return m_tableList;
	}

	void CParser::DeleteAllToken()
	{
		// 交由词法器完成回收内存
		//for ( auto iter = m_tokenList.begin(); iter != m_tokenList.end(); ++iter )
		//{
		//	delete *iter;
		//}
		//m_tokenList.clear();
	}

	const std::string& CParser::GetImmCode()
	{
		if ( ! m_immCode.empty() )
		{
			return m_immCode;
		}

		m_immCode.clear();
		for ( auto iter = m_funStmtMap.begin(); iter != m_funStmtMap.end(); iter ++ )
		{
			AssembyCodeGen::GenCode( ("entry " + iter->first + '\n').c_str() );
			AssembyCodeGen::GenCode( ("mov d1, " + itoa( m_tableList.GetTable( iter->first )->GetID() ) + '\n').c_str() );

			int begin = iter->second->NewLabel();
			int after = iter->second->NewLabel();
			iter->second->EmitLabel( begin );
			iter->second->Gen( begin, after, true );
			iter->second->EmitLabel( after );
		}

		return m_immCode;
	}







}


