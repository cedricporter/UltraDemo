#pragma once

#include "Base.h"
#include "MessageDefinition.h"
// CAnimation

class CAnimationWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CAnimationWnd)

protected:

public:
	Container*		m_pContainer;

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	enum
	{
		TIMERID_UPDATA,
		TIMEID_DRAW,
	};
public:
	static const int size = 10;
	int arr[ size ];

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

public:
	CAnimationWnd();
	virtual ~CAnimationWnd();

	void LoadAnimation( Container* animateContainer );

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

protected:
	afx_msg LRESULT OnUpdateAnimate(WPARAM wParam, LPARAM lParam);
};


