#include "BaseBuffer.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>


#include <assert.h>


BaseBuffer::BaseBuffer()
{
	m_Data = NULL;
	m_Pos = 0;
	m_Size = 0;
	ResizeBuffer(100);
}

BaseBuffer::BaseBuffer(const BaseBuffer& buf)
{
	uint_32 dataSize = buf.m_Pos;
	if(dataSize)
	{
		ResizeBuffer(dataSize);
		m_Pos = 0;
		Write(buf.m_Data,buf.m_Size,dataSize);
	}
}


BaseBuffer& BaseBuffer::operator=(const BaseBuffer& buf)
{
	uint_32 dataSize = buf.m_Pos;
	if(dataSize)
	{
		ResizeBuffer(dataSize);
		m_Pos = 0;
		Write(buf.m_Data,buf.m_Size,dataSize);
	}

	return *this;
}


BaseBuffer::~BaseBuffer()
{
	if(NULL != m_Data)
	{
		free(m_Data);
		m_Data = NULL;
	}
}

uint_32 BaseBuffer::Read(void* pData,uint_32 DataSize,uint_32 ReadSize,bool remove,uint_32 pos)
{
	uint_32 MaxRead = m_Pos - pos;
	uint_32 ReadNum = ReadSize > MaxRead ? MaxRead : ReadSize;

	assert(MaxRead >= 0);
	memcpy_s(pData,DataSize,m_Data + pos,ReadNum);

	if(remove)
	{
		m_Pos -= ReadNum;
		memmove_s(m_Data + pos ,m_Size,m_Data + pos + ReadNum,m_Pos);
	}

	return MaxRead;
}

uint_32 BaseBuffer::Write(void* pData,uint_32 DataSize,uint_32 WriteSize)
{
	uint_32 MaxWrite =  WriteSize > DataSize ? DataSize : WriteSize;
	ResizeBuffer(WriteSize);
	memcpy_s(m_Data + m_Pos,GetCapacity(),pData,MaxWrite);
	m_Pos += MaxWrite;
	return MaxWrite;
}


bool BaseBuffer::isNeedResizeBuffer(uint_32 uSize)
{
	uint_32 capacity = GetCapacity();
	if(capacity >= uSize)
	{
		return false;
	}
	return true;
}


void BaseBuffer::ResizeBuffer(uint_32 size)
{
	if(isNeedResizeBuffer(size))
	{
		uint_32 newSize = m_Size + size;
		m_Data = (byte*)realloc(m_Data,newSize);
		assert(NULL != m_Data);
		m_Size = newSize;
	}
}

uint_32 BaseBuffer::GetCapacity()
{
	return (m_Size - m_Pos);
}

