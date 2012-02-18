#pragma once

#include <GdiPlus.h>
#include "AnimationFactory.h"
#include "TwoDimensionArrayContainer.h"

using namespace ETCompiler;


/*
** 名字：ArrayAnimationFactory
** 说明：数组动画工厂
**
*/
class TwoDimensionArrayAnimationFactory : public AnimationFactory
{
private:

public:
	TwoDimensionArrayAnimationFactory();
	~TwoDimensionArrayAnimationFactory();
	Container* CreateContainer();
	WatchPropertiesFuncType   CreateWatchDogCallback();
};

LPVOID WatchList( WatchPropertiesListType* list );

AnimationFactory* CreateAnimationFactory();