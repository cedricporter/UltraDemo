// SplashDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeHightLight.h"
#include "SplashDlg.h"
#include "afxdialogex.h"


// CSplashDlg 对话框

IMPLEMENT_DYNAMIC(CSplashDlg, CDialogEx)

CSplashDlg::CSplashDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSplashDlg::IDD, pParent)
{

}

CSplashDlg::~CSplashDlg()
{
}

void CSplashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CSplashDlg::ShowSplashDlg( CWnd* pParentWnd )
{
	g_pSplashDlg = new CSplashDlg;
	if ( !g_pSplashDlg->Create( CSplashDlg::IDD, pParentWnd ))
	{
		delete g_pSplashDlg;
	}
	else
	{
		g_pSplashDlg->ShowWindow(SW_SHOW);
		g_pSplashDlg->UpdateWindow();
		g_pSplashDlg->SetTimer(1,2000,NULL);
	}

}

void CSplashDlg::HideSplashDlg()
{
	g_pSplashDlg->KillTimer(1);
	g_pSplashDlg->DestroyWindow();
	AfxGetMainWnd()->UpdateWindow();
	delete g_pSplashDlg;
	g_pSplashDlg = NULL;

}

BEGIN_MESSAGE_MAP(CSplashDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSplashDlg 消息处理程序


BOOL CSplashDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( g_pSplashDlg == NULL)
	{
		return FALSE;
	}
	if ( g_pSplashDlg->m_hWnd != NULL && pMsg->message == WM_KEYDOWN )
	{
		g_pSplashDlg->HideSplashDlg();
		return TRUE;
	}
	return FALSE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSplashDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HideSplashDlg();
}


BOOL CSplashDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	CenterWindow();
	SetWindowPos( &CWnd::wndTopMost, 0 ,0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
