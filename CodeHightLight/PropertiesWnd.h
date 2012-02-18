
#pragma once

#include <string>
#include "def.h"

using namespace ETCompiler;




class CPropertiesWnd : public CDockablePane
{
// Construction
public:
	CPropertiesWnd();

	void AdjustLayout();

// Attributes
public:
	void showVariableValue( const CString& valueName, const CString& theValue );
	void clearAll();
	void LoadText( const CString& text );

protected:
	CRichEditCtrl			m_Edit;
	CMFCPropertyGridCtrl	m_wndPropList;

// Implementation
public:
	virtual ~CPropertiesWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	afx_msg LRESULT OnInsertItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClearAllItems(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOutput(WPARAM wParam, LPARAM lParam);
};

