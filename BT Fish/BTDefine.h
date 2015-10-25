#ifndef BTDEFINE_H
#define BTDEFINE_H
#include <string>

namespace BT
{
	struct RequestInfo
	{
		uint_32 index;
		uint_32 begin;
		uint_32	len;
	};


	struct TorrentInfo
	{
		std::string InfoHash;
		uint_64		FileSize;
		uint_32		PieceCount;
		uint_32		PieceSize;
	};
}
#endif