#include "stdafx.h"
#include "ArrayAnimationFactory.h"



Container* ArrayAnimationFactory::CreateContainer()
{
	static Container* pContainer = NULL;
	if ( pContainer == NULL )
	{
		pContainer = new ArrayContainer;
	}
	return pContainer;
}


WatchPropertiesFuncType ArrayAnimationFactory::CreateWatchDogCallback()
{
	return WatchList;
}

ArrayAnimationFactory::~ArrayAnimationFactory()
{

}

ArrayAnimationFactory::ArrayAnimationFactory()
{
	SetFactoryName( "ArrayAnimation" );
	AddWatchedVarName( "watchedArray" );
	AddWatchedVarName( "watchedSize" );
	SetFactoryDescription( "如果希望动画可以监控您的代码，请以关键字命名您希望被监控关键字。以watchedArray命名数组指针，以watchedSize命名数组长度" );
}

LPVOID WatchList( WatchPropertiesListType* list )
{
	int *arr = NULL;
	int size = 0;
	auto iter = list->begin();
	auto arrIter = list->end();
	auto sizeIter = list->end();

	for ( ; iter != list->end(); ++iter )
	{
		if ( iter->varname == "watchedArray" ) arrIter = iter;
		if ( iter->varname == "watchedSize" ) sizeIter = iter;
	}

	if ( arrIter != list->end() && sizeIter != list->end() )
	{
		arr = (int*)arrIter->val;
		size = sizeIter->val;

		ArrayStruct* arrStruct = new ArrayStruct;
		arrStruct->arr = arr;
		arrStruct->size = size;

		return (LPVOID)arrStruct;
	}

	return 0;
}

AnimationFactory* CreateAnimationFactory()
{
	static AnimationFactory* factory = NULL;
	if ( ! factory )
	{
		factory = new ArrayAnimationFactory;
	}
	return factory;
}
