#pragma once



// CAnimationThread

class CAnimationThread : public CWinThread
{
	DECLARE_DYNCREATE(CAnimationThread)

protected:
	CAnimationThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CAnimationThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


