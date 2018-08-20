// MultimediaPhone.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MultimediaPhone.h"
#include "MultimediaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
g_hHook   =   SetWindowsHookEx(WH_MOUSE,   MouseProc,   _Module.m_hInst,   GetCurrentThreadId());//°²×°HOOK   
LRESULT CALLBACK MouseProc(   int   nCode,WPARAM   wParam,LPARAM   lParam)   
{   
	if(nCode < 0)   
		return CallNextHookEx(g_hHook, nCode, wParam, lParam);   

	MOUSEHOOKSTRUCT*   lpMsg   =   (MOUSEHOOKSTRUCT*)lParam;   
	if(wParam   ==   WM_LBUTTONDOWN)   
	{   
		  printf("WM_LBUTTON DOWN\n");
	}
	if(wParam   ==   WM_LBUTTONUP)   
	{   
		  printf("WM_LBUTTON UP\n");
	}
}
*/
/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneApp

BEGIN_MESSAGE_MAP(CMultimediaPhoneApp, CWinApp)
	//{{AFX_MSG_MAP(CMultimediaPhoneApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneApp construction

CMultimediaPhoneApp::CMultimediaPhoneApp()
	: CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMultimediaPhoneApp object

CMultimediaPhoneApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneApp initialization

BOOL CMultimediaPhoneApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	//HINSTANCE richEdit=::LoadLibrary(_T("riched20.dll"));
	//if (!richEdit)
	//	AfxMessageBox(_T("Unable to load RichEdit2.0"));
	//AfxInitRichEdit();
	//CRichEditCtrl edit;

	//copy new tcpmp
	CopyFile(L"/flashdrv/tcpmp/common.dll", L"/windows/common.dll", FALSE);
	CopyFile(L"/flashdrv/tcpmp/mplayer.exe", L"/windows/mplayer.exe", FALSE);

	extern void InitPinYin();
	InitPinYin();

	inputDlg_ = new CInputDlg;
	inputDlg_->Create(CInputDlg::IDD);

	CMultimediaPhoneDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
