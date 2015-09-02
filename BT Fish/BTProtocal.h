#ifndef BT_PTROTOCAL_H
#define BT_PTROTOCAL_H
//构造BT协议
#include "CommBuffer.h"

class BTProtocal
{
public:
	enum BTCmdType
	{
		HAND,
		BITFIT = 5,
		REQUEST,
		PIECE,
	};
public:
	BTProtocal(std::string infoHash,uint_32 pieceCount,uint_32 pieceLen);
	~BTProtocal();

	bool MakeHandle(CommBuffer& Buffer);
	bool MakeBitfit();
	bool MakeInterested(bool bInterested,CommBuffer& Buffer);
	bool MakeRequest(uint_32 pieceIndex,CommBuffer& Buffer);

private:
	std::string		m_InfoHash;
	uint_32			m_PieceCount;
	uint_32			m_PieceLength;
	CommBuffer		m_HaveBitfit;
	const uint_32	m_SliceLen;
};



#endif