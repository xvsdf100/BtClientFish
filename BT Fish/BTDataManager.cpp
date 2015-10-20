#include "BTDataManager.h"
#include <algorithm>


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
	return false;
}
