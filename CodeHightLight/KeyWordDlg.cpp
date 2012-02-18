// KeyWordDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CodeHightLight.h"
#include "KeyWordDlg.h"
#include "afxdialogex.h"


// KeyWordDlg �Ի���

IMPLEMENT_DYNAMIC(KeyWordDlg, CDialogEx)

KeyWordDlg::KeyWordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(KeyWordDlg::IDD, pParent)
{
	
}

KeyWordDlg::~KeyWordDlg()
{
}

void KeyWordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_KeyWordName);
	DDX_Control(pDX, IDC_COMBO1, m_KeyWords);
	DDX_Control(pDX, IDC_LIST1, m_KeyWordsDisplays);
	DDX_Control(pDX, IDC_BUTTON1, m_btnInsert);
	DDX_Control(pDX, IDC_BUTTON2, m_btnDelete);
	DDX_Control(pDX, IDC_COMBO2, m_ChooseAnimation);
}


CString KeyWordDlg::getKeyWordsName()
{
	CString strTemp;
	m_KeyWordName.GetWindowText( strTemp );
	return strTemp;
}

void KeyWordDlg::addKeyWords( int Id, CString Name )
{
	m_KeyWords.InsertString( Id ,Name );
}

bool KeyWordDlg::saveWords( CString strWord )
{
	int pos;
	KeyWordElem kwe;
	char ch[100];
	CString strTemp,strHelp;
	pos = strWord.Find(" ");
	if ( pos == -1 || pos == 0 )
	{
		return false;
	}
	for ( int i = 0; i < pos; i++)
	{
		ch[i] = strWord[i];
	}
	ch[pos] = '\0';
	strHelp = ch;
	if ( strHelp == "int" )
	{
		kwe.KeyWord = 1;
	}
	else if ( strHelp == "int*")
	{
		kwe.KeyWord = 2;
	}
	else if ( strHelp == "LinkNode")
	{
		kwe.KeyWord = 3;
	}
	else if ( strHelp == "LinkNode*")
	{
		kwe.KeyWord = 4;
	}
	for ( int i = pos; i < strWord.GetLength(); i++ )
	{
		ch[ i - pos ] = strWord[i];
	}
	ch[ strWord.GetLength() - pos ] = '\0';
	strHelp = ch;
	strTemp = strHelp;
	strTemp.TrimLeft();
	strTemp.TrimRight();
	kwe.KeyWordName = strTemp;
	m_KeyWordlist.push_back(kwe);
	return true;
}



BEGIN_MESSAGE_MAP(KeyWordDlg, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON1, &KeyWordDlg::OnBnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON2, &KeyWordDlg::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDOK, &KeyWordDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KeyWordDlg ��Ϣ�������

void KeyWordDlg::OnBnClickedButtonInsert()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strName;
	strName = getKeyWordsName();
	if ( strName != "" )
	{
		int nCount = m_KeyWords.GetCount();      //��ȡComboBox��Ԫ�ظ���
		//��ȡComboBox�ĵ�ǰֵ
		int iPos = m_KeyWords.GetCurSel();       //��ǰѡ�е�Ԫ������
		CString strSelected;
		m_KeyWords.GetLBText( m_KeyWords.GetCurSel(),strSelected);    //��ǰѡ�е��ַ���
		char *pa = (char*)((LPCTSTR)strSelected);
		strSelected = strSelected + "              " + strName;
		m_KeyWordsDisplays.AddString( strSelected  );
	}
}


BOOL KeyWordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitialAlltheElem();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void KeyWordDlg::InitialAlltheElem()
{
	UpdateData(true);
	m_KeyWords.AddString("int");  
	m_KeyWords.AddString("int*");
	m_KeyWords.AddString("LinkNode");
	m_KeyWords.AddString("LinkNode*");

	m_ChooseAnimation.AddString("Array");
	m_ChooseAnimation.AddString("LinkList");

	UpdateData(false);
}


void KeyWordDlg::OnBnClickedBtnDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int pos;
	int nCount = m_KeyWordsDisplays.GetCount();
	pos = m_KeyWordsDisplays.GetCaretIndex();
	if ( pos != LB_ERR && nCount >= 1 )
	{
		m_KeyWordsDisplays.DeleteString(pos);
	}
	else
	{
		MessageBox("The Value List is Empty!");
	}
}


void KeyWordDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str, str2;
	for ( int i=0; i < m_KeyWordsDisplays.GetCount(); i++ )
	{
		m_KeyWordsDisplays.GetText( i, str);
		saveWords(str);
	}

	int nCount = m_ChooseAnimation.GetCount();      //��ȡComboBox��Ԫ�ظ���
	//��ȡComboBox�ĵ�ǰֵ
	int iPos = m_ChooseAnimation.GetCurSel();       //��ǰѡ�е�Ԫ������
	if ( iPos ==  CB_ERR )
	{
		MessageBox( "Please choose the Animation!" );
		return;
	}

	CString strSelected;
	m_ChooseAnimation.GetLBText( m_ChooseAnimation.GetCurSel(),strSelected );    //��ǰѡ�е��ַ���
	if ( strSelected == "Array" )
	{
		m_ChosenAnimation = 1;
	}
	else if ( strSelected == "LinkList" )
	{
		m_ChosenAnimation = 2;
	}
		
	nCount = m_KeyWords.GetCount();      //��ȡComboBox��Ԫ�ظ���
	//��ȡComboBox�ĵ�ǰֵ
    iPos = m_KeyWords.GetCurSel();       //��ǰѡ�е�Ԫ������
	if ( iPos == CB_ERR )
	{
		MessageBox( "Please choose the Key Word!" );
		return;

	}

	m_KeyWordName.GetWindowText( strSelected );
	if ( strSelected == "" )
	{
		MessageBox( "Please Input Variable Name!" );
		return;
	}
	
	CDialogEx::OnOK();
}
