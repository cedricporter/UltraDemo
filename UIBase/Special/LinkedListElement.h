/*���ߣ�������
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

	int m_sn;           //m_elelment�е����

	int m_content;     //�ڵ��е���ֵ

	LinkedListNode*  m_pointer;   //�ڵ��е�ָ��

	int m_arrowx;          //��ͷ��������

	int m_arrowy;              //��ͷ���������

	int barrierwidth;              //��������

	int m_othernodesn;      //Ҫ��ָ���ͷ�Ľڵ����š�

	bool endrawarrowtonext;   //��ʶ�Ƿ�ָ����һ���ڵ�ı�����Ĭ��Ϊtrue

	bool endrawmyself;         //��ʶ�Ƿ��Լ��ľ��κ�ֵ�ı�����Ĭ��Ϊture

	bool endrawarrowtomyself;  //��ʶ�Ƿ�ָ���Լ��ļ�ͷ�ı��� ��Ĭ��Ϊfalse

	bool arrowtoothernode;    //��ʶ�Ƿ񻭼�ͷָ������Ľڵ�



public:

	int setSn(int sn);         //�����Լ���m_element�е�λ�ñ�ʶ
	int getSn();                //�����Լ���m_element�е�λ�ñ�ʶ
	int getValue();            //�����Լ������ֵ
	int setValue(int x);        //�����Լ�Ҫ�����ֵ
	int setPosition(int x,int y);           //�����Լ��ڻ����е�λ��
	int drawMyself( void *pointer);                    //�����μ�д����
	int drawArrowToMyself( void *pointer);             //��ָ���Լ��ļ�ͷ
	int drawArrowToNext( void *pointer);               //��ָ����һ���ڵ�ļ�ͷ
	int drawArrowToOtherNode(void *pointer);    //��ָ���Ľڵ�ļ�ͷ
	int enableDrawArrowToMyself();                    //ʹ�ܻ�ָ���Լ��ļ�ͷ
	int enableArrowToNext();                      //ʹ�ܻ�ָ����һ���ļ�ͷ
	int disableDrawMyself();                   //ʹ�����Լ�����
	int enableDrawMyself();                    //ʹ�ܻ��Լ�
	int enableDrawArrowToOtherNode(int sn);
	int eraseArrowToMyself( );            //ʹ����ָ���Լ��ļ�ͷ
	int eraseArrowToNext( );              //ʹ����ָ����һ���ڵ�ļ�ͷ
	int shiftSn(char dir='R',int shift=0);//�ı��Լ���m_element�е�λ�ñ�ʶ
	int drawArrowToMyself( void *pointer,CStringW str);     //��ָ���Լ��ļ�ͷ��������˵��
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