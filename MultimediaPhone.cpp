// MultimediaPhone.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MultimediaPhone.h"
#include "MultimediaPhoneDlg.h"
#include "afxsock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

VOID WriteLog(CHAR* str)
{
	TCHAR s[256] = {0};
	
    FILE* fp = fopen("\\flashdrv\\log.txt", "a+");
    if (fp != NULL) {
		fwrite(str, strlen(str), 1, fp);
		fwrite("\n", strlen("\n"), 1, fp);
        fclose(fp);
    }
}

VOID WriteLog_(char *ptr, int size)
{
	FILE* fp = fopen("\\flashdrv\\log.dat", "w+b");
    if (fp != NULL) {
		fwrite(ptr, sizeof(char), size, fp);
	    fclose(fp);
    }
}

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

// 	if (!AfxSocketInit())
// 	{
// 	//	AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
// 		return FALSE;
// 	}

	WSADATA wsa;
	//加载winsock动态链接库
	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
	{
		return -1;//代表失败
	}

	AfxEnableControlContainer();

// 	for(int i = 0; i < 20; i++)
// 	{
// 	//	unsigned int w = DMemprintf("test 0");
// 		char *pNew = new char[1024*1024];
// 	//	unsigned int w2 = DMemprintf("test 01");
// 		delete []pNew;
// 		unsigned int w3 =  DMemprintf("test 02");
// 
// 		if(w_ != w3)
// 		{
// 			static int gg_ = 0;
// 			gg_++;
// 			Dprintf("Error:%d %x %x \r\n", gg_, w_ , w3);
// 		}
// 	}
// 	unsigned int w__ = DMemprintf("test 0");

	
#ifdef _DEBUG
#else
	char restartTimer[64];
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	sprintf(restartTimer, "start:%04d-%02d-%02d %02d:%02d:%02d\r\n", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
	WriteLog(restartTimer);
#endif

	HKEY   hkey = 0;
	HKEY   hkey1 = 0;
	HKEY   hkey2 = 0;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE,   L"SOFTWARE\\csplayer0",  0, 0, &hkey);
	RegDeleteKey(hkey, L"PLAY");
	RegFlushKey(HKEY_LOCAL_MACHINE);
	RegCloseKey(hkey);
	
	RegOpenKeyEx(HKEY_LOCAL_MACHINE,   L"SOFTWARE\\csplayer1",  0, 0, &hkey1);
	RegDeleteKey(hkey1, L"PLAY");
	RegFlushKey(HKEY_LOCAL_MACHINE);
	RegCloseKey(hkey1);
	
	RegOpenKeyEx(HKEY_LOCAL_MACHINE,   L"SOFTWARE\\csplayer2",  0, 0, &hkey2);
	RegDeleteKey(hkey2, L"PLAY");
	RegFlushKey(HKEY_LOCAL_MACHINE);
	RegCloseKey(hkey2);
	
	RegOpenKeyEx(HKEY_LOCAL_MACHINE,   L"SOFTWARE\\Cyansoft",  0, 0, &hkey);
	RegDeleteKey(hkey, L"CSplayer0");
	RegFlushKey(HKEY_LOCAL_MACHINE);
	RegCloseKey(hkey);
	
	RegOpenKeyEx(HKEY_LOCAL_MACHINE,   L"SOFTWARE\\Cyansoft",  0, 0, &hkey1);
	RegDeleteKey(hkey1, L"CSplayer1");
	RegFlushKey(HKEY_LOCAL_MACHINE);
	RegCloseKey(hkey1);
	
	RegOpenKeyEx(HKEY_LOCAL_MACHINE,   L"SOFTWARE\\Cyansoft",  0, 0, &hkey2);
	RegDeleteKey(hkey2, L"CSplayer2");
	RegFlushKey(HKEY_LOCAL_MACHINE);
	RegCloseKey(hkey2);

	//copy new tcpmp
	CopyFile(L"/flashdrv/tcpmp/common.dll", L"/windows/common.dll", FALSE);
	CopyFile(L"/flashdrv/tcpmp/mplayer.exe", L"/windows/mplayer.exe", FALSE);
	CopyFile(L"/flashdrv/tcpmp/mplayer1.exe", L"/windows/mplayer1.exe", FALSE);
	CopyFile(L"/flashdrv/tcpmp/mplayer2.exe", L"/windows/mplayer2.exe", FALSE);
	CopyFile(L"/flashdrv/tcpmp/iesample.exe", L"/windows/iesample.exe", FALSE);

 	CopyFile(L"/flashdrv/res_dat/SABTD_CEDLL.dll", L"/SABTD_CEDLL.dll", FALSE);
	CopyFile(L"/flashdrv/res_dat/sa_license.dat", L"/sa_license.dat", FALSE);

	extern void InitPinYin();
	InitPinYin();	

//	inputDlg_ = new CInputDlg;
//	inputDlg_->Create(CInputDlg::IDD);
	int nResponse;
	try {
		CMultimediaPhoneDlg dlg;
		m_pMainWnd = &dlg;
		nResponse = dlg.DoModal();
	} catch (...) {
		printf("exit...");
	}
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

	
	static int gCount_penDwon = 0;
	static int gCount_penUp = 0;
	if(pMsg->message == WM_LBUTTONUP)
	{
	//	((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->CancelBalckLightSaveTimer();
	//	Dprintf("Penup %d (%d, %d)\n", gCount_penUp++, pMsg->pt.x, pMsg->pt.y);

		if(gCount_penUp != gCount_penDwon)
		{
		//	Dprintf("pen Error \n");
			gCount_penUp = gCount_penDwon = 0;
		}
// 		if((gCount_penDwon - gCount_penUp) >= 2)
// 		{
// 			Dprintf("Miss up\n");
// 		}
// 		if(gCount_penDwon != (gCount_penUp+1))
// 		{
// 			gCount_penUp = gCount_penDwon; 
// 			return TRUE;
// 		}
// 
// 		Dprintf("Pen up %d\n", gCount_penUp++);
		::SendMessage(m_pMainWnd->m_hWnd, WM_GEN_EVENT, 0, 0);

		((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->n_bklightcount = 60;
		if(((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pMainDlg->m_mainVideoDlg_->IsWindowVisible())
		{
			//视频播放需要先发送笔点消息
			if(((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->ReStoreBackLight())
			{
				if(pMsg->pt.x != oldx && pMsg->pt.y != oldy)
					::SendMessage(m_pMainWnd->m_hWnd, WM_GEN_EVENT, 1, 0);
				oldx = pMsg->pt.x;
				oldy = pMsg->pt.y;
				return TRUE;
			}

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
			{
			//	Dprintf("WM_GEN_EVENT \r\n");
				if(!((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->ReStoreBackLight())
					::SendMessage(m_pMainWnd->m_hWnd, WM_GEN_EVENT, 1, 0);
				else
				{
					return TRUE;
				}
			}
			oldx = pMsg->pt.x;
			oldy = pMsg->pt.y;
			ret = CWinApp::PreTranslateMessage(pMsg);
		}
	}	
	else if(pMsg->message == WM_LBUTTONDOWN)
	{
		((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->CancelBalckLightSaveTimer();
	//	Dprintf("Pen down %d (%d, %d)\n", gCount_penDwon++, pMsg->pt.x, pMsg->pt.y);
		if(((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_nBackLightStatus == 0)  //为黑
			return TRUE;
		ret = CWinApp::PreTranslateMessage(pMsg);
	}
	else if (pMsg->message == WM_KEYDOWN)
	{
		if ( pMsg->wParam == VK_F1)//音量增加 +
		{
			((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->AddAudio(true);
		}
		else if (pMsg->wParam == VK_F2)//音量减 -
		{
			((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->AddAudio(false);

		}

	}
	else
		ret = CWinApp::PreTranslateMessage(pMsg);
	return ret;
	
// 	static int nU = 0;
// 	static int nD = 0;
// 	if(pMsg->message == WM_LBUTTONDOWN)
// 	{
// 		nD++;
// //		printf("Pen Down %d\r\n", nD);
// 	}	
// 	else if(pMsg->message == WM_LBUTTONUP)
// 	{
// 		nU++;
// //		printf("Pen Up %d\r\n", nU);
// 		if(nU != nD)
// 		{
// //			printf("Pen Error %d %d\r\n", nU, nD);
// 			nU = nD = 0;
// 		}
// 	}
// 	return CWinApp::PreTranslateMessage(pMsg);
}
