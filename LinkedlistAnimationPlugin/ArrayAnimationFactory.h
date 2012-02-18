#pragma once

#include <GdiPlus.h>
#include "AnimationFactory.h"
#include "LinkedListContainer.h"

using namespace ETCompiler;


/*
** ���֣�ArrayAnimationFactory
** ˵��������������
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