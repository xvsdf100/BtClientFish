#ifndef UTILITY_H
#define UTILITY_H


#include "stdafx.h"
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

#define WM_USER_LOG WM_USER + 5000
#define LOG_INFO AfxGetMainWnd(X)->PostMessage(WM_USER_LOG,new CString(X));

#endif