#pragma once

#include <GdiPlus.h>
#include "AnimationFactory.h"
#include "TwoDimensionArrayContainer.h"

using namespace ETCompiler;


/*
** ���֣�ArrayAnimationFactory
** ˵�������鶯������
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