#pragma once

#include <GdiPlus.h>
#include "AnimationFactory.h"
#include "HanoiContainer.h"

using namespace ETCompiler;


/*
** 名字：HanoiAnimationFactory
** 说明：汉诺塔动画工厂
**
*/
class HanoiAnimationFactory : public AnimationFactory
{
public:
	Container* CreateContainer();
	HanoiAnimationFactory();
	~HanoiAnimationFactory();
	WatchPropertiesFuncType CreateWatchDogCallback();
};

LPVOID WatchList( WatchPropertiesListType* list )
{
	int n = 20;
	int start;
	int goal;
	auto iter = list->begin();
	auto startIter = list->end();
	auto nIter = list->end();
	auto goalIter = list->end();

	for ( ; iter != list->end(); ++iter )
	{
		if ( iter->varname == "watchedStart" ) startIter = iter;
		if ( iter->varname == "watchedGoal" ) goalIter = iter;
		if ( iter->varname == "watchedn" ) nIter = iter;
	}

	if ( startIter != list->end() && nIter != list->end() && goalIter != list->end() )
	{
		start = startIter->val;
		goal = goalIter->val;
		n = nIter->val;

		HanoiStruct* hanoiStruct = new HanoiStruct;
		hanoiStruct->start = start;
		hanoiStruct->goal = goal;
		hanoiStruct->n = n;

		return (LPVOID)hanoiStruct;
	}

	return 0;
}

AnimationFactory* CreateAnimationFactory()
{	
	return new HanoiAnimationFactory;
}