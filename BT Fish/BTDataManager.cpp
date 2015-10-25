#include "BTDataManager.h"
#include <algorithm>
#include <assert.h>

CBTDataManager::CBTDataManager()
{

}

CBTDataManager::~CBTDataManager()
{

}

bool CBTDataManager::IsContainRange( const DataRange& range )
{
    //检测是否在存在WantDownList里面

	//查找列表
	return true;
}


bool CBTDataManager::AddWantDownDataRange( const DataRange& range )
{
    if(!IsContainRange(range))
    {
        //不去重复,在完成时候会遍历所有删除
       // m_DownedRangeList.push_back(range);
       //添加
		m_WantDownRangeList.push_back(range);
        return true;
    }

    return false;
}

bool CBTDataManager::AddDownedDataRange( const DataRange& range )
{
	if(!IsContainRange(range))
	{
		m_DownedRangeList.push_back(range);
		return true;
	}
	return false;
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

	return true;
}

//初始化
void CBTDataManager::Init()
{
	//初始化，硬编码
	m_TorrentInfo.FileSize = 290;
	m_TorrentInfo.InfoHash = "";
	m_TorrentInfo.PieceCount = 1;
	m_TorrentInfo.PieceSize = 16384;

	m_DownloadStatusArray.resize(m_TorrentInfo.PieceCount);
	for(uint_32 i = 0; i < m_TorrentInfo.PieceCount; i++)
	{
		m_DownloadStatusArray[i] = NONE;
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

	int_32 index = GetIndexFromRange(range);
	if(-1 == index)	return 1;	//数据异常

	//检验数据正确的性
	m_DownloadStatusArray[index] = DOWNED;
	
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
