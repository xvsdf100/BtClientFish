#include "CommBuffer.h"


CommBuffer::CommBuffer()
{

}

CommBuffer::~CommBuffer()
{

}

void CommBuffer::ReadInt8(int_8& data,bool bRemove,uint_32 pos)
{
	Read(&data,sizeof(data),sizeof(data),bRemove,pos);
}

void CommBuffer::WriteInt8(int_8 data)
{
	Write(&data,sizeof(data),sizeof(data));
}

void CommBuffer::ReadUInt8(uint_8& data,bool bRemove /*= true*/,uint_32 pos /*= 0*/)
{
	Read(&data,sizeof(data),sizeof(data),bRemove,pos);
}

void CommBuffer::WriteUInt8(uint_8 data)
{
	Write(&data,sizeof(data),sizeof(data));
}

void CommBuffer::ReadInt16(int_16& data,bool bRemove,uint_32 pos)
{
	Read(&data,sizeof(data),sizeof(data),bRemove,pos);
}

void CommBuffer::WriteInt16(int_16 data)
{
	Write(&data,sizeof(data),sizeof(data));
}

void CommBuffer::ReadUInt16(uint_16& data,bool bRemove /*= true*/,uint_32 pos /*= 0*/)
{
	Read(&data,sizeof(data),sizeof(data),bRemove,pos);
}

void CommBuffer::WriteUInt16(uint_16 data)
{
	Write(&data,sizeof(data),sizeof(data));
}

void CommBuffer::ReadInt32(int_32& data,bool bRemove,uint_32 pos)
{
	Read(&data,sizeof(data),sizeof(data),bRemove,pos);
}

void CommBuffer::WriteInt32(int_32 data)
{
	Write(&data,sizeof(data),sizeof(data));
}

void CommBuffer::ReadUInt32(uint_32& data,bool bRemove /*= true*/,uint_32 pos /*= 0*/)
{
	Read(&data,sizeof(data),sizeof(data),bRemove,pos);
}

void CommBuffer::WriteUInt32(uint_32 data)
{
	Write(&data,sizeof(data),sizeof(data));
}

void CommBuffer::ReadInt64(int_64& data,bool bRemove,uint_32 pos)
{
	Read(&data,sizeof(data),sizeof(data),bRemove,pos);
}


void CommBuffer::WriteInt64(int_64 data)
{
	Write(&data,sizeof(data),sizeof(data));
}

void CommBuffer::ReadUInt64(uint_64& data,bool bRemove /*= true*/,uint_32 pos /*= 0*/)
{
	Read(&data,sizeof(data),sizeof(data),bRemove,pos);
}

void CommBuffer::WriteUInt64(uint_64 data)
{
	Write(&data,sizeof(data),sizeof(data));
}

uint_32 CommBuffer::ReadString(char* str,uint_32 dataSize,uint_32 readSize,bool bRemove,uint_32 pos)
{
	return Read(str,dataSize,readSize,bRemove,pos);
}

void CommBuffer::ReadString(std::string& str,uint_32 readSize,bool bRemove,uint_32 pos)
{
	//加上终止符号
	str.resize(readSize);
	Read((void*)str.c_str(),readSize,readSize,bRemove,pos);
}

void CommBuffer::WriteString(char* str,uint_32 len)
{
	Write(str,len,len);
}

void CommBuffer::WriteString(std::string str)
{
	Write((char*)str.data(),str.length(),str.length());
}

void CommBuffer::ReadBuffer(CommBuffer& buffer,bool bRemove)
{
	//不是这样写的
	buffer.Write((void*)this->GetData(),this->GetDataSize(),this->GetDataSize());
}

void CommBuffer::WriteBuffer(CommBuffer& buffer)
{
	//Read((void*)buffer.GetData(),buffer.GetDataSize(),buffer.GetDataSize());
	Write((void*)buffer.GetData(),buffer.GetDataSize(),buffer.GetDataSize());
}







