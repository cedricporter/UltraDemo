#include "stdafx.h"
#include "FileReader.h"
#include <sstream>

namespace ETCompiler
{



	int FileReader::OpenFile( const CETString& filename ) 
	{ 
		Reset();

		m_inf.open( filename ); 

		std::stringstream in;
		in << m_inf.rdbuf();
		m_file = in.str();		// 转到string里面

		return m_inf.is_open(); 
	}

	bool FileReader::SetPos( std::size_t pos )
	{
		if ( 0 <= pos && pos < m_file.length() )
		{
			m_pos = pos;
			return true;
		}

		return false;
	}

	int FileReader::LoadFromString( const CETString& codeString )
	{
		Reset();

		m_file = codeString;

		return 0;
	}

	void FileReader::Reset()
	{
		m_pos = 0;
		m_lineno = 1;
	}


}