/*
** ×÷Õß£º»ªÁÁ
**
**
*/

#ifndef _DEF_H
#define _DEF_H

#include "..\..\CompilerFrontPart\def.h"
#include <functional>
#include <list>
#include <string>


#ifdef _UNICODE
#define CETString	std::wstring	
#define TCHAR		wchar_t
#define atoi		_wtoi
//#define ifstream wifstream
#else
#define CETString std::string
#define TCHAR		char
#endif


#define TRUE 1
#define FALSE 0

#define SAFE_DELETE(p) do { if ( (p) != NULL ) { delete (p); (p) = NULL; } } while( 0 )





#endif