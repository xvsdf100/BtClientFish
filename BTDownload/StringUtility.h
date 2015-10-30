#ifndef STRING_UTILITY_H
#define STRING_UTILITY_H
#include "DataType.h"
#include <string>
class StringUtility
{
public:
	static std::string ToHexString( const uint_8* buf, uint_32 len );
	static std::string FromHexString(const uint_8* buf, uint_32 len);
};
#endif