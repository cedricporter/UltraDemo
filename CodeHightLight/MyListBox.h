#pragma once


// MyListBox

class MyListBox : public CListBox
{
	DECLARE_DYNAMIC(MyListBox)

	MyListBox();
public:
	MyListBox( CDialogEx* hWnd );
	virtual ~MyListBox();
	CDialogEx*    m_parentWnd;
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:
	afx_msg LRESULT OnImClicked(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLbnDblclk();
};


