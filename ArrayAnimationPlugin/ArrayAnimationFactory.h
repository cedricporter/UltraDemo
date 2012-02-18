#pragma once

#include <GdiPlus.h>
#include "AnimationFactory.h"
#include "ArrayContainer.h"

using namespace ETCompiler;


/*
** ���֣�ArrayAnimationFactory
** ˵�������鶯������
**
*/
class ArrayAnimationFactory : public AnimationFactory
{
public:
	ArrayAnimationFactory();
	~ArrayAnimationFactory();
	Container* CreateContainer();
	WatchPropertiesFuncType   CreateWatchDogCallback();
};

LPVOID WatchList( WatchPropertiesListType* list );

AnimationFactory* CreateAnimationFactory();