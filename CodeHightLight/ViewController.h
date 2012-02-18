/*
** 作者：华亮
** 说明：
**
*/

#pragma once


enum GLOBALVARIABLE
{
	CODE_VIEW,
	INTER_VIEW,
	ASM_VIEW,
	OUT_VIEW
};



/*
** 名字：CViewController
** 说明：视图控制类，负责管理代码视图，中间代码视图、汇编代码视图和输出视图
**
*/
class CViewController
{
public:
	CViewController() 
	{
		Clear();
	}

	CView*				m_pCodeView;
	CView*				m_pIntermediateCodeView;
	CView*				m_pASMCodeView;
	CView*				m_pOutView;

	void Clear()
	{
		m_pCodeView = m_pIntermediateCodeView = m_pOutView = m_pASMCodeView = NULL;
	}

	void SetView( CView* pView, DWORD flag )
	{
		switch ( flag )
		{
		case CODE_VIEW:
			SetCodeView( pView );
			break;
		case ASM_VIEW:
			SetASMCodeView( pView );
			break;
		case INTER_VIEW:
			SetIntermeditateCodeView( pView );
			break;
		case OUT_VIEW:
			SetOutView( pView );
			break;
		default:
			break;
		}
	}

	void SetCodeView( CView* pView )
	{
		m_pCodeView = pView;
	}

	void SetIntermeditateCodeView( CView* pView )
	{
		m_pIntermediateCodeView = pView;
	}

	void SetASMCodeView( CView* pView )
	{
		m_pASMCodeView = pView;
	}

	void SetOutView( CView* pView )
	{
		m_pOutView = pView;
	}

	CView* GetCodeView() { return m_pCodeView; }
	CView* GetIntermediateView() { return m_pIntermediateCodeView; }
	CView* GetASMView() { return m_pASMCodeView; }
	CView* GetOutView() { return m_pOutView; }
};