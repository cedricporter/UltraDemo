/*
** 作者：安迪
** 绘制汉诺塔容器
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
	int dishHeight = 20;//每个碟子的高度
	int dishWidth = 50;//碟子单位宽度
	int maxDepth = size;//汉诺塔的盘子个数，也就是栈的最大深度
	int Number = 1;//塔的序号
	int pointy, initpointy;
	int pointx[ 4 ];

	ParamStruct param;
	param.pDC = (CDC*)pointer;
	Graphics graphics( *(CDC*)pointer );

	Pen				blackPen(Color( 255, 50, 50, 50), 0 );//黑色刷子
	//Pen				whitePen(Color( 100, 255, 255, 255), 0 );//白色刷子

	FontFamily      fontFamily(L"Times New Roman");//指定字体
	Gdiplus::Font   font(&fontFamily, 24, FontStyleRegular, UnitPixel);//Font对象

	SolidBrush      solidBrush(Color( 230, 255, 204, 51));//背景色
	SolidBrush      changeBackBrush(Color( 230, 255, 64, 0));//改变后的背景色
	SolidBrush      wordBrush(Color( 255, 0, 0, 0));//字体颜色
	
	//第一个汉诺塔
	Number = 1;
	pointx[ Number ] = ptX * Number;
	pointx[ 0 ] = pointx[ Number ];
	initpointy = 20 + maxDepth * dishHeight;//最底下图形参照点
	pointy = initpointy;
	PointF point1( pointx[ Number ] + dishWidth * maxDepth / 2 - 10, initpointy + dishHeight + 10);//文字的参照点
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

	//第二个汉诺塔
	Number = 2;
	pointx[ Number ] = ptX * Number + dishWidth * maxDepth * ( Number - 1 );
	pointx[ 0 ] = pointx[ Number ];
	pointy = initpointy;//最底下图形参照点
	PointF point2( pointx[ Number ] + dishWidth * maxDepth / 2 - 10, initpointy + dishHeight + 10);//文字的参照点
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

	//第三个汉诺塔
	Number = 3;
	pointx[ Number ] = ptX * Number + dishWidth * maxDepth * (Number - 1);
	pointx[ 0 ] = pointx[ Number ];
	pointy = initpointy;//最底下图形参照点
	PointF point3( pointx[ Number ] + dishWidth * maxDepth / 2 - 10, initpointy + dishHeight + 10);//文字的参照点
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
