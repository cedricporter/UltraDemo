// MyKeyWordsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeHightLight.h"
#include "MyKeyWordsDlg.h"
#include "afxdialogex.h"
#include "MessageDefinition.h"

// MyKeyWordsDlg 对话框

IMPLEMENT_DYNAMIC(AnimationDialog, CDialogEx)

AnimationDialog::AnimationDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(AnimationDialog::IDD, pParent), m_AnimationList( this )
{
	
}

AnimationDialog::~AnimationDialog()
{
}

void AnimationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_KeyWords);
	DDX_Control(pDX, IDC_LIST2, m_AnimationList);
	DDX_Control(pDX, IDC_EDIT1, m_ChosenAnimation);
	DDX_Control(pDX, IDC_EDIT2, m_DescriptionEdit);
}

bool AnimationDialog::Initialization()
{
	m_ChosenAnimation.SetReadOnly(TRUE);
	m_DescriptionEdit.SetReadOnly(TRUE);
	skinppLoadSkin("skins\\skins\\Steel.ssk");
	std::map<CString, std::list<std::string>>::iterator pos;
	std::list<std::string>::iterator lpos;
	for ( pos = m_AnimationKeyWords.begin(); pos != m_AnimationKeyWords.end(); ++pos )
	{
		m_AnimationList.AddString(pos->first);
	}
	if  (m_AnimationList.GetCount() > 0 ) return true;
	else return false;
}

bool AnimationDialog::addAnimationDescription( const CString& animationName, const CString& descriptionList )
{
	m_AnimationDecription[ animationName ] = descriptionList;
	return true;
}

bool AnimationDialog::AddAnimation( const CString& animationName, const std::list<std::string>& Map )
{
	m_AnimationKeyWords[ animationName ] = Map;

	//std::map<CString, std::list<CString>>::iterator pos;
	//for ( pos = Map.begin(); pos != Map.end(); ++pos )
	//{
	//	m_AnimationKeyWords.insert(std::map<CString, std::list<CString>>::value_type( pos->first, pos->second ));
	//}
	return true;
}

BOOL AnimationDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData();

	if (Initialization())
	{
		UpdateData(FALSE);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
}

BEGIN_MESSAGE_MAP(AnimationDialog, CDialogEx)
	ON_MESSAGE(WM_LISTBOX_CLICK, &AnimationDialog::OnListboxClick)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &AnimationDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// MyKeyWordsDlg 消息处理程序




afx_msg LRESULT AnimationDialog::OnListboxClick(WPARAM wParam, LPARAM lParam)
{
	m_chosenAnimation.Empty();

	CString strTemp;
	int nCount;
	m_KeyWords.ResetContent();
	m_KeyWords.PostMessage(WM_PAINT);
	int pos;
	nCount = m_AnimationList.GetCount();
	pos = m_AnimationList.GetCurSel();
	if ( pos != LB_ERR )
	{
		m_AnimationList.GetText(pos,strTemp);
		std::list<std::string>::iterator lpos;

		for ( lpos = m_AnimationKeyWords[strTemp].begin(); lpos != m_AnimationKeyWords[strTemp].end(); ++lpos )
		{
			m_KeyWords.AddString( lpos->c_str() );
		}
		m_ChosenAnimation.SetWindowText( strTemp );
		m_chosenAnimation = strTemp;
		m_DescriptionEdit.SetWindowText( m_AnimationDecription[ strTemp ] );
	}
	else
	{
		MessageBox("Please Choose An Animation!");
	}
	return 0;

}


void AnimationDialog::OnDestroy()
{
	CDialogEx::OnDestroy();
	skinppExitSkin();

	// TODO: 在此处添加消息处理程序代码
}


void AnimationDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
