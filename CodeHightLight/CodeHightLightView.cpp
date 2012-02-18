
// CodeHightLightView.cpp : implementation of the CCodeHightLightView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CodeHightLight.h"
#endif

#include "CodeHightLightDoc.h"
#include "CntrItem.h"
#include "resource.h"
#include "CodeHightLightView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCodeHightLightView

IMPLEMENT_DYNCREATE(CCodeHightLightView, CSynEditView)

BEGIN_MESSAGE_MAP(CCodeHightLightView, CSynEditView)
	ON_WM_DESTROY()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CSynEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CSynEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCodeHightLightView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_SAVE, &CCodeHightLightView::OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, &CCodeHightLightView::OnFileOpen)
END_MESSAGE_MAP()

// CCodeHightLightView construction/destruction

CCodeHightLightView::CCodeHightLightView()
{
	// TODO: add construction code here

}

CCodeHightLightView::~CCodeHightLightView()
{
}

BOOL CCodeHightLightView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CSynEditView::PreCreateWindow(cs);
}

void CCodeHightLightView::OnInitialUpdate()
{
	CSynEditView::OnInitialUpdate();


	// Set the printing margins (720 twips = 1/2 inch)
	SetMargins(CRect(720, 720, 720, 720));
}


// CCodeHightLightView printing


void CCodeHightLightView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CCodeHightLightView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


void CCodeHightLightView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
   CSynEditView::OnDestroy();
}


void CCodeHightLightView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCodeHightLightView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCodeHightLightView diagnostics

#ifdef _DEBUG
void CCodeHightLightView::AssertValid() const
{
	CSynEditView::AssertValid();
}

void CCodeHightLightView::Dump(CDumpContext& dc) const
{
	CSynEditView::Dump(dc);
}

CCodeHightLightDoc* CCodeHightLightView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCodeHightLightDoc)));
	return (CCodeHightLightDoc*)m_pDocument;
}
#endif //_DEBUG


// CCodeHightLightView message handlers


void CCodeHightLightView::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(true);
	CString strFilter, fileName, strText, lpszDefExt;		// lpszDefExt 指定默认的文件扩展名,lpszFileName 指定默认的文件名
	lpszDefExt = ".hpp";
	GetWindowText( strText );
	strFilter="Hpp Files(*.hpp)|*.hpp||";
	CFileDialog dlg(FALSE, lpszDefExt, NULL, OFN_EXPLORER|OFN_HIDEREADONLY|
		OFN_ENABLESIZING|OFN_FILEMUSTEXIST, strFilter );
	if(dlg.DoModal() == IDOK )								       //显示保存文件对话框
	{
		fileName=dlg.GetPathName();
		CFile savefile(fileName,CFile::modeCreate|CFile::modeWrite);		//构造CFile对象
		savefile.Write( strText,strText.GetLength() );							//写文件数据
		savefile.Close();
	}
}


void CCodeHightLightView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码


	CString strFilter,fileName,strTempText;;
	strFilter="Hpp Files(*.hpp)|*.hpp|Text Files(*.txt)|*.txt||";	
	CFileDialog dlg(TRUE, NULL, NULL, OFN_EXPLORER|OFN_HIDEREADONLY|OFN_ENABLESIZING|OFN_FILEMUSTEXIST,strFilter);
	if(dlg.DoModal() == IDOK )								//显示打开文件对话框
	{
		fileName=dlg.GetPathName();
		CFile openfile(fileName,CFile::modeRead);				//构造CFile对象
		int length=openfile.GetLength();						//获取文件长度
		char *strText;
		strText=new char[ length + 1 ];	
		openfile.Read(strText,length);
		openfile.Close();										//关闭文件
		strText[length] = '\0';
		strTempText = strText;
		delete [] strText;
		UpdateData(false);
		CDocument* doc = theApp.GetDocTemplate()->OpenDocumentFile( NULL );
		POSITION pos = doc->GetFirstViewPosition();
		CView* pView = doc->GetNextView( pos );
		dynamic_cast<CCodeHightLightView*>( pView )->LoadText( strTempText );
	}
	else
		return;

	
}
