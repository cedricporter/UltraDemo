// PropertiesWndThread.cpp : ʵ���ļ�
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
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	//m_wndProperties.create
	// Create properties window
	

	return TRUE;
}

int CPropertiesWndThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CPropertiesWndThread, CWinThread)
END_MESSAGE_MAP()


// CPropertiesWndThread ��Ϣ�������
