
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CodeHightLight.h"

#include "MainFrm.h"

#include "Imm2asm.h"
#include "SynEditView.h"
#include "Syntax.h"
#include "SpeedDialog.h"
//#include "KeyWordDlg.h"
#include "FindFileClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_TOOLS_COMPILE, &CMainFrame::OnToolsCompile)
	ON_COMMAND(ID_TOOLS_STEP, &CMainFrame::OnToolsStep)
	ON_COMMAND(ID_TOOLS_RUN, &CMainFrame::OnToolsRun)
	ON_WM_TIMER()
	ON_COMMAND(ID_TOOLS_STOP, &CMainFrame::OnToolsStop)
	ON_COMMAND(ID_TOOLS_PAUSE, &CMainFrame::OnToolsPause)
	ON_COMMAND(ID_TOOLS_KEYWORDSSETTING, &CMainFrame::OnToolsChooseAnimation)
	ON_COMMAND(ID_TOOLS_RESUME, &CMainFrame::OnToolsResume)
	ON_COMMAND(ID_TOOLS_SPEEDSETTING, &CMainFrame::OnToolsSpeedsetting)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() : m_delayTime( 50 )
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLACK);

	m_hMyAccel = NULL;

	/************************************************************************/
	/* 测试                                                                 */
	/************************************************************************/
	FindFileClass findClass;
	auto list = findClass.getDllFileName( "Plugins" );

	typedef AnimationFactory*(*CreateFactory)(); //宏定义函数指针类型 



	for ( auto iter = list.begin(); iter != list.end(); ++iter )
	{
		CreateFactory createFactoryFun; //函数指针 

		HINSTANCE hDll; //DLL句柄 
		hDll = LoadLibrary( "Plugins\\" + *iter ); 

		if (hDll != NULL) 
		{ 
			m_dllhInstList.push_back( hDll );

			createFactoryFun = (CreateFactory)GetProcAddress(hDll, "CreateAnimationFactory"); 
			if ( createFactoryFun != NULL ) 
			{ 
				AnimationFactory* animationFactory = (*createFactoryFun)();		// 创建动态工厂

				WatchedVarNameList* list = animationFactory->GetFactoryWatchedList();	// 获取工厂关注表

				m_AnimationSelectiondDlg.AddAnimation( animationFactory->GetFactoryName()->c_str(), *list );			
				m_AnimationSelectiondDlg.addAnimationDescription( animationFactory->GetFactoryName()->c_str(), 
					animationFactory->GetFactoryDescrption()->c_str() );
				
				m_factoryList[ *animationFactory->GetFactoryName() ] = animationFactory;	// 保存动画工厂
			} 

		}
	}

	
}

CMainFrame::~CMainFrame()
{
	m_execController.Stop();
	//Sleep( 1000 );

	for ( auto iter = m_dllhInstList.begin(); iter != m_dllhInstList.end(); ++iter )
	{
		//FreeLibrary( *iter );
	}

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = FALSE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	//m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndFileView);
	//CDockablePane* pTabbedBar = NULL;
	//m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	//m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndOutput);
	//m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndProperties);
	m_wndAnimation.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndAnimation);
	m_wndMemSpy.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMemSpy);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndAbstractSyntasTree.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndAbstractSyntasTree);

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_EDIT_SELECT_ALL);
	lstBasicCommands.AddTail(ID_EDIT_FIND);
	lstBasicCommands.AddTail(ID_EDIT_REPEAT);
	lstBasicCommands.AddTail(ID_EDIT_REPLACE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0, FWS_PREFIXTITLE);


	m_hMyAccel = LoadAccelerators( theApp.m_hInstance, ATL_MAKEINTRESOURCE(IDR_POPUP_EXPLOER) );
	if ( m_hMyAccel == NULL ) 
	{ 
		MessageBox( " fail ");
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	//// Create class view
	//CString strClassView;
	//bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	//ASSERT(bNameValid);
	//if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create Class View window\n");
	//	return FALSE; // failed to create
	//}

	//// Create file view
	//CString strFileView;
	//bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	//ASSERT(bNameValid);
	//if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create File View window\n");
	//	return FALSE; // failed to create
	//}

	//// Create output window
	//CString strOutputWnd;
	//bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	//ASSERT(bNameValid);
	//if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create Output window\n");
	//	return FALSE; // failed to create
	//}

	//// Create properties window
	//CString strPropertiesWnd;
	//bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	//ASSERT(bNameValid);
	//if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create Properties window\n");
	//	return FALSE; // failed to create
	//}

	CString strMemView;
	bNameValid = strMemView.LoadString( ID_VIEW_MEM_VIEW );
	ASSERT(bNameValid);
	if (! m_wndMemSpy.Create( strMemView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_MEM_VIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Memory window\n");
		return FALSE; // failed to create
	}

	CString strOutput;
	bNameValid = strOutput.LoadString( IDS_OUTPUT_WND );
	ASSERT(bNameValid);
	if (! m_wndOutput.Create( strOutput, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM  | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Output window\n");
		return FALSE; // failed to create
	}

	// Create animated window
	CString strAnimateWnd;
	bNameValid = strAnimateWnd.LoadString( IDS_ANIMATION_VIEW );
	ASSERT(bNameValid);
	if (!m_wndAnimation.Create(strAnimateWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_ANIMATIONWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Animation window\n");
		return FALSE; // failed to create
	}

	//bNameValid = strAnimateWnd.LoadString( IDS_ANIMATION_VIEW );
	strAnimateWnd = _T("Abstract Syntax Tree");
	ASSERT(bNameValid);
	if (!m_wndAbstractSyntasTree.Create(strAnimateWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_SYNTAX_TREE, WS_CHILD | 0/*WS_VISIBLE*/ | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Animation window\n");
		return FALSE; // failed to create
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);

	// 保存输出框和动画框到控制器
	//m_execController.SetPropertiesWnd( &m_wndProperties );
	InitialExecController();


	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	/*HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);*/

	UpdateMDITabbedBarsIcons();
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	//m_wndOutput.UpdateFonts();
}

void CMainFrame::OnToolsCompile()
{
	// TODO: 在此添加命令处理程序代码
	m_execController.Compile( this );
}

void CMainFrame::OnToolsStep()
{
	// TODO: 在此添加命令处理程序代码
	static bool bRun = true;

	m_execController.Step( this );
}

void CMainFrame::OnToolsRun()
{
	// TODO: 在此添加命令处理程序代码

	m_execController.AnimatedRun( m_delayTime );	
	//m_execController.Run( this );

}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	//m_wndAnimation.arr[ rand() % ( m_wndAnimation.size - 1 ) ] = rand() % 100;

	//struct Param
	//{
	//	int* arr;
	//	int size;
	//};

	//Param p;
	//p.arr = m_wndAnimation.arr;
	//p.size = m_wndAnimation.size;
	//m_wndAnimation.m_pContainer->OnUIElementChangedEvent( (CETObject*)this, (LPVOID)&p );


	//m_wndAnimation.Invalidate();
	


	CMDIFrameWndEx::OnTimer(nIDEvent);
}

void CMainFrame::NewView()
{
	theApp.GetDocTemplate()->OpenDocumentFile( NULL );
}

void CMainFrame::OnToolsStop()
{
	// TODO: 在此添加命令处理程序代码
	m_execController.Stop();
}

void CMainFrame::OnToolsPause()
{
	// TODO: 在此添加命令处理程序代码
	m_execController.Pause();

//	SetTimer( 0, 1000, 0 );

}

void CMainFrame::OnToolsResume()
{
	// TODO: 在此添加命令处理程序代码
	m_execController.Resume();
}

void CMainFrame::OnToolsSpeedsetting()
{
	// TODO: 在此添加命令处理程序代码

	SpeedDialog sDialog;
	sDialog.DoModal();

	m_delayTime = 50 + sDialog.m_val * 10;

}


BOOL CMainFrame::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_execController.Stop();

	return CMDIFrameWndEx::DestroyWindow();
}

void CMainFrame::OnToolsChooseAnimation()
{
	// TODO: 在此添加命令处理程序代码
	m_execController.Stop();	// 终止动画线程

	CString aniName;
	m_AnimationSelectiondDlg.DoModal();
	if ( m_AnimationSelectiondDlg.GetSelectedAnimationName( aniName ) )
	{
		AnimationFactory* factory = m_factoryList[ aniName.GetString() ];
		
		m_execController.LoadAnimationContainer( factory->CreateContainer() );
		m_execController.SetAnimateWatchCallback( factory->CreateWatchDogCallback() );

		m_execController.ClearInterestingList();	// 清空关注列表

		// 从DLL中读取新的关注表
		for ( WatchedVarNameList::iterator iter = factory->GetFactoryWatchedList()->begin();
			iter != factory->GetFactoryWatchedList()->end(); ++iter )
		{
			std::string varname = *iter;
			m_execController.AddWatchedVariable( *iter );
		}
	}
	//m_AnimationSelectiondDlg
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if( m_hMyAccel && TranslateAcceleratorA ( m_hWnd, m_hMyAccel, pMsg ) )
		return TRUE;
	return CMDIFrameWndEx::PreTranslateMessage(pMsg);
}

void CMainFrame::OnDestroy()
{
	m_execController.Stop();

	CMDIFrameWndEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}


void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_execController.Stop();

	CMDIFrameWndEx::OnClose();
}

void CMainFrame::InitialExecController()
{
	m_execController.SetAnimateWnd( &m_wndAnimation );
	m_execController.SetOutputWnd( &m_wndOutput );
	m_execController.SetMemSpyWnd( &m_wndMemSpy );
	m_execController.SetSyntaxTreeWnd( &m_wndAbstractSyntasTree );
}
