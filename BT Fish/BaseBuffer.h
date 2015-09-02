#ifndef BASE_BUFFER_H
#define BASE_BUFFER_H
#include "DataType.h"
//用于Buffer读写

//todo: 拷贝，赋值构造函数
class BaseBuffer
{
public:
	BaseBuffer();
	virtual ~BaseBuffer();

	BaseBuffer(const BaseBuffer& buf);

	BaseBuffer& operator =(const BaseBuffer& buf);

	virtual uint_32 Read(void* pData,uint_32 DataSize,uint_32 ReadSize,bool remove = true,uint_32 pos=0);

	virtual uint_32 Write(void* pData,uint_32 DataSize,uint_32 WriteSize);

	uint_32				GetDataSize()	{return m_Pos;}

	const byte*			GetData()	{return m_Data;}
private:
	bool isNeedResizeBuffer(uint_32 uSize);

	void ResizeBuffer(uint_32 size);

	uint_32	GetCapacity();
private:
	byte*	m_Data;
	uint_32	m_Size;
	uint_32	m_Pos;
};


#endif