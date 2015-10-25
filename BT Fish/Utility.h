#ifndef UTILITY_H
#define UTILITY_H
#include "DataType.h"
#include <vector>

#define SAFEDEL(x) if(NULL == x)    {delete x; x = NULL;}

typedef	struct tagDataRange
{
	int_64	pos;
	uint_32	len;
	uint_32 index;
}DataRange;

typedef std::vector<DataRange>  DataRangeList;

//�Ƿ����
bool IsContainRange(const DataRange& range,const DataRangeList& rangeList);

//ɾ��
bool RemoveDataRange(const DataRange& range,DataRangeList& rangeList);

//��ȡ����
bool GetInterSectionRange(const DataRange& range1,const DataRange& range2,DataRange& intersecRange);

#endif