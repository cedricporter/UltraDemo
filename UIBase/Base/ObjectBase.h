/*
** ���ߣ�����
**
**
*/


#ifndef _OBJECTBASE_H
#define _OBJECTBASE_H

#include <map>
#include <string>
#include "TClassFactory.h"

/*
** ���֣�CETObject
** ˵�����๤���Ľӿڣ�Ҳ��������Ļ���
**
*/
class CETObject
{
public:
	virtual ~CETObject() {}
};


/*
** ���֣�
** ˵����������CETObjectΪ������๤����
**      �๤��Ϊ����ģʽ
*/
typedef TFactory<CETObject> ETFactory; 





#endif