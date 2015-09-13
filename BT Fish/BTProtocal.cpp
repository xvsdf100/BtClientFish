#include "stdafx.h"
#include "BTProtocal.h"
#include <string>
#include "StringUtility.h"

using namespace std;

BTProtocal::BTProtocal(std::string infoHash,uint_32 pieceCount,uint_32 pieceLen):m_SliceLen(1024*16)
{
	m_InfoHash = StringUtility::FromHexString((const uint_8*)infoHash.c_str(),infoHash.length());
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
	int_32 len = htonl(1);
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
	int_32 piecelen = htonl(100);
	pieceIndex = htonl(pieceIndex);
	len = htonl(len);
	Buffer.WriteInt32(len);
	Buffer.WriteInt8(id);
	Buffer.WriteInt32(pieceIndex);
	Buffer.WriteInt32(begin);
	Buffer.WriteInt32(piecelen);

	return true;
}

bool BTProtocal::DecodePiecePacket(CommBuffer* pBuffer,uint_32& dataLength,uint_32& index,uint_32& begin,ByteArray& block)
{
	if(NULL != pBuffer)
	{
		int_32 len = 0;
		int_8	id = 0;
		pBuffer->ReadInt32(len);
		len = ntohl(len);
		if(len > 9)
		{
			dataLength = len - 9;
			pBuffer->ReadInt8(id);
			if(id == 7)
			{
				pBuffer->ReadUInt32(index);
				index = ntohl(index);
				block.resize(dataLength);
				pBuffer->Read(&(*block.begin()),block.size(),block.size());
				return true;
			}

		}
	}
	return false;
}
