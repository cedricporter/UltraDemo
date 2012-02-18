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
	SetFactoryDescription( "���ϣ���������Լ�����Ĵ��룬���Թؼ���������ϣ������عؼ��֡���watchedn������ŵ����Ŀ����watchedStart�����Ƴ�������ţ���watchedGoal��������������" );
}

HanoiAnimationFactory::~HanoiAnimationFactory()
{

}