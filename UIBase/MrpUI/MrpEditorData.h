/*
** 作者：华亮
** 说明：
**
*/

#pragma once

#include "Def.h"
#include "InfoUIElement.h"
#include "Elements.h"
#include <list>
#include <fstream>
#include <string>


/*
** 名字：class Data
** 说明：保存状态和谓词
**
*/
class Data
{
private:
	std::list< StateElement* > m_actionList;
	std::list< PredElement* > m_predList;


public:
	~Data()
	{
		Clear();
	}

	const std::list< StateElement* >& GetActionList() const { return m_actionList; }
	const std::list< PredElement* >& GetPredList() const	{ return m_predList; }

	/*
	** 名字：Clear() 
	** 说明：清空数据，释放内存
	**
	*/
	void Clear();

	/*
	** 名字：AddElement( InfoUIElement* element )
	** 说明：添加Element，分类保存
	**
	*/
	int AddElement( InfoUIElement* element );

	/*
	** 名字：DelElement( InfoUIElement* element )
	** 说明：删除Element
	**
	*/
	void DelElement( InfoUIElement* element );
};