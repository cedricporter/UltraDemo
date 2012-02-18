#include "stdafx.h"
#include "AnimationFactory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


AnimationFactory::~AnimationFactory()
{
	for ( auto iter = m_containerList.begin(); iter != m_containerList.end(); ++iter )
	{
		delete *iter;
	}
}


std::string* AnimationFactory::GetFactoryDescrption()
{
	return &m_factoryDescription;
}

std::string* AnimationFactory::GetFactoryName()
{
	return &m_factoryName;
}

void AnimationFactory::SetFactoryName( const std::string& factoryname )
{
	m_factoryName = factoryname;
}

void AnimationFactory::SetFactoryDescription( const std::string& factoryDescription )
{
	m_factoryDescription = factoryDescription;
}

void AnimationFactory::AddWatchedVarName( const std::string& varname )
{
	m_watchedList.push_back( varname );
}

WatchedVarNameList* AnimationFactory::GetFactoryWatchedList()
{
	return &m_watchedList;
}

void AnimationFactory::AutoDeleteContainer( Container* container )
{
	m_containerList.push_back( container );
}
