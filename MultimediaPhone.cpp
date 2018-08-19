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

BEGIN_MESSAGE_MAP(CMultimediaPhoneApp, CWinApp)
    //{{AFX_MSG_MAP(CMultimediaPhoneApp)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG
END_MESSAGE_MAP()

VOID WriteLog(CHAR* str) {
    TCHAR s[256] = {
        0
    };

    FILE* fp = 0;

    fopen_s(&fp, log_file, "a+");
    if (fp != NULL) {
        fwrite(str, strlen(str), 1, fp);
        fwrite("\n", strlen("\n"), 1, fp);
        fclose(fp);
    }
}

VOID WriteLog_(char* ptr, int size) {
    FILE* fp = 0;

    fopen_s(&fp, log_dat_file, "w+b");
    if (fp != NULL) {
        fwrite(ptr, sizeof(char), size, fp);
        fclose(fp);
    }
}


CMultimediaPhoneApp::CMultimediaPhoneApp()
: CWinApp() {
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMultimediaPhoneApp object

CMultimediaPhoneApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneApp initialization

BOOL CMultimediaPhoneApp::InitInstance() {
    WSADATA wsa;
    //加载winsock动态链接库
    if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0) {
        return -1;//代表失败
    }

    AfxEnableControlContainer();

#ifdef _DEBUG
#else
    char restartTimer[64] = {
        0
    };
    SYSTEMTIME tm;
    GetLocalTime(&tm);
    sprintf_s(restartTimer, 64, "start:%04d-%02d-%02d %02d:%02d:%02d\r\n", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
    WriteLog(restartTimer);
#endif

    HKEY hkey = 0;
    HKEY hkey1 = 0;
    HKEY hkey2 = 0;
    RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\csplayer0", 0, 0, &hkey);
    RegDeleteKey(hkey, L"PLAY");
    RegFlushKey(HKEY_LOCAL_MACHINE);
    RegCloseKey(hkey);

    RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\csplayer1", 0, 0, &hkey1);
    RegDeleteKey(hkey1, L"PLAY");
    RegFlushKey(HKEY_LOCAL_MACHINE);
    RegCloseKey(hkey1);

    RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\csplayer2", 0, 0, &hkey2);
    RegDeleteKey(hkey2, L"PLAY");
    RegFlushKey(HKEY_LOCAL_MACHINE);
    RegCloseKey(hkey2);

    RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Cyansoft", 0, 0, &hkey);
    RegDeleteKey(hkey, L"CSplayer0");
    RegFlushKey(HKEY_LOCAL_MACHINE);
    RegCloseKey(hkey);

    RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Cyansoft", 0, 0, &hkey1);
    RegDeleteKey(hkey1, L"CSplayer1");
    RegFlushKey(HKEY_LOCAL_MACHINE);
    RegCloseKey(hkey1);

    RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Cyansoft", 0, 0, &hkey2);
    RegDeleteKey(hkey2, L"CSplayer2");
    RegFlushKey(HKEY_LOCAL_MACHINE);
    RegCloseKey(hkey2);

    //copy new tcpmp


    CopyFile(common_dll, win_comm_dll, FALSE);
    CopyFile(mplayer_exe, win_mplayer_exe, FALSE);
    CopyFile(mplayer_exe_1, win_mplayer_exe_1, FALSE);
    CopyFile(mplayer_exe_2, win_mplayer_exe_2, FALSE);
    CopyFile(iesample_exe, win_iesample_exe, FALSE);

    CopyFile(sabtd_cedll_dll, short_sabtd_cedll_dll, FALSE);
    CopyFile(sa_lice_dat, short_sa_lice_dat, FALSE);

    extern void InitPinYin();
    InitPinYin();   

    //  inputDlg_ = new CInputDlg;
    //  inputDlg_->Create(CInputDlg::IDD);
    int nResponse;
    try {
        CMultimediaPhoneDlg dlg;
        m_pMainWnd = &dlg;
        nResponse = dlg.DoModal();
    } catch (...) {
        printf("exit...");
    }
    if (nResponse == IDOK) {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    } else if (nResponse == IDCANCEL) {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    }

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return FALSE;
}

BOOL CMultimediaPhoneApp::PreTranslateMessage(MSG* pMsg) {
    static int oldx = 0;
    static int oldy = 0;
    BOOL ret;

    static int gCount_penDwon = 0;
    static int gCount_penUp = 0;
    if (pMsg->message == WM_LBUTTONUP) {
        if (gCount_penUp != gCount_penDwon) {
            gCount_penUp = gCount_penDwon = 0;
        }
        ::SendMessage(m_pMainWnd->m_hWnd, WM_GEN_EVENT, 0, 0);

        ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->n_bklightcount = 60;
        if (((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pMainDlg->m_mainVideoDlg_->IsWindowVisible()) {
            //视频播放需要先发送笔点消息
            if (((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->ReStoreBackLight()) {
                if (pMsg->pt.x != oldx && pMsg->pt.y != oldy) {
                    ::SendMessage(m_pMainWnd->m_hWnd, WM_GEN_EVENT, 1, 0);
                }
                oldx = pMsg->pt.x;
                oldy = pMsg->pt.y;
                return TRUE;
            }

            ret = CWinApp::PreTranslateMessage(pMsg);

            if (pMsg->pt.x != oldx && pMsg->pt.y != oldy) {
                ::SendMessage(m_pMainWnd->m_hWnd, WM_GEN_EVENT, 1, 0);
            }
            oldx = pMsg->pt.x;
            oldy = pMsg->pt.y;
        } else {
            //屏保 要后发送笔点消息
            if (pMsg->pt.x != oldx && pMsg->pt.y != oldy) {
                //  Dprintf("WM_GEN_EVENT \r\n");
                if (!((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->ReStoreBackLight()) {
                    ::SendMessage(m_pMainWnd->m_hWnd, WM_GEN_EVENT, 1, 0);
                } else {
                    return TRUE;
                }
            }
            oldx = pMsg->pt.x;
            oldy = pMsg->pt.y;
            ret = CWinApp::PreTranslateMessage(pMsg);
        }
    } else if (pMsg->message == WM_LBUTTONDOWN) {
        ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->CancelBalckLightSaveTimer();
        if (((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_nBackLightStatus == 0) { //为黑
            return TRUE;
        }
        ret = CWinApp::PreTranslateMessage(pMsg);
    } else {
        ret = CWinApp::PreTranslateMessage(pMsg);
    }
    return ret;
}
