/*
** 作者：杨旭瑜
** 说明：
**
*/


#include "stdafx.h"
#include "SpeedDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(SpeedDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &SpeedDialog::OnBnClickedOk)
	ON_WM_HSCROLL()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

SpeedDialog::SpeedDialog() : CDialogEx( SpeedDialog::IDD ) 
{
	m_val = 0;
}

void SpeedDialog::DoDataExchange(CDataExchange* pDX)
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER_SPEED, m_val);
	DDV_MinMaxInt(pDX, m_val, 0, 100);
	DDX_Control(pDX, IDC_SLIDER_SPEED, m_speedSlider);

}


void SpeedDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData();
	CDialogEx::OnOK();
}


BOOL SpeedDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	skinppLoadSkin("skins\\skins\\Steel.ssk");
	// TODO:  在此添加额外的初始化
	m_speedSlider.SetRange( 0, 100 );
	m_speedSlider.SetPos( 0 );
	m_speedSlider.SetTicFreq( 10 );
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void SpeedDialog::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
	skinppExitSkin();
}
