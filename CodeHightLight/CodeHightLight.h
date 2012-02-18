
// CodeHightLight.h : main header file for the CodeHightLight application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include <GdiPlus.h>



// CCodeHightLightApp:
// See CodeHightLight.cpp for the implementation of this class
//

class CCodeHightLightApp : public CWinAppEx
{
public:
	CCodeHightLightApp();

private:
	CMultiDocTemplate*	m_pDocTemplate;
	ULONG_PTR			m_gdiplusToken;

public:
	CMultiDocTemplate* GetDocTemplate()	{ return m_pDocTemplate; }

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CCodeHightLightApp theApp;
