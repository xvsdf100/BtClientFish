// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CommBuffer.h"
#include <iostream>
using namespace std;


void TestBuffer()
{
	CommBuffer buffer;
	buffer.WriteInt8(1);
	buffer.WriteInt16(2);
	buffer.WriteInt32(3);
	buffer.WriteInt64(4);

	int_8 i1 = 0;
	buffer.ReadInt8(i1);
	int_16 i2 = 0;
	buffer.ReadInt16(i2);
	int_32 i3 = 0;
	buffer.ReadInt32(i3);
	int_64 i4 = 0;
	buffer.ReadInt64(i4);

	char buf[]="1234";
	buffer.WriteString(buf,5);

	char buf1[10];
	buffer.ReadString((char*)buf1,10,5);

	string testBuf = "56789";
	buffer.WriteString(testBuf);
	string testBuf1;
	buffer.ReadString(testBuf1,5);

	cout << i1 << "," <<i2<<","<<i3<<","<<i4 <<","<< buf1;

	buffer.WriteInt32(4);
	buffer.ReadInt32(i3,false);
	buffer.ReadInt32(i3,false);
}

void TestVector()
{
	std::vector<int> array;
	array.resize(1);
	array[0] = 23;
}

int _tmain(int argc, _TCHAR* argv[])
{
	TestVector();
	getchar();
	return 0;
}

