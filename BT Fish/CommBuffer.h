#ifndef COMM_BUFFER_H
#define COMM_BUFFER_H
#include "BaseBuffer.h"
#include <string>
class CommBuffer:public BaseBuffer
{
public:
	CommBuffer();
	~CommBuffer();

	void ReadInt8(int_8& data,bool bRemove = true,uint_32 pos = 0);
	void WriteInt8(int_8 data);

	void ReadInt16(int_16& data,bool bRemove = true,uint_32 pos = 0);
	void WriteInt16(int_16 data);

	void ReadInt32(int_32& data,bool bRemove = true,uint_32 pos = 0);
	void WriteInt32(int_32 data);

	void ReadInt64(int_64& data,bool bRemove = true,uint_32 pos = 0);
	void WriteInt64(int_64 data);

	uint_32 ReadString(char* str,uint_32 dataSize,uint_32 readSize,bool bRemove = true,uint_32 pos = 0);
	void ReadString(std::string& str,uint_32 readSize,bool bRemove = true,uint_32 pos = 0);
	void WriteString(char* str,uint_32 len);
	void WriteString(std::string str);

	void ReadBuffer(CommBuffer& buffer,bool bRemove = true);
	void WriteBuffer(CommBuffer& buffer);
};

#endif