#pragma once
//˵����BT λͼ������Ϣ

#include <vector>
#include "Utility.h"
class CBTBitmap
{
public:
	typedef std::vector<uint_32>	BitmapArray;

public:
	CBTBitmap(void);
	~CBTBitmap(void);

	//��Ӷ����Ƶ�λͼ
	void Add(char* bitmap,uint_32 len);

	//����λͼ����
	void Add(uint_32 index);

	uint_32 GetCount()	{return m_Bitmap.size();}

	void Clear()	{m_Bitmap.clear();}

    bool isHave(uint_32 index);

private:
	BitmapArray	m_Bitmap;
};
