// MyComboBox.cpp : ʵ���ļ�
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
	// ��������Ϣ�������WM_KEYDOWN��Ϣ ���Զ���ɹ��ܾ�����Ч�ģ��������
	// Delete��������Backspace�����£���
	// ˵���û���ʱ�����޸��ı��������ʱ
	// Ӧ�ùر��Զ���ɹ���

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



// MyComboBox ��Ϣ�������




void MyComboBox::OnCbnEditupdate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: Add your control notification handler code here
	if (!m_bAutoComplete) // �����ǰ�Զ����ģʽ��Ч��ֱ�ӷ���
		return;
	// ȡ����Ͽ��������ı�����
	CString strText;
	GetWindowText(strText);
	int nLength = strText.GetLength();
	// ��õ�ǰ��Ͽ���ѡ����ı���Χ
	DWORD dwCurSel = GetEditSel();
	WORD dStart = LOWORD(dwCurSel);
	WORD dEnd   = HIWORD(dwCurSel);	
	//����Ͽ���б����������Ƿ������ƶ��ı�����ͷ���б���
	if (SelectString(-1, strText) == CB_ERR)
	{
		//���û���ı���ѡ�У�������ԭ�����ַ�������
		SetWindowText(strText);		
		if (dwCurSel != CB_ERR)
			SetEditSel(dStart, dEnd);//�ָ�ԭ���Ĺ��λ��
	}

	// ѡ�иո��Զ����ӵ��ı�������
	if (dEnd < nLength && dwCurSel != CB_ERR)
		SetEditSel(dStart, dEnd);
	else
		SetEditSel(nLength, -1);
}
