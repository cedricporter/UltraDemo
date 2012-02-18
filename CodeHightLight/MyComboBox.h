#pragma once


// MyComboBox

class MyComboBox : public CComboBox
{
	DECLARE_DYNAMIC(MyComboBox)

public:
	MyComboBox();
	virtual ~MyComboBox();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCbnEditupdate();

private:
	BOOL m_bAutoComplete;
};


