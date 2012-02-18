/*
** 作者：华亮
** 说明：实现base.h里面的类定义
**
*/

#include "stdafx.h"
#include "Base.h"
#include <Windows.h>


//////////////////////////////////////////////////////////////////////////
//
// Class UIElement
// 

UIElement::UIElement() : m_width( 0 ), m_height( 0 ), m_x( 0 ), m_y( 0 ), m_pParent( NULL ), m_pAnchor( NULL ) {}

UIElement::~UIElement()
{ 
	if ( m_pAnchor ) m_pAnchor->DetachAnchor(); 

}


bool UIElement::AddUIElementChangedListener( UIElementChangedListener* pObserver )
{
	for ( std::list< UIElementChangedListener* >::iterator iter = m_UIElementChangedListeners.begin(); 
		iter != m_UIElementChangedListeners.end(); iter++ )
	{
		if ( (*iter) == pObserver )
		{
			return false;
		}
	}

	m_UIElementChangedListeners.push_back( pObserver );
	return true;
}


bool UIElement::RemoveUIElementChangedListener( UIElementChangedListener* pObserver )
{
	for ( std::list< UIElementChangedListener* >::iterator iter = m_UIElementChangedListeners.begin(); 
		iter != m_UIElementChangedListeners.end(); iter++ )
	{
		if ( (*iter) == pObserver )
		{
			m_UIElementChangedListeners.erase( iter );
			return true;
		}
	}
	return false;
}


void UIElement::PublishElementChangedEvent()
{
	for ( std::list< UIElementChangedListener* >::iterator iter = m_UIElementChangedListeners.begin(); 
		iter != m_UIElementChangedListeners.end(); iter++ )
	{
		if ( *iter )
		{
			(*iter)->OnUIElementChangedEvent( this, NULL );
		}
	}
}



//////////////////////////////////////////////////////////////////////////
//
// Class UIElementAnchorAttach
// 

UIElement* UIElementAnchor::Attach( UIElement* element ) 
{ 
	// 将自己从原来挂在的元素中卸除
	if ( m_pAnchoredUIElement ) m_pAnchoredUIElement->DetachAnchor();			
	if ( element ) element->AttachAnchor( this );	// 将自己挂在新的元素上
	UIElement* old = m_pAnchoredUIElement;	// 原来的元素用于返回
	m_pAnchoredUIElement = element;			// 
	return old;
}

UIElement* UIElementAnchor::Detach() 
{ 
	if ( m_pAnchoredUIElement ) m_pAnchoredUIElement->DetachAnchor();
	UIElement* old = m_pAnchoredUIElement;
	m_pAnchoredUIElement = NULL;
	return old; 
}
