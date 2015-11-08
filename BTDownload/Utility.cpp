#include "Utility.h"

bool IsContainRange( const DataRange& range,const DataRangeList& rangeList )
{
    bool IsContain = false;
    DataRangeList::const_iterator it;
    for(it = rangeList.begin(); it != rangeList.end(); it++)
    {
        if(range.pos >= it->pos && range.len <= it->len)
        {
            IsContain = true;
            break;
        }
    }

    return IsContain;
}

bool RemoveDataRange( const DataRange& range,DataRangeList& rangeList )
{
    DataRangeList::iterator it;
    for(it = rangeList.begin(); it != rangeList.end(); it++)
    {
        
    }

	return true;
}

bool GetInterSectionRange( const DataRange& range1,const DataRange& range2,DataRange& intersecRange )
{
    uint_64 PosEnd1 = range1.pos + range1.len;
    uint_64 PosEnd2 = range2.pos + range2.len;
    if(range1.pos < range2.pos)
    {
        if(PosEnd1 > range2.pos)
        {
            intersecRange.pos = range2.pos ;
            intersecRange.len = (range1.len - (range2.pos - range1.pos)) >= range2.len ? range2.len : range1.len;
            return true;
        }
    }
    else
    {
        // >=
        if(range1.pos > PosEnd2)
        {
            intersecRange.pos = range1.pos;
            intersecRange.len = PosEnd1 > PosEnd2;
        }
    }

    return false;
}

void Log_Debug(const char *format, ...)
{
	std::string strMsg;
	strMsg.resize(1024*8);
	
	va_list arglist;
	va_start(arglist, format);
	_vsprintf_s_l((char*)strMsg.c_str(), strMsg.capacity(), format, NULL, arglist);

	strMsg += "\n";
	OutputDebugStringA(strMsg.c_str());
}
