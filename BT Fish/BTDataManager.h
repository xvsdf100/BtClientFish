#ifndef BTMANAGER_H
#define BTMANAGER_H
#include "Utility.h"
#include "BTDefine.h"
#include "BTBitmap.h"
#include <Windows.h>

class CBTDataManager
{
public:
	enum DataRangeDownStatus
	{
		NONE,
		DOWNING,
		DOWNED,
	};

	typedef std::vector<DataRangeDownStatus>	DownloadStatusArray;
public:
	CBTDataManager();
	~CBTDataManager();

    bool IsContainRange(const DataRange& range);

    //���DataRange���������ý���У��
    bool AddWantDownDataRange(const DataRange& range);

    bool AddDownedDataRange(const DataRange& range);

	//�Ƿ�������
	bool isComplete();

	void Init();

	//��ȡ��Ҫ���ص�DataRange
	bool GetNoneDownDataRange(DataRange& range,bool bReset = true);

	int	AddDownedDataRange(const DataRange& range,uint_8* buffer);

private:
	int_32	GetIndexFromRange(const DataRange& range);

private:
	CBTBitmap	    m_LocalBitmap;
    DataRangeList   m_WantDownRangeList;
    DataRangeList   m_DowningRangeList;
    DataRangeList   m_DownedRangeList;
	BT::TorrentInfo	m_TorrentInfo;
	DownloadStatusArray	m_DownloadStatusArray;
	HANDLE			m_hFile;
};


#endif