#pragma once



// CAnimationThread

class CAnimationThread : public CWinThread
{
	DECLARE_DYNCREATE(CAnimationThread)

protected:
	CAnimationThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CAnimationThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


