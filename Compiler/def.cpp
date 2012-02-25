/*
** 作者：华亮
** 说明：
**
*/

#include "stdafx.h"
#include "Def.h"
#include <string>

namespace ETCompiler
{
	std::string itoa( int num )
	{
		char str[ 16 ];
		_itoa_s( num, str, 10 );
		return str;
	}
}

