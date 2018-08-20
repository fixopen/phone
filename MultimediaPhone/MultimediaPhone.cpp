
// MultimediaPhone.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "MultimediaPhone.h"
#include "MultimediaPhoneDlg.h"

#include <psapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void WriteLog(char* str)
{
	TCHAR s[256] = {0};
	
    FILE* fp = fopen("\\flashdrv\\log.txt", "a+");
    if (fp != NULL) {
		fwrite(str, strlen(str), 1, fp);
		fwrite("\n", strlen("\n"), 1, fp);
        fclose(fp);
    }
}

// CMultimediaPhoneApp

BEGIN_MESSAGE_MAP(CMultimediaPhoneApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMultimediaPhoneApp 构造

CMultimediaPhoneApp::CMultimediaPhoneApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CMultimediaPhoneApp 对象

CMultimediaPhoneApp theApp;


// CMultimediaPhoneApp 初始化
namespace Control {
    extern void InitPinYin();
}

BOOL CMultimediaPhoneApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
#ifdef _DEBUG
#else
	char restartTimer[64];
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	sprintf(restartTimer, "start:%04d-%02d-%02d %02d:%02d:%02d\r\n", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
	WriteLog(restartTimer);
#endif

#if 0
	//copy new tcpmp
	CopyFile(L"/flashdrv/tcpmp/common.dll", L"/windows/common.dll", FALSE);
	CopyFile(L"/flashdrv/tcpmp/mplayer.exe", L"/windows/mplayer.exe", FALSE);
	CopyFile(L"/flashdrv/tcpmp/mplayer1.exe", L"/windows/mplayer1.exe", FALSE);
	CopyFile(L"/flashdrv/tcpmp/mplayer2.exe", L"/windows/mplayer2.exe", FALSE);
	CopyFile(L"/flashdrv/tcpmp/iesample.exe", L"/windows/iesample.exe", FALSE);
 	CopyFile(L"/flashdrv/res_dat/SABTD_CEDLL.dll", L"/SABTD_CEDLL.dll", FALSE);
	CopyFile(L"/flashdrv/res_dat/sa_license.dat", L"/sa_license.dat", FALSE);
#endif

	InitPinYin();

    //DWORD aProcesses[1024], cbNeeded;
    //if ( ::EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) ) {

    //    // Calculate how many process identifiers were returned.

    //    DWORD cProcesses = cbNeeded / sizeof(DWORD);

    //    // Print the name and process identifier for each process.

    //    for (unsigned int  i = 0; i < cProcesses; ++i ) {
    //        if( aProcesses[i] != 0 ) {
    //            HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
    //                                   PROCESS_VM_READ | PROCESS_ALL_ACCESS,
    //                                   FALSE, aProcesses[i] );
    //            TCHAR filename[512] = {0};
    //            ::GetProcessImageFileName(hProcess, filename, 512);
    //            if (wcsstr(filename, _T("explorer.exe")) != NULL) {
    //                TerminateProcess(hProcess, 0);
    //                break;
    //            }
    //            CloseHandle( hProcess );
    //        }
    //    }
    //}
	CMultimediaPhoneDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

BOOL CMultimediaPhoneApp::PreTranslateMessage(MSG* pMsg)
{
	BOOL ret;

	static int oldx = 0;
	static int oldy = 0;
    static int gCount_penDwon = 0;
	static int gCount_penUp = 0;

	if (pMsg->message == WM_LBUTTONUP)
	{
		if( gCount_penUp != gCount_penDwon)
		{
			gCount_penUp = gCount_penDwon = 0;
		}
		((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->n_bklightcount = 60;
		if (((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pMainDlg->m_mainVideoDlg_->IsWindowVisible())
		{
			//视频播放需要先发送笔点消息
			if (((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->ReStoreBackLight())
			{
				if (pMsg->pt.x != oldx && pMsg->pt.y != oldy)
					::SendMessage(m_pMainWnd->m_hWnd, WM_GEN_EVENT, 1, 0);
				oldx = pMsg->pt.x;
				oldy = pMsg->pt.y;
				return TRUE;
			}

			ret = CWinApp::PreTranslateMessage(pMsg);

			if (pMsg->pt.x != oldx && pMsg->pt.y != oldy)
				::SendMessage(m_pMainWnd->m_hWnd, WM_GEN_EVENT, 1, 0);
			oldx = pMsg->pt.x;
			oldy = pMsg->pt.y;
		}
		else
		{
			//屏保 要后发送笔点消息
			if (pMsg->pt.x != oldx && pMsg->pt.y != oldy)
			{
				if (!((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->ReStoreBackLight())
                {
					::SendMessage(m_pMainWnd->m_hWnd, WM_GEN_EVENT, 1, 0);
                }
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
	else if (pMsg->message == WM_LBUTTONDOWN)
	{
		((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->CancelBalckLightSaveTimer();
		if(((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_nBackLightStatus == 0)  //为黑
        {
			return TRUE;
        }
		ret = CWinApp::PreTranslateMessage(pMsg);
	}
	else
    {
		ret = CWinApp::PreTranslateMessage(pMsg);
    }
	return ret;
}