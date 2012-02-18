// CodeHighLightView.h : CCodeHighLightView ��Ľӿ�
//


#pragma once

#include "SynEditView.h"

class CCodeHighLightCntrItem;
class CCodeHighLightDoc;

class CCodeHighLightView : public CSynEditView
{
protected: // �������л�����
	CCodeHighLightView();
	DECLARE_DYNCREATE(CCodeHighLightView)

// ����
public:
	CCodeHighLightDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CCodeHighLightView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // CodeHighLightView.cpp �еĵ��԰汾
inline CCodeHighLightDoc* CCodeHighLightView::GetDocument() const
   { return reinterpret_cast<CCodeHighLightDoc*>(m_pDocument); }
#endif

