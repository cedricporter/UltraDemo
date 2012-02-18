/*
** ���ߣ�����
** ˵����
**
*/

#pragma once

#include <Windows.h>


/*
** ���֣�FSMThread
** ˵�����̻߳��࣬��װ�����̵߳ķ���
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