/*
** 作者：华亮
** 说明：
**
*/
#pragma once

#include "InfoUIElement.h"
#include <algorithm>
#include <list>
#include <map>
#include <cassert>



class PredElement;

//////////////////////////////////////////////////////////////////////////
//
// Class StateElement
// 
class StateElement : public InfoUIElement
{
private:
	std::list< PredElement* > m_PredJumpsOut;
	std::list< PredElement* > m_PredJumpsIn;

public:
	bool RemovePredFrom( std::list< PredElement* >& preds, PredElement* pPred );
	void AddJumpPredIn( PredElement* pPred )
	{ m_PredJumpsIn.push_back(pPred); }
	bool RemoveJumpPredIn( PredElement* pPred )
	{ return RemovePredFrom(m_PredJumpsIn, pPred); }
	void AddJumpPred( PredElement* pPred )
	{ m_PredJumpsOut.push_back(pPred); }
	bool RemoveJumpPred( PredElement* pPred )
	{ return RemovePredFrom(m_PredJumpsOut, pPred); }

	virtual int Draw( void* param );
	virtual int OnDeleted();
};

//////////////////////////////////////////////////////////////////////////
//
// Class PredElement
// 
class PredElement : public InfoUIElement
{
private:
	StateElement* m_pSource;
	StateElement* m_pTarget;

public:
	PredElement() : m_pSource( 0 ), m_pTarget( 0 ) {}

	void SetSource( StateElement* source ) { m_pSource = source; }
	void SetTarget( StateElement* target ) { m_pTarget = target; }
	StateElement* GetSource() const { return m_pSource; }
	StateElement* GetTarget() const { return m_pTarget; }	
	int RemoveSource()
	{
		assert( m_pSource );
		return m_pSource->RemoveJumpPred( this );
	}
	int RemoveTarget()
	{
		assert( m_pTarget );
		return m_pTarget->RemoveJumpPredIn( this );
	}

	virtual int Draw( void* param );
	virtual int OnDeleted() 
	{
		/*如果协商这两条语句会导致逻辑错误！因为如果在State中用for_each来调用了OnDelete(),会导致for_each途中list改变*/
		//RemoveSource();
		//RemoveTarget();

		// Delete From source
		return (*m_pOnDeletedCB)( this );
	}
	virtual bool HitTest( int x, int y );
};


/*
** 名字：InfoUIElementAnchor
** 说明：修饰UIElement
**
*/
class InfoUIElementAnchor : public UIElementAnchor
{
public:
	virtual int Draw( void* param );

	void DrawPredAnchor( Gdiplus::Graphics &g );
	void DrawStateAnchor( Gdiplus::Graphics &g );

};


/*
** 名字：InfoElementContainer
** 说明：布局容器，UI层
**
*/
class InfoElementContainer : public Container
{
	typedef std::map< UIElement*, UIElement* > PredListType;

	PredListType m_predList;

	bool AddPred( PredElement* pPred )
	{
		if ( m_predList.find( pPred ) == m_predList.end() ) // 找不到
		{
			m_predList[ pPred ] = pPred;
			return true;
		}
		return false;
	}

	bool RemovePred( PredElement* pPred )
	{
		PredListType::iterator iter;
		if (( iter = m_predList.find( pPred ) ) != m_predList.end() ) // 找到了
		{
			m_predList.erase( iter );
			return true;
		}
		return false;
	}

public:

	virtual bool Add( UIElement* pUI )
	{
		if ( typeid( *pUI ) == typeid( StateElement ) )
		{
			return Container::Add(pUI);
		}
		else if ( typeid( *pUI ) == typeid( PredElement ) )
		{
			return AddPred( (PredElement*)pUI );
		}

		return false;
	}

	virtual bool Remove( UIElement* pUI )
	{
		if ( typeid( *pUI ) == typeid( StateElement ) )
		{
			return Container::Remove(pUI);
		}
		else if ( typeid( *pUI ) == typeid( PredElement ) )
		{
			return RemovePred( (PredElement*)pUI );
		}

		return false;
	}

	// Event Handlers
	virtual UIElement* OnClick( int x, int y )
	{
		for ( auto iter = m_Children.begin(); iter != m_Children.end(); iter++ )
		{
			if ( (*iter)->HitTest( x, y ) )
				return *iter;
		}

		for ( auto iter = m_predList.begin(); iter != m_predList.end(); iter ++ )
		{
			if ( iter->second->HitTest( x, y ) )
			{
				return iter->second;
			}
		}

		return NULL;
	}

	virtual int Draw( void* param )
	{
		std::for_each( m_predList.begin(), m_predList.end(), [&]( const PredListType::value_type &p ) { p.second->Draw( param ); });

		std::for_each( m_Children.begin(), m_Children.end(), [&]( UIElement* e ) { e->Draw( param ); });

		return 0;

	}

};