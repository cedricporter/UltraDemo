/*
** 作者：华亮
** 说明：处于界面和后端中间的控制类
**
*/

#pragma once
#include <Windows.h>
#include <functional>
#include "ETMachine.h"
#include "Parser.h"
#include "ViewController.h"
#include "SynEditView.h"
#include "Imm2asm.h"
#include "Threads.h"
#include "AnimationWnd.h"
#include "OutPutResultWnd.h"
#include "MemerySpy.h"
#include "AbstactSyntaxTree.h"
#include <list>
#include "AnimationFactory.h"

using namespace ETCompiler;

class CMainFrame;
class CPropertiesWnd;



/*
** 名字：CExecuteController
** 说明：界面的控制类
**
*/
class CExecuteController
{
	AnimatedRunCallBackFuncType		m_animateRunCB;

	IParser*				m_pParser;				// 编译器前端
	ETMachine				m_machine;				// 编译器后端 
	CViewController			m_viewController;		// 视图控制类
	std::list<IDebugger*>	m_debuggers;			// 提供内存回收用
	std::list<std::string>	m_interestVarNameList;	// 感兴趣里列表

	AnimateRunThread		m_animateRunThread;		// 动画线程控制类

	CAnimationWnd*			m_pAnimateWnd;			// 动画框
	MemorySpy*				m_pMemSpy;				// 内存框
	OutPutResultWnd*		m_pOutput;				// 输出框
	AbstactSyntaxTree*		m_pSyntaxTreeWnd;		// 语法树窗

	WatchPropertiesFuncType m_propertiesSpyFunc;	// WatchDogDebugger每次会调用它将关注表传入

	/************************************************************************/
	/* 调试器                                                               */
	/************************************************************************/
	IDebugger* pLogDebugger;
	IDebugger* pLineDebugger;
	IDebugger* pMemViewDebugger;
	IDebugger* pBreakPointDebugger;
	IDebugger* pWatchDogDebugger;

public:

	CExecuteController();

	~CExecuteController();

	void ClearInterestingList();

	void AddWatchedVariable( const std::string& varname );

	void RegisterCallbackToDebuggers();

	void SetAnimationSpeed( int speed );

	void Compile( CMainFrame* pFrame );

	void Reset();

	void Run( CMainFrame* pFrame );

	void Step( CMainFrame* pFrame );

	void AnimatedRun( DWORD interval );

	void Pause();

	void Stop();

	void Resume();

	void LoadAnimationContainer( Container* animationContainer );

	void SetAnimateWnd( CAnimationWnd* wnd );

	void SetMemSpyWnd( MemorySpy* wnd );

	void SetOutputWnd( OutPutResultWnd* wnd );

	void SetAnimateWatchCallback( WatchPropertiesFuncType function );

	// 清楚所有视图
	void ClearAllView();

	// 设置视图
	void SetView( CView* pView, DWORD flag );

	void SetSyntaxTreeWnd( AbstactSyntaxTree* wnd );
};