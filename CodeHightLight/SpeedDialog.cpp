/*
** ���ߣ������
** ˵����
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
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER_SPEED, m_val);
	DDV_MinMaxInt(pDX, m_val, 0, 100);
	DDX_Control(pDX, IDC_SLIDER_SPEED, m_speedSlider);

}


void SpeedDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UpdateData();
	CDialogEx::OnOK();
}


BOOL SpeedDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	skinppLoadSkin("skins\\skins\\Steel.ssk");
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_speedSlider.SetRange( 0, 100 );
	m_speedSlider.SetPos( 0 );
	m_speedSlider.SetTicFreq( 10 );
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void SpeedDialog::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: �ڴ˴������Ϣ����������
	skinppExitSkin();
}
