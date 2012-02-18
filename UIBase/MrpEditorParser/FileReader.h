/*
** 作者：华亮
** 说明：文件读取类
**
*/
#pragma once

#include "Def.h"
#include <cassert>

class FileReader
{
	std::ifstream	m_inf;				
	CETString		m_file;
	std::size_t		m_pos;
	std::size_t		m_lineno;

public:
	FileReader() : m_pos( 0 ), m_lineno( 1 ) { }
	~FileReader() { CloseFile(); }

	int OpenFile( const CETString& filename );
	void CloseFile() { m_inf.close(); }
	bool SetPos( std::size_t pos );
	void Next() { if ( m_file[ m_pos++ ] == '\n' ) m_lineno++; }
	void Prev() { m_pos--; }
	int GetChar() { if (m_pos >= m_file.length()) {throw EOF;} return m_file[ m_pos ]; }
	int GetNextChar() { if (m_pos >= m_file.length()) {throw EOF;} Next(); return m_file[ m_pos - 1 ]; }
	std::size_t GetLineno() const { return m_lineno; }
};