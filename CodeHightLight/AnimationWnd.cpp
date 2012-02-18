// Animation.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CodeHightLight.h"
#include "AnimationWnd.h"
#include "ArrayContainer.h"
#include "ArrayElement.h"
#include "LinkedListContainer.h"

// CAnimationWnd


IMPLEMENT_DYNAMIC(CAnimationWnd, CDockablePane)

CAnimationWnd::CAnimationWnd() : m_pContainer( NULL )
{

}

CAnimationWnd::~CAnimationWnd()
{
}


BEGIN_MESSAGE_MAP(CAnimationWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_MESSAGE(WM_UPDATE_ANIMATE, &CAnimationWnd::OnUpdateAnimate)
END_MESSAGE_MAP()



// CAnimationWnd ��Ϣ�������




int CAnimationWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	//m_pContainer = new ArrayContainer;
	//m_pContainer = new LinkedListContainer;

	PostMessage( WM_PAINT );

	return 0;
}


void CAnimationWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDockablePane::OnPaint()	

	CRect rect;
	GetClientRect( &rect );

	int nWidth = rect.Width();
	int nHeight = rect.Height();

	CDC   MemDC;   //���ȶ���һ����ʾ�豸����   
	CBitmap   MemBitmap;//����һ��λͼ����   

	//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸   
	MemDC.CreateCompatibleDC(NULL);   
	//��ʱ�����ܻ�ͼ����Ϊû�еط���   ^_^   
	//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С   
	MemBitmap.CreateCompatibleBitmap( &dc, nWidth, nHeight );   

	//��λͼѡ�뵽�ڴ���ʾ�豸��   
	//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��   
	CBitmap   *pOldBit = MemDC.SelectObject(&MemBitmap);   

	//���ñ���ɫ��λͼ����ɾ����������õ��ǰ�ɫ��Ϊ����   
	//��Ҳ�������Լ�Ӧ���õ���ɫ   
	MemDC.FillSolidRect(0,0,nWidth,nHeight,RGB( 123, 123, 123 ));   
	
	if ( m_pContainer ) m_pContainer->Draw( &MemDC );

	//���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ   
	dc.BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);   

	//��ͼ��ɺ������   
	MemBitmap.DeleteObject();   
	MemDC.DeleteDC();   

}

void CAnimationWnd::OnTimer( UINT_PTR nIDEvent )
{
	CDockablePane::OnTimer( nIDEvent );
}


afx_msg LRESULT CAnimationWnd::OnUpdateAnimate(WPARAM wParam, LPARAM lParam)
{
	//ArrayStruct* param = (ArrayStruct*)wParam;
	struct Param
	{
		CDC* pDC;
		int width;
		int height;
	};

	CPaintDC dc(this);

	CRect rect;
	GetClientRect( &rect );

	Param param;
	param.pDC = &dc;
	param.width = rect.Width();
	param.height = rect.Height();

	m_pContainer->OnUIElementChangedEvent( (CETObject*)&param, (LPVOID)wParam );	// ������ɾ��

	Invalidate();

	// ������ɾ
	//delete param;

	return 0;
}

void CAnimationWnd::LoadAnimation( Container* animateContainer )
{
	m_pContainer = animateContainer;
}
