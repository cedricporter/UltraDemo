// CodeHighLightView.cpp : CCodeHighLightView ���ʵ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CSynEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CSynEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSynEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CCodeHighLightView ����/����

CCodeHighLightView::CCodeHighLightView()
{
	// TODO: �ڴ˴���ӹ������

}

CCodeHighLightView::~CCodeHighLightView()
{
}

BOOL CCodeHighLightView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CSynEditView::PreCreateWindow(cs);
}

void CCodeHighLightView::OnInitialUpdate()
{
	CSynEditView::OnInitialUpdate();


	// ���ô�ӡ�߾�(720 � = 1/2 Ӣ��)
	SetMargins(CRect(720, 720, 720, 720));
}


// CCodeHighLightView ��ӡ

BOOL CCodeHighLightView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}


void CCodeHighLightView::OnDestroy()
{
	// ͣ�ô��������е������
	// ʹ�ò������ͼʱ�ǳ���Ҫ 
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
   CSynEditView::OnDestroy();
}



// CCodeHighLightView ���

#ifdef _DEBUG
void CCodeHighLightView::AssertValid() const
{
	CSynEditView::AssertValid();
}

void CCodeHighLightView::Dump(CDumpContext& dc) const
{
	CSynEditView::Dump(dc);
}

CCodeHighLightDoc* CCodeHighLightView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCodeHighLightDoc)));
	return (CCodeHighLightDoc*)m_pDocument;
}
#endif //_DEBUG


// CCodeHighLightView ��Ϣ�������
