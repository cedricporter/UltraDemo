
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "CodeHightLight.h"
#include "MessageDefinition.h"
#include <list>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////


CPropertiesWnd::CPropertiesWnd()
{
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_INSERT_ITEM, &CPropertiesWnd::OnInsertItem)
	ON_MESSAGE(WM_CLEAR_ALL_ITEMS, &CPropertiesWnd::OnClearAllItems)
	ON_MESSAGE(WM_OUTPUT, &CPropertiesWnd::OnOutput)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CRect rectClient,rectEdit,rectProperty;
	GetClientRect(rectClient);

	m_Edit.GetRect(&rectEdit);
	rectProperty = m_wndPropList.GetListRect();

	int cxEdit = rectEdit.Width();

	m_Edit.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width()/2 - 20, rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left + cxEdit + 40, rectClient.top, rectClient.Width()/2 , rectClient.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_Edit.Create( WS_CHILD | WS_VISIBLE | ES_MULTILINE  | ES_WANTRETURN |ES_AUTOVSCROLL | WS_VSCROLL, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}

	if (!m_wndPropList.Create( WS_CHILD | WS_VISIBLE , rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}

	PostMessage(WM_PAINT);
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}


void CPropertiesWnd::OnPaint()
{
	AdjustLayout();
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDockablePane::OnPaint()
	CPen* pOldPen, *pNewPen;
	pNewPen = new CPen( PS_SOLID , 40, RGB(200,200,200));
	CRect rectEdit, rectClient;
	m_Edit.GetRect( &rectEdit );
	GetClientRect(&rectClient);
	pOldPen = dc.SelectObject(pNewPen);
	CPoint pStar, pEnd;
	pStar.x = rectClient.left + rectEdit.Width() + 20;
	pStar.y = rectClient.top;
	pEnd.x = rectClient.left + rectEdit.Width() + 20;
	pEnd.y = rectClient.bottom;
	MoveToEx(dc,pStar.x,pStar.y,NULL);
	LineTo(dc,pEnd.x,pEnd.y );
	dc.SelectObject(pOldPen);
}

void CPropertiesWnd::showVariableValue( const CString& valueName, const CString& theValue )
{
	m_wndPropList.AddProperty( new CMFCPropertyGridProperty( valueName, (_variant_t) theValue, _T("")));
	PostMessage(WM_PAINT);
}

void CPropertiesWnd::clearAll()
{
	m_wndPropList.RemoveAll();
	PostMessage(WM_PAINT);
}

LRESULT CPropertiesWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类


	return CDockablePane::WindowProc(message, wParam, lParam);
}


afx_msg LRESULT CPropertiesWnd::OnInsertItem(WPARAM wParam, LPARAM lParam)
{
	//MessageBox( "e", "e", 0 );
	std::list< PropertiesInsertItem >* itemList = (std::list< PropertiesInsertItem >*)wParam;

	for ( auto iter = itemList->begin(); iter != itemList->end(); ++iter )
	{		
		showVariableValue( iter->varname.c_str(), itoa( iter->val ).c_str() );
	}

	delete itemList;

	return 0;
}


afx_msg LRESULT CPropertiesWnd::OnClearAllItems(WPARAM wParam, LPARAM lParam)
{
	clearAll();
	return 0;
}

void CPropertiesWnd::LoadText( const CString& text )
{
	m_Edit.SetWindowTextA( text );
}


afx_msg LRESULT CPropertiesWnd::OnOutput(WPARAM wParam, LPARAM lParam)
{
	CString* output = (CString*)wParam;
	LoadText( *output );
	delete output;

	return 0;
}
