// CodeHighLight.h : CodeHighLight Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CCodeHighLightApp:
// �йش����ʵ�֣������ CodeHighLight.cpp
//

class CCodeHighLightApp : public CWinApp
{
public:
	CCodeHighLightApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCodeHighLightApp theApp;