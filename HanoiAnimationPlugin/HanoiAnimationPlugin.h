// HanoiAnimationPlugin.h : HanoiAnimationPlugin DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHanoiAnimationPluginApp
// �йش���ʵ�ֵ���Ϣ������� HanoiAnimationPlugin.cpp
//

class CHanoiAnimationPluginApp : public CWinApp
{
public:
	CHanoiAnimationPluginApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
