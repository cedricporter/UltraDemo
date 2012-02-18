/*
** 作者：华亮
** 说明：
**
*/

#include "stdafx.h"
#include "TwoDimensionArrayContainer.h"

TwoDimensionArrayContainer::TwoDimensionArrayContainer() : m_array( NULL ), m_arrayWidth( 0 ), m_arrayHeight( 0 )
{

}


int TwoDimensionArrayContainer::Draw( void* param )
{
	if ( m_array == NULL )	return -1;

	using namespace Gdiplus;

	Graphics graphics( *(CDC*)param );

	Pen				whitePen( Color( 100, 255, 255, 255 ), 0 );			
	SolidBrush      BlankBrush(	Color( 230, 173, 216, 230 ) );		//背景色
	SolidBrush      WallBrush(	Color( 255, 0, 0, 15 ) );			//背景色
	SolidBrush      ManBrush(	Color( 230, 255, 12, 12 ) );		//背景色
	SolidBrush      QueneBrush(	Color( 230, 1, 204, 51 ) );			//背景色
	SolidBrush      ETBrush(	Color( 230, 255, 204, 51 ) );			//背景色



	const int SIZE = 30;
	FontFamily      fontFamily(L"Times New Roman");//指定字体
	Gdiplus::Font   font( &fontFamily, 24, FontStyleRegular, UnitPixel );//Font对象
	PointF          pointWord( m_x + 5 , m_y + 5);//文字的参照点

	enum 
	{
		BLANK	= 0,
		WALL	= 1,
		MAN		= 2,
		QUEUE	= 3,
		ET		= 4
	};
	
	for ( int i = 0; i < m_arrayHeight; i++ )
	{
		for ( int j = 0; j < m_arrayWidth; j++ )
		{
			PointF point( j * SIZE, i * SIZE );

			switch ( m_array[ i * m_arrayWidth + j ] )
			{
			case 999:
			case BLANK:
				graphics.FillRectangle( &BlankBrush, j * SIZE, i * SIZE, SIZE, SIZE );

				break;
			case WALL:
				graphics.FillRectangle( &WallBrush, j * SIZE, i * SIZE, SIZE, SIZE );

				break;
			case MAN:
				graphics.FillRectangle( &ManBrush, j * SIZE, i * SIZE, SIZE, SIZE );

				break;
			case QUEUE:
				graphics.FillRectangle( &QueneBrush, j * SIZE, i * SIZE, SIZE, SIZE );

				break;
			case ET:
				graphics.FillRectangle( &QueneBrush, j * SIZE, i * SIZE, SIZE, SIZE );
				break;
			}

			graphics.DrawRectangle( &whitePen, j * SIZE, i * SIZE, SIZE, SIZE );


			//CStringW	wStr;
			//wStr.Format( L"%d", m_array[ i * m_arrayWidth + j ] );
			//graphics.DrawString( wStr, -1, &font, point, NULL, &solidBrush );
		}
	}

	return 0;
}

void TwoDimensionArrayContainer::OnUIElementChangedEvent( CETObject* source, void* param )
{
	TwoDimensionArrayContainerParam* par = (TwoDimensionArrayContainerParam*)param;

	if ( par->arr != NULL && m_array != par->arr )
	{
		m_array = par->arr;
		m_arrayHeight = par->height;
		m_arrayWidth = par->width;
	}
}


