
// MultimediaPhone.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// CMultimediaPhoneApp:
// �йش����ʵ�֣������ MultimediaPhone.cpp
//

class CMultimediaPhoneApp : public CWinAppEx
{
public:
	CMultimediaPhoneApp();

// ��д
	public:
	virtual BOOL InitInstance();
    BOOL PreTranslateMessage(MSG* pMsg);
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMultimediaPhoneApp theApp;