#include "stdafx.h"
#include "HanoiAnimationFactory.h"



Container* HanoiAnimationFactory::CreateContainer()
{
	return new HanoiContainer;
}


WatchPropertiesFuncType HanoiAnimationFactory::CreateWatchDogCallback()
{
	return WatchList;
}

HanoiAnimationFactory::HanoiAnimationFactory()
{
	SetFactoryName( "HanoiAnimation" );
	AddWatchedVarName( "watchedn" );
	AddWatchedVarName( "watchedStart" );
	AddWatchedVarName( "watchedGoal" );
	SetFactoryDescription( "如果希望动画可以监控您的代码，请以关键字命名您希望被监控关键字。以watchedn命名汉诺塔数目，以watchedStart命名移出的塔编号，以watchedGoal命名移入的塔编号" );
}

HanoiAnimationFactory::~HanoiAnimationFactory()
{

}