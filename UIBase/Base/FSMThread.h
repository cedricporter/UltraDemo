/*
** 作者：华亮
** 说明：
**
*/

#pragma once

#include <Windows.h>


/*
** 名字：FSMThread
** 说明：线程基类，封装创建线程的方法
**
*/
class FSMThread
{
public:
	typedef UINT (*THREAD_PROC)( LPVOID param );

	virtual void BeginThread( THREAD_PROC proc, LPVOID param ) = 0;

};

class MFCFSMThread : public FSMThread
{
public:
	void BeginThread( THREAD_PROC proc, LPVOID param )
	{
		AfxBeginThread( proc, param );
	}
};