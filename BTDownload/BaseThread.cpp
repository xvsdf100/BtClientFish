#include "BaseThread.h"

BaseThread::BaseThread()
{
	m_hThread = INVALID_HANDLE_VALUE;
}

BaseThread::~BaseThread()
{
	CloseThread();
}

unsigned int BaseThread::ThreadCallBack(void* lpParam)
{
	BaseThread* pThis = (BaseThread*)lpParam;
	pThis->Run();
	return 1;
}




void BaseThread::CloseThread()
{
	if(m_hThread != INVALID_HANDLE_VALUE)
	{
		_endthreadex((unsigned int)m_hThread);
		m_hThread = INVALID_HANDLE_VALUE;
	}
}


bool BaseThread::CreateThread(PSECURITY_ATTRIBUTES Sec /*= NULL */,unsigned stack_size /*= 0*/,unsigned initflag /*= 0*/)
{
	HANDLE hThread = (HANDLE)_beginthreadex(Sec,stack_size,ThreadCallBack,this,initflag,(unsigned int*)&m_ThreadID);
	if(0 != hThread)
	{
		m_hThread = hThread;
		return true;
	}

	return false;
}