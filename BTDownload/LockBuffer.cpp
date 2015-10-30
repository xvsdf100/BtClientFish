#include "LockBuffer.h"

LockBuffer::LockBuffer()
{
	InitializeCriticalSection(&m_Section);
}

LockBuffer::~LockBuffer()
{
	DeleteCriticalSection(&m_Section);
}

uint_32 LockBuffer::Read(void* pData,uint_32 DataSize,uint_32 ReadSize,bool remove /*= true*/)
{
	uint_32 ret = 0;
	EnterCriticalSection(&m_Section);
	ret = __super::Read(pData,DataSize,ReadSize,remove);
	LeaveCriticalSection(&m_Section);
	return ret;
}

uint_32 LockBuffer::Write(void* pData,uint_32 DataSize,uint_32 WriteSize)
{
	uint_32 ret = 0;
	EnterCriticalSection(&m_Section);
	ret = __super::Write(pData,DataSize,WriteSize);
	LeaveCriticalSection(&m_Section);
	return ret;
}

uint_32 LockBuffer::Remove( uint_32 begin,uint_32 size )
{
    EnterCriticalSection(&m_Section);
    return __super::Remove(begin,size);
    LeaveCriticalSection(&m_Section);
}

