#include "BTDataManager.h"
#include <algorithm>
#include <assert.h>

CBTDataManager::CBTDataManager()
{
	m_hFile = INVALID_HANDLE_VALUE;
}

CBTDataManager::~CBTDataManager()
{

}




bool CBTDataManager::isComplete()
{
	//检测所有是否完整
	bool isOk = true;
	for(uint_32 i = 0; i < m_DownloadStatusArray.size(); i++)
	{
		if(m_DownloadStatusArray[i] != DOWNED)
		{
			isOk = false;
			break;
		}
	}

	return isOk;
}

//初始化
void CBTDataManager::Init()
{
	//初始化，硬编码
	m_TorrentInfo.FileSize = 48201;
	m_TorrentInfo.InfoHash = "";
	m_TorrentInfo.PieceCount = 2;
	m_TorrentInfo.PieceSize = 16384*2;

	m_DownloadStatusArray.resize(m_TorrentInfo.PieceCount);
	for(uint_32 i = 0; i < m_TorrentInfo.PieceCount; i++)
	{
		m_DownloadStatusArray[i] = NONE;
	}

	HANDLE hFile = CreateFile(L"testBittorrent.txt",GENERIC_WRITE,NULL,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(INVALID_HANDLE_VALUE != hFile)	
	{
		m_hFile = hFile;
	}

}

bool CBTDataManager::GetNoneDownDataRange(DataRange& range,bool bReset)
{
	for(uint_32 i = 0; i < m_TorrentInfo.PieceCount; i++)
	{
		if(m_DownloadStatusArray[i] == NONE)
		{
			range.pos = i*m_TorrentInfo.PieceSize;
			range.len = (uint_32)(i == m_TorrentInfo.PieceCount - 1) ? m_TorrentInfo.PieceSize - (m_TorrentInfo.PieceCount*m_TorrentInfo.PieceSize - m_TorrentInfo.FileSize) : m_TorrentInfo.PieceSize;
			range.index = i;
			if(bReset)
			m_DownloadStatusArray[i] = DOWNING;

			return true;
		}
	}

	return false;
}


int CBTDataManager::AddDownedDataRange( const DataRange& range,uint_8* buffer )
{
	assert(NULL != buffer);
	assert(0 != range.len);


	if(INVALID_HANDLE_VALUE == m_hFile)
	{
		return 2;
	}

	DWORD dwWrite;
	if(!WriteFile(m_hFile,buffer,range.len,&dwWrite,NULL))
	{
		return 3;
	}
	
	//检验数据正确的性
	m_DownloadStatusArray[range.index] = DOWNED;
	
	return 0;
}

int_32 CBTDataManager::GetIndexFromRange( const DataRange& range )
{
	if(range.pos%m_TorrentInfo.PieceSize)
	{
		return (int_32)(range.pos/m_TorrentInfo.PieceSize);
	}

	return -1;
}
