/*
** 作者：华亮
** 说明：
**
*/
#pragma once
#include "Base.h"
#include <comdef.h>
#include <gdiplus.h>

struct TwoDimensionArrayContainerParam
{
	int*	arr;		// 二维数组头指针
	int		height;		// 二维数组维度
	int		width;
};

class TwoDimensionArrayContainer : public Container
{
	int*	m_array;
	int		m_arrayWidth;
	int		m_arrayHeight;

public:
	TwoDimensionArrayContainer();

	int Draw( void* param );

	void OnUIElementChangedEvent( CETObject* source, void* param );

};