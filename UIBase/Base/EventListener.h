/*
** ���ߣ�����
** ˵���������������¼��ӿڵĶ���
**
*/


#ifndef _EVENTLISTENER_H
#define _EVENTLISTENER_H

#include "ObjectBase.h"


/*
** ���֣�Event
** ˵���������¼��Ļ���
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



// Ԫ�ظı��¼�������
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
** ���֣�MouseLeftButtonDownListener
** ˵����������������Ϣ
**
*/
class MouseLeftButtonDownListener
{
public:
	virtual void OnMouseLeftButtonDownEvent( CETObject* source, MouseEventProperty* param ) = 0;
};


#endif