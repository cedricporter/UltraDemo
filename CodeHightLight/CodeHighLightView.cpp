// CodeHighLightView.cpp : CCodeHighLightView 类的实现
//

#include "stdafx.h"
#include "CodeHighLight.h"

#include "CodeHighLightDoc.h"
#include "CntrItem.h"
#include "CodeHighLightView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCodeHighLightView

IMPLEMENT_DYNCREATE(CCodeHighLightView, CSynEditView)

BEGIN_MESSAGE_MAP(CCodeHighLightView, CSynEditView)
	ON_WM_DESTROY()
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CSynEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CSynEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSynEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CCodeHighLightView 构造/析构

CCodeHighLightView::CCodeHighLightView()
{
	// TODO: 在此处添加构造代码

}

CCodeHighLightView::~CCodeHighLightView()
{
}

BOOL CCodeHighLightView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CSynEditView::PreCreateWindow(cs);
}

void CCodeHighLightView::OnInitialUpdate()
{
	CSynEditView::OnInitialUpdate();


	// 设置打印边距(720 缇 = 1/2 英寸)
	SetMargins(CRect(720, 720, 720, 720));
}


// CCodeHighLightView 打印

BOOL CCodeHighLightView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}


void CCodeHighLightView::OnDestroy()
{
	// 停用处于析构中的项；这在
	// 使用拆分器视图时非常重要 
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
   CSynEditView::OnDestroy();
}



// CCodeHighLightView 诊断

#ifdef _DEBUG
void CCodeHighLightView::AssertValid() const
{
	CSynEditView::AssertValid();
}

void CCodeHighLightView::Dump(CDumpContext& dc) const
{
	CSynEditView::Dump(dc);
}

CCodeHighLightDoc* CCodeHighLightView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCodeHighLightDoc)));
	return (CCodeHighLightDoc*)m_pDocument;
}
#endif //_DEBUG


// CCodeHighLightView 消息处理程序
