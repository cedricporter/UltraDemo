/*
** ���ߣ�����
** ��������Ԫ��
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

	CRect			rect( m_x, m_y, m_width, m_height );//�������

	Pen				blackPen(Color( 255, 50, 50, 50), 0 );//��ɫˢ��
	Pen				whitePen(Color( 100, 255, 255, 255), 0 );//��ɫˢ��

	FontFamily      fontFamily(L"Times New Roman");//ָ������
	Gdiplus::Font   font(&fontFamily, 24, FontStyleRegular, UnitPixel);//Font����

	SolidBrush      solidBrush(Color(230, 255, 204, 51));//����ɫ
	SolidBrush      changeBackBrush(Color(230, 255, 64, 0));//�ı��ı���ɫ
	SolidBrush		exchangeBackBrush(Color(230, 255, 255, 255));
	SolidBrush      wordBrush(Color(255, 0, 0, 0));//������ɫ
	SolidBrush		changeWordBrush(Color(255, 255, 255, 255));//�ı���������ɫ
	SolidBrush		exchangeWordBrush(Color(255, 0, 0, 0));

	PointF          pointWord( m_x + 5 , m_y + 5);//���ֵĲ��յ�

	graphics.DrawRectangle(&blackPen, rect.left, rect.top, m_width, m_height);

	CStringW strw;
	strw.Format( L"%d", GetValue() );//���ָ�ʽת��
	if (GetMode()==2)//����ı�
	{
		graphics.FillRectangle(&exchangeBackBrush, rect.left + 1 , rect.top + 1 , m_width - 1, m_height - 1);
		graphics.DrawString(strw, -1, &font, pointWord, NULL, &exchangeWordBrush);
		//MessageBox( NULL, _T("Exchange"), _T("Tips"), MB_OK );
	}
	else if (GetMode()==1)
	{
		graphics.FillRectangle(&changeBackBrush, rect.left + 1 , rect.top + 1 , m_width - 1, m_height - 1);//���ı�������Ԫ�ر�����ɫ
		graphics.DrawString(strw, -1,  &font, pointWord, NULL, &changeWordBrush);//ǿ���ı��Ԫ��
	}
	else//�������
	{
		graphics.FillRectangle(&solidBrush, rect.left + 1 , rect.top + 1 , m_width - 1, m_height - 1);//�������Ԫ�ر�����ɫ
		graphics.DrawString(strw, -1,  &font, pointWord, NULL, &wordBrush);//������������
	}

	return 0;
}