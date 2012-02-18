/*
** ���ߣ�����
** ˵�������ڽ���ͺ���м�Ŀ�����
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
** ���֣�CExecuteController
** ˵��������Ŀ�����
**
*/
class CExecuteController
{
	AnimatedRunCallBackFuncType		m_animateRunCB;

	IParser*				m_pParser;				// ������ǰ��
	ETMachine				m_machine;				// ��������� 
	CViewController			m_viewController;		// ��ͼ������
	std::list<IDebugger*>	m_debuggers;			// �ṩ�ڴ������
	std::list<std::string>	m_interestVarNameList;	// ����Ȥ���б�

	AnimateRunThread		m_animateRunThread;		// �����߳̿�����

	CAnimationWnd*			m_pAnimateWnd;			// ������
	MemorySpy*				m_pMemSpy;				// �ڴ��
	OutPutResultWnd*		m_pOutput;				// �����
	AbstactSyntaxTree*		m_pSyntaxTreeWnd;		// �﷨����

	WatchPropertiesFuncType m_propertiesSpyFunc;	// WatchDogDebuggerÿ�λ����������ע����

	/************************************************************************/
	/* ������                                                               */
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

	// ���������ͼ
	void ClearAllView();

	// ������ͼ
	void SetView( CView* pView, DWORD flag );

	void SetSyntaxTreeWnd( AbstactSyntaxTree* wnd );
};