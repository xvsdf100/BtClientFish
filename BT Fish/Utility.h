#ifndef UTILITY_H
#define UTILITY_H
#include "DataType.h"


#define SAFEDEL(x) if(NULL == x)    {delete x; x = NULL;}

typedef	struct tagDataRange
{
	int_64	pos;
	int_32	len;
}DataRange;


#endif