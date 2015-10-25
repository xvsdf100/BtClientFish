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
    //����Ƿ��ڴ���WantDownList����

	//�����б�
	return true;
}


bool CBTDataManager::AddWantDownDataRange( const DataRange& range )
{
    if(!IsContainRange(range))
    {
        //��ȥ�ظ�,�����ʱ����������ɾ��
       // m_DownedRangeList.push_back(range);
       //���
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
	//��������Ƿ�����
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

//��ʼ��
void CBTDataManager::Init()
{
	//��ʼ����Ӳ����
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
	if(-1 == index)	return 1;	//�����쳣

	//����������ȷ����
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
