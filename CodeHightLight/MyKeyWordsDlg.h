#pragma once
#include "afxwin.h"
#include <map>
#include <list>
#include "MyListBox.h"
#include "Resource.h"

// MyKeyWordsDlg �Ի���

class AnimationDialog : public CDialogEx
{
	DECLARE_DYNAMIC(AnimationDialog)

public:
	AnimationDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AnimationDialog();

// �Ի�������
	enum { IDD = IDD_MYKEYWORDSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()


	CListBox									m_KeyWords;
	MyListBox								    m_AnimationList;
	CEdit									    m_ChosenAnimation;
	std::map<CString, std::list<std::string>>   m_AnimationKeyWords;
	std::map<CString, CString>   m_AnimationDecription;

	CString	m_chosenAnimation;

public:
	bool AddAnimation( const CString& animationName, const std::list<std::string>& Map ); // �ⲿʹ�ô˺������Ի���ĳ�Աm_AnimationKeyWords
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
