#pragma once
#include "afxwin.h"
#include "afxvslistbox.h"
#include <list>
#include "MyComboBox.h"


// KeyWordDlg 对话框
struct KeyWordElem
{
	int KeyWord;
	CString KeyWordName;
};


class KeyWordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(KeyWordDlg)

public:
	KeyWordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~KeyWordDlg();

// 对话框数据
	enum { IDD = IDD_KEYWORDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CEdit                    m_KeyWordName;// 用户输入变量名字
	MyComboBox				 m_KeyWords;// 列举可以选择的关键字
	MyComboBox				 m_ChooseAnimation;// 列举可以选择的关键字
	CListBox			     m_KeyWordsDisplays;// 显示插入的关键字
	std::list<KeyWordElem>   m_KeyWordlist;// 保存显示出来的变量
	int                      m_ChosenAnimation;// 1: Array    2: LinkList
	CButton                  m_btnInsert; 
	CButton                  m_btnDelete;

private:
	CString getKeyWordsName(); // 得到用户输入的变量名字
	void addKeyWords( int Id, CString Name ); // 增加可以选择的关键字
	bool saveWords ( CString strWord );

public:
	afx_msg void OnBnClickedButtonInsert();
	virtual BOOL OnInitDialog();
	void InitialAlltheElem();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedOk();
	void getChosen();
	std::list<KeyWordElem> getKeyWord() { return m_KeyWordlist; } // 1: int, 2:int*, 3:LinkNode, 4:LinkNode*
	int getChosenAnimation() { return m_ChosenAnimation; }
	
};
