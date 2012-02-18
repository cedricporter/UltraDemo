#pragma once

#include <fstream>
#include <string>
#include <sstream>

class CodeSupplier
{
public:
	std::string m_buffer;

	CodeSupplier() 
	{
		std::ifstream inf( "SAMPLE.hpp" );

		if ( inf.is_open() )
		{
			std::stringstream ss;
			inf >> ss.rdbuf();

			m_buffer = ss.str();
		}

	}

};