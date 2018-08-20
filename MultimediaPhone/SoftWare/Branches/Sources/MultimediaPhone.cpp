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
g_hHook   =   SetWindowsHookEx(WH_MOUSE,   MouseProc,   _Module.m_hInst,   GetCurrentThreadId());//安装HOOK   
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

BOOL CMultimediaPhoneApp::PreTranslateMessage(MSG* pMsg)
{
	static int oldx = 0;
	static int oldy = 0;
	BOOL ret;
	if(pMsg->message == WM_LBUTTONUP)
	{
		if(((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pMainDlg->m_mainVideoDlg_->IsWindowVisible())
		{
			//视频播放需要先发送笔点消息
			ret = CWinApp::PreTranslateMessage(pMsg);
			if(pMsg->pt.x != oldx && pMsg->pt.y != oldy)
				::SendMessage(m_pMainWnd->m_hWnd, WM_GEN_EVENT, 1, 0);
			oldx = pMsg->pt.x;
			oldy = pMsg->pt.y;
		}
		else
		{
			//屏保 要后发送笔点消息
			if(pMsg->pt.x != oldx && pMsg->pt.y != oldy)
				::SendMessage(m_pMainWnd->m_hWnd, WM_GEN_EVENT, 1, 0);
			oldx = pMsg->pt.x;
			oldy = pMsg->pt.y;
			ret = CWinApp::PreTranslateMessage(pMsg);
		}
	}	
	else
		ret = CWinApp::PreTranslateMessage(pMsg);
	return ret;
}