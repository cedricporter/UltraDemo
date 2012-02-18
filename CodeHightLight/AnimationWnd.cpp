// Animation.cpp : 实现文件
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



// CAnimationWnd 消息处理程序




int CAnimationWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	//m_pContainer = new ArrayContainer;
	//m_pContainer = new LinkedListContainer;

	PostMessage( WM_PAINT );

	return 0;
}


void CAnimationWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDockablePane::OnPaint()	

	CRect rect;
	GetClientRect( &rect );

	int nWidth = rect.Width();
	int nHeight = rect.Height();

	CDC   MemDC;   //首先定义一个显示设备对象   
	CBitmap   MemBitmap;//定义一个位图对象   

	//随后建立与屏幕显示兼容的内存显示设备   
	MemDC.CreateCompatibleDC(NULL);   
	//这时还不能绘图，因为没有地方画   ^_^   
	//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小   
	MemBitmap.CreateCompatibleBitmap( &dc, nWidth, nHeight );   

	//将位图选入到内存显示设备中   
	//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上   
	CBitmap   *pOldBit = MemDC.SelectObject(&MemBitmap);   

	//先用背景色将位图清除干净，这里我用的是白色作为背景   
	//你也可以用自己应该用的颜色   
	MemDC.FillSolidRect(0,0,nWidth,nHeight,RGB( 123, 123, 123 ));   
	
	if ( m_pContainer ) m_pContainer->Draw( &MemDC );

	//将内存中的图拷贝到屏幕上进行显示   
	dc.BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);   

	//绘图完成后的清理   
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

	m_pContainer->OnUIElementChangedEvent( (CETObject*)&param, (LPVOID)wParam );	// 由容器删除

	Invalidate();

	// 链表不能删
	//delete param;

	return 0;
}

void CAnimationWnd::LoadAnimation( Container* animateContainer )
{
	m_pContainer = animateContainer;
}
