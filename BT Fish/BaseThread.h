#ifndef BASE_THREAD
#define BASE_THREAD
#include <process.h>
#include <windows.h>

class BaseThread
{
public:
	BaseThread();
	virtual ~BaseThread();

	static  unsigned int  _stdcall ThreadCallBack(void* lpParam);

	virtual 

	bool	CreateThread(PSECURITY_ATTRIBUTES Sec = NULL ,unsigned stack_size = 0,unsigned initflag = 0);

	void	CloseThread();

	virtual void Run() = 0;

protected:
	HANDLE	m_hThread;
	DWORD	m_ThreadID;
};

#endif