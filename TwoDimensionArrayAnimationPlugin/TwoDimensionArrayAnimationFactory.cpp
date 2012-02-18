#include "stdafx.h"
#include "TwoDimensionArrayAnimationFactory.h"



Container* TwoDimensionArrayAnimationFactory::CreateContainer()
{
	Container* pContainer = new TwoDimensionArrayContainer;	

	AutoDeleteContainer( pContainer );
	return pContainer;
}


WatchPropertiesFuncType TwoDimensionArrayAnimationFactory::CreateWatchDogCallback()
{
	return WatchList;
}

TwoDimensionArrayAnimationFactory::~TwoDimensionArrayAnimationFactory()
{
}

TwoDimensionArrayAnimationFactory::TwoDimensionArrayAnimationFactory() 
{
	SetFactoryName( "TwoDimensionArrayAnimation" );
	AddWatchedVarName( "watchedArray" );
	AddWatchedVarName( "watchedWidth" );
	AddWatchedVarName( "watchedHeight" );
	SetFactoryDescription( "如果希望动画可以监控您的代码，请以关键字命名您希望被监控关键字。以watchedArray命名数组指针，以watchedWidth命名数组维度" );
}

LPVOID WatchList( WatchPropertiesListType* list )
{
	auto iter = list->begin();
	auto arrIter = list->end();
	auto widthIter = list->end();
	auto heightIter = list->end();

	for ( ; iter != list->end(); ++iter )
	{
		if ( iter->varname == "watchedArray" ) arrIter = iter;
		if ( iter->varname == "watchedWidth" ) widthIter = iter;
		if ( iter->varname == "watchedHeight" ) heightIter = iter;
	}

	if ( arrIter != list->end() && widthIter != list->end() && heightIter != list->end() )
	{
		TwoDimensionArrayContainerParam* param = new TwoDimensionArrayContainerParam;
		param->arr = (int*)arrIter->val;
		param->width = widthIter->val;
		param->height = heightIter->val;

		return (LPVOID)param;
	}

	return 0;
}

AnimationFactory* CreateAnimationFactory()
{
	static AnimationFactory* factory = NULL;
	if ( ! factory )
	{
		factory = new TwoDimensionArrayAnimationFactory;
	}
	return factory;
}
