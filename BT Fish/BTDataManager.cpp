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
	return false;
}
