#pragma once


// MemerySpy

class MemorySpy : public CDockablePane
{
	DECLARE_DYNAMIC(MemorySpy)

public:
	MemorySpy();
	virtual ~MemorySpy();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	CMFCPropertyGridCtrl	m_wndPropList;

public:
	void showVariableValue( const CString& valueName, const CString& theValue );// ����һ����¼
	void clearAll(); // ������м�¼
protected:
	afx_msg LRESULT OnInsertItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClearAllItems(WPARAM wParam, LPARAM lParam);
};


