#include "StdAfx.h"
#include "BTBitmap.h"
#include <algorithm>

CBTBitmap::CBTBitmap(void)
{
}

CBTBitmap::~CBTBitmap(void)
{
}

void CBTBitmap::Add(char* bitmap,uint_32 len)
{
	for(uint_32 i = 0; i < len; i++)
	{
		byte ch = bitmap[i];
		for(uint_32 n = 0; n < 8; n++ )
		{
			uint_32 index = i*8 + n;
			byte lastbit = ch >> n & 0x1;
			if(lastbit&0x1)
			{
				Add(index);
			}
		}
	}
}

void CBTBitmap::Add(uint_32 index)
{
	if(m_Bitmap.end() == std::find(m_Bitmap.begin(),m_Bitmap.end(),index))
	{
		m_Bitmap.push_back(index);
	}
}

bool CBTBitmap::isHave( uint_32 index )
{
    if(m_Bitmap.end() == std::find(m_Bitmap.begin(),m_Bitmap.end(),index))
    {
        return false;
    }

    return true;
}
