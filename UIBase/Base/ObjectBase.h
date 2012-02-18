/*
** 作者：华亮
**
**
*/


#ifndef _OBJECTBASE_H
#define _OBJECTBASE_H

#include <map>
#include <string>
#include "TClassFactory.h"

/*
** 名字：CETObject
** 说明：类工厂的接口，也是所有类的基类
**
*/
class CETObject
{
public:
	virtual ~CETObject() {}
};


/*
** 名字：
** 说明：定义以CETObject为基类的类工厂，
**      类工厂为单例模式
*/
typedef TFactory<CETObject> ETFactory; 





#endif