
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "AnimationWnd.h"
#include "CExecuteController.h"
#include "MemerySpy.h"
#include "OutPutResultWnd.h"
#include "AnimationFactory.h"
#include "MyKeyWordsDlg.h"
#include "AbstactSyntaxTree.h"

class CExecuteController;

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();


// Attributes
public:
	int			m_delayTime;
// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CAnimationWnd		m_wndAnimation;
	//CClassView			m_wndClassView;
	CExecuteController	m_execController;
	//CFileView			m_wndFileView;
	CMFCMenuBar			m_wndMenuBar;
	CMFCStatusBar		m_wndStatusBar;
	CMFCToolBar			m_wndToolBar;
	CMFCToolBarImages	m_UserImages;
	//COutputWnd			m_wndOutput;
	//CPropertiesWnd		m_wndProperties;
	MemorySpy			m_wndMemSpy;
	OutPutResultWnd		m_wndOutput;

	AnimationFactory*	m_pAnimationFactory;

	AnimationDialog		m_AnimationSelectiondDlg;	

	std::list<HINSTANCE> m_dllhInstList;
	std::map<std::string, AnimationFactory*> m_factoryList;

	AbstactSyntaxTree   m_wndAbstractSyntasTree;


public:
	void NewView();


// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();

	void InitialExecController();

	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnToolsCompile();
	afx_msg void OnToolsStep();
	afx_msg void OnToolsRun();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnToolsStop();
	afx_msg void OnToolsPause();
	afx_msg void OnToolsResume();
	afx_msg void OnToolsSpeedsetting();
	virtual BOOL DestroyWindow();
	afx_msg void OnToolsChooseAnimation();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	HACCEL m_hMyAccel;
public:
	afx_msg void OnDestroy();
	afx_msg void OnClose();
};


