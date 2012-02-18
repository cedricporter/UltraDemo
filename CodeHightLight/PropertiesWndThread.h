/*
** 作者：华亮
** 说明：
**
*/

#pragma once

#include "PropertiesWnd.h"

// CPropertiesWndThread

class CPropertiesWndThread : public CWinThread
{
	DECLARE_DYNCREATE(CPropertiesWndThread)

protected:
	CPropertiesWndThread();           // 动态创建所使用的受保护的构造函数
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


