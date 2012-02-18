
// CntrItem.h : interface of the CCodeHightLightCntrItem class
//

#pragma once

class CCodeHightLightDoc;
class CCodeHightLightView;

class CCodeHightLightCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CCodeHightLightCntrItem)

// Constructors
public:
	CCodeHightLightCntrItem(REOBJECT* preo = NULL, CCodeHightLightDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer

// Attributes
public:
	CCodeHightLightDoc* GetDocument()
		{ return reinterpret_cast<CCodeHightLightDoc*>(CRichEditCntrItem::GetDocument()); }
	CCodeHightLightView* GetActiveView()
		{ return reinterpret_cast<CCodeHightLightView*>(CRichEditCntrItem::GetActiveView()); }

// Implementation
public:
	~CCodeHightLightCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

