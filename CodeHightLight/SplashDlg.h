#pragma once


// CSplashDlg �Ի���

class CSplashDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSplashDlg)

public:
	CSplashDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSplashDlg();
	static void ShowSplashDlg( CWnd* pParentWnd );
	void HideSplashDlg();
// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};

static CSplashDlg* g_pSplashDlg;