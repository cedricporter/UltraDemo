#pragma once


// OutPutResultWnd

class OutPutResultWnd : public CDockablePane
{
	DECLARE_DYNAMIC(OutPutResultWnd)

public:
	OutPutResultWnd();
	virtual ~OutPutResultWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

protected:
	CRichEditCtrl	 m_RichEdit;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	void LoadText( const CString& text );

protected:
	afx_msg LRESULT OnOutput(WPARAM wParam, LPARAM lParam);
};


