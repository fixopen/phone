// RecordPhone.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CRecordPhoneApp:
// See RecordPhone.cpp for the implementation of this class
//

class CRecordPhoneApp : public CWinApp {
public:
	CRecordPhoneApp();
	
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

#define UM_CONTACT_UPDATE_FINALLY (WM_USER + 100)
#define UM_CATEGORY_UPDATE_FINALLY (UM_CONTACT_UPDATE_FINALLY + 1)
#define UM_CATEGORY_DELETE_FINALLY (UM_CATEGORY_UPDATE_FINALLY + 1)
#define UM_CALLINFO_UPDATE_FINALLY (UM_CATEGORY_DELETE_FINALLY + 1)
#define UM_SHOW_TELEPHONE (UM_CALLINFO_UPDATE_FINALLY + 1)
#define UM_CONFIRM (UM_SHOW_TELEPHONE + 1)
#define UM_CHANGE_CONTACT_TYPE (UM_CONFIRM + 1)

extern CRecordPhoneApp theApp;
