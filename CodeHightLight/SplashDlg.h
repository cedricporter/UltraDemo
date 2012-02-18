#pragma once


// CSplashDlg 对话框

class CSplashDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSplashDlg)

public:
	CSplashDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSplashDlg();
	static void ShowSplashDlg( CWnd* pParentWnd );
	void HideSplashDlg();
// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};

static CSplashDlg* g_pSplashDlg;