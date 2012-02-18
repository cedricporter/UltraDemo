/*
** ���ߣ�����
** ˵����
**
*/


#include "stdafx.h"
#include "CExecuteController.h"
#include "Syntax.h"
#include "MainFrm.h"
#include "PropertiesWnd.h"
#include "JiafeiDebugger.h"
#include "MessageDefinition.h"
#include <algorithm>
#include "ArrayContainer.h"
#include "LinkedListContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
//
// Class CExecuteController
// 
CExecuteController::CExecuteController() : m_propertiesSpyFunc( 0 )
{
	pLogDebugger = NULL;
	pLineDebugger = NULL;
	pMemViewDebugger = NULL;
	pBreakPointDebugger = NULL;
	pWatchDogDebugger = NULL;

	m_pParser = new CParser;

	RegisterCallbackToDebuggers();

	/************************************************************************/
	/* Test                                                                 */
	/************************************************************************/

	ClearInterestingList();



}

CExecuteController::~CExecuteController()
{
	m_animateRunThread.Stop();

	delete m_pParser;

	//SAFE_DELETE( pLogDebugger );
	//SAFE_DELETE( pLineDebugger );
	//SAFE_DELETE( pMemViewDebugger );
	//SAFE_DELETE( pBreakPointDebugger );
	//SAFE_DELETE( pWatchDogDebugger );

	for ( auto iter = m_debuggers.begin(); iter != m_debuggers.end(); iter++ )
	{
		delete *iter;
	}
}

void CExecuteController::Compile( CMainFrame* pFrame )
{
	try
	{
		// �����﷨��
		m_pParser->Reset();
		// ���������Ϣ
		Reset();

		CString code;
		CView* pView = pFrame->GetActiveFrame()->GetActiveView();

		if ( pView )
		{
			// ������봰
			//dynamic_cast<CSynEditView*>(pView)->SelectLanguage( _CPP );
			m_viewController.SetView( pView, CODE_VIEW );
			pView->GetWindowText( code );
			//pView->GetDocument()->SetTitle("C_Code");
			// ����
			m_pParser->LoadFromString( code.GetString() );
			m_pParser->Parse( TRUE );
			m_machine.Reset();

			m_pSyntaxTreeWnd->LoadSyntaxTree( (Fun_StmtMap*)m_pParser->GetSyntaxTree() );

			//�м���뷭��Ϊ������
			Imm2asm i2a;
			CString asmcode = i2a.TranslateToAsm( m_pParser->GetImmCode(), m_pParser->GetTables() ).c_str();

			if ( TRUE )
			{
				CSynEditView* pEdit;

				// �����м���봰
				pFrame->NewView();		// �½�����
				pEdit = dynamic_cast<CSynEditView*>( pFrame->GetActiveFrame()->GetActiveView() );
				m_viewController.SetView( pEdit, INTER_VIEW );
				CString immCode = m_pParser->GetImmCode().c_str();
				pEdit->LoadText( immCode );
				CRichEditCtrl& synCtrl_1 = pEdit->GetRichEditCtrl();
				synCtrl_1.SetReadOnly(TRUE);
				pEdit->GetDocument()->SetTitle("Intermediate Code");

				// ����������
				pFrame->NewView();
				pEdit = dynamic_cast<CSynEditView*>( pFrame->GetActiveFrame()->GetActiveView() );
				pEdit->SelectLanguage( _ASM );
				m_viewController.SetView( pEdit, ASM_VIEW );
				pEdit->LoadText( asmcode );
				CRichEditCtrl& synCtrl_2 = pEdit->GetRichEditCtrl();
				synCtrl_2.SetReadOnly(TRUE);
				pEdit->GetDocument()->SetTitle("Assembly Code");
			}


			// Ϊ��������ش���
			m_machine.LoadCode( asmcode.GetString() );

			dynamic_cast<CSynEditView*>(pView)->SelectLanguage( _CPP ); // ת����C++ģʽ

			Reset();

			MessageBox( pFrame->GetSafeHwnd(), "Compile done!", "Congratulation!", MB_OK );
		}
	}
	catch ( char* err )
	{
		MessageBox( NULL, err, "Error", MB_ICONERROR | MB_OK );
	}
}

// һ��ִ����
void CExecuteController::Run( CMainFrame* pFrame )
{
	while ( m_machine.Step() );
}

// ����
void CExecuteController::Step( CMainFrame* pFrame )
{
	m_machine.Step();
}

// �Զ����ʱ�䶯��
void CExecuteController::AnimatedRun( DWORD interval )
{	
	AnimateRunThreadParam* param = new AnimateRunThreadParam;
	param->time = interval;
	param->func = [&]()->int
	{
		return m_machine.Step();
	};

	m_machine.Reset();
	m_animateRunThread.Start( param );
}

void CExecuteController::Pause()
{
	m_animateRunThread.Pause();
}

void CExecuteController::Stop()
{
	m_animateRunThread.Stop();
}

void CExecuteController::Resume()
{
	m_animateRunThread.Resume();
}

void CExecuteController::RegisterCallbackToDebuggers()
{
	/************************************************************************/
	/* ע�������                                                            */
	/************************************************************************/

	// ע�ắ��
	auto regist = [&]( IDebugger* debug, std::function< LPVOID(LPVOID) > pfunc )
	{
		debug->SetCallBack( pfunc );				// Ϊ���������ûص�����
		m_machine.AddDebugger( debug );			// ��������ע�ᵽET������
		m_debuggers.push_back( debug );	
	};



	// ������������Ϣ
	pLogDebugger = new LogDebugger;
	auto func = [&]( LPVOID param )->LPVOID
	{	
		if ( ! dynamic_cast<LogDebugger*>( pLogDebugger )->GetLog().empty() )
		{
			CString* log = new CString( dynamic_cast<LogDebugger*>( pLogDebugger )->GetLog().c_str() );		
			m_pOutput->PostMessageA( WM_OUTPUT, (WPARAM)log );
		}
		return NULL;
	};
	regist( pLogDebugger, func );



	// ��������к�
	pLineDebugger = new LineInfoDebugger;
	auto func2 = [&]( LPVOID param )->LPVOID
	{
		static int lineno = 0;
		if ( (int)param != lineno )
		{
			lineno = (int)param;
			dynamic_cast<CSynEditView*>( m_viewController.GetCodeView() )->SelectLine( lineno );
		}
		return NULL;
	};
	regist( pLineDebugger, func2 );





	// �����ڴ���Ϣ��ʾ
	pMemViewDebugger = new MemoryViewDebugger;
	auto func4 = [&]( LPVOID param )->LPVOID
	{
		switch ( (int)param )
		{
		case 0:
			return m_pParser;	// �����﷨��
		case 1:
			return &m_pMemSpy;	// �����ڴ��ش���
		}

		return NULL;
	};
	regist( pMemViewDebugger, func4 );



	// ��ȡ��ע��Ϣ
	pWatchDogDebugger = new WatchDogDebugger;
	auto func5 = [&]( LPVOID param )->LPVOID
	{
		switch ( (int)param )
		{
		case 0:					// Լ������NULL��ô�����﷨��
			return m_pParser;
		case 1:					// Լ������1��ô���ظ���Ȥ������
			return &m_interestVarNameList;
		case 2:					
			return &m_pMemSpy;	// �����ڴ��ش���
		}

		// ȡ�ù�ע��
		WatchPropertiesListType* list = (std::list< PropertiesInsertItem >*)param;

		if ( ! m_propertiesSpyFunc )	
		{
			delete list;
			return 0;
		}

		// ���ùؼ��ִ���ص�
		WPARAM wparam = (WPARAM)m_propertiesSpyFunc( list );

		if ( wparam )	m_pAnimateWnd->PostMessageA( WM_UPDATE_ANIMATE, wparam, 0 );

		delete list;

		return NULL;
	};
	regist( pWatchDogDebugger, func5 );



	// ����ϵ㴦��
	pBreakPointDebugger = new BreakPointDebugger;
	auto func3 = [&]( LPVOID param )->LPVOID
	{
		auto list = dynamic_cast<CSynEditView*>( m_viewController.GetCodeView() )->GetBreakPointList();
		static int count = 0;	// ͬһ�жϵ㱻�ϴ���
		static int lastBreakpoint = -1;
		auto iter = std::find( list.begin(), list.end(), ((int)param)-1 );
		if ( iter != list.end() )
		{
			Pause();	// ֪ͨ������߳���ͣ			
		}

		return NULL;
	};
	regist( pBreakPointDebugger, func3 );


}

void CExecuteController::Reset()
{
	if ( pLogDebugger ) dynamic_cast<LogDebugger*>( pLogDebugger )->ClearLog();

}


void CExecuteController::ClearAllView()
{
	m_viewController.Clear();
}

void CExecuteController::SetView( CView* pView, DWORD flag )
{
	m_viewController.SetView( pView, flag );
}

void CExecuteController::SetAnimateWnd( CAnimationWnd* wnd )
{
	m_pAnimateWnd = wnd;
}

void CExecuteController::SetMemSpyWnd( MemorySpy* wnd )
{
	m_pMemSpy = wnd;
}

void CExecuteController::SetOutputWnd( OutPutResultWnd* wnd )
{
	m_pOutput = wnd;
}

void CExecuteController::LoadAnimationContainer( Container* animationContainer )
{
	m_pAnimateWnd->LoadAnimation( animationContainer );
}

void CExecuteController::SetAnimateWatchCallback( WatchPropertiesFuncType function )
{
	m_propertiesSpyFunc = function;
}

void CExecuteController::ClearInterestingList()
{
	m_interestVarNameList.clear();
}

void CExecuteController::AddWatchedVariable( const std::string& varname )
{
	m_interestVarNameList.push_back( varname );
}


void CExecuteController::SetSyntaxTreeWnd( AbstactSyntaxTree* wnd )
{
	m_pSyntaxTreeWnd = wnd;
}
