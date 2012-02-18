/*
** ���ߣ�����
** ˵����
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

	// Action�;ֲ�ν��
	m_pStateList = parser.GetStateList( _T("data/states.txt" ) );
	for ( auto iter = m_pStateList->begin(); iter != m_pStateList->end(); iter++ )
	{
		m_ActionList.push_back( (*iter)->params );	// ���Action

		if ( ! (*iter)->subPred.empty() )	// �оֲ�ν��
		{
			m_LocalPredMap[ (*iter)->params.at( 0 ) ] = (*iter)->subPred;
		}
	}

	// ȫ��ν��
	m_pPredList = parser.GetStateList( _T("data/Predicates.txt" ) );
	for ( auto iter = m_pPredList->begin(); iter != m_pPredList->end(); iter++ )
	{
		m_PredList.push_back( (*iter)->params );	// ���ȫ��ν��
	}
}

InfoMananger::~InfoMananger()
{
	// ����Action�;ֲ�ν��
	std::for_each( m_pStateList->begin(), m_pStateList->end(), [&]( StateNode* node )
	{
		delete node;
	});
	delete m_pStateList;

	// ����ȫ��ν��
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

	AddHandlers();	// ΪDrawManagerע��ص�����

	// ����GDIplus
	using namespace Gdiplus;	Gdiplus::GdiplusStartupInput gdiplusStartupInput;           	GdiplusStartup( &m_gdiplusToken, &gdiplusStartupInput, NULL ); 
}

GlobalMananger::~GlobalMananger()
{
	delete m_pDrawManager;
	delete m_pData;
	delete m_pContainer;
	delete m_pFSMConfigFile;


	// �ر�GDIplus
	Gdiplus::GdiplusShutdown( m_gdiplusToken );    
}

/*
** ���֣�AddHandlers()
** ˵����ΪDrawManagerע��ص�����
**
*/
void GlobalMananger::AddHandlers()
{
	// һ��Ҫ��m_pDrawManager->m_pAddStateCB��m_pDrawManager->m_pAddPredCB֮ǰ���壬����ɾ���Ļص�����Ϊ��
	m_DeleteFromSourceCB = [&]( InfoUIElement* element )->int
	{
		int ret = m_pContainer->Remove( element );	// ��UI�������Ƴ�
		m_pData->DelElement( element );				// ������Դɾ���Լ���ϵͳ�����ڴ�

		return ret;
	};


	// ������ص�
	m_pDrawManager->m_ClickCB = [&]( int x, int y )->InfoUIElement*
	{
		return dynamic_cast<InfoUIElement*>( m_pContainer->OnClick( x, y ) );
	};

	// ���״̬Action�Ļص�
	m_pDrawManager->m_AddStateCB = [&]( InfoUIElement* element )->int
	{
		element->SetDeleteFromDataSourceCB( & m_DeleteFromSourceCB );	// ���ɾ���ص�

		m_pData->AddElement( element );				// ���浽����Դ�����
		return m_pContainer->Add( element );		// ��ӵ�UI����
	};

	// ���״̬ν�ʵĻص�
	m_pDrawManager->m_AddPredCB = [&]( StateElement* source, StateElement* target, PredElement* pred )->int
	{	
		pred->SetDeleteFromDataSourceCB( & m_DeleteFromSourceCB );	// ���ɾ���ص�

		m_pData->AddElement( pred );		// ���浽����Դ�����	
		m_pContainer->Add( pred );			// ��ӵ�UI����

		// ��״̬��תν����ӵ�״̬
		source->AddJumpPred( pred );		
		target->AddJumpPredIn( pred );

		// Ϊν��������Դ״̬��Ŀ��״̬
		pred->SetSource( source );
		pred->SetTarget( target );

		return 0;
	};

	// �ṩ�������ƵĻص�
	m_pDrawManager->m_DrawCB = [&]( CDC* pDC )->int
	{	
		return m_pContainer->Draw( pDC );
	};

}


/*
** ���֣����������ļ����߳�
** ˵�����������Ϊ��̬�������������̱߳����ļ�
**		[�������] paramstructΪSaveFileStruct�����汣���ļ����ʹ�������ļ��Ļص�������
**			�����ڴ��ڴ˺������ͷš�
**
*/
UINT GlobalMananger::SaveFileThreadFunc( LPVOID paramstruct )
{
	SaveFileStruct* param = (SaveFileStruct*)paramstruct;
	SaveFileStruct::SAVEFILEFUNC* pFunc = param->pFunc;

	(*pFunc)( param->filename );	// �����ļ�

	delete pFunc;	// ���ջص�����
	delete param;	// ���ղ����ṹ��

	return TRUE;
}


int GlobalMananger::SaveFSMConfigFile( const CETString& filename )
{
	// ʹ��MFC�Ĵ��������̷߳�ʽ
	MFCFSMThread mfcThread;
	FSMThread& fsmThread = mfcThread;

	// �����ļ��Ļص�����
	SaveFileStruct::SAVEFILEFUNC* pFunc = new SaveFileStruct::SAVEFILEFUNC;
	*pFunc = [&]( const CETString& file_name )->int 
	{
		m_pFSMConfigFile->LoadFSMDescription( *m_pData );
		m_pFSMConfigFile->SaveConfigFile( file_name );
		return TRUE;
	};

	// ������Ϣ
	SaveFileStruct* pParam = new SaveFileStruct;
	pParam->pFunc = pFunc;
	pParam->filename = filename;

	// �������߳�
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
** ���֣�OnLButtonDown( int x, int y )
** ˵������Ӧ����������
**
*/
int DrawManager::OnLButtonDown( int x, int y )
{
	int ret = FALSE;	// ��Ҫ�ػ������ΪTRUE
	m_pLastSelectedElememt = m_pSelectedElememt;// �����ϴ�ѡ��Ԫ��

	// ���в���
	m_pSelectedElememt = m_ClickCB( x, y );		// ��ȡѡ��Ԫ�أ�δѡ���κεõ�NULL
	OnUpdateSelectedElement();					// ����ǰԪ�ظı����Ϣ

	ret |= TestAndDoAddStateElement( x, y );	// �������������StateElement
	ret |= TestAndDoAddPredElement( x, y );		// �������������ν�ʻ��߽����϶�״̬


	return ret;	// ����TRUE֪ͨ�ػ�
}

/*
** ���֣�OnLButtonUp( int x, int y )
** ˵������Ӧ��굯��
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
			// �����State�����ͷţ���ô���Pred
			m_pTempStateForDrawingJumpPred->SetX( x );
			m_pTempStateForDrawingJumpPred->SetY( y );

			m_AddPredCB( dynamic_cast<StateElement*>(m_pSelectedElememt), 
				dynamic_cast<StateElement*>(pTargetElement), m_pDrawingPredElement );

			AddInfoToElement( m_pDrawingPredElement );

			// �ɹ����ν��Pred�����ѡ��
			SetSelectedDrawingElementType( NULL );
		}
		else
		{
			// ��겻��State�����ͷţ�delete����ǰ���Ƶ�Pred
			SAFE_DELETE( m_pDrawingPredElement );
		}
	}
	ret = TRUE;

	return ret;
}

/*
** ���֣�OnMouseMove( int x, int y )
** ˵������Ӧ����ƶ�
**
*/
int DrawManager::OnMouseMove( int x, int y )
{
	int ret =  FALSE;

	if ( m_bDragMoving )	// �϶�State
	{
		assert( m_pSelectedElememt );

		int offsetX = x - m_lastPoint.x;
		int offsetY = y - m_lastPoint.y;

		m_lastPoint.x = x;
		m_lastPoint.y = y;

		// �ƶ�ѡ��Ԫ��
		m_pSelectedElememt->SetX( m_pSelectedElememt->GetX() + offsetX );
		m_pSelectedElememt->SetY( m_pSelectedElememt->GetY() + offsetY );
		ret = TRUE;
	}
	else if ( m_bDrawingLine )	// ����ν������
	{
		// ����ν������Ŀ���XY����Ϊν�ʵ��յ�ʼ�ն�ȡĿ��״̬������
		m_pTempStateForDrawingJumpPred->SetX( x - (m_stateWidth >> 1) );
		m_pTempStateForDrawingJumpPred->SetY( y - (m_stateHeight >> 1) );
		ret = TRUE;
	}
	else if ( m_pSelectedDrawingType && m_pSelectedDrawingType->tag == SelectedDrawingType::Tag::STATE )	// �������ʾ�����ṩԤ��
	{
		m_pTempStateForDrawingJumpPred->SetX( x - (m_stateWidth >> 1) );
		m_pTempStateForDrawingJumpPred->SetY( y - (m_stateHeight >> 1) );
		ret = TRUE;
	}

	return ret;	// ֪ͨ�ػ�
}

/*
** ���֣�DrawManager
** ˵����ɾ��ѡ��Ԫ��
**
*/
int DrawManager::OnDelete() 
{
	int ret = 0; 
	if ( m_pSelectedElememt ) 
	{
		m_pElementAnchor->Detach();					// ��Anchor����
		ret = m_pSelectedElememt->OnDeleted();		// ������Դ��ɾ��
		m_pSelectedElememt = NULL;
	} 
	return ret; 
}


/*
** ���֣�Draw( CDC* pDC )
** ˵������ͼ
**
*/
int DrawManager::Draw( CDC* pDC )
{
	// ���Ƶ�ǰ���ڻ���ν������
	if ( m_bDrawingLine )
	{
		m_pDrawingPredElement->Draw( pDC );
	}
	else if ( m_pSelectedDrawingType && m_pSelectedDrawingType->tag == SelectedDrawingType::Tag::STATE )	// �������ʾ�����ṩԤ��
	{
		m_pTempStateForDrawingJumpPred->Draw( pDC );
	}

	m_DrawCB( pDC );	// ����������Ԫ��

	// Ϊѡ�е�Ԫ�ػ���ѡ�п�
	m_pElementAnchor->Draw( pDC );

	return TRUE;
}

/*
** ���֣�AddInfoToElement( InfoUIElement* element )
** ˵������ѡ��Ԫ����Ϣ��Ϊ��ǰ���Ƶ�Ԫ��������ֺͲ���
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
** ���֣�TestAndDoAddStateElement
** ˵�����ж��Ƿ�������StateElement�����������
**
*/
int DrawManager::TestAndDoAddStateElement( int x, int y )
{
	int ret = FALSE;
	if ( ! m_pSelectedElememt && m_pSelectedDrawingType )	// ���пհ��������Ѿ�ѡ��Ҫ���Ƶ����ͣ���ô׼����ʼ��ͼ
	{
		// �����ǰ��������Ϊ״̬�����һ��״̬
		if ( m_pSelectedDrawingType->tag == SelectedDrawingType::Tag::STATE )
		{
			/*** Test Starts***/
			StateElement* e = new StateElement;

			AddInfoToElement( e );	// ��Ӳ�����Ϣ

			e->SetX( x - (m_stateWidth >> 1) );		// ��������
			e->SetY( y - (m_stateHeight >> 1) );
			e->SetHeight( m_stateHeight );
			e->SetWidth( m_stateWidth );	

			m_AddStateCB( e );		 
			/*** Test Ends ***/
			m_pSelectedDrawingType = NULL;	// ����û��ѡ��
			ret = TRUE;
		}
	}	

	return ret;
}


/*
** ���֣�TestAndDoAddPredElement
** ˵�����ж��Ƿ�������ν�ʣ���������ӡ�
**
*/
int DrawManager::TestAndDoAddPredElement( int x, int y )
{
	int ret = FALSE;
	if ( m_pSelectedElememt )	// ����Ԫ��
	{
		m_lastPoint.x = x;
		m_lastPoint.y = y;

		// �������Ԫ��ΪState�ҵ�ǰѡ�еĻ���Ԫ��ΪPred
		if ( typeid( *m_pSelectedElememt ) == typeid( StateElement )
			&& m_pSelectedDrawingType
			&& ( m_pSelectedDrawingType->tag == SelectedDrawingType::Tag::PRED || m_pSelectedDrawingType->tag == SelectedDrawingType::Tag::LOCALPRED ) ) 
		{
			// �ֲ�ν��
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
					m_bDrawingLine = true;		// ���õ�ǰΪ����ν������״̬
					m_pDrawingPredElement = new PredElement;								// �����µ�ν�����ߡ�
					m_pDrawingPredElement->SetSource( (StateElement*)m_pSelectedElememt );	// ����ν����ԴΪ��ǰ����Action��
					m_pDrawingPredElement->SetTarget( m_pTempStateForDrawingJumpPred );		// ����ν��Ŀ��ԴΪ��ʱĿ�꣬
					// ����ƶ�ʱ�ı����Ŀ��������ʵ����Ƥ��Ч����
				}
				else
				{
					CString errorStr;
					errorStr = m_pSelectedElememt->GetName().c_str();
					errorStr += _T("��������ǰ�ֲ�ν�ʣ�");

					m_pSelectedElememt = m_pLastSelectedElememt;	// ��ԭ�ص�ǰ�ֲ�ν�ʵ�ActionΪ��ǰѡ��Ԫ�أ���Ϊ��������Ѻ�Щ
					OnUpdateSelectedElement();						// ֪ͨ��ǰѡ��Ԫ�ر��޸���

					MessageBox( NULL, errorStr, _T("����"), MB_OK );

					ret = TRUE;
				}
			}
			else 
			{
				m_bDrawingLine = true;		// ���õ�ǰΪ����ν������״̬
				m_pDrawingPredElement = new PredElement;								// �����µ�ν�����ߡ�
				m_pDrawingPredElement->SetSource( (StateElement*)m_pSelectedElememt );	// ����ν����ԴΪ��ǰ����Action��
				m_pDrawingPredElement->SetTarget( m_pTempStateForDrawingJumpPred );		// ����ν��Ŀ��ԴΪ��ʱĿ�꣬
				// ����ƶ�ʱ�ı����Ŀ��������ʵ����Ƥ��Ч����
			}
		}
		else if ( m_pSelectedElememt && typeid( *m_pSelectedElememt ) == typeid( StateElement ) )// �����϶�״̬
		{
			m_bDragMoving = true;
			ret = TRUE;
		}
	}	

	return ret;
}

void DrawManager::OnUpdateSelectedElement()
{
	m_pElementAnchor->Attach( m_pSelectedElememt );	// Ϊѡ�п���ӵ�ǰѡ��Ԫ��
	m_RefreshPropWndCB();							// ����ǰѡ��Ԫ�ص���Ϣ��ʾ���������
}
