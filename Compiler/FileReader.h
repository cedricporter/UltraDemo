/*
** ���ߣ�����
** ˵�����ļ���ȡ��
**
*/
#pragma once

#include "Def.h"
#include <cassert>

namespace ETCompiler
{
	class FileReader
	{
		std::ifstream	m_inf;				
		CETString		m_file;
		std::size_t		m_pos;
		std::size_t		m_lineno;

	public:
		FileReader() : m_pos( 0 ), m_lineno( 1 ) { }
		~FileReader() { CloseFile(); }

		int OpenFile( const CETString& filename );				// ���ļ�	
		int LoadFromString( const CETString& codeString );		// ���ַ����м���
		void CloseFile() { m_inf.close(); }						// �ر��ļ�
		void Reset();
		bool SetPos( std::size_t pos );							// ���õ�ǰָ��λ��
		void Next() 
		{ if ( m_file[ m_pos++ ] == '\n' ) m_lineno++; }
		void Prev()
		{ m_pos--; }
		int GetChar()
		{ if (m_pos >= m_file.length()) {throw EOF;} return m_file[ m_pos ]; }
		int GetNextChar()
		{ if (m_pos >= m_file.length()) {throw EOF;} Next(); return m_file[ m_pos - 1 ]; }
		std::size_t GetLineno() const
		{ return m_lineno; }
	};


}