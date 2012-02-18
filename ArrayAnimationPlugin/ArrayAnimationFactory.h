#pragma once

#include <GdiPlus.h>
#include "AnimationFactory.h"
#include "ArrayContainer.h"

using namespace ETCompiler;


/*
** 名字：ArrayAnimationFactory
** 说明：数组动画工厂
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