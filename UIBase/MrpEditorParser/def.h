/*
** 作者：华亮
** 说明：Parser需要的头文件和定义
**
*/


#include <fstream>
#include <iostream>
#include <string>
#include <tchar.h>
#include <map>
#include <functional>

#ifdef _UNICODE
#define CETString	std::wstring	
#define TCHAR		wchar_t
#define atoi		_wtoi
//#define ifstream wifstream
#else
#define CETString std::string
#define TCHAR		char
#endif

typedef void* LPVOID;

#define SAFE_DELETE(p) do { if ( (p) != NULL ) { delete (p); (p) = NULL; } } while( 0 )