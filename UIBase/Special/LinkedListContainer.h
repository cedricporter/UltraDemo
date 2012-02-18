/*
** 作者：杨明锦
**
**
**
*/


#ifndef _LINKEDLISTCONTAINER_H
#define _LINKEDLISTCONTAINER_H

#include "base.h"
#include "LinkedListElement.h"
#define DELETENODE        1111
#define DELETEFROMONENODE 1112
#define INSERTNODE        1113
#define NOCHANGE          1114
#define CHANGEVALUE       1115
#define APPLIST        1116
class LinkedListElement;
typedef std::list< LinkedListElement*> LinkedList;



struct LinkedListNode
{
	int val;
	LinkedListNode* next;
};

struct elementNode
{
	LinkedListNode* head;
	std::list<LinkedListNode*> pointerlist;
};

class LinkedListContainer : public Container
{
public: 
	DECLARE_REGISTER_FACTORY( LinkedListContainer, CETObject )

public:




	// 重新计算元素布局
	virtual void OnUIElementChangedEvent( CETObject* source, void* param );

	virtual void OnMouseLeftButtonDownEvent( CETObject* source, MouseEventProperty* param );


	LinkedListContainer()
	{
		iniflag=false;
		emptyflag=true;
		m_opernum=0;
		m_changeType=NOCHANGE;
	}

	int Draw( void* pointer );

	int findchangedpoisition(int changeflag);   //找出发生改变的节点在m_element中的位置

	int updateListdata();                      

	int CheckChangeInMemory();

	LinkedList::iterator  getNodeposition(int sn); //根据节点在m_element中的位置获得对应节点的迭代器

	LinkedList::iterator  getNewNodeposition(int sn); //根据节点在m_element中的位置获得对应节点的迭代器

	int enableDrawArrowToMyself(int sn);           //使能画出第sn个节点指向节点本身的箭头，默认画出

	int disableDrawArrowToMyself(int sn);          //使  不能   画出第sn个节点指向节点本身的箭头

	int disableDrawArrowToNext(int sn);            ////使不能画出第sn个节点指向下一个节点的箭头，该箭头是默认画出的

	int disableDrawArrowToNextNew(int sn);            ////使不能画出第sn个节点指向下一个节点的箭头，该箭头是默认画出的

	int enableDrawMyself(int sn);                  //使能 画 sn节点自己的矩形和值

	int disableDrawMyself(int sn);                 //使不能 画 sn节点自己的矩形和值

	int disableDrawMyselfAfter(int sn);           //使不能画sn节点后的所有元素

	int connect( int sn1,int sn2);


	
	bool Add( UIElement* pUI );
	bool  Remove( UIElement* pUI );
	UIElement* OnClick( int x, int y );
;
	void ShowList(CDC*pDC);                    //画m_element中的内容，不带说明

	void ShowList(CDC*pDC,CStringW str);       //画m_element中的内容，带说明

private:

	LinkedList m_element;                     //保存之前从外面传进来的头指针中获得的list
	LinkedList m_newelement;                  //保存新的从外面传进来的头指针中获得的list

	bool iniflag;                           //标识是否已经获得过数据，初始化为false
	bool emptyflag;                         //标识m_element是否为空
	int m_changeType;                       //标识改变类型
	int m_changesn;                         //标识改变的节点在m_element中的位置
	int m_opernum;                         //标识draw步数
	int m_paintheight;
	int m_paintwidth;


};


#endif