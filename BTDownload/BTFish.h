
// BTFish.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBTFishApp:
// �йش����ʵ�֣������ BT Fish.cpp
//

class CBTFishApp : public CWinAppEx
{
public:
	CBTFishApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBTFishApp theApp;