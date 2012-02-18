
// CodeHightLightView.h : interface of the CCodeHightLightView class
//

#pragma once
#include "SynEditView.h"

class CCodeHightLightCntrItem;

class CCodeHightLightView : public CSynEditView
{
protected: // create from serialization only
	CCodeHightLightView();
	DECLARE_DYNCREATE(CCodeHightLightView)

// Attributes
public:
	CCodeHightLightDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CCodeHightLightView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnDestroy();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
};

#ifndef _DEBUG  // debug version in CodeHightLightView.cpp
inline CCodeHightLightDoc* CCodeHightLightView::GetDocument() const
   { return reinterpret_cast<CCodeHightLightDoc*>(m_pDocument); }
#endif

