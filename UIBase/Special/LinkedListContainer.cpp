/*
** ���ߣ�������
**
**
*/

#include "stdafx.h"
#include "LinkedListContainer.h"
#include "LinkedListElement.h"
#include <GdiPlus.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class LinkedListElement;
const int arrowLength=30;

IMPLEMENT_REGISTER_FACTORY( LinkedListContainer, CETObject );

	struct Param
	{
		CDC* pDC;
		int width;
		int height;
	};
	struct WatchParam
		{
			LinkedListNode* pHead;
			LinkedListNode* pNode;
		};


void LinkedListContainer::OnUIElementChangedEvent( CETObject* source, void* param )
{
	
	Param *drawparam=(Param*)source;
	WatchParam* watchparam=(WatchParam*)param;
	LinkedListNode* datasource=(LinkedListNode*)watchparam->pHead;
	if ( ! datasource||!drawparam->pDC )	//�������ָ��Ϊ�գ����m_element,����
		{
			m_element.clear();
			m_newelement.clear();
		    iniflag=false;
			return;
	}
	m_paintheight=drawparam->height;
	m_paintwidth=drawparam->width;
	
	int nodemount=1;//����nodemount����ʶҪ����LinkedListNode��m_element��m_newelement����ţ���λ�ã�
	if(!iniflag)   //����ǵ�һ�λ�ȡ��Ϣ
	{
		while( datasource->next )                                                  //��һ��ͬʱ���m_element ��m_newelement
		{
			LinkedListElement*p=new LinkedListElement(datasource,datasource->val);
			if (!p) return ;
			LinkedListElement*p2=new LinkedListElement(datasource,datasource->val);
			if (!p2) return ;
			p->setSn(nodemount);
			p2->setSn(nodemount++);
			m_element.push_back(p);
			m_newelement.push_back(p2);
			datasource=datasource->next;

		}
		LinkedListElement*p=new LinkedListElement(datasource->next,datasource->val);
		if (!p) return ;
		LinkedListElement*p2=new LinkedListElement(datasource->next,datasource->val);
		if (!p2) return ;
		p->setSn(nodemount);
		p2->setSn(nodemount);
		m_element.push_back(p);
		m_newelement.push_back(p2);
		iniflag=true;

	}
	else  //���ǵ�һ�ξ�ֻ���m_newelement 
	{



		updateListdata();//delete m_element�еĽڵ㣬���m_element,��m_newelement��������䵽m_element,delete m_newelement�еĽڵ㣬�����m_newelement���ȴ����m_newelement
		while(datasource->next)
		{
			LinkedListElement*p=new LinkedListElement(datasource,datasource->val);
			if (!p) return ;
			p->setSn(nodemount++);
			m_newelement.push_back(p);
			datasource=datasource->next;

		}
		LinkedListElement*p=new LinkedListElement(datasource->next,datasource->val);
		if (!p) return ;
		p->setSn(nodemount);
		m_newelement.push_back(p);	

	}



	disableDrawArrowToNext(m_element.size()); 
	disableDrawArrowToNextNew(m_newelement.size());

	m_changeType=CheckChangeInMemory();   //�жϷ����ı�����ͣ����غ�

	if(m_changeType!=NOCHANGE) 
	{


	m_changesn=findchangedpoisition(m_changeType);//�ҵ������ı�Ľڵ��� m_element �е���ţ�λ�ã�
	
	}
	switch (m_changeType)
	{
	case DELETENODE:
		{
			
			m_opernum=0;
			enableDrawArrowToMyself(m_changesn);
			
		break;
		}
       // DELETEFROMONENODE:
case INSERTNODE:
		{
			m_opernum=0;
			enableDrawArrowToMyself(m_changesn);
		break;
		}
case CHANGEVALUE:
		{
			m_opernum=0;
			enableDrawArrowToMyself(m_changesn);
		break;
		}
case APPLIST:
		{
			m_opernum=0;
			enableDrawArrowToMyself(m_changesn);
		break;
		}
	default:
		break;
	}
	
	//if(m_changeType!=NOCHANGE)
	{
	for(int i=0;i<4;++i)
	{
	Draw(drawparam->pDC);
	}
	}
 }



LinkedList::iterator LinkedListContainer:: getNodeposition(int sn)
{
	LinkedList::iterator iter=m_element.begin();
	int i=1;
	while(i<sn)
	{
		iter++;
		i++;
	}
	return iter;

}

LinkedList::iterator LinkedListContainer:: getNewNodeposition(int sn)
{
	LinkedList::iterator iter=m_newelement.begin();
	int i=1;
	while(i<sn)
	{
		iter++;
		i++;
	}
	return iter;

}

int LinkedListContainer::updateListdata() //��m_newelement�����ݸ���m_element����delete��Ӧ���ڴ�
{

	
	for(std::list< LinkedListElement* >::iterator iter = m_element.begin(); iter != m_element.end();)//�ͷ�m_element����Ķ�
	{
	  delete (*iter++);  //�ڶ���updatelistdata������͹ҵ���
	}
	m_element.clear();//���m_element


	for(std::list< LinkedListElement* >::iterator iter = m_newelement.begin(); iter != m_newelement.end();++iter)//��ֵm_newelement��m_elelment
	{
		LinkedListElement* p = new LinkedListElement();
		if (!p) return -1;

		//memcpy(p,(*iter),sizeof(LinkedListElement));
		
		*p = **iter;

		m_element.push_back(p);
	}
		for(std::list<LinkedListElement*>::iterator iter=m_newelement.begin();iter!=m_newelement.end();)//�ͷ�m_newelement������ڴ�
	{
		delete (*iter++);
	}
	m_newelement.clear();

	return 0;
}

int LinkedListContainer::Draw( void* pointer )
{


	emptyflag=m_element.empty();
	if(emptyflag)  return -1;
	using namespace Gdiplus;
	CDC*pDC= (CDC*)pointer;
	Graphics graphics( *pDC );
	

	switch (m_changeType)
	{
	case DELETENODE:
		{
		
		switch(m_opernum)
		{
		case 0:
			{
				//AfxMessageBox("d");
				enableDrawArrowToMyself(m_changesn);
				CStringW str=L"delete ";
			    ShowList(pDC,str);
				++m_opernum;
				
			}return 0;
		case 1:
			{
				
				disableDrawArrowToNext(m_changesn-1);
			    ShowList(pDC);

				++m_opernum;
				Sleep(1000);

			}return 0;
		case 2:
			{
				connect(m_changesn-1,m_changesn+1);
			    ShowList(pDC);
				++m_opernum;
				Sleep(1000);

			}return 0;

		case 3:
			{
				disableDrawMyself(m_changesn);
				ShowList(pDC);
				Sleep(1000);
			}
			case 4:
				{
					updateListdata();
					ShowList(pDC);
				}return 0;

			default:
				//updateListdata();
				ShowList(pDC);
		}
			
			
	
		
		}break;

	case DELETEFROMONENODE:
		{

			

			
			switch(m_opernum)
			{
			case 0:
				{
					enableDrawArrowToMyself(m_changesn);
					CStringW str=L"delete after";
			        ShowList(pDC,str);
					++m_opernum;
					Sleep(1000);
				}return 0;
			case 1:
				{
					updateListdata();
					ShowList(pDC);
				}return 0;
			default:
				//updateListdata();
				ShowList(pDC);
				return 0;
			}
					
		}break;



	case INSERTNODE:
		{
			
			switch(m_opernum)
			{
			case 0:
				{
					enableDrawArrowToMyself(m_changesn);
					CStringW str=L"insert after";
			        ShowList(pDC,str);
					++m_opernum;
					Sleep(1000);

				}return 0;
			case 1:
				{
					updateListdata();
					ShowList(pDC);
				}return 0;
			default:
				//updateListdata();
				ShowList(pDC);
				return 0;
			}
			       
					
		}break;


	case NOCHANGE:
		{
		
			//updateListdata();
			ShowList(pDC);

		}break;


	case CHANGEVALUE:
		{

			
			switch(m_opernum)
			{
			case 0:
				{
					enableDrawArrowToMyself(m_changesn);
					CStringW str=L"alert value";
			        ShowList(pDC,str);
					++m_opernum;
					Sleep(1000);

				}return 0;
			case 1:
				{
					updateListdata();
					ShowList(pDC);
				}return 0;
			default:
				updateListdata();
				ShowList(pDC);
				break;
			}
			
		}break;


	case APPLIST:
		{

			
			
			switch(m_opernum)
			{
			case 0:
				{
					enableDrawArrowToMyself(m_changesn);
					CStringW str=L"append list ";
			        ShowList(pDC,str);
					++m_opernum;
					Sleep(1000);


				}return 0;
			case 1:
				{
					updateListdata();
					ShowList(pDC);
				}return 0;
			default:
				ShowList(pDC);
		
			}
				

		}break;
	default:
		AfxMessageBox("error");
		break;
	}
	
	//ShowList(pDC);
	return 0;
	
	
}




UIElement* LinkedListContainer::OnClick( int x, int y )
{

	return NULL; 
}


void LinkedListContainer::ShowList(CDC*pDC)
{
	using namespace Gdiplus;
	Graphics graphics( *pDC );
	SolidBrush mySolidBrush(Color(211, 211, 211, 255));
	graphics.FillRectangle(&mySolidBrush, 0, 0, m_paintwidth ,m_paintheight);
	for ( std::list< LinkedListElement* >::iterator iter = m_element.begin(); iter != m_element.end(); iter++ )
	{

		(*iter)->drawMyself(pDC);
		(*iter)->drawArrowToMyself(pDC);
		(*iter)->drawArrowToNext(pDC);
		(*iter)->drawArrowToOtherNode(pDC);
	}
}
void LinkedListContainer::ShowList(CDC*pDC,CStringW str)
{
	using namespace Gdiplus;
	Graphics graphics( *pDC );
	SolidBrush mySolidBrush((Color(211, 211, 211, 255)));
	graphics.FillRectangle(&mySolidBrush, 0, 0, m_paintwidth ,m_paintheight);
	for ( std::list< LinkedListElement* >::iterator iter = m_element.begin(); iter != m_element.end(); iter++ )
	{

		(*iter)->drawMyself(pDC);
		(*iter)->drawArrowToMyself(pDC,str);
		(*iter)->drawArrowToNext(pDC);
		(*iter)->drawArrowToOtherNode(pDC);
	}
}
int LinkedListContainer::CheckChangeInMemory( ) 
{
	int changemount=m_newelement.size()-m_element.size();
	int changeflags=false;
	if(changemount>1)        return APPLIST;
	else if(changemount==1)  return INSERTNODE;
	else if(changemount==0)
	{

		for(std::list< LinkedListElement* >::iterator iter1 = m_element.begin(),iter2=m_newelement.begin()
			; iter1 != m_element.end(); iter1++,iter2++ )
		{
			if(((*iter1))->getValue()==((*iter2)->getValue())) continue;
			else 
			{
				changeflags=true;
				break;
			}

		}
	} 
	else if(changemount==-1)  return DELETENODE;
	else if(changemount<-1)   return DELETEFROMONENODE;

	if(changeflags)
		return   CHANGEVALUE;
	else
		return   NOCHANGE;



}

int LinkedListContainer::findchangedpoisition(int cflags)//
{
	if(cflags==NOCHANGE) return 0;
	int changesn=1;
	if(cflags==DELETEFROMONENODE)  
	{
		changesn=m_newelement.size();
	}
	else if(cflags==APPLIST) 
	{
		changesn=m_element.size();
	}
	else if(cflags==CHANGEVALUE||cflags==DELETENODE||cflags==INSERTNODE)
	{
		for(std::list< LinkedListElement* >::iterator iter1 = m_element.begin(),iter2=m_newelement.begin()
			; iter1 != m_element.end(); iter1++,iter2++,++changesn )
		{
			if(((*iter1))->getValue() ==((*iter2)->getValue()))
				{continue;
			    }
			else 
			{
				return changesn;
			}

		}
	}
	return  changesn;
}



int LinkedListContainer::enableDrawArrowToMyself(int sn)
{

	if(sn>m_element.size()||sn<=0) return -1;
	(*(getNodeposition(sn)))->enableDrawArrowToMyself();
	return 0;
}

int LinkedListContainer::disableDrawMyself(int sn)
{
	if(sn>m_element.size()||sn<=0) return -1;
	(*(getNodeposition(sn)))->disableDrawMyself();
	return 0;
}

int LinkedListContainer::connect(int sn1,int sn2)
{
	if(sn1==0||sn2==0) return -1;
	if(sn1>m_element.size()||sn2>m_element.size()) return -1;
	(*(getNodeposition(sn1)))->enableDrawArrowToOtherNode(sn2);
	return 0;
}
int LinkedListContainer::disableDrawMyselfAfter(int  sn)
{
	
	++sn;
	if(sn>m_element.size()||sn<=0) return -1;
	for(;sn<m_element.size( );sn++)
	{
		(*(getNodeposition(sn)))->disableDrawMyself();
	}
	return 0;
}
int LinkedListContainer::enableDrawMyself(int sn)
{
	if(sn>m_element.size()||sn<=0) return -1;
	(*(getNodeposition(sn)))->enableDrawMyself();
	return 0;
}
int LinkedListContainer::disableDrawArrowToMyself(int sn)
{
	if(sn>m_element.size()||sn<=0) return -1;
	(*(getNodeposition(sn)))->eraseArrowToMyself( );
	return 0;
}

int LinkedListContainer::disableDrawArrowToNext(int sn)
{
	if(sn>m_element.size()||sn<=0) return -1;
	(*(getNodeposition(sn)))->eraseArrowToNext( );
	return 0;
}
int LinkedListContainer::disableDrawArrowToNextNew(int sn)
{
	if(sn>m_newelement.size()||sn<=0) return -1;
	(*(getNewNodeposition(sn)))->eraseArrowToNext( );
	return 0;
}
bool LinkedListContainer:: Add( UIElement* pUI )
{
	for ( LinkedList::iterator iter = m_element.begin(); iter != m_element.end(); iter++ )
	{
		if ( *iter == pUI )
			return false;	// already exited, failed
	}

	pUI->SetParent( this );						// ����Ԫ�صĸ�ĸΪ�������
	m_element.push_back( (LinkedListElement*)pUI );				// �����Ԫ��
	pUI->AddUIElementChangedListener( this );	// ������Ԫ�صĸı���Ϣ

	return true;
}
// Removes an UIElement from the m_Children
bool LinkedListContainer:: Remove( UIElement* pUI )
{
	for ( LinkedList::iterator iter = m_element.begin(); iter != m_element.end(); iter++ )
	{
		if ( *iter == pUI )
		{
			m_element.erase( iter );						// ɾ����Ԫ��
			pUI->RemoveUIElementChangedListener( this );	// ȡ��������Ԫ�صĸı���Ϣ				
			pUI->SetParent( NULL );							// ����Ԫ�صĸ�ĸΪ��

			return true;
		}
	}

	return false;	// NOT exited
}


void LinkedListContainer::OnMouseLeftButtonDownEvent( CETObject* source, MouseEventProperty* param )
{
	
}