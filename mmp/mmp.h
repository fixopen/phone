// mmp.h : main header file for the MMP application
//

#if !defined(AFX_MMP_H__461B3646_D651_420A_86AC_1EBFA7D01AC7__INCLUDED_)
#define AFX_MMP_H__461B3646_D651_420A_86AC_1EBFA7D01AC7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMmpApp:
// See mmp.cpp for the implementation of this class
//

class CMmpApp : public CWinApp
{
public:
	CMmpApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMmpApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMmpApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MMP_H__461B3646_D651_420A_86AC_1EBFA7D01AC7__INCLUDED_)
