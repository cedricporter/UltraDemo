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
	void showVariableValue( const CString& valueName, const CString& theValue );// 插入一条记录
	void clearAll(); // 清除所有记录
protected:
	afx_msg LRESULT OnInsertItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClearAllItems(WPARAM wParam, LPARAM lParam);
};


