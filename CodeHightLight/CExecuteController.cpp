/*
** 作者：华亮
** 说明：
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
		// 重置语法器
		m_pParser->Reset();
		// 重置相关信息
		Reset();

		CString code;
		CView* pView = pFrame->GetActiveFrame()->GetActiveView();

		if ( pView )
		{
			// 保存代码窗
			//dynamic_cast<CSynEditView*>(pView)->SelectLanguage( _CPP );
			m_viewController.SetView( pView, CODE_VIEW );
			pView->GetWindowText( code );
			//pView->GetDocument()->SetTitle("C_Code");
			// 编译
			m_pParser->LoadFromString( code.GetString() );
			m_pParser->Parse( TRUE );
			m_machine.Reset();

			m_pSyntaxTreeWnd->LoadSyntaxTree( (Fun_StmtMap*)m_pParser->GetSyntaxTree() );

			//中间代码翻译为汇编代码
			Imm2asm i2a;
			CString asmcode = i2a.TranslateToAsm( m_pParser->GetImmCode(), m_pParser->GetTables() ).c_str();

			if ( TRUE )
			{
				CSynEditView* pEdit;

				// 创建中间代码窗
				pFrame->NewView();		// 新建窗口
				pEdit = dynamic_cast<CSynEditView*>( pFrame->GetActiveFrame()->GetActiveView() );
				m_viewController.SetView( pEdit, INTER_VIEW );
				CString immCode = m_pParser->GetImmCode().c_str();
				pEdit->LoadText( immCode );
				CRichEditCtrl& synCtrl_1 = pEdit->GetRichEditCtrl();
				synCtrl_1.SetReadOnly(TRUE);
				pEdit->GetDocument()->SetTitle("Intermediate Code");

				// 创建汇编代码
				pFrame->NewView();
				pEdit = dynamic_cast<CSynEditView*>( pFrame->GetActiveFrame()->GetActiveView() );
				pEdit->SelectLanguage( _ASM );
				m_viewController.SetView( pEdit, ASM_VIEW );
				pEdit->LoadText( asmcode );
				CRichEditCtrl& synCtrl_2 = pEdit->GetRichEditCtrl();
				synCtrl_2.SetReadOnly(TRUE);
				pEdit->GetDocument()->SetTitle("Assembly Code");
			}


			// 为虚拟机加载代码
			m_machine.LoadCode( asmcode.GetString() );

			dynamic_cast<CSynEditView*>(pView)->SelectLanguage( _CPP ); // 转换回C++模式

			Reset();

			MessageBox( pFrame->GetSafeHwnd(), "Compile done!", "Congratulation!", MB_OK );
		}
	}
	catch ( char* err )
	{
		MessageBox( NULL, err, "Error", MB_ICONERROR | MB_OK );
	}
}

// 一次执行完
void CExecuteController::Run( CMainFrame* pFrame )
{
	while ( m_machine.Step() );
}

// 单步
void CExecuteController::Step( CMainFrame* pFrame )
{
	m_machine.Step();
}

// 自动间隔时间动画
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
	/* 注册调试器                                                            */
	/************************************************************************/

	// 注册函数
	auto regist = [&]( IDebugger* debug, std::function< LPVOID(LPVOID) > pfunc )
	{
		debug->SetCallBack( pfunc );				// 为调试器设置回调函数
		m_machine.AddDebugger( debug );			// 将调试器注册到ET机器中
		m_debuggers.push_back( debug );	
	};



	// 负责监视输出信息
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



	// 负责更新行号
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





	// 负责内存信息显示
	pMemViewDebugger = new MemoryViewDebugger;
	auto func4 = [&]( LPVOID param )->LPVOID
	{
		switch ( (int)param )
		{
		case 0:
			return m_pParser;	// 返回语法器
		case 1:
			return &m_pMemSpy;	// 返回内存监控窗口
		}

		return NULL;
	};
	regist( pMemViewDebugger, func4 );



	// 获取关注信息
	pWatchDogDebugger = new WatchDogDebugger;
	auto func5 = [&]( LPVOID param )->LPVOID
	{
		switch ( (int)param )
		{
		case 0:					// 约定传入NULL那么返回语法器
			return m_pParser;
		case 1:					// 约定传入1那么返回感兴趣变量名
			return &m_interestVarNameList;
		case 2:					
			return &m_pMemSpy;	// 返回内存监控窗口
		}

		// 取得关注表
		WatchPropertiesListType* list = (std::list< PropertiesInsertItem >*)param;

		if ( ! m_propertiesSpyFunc )	
		{
			delete list;
			return 0;
		}

		// 调用关键字处理回调
		WPARAM wparam = (WPARAM)m_propertiesSpyFunc( list );

		if ( wparam )	m_pAnimateWnd->PostMessageA( WM_UPDATE_ANIMATE, wparam, 0 );

		delete list;

		return NULL;
	};
	regist( pWatchDogDebugger, func5 );



	// 负责断点处理
	pBreakPointDebugger = new BreakPointDebugger;
	auto func3 = [&]( LPVOID param )->LPVOID
	{
		auto list = dynamic_cast<CSynEditView*>( m_viewController.GetCodeView() )->GetBreakPointList();
		static int count = 0;	// 同一行断点被断次数
		static int lastBreakpoint = -1;
		auto iter = std::find( list.begin(), list.end(), ((int)param)-1 );
		if ( iter != list.end() )
		{
			Pause();	// 通知虚拟机线程暂停			
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
