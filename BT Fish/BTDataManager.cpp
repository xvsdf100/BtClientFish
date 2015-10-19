#include "BTDataManager.h"


CBTDataManager::CBTDataManager()
{

}

CBTDataManager::~CBTDataManager()
{

}

bool CBTDataManager::IsContainRange( const DataRange& range )
{
    //检测是否在存在WantDownList里面
    //if(IsContainRange(range,m_WantDownRangeList))   return true;

    //if(IsContainRange(range,m_DownedRangeList))     return true;

	return true;
}


bool CBTDataManager::AddWantDownDataRange( const DataRange& range )
{
    if(!IsContainRange(range))
    {
        //不去重复,在完成时候会遍历所有删除
        m_DownedRangeList.push_back(range);
        //添加
        return true;
    }

    return false;
}
