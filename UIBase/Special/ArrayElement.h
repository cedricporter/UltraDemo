/*
** 作者：安迪
** 数组元素
**
**
*/
#ifndef _ARRAYELEMENT_H
#define _ARRAYELEMENT_H

#include "base.h"
#include "ArrayContainer.h"
#include <string>
#include <GdiPlus.h>

class ArrayElement : public UIElement
{
	DECLARE_REGISTER_FACTORY( ArrayElement, CETObject )

private:
	int m_Val;
	int m_Mode;

public:
	void SetValue( int Val )
	{
		m_Val = Val;
	}

	int GetValue()
	{
		return m_Val;
	}

	void SetMode(int Mode)
	{
		m_Mode = Mode;
	}

	int GetMode()
	{
		return m_Mode;
	}

	int Draw( void* pointer );
};

#endif