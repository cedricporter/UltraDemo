// CodeHighLightView.h : CCodeHighLightView 类的接口
//


#pragma once

#include "SynEditView.h"

class CCodeHighLightCntrItem;
class CCodeHighLightDoc;

class CCodeHighLightView : public CSynEditView
{
protected: // 仅从序列化创建
	CCodeHighLightView();
	DECLARE_DYNCREATE(CCodeHighLightView)

// 属性
public:
	CCodeHighLightDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

// 实现
public:
	virtual ~CCodeHighLightView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // CodeHighLightView.cpp 中的调试版本
inline CCodeHighLightDoc* CCodeHighLightView::GetDocument() const
   { return reinterpret_cast<CCodeHighLightDoc*>(m_pDocument); }
#endif

