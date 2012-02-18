/*
** 作者：安迪
** 绘制数组元素
**
*/

#include "stdafx.h"
#include "ArrayElement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_REGISTER_FACTORY( ArrayElement, CETObject )

int ArrayElement::Draw( void* pointer )
{
	using namespace Gdiplus;

	ArrayContainer::ParamStruct* param = (ArrayContainer::ParamStruct*)pointer;
	CDC* pDC = param->pDC;
	Graphics graphics( *pDC );

	CRect			rect( m_x, m_y, m_width, m_height );//定义矩形

	Pen				blackPen(Color( 255, 50, 50, 50), 0 );//黑色刷子
	Pen				whitePen(Color( 100, 255, 255, 255), 0 );//白色刷子

	FontFamily      fontFamily(L"Times New Roman");//指定字体
	Gdiplus::Font   font(&fontFamily, 24, FontStyleRegular, UnitPixel);//Font对象

	SolidBrush      solidBrush(Color(230, 255, 204, 51));//背景色
	SolidBrush      changeBackBrush(Color(230, 255, 64, 0));//改变后的背景色
	SolidBrush		exchangeBackBrush(Color(230, 255, 255, 255));
	SolidBrush      wordBrush(Color(255, 0, 0, 0));//字体颜色
	SolidBrush		changeWordBrush(Color(255, 255, 255, 255));//改变后的字体颜色
	SolidBrush		exchangeWordBrush(Color(255, 0, 0, 0));

	PointF          pointWord( m_x + 5 , m_y + 5);//文字的参照点

	graphics.DrawRectangle(&blackPen, rect.left, rect.top, m_width, m_height);

	CStringW strw;
	strw.Format( L"%d", GetValue() );//文字格式转换
	if (GetMode()==2)//如果改变
	{
		graphics.FillRectangle(&exchangeBackBrush, rect.left + 1 , rect.top + 1 , m_width - 1, m_height - 1);
		graphics.DrawString(strw, -1, &font, pointWord, NULL, &exchangeWordBrush);
		//MessageBox( NULL, _T("Exchange"), _T("Tips"), MB_OK );
	}
	else if (GetMode()==1)
	{
		graphics.FillRectangle(&changeBackBrush, rect.left + 1 , rect.top + 1 , m_width - 1, m_height - 1);//填充改变后的数组元素背景颜色
		graphics.DrawString(strw, -1,  &font, pointWord, NULL, &changeWordBrush);//强调改变后元素
	}
	else//如果不变
	{
		graphics.FillRectangle(&solidBrush, rect.left + 1 , rect.top + 1 , m_width - 1, m_height - 1);//填充数组元素背景颜色
		graphics.DrawString(strw, -1,  &font, pointWord, NULL, &wordBrush);//绘制正常文字
	}

	return 0;
}