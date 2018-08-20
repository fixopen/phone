// MultimediaPhone.h : main header file for the MULTIMEDIAPHONE application
//

#if !defined(AFX_MULTIMEDIAPHONE_H__B1504A4F_9CE6_43BB_9108_F1FFFA72E62C__INCLUDED_)
#define AFX_MULTIMEDIAPHONE_H__B1504A4F_9CE6_43BB_9108_F1FFFA72E62C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#define WM_SOFTKEY		(WM_USER+200)

#include "resource.h"		// main symbols
#include "./Control/Inputdlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneApp:
// See MultimediaPhone.cpp for the implementation of this class
//

class CMultimediaPhoneApp : public CWinApp
{
public:
	CInputDlg	*inputDlg_; 
	

public:
	CMultimediaPhoneApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultimediaPhoneApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMultimediaPhoneApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CMultimediaPhoneApp theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIMEDIAPHONE_H__B1504A4F_9CE6_43BB_9108_F1FFFA72E62C__INCLUDED_)
