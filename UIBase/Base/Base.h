/*
** 作者：华亮
** 说明：绘图元素的基类
**
*/


#ifndef _ANIMATEDBASE_H
#define _ANIMATEDBASE_H

#include <list>
#include "EventListener.h"


class UIElementAnchor;
class Container;

/*
** UIElement
** 说明： 所有UI元素的基类，提供基本的功能
**
*/
class UIElement : public CETObject
{
protected:
	int m_width;	// The width of the element
	int m_height;	// The height of the element

	int m_x;		// x of top left point 
	int m_y;		// y of top left point

	UIElement* m_pParent;	
	std::list< UIElementChangedListener* > m_UIElementChangedListeners;

	UIElementAnchor* m_pAnchor;

public:
	UIElement();
	~UIElement();

	virtual bool HitTest( int x, int y ) 
	{ if ( x >= m_x && x <= m_x + m_width && y >= m_y && y <= m_y + m_height ) { return true; } return false; }

	// Getter
	virtual int GetX() const { return m_x; }
	virtual int GetY() const { return m_y; }
	virtual int GetWidth() const { return m_width; }
	virtual int GetHeight() const { return m_height; }

	// Setter
	virtual bool SetX( int x ) { m_x = x; return true; }
	virtual bool SetY( int y ) { m_y = y; return true; }
	virtual bool SetWidth( int w ) { if ( w >= 0 ) { m_width = w; return true; } else return false; }
	virtual bool SetHeight( int h ) { if ( h >= 0 ) { m_height = h; return true; } else return false; }

	// Draw Function, you can override it to do your own drawing
	virtual int Draw( void* param ) = 0;

	UIElement* SetParent( UIElement* parent ) { UIElement* old = m_pParent; m_pParent = parent; return old; }
	const UIElement* GetParent() const { return m_pParent; }

	virtual UIElementAnchor* AttachAnchor( UIElementAnchor* element ) 
	{ 
		UIElementAnchor* old = m_pAnchor;
		m_pAnchor = element;
		return old;
	}
	virtual UIElementAnchor* DetachAnchor() 
	{ 
		UIElementAnchor* old = m_pAnchor;
		m_pAnchor = NULL;
		return old; 
	}

	//////////////////////////////////////////////////////////////////////////
	// Listeners APIs
	
	virtual bool AddUIElementChangedListener( UIElementChangedListener* pObserver );		// Add Listener	
	virtual bool RemoveUIElementChangedListener( UIElementChangedListener* pObserver );		// remove Listener	
	virtual void PublishElementChangedEvent();												// notifies the Listeners

	virtual UIElement* OnClick( int x, int y )
	{
		if ( HitTest( x, y ) ) return this;
		return NULL;
	}

};


/*
** 名字：UIElementAnchor
** 说明：依附在UIElement上面，可以用于修饰。
**
*/
class UIElementAnchor : public UIElement
{
protected:
	UIElement* m_pAnchoredUIElement;

public:
	UIElementAnchor() : m_pAnchoredUIElement( NULL ) {}

	virtual UIElement* Attach( UIElement* element );
	virtual UIElement* Detach();
};




/*
** Container
** 说明：容器，里面可以放置元素，会自动回收容器内部元素
**
*/
class Container : public UIElement, public UIElementChangedListener
{
protected:
	typedef std::list< UIElement* > UIList;
	UIList m_Children;		// contains all the UIElements as subjects in this container	

public:

	~Container()
	{
		////for ( UIList::iterator iter = m_Children.begin(); iter != m_Children.end(); iter++ )
		////{
		////	delete *iter;
		////}

		//m_Children.clear();
	}

	// Adds an UIElement into the m_Children
	virtual bool Add( UIElement* pUI )
	{
		for ( UIList::iterator iter = m_Children.begin(); iter != m_Children.end(); iter++ )
		{
			if ( *iter == pUI )
				return false;	// already exited, failed
		}

		pUI->SetParent( this );						// 设置元素的父母为这个容器
		m_Children.push_back( pUI );				// 添加子元素
		pUI->AddUIElementChangedListener( this );	// 订阅子元素的改变消息

		return true;
	}
	// Removes an UIElement from the m_Children
	virtual bool Remove( UIElement* pUI )
	{
		for ( UIList::iterator iter = m_Children.begin(); iter != m_Children.end(); iter++ )
		{
			if ( *iter == pUI )
			{
				m_Children.erase( iter );						// 删除子元素
				pUI->RemoveUIElementChangedListener( this );	// 取消订阅子元素的改变消息				
				pUI->SetParent( NULL );							// 设置元素的父母为空

				return true;
			}
		}

		return false;	// NOT exited
	}

	// Event Handlers
	virtual UIElement* OnClick( int x, int y )
	{
		for ( UIList::iterator iter = m_Children.begin(); iter != m_Children.end(); iter++ )
		{
			if ( (*iter)->HitTest( x, y ) )
				return *iter;
		}
		return NULL;
	}

	// Event Handlers
	virtual void OnUIElementChangedEvent( CETObject* source, void* param )
	{

	}

};






#endif