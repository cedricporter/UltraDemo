#pragma once

#include <GdiPlus.h>
#include "AnimationFactory.h"
#include "LinkedListContainer.h"

using namespace ETCompiler;


/*
** 名字：ArrayAnimationFactory
** 说明：链表动画工厂
**
*/
class LinkedListAnimationFactory : public AnimationFactory
{
public:
	LinkedListAnimationFactory();
	~LinkedListAnimationFactory();
	Container* CreateContainer();
	WatchPropertiesFuncType   CreateWatchDogCallback();
};

LPVOID WatchList( WatchPropertiesListType* list );

AnimationFactory* CreateAnimationFactory();