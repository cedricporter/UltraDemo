/*
** ���ߣ�����
** ˵����
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
** ���֣�class Data
** ˵��������״̬��ν��
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
	** ���֣�Clear() 
	** ˵����������ݣ��ͷ��ڴ�
	**
	*/
	void Clear();

	/*
	** ���֣�AddElement( InfoUIElement* element )
	** ˵�������Element�����ౣ��
	**
	*/
	int AddElement( InfoUIElement* element );

	/*
	** ���֣�DelElement( InfoUIElement* element )
	** ˵����ɾ��Element
	**
	*/
	void DelElement( InfoUIElement* element );
};