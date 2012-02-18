/*
** 作者：杨旭瑜
** 说明：
**
*/

#pragma once

#include "resource.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "afxcmn.h"

class SpeedDialog : public CDialogEx
{
public:
	SpeedDialog();

	enum { IDD = IDD_DIALOG_SPEED };

protected:
	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange(CDataExchange* pDX);
public:
	int m_val;
	afx_msg void OnBnClickedOk();
	CSliderCtrl m_speedSlider;
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
};



