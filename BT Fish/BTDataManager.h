#ifndef BTMANAGER_H
#define BTMANAGER_H
#include "Utility.h"
#include "BTBitmap.h"

class CBTDataManager
{
public:
	CBTDataManager();
	~CBTDataManager();

    bool IsContainRange(const DataRange& range);

    //添加DataRange，根据配置进行校验
    bool AddWantDownDataRange(const DataRange& range);

    bool AddDownedDataRange(const DataRange& range);

	//是否是完整
	bool isComplete();

private:
	CBTBitmap	    m_LocalBitmap;
    DataRangeList   m_WantDownRangeList;
    DataRangeList   m_DowningRangeList;
    DataRangeList   m_DownedRangeList;
};


#endif