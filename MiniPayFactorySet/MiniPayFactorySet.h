
// MiniPayFactorySet.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMiniPayFactorySetApp:
// �йش����ʵ�֣������ MiniPayFactorySet.cpp
//


class CMiniPayFactorySetApp : public CWinApp
{
public:
	CMiniPayFactorySetApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMiniPayFactorySetApp theApp;