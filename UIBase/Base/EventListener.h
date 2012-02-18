/*
** 作者：华亮
** 说明：这里是所有事件接口的定义
**
*/


#ifndef _EVENTLISTENER_H
#define _EVENTLISTENER_H

#include "ObjectBase.h"


/*
** 名字：Event
** 说明：所有事件的基类
**
*/
class Event
{
public:
	bool m_bHandled;

public:
	Event() : m_bHandled( false ) {}
	virtual ~Event() {}
};



// 元素改变事件监听器
class UIElementChangedListener
{
public:
	virtual void OnUIElementChangedEvent( CETObject* source, void* param ) = 0;
};



class MouseEventProperty : public Event
{
public:
	int m_x;
	int m_y;

	bool m_bHitTest;

	MouseEventProperty( int x = 0, int y = 0, bool bHit = false ) : m_x( x ), m_y( y ), m_bHitTest( bHit ) {}
	
};

/*
** 名字：MouseLeftButtonDownListener
** 说明：鼠标左键按下消息
**
*/
class MouseLeftButtonDownListener
{
public:
	virtual void OnMouseLeftButtonDownEvent( CETObject* source, MouseEventProperty* param ) = 0;
};


#endif