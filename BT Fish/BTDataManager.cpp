#include "BTDataManager.h"


CBTDataManager::CBTDataManager()
{

}

CBTDataManager::~CBTDataManager()
{

}

bool CBTDataManager::IsContainRange( const DataRange& range )
{
    //����Ƿ��ڴ���WantDownList����
    //if(IsContainRange(range,m_WantDownRangeList))   return true;

    //if(IsContainRange(range,m_DownedRangeList))     return true;

	return true;
}


bool CBTDataManager::AddWantDownDataRange( const DataRange& range )
{
    if(!IsContainRange(range))
    {
        //��ȥ�ظ�,�����ʱ����������ɾ��
        m_DownedRangeList.push_back(range);
        //���
        return true;
    }

    return false;
}
