// 3GPhone.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CMy3GPhoneApp:
// �йش����ʵ�֣������ 3GPhone.cpp
//

class CMy3GPhoneApp : public CWinApp
{
public:
	CMy3GPhoneApp();
	
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy3GPhoneApp theApp;
