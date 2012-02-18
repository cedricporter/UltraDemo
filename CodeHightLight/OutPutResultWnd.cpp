// OutPutResultWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeHightLight.h"
#include "OutPutResultWnd.h"
#include "MessageDefinition.h"


// OutPutResultWnd

IMPLEMENT_DYNAMIC(OutPutResultWnd, CDockablePane)

OutPutResultWnd::OutPutResultWnd()
{

}

OutPutResultWnd::~OutPutResultWnd()
{
}


BEGIN_MESSAGE_MAP(OutPutResultWnd, CDockablePane)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_OUTPUT, &OutPutResultWnd::OnOutput)
END_MESSAGE_MAP()



// OutPutResultWnd 消息处理程序




void OutPutResultWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDockablePane::OnPaint()
}


int OutPutResultWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_RichEdit.Create( WS_CHILD | WS_VISIBLE | ES_MULTILINE  | ES_WANTRETURN |ES_AUTOVSCROLL | WS_VSCROLL, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}
	m_RichEdit.SetReadOnly(TRUE);
	PostMessage(WM_SIZE);
	return 0;
}


void OutPutResultWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	CRect rectClient,rectRichEdit;
	GetClientRect(rectClient);
	m_RichEdit.GetRect(&rectRichEdit);
	int cxEdit = rectRichEdit.Width();
	m_RichEdit.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

}

void OutPutResultWnd::LoadText( const CString& text )
{
	m_RichEdit.SetWindowText( text );
}

afx_msg LRESULT OutPutResultWnd::OnOutput(WPARAM wParam, LPARAM lParam)
{
	CString* output = (CString*)wParam;
	LoadText( *output );
	delete output;

	return 0;
}
