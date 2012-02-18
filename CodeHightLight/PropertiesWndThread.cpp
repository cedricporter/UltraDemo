// PropertiesWndThread.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeHightLight.h"
#include "PropertiesWndThread.h"


// CPropertiesWndThread

IMPLEMENT_DYNCREATE(CPropertiesWndThread, CWinThread)

CPropertiesWndThread::CPropertiesWndThread()
{
}

CPropertiesWndThread::CPropertiesWndThread( HWND pMainframe ) : m_wndParent( pMainframe )
{

}



CPropertiesWndThread::~CPropertiesWndThread()
{
}

BOOL CPropertiesWndThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	//m_wndProperties.create
	// Create properties window
	

	return TRUE;
}

int CPropertiesWndThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CPropertiesWndThread, CWinThread)
END_MESSAGE_MAP()


// CPropertiesWndThread 消息处理程序
