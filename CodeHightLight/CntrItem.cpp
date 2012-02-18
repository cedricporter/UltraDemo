
// CntrItem.cpp : implementation of the CCodeHightLightCntrItem class
//

#include "stdafx.h"
#include "CodeHightLight.h"

#include "CodeHightLightDoc.h"
#include "CodeHightLightView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCodeHightLightCntrItem implementation

IMPLEMENT_SERIAL(CCodeHightLightCntrItem, CRichEditCntrItem, 0)

CCodeHightLightCntrItem::CCodeHightLightCntrItem(REOBJECT* preo, CCodeHightLightDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: add one-time construction code here
}

CCodeHightLightCntrItem::~CCodeHightLightCntrItem()
{
	// TODO: add cleanup code here
}


// CCodeHightLightCntrItem diagnostics

#ifdef _DEBUG
void CCodeHightLightCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CCodeHightLightCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

