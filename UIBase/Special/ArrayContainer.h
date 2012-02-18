/*
** ���ߣ�����
** ��������
**
**
*/
#ifndef _ARRAYCONTAINER_H
#define _ARRAYCONTAINER_H

#include "base.h"
#include <GdiPlus.h>

struct ArrayStruct
{
	int* arr;
	int size;
};

class ArrayContainer : public Container
{
private:
	int *old;
	int *status;
	int First;
	int changeFlag;
	int lastChangePos;
	int lastChangeVal;
public:
	DECLARE_REGISTER_FACTORY( ArrayContainer, CETObject )

public:
	struct ParamStruct
	{
		CDC* pDC;
		int flag;
	};
	// ���¼���Ԫ�ز���
	virtual void OnUIElementChangedEvent( CETObject* source, void* param );
	virtual void OnMouseLeftButtonDownEvent( CETObject* source, MouseEventProperty* param );

	int Draw( void* pointer);
	UIElement* OnClick( int x, int y );



};

#endif