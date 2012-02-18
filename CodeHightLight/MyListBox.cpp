// MyListBox.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeHightLight.h"
#include "MyListBox.h"
#include "MessageDefinition.h"

// MyListBox

IMPLEMENT_DYNAMIC(MyListBox, CListBox)

MyListBox::MyListBox()
{
}

MyListBox::~MyListBox()
{
}
MyListBox::MyListBox( CDialogEx* hWnd ) : m_parentWnd( hWnd )
{
}


BEGIN_MESSAGE_MAP(MyListBox, CListBox)
	ON_WM_LBUTTONDBLCLK()
	ON_CONTROL_REFLECT(LBN_DBLCLK, &MyListBox::OnLbnDblclk)
END_MESSAGE_MAP()



// MyListBox 消息处理程序




void MyListBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CListBox::OnLButtonDblClk(nFlags, point);
	::SendMessage ( m_parentWnd->GetSafeHwnd(), WM_LISTBOX_CLICK, 0, 0 );
}


void MyListBox::OnLbnDblclk()
{
	// TODO: 在此添加控件通知处理程序代码
	::PostMessage ( m_parentWnd->GetSafeHwnd(), WM_LISTBOX_CLICK, 0, 0 );

}
