#ifndef LOCK_BUFFER_H_
#define LOCK_BUFFER_H_

#include "CommBuffer.h"
#include <Windows.h>
class LockBuffer:public CommBuffer
{
public:
	LockBuffer();
	~LockBuffer();

	virtual uint_32 Read(void* pData,uint_32 DataSize,uint_32 ReadSize,bool remove = true);

	virtual uint_32 Write(void* pData,uint_32 DataSize,uint_32 WriteSize);

    virtual uint_32 Remove(uint_32 begin,uint_32 size);

private:
	CRITICAL_SECTION	m_Section;
};
#endif