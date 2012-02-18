#include "stdafx.h"
#include "FileReader.h"
#include <sstream>


int FileReader::OpenFile( const CETString& filename ) 
{ 
	m_inf.close();
	m_inf.open( filename ); 

	m_pos = 0;
	m_lineno = 1;

	std::stringstream in;
	in << m_inf.rdbuf();
	m_file = in.str();		// 转到string里面
	//std::cout << m_file;

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


