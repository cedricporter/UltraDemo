#pragma once
#include "afx.h"
#include <list>
typedef std::list<CString> FileNameList;
class FindFileClass :
	public CFileFind
{
public:
	FindFileClass(void);
	~FindFileClass(void);
private:
	bool getAllFileName ( CString strPath ); 
	FileNameList  m_FileNameList; 
public:
	FileNameList getDllFileName ( CString strPath );
};

