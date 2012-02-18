/*
** ���ߣ�����
** ˵����
**
*/

/*
ָ���б�
add souce, lhs, rhs
call label
div
in
jeq
jge
jgt
jle
jlt lhs, rhs, label
jmp
jne
lea
leave
mov
mul
out
st a, reg
sub
*/

#pragma once
#include <string>
#include <vector>
#include "Def.h"
#include "SymbolTable.h"

namespace ETCompiler
{
	typedef std::vector< std::string >	TokenList;
	
	/*
	** ���֣�Imm2asm
	** ˵�������м����ת��Ϊ�����룬��Ҫ���ű�
	**		ɨ��3�飺	
	**				��һ�飺չ������
	**				�ڶ��飺����Label
	**				�����飺����ƫ�ƣ�����Ϊ���
	**
	*/
	class Imm2asm
	{
		typedef std::map< std::string, int > StrIntMap;
		bool				m_bNewStmt;		// ���λ�����ڱ�����Ƿ�������������·���Ĵ���
		Environment*		m_pTables;		// ���ű�
		int					m_label;		// ���һ��Label
		int					m_lineno;		// ��ǰ��������кţ���0��ʼ
		int					m_regBaseNum;	// �Ĵ�����ʼ���������ڵ�����ʱ������ߵ�����
		std::string			m_asmcode;		// ת���ɵĻ�����
		std::vector< int >	m_jumpMap;		// Label��ʵ�ʴ����кŵ�ӳ��
		StrIntMap			m_entryMap;
		Table*				m_pCurTable;
		static const int SizeofJumpMap = 500;

	public:
		Imm2asm();
		// ���м����ת��ΪET�����Ļ�����
		std::string TranslateToAsm( const std::string& imm, const Environment& tables );
		std::string ExpandCode( const std::string& imm );

	private:
		void TranslateLine( std::string& line );
		// ��һ���м������Ϊһ����Token
		TokenList str2Tok( std::string &line );
		// ��һ���ַ����ĵڶ�λ�Լ����������תΪ�������ͣ���һλͨ�����ڱ��
		int combinationToInt( const std::string& num );
		// Ϊ��ʱ��������Ĵ���
		std::string temp2reg( const std::string& temp );	
		// ����Labelʵ��λ��
		void LocateLabel( const std::string& imm );
		// ����һ��
		void _translateLine( TokenList& tokenList );
		std::string _if( TokenList& tokenList );
		std::string _iffalse( TokenList& tokenList );
		std::string _goto( TokenList& tokenList );
		// ���ɸ�ֵ���
		std::string GenAssign( TokenList& tokenList );
		// ���üĴ���������Ϊ����Ĵ����ṩ��Ҫ����
		void SetRegBaseNum( TokenList& tokenList );
		// ���code��������
		void AddToASM( const std::string& code );
	};

	

	


}