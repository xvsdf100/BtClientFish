#include "stdafx.h"
#include "BTProtocal.h"
#include <string>

using namespace std;

BTProtocal::BTProtocal(std::string infoHash,uint_32 pieceCount,uint_32 pieceLen):m_SliceLen(1024*16)
{
	m_InfoHash = infoHash;
	pieceCount = pieceCount;
	pieceLen = pieceLen;
}

BTProtocal::~BTProtocal()
{

}

bool BTProtocal::MakeHandle(CommBuffer& Buffer)
{	
	int_8 MessageLen = 19;
	string strMsg = "BitTorrent protocol";
	char Reserved[8] = {0};
	std::string PeerID = "BTFish12345678912345";

	Buffer.WriteInt8(MessageLen);
	Buffer.WriteString(strMsg);
	Buffer.WriteString(Reserved,8);
	Buffer.WriteString(m_InfoHash);
	Buffer.WriteString(PeerID);

	return true;
}

bool BTProtocal::MakeBitfit()
{
	return true;
}

bool BTProtocal::MakeInterested(bool bInterested,CommBuffer& Buffer)
{
	int_32 len = 1;
	int_8 id = bInterested ? 2 : 3;

	Buffer.WriteInt32(len);
	Buffer.WriteInt8(id);
	return true;
}

bool BTProtocal::MakeRequest(uint_32 pieceIndex,CommBuffer& Buffer)
{
	int_32 len = 13;
	int_8 id = 6;
	int_32 begin = 0;
	pieceIndex = htonl(pieceIndex);
	len = htonl(len);
	Buffer.WriteInt32(len);
	Buffer.WriteInt8(id);
	Buffer.WriteInt32(pieceIndex);
	Buffer.WriteInt32(begin);
	Buffer.WriteInt32(m_SliceLen);

	return true;
}
