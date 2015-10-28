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

    //添加DataRange，根据配置进行校验
    bool AddWantDownDataRange(const DataRange& range);

    bool AddDownedDataRange(const DataRange& range);

	//是否是完整
	bool isComplete();

	void Init();

	//获取需要下载的DataRange
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