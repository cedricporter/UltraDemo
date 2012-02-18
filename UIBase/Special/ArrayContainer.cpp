/*
** 作者：安迪
** 绘制数组容器
**
*/

#include "stdafx.h"
#include "ArrayContainer.h"
#include "ArrayElement.h"
#include <Windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_REGISTER_FACTORY( ArrayContainer, CETObject );

void ArrayContainer::OnMouseLeftButtonDownEvent( CETObject* source, MouseEventProperty* param )
{
	//// 找到被点击的子元素
	//std::list< UIElement* >::iterator iter;
	//for ( iter = m_Children.begin(); iter != m_Children.end(); iter++ )
	//{
	//	(*iter)->OnMouseLeftButtonDownEvent( this, param );
	//	if ( param->m_bHitTest )
	//	{
	//		break;
	//	}
	//}

	//// 这个纯属无聊，显示子元素信息
	//if ( iter != m_Children.end() )
	//{
	//	ArrayElement* element = dynamic_cast< ArrayElement* >( *iter );
	//	//element->SetY( element->GetY() + 20 );
	//	element->SetWidth( element->GetWidth() + 20 );
	//	element->SetHeight( element->GetHeight() + 20 );
	//	element->SetInfo( element->GetInfo() + _T("说：\n啊，我被点了~") );
	//}
}

void ArrayContainer::OnUIElementChangedEvent( CETObject* source, void* param )
{
	if ( ! param )	return;

	ArrayStruct *par;
	par = (ArrayStruct *)param;

	if ( par->arr == NULL )	return;
	if ( par->size < 0 || par->size > 10000 )	return;

	m_Children.clear();

	const int initx = 20;
	const int inity = 10;
	//const int freshPeriod = 3;
	int x = initx;//初始x坐标位置
	int y = inity;//初始y坐标位置
	int eWidth = 130;//数组元素宽度
	int eHeight = 30;
	int num = 7;



	int size = par->size;

	if ( First != 1 )
	{
		old = (int *) malloc( size * sizeof(int));
		status = (int *) malloc( size * sizeof(int));
		for (int i=0; i<size; i++)
		{
			old[i]= par->arr[i];
			status[i]= 0;
		}
		First = 1;
		lastChangeVal = par->arr[0];
		lastChangePos = 0;
	}


	changeFlag = 0;//上一次元素改变的位置

	for ( int i=0; i< size; i++)//比较元素，处理关系数组
	{
		if (par->arr[ i ] != old[ i ] )
		{
			for (int k= changeFlag; k < size; k++)
				status[ k ] = 0;
			changeFlag = i + 1;
			status[ i ] = 1;//与前一个数组状态不同

			if ( i != 0 )
			{
				if (par->arr[ i ] == lastChangeVal && old[ i ] == par->arr[ lastChangePos ] )
				{
					status[ i ] = 2;
					status[ lastChangePos ] = 2;
					//MessageBox( NULL, _T("Exchange"), _T("Tips"), MB_OK );
				}
			}
			lastChangePos = i;
			lastChangeVal = old[ i ];
		}
	}

	for ( int i=0; i < ( size/num ) + 1; i++ )//行排版
	{
		for (int j=0; j < (i == size/num ? size % num : num ) ; j++)//列排版
		{
			ArrayElement * e = new ArrayElement;
			e->SetX( x );
			e->SetY( y );
			e->SetWidth( eWidth );
			e->SetHeight( eHeight );
			e->SetValue( par->arr[ i * num + j ] );
			e->SetMode( status[ i * num + j ] );
			this->Add( e );
			x += eWidth;
			old[ i * num + j ] = par->arr[ i * num + j ];
		}
		y += (eHeight * 2);//y坐标往下移两倍元素高度
		x = initx;//x坐标恢复初始值
	}
}

int ArrayContainer::Draw( void* pointer )
{
	using namespace Gdiplus;

	ParamStruct param;
	param.pDC = (CDC*)pointer;

	Graphics graphics( *param.pDC );

	for ( std::list< UIElement* >::iterator iter = m_Children.begin(); iter != m_Children.end(); iter++ )
	{
		(*iter)->Draw( &param );
	}

	return 0;
}

UIElement* ArrayContainer::OnClick( int x, int y )
{
	return NULL; 
}
