/*
** ���ߣ�����
** ˵����
**
*/

#include "stdafx.h"
#include "MrpEditorData.h"


/*
** ���֣�Clear() 
** ˵����������ݣ��ͷ��ڴ�
**
*/
void Data::Clear() 
{ 
	std::for_each( m_actionList.begin(), m_actionList.end(), [&]( StateElement*& element )
	{ delete element; });
	m_actionList.clear();

	std::for_each( m_predList.begin(), m_predList.end(), [&]( PredElement*& element )
	{ delete element; });
	m_predList.clear();
}



/*
** ���֣�AddElement( InfoUIElement* element )
** ˵�������Element�����ౣ��
**
*/
int Data::AddElement( InfoUIElement* element )
{
	if ( typeid( *element ) == typeid( StateElement ) )
	{
		m_actionList.push_back( (StateElement*)element );
	}
	else if ( typeid( *element ) == typeid( PredElement ) )
	{
		m_predList.push_back( (PredElement*)element );
	}

	return 0;
}


/*
** ���֣�DelElement( InfoUIElement* element )
** ˵����ɾ��Element
**
*/
void Data::DelElement( InfoUIElement* element )
{
	std::function< bool( InfoUIElement* ) > ifToBeDeleted =  [&]( InfoUIElement* e )->bool
	{
		return element == e;
	};

	if ( typeid( *element ) == typeid( StateElement ) )
	{
		m_actionList.remove_if( ifToBeDeleted );
	}
	else if ( typeid( *element ) == typeid( PredElement ) )
	{
		m_predList.remove_if( ifToBeDeleted );
	}

	delete element;
}