/*
** ���ߣ�����
** ˵����Parser��Ҫ��ͷ�ļ��Ͷ���
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