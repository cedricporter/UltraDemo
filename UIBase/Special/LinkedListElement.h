/*作者：杨明锦
*/


#ifndef _LINKEDLISTELEMENT_H
#define _LINKEDLISTELEMENT_H

#include "base.h"
#include "LinkedListContainer.h"
#include <string>

struct LinkedListNode;
class LinkedListElement : public UIElement
{
	DECLARE_REGISTER_FACTORY( LinkedListElement, CETObject )

private:

	int m_sn;           //m_elelment中的序号

	int m_content;     //节点中的数值

	LinkedListNode*  m_pointer;   //节点中的指针

	int m_arrowx;          //箭头起点横坐标

	int m_arrowy;              //箭头起点纵坐标

	int barrierwidth;              //间隔条宽度

	int m_othernodesn;      //要画指向箭头的节点的序号。

	bool endrawarrowtonext;   //标识是否画指向下一个节点的变量，默认为true

	bool endrawmyself;         //标识是否画自己的矩形和值的变量，默认为ture

	bool endrawarrowtomyself;  //标识是否画指向自己的箭头的变量 ，默认为false

	bool arrowtoothernode;    //标识是否画箭头指向另外的节点



public:

	int setSn(int sn);         //设置自己在m_element中的位置标识
	int getSn();                //返回自己在m_element中的位置标识
	int getValue();            //返回自己保存的值
	int setValue(int x);        //设置自己要保存的值
	int setPosition(int x,int y);           //设置自己在画布中的位置
	int drawMyself( void *pointer);                    //画矩形及写内容
	int drawArrowToMyself( void *pointer);             //画指向自己的箭头
	int drawArrowToNext( void *pointer);               //画指向下一个节点的箭头
	int drawArrowToOtherNode(void *pointer);    //画指向别的节点的箭头
	int enableDrawArrowToMyself();                    //使能画指向自己的箭头
	int enableArrowToNext();                      //使能画指向下一个的箭头
	int disableDrawMyself();                   //使不能自己本身
	int enableDrawMyself();                    //使能画自己
	int enableDrawArrowToOtherNode(int sn);
	int eraseArrowToMyself( );            //使不能指向自己的箭头
	int eraseArrowToNext( );              //使不能指向下一个节点的箭头
	int shiftSn(char dir='R',int shift=0);//改变自己在m_element中的位置标识
	int drawArrowToMyself( void *pointer,CStringW str);     //画指向自己的箭头，带文字说明
	LinkedListNode *getmypointer();

	
	
	LinkedListElement(LinkedListNode *p=NULL,int content=0)
	{
		m_width=80;	// The width of the element
		m_height=50;	// The height of the element
		m_content=0;
		m_pointer=p;
		m_sn=0;
		barrierwidth=m_width/4;
		endrawarrowtonext=true;
		endrawmyself=true;
		endrawarrowtomyself=false;  
		m_content=content;
		arrowtoothernode=false;

	}

	
	int Draw( void* pointer );

};
#endif