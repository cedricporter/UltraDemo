/*
** 作者：华亮
** 说明：文件读取类
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

		int OpenFile( const CETString& filename );				// 打开文件	
		int LoadFromString( const CETString& codeString );		// 从字符串中加载
		void CloseFile() { m_inf.close(); }						// 关闭文件
		void Reset();
		bool SetPos( std::size_t pos );							// 设置当前指针位置
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