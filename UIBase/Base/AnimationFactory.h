/*
** 作者：华亮
** 说明：
**
*/

#pragma once
#include "Def.h"
#include "base.h"


using namespace ETCompiler;

typedef std::list<std::string> WatchedVarNameList;

/*
** 名字：AnimationFactory
** 说明：动画工厂
**
*/
class AnimationFactory
{
private:
	std::string			m_factoryName;					// 动画工厂
	std::string			m_factoryDescription;			// 动画描述
	WatchedVarNameList	m_watchedList;					// 关注变量的列表
	std::list<Container*>	m_containerList;			// 保存创建出来的容器以删除


protected:
	virtual void AutoDeleteContainer( Container* container );
	virtual void SetFactoryName( const std::string& factoryname );					// 设置动画工厂名
	virtual void SetFactoryDescription( const std::string& factoryDescription );	// 设置动画工厂描述
	virtual void AddWatchedVarName( const std::string& varname );					// 添加关注变量名

public:
	AnimationFactory() {}
	virtual ~AnimationFactory();

	virtual Container* CreateContainer() = 0;						// 创建动画容器
	virtual	WatchPropertiesFuncType CreateWatchDogCallback() = 0;	// 创建关注回调	
	virtual WatchedVarNameList* GetFactoryWatchedList();			// 获取关注变量列表
	virtual std::string* GetFactoryName();;							// 获取工厂名
	virtual std::string* GetFactoryDescrption();					// 获取工厂描述
};


