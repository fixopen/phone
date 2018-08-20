
// MultimediaPhone.cpp : ����Ӧ�ó��������Ϊ��
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


// CMultimediaPhoneApp ����

CMultimediaPhoneApp::CMultimediaPhoneApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMultimediaPhoneApp ����

CMultimediaPhoneApp theApp;


// CMultimediaPhoneApp ��ʼ��
namespace Control {
    extern void InitPinYin();
}

BOOL CMultimediaPhoneApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
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
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
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
			//��Ƶ������Ҫ�ȷ��ͱʵ���Ϣ
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
			//���� Ҫ���ͱʵ���Ϣ
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
		if(((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_nBackLightStatus == 0)  //Ϊ��
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