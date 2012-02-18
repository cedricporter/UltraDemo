/*
** 作者：华亮
** 说明：
**
*/
#pragma once

#include "Scanner.h"

typedef std::vector< CETString >			ListElementType;
typedef std::vector< ListElementType >		ListTpye;
typedef std::map< CETString, ListTpye >		MapType;

/*
** 名字：StateNode
** 说明：保存各个状态及其局部谓词的节点
**		[内部格式]	1.params中第一个元素的这个状态的名字，后面的是它的参数
**					2.subPred链表中保存的是
*/
struct StateNode
{
	ListElementType	params;
	ListTpye		subPred;
};

/*
** 名字：MrpStateParser
** 说明：解析状态文件
**
*/
class MrpStateParser
{
public:
	MrpStateParser() : m_token( NULL ), m_pStateList( NULL ) {}


	std::list< StateNode* >* GetStateList( const CETString& filename )
	{
		m_pStateList = new std::list< StateNode* >;

		m_scanner.OpenFile(filename);

		StateFuncList();

		return m_pStateList;
	}
		
private:

	Scanner m_scanner;	// 词法器
	Token*	m_token;	// 当前token
	bool	ret;		// 保存返回值...暂时没用

	std::list< StateNode* >* m_pStateList;	// 建好的状态信息表

	/*
	** 名字：
	** 说明：递归下降分析文件
	**
	** [ 文法 ]
	**
	** StateFuncList	-> {fun_declaration}*
	** fun_declaration	-> ID "(" paramList ")" block
	** paramList		-> param {"," param}* | <empty>
	** param			-> ID
	** block			-> "{" funList "}" | <empty>
	** funList			-> {fun_declaration}+
	**
	*/

	bool Match( Tag::TagEnum tag )
	{
		assert( m_token );

		if ( tag == m_token->m_Tag )
		{
			delete m_token;
			m_token = m_scanner.GetToken();
			return true;
		}
		return false;
	}

	bool param( ListElementType& params )
	{
		assert( m_token );

		if ( m_token->m_Tag != Tag::RPAREN )
		{
			if ( Tag::ID == m_token->m_Tag ) params.push_back( dynamic_cast<Word*>(m_token)->m_lexeme );
			return Match( Tag::ID );
		}

		return false;
	}

	bool paramList( ListElementType& params )
	{
		assert( m_token );

		ret = param( params );

		while ( m_token->m_Tag != Tag::RPAREN )
		{
			ret = Match( Tag::COMMA );

			ret = param( params );
		}

		return ret;
	}

	bool fun_declaration( ListElementType& params, int level = 1 )
	{	
		assert( m_token );

		if ( Tag::ID == m_token->m_Tag )
			params.push_back( dynamic_cast<Word*>(m_token)->m_lexeme );

		ret = Match( Tag::ID );

		StateNode* state;
		if ( level == 0 )
		{
			state = new StateNode;;
			state->params = params;
			m_pStateList->push_back( state );
		}


		// 有左圆括号
		if ( Tag::LPAREN == m_token->m_Tag )
		{
			ret = Match( Tag::LPAREN );

			ret = paramList( params );
			if ( level == 0 ) state->params = params;
			

			ret = Match( Tag::RPAREN );

			// 有右大括号，递归进入添加子谓词
			if ( m_token->m_Tag == Tag::LBRACE )
			{
				ret = Block( state->subPred );
			}
		}	

		return ret;
	}

	bool funList( ListTpye& subPredList )
	{
		assert( m_token );

		ListElementType params;
		ret = fun_declaration( params );

		subPredList.push_back( params );

		while ( m_token->m_Tag == Tag::ID )
		{
			ListElementType params;
			ret = fun_declaration( params );
			subPredList.push_back( params );

		}

		return ret;
	}

	bool Block( ListTpye& subPredList )
	{
		assert( m_token );

		ret = Match( Tag::LBRACE );

		if ( Tag::RBRACE != m_token->m_Tag )	// empty
			funList( subPredList );

		ret = Match( Tag::RBRACE );

		return ret;
	}

	bool StateFuncList()
	{
		StateNode node;

		m_token = m_scanner.GetToken();

		while ( m_token->m_Tag == Tag::ID )
		{
			node.params.clear();
			ret = fun_declaration( node.params, 0 );
		}

		return ret;
	}

};
