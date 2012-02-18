/*
** ���ߣ�����
** ˵����
**
*/

#pragma once
#include "Def.h"
#include "base.h"


using namespace ETCompiler;

typedef std::list<std::string> WatchedVarNameList;

/*
** ���֣�AnimationFactory
** ˵������������
**
*/
class AnimationFactory
{
private:
	std::string			m_factoryName;					// ��������
	std::string			m_factoryDescription;			// ��������
	WatchedVarNameList	m_watchedList;					// ��ע�������б�
	std::list<Container*>	m_containerList;			// ���洴��������������ɾ��


protected:
	virtual void AutoDeleteContainer( Container* container );
	virtual void SetFactoryName( const std::string& factoryname );					// ���ö���������
	virtual void SetFactoryDescription( const std::string& factoryDescription );	// ���ö�����������
	virtual void AddWatchedVarName( const std::string& varname );					// ��ӹ�ע������

public:
	AnimationFactory() {}
	virtual ~AnimationFactory();

	virtual Container* CreateContainer() = 0;						// ������������
	virtual	WatchPropertiesFuncType CreateWatchDogCallback() = 0;	// ������ע�ص�	
	virtual WatchedVarNameList* GetFactoryWatchedList();			// ��ȡ��ע�����б�
	virtual std::string* GetFactoryName();;							// ��ȡ������
	virtual std::string* GetFactoryDescrption();					// ��ȡ��������
};


