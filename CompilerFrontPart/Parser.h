/*
** 作者：华亮，罗嘉飞
** 说明：
**
*/

#pragma once

#include "Scanner.h"
#include "TreeDef.h"
#include "SymbolTable.h"
#include <Windows.h>
#include <map>
#include <string>
#include <iostream>

namespace ETCompiler
{

	class IdList
	{
	public:
		typedef std::map< CETString, Id* > MapType;

		~IdList()
		{
			Clear();

			for ( auto iter = m_globalMap.begin(); iter != m_globalMap.end(); ++iter )
			{
				delete iter->second;
			}
		}

		MapType m_idMap;
		MapType m_globalMap;

		void Put( const CETString& name, Id* id, bool bGlobal = false )
		{
			if ( bGlobal )
			{
				m_globalMap[ name ] = id;
			}
			else
			{
				m_idMap[ name ] = id;
			}			
		}

		Id* GetId( const CETString& name )
		{
			Id* id = NULL;
			if ( m_idMap.find( name ) != m_idMap.end() )
			{
				id = m_idMap[ name ];
			}
			else if ( m_globalMap.find( name ) != m_globalMap.end() )
			{
				id = m_globalMap[ name ];
			}

			return id;
		}

		// 清空局部变量
		void Clear()
		{
			for ( auto iter = m_idMap.begin(); iter != m_idMap.end(); ++iter )
			{
				delete iter->second;
			}
			m_idMap.clear();
		}
	};


	/*
	** 名字：ComTypeList
	** 说明：组合类型
	**
	*/
	class ComTypeList
	{
	public:
		typedef std::map< std::string, Type* >	TypeMap;
		TypeMap		m_typeMap;

		ComTypeList()
		{

		}

		~ComTypeList()
		{
			for ( auto iter = m_typeMap.begin(); iter != m_typeMap.end(); iter++ )
			{
				delete iter->second;
			}
		}

		void InsertType( const std::string& type_name, Type* type )
		{
			m_typeMap[ type_name ] = type;
		}

		Type* GetType( const std::string type_name )
		{
			if ( m_typeMap.find( type_name ) == m_typeMap.end() )	return NULL;

			return m_typeMap[ type_name ];
		}

	};

	class Id;

	/*
	** 名字：IParser
	** 说明：抽象语法器
	**
	*/
	class IParser
	{
	public:	
		IParser() : m_pScanner( NULL ), m_pToken( NULL ), m_pRoot( NULL )
		{
		}

		virtual ~IParser() 
		{
			SAFE_DELETE( m_pScanner );
			SAFE_DELETE( m_pToken );
			SAFE_DELETE( m_pRoot );
		}


		// 从文件加载
		virtual int LoadFromFile( const CETString& filename ) = 0;
		virtual void LoadFromString( const CETString& codeString ) = 0;
		virtual const Environment& GetTables() = 0;
		virtual void Reset()
		{
			m_pToken = NULL;
			m_pRoot = NULL;
		}
		virtual void Parse( int flag = 0 ) = 0;
		// 获取语法树根节点
		virtual LPVOID GetSyntaxTree() const
		{
			return (LPVOID)m_pRoot;
		}

		virtual void InsertCode( const char* code )
		{
			m_immCode += code;
		}

		// 返回中间代码
		virtual const std::string& GetImmCode()
		{
			return m_immCode;
		}

	protected:
		IScanner*	m_pScanner;		// 词法器
		Token*		m_pToken;		// 当前Token
		TreeNode*	m_pRoot;		// 语法树根节点
		std::string m_immCode;		// 中间代码

	};


	typedef std::map< std::string, Stmt* > Fun_StmtMap;	


	/*
	** 名字：
	** 说明：
	**
	*/
	class CParser : public IParser
	{
	public:	
		CParser();

		~CParser();

		void DeleteAllToken();

		// 从文件加载
		int LoadFromFile( const CETString& filename );

		void LoadFromString( const CETString& codeString );

		void Reset();

		// 分析
		void Parse(  int flag = 0 );

		const Environment& GetTables();		

		const std::string& GetImmCode();

		LPVOID GetSyntaxTree() const
		{
			return (LPVOID)&m_funStmtMap;
		}

	private:

		//Token*		m_pToken;		// 当前Token	
		//TreeNode*	m_pRoot;		// 语法树根节点
		Environment	m_tableList;
		IdList		m_env;
		Id			m_fooId;

		Fun_StmtMap	m_funStmtMap;		// 函数名和下面语句的映射
		ComTypeList m_comTypeList;		// 组合类型，如struct

		std::list<Token*> m_tokenList;	// 记录Token以回收内存放

		//std::list< Type* >	m_typeList;	// 用于保存当前函数的参数类型列表
		std::string			m_printTreeLog;

	private:

		void Move();

		void Match( Tag::TagEnum tag );


		/************************************************************************/
		/* 自顶下下分析                                                          */
		/************************************************************************/

		// 1.	Program 			-> func_definition
		void program( bool bASM );


		/************************************************************************/
		/* 语句		                                                            */
		/************************************************************************/

		/*
		2.	external_declaration-> Func_definitions | external_decl_stmt
		3.	Func_definitions	-> {Func_definition}+
		4.	Func_definition	-> type id “(“ param_type_list “)” block
		5.	external_decl_stmt-> <empty> | type declarators “;”
		*/
		Stmt* external_declaration( bool bASM );

		// 4.	Param_type_list 	-> type id {type_param}* | <empty>
		Stmt* param_type_list();

		void argname( Type* type );	

		Stmt* block();

		/*
		11.	Decl_stmt		-> <empty> | type declarators “;”
		12.	Declarators		-> declarator { “,” declarator }
		13.	Declarator		-> {Pointer}? id { arrayDeclarator}	? assignForDecl
		14.	Pointer			-> “*” {pointer}?
		15.	arrayDeclarator	-> “[“ {const_expr}? “]” {arrayDeclarator}?
		16.	const_expr		-> conditional_expr
		17.	conditional_expr	-> logical_expr | logical_expr “?” expr : conditional_expr
		18.	assignForDecl		-> <empty> | “=” expr
		*/
		Stmt* decl_stmt( bool bGlobal = false );

		// 12.	Declarators		-> declarator { “,” declarator }
		void declarators( DeclareStmt* pDecl, Type* tp, bool bGlobal = false );

		void declarator( DeclareStmt* pDecl, Type* tp, bool bGlobal = false );	

		void InsertSymbol( Id* id, bool bParam = false, Array* type = NULL ); 

		int pointer();

		//15.	arrayDeclarator	-> “[“ {const_expr}? “]” {arrayDeclarator}?
		void arrayDeclarator( Id& id );

		// 14.	assignForDecl		-> <empty> | “=” expr
		Expr* assignForDecl();

		void struct_specifier();	

		Type* type();

		Type* pointers( Type* p );

		Type* dimens( Type* p );

		// 记录数组偏移用
		ArrayAccess* offset( const Id& a );	

		// 10.	Stmts			-> decl_stmt | if_stmt | while_stmt | for_stmt 
		//					| dowhile_stmt | switch_stmt | return_stmt | <empty>
		Stmt* stmt();		

		Stmt* stmts();		

		//26.	assign			-> Declarator “=” expr	
		Stmt* assign();		

		Stmt* compound_stmt();




		/************************************************************************/
		/* 表达式                                                               */
		/************************************************************************/


		Expr* boolExpr();

		Expr* join();

		Expr* equality();

		Expr* rel();

		Expr* addictive_expr();

		Expr* term();

		Expr* unary();

		Expr* factor();

		// 函数调用参数列表
		// 14.	param_list		-> <empty> | param { “,” param }*
		// 15.	param			-> expr
		Expr* param_list( CallExpr* pCall );

		// 13.	abstract_declarator-> id | id “[“ expr “]” | id “(“ param_list “)” 
		//						| id “->” abstract_declarator | id “.” abstract_declarator
		Expr* abstract_declarator();

		//16.	const_expr		-> conditional_expr
		//	17.	conditional_expr	-> logical_expr | logical_expr “?” expr : conditional_expr

		Expr* const_expr();

		Expr* conditional_expr();

		Expr* logical_expr();


		//	27.	expr				-> assignment_expr {“,” assignment_expr }+
		//	28.	assignment_expr	-> unary_expr assgnment_op assignment_expr
		//	29.	assgnment_op	  	-> “=” | “+=” | …
		//	30.	unary_op			-> “&” | “+” | “*” | …
		//	31.	unary_expr		-> postfix_expr | “++” unary_expr | “--“ unary_expr 
		//	| unary_op cast_expr
		//	32.	cast_expr			-> unary_expr | “(“ type_name “)” cast_expr
		//	33.	postfix_expr		-> primary_expr { postfix_op }+
		//	34.	postfix_op		-> “[“ expr “]” | “(“ assignment_expr “)” | “->” id | “.” Id | “++” | “—“
		//	35.	primary_expr		-> id | constant | string | “(“ expr “)”
		//	36.	constant			-> integer_const | char_const | floating_const | enum_const

		Expr* expr();

		Expr* assignment_expr();

		bool isUnaryExpr();

		bool assignment_op();

		bool unary_op();

		Expr* unary_expr();

		Expr* cast_expr();



	};




}







