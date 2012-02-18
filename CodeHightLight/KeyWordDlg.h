#pragma once
#include "afxwin.h"
#include "afxvslistbox.h"
#include <list>
#include "MyComboBox.h"


// KeyWordDlg �Ի���
struct KeyWordElem
{
	int KeyWord;
	CString KeyWordName;
};


class KeyWordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(KeyWordDlg)

public:
	KeyWordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~KeyWordDlg();

// �Ի�������
	enum { IDD = IDD_KEYWORDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CEdit                    m_KeyWordName;// �û������������
	MyComboBox				 m_KeyWords;// �оٿ���ѡ��Ĺؼ���
	MyComboBox				 m_ChooseAnimation;// �оٿ���ѡ��Ĺؼ���
	CListBox			     m_KeyWordsDisplays;// ��ʾ����Ĺؼ���
	std::list<KeyWordElem>   m_KeyWordlist;// ������ʾ�����ı���
	int                      m_ChosenAnimation;// 1: Array    2: LinkList
	CButton                  m_btnInsert; 
	CButton                  m_btnDelete;

private:
	CString getKeyWordsName(); // �õ��û�����ı�������
	void addKeyWords( int Id, CString Name ); // ���ӿ���ѡ��Ĺؼ���
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
