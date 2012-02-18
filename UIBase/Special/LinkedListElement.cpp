/*
** 作者：杨明锦
**
**
*/

#include "stdafx.h"
#include "LinkedListElement.h"
#include "windows.h"
#include "GdiPlus.h"
#include "string.h"
const int mount=8;   //每行显示的节点的数目
using namespace Gdiplus; 
#pragma comment( lib, "gdiplus.lib" ) 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define arrowLength 30
IMPLEMENT_REGISTER_FACTORY( LinkedListElement, CETObject )



	int LinkedListElement::eraseArrowToMyself()                   //擦写自己本身
{
	endrawarrowtomyself=false;
	return 0;
}


int LinkedListElement::enableDrawArrowToMyself()
{
	endrawarrowtomyself=true;
	return 0;
}

int LinkedListElement::enableArrowToNext()
{
	endrawarrowtonext=true;
	return 0;
}

int LinkedListElement::enableDrawMyself()                   //enable to draw myself
{
	endrawmyself=true;
	return 0;
}
int LinkedListElement::enableDrawArrowToOtherNode(int sn)
{
	 arrowtoothernode=true;
	 m_othernodesn=sn;

	return 0;
}
int LinkedListElement::disableDrawMyself()                   //擦写自己本身
{
	endrawmyself=false;
	endrawarrowtonext=false;
	return 0;
}

int LinkedListElement::eraseArrowToNext()                   //擦写自己本身
{
	endrawarrowtonext=false;
	return 0;
}

int LinkedListElement::setPosition(int x,int y)
{
	m_x=x;
	m_y=y;
	m_arrowx=m_x+m_width-2;
	m_arrowy=m_x-m_height-arrowLength;
	return 0;

}

int LinkedListElement::getValue()
{
	return m_content;
}
int LinkedListElement::setValue(int x)
{
	m_content=x;
	return 0;
}
int LinkedListElement::getSn()
{
	return m_sn;
}
int LinkedListElement::setSn(int sn)
{
	m_sn=sn;
	setPosition(50+((m_sn%mount)*(m_width+arrowLength)) , 30+(3*m_height*(int(m_sn/mount))));

	return 0;
}





int LinkedListElement::shiftSn(char dir, int shift)
{
	if (dir=='R')
	{
		m_sn+=shift;
	}
	else if(dir=='L')
	{
		m_sn-=shift;
	}
	return 0;
}
LinkedListNode* LinkedListElement::getmypointer()
{
	return m_pointer;
}
int LinkedListElement::drawArrowToNext( void *pointer)
{
	if(endrawarrowtonext)
	{
		if(m_sn%mount!=(mount-1))
		{
			using namespace Gdiplus;
			CDC* pDC = (CDC*)pointer;
			Graphics graphics( *pDC );
			Pen myPen(Color(255, 69, 0, 255), m_width/20);
			myPen.SetEndCap(LineCapArrowAnchor);
			//graphics.DrawLine(&myPen , 55 , 55  ,77 , 77);
			graphics.DrawLine(&myPen , m_x + m_width , m_y + m_height/2  , m_x  + m_width + arrowLength , m_y + m_height/2);
		}
		else
		{
			using namespace Gdiplus;
			CDC* pDC = (CDC*)pointer;
			Graphics graphics( *pDC );
			//Pen myPen(Color(128, 255, 128, 255), m_width/20);
			Pen myPen(Color(255, 69, 0, 255), m_width/20);
			graphics.DrawLine(&myPen , 
				m_x + m_width , m_y + m_height/2  , m_x  + m_width + arrowLength/5 , m_y + m_height/2);
			graphics.DrawLine(&myPen ,
				m_x  + m_width + arrowLength/5 , m_y + m_height/2,  m_x  + m_width + arrowLength/5  , m_y + m_height+arrowLength/2*3);
			graphics.DrawLine(&myPen ,
				m_x  + m_width + arrowLength/5  , m_y + m_height+arrowLength/2*3 ,m_x  - m_width*(mount-1) - arrowLength/5*((mount-1)*5+1) ,  m_y + m_height+arrowLength/2*3);
			graphics.DrawLine(&myPen 
				,m_x  - m_width*(mount-1) - arrowLength/5*((mount-1)*5+1) ,  m_y + m_height+arrowLength/2*3,m_x  - m_width*(mount-1) - arrowLength*(mount-1)-arrowLength/5 ,  m_y + m_height*7/2);


			myPen.SetEndCap(LineCapArrowAnchor);
			graphics.DrawLine(&myPen , m_x  - m_width*(mount-1) - arrowLength/5*((mount-1)*5+1) ,   m_y + m_height*7/2  , m_x -m_width*(mount-1)-arrowLength*(mount-1), m_y + m_height*7/2);
		}
	}


	return 0;
}





int LinkedListElement::drawArrowToOtherNode(void *pointer)
{
	if(arrowtoothernode)
	{
	using namespace Gdiplus;
	CDC* pDC = (CDC*)pointer;
	Graphics graphics( *pDC );
	Pen myPen(Color(255, 69, 0, 255), m_width/20);
	graphics.DrawLine(&myPen , m_x  + (barrierwidth/2) , m_y + m_height , m_x + (barrierwidth/2) , m_y + m_height +m_height);
	int rowdiff;
	if(int(m_sn/mount)>int(  m_othernodesn/mount))
	{
		rowdiff =int(m_sn/mount)-int(  m_othernodesn/mount);
	}
	else if(int(m_sn/mount)<int(  m_othernodesn/mount))
	{
		rowdiff =int(m_sn/mount)-int(  m_othernodesn/mount);
	}
	else
	{
		rowdiff=0;
	}
	if(rowdiff==0)
	{
		/*if(m_sn>sn)*/
		{
			graphics.DrawLine(&myPen 
				, m_x + (barrierwidth/2) , m_y + m_height +m_height ,
				m_x -(m_sn-  m_othernodesn)*arrowLength-(m_sn-  m_othernodesn)*m_width , m_y + m_height +m_height);
		}
		/*else
		{
		graphics.DrawLine(&myPen 
		, m_x + (barrierwidth/2) , m_y + m_height +arrowLength+5 ,
		,m_x -(m_sn-sn)*arrowLength-(m_sn-sn-1)*m_width , m_y + m_height +arrowLength+5
		}*/
		myPen.SetEndCap(LineCapArrowAnchor);
		graphics.DrawLine(&myPen 
			,m_x -(m_sn-  m_othernodesn)*arrowLength-(m_sn-  m_othernodesn)*m_width ,m_y + m_height +m_height
			,m_x -(m_sn-  m_othernodesn)*arrowLength-(m_sn-  m_othernodesn)*m_width , m_y + m_height);

	}
	/*else if(rowdiff>0)
	{
	graphics.DrawLine(&myPen 
	, m_x + (barrierwidth/2) , m_y + m_height +arrowLength+5
	,(m_sn%4)*m_width-(m_sn%4-1)*arrowLength , m_y + m_height +arrowLength+5);

	graphics.DrawLine(&myPen 
	,(m_sn%4)*m_width-(m_sn%4-1)*arrowLength , m_y + m_height +arrowLength+5
	,(m_sn%4)*m_width-(m_sn%4-1)*arrowLength , m_y + rowdiff*3*m_height -2*arrowLength-10);

	graphics.DrawLine(&myPen 
	,(m_sn%4)*m_width-(m_sn%4-1)*arrowLength , m_y + m_height +arrowLength+5
	,50+(sn%4)*m_width+(sn%4-1)*arrowLength , m_y + rowdiff*3*m_height -2*arrowLength-10);

	myPen.SetEndCap(LineCapArrowAnchor);
	graphics.DrawLine(&myPen 
	,50+(sn%4)*m_width+(sn%4-1)*arrowLength , m_y + rowdiff*3*m_height -2*arrowLength-10
	,50+(sn%4)*m_width+(sn%4-1)*arrowLength , m_y + rowdiff*3*m_height -2*arrowLength-10+arrowLength);

	}*/
	else 
	{
		graphics.DrawLine(&myPen 
			, m_x + (barrierwidth/2) , m_y + m_height +m_height
			,30 , m_y + m_height +m_height);

		graphics.DrawLine(&myPen 
			,30 , m_y + m_height +m_height
			,30 , m_y +2*m_height-rowdiff*3*m_height );

		graphics.DrawLine(&myPen 
			,30 , m_y +2*m_height-rowdiff*3*m_height
			,50+(  m_othernodesn%4)*m_width+(  m_othernodesn%4)*arrowLength , m_y +2*m_height-rowdiff*3*m_height);

		myPen.SetEndCap(LineCapArrowAnchor);
		graphics.DrawLine(&myPen 
			,50+(  m_othernodesn%4)*m_width+(  m_othernodesn%4)*arrowLength , m_y +2*m_height-rowdiff*3*m_height
			,50+(  m_othernodesn%4)*m_width+(  m_othernodesn%4)*arrowLength , m_y +2*m_height-rowdiff*3*m_height-m_height);
	}



	}
	return 0;
}
int LinkedListElement::drawArrowToMyself( void *pointer)     //画属于自己的箭头
{
	using namespace Gdiplus;
	if(endrawarrowtomyself)
	{
		CDC* pDC = (CDC*)pointer;
		Graphics graphics( *pDC );
		Pen myPen(Color(255, 0, 0, 255), m_width/10);
		myPen.SetEndCap(LineCapArrowAnchor);
		//graphics.DrawLine(&myPen , 55 , 55  ,77 , 77);
		graphics.DrawLine(&myPen , m_x + (barrierwidth/2) , m_y + m_height +arrowLength  , m_x  + (barrierwidth/2) , m_y + m_height);

	}
	return 0;
}

int LinkedListElement::drawArrowToMyself( void *pointer,CStringW str)     //画属于自己的箭头
{
	using namespace Gdiplus;
	if(endrawarrowtomyself)
	{
		CDC* pDC = (CDC*)pointer;
		Graphics graphics( *pDC );
		Pen myPen(Color(255, 0, 0, 255), m_width/10);
		myPen.SetEndCap(LineCapArrowAnchor);
		//graphics.DrawLine(&myPen , 55 , 55  ,77 , 77);
		graphics.DrawLine(&myPen , m_x + (barrierwidth/2) , m_y + m_height +arrowLength  , m_x  + (barrierwidth/2) , m_y + m_height);
		FontFamily  fontFamily(L"Times New Roman");
		Gdiplus::Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
		PointF      pointF( m_x  + (barrierwidth/2*3), m_y+m_height);
		SolidBrush  solidBrush(Color(255, 0, 0, 255));
		graphics.DrawString(str, -1, &font, pointF, &solidBrush);

	}
	return 0;
}
int LinkedListElement::drawMyself( void *pointer)//画矩形和节点值
{
	using namespace Gdiplus;
	if(endrawmyself)
	{
		CDC* pDC = (CDC*)pointer;
		Graphics graphics( *pDC );
		Pen myPen(Color(255, 0, 0, 255), 2);
		Rect myRect(m_x, m_y, m_width, m_height);
		graphics.DrawRectangle(&myPen, myRect);
		graphics.DrawLine(&myPen , m_x + m_width - barrierwidth , m_y  , m_x + m_width - barrierwidth , m_y + m_height);
		FontFamily  fontFamily(L"Times New Roman");
		Gdiplus::Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
		PointF      pointF(m_x+2, m_y+m_height/5);
		SolidBrush  solidBrush(Color(255, 0, 0, 255));
		SolidBrush mySolidBrush(Color(255, 255, 128, 0));
		graphics.FillRectangle(&mySolidBrush, m_x, m_y, m_width - barrierwidth ,m_height);

		CStringW str; 
		str.Format( L"%d", m_content);
		graphics.DrawString(str, -1, &font, pointF, &solidBrush);
	}



	return 0;

}

int LinkedListElement::Draw( void* pointer )
{
	using namespace Gdiplus;

	

	return 0;
}