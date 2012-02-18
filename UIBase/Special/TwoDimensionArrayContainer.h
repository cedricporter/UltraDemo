/*
** ���ߣ�����
** ˵����
**
*/
#pragma once
#include "Base.h"
#include <comdef.h>
#include <gdiplus.h>

struct TwoDimensionArrayContainerParam
{
	int*	arr;		// ��ά����ͷָ��
	int		height;		// ��ά����ά��
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