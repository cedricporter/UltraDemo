#include "stdafx.h"
#include "Threads.h"
#include<Mmsystem.h>   #pragma comment(lib,   "Winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
//
// Class AnimateRunThread
// 

CCriticalSection AnimateRunThread::m_cs;

volatile bool AnimateRunThread::m_bRun = true;
volatile bool AnimateRunThread::m_bWaited = false;
volatile bool AnimateRunThread::m_bWorking = false;

AnimateRunThread::AnimateRunThread() : m_handle( 0 )
{

}

AnimateRunThread::~AnimateRunThread()
{
	m_bRun = false;
	m_bWaited = false;	

	if ( m_handle != 0 )
	{
		TerminateThread();
	}

	//WaitForSingleObject(reinterpret_cast<HANDLE>( m_handle ),INFINITE); //一直等到线程结束
	//CloseHandle(reinterpret_cast<HANDLE>( m_handle ));

}


// 时间间隔线程
UINT AnimateRunThread::_AnimateRunThread( LPVOID param )
{
	m_cs.Lock();
	{
		m_bWorking = true;
	}
	m_cs.Unlock();

	//MessageBox( NULL, _T("Thread Starts"), _T("Tips"), MB_OK );

	AnimateRunThreadParam* p = (AnimateRunThreadParam*)param;
	const DWORD Interval = p->time;
	auto func = p->func;
	delete p;

	PlaySound( "backgound.wav", AfxGetResourceHandle(), SND_ASYNC| SND_FILENAME | SND_LOOP );

	DWORD startTime = GetTickCount();
	int flag = TRUE;
	bool bRun = m_bRun;
	bool bWaited = false;
	while ( flag && bRun )
	{
		// 等待
		while ( GetTickCount() - startTime  < Interval )
		{
			Sleep( 10 );
		}
		startTime = GetTickCount();

		m_cs.Lock();
		{
			bWaited = m_bWaited;
			bRun = m_bRun;
		}
		m_cs.Unlock();

		if ( bWaited )
		{
			Sleep( 15 );
		}
		else
		{
			m_cs.Lock();
			{
				flag = func();
			}
			m_cs.Unlock();
		}
	}

	m_cs.Lock();
	{
		m_bWorking = false;
	}
	m_cs.Unlock();

	PlaySound( NULL, AfxGetResourceHandle(), SND_PURGE | SND_FILENAME );

	MessageBox( NULL, _T("Virtual Machine Done"), _T("Tips"), MB_OK );

	/*::_endthread();*/
	return 1234;
	//	ResetEvent( m_event );
}

void AnimateRunThread::Reset()
{
	m_bRun = false;
	m_bWaited = false;
}

void AnimateRunThread::Stop()
{
	m_cs.Lock();
	{
		m_bRun = false;
	}
	m_cs.Unlock();

	TerminateThread();
}

void AnimateRunThread::Resume()
{
	m_cs.Lock();
	{
		m_bWaited = false;
	}
	m_cs.Unlock();
}

void AnimateRunThread::Pause()
{
	m_cs.Lock();
	{
		m_bWaited = true;
	}
	m_cs.Unlock();
}

void AnimateRunThread::Start( const AnimateRunThreadParam* param )
{
	if ( m_handle )
	{
		Stop();
	}

	Reset();

	bool bRun = true;
	m_cs.Lock();
	{
		bRun = m_bRun;
		if ( m_bRun == true )	Stop();
	}
	m_cs.Unlock();

	if ( bRun == false )
	{
		m_cs.Lock();
		{
			m_bRun = true;
		}
		m_cs.Unlock();
		unsigned int hThread = NULL;
		unsigned nThreadID;
		m_handle = ::_beginthreadex( NULL, 0, _AnimateRunThread, (LPVOID)param, 0, &nThreadID );	
	}
}

void AnimateRunThread::TerminateThread()
{
	if ( m_handle )
	{
		m_bRun = false;
		m_bWaited = false;

		while ( m_bWorking )
		{
			Sleep( 10 );
		}

		CloseHandle( (HANDLE)m_handle );
		m_handle = 0;

		//::TerminateThread( (HANDLE)m_handle, 0 );
		//::WaitForSingleObject( m_event.m_hObject, INFINITE ); //一直等到线程结束
		//::CloseHandle(reinterpret_cast<HANDLE>( m_handle ));
		//	m_handle = 0;
	}
}


