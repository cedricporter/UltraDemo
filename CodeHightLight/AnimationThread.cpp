// AnimationThread.cpp : 实现文件
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
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CAnimationThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CAnimationThread, CWinThread)
END_MESSAGE_MAP()


// CAnimationThread 消息处理程序
