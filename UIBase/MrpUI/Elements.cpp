/*
** 作者：华亮
** 说明：
**
*/

#include "stdafx.h"
#include "Elements.h"
#include <GdiPlus.h>



//////////////////////////////////////////////////////////////////////////
//
// Class StateElement
// 

bool StateElement::RemovePredFrom( std::list< PredElement* >& preds, PredElement* pPred )
{
	preds.remove( pPred );

	return true;
}

int StateElement::Draw( void* param ) 
{
	using namespace Gdiplus;

	Graphics g( *(CDC*)param );

	Pen blackPen(Color( 255, 50, 50, 50), 0 );

	g.DrawRectangle( &blackPen, m_x, m_y, m_width, m_height );
	g.DrawEllipse( &blackPen, m_x, m_y, m_width, m_height );

	FontFamily      fontFamily(L"Times New Roman");
	Gdiplus::Font   font(&fontFamily, 12, FontStyleBold, UnitPixel);

	g.FillRectangle( &SolidBrush( Color( 100, 250, 255, 200 ) ), m_x, m_y, m_width, m_height >> 1 );

	SolidBrush      solidBrush(Color( 255, 250, 50, 50 ));
	PointF pointF( (REAL)m_x, (REAL)m_y );
	g.DrawString( m_wname.c_str(), -1, &font, pointF, NULL,&solidBrush);

	if ( m_pAnchor )	m_pAnchor->Draw( param );	// 绘制Anchor

	return 0;
}

int StateElement::OnDeleted() 
{
	// 从跳出谓词中删除自己
	std::for_each( m_PredJumpsOut.begin(), m_PredJumpsOut.end(), [&]( PredElement* element )
	{
		element->RemoveTarget();	// 将状态跳转谓词从跳到的目标中删除
		element->OnDeleted();
		//element->RemoveSource();
	});

	std::for_each( m_PredJumpsIn.begin(), m_PredJumpsIn.end(), [&]( PredElement* element )
	{
		element->RemoveSource();
		element->OnDeleted();
	});

	// Delete From source
	return (*m_pOnDeletedCB)( this );
}



//////////////////////////////////////////////////////////////////////////
//
// Class PredElement
// 

bool PredElement::HitTest( int x, int y )
{
	// 修正坐标
	int x1 = m_pSource->GetX() + m_pSource->GetWidth() / 2;
	int y1 = m_pSource->GetY() + m_pSource->GetHeight() / 2;
	int x2 = m_pTarget->GetX() + m_pTarget->GetWidth() / 2;
	int y2 = m_pTarget->GetY() + m_pTarget->GetHeight() / 2;

	double k = ( x1 != x2 ? double( y1 - y2 ) / double( x1 - x2 ) : 1 ); 

	int line_y = k * ( x - x1 ) + y1;

	int deltaHeight = 50;
	int deltaY = deltaHeight * sin( fabs( atan( k ) ) );
	int baseHeight = 5;

	if ( abs( y - line_y ) <= deltaY + baseHeight )
	{
		TRACE1("线被击中: %s", m_name.c_str() );
		return true;
	}
	else
	{
		return false;
	}
}

int PredElement::Draw( void* param ) 
{
	assert( m_pSource && m_pTarget );

	using namespace Gdiplus;

	Graphics g( *(CDC*)param );

	Pen blackPen(Color( 255, 250, 125, 125 ), 5 );
	blackPen.SetEndCap( LineCapArrowAnchor );

	int x1 = m_pSource->GetX();
	int y1 = m_pSource->GetY();
	int x2 = m_pTarget->GetX();
	int y2 = m_pTarget->GetY();

	int r = m_pSource->GetWidth() >> 1;
	double theta = atan2( double(y1 - y2), double(x1 - x2) );
	double sinTheta = sin( theta );
	double cosTheta = cos( theta );

	int dx = r * cosTheta;
	int dy = r * sinTheta;

	//GraphicsPath hPath( FillModeAlternate );	

	//g.DrawPath( &blackPen, &hPath );

	int sourceX = m_pSource->GetX() + (m_pSource->GetWidth() >> 1) - dx;
	int sourceY = m_pSource->GetY() + (m_pSource->GetHeight() >> 1) - dy;
	int targetX = m_pTarget->GetX() + (m_pTarget->GetWidth() >> 1) + dx;
	int targetY = m_pTarget->GetY() + (m_pTarget->GetHeight() >> 1) + dy;

	// 画连线
	g.DrawLine( &blackPen, 
		sourceX, 
		sourceY, 
		targetX, 
		targetY
		);

	//int anchorWidth = 20;
	//int anchorHeight = 30;




	return 0;
}


//////////////////////////////////////////////////////////////////////////
//
// Class InfoUIElementAnchor
// 

int InfoUIElementAnchor::Draw( void* param )
{
	if ( ! m_pAnchoredUIElement )
	{
		return -1;
	}

	using namespace Gdiplus;
	Graphics g( *(CDC*)param );

	if ( typeid( *m_pAnchoredUIElement ) == typeid( StateElement ) )
	{
		DrawStateAnchor(g);
	}
	else if ( typeid( *m_pAnchoredUIElement ) == typeid( PredElement ) )
	{
		DrawPredAnchor(g);
	}

	return 1;
}

void InfoUIElementAnchor::DrawStateAnchor( Gdiplus::Graphics &g )
{
	Gdiplus::SolidBrush brush( Gdiplus::Color( 125, 150, 0 ) );

	int radius = 10;
	int offsetX = radius >> 1;
	int offsetY = radius >> 1;

	g.FillEllipse( &brush, m_pAnchoredUIElement->GetX() - offsetX, 
		m_pAnchoredUIElement->GetY() - offsetY, 
		radius, radius );
	g.FillEllipse( &brush, m_pAnchoredUIElement->GetX() + m_pAnchoredUIElement->GetWidth() - offsetX, 
		m_pAnchoredUIElement->GetY() - offsetY, 
		radius, radius );
	g.FillEllipse( &brush, m_pAnchoredUIElement->GetX() - offsetX, 
		m_pAnchoredUIElement->GetY() + m_pAnchoredUIElement->GetHeight() - offsetY,
		radius, radius );
	g.FillEllipse( &brush, m_pAnchoredUIElement->GetX() + m_pAnchoredUIElement->GetWidth() - offsetX, 
		m_pAnchoredUIElement->GetY() + m_pAnchoredUIElement->GetHeight() - offsetY, 
		radius, radius );


	//g.FillRectangle( 
	//	&brush, m_pAnchoredUIElement->GetX(), m_pAnchoredUIElement->GetY(), 
	//	m_pAnchoredUIElement->GetWidth(), m_pAnchoredUIElement->GetHeight() );


}

void InfoUIElementAnchor::DrawPredAnchor( Gdiplus::Graphics &g )
{
	PredElement* pPred = dynamic_cast<PredElement*>( m_pAnchoredUIElement );

	Gdiplus::Pen blackPen( Gdiplus::Color( 255, 0, 255, 255 ), 5 );
	blackPen.SetEndCap( Gdiplus::LineCapArrowAnchor );

	int x1 = pPred->GetSource()->GetX();
	int y1 = pPred->GetSource()->GetY();
	int x2 = pPred->GetTarget()->GetX();
	int y2 = pPred->GetTarget()->GetY();

	int r = pPred->GetSource()->GetWidth() >> 1;
	double theta = atan2( double(y1 - y2), double(x1 - x2) );
	double sinTheta = sin( theta );
	double cosTheta = cos( theta );

	int dx = r * cosTheta;
	int dy = r * sinTheta;

	//GraphicsPath hPath( FillModeAlternate );	

	//g.DrawPath( &blackPen, &hPath );

	int sourceX = x1 + (pPred->GetSource()->GetWidth() >> 1) - dx;
	int sourceY = y1 + (pPred->GetSource()->GetHeight() >> 1) - dy;
	int targetX = x2 + (pPred->GetTarget()->GetWidth() >> 1) + dx;
	int targetY = y2 + (pPred->GetTarget()->GetHeight() >> 1) + dy;

	// »­Á¬Ïß
	g.DrawLine( &blackPen, 
		sourceX, 
		sourceY, 
		targetX, 
		targetY
		);
}
