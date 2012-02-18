// KeyWordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeHightLight.h"
#include "KeyWordDlg.h"
#include "afxdialogex.h"


// KeyWordDlg 对话框

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


// KeyWordDlg 消息处理程序

void KeyWordDlg::OnBnClickedButtonInsert()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strName;
	strName = getKeyWordsName();
	if ( strName != "" )
	{
		int nCount = m_KeyWords.GetCount();      //获取ComboBox中元素个数
		//获取ComboBox的当前值
		int iPos = m_KeyWords.GetCurSel();       //当前选中的元素索引
		CString strSelected;
		m_KeyWords.GetLBText( m_KeyWords.GetCurSel(),strSelected);    //当前选中的字符串
		char *pa = (char*)((LPCTSTR)strSelected);
		strSelected = strSelected + "              " + strName;
		m_KeyWordsDisplays.AddString( strSelected  );
	}
}


BOOL KeyWordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	InitialAlltheElem();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	CString str, str2;
	for ( int i=0; i < m_KeyWordsDisplays.GetCount(); i++ )
	{
		m_KeyWordsDisplays.GetText( i, str);
		saveWords(str);
	}

	int nCount = m_ChooseAnimation.GetCount();      //获取ComboBox中元素个数
	//获取ComboBox的当前值
	int iPos = m_ChooseAnimation.GetCurSel();       //当前选中的元素索引
	if ( iPos ==  CB_ERR )
	{
		MessageBox( "Please choose the Animation!" );
		return;
	}

	CString strSelected;
	m_ChooseAnimation.GetLBText( m_ChooseAnimation.GetCurSel(),strSelected );    //当前选中的字符串
	if ( strSelected == "Array" )
	{
		m_ChosenAnimation = 1;
	}
	else if ( strSelected == "LinkList" )
	{
		m_ChosenAnimation = 2;
	}
		
	nCount = m_KeyWords.GetCount();      //获取ComboBox中元素个数
	//获取ComboBox的当前值
    iPos = m_KeyWords.GetCurSel();       //当前选中的元素索引
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
