#ifndef _ICONNECTION_H_
#define _ICONNECTION_H_
//Declare:	提供基本连接抽象类

#include <string>

class	IConnect
{
public:
	typedef unsigned char byte;
	//typedef	int SOCKETHNDLE;
public:
	virtual bool Connect(std::string ip,int nPort) = 0;

	virtual int Send(const byte* Data,int nSize) = 0;

	virtual int Recv(byte* Data,int nSize) = 0;

	virtual void Close() = 0;

	virtual int GetHandle() = 0;

	virtual bool isValid() = 0;

};

#endif