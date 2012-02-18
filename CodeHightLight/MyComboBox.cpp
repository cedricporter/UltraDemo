// MyComboBox.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeHightLight.h"
#include "MyComboBox.h"


// MyComboBox

IMPLEMENT_DYNAMIC(MyComboBox, CComboBox)

MyComboBox::MyComboBox()
{

}

MyComboBox::~MyComboBox()
{
}

BOOL MyComboBox::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	// 检查各条消息，如果是WM_KEYDOWN消息 则自动完成功能就是有效的，但如果是
	// Delete键或者是Backspace键按下，则
	// 说明用户此时正在修改文本，因此这时
	// 应该关闭自动完成功能

	if (pMsg->message == WM_KEYDOWN)
	{
		m_bAutoComplete = TRUE;

		int nVirtKey = (int) pMsg->wParam;
		if (nVirtKey == VK_DELETE || nVirtKey == VK_BACK)
			m_bAutoComplete = FALSE;
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(MyComboBox, CComboBox)
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, &MyComboBox::OnCbnEditupdate)
END_MESSAGE_MAP()



// MyComboBox 消息处理程序




void MyComboBox::OnCbnEditupdate()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: Add your control notification handler code here
	if (!m_bAutoComplete) // 如果当前自动完成模式无效，直接返回
		return;
	// 取得组合框中现有文本内容
	CString strText;
	GetWindowText(strText);
	int nLength = strText.GetLength();
	// 获得当前组合框中选择的文本范围
	DWORD dwCurSel = GetEditSel();
	WORD dStart = LOWORD(dwCurSel);
	WORD dEnd   = HIWORD(dwCurSel);	
	//从组合框的列表项中搜索是否有以制定文本串开头的列表项
	if (SelectString(-1, strText) == CB_ERR)
	{
		//如果没有文本被选中，则填入原来的字符串内容
		SetWindowText(strText);		
		if (dwCurSel != CB_ERR)
			SetEditSel(dStart, dEnd);//恢复原来的光标位置
	}

	// 选中刚刚自动增加的文本项内容
	if (dEnd < nLength && dwCurSel != CB_ERR)
		SetEditSel(dStart, dEnd);
	else
		SetEditSel(nLength, -1);
}
