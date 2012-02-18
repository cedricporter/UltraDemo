/*
** 作者：华亮
** 说明：Parser需要的头文件和定义
**
*/
#pragma once

#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <stack>
#include <string>
#include <tchar.h>
#include <vector>
#include <afx.h>

#undef _UNICODE






//#ifdef _UNICODE
//#define CETString	std::wstring	
//#define TCHAR		wchar_t
//#define atoi		_wtoi
//#define ifstream wifstream
//#else
#ifndef CETString
#define CETString std::string
#define TCHAR		char
#endif

//#endif

typedef void* LPVOID;

#define SAFE_DELETE(p) do { if ( (p) != NULL ) { delete (p); (p) = NULL; } } while( 0 )

namespace ETCompiler
{
	typedef unsigned char	uchar;
	typedef uchar*			uchar_pointer;

	typedef unsigned long dword;
	typedef dword*	dword_pointer;

	std::string itoa( int num );

	struct PropertiesInsertItem
	{
		std::string varname;
		dword		val;

		bool operator==( const PropertiesInsertItem& rhs )
		{
			int ret = 0;
			ret = ( varname != rhs.varname );
			ret |= ( val != rhs.val );

			return !ret; 
		}

		friend bool operator==( const PropertiesInsertItem& lhs, const PropertiesInsertItem& rhs )
		{
			int ret = 0;
			ret = ( lhs.varname != rhs.varname );
			ret |= ( lhs.val != rhs.val );
			return !ret; 
		}
	}; 

	typedef std::list< PropertiesInsertItem > WatchPropertiesListType;
	typedef LPVOID (*WatchPropertiesFuncType)( WatchPropertiesListType* );
}