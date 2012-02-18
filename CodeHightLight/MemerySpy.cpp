// MemerySpy.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeHightLight.h"
#include "MemerySpy.h"
#include <list>
#include "PropertiesWnd.h"
#include "MessageDefinition.h"
#include "CExecuteController.h"

// MemerySpy

IMPLEMENT_DYNAMIC(MemorySpy, CDockablePane)

	MemorySpy::MemorySpy()
{

}

MemorySpy::~MemorySpy()
{
}


BEGIN_MESSAGE_MAP(MemorySpy, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_MESSAGE(WM_INSERT_ITEM, &MemorySpy::OnInsertItem)
	ON_MESSAGE(WM_CLEAR_ALL_ITEMS, &MemorySpy::OnClearAllItems)
END_MESSAGE_MAP()



// MemerySpy 消息处理程序




void MemorySpy::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码

	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
	CRect rectClient,rectProperty;
	GetClientRect(rectClient);
	rectProperty = m_wndPropList.GetListRect();
	m_wndPropList.SetWindowPos( NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER );
}


int MemorySpy::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList.Create( WS_CHILD | WS_VISIBLE , rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}
	PostMessage(WM_SIZE);
	return 0;
}

void MemorySpy::showVariableValue( const CString& valueName, const CString& theValue )
{
	m_wndPropList.AddProperty( new CMFCPropertyGridProperty( valueName, (_variant_t) theValue, _T("")));
	PostMessage(WM_PAINT);
}

void MemorySpy::clearAll()
{
	m_wndPropList.RemoveAll();
	PostMessage(WM_PAINT);
}

afx_msg LRESULT MemorySpy::OnInsertItem(WPARAM wParam, LPARAM lParam)
{
	//clearAll();
	std::list< PropertiesInsertItem >* itemList = (std::list< PropertiesInsertItem >*)wParam;

	int flag = 0;
	auto itemIter = itemList->begin();

	for ( int i = 0; i < m_wndPropList.GetPropertyCount() && itemIter!=itemList->end(); i++,++itemIter  )
	{
		flag = 0;
		auto prop = m_wndPropList.GetProperty( i );
		if (prop->GetName() == itemIter->varname)
		{		
			flag = 1;
			if ( itoa( itemIter->val ).c_str() != (CString)prop->GetValue() )
			{ 
				prop->SetValue( itoa( itemIter->val).c_str() ); 					
			}
		}
	}

	if (flag == 0)
	{
		clearAll();
		for ( itemIter = itemList->begin(); itemIter != itemList->end() ; ++itemIter)
		{
			m_wndPropList.AddProperty( new CMFCPropertyGridProperty( itemIter->varname.c_str(), (_variant_t) itoa( itemIter->val ).c_str(), _T("")));
		} 
	}



	delete itemList;
	return 0;
}




		



	afx_msg LRESULT MemorySpy::OnClearAllItems(WPARAM wParam, LPARAM lParam)
	{
		//clearAll();

		return 0;
	}
