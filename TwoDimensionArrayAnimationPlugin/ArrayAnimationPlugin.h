// ArrayAnimationPlugin.h : ArrayAnimationPlugin DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CArrayAnimationPluginApp
// �йش���ʵ�ֵ���Ϣ������� ArrayAnimationPlugin.cpp
//

class CArrayAnimationPluginApp : public CWinApp
{
public:
	CArrayAnimationPluginApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
