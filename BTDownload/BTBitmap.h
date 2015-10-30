#pragma once
//说明：BT 位图保存信息

#include <vector>
#include "Utility.h"
class CBTBitmap
{
public:
	typedef std::vector<uint_32>	BitmapArray;

public:
	CBTBitmap(void);
	~CBTBitmap(void);

	//添加二进制的位图
	void Add(char* bitmap,uint_32 len);

	//增加位图索引
	void Add(uint_32 index);

	uint_32 GetCount()	{return m_Bitmap.size();}

	void Clear()	{m_Bitmap.clear();}

    bool isHave(uint_32 index);

private:
	BitmapArray	m_Bitmap;
};
