/*
** ���ߣ�������
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




	// ���¼���Ԫ�ز���
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

	int findchangedpoisition(int changeflag);   //�ҳ������ı�Ľڵ���m_element�е�λ��

	int updateListdata();                      

	int CheckChangeInMemory();

	LinkedList::iterator  getNodeposition(int sn); //���ݽڵ���m_element�е�λ�û�ö�Ӧ�ڵ�ĵ�����

	LinkedList::iterator  getNewNodeposition(int sn); //���ݽڵ���m_element�е�λ�û�ö�Ӧ�ڵ�ĵ�����

	int enableDrawArrowToMyself(int sn);           //ʹ�ܻ�����sn���ڵ�ָ��ڵ㱾��ļ�ͷ��Ĭ�ϻ���

	int disableDrawArrowToMyself(int sn);          //ʹ  ����   ������sn���ڵ�ָ��ڵ㱾��ļ�ͷ

	int disableDrawArrowToNext(int sn);            ////ʹ���ܻ�����sn���ڵ�ָ����һ���ڵ�ļ�ͷ���ü�ͷ��Ĭ�ϻ�����

	int disableDrawArrowToNextNew(int sn);            ////ʹ���ܻ�����sn���ڵ�ָ����һ���ڵ�ļ�ͷ���ü�ͷ��Ĭ�ϻ�����

	int enableDrawMyself(int sn);                  //ʹ�� �� sn�ڵ��Լ��ľ��κ�ֵ

	int disableDrawMyself(int sn);                 //ʹ���� �� sn�ڵ��Լ��ľ��κ�ֵ

	int disableDrawMyselfAfter(int sn);           //ʹ���ܻ�sn�ڵ�������Ԫ��

	int connect( int sn1,int sn2);


	
	bool Add( UIElement* pUI );
	bool  Remove( UIElement* pUI );
	UIElement* OnClick( int x, int y );
;
	void ShowList(CDC*pDC);                    //��m_element�е����ݣ�����˵��

	void ShowList(CDC*pDC,CStringW str);       //��m_element�е����ݣ���˵��

private:

	LinkedList m_element;                     //����֮ǰ�����洫������ͷָ���л�õ�list
	LinkedList m_newelement;                  //�����µĴ����洫������ͷָ���л�õ�list

	bool iniflag;                           //��ʶ�Ƿ��Ѿ���ù����ݣ���ʼ��Ϊfalse
	bool emptyflag;                         //��ʶm_element�Ƿ�Ϊ��
	int m_changeType;                       //��ʶ�ı�����
	int m_changesn;                         //��ʶ�ı�Ľڵ���m_element�е�λ��
	int m_opernum;                         //��ʶdraw����
	int m_paintheight;
	int m_paintwidth;


};


#endif