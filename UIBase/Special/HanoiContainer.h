/*
** ���ߣ�����
** ��ŵ����������
**
**
*/
#ifndef _HanoiCONTAINER_H
#define _HanoiCONTAINER_H

#include "base.h"
#include <GdiPlus.h>

struct HanoiStruct
{
	int n;
	int start;
	int goal;
};

class HanoiContainer : public Container
{
private:
	int *hanoi[ 4 ];
	int First;
	int size;
	int startold, goalold;
	int addstack;
	int top[ 4 ];//ջ��Ԫ��ָ�룬-1ʱջΪ��

public:
	DECLARE_REGISTER_FACTORY( HanoiContainer, CETObject )

public:
	struct ParamStruct
	{
		CDC* pDC;
		int flag;
	};
	// ���¼���Ԫ�ز���
	virtual void OnUIElementChangedEvent( CETObject* source, void* param );

	int Draw( void* pointer);
	UIElement* OnClick( int x, int y );

	HanoiContainer()
	{
		First = 0;
		size = 3;
	}

};

#endif