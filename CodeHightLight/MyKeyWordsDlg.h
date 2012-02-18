#pragma once
#include "afxwin.h"
#include <map>
#include <list>
#include "MyListBox.h"
#include "Resource.h"

// MyKeyWordsDlg 对话框

class AnimationDialog : public CDialogEx
{
	DECLARE_DYNAMIC(AnimationDialog)

public:
	AnimationDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AnimationDialog();

// 对话框数据
	enum { IDD = IDD_MYKEYWORDSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()


	CListBox									m_KeyWords;
	MyListBox								    m_AnimationList;
	CEdit									    m_ChosenAnimation;
	std::map<CString, std::list<std::string>>   m_AnimationKeyWords;
	std::map<CString, CString>   m_AnimationDecription;

	CString	m_chosenAnimation;

public:
	bool AddAnimation( const CString& animationName, const std::list<std::string>& Map ); // 外部使用此函数填充对话框的成员m_AnimationKeyWords
	bool Initialization();
	bool GetSelectedAnimationName( CString& str )
	{
		str = m_chosenAnimation;
		if ( str.GetLength() )
		{
			return true;
		}
		return false;
	}
	bool addAnimationDescription ( const CString& animationName, const CString& descriptionList );
	
protected:
	afx_msg LRESULT OnListboxClick(WPARAM wParam, LPARAM lParam);
	CEdit m_DescriptionEdit;
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
};
