// AnimationThread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CodeHightLight.h"
#include "AnimationThread.h"


// CAnimationThread

IMPLEMENT_DYNCREATE(CAnimationThread, CWinThread)

CAnimationThread::CAnimationThread()
{
}

CAnimationThread::~CAnimationThread()
{
}

BOOL CAnimationThread::InitInstance()
{
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	return TRUE;
}

int CAnimationThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CAnimationThread, CWinThread)
END_MESSAGE_MAP()


// CAnimationThread ��Ϣ�������
