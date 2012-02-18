/*
** ���ߣ�����
** ������������
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
	//// �ҵ����������Ԫ��
	//std::list< UIElement* >::iterator iter;
	//for ( iter = m_Children.begin(); iter != m_Children.end(); iter++ )
	//{
	//	(*iter)->OnMouseLeftButtonDownEvent( this, param );
	//	if ( param->m_bHitTest )
	//	{
	//		break;
	//	}
	//}

	//// ����������ģ���ʾ��Ԫ����Ϣ
	//if ( iter != m_Children.end() )
	//{
	//	ArrayElement* element = dynamic_cast< ArrayElement* >( *iter );
	//	//element->SetY( element->GetY() + 20 );
	//	element->SetWidth( element->GetWidth() + 20 );
	//	element->SetHeight( element->GetHeight() + 20 );
	//	element->SetInfo( element->GetInfo() + _T("˵��\n�����ұ�����~") );
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
	int x = initx;//��ʼx����λ��
	int y = inity;//��ʼy����λ��
	int eWidth = 130;//����Ԫ�ؿ��
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


	changeFlag = 0;//��һ��Ԫ�ظı��λ��

	for ( int i=0; i< size; i++)//�Ƚ�Ԫ�أ������ϵ����
	{
		if (par->arr[ i ] != old[ i ] )
		{
			for (int k= changeFlag; k < size; k++)
				status[ k ] = 0;
			changeFlag = i + 1;
			status[ i ] = 1;//��ǰһ������״̬��ͬ

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

	for ( int i=0; i < ( size/num ) + 1; i++ )//���Ű�
	{
		for (int j=0; j < (i == size/num ? size % num : num ) ; j++)//���Ű�
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
		y += (eHeight * 2);//y��������������Ԫ�ظ߶�
		x = initx;//x����ָ���ʼֵ
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
