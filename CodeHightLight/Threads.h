/*
** 作者：华亮
** 说明：
**
*/

#pragma once

#include <Windows.h>
#include <functional>


typedef std::function< int() > AnimatedRunCallBackFuncType;


/*
** 名字：AnimateRunThreadParam
** 说明：线程动画的参数
**
*/
struct AnimateRunThreadParam
{
	DWORD						time;	// 延迟时间
	AnimatedRunCallBackFuncType	func;	// 回调函数
};



/*
** 名字：AnimateRunThread
** 说明：动画线程
**
*/
class AnimateRunThread
{
private:
	static CCriticalSection	m_cs;				// 临界区，供线程互斥用
	volatile static bool	m_bRun;				// 用于标记动画线程是否正在工作：设置为false时动画线程停止
	volatile static bool	m_bWaited;			// 用于标记线程是否
	volatile static bool	m_bWorking;

	UINT				m_handle;

public:
	AnimateRunThread();
	~AnimateRunThread();

	void Start( const AnimateRunThreadParam* param );

	void Pause();

	void Resume();

	void Stop();

	void TerminateThread();

	void Reset();

	// 时间间隔线程
	static UINT __stdcall _AnimateRunThread( LPVOID param );
};