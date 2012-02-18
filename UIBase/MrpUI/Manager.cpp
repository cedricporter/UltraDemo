/*
** 作者：华亮
** 说明：
**
*/

#include "stdafx.h"
#include "Manager.h"
#include "..\Base\FSMThread.h"

#include <GdiPlus.h>
#include <comdef.h>#pragma comment( lib, "gdiplus.lib" )

//////////////////////////////////////////////////////////////////////////
// 
// InfoMananger
//

InfoMananger* InfoMananger::m_pInstance = NULL;

InfoMananger::InfoMananger()
{
	MrpStateParser parser;

	// Action和局部谓词
	m_pStateList = parser.GetStateList( _T("data/states.txt" ) );
	for ( auto iter = m_pStateList->begin(); iter != m_pStateList->end(); iter++ )
	{
		m_ActionList.push_back( (*iter)->params );	// 添加Action

		if ( ! (*iter)->subPred.empty() )	// 有局部谓词
		{
			m_LocalPredMap[ (*iter)->params.at( 0 ) ] = (*iter)->subPred;
		}
	}

	// 全局谓词
	m_pPredList = parser.GetStateList( _T("data/Predicates.txt" ) );
	for ( auto iter = m_pPredList->begin(); iter != m_pPredList->end(); iter++ )
	{
		m_PredList.push_back( (*iter)->params );	// 添加全局谓词
	}
}

InfoMananger::~InfoMananger()
{
	// 回收Action和局部谓词
	std::for_each( m_pStateList->begin(), m_pStateList->end(), [&]( StateNode* node )
	{
		delete node;
	});
	delete m_pStateList;

	// 回收全局谓词
	std::for_each( m_pPredList->begin(), m_pPredList->end(), [&]( StateNode* node )
	{
		delete node;
	});
	delete m_pPredList;
}



//////////////////////////////////////////////////////////////////////////
//
// GlobalMananger
//

GlobalMananger* GlobalMananger::m_pInstance = NULL;

GlobalMananger::GlobalMananger()
{
	m_pDrawManager	= new DrawManager;
	m_pData			= new Data;
	m_pContainer	= new InfoElementContainer;
	m_pFSMConfigFile= new MrpFSMConfigFile;

	AddHandlers();	// 为DrawManager注册回调函数

	// 启动GDIplus
	using namespace Gdiplus;	Gdiplus::GdiplusStartupInput gdiplusStartupInput;           	GdiplusStartup( &m_gdiplusToken, &gdiplusStartupInput, NULL ); 
}

GlobalMananger::~GlobalMananger()
{
	delete m_pDrawManager;
	delete m_pData;
	delete m_pContainer;
	delete m_pFSMConfigFile;


	// 关闭GDIplus
	Gdiplus::GdiplusShutdown( m_gdiplusToken );    
}

/*
** 名字：AddHandlers()
** 说明：为DrawManager注册回调函数
**
*/
void GlobalMananger::AddHandlers()
{
	// 一定要在m_pDrawManager->m_pAddStateCB和m_pDrawManager->m_pAddPredCB之前定义，否则删除的回调函数为空
	m_DeleteFromSourceCB = [&]( InfoUIElement* element )->int
	{
		int ret = m_pContainer->Remove( element );	// 从UI容器中移除
		m_pData->DelElement( element );				// 从数据源删除自己，系统回收内存

		return ret;
	};


	// 鼠标点击回调
	m_pDrawManager->m_ClickCB = [&]( int x, int y )->InfoUIElement*
	{
		return dynamic_cast<InfoUIElement*>( m_pContainer->OnClick( x, y ) );
	};

	// 添加状态Action的回调
	m_pDrawManager->m_AddStateCB = [&]( InfoUIElement* element )->int
	{
		element->SetDeleteFromDataSourceCB( & m_DeleteFromSourceCB );	// 添加删除回调

		m_pData->AddElement( element );				// 保存到数据源中添加
		return m_pContainer->Add( element );		// 添加到UI容器
	};

	// 添加状态谓词的回调
	m_pDrawManager->m_AddPredCB = [&]( StateElement* source, StateElement* target, PredElement* pred )->int
	{	
		pred->SetDeleteFromDataSourceCB( & m_DeleteFromSourceCB );	// 添加删除回调

		m_pData->AddElement( pred );		// 保存到数据源中添加	
		m_pContainer->Add( pred );			// 添加到UI容器

		// 将状态跳转谓词添加到状态
		source->AddJumpPred( pred );		
		target->AddJumpPredIn( pred );

		// 为谓词设置来源状态和目标状态
		pred->SetSource( source );
		pred->SetTarget( target );

		return 0;
	};

	// 提供容器绘制的回调
	m_pDrawManager->m_DrawCB = [&]( CDC* pDC )->int
	{	
		return m_pContainer->Draw( pDC );
	};

}


/*
** 名字：创建保存文件的线程
** 说明：这个函数为静态函数，用于新线程保存文件
**		[传入参数] paramstruct为SaveFileStruct，里面保存文件名和处理这个文件的回调函数。
**			参数内存在此函数中释放。
**
*/
UINT GlobalMananger::SaveFileThreadFunc( LPVOID paramstruct )
{
	SaveFileStruct* param = (SaveFileStruct*)paramstruct;
	SaveFileStruct::SAVEFILEFUNC* pFunc = param->pFunc;

	(*pFunc)( param->filename );	// 保存文件

	delete pFunc;	// 回收回调函数
	delete param;	// 回收参数结构体

	return TRUE;
}


int GlobalMananger::SaveFSMConfigFile( const CETString& filename )
{
	// 使用MFC的创建创建线程方式
	MFCFSMThread mfcThread;
	FSMThread& fsmThread = mfcThread;

	// 保存文件的回调函数
	SaveFileStruct::SAVEFILEFUNC* pFunc = new SaveFileStruct::SAVEFILEFUNC;
	*pFunc = [&]( const CETString& file_name )->int 
	{
		m_pFSMConfigFile->LoadFSMDescription( *m_pData );
		m_pFSMConfigFile->SaveConfigFile( file_name );
		return TRUE;
	};

	// 参数信息
	SaveFileStruct* pParam = new SaveFileStruct;
	pParam->pFunc = pFunc;
	pParam->filename = filename;

	// 创建新线程
	fsmThread.BeginThread( SaveFileThreadFunc, (LPVOID)pParam );

	return TRUE;
}




//////////////////////////////////////////////////////////////////////////
//
// DrawManager
//

DrawManager::DrawManager() 
	: 
m_bDrawingLine( false ), m_bDragMoving( false ), 
	m_pSelectedElememt( NULL ), m_pSelectedDrawingType( NULL )
{
	m_stateWidth = 50;
	m_stateHeight = 50;

	m_pElementAnchor = new InfoUIElementAnchor;

	m_pTempStateForDrawingJumpPred = new StateElement;
	m_pTempStateForDrawingJumpPred->SetWidth( m_stateWidth );
	m_pTempStateForDrawingJumpPred->SetHeight( m_stateHeight );
}

DrawManager::~DrawManager()
{
	delete m_pTempStateForDrawingJumpPred;
	delete m_pElementAnchor;
}

/*
** 名字：OnLButtonDown( int x, int y )
** 说明：响应鼠标左键按下
**
*/
int DrawManager::OnLButtonDown( int x, int y )
{
	int ret = FALSE;	// 需要重绘就设置为TRUE
	m_pLastSelectedElememt = m_pSelectedElememt;// 保存上次选中元素

	// 命中测试
	m_pSelectedElememt = m_ClickCB( x, y );		// 获取选中元素，未选中任何得到NULL
	OnUpdateSelectedElement();					// 处理当前元素改变的消息

	ret |= TestAndDoAddStateElement( x, y );	// 满足条件则添加StateElement
	ret |= TestAndDoAddPredElement( x, y );		// 满足条件则添加谓词或者进入拖动状态


	return ret;	// 返回TRUE通知重绘
}

/*
** 名字：OnLButtonUp( int x, int y )
** 说明：响应鼠标弹起
**
*/
int DrawManager::OnLButtonUp( int x, int y )
{
	m_bDragMoving = false;
	int ret = FALSE;

	if ( m_bDrawingLine )
	{
		m_bDrawingLine = false;

		InfoUIElement* pTargetElement = m_ClickCB( x, y );
		if ( pTargetElement != NULL 
			&& pTargetElement != m_pSelectedElememt
			&& typeid( *pTargetElement ) == typeid( StateElement ) )
		{
			// 鼠标在State上面释放，那么添加Pred
			m_pTempStateForDrawingJumpPred->SetX( x );
			m_pTempStateForDrawingJumpPred->SetY( y );

			m_AddPredCB( dynamic_cast<StateElement*>(m_pSelectedElememt), 
				dynamic_cast<StateElement*>(pTargetElement), m_pDrawingPredElement );

			AddInfoToElement( m_pDrawingPredElement );

			// 成功添加谓词Pred后，清空选中
			SetSelectedDrawingElementType( NULL );
		}
		else
		{
			// 鼠标不在State上面释放，delete调当前绘制的Pred
			SAFE_DELETE( m_pDrawingPredElement );
		}
	}
	ret = TRUE;

	return ret;
}

/*
** 名字：OnMouseMove( int x, int y )
** 说明：响应鼠标移动
**
*/
int DrawManager::OnMouseMove( int x, int y )
{
	int ret =  FALSE;

	if ( m_bDragMoving )	// 拖动State
	{
		assert( m_pSelectedElememt );

		int offsetX = x - m_lastPoint.x;
		int offsetY = y - m_lastPoint.y;

		m_lastPoint.x = x;
		m_lastPoint.y = y;

		// 移动选择元素
		m_pSelectedElememt->SetX( m_pSelectedElememt->GetX() + offsetX );
		m_pSelectedElememt->SetY( m_pSelectedElememt->GetY() + offsetY );
		ret = TRUE;
	}
	else if ( m_bDrawingLine )	// 绘制谓词连线
	{
		// 设置谓词连线目标的XY，因为谓词的终点始终读取目标状态的坐标
		m_pTempStateForDrawingJumpPred->SetX( x - (m_stateWidth >> 1) );
		m_pTempStateForDrawingJumpPred->SetY( y - (m_stateHeight >> 1) );
		ret = TRUE;
	}
	else if ( m_pSelectedDrawingType && m_pSelectedDrawingType->tag == SelectedDrawingType::Tag::STATE )	// 如果有显示，则提供预览
	{
		m_pTempStateForDrawingJumpPred->SetX( x - (m_stateWidth >> 1) );
		m_pTempStateForDrawingJumpPred->SetY( y - (m_stateHeight >> 1) );
		ret = TRUE;
	}

	return ret;	// 通知重绘
}

/*
** 名字：DrawManager
** 说明：删除选中元素
**
*/
int DrawManager::OnDelete() 
{
	int ret = 0; 
	if ( m_pSelectedElememt ) 
	{
		m_pElementAnchor->Detach();					// 将Anchor重置
		ret = m_pSelectedElememt->OnDeleted();		// 从数据源自删除
		m_pSelectedElememt = NULL;
	} 
	return ret; 
}


/*
** 名字：Draw( CDC* pDC )
** 说明：绘图
**
*/
int DrawManager::Draw( CDC* pDC )
{
	// 绘制当前正在画的谓词连线
	if ( m_bDrawingLine )
	{
		m_pDrawingPredElement->Draw( pDC );
	}
	else if ( m_pSelectedDrawingType && m_pSelectedDrawingType->tag == SelectedDrawingType::Tag::STATE )	// 如果有显示，则提供预览
	{
		m_pTempStateForDrawingJumpPred->Draw( pDC );
	}

	m_DrawCB( pDC );	// 绘制容器内元素

	// 为选中的元素绘制选中框
	m_pElementAnchor->Draw( pDC );

	return TRUE;
}

/*
** 名字：AddInfoToElement( InfoUIElement* element )
** 说明：从选中元素信息中为当前绘制的元素添加名字和参数
**
*/
void DrawManager::AddInfoToElement( InfoUIElement* element )
{
	ListElementType::iterator iter = m_pSelectedDrawingType->params->begin();
	element->SetName( *iter );

	ListElementType param;
	std::for_each( ++iter, m_pSelectedDrawingType->params->end(), [&]( CETString& element )
	{
		param.push_back( element );
	});

	element->SetParamNames( param );
}

/*
** 名字：TestAndDoAddStateElement
** 说明：判断是否可以添加StateElement，可以则添加
**
*/
int DrawManager::TestAndDoAddStateElement( int x, int y )
{
	int ret = FALSE;
	if ( ! m_pSelectedElememt && m_pSelectedDrawingType )	// 点中空白区域，且已经选中要绘制的类型，那么准备开始绘图
	{
		// 如果当前绘制类型为状态，添加一个状态
		if ( m_pSelectedDrawingType->tag == SelectedDrawingType::Tag::STATE )
		{
			/*** Test Starts***/
			StateElement* e = new StateElement;

			AddInfoToElement( e );	// 添加参数信息

			e->SetX( x - (m_stateWidth >> 1) );		// 修正坐标
			e->SetY( y - (m_stateHeight >> 1) );
			e->SetHeight( m_stateHeight );
			e->SetWidth( m_stateWidth );	

			m_AddStateCB( e );		 
			/*** Test Ends ***/
			m_pSelectedDrawingType = NULL;	// 设置没有选中
			ret = TRUE;
		}
	}	

	return ret;
}


/*
** 名字：TestAndDoAddPredElement
** 说明：判断是否可以添加谓词，可以则添加。
**
*/
int DrawManager::TestAndDoAddPredElement( int x, int y )
{
	int ret = FALSE;
	if ( m_pSelectedElememt )	// 点中元素
	{
		m_lastPoint.x = x;
		m_lastPoint.y = y;

		// 如果击中元素为State且当前选中的绘制元素为Pred
		if ( typeid( *m_pSelectedElememt ) == typeid( StateElement )
			&& m_pSelectedDrawingType
			&& ( m_pSelectedDrawingType->tag == SelectedDrawingType::Tag::PRED || m_pSelectedDrawingType->tag == SelectedDrawingType::Tag::LOCALPRED ) ) 
		{
			// 局部谓词
			if ( m_pSelectedDrawingType->tag == SelectedDrawingType::Tag::LOCALPRED )
			{
				CETString& predName = m_pSelectedDrawingType->params->at( 0 );

				InfoMananger& infoManager = InfoMananger::GetInstance();
				InfoMananger::ListTpye& predList = infoManager.GetLocalPredMap()[ m_pSelectedElememt->GetName() ];

				bool bFound = false;
				for ( auto iter = predList.begin(); iter != predList.end(); iter++ )
				{
					if ( predName == iter->at( 0 ) )
					{
						bFound = true;
						break;
					}
				}

				if ( bFound )
				{
					m_bDrawingLine = true;		// 设置当前为绘制谓词连线状态
					m_pDrawingPredElement = new PredElement;								// 创建新的谓词连线。
					m_pDrawingPredElement->SetSource( (StateElement*)m_pSelectedElememt );	// 设置谓词起源为当前击中Action。
					m_pDrawingPredElement->SetTarget( m_pTempStateForDrawingJumpPred );		// 设置谓词目标源为临时目标，
					// 鼠标移动时改变这个目标的左边来实现橡皮筋效果。
				}
				else
				{
					CString errorStr;
					errorStr = m_pSelectedElememt->GetName().c_str();
					errorStr += _T("不包含当前局部谓词！");

					m_pSelectedElememt = m_pLastSelectedElememt;	// 还原回当前局部谓词的Action为当前选中元素，认为这样会更友好些
					OnUpdateSelectedElement();						// 通知当前选中元素被修改了

					MessageBox( NULL, errorStr, _T("警告"), MB_OK );

					ret = TRUE;
				}
			}
			else 
			{
				m_bDrawingLine = true;		// 设置当前为绘制谓词连线状态
				m_pDrawingPredElement = new PredElement;								// 创建新的谓词连线。
				m_pDrawingPredElement->SetSource( (StateElement*)m_pSelectedElememt );	// 设置谓词起源为当前击中Action。
				m_pDrawingPredElement->SetTarget( m_pTempStateForDrawingJumpPred );		// 设置谓词目标源为临时目标，
				// 鼠标移动时改变这个目标的左边来实现橡皮筋效果。
			}
		}
		else if ( m_pSelectedElememt && typeid( *m_pSelectedElememt ) == typeid( StateElement ) )// 进入拖动状态
		{
			m_bDragMoving = true;
			ret = TRUE;
		}
	}	

	return ret;
}

void DrawManager::OnUpdateSelectedElement()
{
	m_pElementAnchor->Attach( m_pSelectedElememt );	// 为选中框添加当前选中元素
	m_RefreshPropWndCB();							// 将当前选中元素的信息显示到属性面板
}
