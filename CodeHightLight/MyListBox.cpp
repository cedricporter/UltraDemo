// MyListBox.cpp : ʵ���ļ�
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



// MyListBox ��Ϣ�������




void MyListBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CListBox::OnLButtonDblClk(nFlags, point);
	::SendMessage ( m_parentWnd->GetSafeHwnd(), WM_LISTBOX_CLICK, 0, 0 );
}


void MyListBox::OnLbnDblclk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	::PostMessage ( m_parentWnd->GetSafeHwnd(), WM_LISTBOX_CLICK, 0, 0 );

}
