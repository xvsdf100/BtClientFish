#include "StringUtility.h"

std::string StringUtility::ToHexString(const uint_8* buf, uint_32 len)
{
	std::string temp;
	temp.reserve(len * 2);

	unsigned char hb = 0;  
	unsigned char lb = 0;  

	for(uint_32 i = 0; i < len; i++)  
	{  
		hb =(buf[i] & 0xF0) >> 4;  

		if(hb >= 0 && hb <= 9)  
			hb += 0x30;  
		else if(hb >= 10 && hb <= 15)  
			hb = hb - 10 + 'A';
		else 
			return "";

		lb = buf[i] & 0x0F;  
		if(lb >= 0 && lb <= 9)  
			lb += 0x30;  
		else if(lb >= 10 && lb <= 15)  
			lb = lb - 10 + 'A';
		else  
			return "";  

		temp.push_back(hb);
		temp.push_back(lb);
	}  

	return temp;
}

std::string StringUtility::FromHexString(const uint_8* buf, uint_32 len)
{
	if(len % 2 != 0) return "";

	std::string temp;
	temp.reserve(len / 2);

	unsigned char hb = 0;  
	unsigned char lb = 0;

	for(uint_32 i = 0; i < len; ++i)  
	{  
		hb = buf[i];
		if(hb >= 'a' && hb <= 'f')
			hb = hb - 'a' + 10;
		else if(hb >= 'A' && hb <= 'F')  
			hb = hb - 'A' + 10;  
		else if(hb >= '0' && hb <= '9' )  
			hb = hb - '0';  
		else  
			return "";  

		i++;
		lb = buf[i];  

		if(lb >= 'a' && lb <= 'f')
			lb = lb - 'a' + 10;
		else if(lb >= 'A' && lb <= 'F')  
			lb = lb - 'A' + 10;  
		else if(lb >= '0' && lb <= '9')  
			lb = lb - '0';  
		else  
			return "";  

		temp.push_back((hb << 4)|(lb));
	}  

	return temp;  
}
