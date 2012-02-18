/*
** 作者：安迪
** 汉诺塔容器声明
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
	int top[ 4 ];//栈顶元素指针，-1时栈为空

public:
	DECLARE_REGISTER_FACTORY( HanoiContainer, CETObject )

public:
	struct ParamStruct
	{
		CDC* pDC;
		int flag;
	};
	// 重新计算元素布局
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