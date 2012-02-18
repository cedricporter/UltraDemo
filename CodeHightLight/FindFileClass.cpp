#include "StdAfx.h"
#include "FindFileClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

FindFileClass::FindFileClass(void)
{
}


FindFileClass::~FindFileClass(void)
{
}

bool FindFileClass::getAllFileName( CString strPath  )
{
	CString   strWildcard ( strPath ); 
	CFileFind fileFinder;
	strWildcard += _T( "\\*.* "); 
	
	BOOL bWorking = fileFinder.FindFile( strWildcard  );
	while ( bWorking ) 
	{ 
		bWorking = fileFinder.FindNextFile(); 
		if ( fileFinder.IsDots() ) 
			continue; 

		if ( fileFinder.IsDirectory() ) 
		{ 
			continue;
		}
		m_FileNameList.push_back( fileFinder.GetFileName() );
	} 
	fileFinder.Close();
	return true;
}

FileNameList FindFileClass::getDllFileName( CString strPath )
{
	getAllFileName( strPath );
	FileNameList newList;
	if ( !m_FileNameList.empty() )
	{
		FileNameList::iterator iter;
		int pos;
		CString strTemp;
		for ( iter = m_FileNameList.begin(); iter != m_FileNameList.end(); ++iter)
		{
			strTemp = (*iter);
			strTemp.MakeLower();
			pos = strTemp.Find(".dll");
			if ( pos != -1 )
			{
				newList.push_back( (*iter) );
			}
		}
	}
	return newList;
}