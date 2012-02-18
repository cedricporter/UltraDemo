#include "stdafx.h"
#include "ArrayAnimationFactory.h"



Container* LinkedListAnimationFactory::CreateContainer()
{
	return new LinkedListContainer;
}


WatchPropertiesFuncType LinkedListAnimationFactory::CreateWatchDogCallback()
{
	return WatchList;
}

LinkedListAnimationFactory::~LinkedListAnimationFactory()
{

}


LinkedListAnimationFactory::LinkedListAnimationFactory()
{
	SetFactoryName( "LinkedListAnimation" );
	AddWatchedVarName( "watchedpNode" );
	AddWatchedVarName( "watchedpHead" );
	SetFactoryDescription( "如果希望动画可以监控您的代码，请以关键字命名您希望被监控关键字。以watchedpNode命名结构体指针。结构体定义为struct Node\n{\nint val;\nNode* next;\n};" );
}

LPVOID WatchList( WatchPropertiesListType* list )
{
	int *arr = NULL;
	int size = 0;
	auto iter = list->begin();
	auto headIter = list->end();
	auto pNodeIter = list->end();

	for ( ; iter != list->end(); ++iter )
	{
		if ( iter->varname == "watchedpNode" ) pNodeIter = iter;
		else if ( iter->varname == "watchedpHead" ) headIter = iter;
	}

	if ( pNodeIter != list->end() || headIter != list->end() )
	{
		struct WatchParam
		{
			LinkedListNode* pHead;
			LinkedListNode* pNode;
		};

		WatchParam* param = new WatchParam;
		param->pNode = param->pHead = NULL;

		if ( headIter != list->end() ) param->pHead = (LinkedListNode*)headIter->val;
		if ( pNodeIter != list->end() ) param->pNode = (LinkedListNode*)pNodeIter->val;

		return (LPVOID)param;
	}

	return 0;
}

AnimationFactory* CreateAnimationFactory()
{
	static AnimationFactory* factory = NULL;
	if ( ! factory )
	{
		factory = new LinkedListAnimationFactory;
	}
	return factory;
}
