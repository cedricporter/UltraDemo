/*
** ���ߣ�����
** ˵����
**
*/

#pragma once

#include <Windows.h>
#include <functional>


typedef std::function< int() > AnimatedRunCallBackFuncType;


/*
** ���֣�AnimateRunThreadParam
** ˵�����̶߳����Ĳ���
**
*/
struct AnimateRunThreadParam
{
	DWORD						time;	// �ӳ�ʱ��
	AnimatedRunCallBackFuncType	func;	// �ص�����
};



/*
** ���֣�AnimateRunThread
** ˵���������߳�
**
*/
class AnimateRunThread
{
private:
	static CCriticalSection	m_cs;				// �ٽ��������̻߳�����
	volatile static bool	m_bRun;				// ���ڱ�Ƕ����߳��Ƿ����ڹ���������Ϊfalseʱ�����߳�ֹͣ
	volatile static bool	m_bWaited;			// ���ڱ���߳��Ƿ�
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

	// ʱ�����߳�
	static UINT __stdcall _AnimateRunThread( LPVOID param );
};