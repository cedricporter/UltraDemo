/*
** ���ߣ�����
** ˵����
**
*/

#pragma once

#include "PropertiesWnd.h"

// CPropertiesWndThread

class CPropertiesWndThread : public CWinThread
{
	DECLARE_DYNCREATE(CPropertiesWndThread)

protected:
	CPropertiesWndThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CPropertiesWndThread();

public:
	CPropertiesWndThread( HWND pMainframe );

protected:
	HWND					m_wndParent;
	CEdit					m_Edit;
	CMFCPropertyGridCtrl	m_wndPropList;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


