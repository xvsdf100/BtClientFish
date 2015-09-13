#ifndef BT_PTROTOCAL_H
#define BT_PTROTOCAL_H
//构造BT协议
#include "CommBuffer.h"

class BTProtocal
{
public:
	enum BTCmdType
	{
		CHOKE = 0,
		UNCHOKE = 1,
		INTERESTED = 2,
		NOTINTERESTED = 3,
		HAVE = 4,
		BITFIT = 5,
		REQUEST = 6,
		PIECE = 7,
		HAND,
	};
public:
	BTProtocal(std::string infoHash,uint_32 pieceCount,uint_32 pieceLen);
	~BTProtocal();

	bool MakeHandle(CommBuffer& Buffer);
	bool MakeBitfit();
	bool MakeInterested(bool bInterested,CommBuffer& Buffer);
	bool MakeRequest(uint_32 pieceIndex,CommBuffer& Buffer);

	//解析数据包
	bool DecodePiecePacket(CommBuffer* pBuffer,uint_32& dataLength,uint_32& index,uint_32& begin,ByteArray& block);

private:
	std::string		m_InfoHash;
	uint_32			m_PieceCount;
	uint_32			m_PieceLength;
	CommBuffer		m_HaveBitfit;
	const uint_32	m_SliceLen;
};



#endif