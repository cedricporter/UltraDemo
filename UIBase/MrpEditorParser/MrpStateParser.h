/*
** ���ߣ�����
** ˵����
**
*/
#pragma once

#include "Scanner.h"

typedef std::vector< CETString >			ListElementType;
typedef std::vector< ListElementType >		ListTpye;
typedef std::map< CETString, ListTpye >		MapType;

/*
** ���֣�StateNode
** ˵�����������״̬����ֲ�ν�ʵĽڵ�
**		[�ڲ���ʽ]	1.params�е�һ��Ԫ�ص����״̬�����֣�����������Ĳ���
**					2.subPred�����б������
*/
struct StateNode
{
	ListElementType	params;
	ListTpye		subPred;
};

/*
** ���֣�MrpStateParser
** ˵��������״̬�ļ�
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

	Scanner m_scanner;	// �ʷ���
	Token*	m_token;	// ��ǰtoken
	bool	ret;		// ���淵��ֵ...��ʱû��

	std::list< StateNode* >* m_pStateList;	// ���õ�״̬��Ϣ��

	/*
	** ���֣�
	** ˵�����ݹ��½������ļ�
	**
	** [ �ķ� ]
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


		// ����Բ����
		if ( Tag::LPAREN == m_token->m_Tag )
		{
			ret = Match( Tag::LPAREN );

			ret = paramList( params );
			if ( level == 0 ) state->params = params;
			

			ret = Match( Tag::RPAREN );

			// ���Ҵ����ţ��ݹ���������ν��
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
