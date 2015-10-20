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

    //���DataRange���������ý���У��
    bool AddWantDownDataRange(const DataRange& range);

    bool AddDownedDataRange(const DataRange& range);

	//�Ƿ�������
	bool isComplete();

private:
	CBTBitmap	    m_LocalBitmap;
    DataRangeList   m_WantDownRangeList;
    DataRangeList   m_DowningRangeList;
    DataRangeList   m_DownedRangeList;
};


#endif