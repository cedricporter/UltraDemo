// SplashDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CodeHightLight.h"
#include "SplashDlg.h"
#include "afxdialogex.h"


// CSplashDlg �Ի���

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


// CSplashDlg ��Ϣ�������


BOOL CSplashDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	HideSplashDlg();
}


BOOL CSplashDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CenterWindow();
	SetWindowPos( &CWnd::wndTopMost, 0 ,0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
