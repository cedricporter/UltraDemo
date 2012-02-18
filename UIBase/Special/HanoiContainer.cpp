/*
** ���ߣ�����
** ���ƺ�ŵ������
**
*/

#include "stdafx.h"
#include "HanoiContainer.h"
#include <Windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_REGISTER_FACTORY( HanoiContainer, CETObject );

void HanoiContainer::OnUIElementChangedEvent( CETObject* source, void* param )
{
	if ( ! param )	return;

	HanoiStruct *par;
	par = (HanoiStruct *)param;

	if ( par->n <= 0 || par->n > 100 || par->start <= 0 || par->goal <= 0 || par->goal > 3 || par->start > 3 )	return;

	int start = par->start;
	int goal = par->goal;

	if ( First != 1 )
	{
		size = par->n;
		hanoi[ 1 ] = new int[ size ];
		hanoi[ 2 ] = new int[ size ];
		hanoi[ 3 ] = new int[ size ];

		for (int i=0; i<size; i++)
		{
			hanoi[ 1 ][ i ]= size - i;
		}
		top[ 1 ] = size - 1;
		top[ 2 ] = top[ 3 ] = -1;
		First = 1;
	}

	if ( ( start != startold || goal != goalold ) && ( top[ start ] != -1 ) && ( top[ goal ] != size - 1 ) )
	{
		hanoi[ goal ][ ++top[ goal ] ] = hanoi[ start ][ top[ start ]-- ];

		startold = start;
		goalold = goal;
		addstack = goal;
	}

}

int HanoiContainer::Draw( void* pointer )
{
	using namespace Gdiplus;

	const int ptX = 40;
	int dishHeight = 20;//ÿ�����ӵĸ߶�
	int dishWidth = 50;//���ӵ�λ���
	int maxDepth = size;//��ŵ�������Ӹ�����Ҳ����ջ��������
	int Number = 1;//�������
	int pointy, initpointy;
	int pointx[ 4 ];

	ParamStruct param;
	param.pDC = (CDC*)pointer;
	Graphics graphics( *(CDC*)pointer );

	Pen				blackPen(Color( 255, 50, 50, 50), 0 );//��ɫˢ��
	//Pen				whitePen(Color( 100, 255, 255, 255), 0 );//��ɫˢ��

	FontFamily      fontFamily(L"Times New Roman");//ָ������
	Gdiplus::Font   font(&fontFamily, 24, FontStyleRegular, UnitPixel);//Font����

	SolidBrush      solidBrush(Color( 230, 255, 204, 51));//����ɫ
	SolidBrush      changeBackBrush(Color( 230, 255, 64, 0));//�ı��ı���ɫ
	SolidBrush      wordBrush(Color( 255, 0, 0, 0));//������ɫ
	
	//��һ����ŵ��
	Number = 1;
	pointx[ Number ] = ptX * Number;
	pointx[ 0 ] = pointx[ Number ];
	initpointy = 20 + maxDepth * dishHeight;//�����ͼ�β��յ�
	pointy = initpointy;
	PointF point1( pointx[ Number ] + dishWidth * maxDepth / 2 - 10, initpointy + dishHeight + 10);//���ֵĲ��յ�
	graphics.DrawString(L"1", -1,  &font, point1, NULL, &wordBrush);

	for( int i = 0; i <= top[ Number ]; i++ )
	{
		if( i == 0 )
			pointx[ 0 ] = pointx[ Number ] + dishWidth * ( maxDepth - hanoi[ Number ][ i ] ) / 2;
		graphics.DrawRectangle( &blackPen, pointx[ 0 ], pointy, dishWidth * hanoi[ Number ][ i ], dishHeight);
		if( addstack == Number && i == top[ Number ] )
			graphics.FillRectangle( &changeBackBrush, pointx[ 0 ] + 1 , pointy + 1 , dishWidth * hanoi[ Number ][ i ] - 1, dishHeight - 1);
		else
			graphics.FillRectangle( &solidBrush, pointx[ 0 ] + 1 , pointy + 1 , dishWidth * hanoi[ Number ][ i ] - 1, dishHeight - 1);
		if(i < top[ Number ])
			pointx[ 0 ] += dishWidth / 2 * (hanoi[ Number ][ i ] - hanoi[ Number ][ i+1 ]);
		pointy -= dishHeight;
	}
	graphics.DrawLine(&blackPen , pointx[ Number ] + dishWidth * maxDepth / 2 , pointy + dishHeight  , pointx[ Number ] + dishWidth * maxDepth / 2 , 0);

	//�ڶ�����ŵ��
	Number = 2;
	pointx[ Number ] = ptX * Number + dishWidth * maxDepth * ( Number - 1 );
	pointx[ 0 ] = pointx[ Number ];
	pointy = initpointy;//�����ͼ�β��յ�
	PointF point2( pointx[ Number ] + dishWidth * maxDepth / 2 - 10, initpointy + dishHeight + 10);//���ֵĲ��յ�
	graphics.DrawString(L"2", -1,  &font, point2, NULL, &wordBrush);

	for( int i = 0; i <= top[ Number ]; i++ )
	{
		if( i == 0 )
			pointx[ 0 ] = pointx[ Number ] + dishWidth * ( maxDepth - hanoi[ Number ][ i ] ) / 2;
		graphics.DrawRectangle( &blackPen, pointx[ 0 ], pointy, dishWidth * hanoi[ Number ][ i ], dishHeight);
		if( addstack == Number && i == top[ Number ] )
			graphics.FillRectangle( &changeBackBrush, pointx[ 0 ] + 1 , pointy + 1 , dishWidth * hanoi[ Number ][ i ] - 1, dishHeight - 1);
		else
			graphics.FillRectangle( &solidBrush, pointx[ 0 ] + 1 , pointy + 1 , dishWidth * hanoi[ Number ][ i ] - 1, dishHeight - 1);
		if(i < top[ Number ])
			pointx[ 0 ] += dishWidth / 2 * (hanoi[ Number ][ i ] - hanoi[ Number ][ i+1 ]);
		pointy -= dishHeight;
	}
	graphics.DrawLine(&blackPen , pointx[ Number ] + dishWidth * maxDepth / 2 , pointy + dishHeight  , pointx[ Number ] + dishWidth * maxDepth / 2 , 0);

	//��������ŵ��
	Number = 3;
	pointx[ Number ] = ptX * Number + dishWidth * maxDepth * (Number - 1);
	pointx[ 0 ] = pointx[ Number ];
	pointy = initpointy;//�����ͼ�β��յ�
	PointF point3( pointx[ Number ] + dishWidth * maxDepth / 2 - 10, initpointy + dishHeight + 10);//���ֵĲ��յ�
	graphics.DrawString(L"3", -1, &font, point3, NULL, &wordBrush);

	for( int i = 0; i <= top[ Number ]; i++ )
	{
		if( i == 0 )
			pointx[ 0 ] = pointx[ Number ] + dishWidth * ( maxDepth - hanoi[ Number ][ i ] ) / 2;
		graphics.DrawRectangle( &blackPen, pointx[ 0 ], pointy, dishWidth * hanoi[ Number ][ i ], dishHeight);
		if( addstack == Number && i == top[ Number ] )
			graphics.FillRectangle( &changeBackBrush, pointx[ 0 ] + 1 , pointy + 1 , dishWidth * hanoi[ Number ][ i ] - 1, dishHeight - 1);
		else
			graphics.FillRectangle( &solidBrush, pointx[ 0 ] + 1 , pointy + 1 , dishWidth * hanoi[ Number ][ i ] - 1, dishHeight - 1);
		if(i < top[ Number ])
			pointx[ 0 ] += dishWidth / 2 * (hanoi[ Number ][ i ] - hanoi[ Number ][ i+1 ]);
		pointy -= dishHeight;
	}
	graphics.DrawLine(&blackPen, pointx[ Number ] + dishWidth * maxDepth / 2, pointy + dishHeight, pointx[ Number ] + dishWidth * maxDepth / 2, 0);
	graphics.DrawLine(&blackPen, 0, initpointy + dishHeight, 2000, initpointy + dishHeight);
	return 0;
}

UIElement* HanoiContainer::OnClick( int x, int y )
{
	return NULL; 
}
