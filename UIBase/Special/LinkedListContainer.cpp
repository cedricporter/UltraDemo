/*
** 作者：杨明锦
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
	if ( ! datasource||!drawparam->pDC )	//如果传进指针为空，清空m_element,返回
		{
			m_element.clear();
			m_newelement.clear();
		    iniflag=false;
			return;
	}
	m_paintheight=drawparam->height;
	m_paintwidth=drawparam->width;
	
	int nodemount=1;//借助nodemount来标识要填充的LinkedListNode在m_element或m_newelement的序号（即位置）
	if(!iniflag)   //如果是第一次获取信息
	{
		while( datasource->next )                                                  //第一次同时填充m_element 和m_newelement
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
	else  //不是第一次就只填充m_newelement 
	{



		updateListdata();//delete m_element中的节点，清空m_element,把m_newelement的内容填充到m_element,delete m_newelement中的节点，并清空m_newelement，等待填充m_newelement
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

	m_changeType=CheckChangeInMemory();   //判断发生改变的类型，返回宏

	if(m_changeType!=NOCHANGE) 
	{


	m_changesn=findchangedpoisition(m_changeType);//找到发生改变的节点在 m_element 中的序号（位置）
	
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

int LinkedListContainer::updateListdata() //把m_newelement的内容赋给m_element，并delete对应的内存
{

	
	for(std::list< LinkedListElement* >::iterator iter = m_element.begin(); iter != m_element.end();)//释放m_element申请的堆
	{
	  delete (*iter++);  //第二次updatelistdata到这里就挂掉了
	}
	m_element.clear();//清空m_element


	for(std::list< LinkedListElement* >::iterator iter = m_newelement.begin(); iter != m_newelement.end();++iter)//赋值m_newelement到m_elelment
	{
		LinkedListElement* p = new LinkedListElement();
		if (!p) return -1;

		//memcpy(p,(*iter),sizeof(LinkedListElement));
		
		*p = **iter;

		m_element.push_back(p);
	}
		for(std::list<LinkedListElement*>::iterator iter=m_newelement.begin();iter!=m_newelement.end();)//释放m_newelement申请的内存
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

	pUI->SetParent( this );						// 设置元素的父母为这个容器
	m_element.push_back( (LinkedListElement*)pUI );				// 添加子元素
	pUI->AddUIElementChangedListener( this );	// 订阅子元素的改变消息

	return true;
}
// Removes an UIElement from the m_Children
bool LinkedListContainer:: Remove( UIElement* pUI )
{
	for ( LinkedList::iterator iter = m_element.begin(); iter != m_element.end(); iter++ )
	{
		if ( *iter == pUI )
		{
			m_element.erase( iter );						// 删除子元素
			pUI->RemoveUIElementChangedListener( this );	// 取消订阅子元素的改变消息				
			pUI->SetParent( NULL );							// 设置元素的父母为空

			return true;
		}
	}

	return false;	// NOT exited
}


void LinkedListContainer::OnMouseLeftButtonDownEvent( CETObject* source, MouseEventProperty* param )
{
	
}