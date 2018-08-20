// WebDialog.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "..\multimediaphoneDlg.h"
#include "WebDialog.h"
#include "../Data/SkinStyle.h"
#include "../Data/LanguageResource.h"
#include "sip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebDialog dialog
#define WM_BROW_FORWARD	WM_USER+100
#define WM_BROW_BACK	WM_USER+101
#define WM_BROW_FRESH	WM_USER+102
#define WM_BROW_STOP	WM_USER+103
#define WM_BROW_URL		WM_USER+104

TCHAR tUrl[256] = {0};
COPYDATASTRUCT gdata;

extern BOOL DetectDIR(TCHAR *sDir);
CWebDialog::CWebDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CWebDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWebDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
/*	HWND hwnd = ::FindWindow(TEXT("iExplore"), NULL);
	::MoveWindow(hwnd, 50, 50, 600, 400, TRUE);
	*/
	m_bPlaying = FALSE;
	m_hIEWnd = NULL;
	url_ = "";

}


void CWebDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWebDialog, CDialog)
	//{{AFX_MSG_MAP(CWebDialog)
//	ON_BN_CLICKED(IDC_BUTTON_NOTE_CLOSE, OnButtonOk)
//	ON_BN_CLICKED(IDC_BUTTON_SOUND_CLOSE, OnButtonClose)
//	ON_MESSAGE(WM_COMMBOX_CLICKED, OnUrlSelect)
//	ON_NOTIFY(NM_CLICK, IDC_LIST_MP3LIST, OnClickPlayList)
//	ON_MESSAGE(WM_LISTCTRL_CLICK, OnListCltrlClick)
	ON_WM_TIMER()
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_EDITENTER, OnEnter)
	ON_MESSAGE(WM_KILLWEBSHOW, OnKillWEBShow)
	ON_MESSAGE(WM_COPYDATA, OnSetLink)
	ON_MESSAGE(WM_COMMBOX_CLICKED, OnSelectURL)
	ON_MESSAGE(WM_USER+100, OnIEClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CWebDialog::OnIEClick(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->ReStoreBackLight();
}

void CWebDialog::OnSelectURL(WPARAM w, LPARAM l)
{
	if(w == IDC_COMBOBOX_SETTING_RINGTIMES)
	{
		CString s;
		m_cmbURL.GetWindowText(s);
		m_edit.SetWindowText(s);
		if(url_ != s)
		{
			SendtoWebMessage(WM_BROW_STOP, 0, 0);
			SetURL(s);
			m_MJPGList.Invalidate();
		}
	}
}

void CWebDialog::OnEnter(WPARAM w, LPARAM l)
{
	SendtoWebMessage(WM_BROW_STOP, 0, 0);

	CString s;
	m_edit.GetWindowText(s);
	SetURL(s);
}

void CWebDialog::OnClickMJPG(WPARAM w, LPARAM l)
{
//	if(!browser_)
//		return;
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->CancelBalckLightSaveTimer();
 	switch(w)
 	{
	case 1:				//上一页
		//browser_->GoBack();
		SendtoWebMessage(WM_BROW_BACK, 0, 0);
		m_MJPGList.Invalidate();
		break;
	case 2:				//下一页
		SendtoWebMessage(WM_BROW_FORWARD, 0, 0);
		m_MJPGList.Invalidate();
		//browser_->GoForward();
		break;
	case 3:				//停止
		SendtoWebMessage(WM_BROW_STOP, 0, 0);
		m_MJPGList.Invalidate();
		//browser_->Stop();
		break;
	case 4:				//刷新
		SendtoWebMessage(WM_BROW_FRESH, 0, 0);
		m_MJPGList.Invalidate();
		//browser_->Refresh();
		break;
	case 5:				//主页
		{
			CString s = "www.sohu.com";
			if(strlen(m_sUrlList[0]) > 0)
				s = m_sUrlList[0];
			SetURL(s);   //http://www.wuhan.net.cn/
			m_MJPGList.Invalidate();
		}
		break;
	case 6:				//查找
		{
			CRect rt;
			GetWindowRect(&rt);
			if(rt.Height() > 420)
			{
				MoveWindow(CRect(0, 0, 800, 420));
				m_IERect = CRect(13, 55, 786, 419);  //CRect(7, 54, 7+776, 54+356+10);   
				m_MJPGList.Invalidate();
			//	Invalidate();
			}
			else
			{
				MoveWindow(CRect(0, 0, 800, 480));
				m_IERect = CRect(13, 55, 786, 474);
				m_MJPGList.Invalidate();
			//	Invalidate();
			}
		}
		//SetURL(L"www.baidu.com");
		break;
	case 7:				//关闭
		{
			SipShowIM(SIPF_OFF);
			KillTimer(1);
			ExitWeb();
			ShowWindow(SW_HIDE);
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SetStatusAll(FALSE);
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->SetScreenSaveTimer();

			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->IsUnConnectNet();
		}		
		break;
	case 8:				//确定
		{
			SendtoWebMessage(WM_BROW_STOP, 0, 0);
			CString s;
			m_edit.GetWindowText(s);
			SetURL(s);
		}
		break;
	case 9:
		{
			CString s;
			m_edit.GetWindowText(s);
			AddHistory(s);
		}
		break;
	case 10:
		break;
	default:
 		break;
 	}
}
 
// void CWebDialog::OnListCltrlClick(WPARAM w, LPARAM l)
// {
// 	LRESULT ret;
// 	if(w == IDC_LIST_MP3LIST)
// 		OnClickPlayList(NULL, &ret);
// }
// 
// /////////////////////////////////////////////////////////////////////////////
// // CWebDialog message handlers
// void CWebDialog::OnUrlSelect(WPARAM w, LPARAM l)
// {
// // 	CString s;
// // 	m_cmbURL.GetWindowText(s);
// // 	m_edit.SetWindowText(s);
// }

// void CWebDialog::OnButtonClose()
// {
// 	//ShowWindow(SW_HIDE);
// }

// void CWebDialog::OnButtonOk()
// {
// 	CString url;
// 	m_edit.GetWindowText(url);
// 
// 	PROCESS_INFORMATION processInfo;
// 	CString exeFilename;
// 	if(url.Find(L".doc") != -1 || url.Find(L".DOC") != -1||url.Find(L".text1") != -1 || url.Find(L".text1") != -1)
// 	{
// 		exeFilename = "\\windows\\docviewer.exe";
// 	}
// 	else if(url.Find(L".ppt") != -1 || url.Find(L".PPT") != -1)
// 	{
// 		exeFilename = "\\windows\\presviewer.exe";
// 	}
// 
// 	if (!CreateProcess(exeFilename, url, NULL, NULL, NULL, CREATE_NEW_CONSOLE, NULL, NULL, NULL, &processInfo))
// 	{
// 		//Dprintf("不能打开文件!\n");
// 		MessageBox(_T("启动程序失败"), _T("程序启动失败"), 0);
// 	}
// 	CloseHandle(processInfo.hThread);
// 	CloseHandle(processInfo.hProcess);
// 	ShowWindow(SW_HIDE);
// }
void CWebDialog::AddHistory(CString s)
{
	if(s != "")
	{
		for(int j = 0; j < 10; j++)
		{
			if(strlen(m_sUrlList[j]) > 0)
			{
				if(memcmp(m_sUrlList[j], Util::StringOp::FromCString(s).c_str(), strlen(m_sUrlList[j])) == 0)
				{
					Dprintf("UrlList is isExit\r\n");
					return;
				}
			}
		}

		int i = m_nUrlCount-1;
		for(; i >= 0; i--)
		{
			if(i < 9 )
				strcpy(m_sUrlList[i+1], m_sUrlList[i]);
		}
		strcpy(m_sUrlList[0], Util::StringOp::FromCString(s).c_str());
	}
	else
		return;
	FILE *file = fopen("\\flashdrv\\my_web\\weblist.txt", "w+b");
	if(file)
	{
		for(int i = 0; i < 10; i++)
		{
			if(strlen(m_sUrlList[i]) > 0)
			{
				fwrite(m_sUrlList[i], sizeof(char), strlen(m_sUrlList[i]), file);
				fwrite("\r\n", sizeof(char), 2, file);
			}
		}
		fclose(file);
	}
	SetHistoryList();
}

void CWebDialog::SetHistoryList()
{
	m_nUrlCount = 0;
	int nCount = 0;
	m_cmbURL.ResetContent();
	FILE *file = fopen("\\flashdrv\\my_web\\weblist.txt", "r+b");
	if(file)
	{
		while(!feof(file))
		{
			char txt[128] = {0};
			fgets(txt, 127, file);
			CString s = txt;
			s.TrimRight(L" ");
			s.TrimLeft(L" ");
			s.TrimRight(L"\n");
			s.TrimRight(L"\r");
			s.TrimRight(L" ");
			s.TrimLeft(L" ");
			if(s != "")
			{
				if(nCount < 10)
				{
					strcpy(m_sUrlList[nCount], Util::StringOp::FromCString(s).c_str());
					m_cmbURL.AddString(s);
					nCount++;
				}
			}
		}
		fclose(file);
		m_nUrlCount = nCount;
		for(int i = nCount; i < 10; i++)
		{
			memset(m_sUrlList[i], 0, 128);
		}
	}
	else
	{
		m_cmbURL.AddString(L"www.sina.com.cn");
		m_cmbURL.AddString(L"www.sohu.com");
		m_cmbURL.AddString(L"www.baidu.com");
		m_cmbURL.AddString(L"www.126.com");
		m_cmbURL.AddString(L"www.caijing.com.cn");
		m_cmbURL.AddString(L"www.gutx.com");
		m_cmbURL.AddString(L"http://sports.china.com");
		m_cmbURL.AddString(L"http://sports.people.com.cn");
	}
}


BOOL CWebDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	MoveWindow(CRect(0, 0, 800, 480), FALSE);

	m_edit.Create(WS_CHILD|WS_VISIBLE, CRect(346, 1, 702-57, 42), this, 0xFFFF, 24);
	m_edit.SetWindowText(L"");

	m_cmbURL.Create(WS_CHILD|WS_VISIBLE, CRect(346, 1, 702, 400),  this, IDC_COMBOBOX_SETTING_RINGTIMES);
	memset(m_sUrlList, 0, 10*128);
	SetHistoryList();

/*
	m_lstPlayList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(8, 48, 415, 197), this, IDC_LIST_MP3LIST, TRUE, 1);
	m_lstPlayList.SetListColor(Data::g_listctrlBackRGB1[Data::g_skinstyle], Data::g_listctrlBackRGB2[Data::g_skinstyle]);
	m_lstPlayList.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 387);

	m_pImageList = new CImageList();
	m_pImageList->Create(32, 32, ILC_COLOR32|ILC_MASK, 5, 5);   
    //HICON hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_CHECK0));   
	//m_pImageList->Add(hIcon);  
    //hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_CHECK1));   
	//m_pImageList->Add(hIcon);   
	CBitmap bm;
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[1][Data::g_skinstyle]);
	m_pImageList->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[0][Data::g_skinstyle]);
	m_pImageList->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[2][Data::g_skinstyle]);
	m_pImageList->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[3][Data::g_skinstyle]);
	m_pImageList->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[4][Data::g_skinstyle]);
	m_pImageList->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	m_lstPlayList.SetImageList(m_pImageList, LVSIL_SMALL);

	SetPlayList(_T("\\storagecard\\"), 1);
	m_lstPlayList.SetScrollRagle(FALSE);
	

	m_btn.Create(L"确定", Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 28, 471, 48), this, IDC_BUTTON_NOTE_CLOSE);
	m_btn.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	m_btnClose.Create(L"关闭", Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 177, 471, 197), this, IDC_BUTTON_SOUND_CLOSE);
	m_btnClose.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	m_sticBack.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(8, 27, 415, 197), this);
	m_sticBack.SetAlign(DT_CENTER);
	m_sticBack.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));
	
	m_sticPanel.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(416, 26, 472, 198), this);
	m_sticPanel.SetColor(RGB(0, 0, 0), Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);
	
	//	runstocks();
	m_sticBackground.Create(CRect(0, 0, 480, 204), this, 2);
	
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	ts[0].txtRect = CRect(10, 4, 160, 24);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT;
	char *title = "浏览文档";
	memcpy(ts[0].sTxt, title, strlen(title));
	
 	m_sticBackground.SetTextStruct(ts, 1, FALSE);

  */

	// TODO: Add extra initialization here

// 	//web
// 	if (!browserContainer_.CreateControl(CLSID_WebBrowser, NULL, WS_VISIBLE | WS_CHILD, CRect(7, 54, 7+786, 54+356), this, AFX_IDW_PANE_FIRST))
// 	{
// 		DestroyWindow();
// 		Dprintf("create web browser error\n");
// 		return FALSE;
// 	}
// 	browser_ = NULL;

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 480), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\web.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 480));

	m_IERect = CRect(13, 55, 786, 474);
	
	/*
	///得到指针
	LPUNKNOWN lpUnk = browserContainer_.GetControlUnknown();
	HRESULT hr = lpUnk->QueryInterface(IID_IWebBrowser2, (void**)&browser_);
	if (!SUCCEEDED(hr))
	{
		Dprintf("query IWebBrowser2 interface error\n");
		browser_ = NULL;
		browserContainer_.DestroyWindow();
		DestroyWindow();
		return FALSE;
	}
	*/

	//SetURL(url_);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void IEThreadPrc(void)
{
// 	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
// 	
// 	if(main->m_pMainDlg->m_pWebDialog->browser_)
// 	{
// 		main->m_pMainDlg->m_pWebDialog->browser_->Release();
// 		main->m_pMainDlg->m_pWebDialog->browser_ = NULL;
// 	}
// 
// 	///得到指针
// 	LPUNKNOWN lpUnk = main->m_pMainDlg->m_pWebDialog->browserContainer_.GetControlUnknown();
// 	HRESULT hr = lpUnk->QueryInterface(IID_IWebBrowser2, (void**)&main->m_pMainDlg->m_pWebDialog->browser_);
// 	if (!SUCCEEDED(hr))
// 	{
// 		Dprintf("query IWebBrowser2 interface error\n");
// 		main->m_pMainDlg->m_pWebDialog->browser_ = NULL;
// 		main->m_pMainDlg->m_pWebDialog->browserContainer_.DestroyWindow();
// 		main->m_pMainDlg->m_pWebDialog->DestroyWindow();
// 		return ;
// 	}
// 	
// 	tagVARIANT	flag;
// 	flag.vt = navNoWriteToCache;
// 	long v = main->m_pMainDlg->m_pWebDialog->browser_->Navigate((unsigned short*)(LPCTSTR)main->m_pMainDlg->m_pWebDialog->url_, &flag, 0, 0, 0); //, flags, targetFrameName, postData, headers
// 	main->m_pMainDlg->m_pWebDialog->m_edit.SetWindowText(main->m_pMainDlg->m_pWebDialog->url_);
// 	Dprintf("v value is %x\n", v);
// 
// 	main->m_pMainDlg->m_pWebDialog->m_bIsExitThread = FALSE;
// 	while(!main->m_pMainDlg->m_pWebDialog->m_bIsExitThread)
// 	{
// 		::Sleep(30);
// 	}
}

void CWebDialog::SetURL(CString url)
{
// 	m_bIsExitThread = TRUE;
// 	SetTimer(1, 1500, NULL);
// 	url_ = url;
// 	HANDLE hIEEndThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)IEThreadPrc, NULL, 0, NULL);
// 	KillTimer(1);
// 	if(browser_)
// 	{
// 		browser_->Release();
// 		browser_ = NULL;
// 	}
// 
// 	///得到指针
// 	LPUNKNOWN lpUnk = browserContainer_.GetControlUnknown();
// 	HRESULT hr = lpUnk->QueryInterface(IID_IWebBrowser2, (void**)&browser_);
// 	if (!SUCCEEDED(hr))
// 	{
// 		Dprintf("query IWebBrowser2 interface error\n");
// 		browser_ = NULL;
// 		browserContainer_.DestroyWindow();
// 		DestroyWindow();
// 		return ;
// 	}
// 	
// 	tagVARIANT	flag;
// 	flag.vt = navNoWriteToCache;
// 	url_ = url;
// 	long v = browser_->Navigate((unsigned short*)(LPCTSTR)url_, &flag, 0, 0, 0); //, flags, targetFrameName, postData, headers
// 	m_edit.SetWindowText(url_);
// 	SetTimer(1, 1500, NULL);
// 	Dprintf("v value is %x\n", v);

	if(!m_bPlaying)
	{
		PROCESS_INFORMATION processInfo;
		STARTUPINFO lpStartupInfo; // 用于在创建子进程时设置各种属性 

		memset(&lpStartupInfo, 0, sizeof(lpStartupInfo));
		
		lpStartupInfo.cb = sizeof(lpStartupInfo);
		
		lpStartupInfo.dwX = 0;
		lpStartupInfo.dwY = 0;
		lpStartupInfo.dwXSize = 800;
		lpStartupInfo.dwYSize = 420;
		lpStartupInfo.wShowWindow= SW_SHOWNORMAL; 
		lpStartupInfo.dwFlags= 1|2|4;
		
		memset(&processInfo, 0, sizeof(processInfo));
		CString s = "http://www.google.com";
		if(strlen(m_sUrlList[0]) > 0)
			s = m_sUrlList[0];
		if (!CreateProcess(L"\\windows\\iesample.exe", s, NULL, NULL, NULL, CREATE_NEW_CONSOLE, NULL, NULL, /*&lpStartupInfo*/ 0, &processInfo))
		{
			
		}
		m_bPlaying = TRUE;
		
		::Sleep(50);
		
		m_hIEWnd = ::FindWindow(TEXT("iExplore"), NULL);
		SetTimer(2, 5, NULL);
	
		::MoveWindow(m_hIEWnd, m_IERect.left, m_IERect.top, m_IERect.Width(), m_IERect.Height(), TRUE);
		//::MoveWindow(m_hIEWnd, 7, 54, 786, 480-54, TRUE);

		m_edit.SetWindowText(s);
		
		CloseHandle(processInfo.hThread);
		CloseHandle(processInfo.hProcess);
	}

	if(url != "")
	{
		url_ = url;
		memset(tUrl, 0, 256*2);
		wcscpy(tUrl, (LPCWSTR)(LPCTSTR)url_/*.GetBuffer(256)*/);

		gdata.dwData = 256;
		gdata.cbData = wcslen(tUrl)*2;
		gdata.lpData = (PVOID)tUrl;
		SendtoWebMessage(WM_COPYDATA, WPARAM(m_hWnd), LPARAM(&gdata));
	}
	//send url;
}

void CWebDialog::OnSetLink(WPARAM w, LPARAM l)
{
	COPYDATASTRUCT *ldata;

	/*
	char txt[256];
	Dprintf(txt, "hwnd: 0x%x\r\n", w);
	
	Dprintf(txt, "len %d %d\r\n", ldata->dwData, ldata->cbData);
	memset(txt, 0, 256);
	wcstombs(txt, (TCHAR *)ldata->lpData, ldata->cbData);
	Dprintf(txt);
	Dprintf("end \r\n");
	*/
	/*   //lxz 20090304
	ldata = (COPYDATASTRUCT *)l;
	if(ldata->lpData)
		m_edit.SetWindowText((TCHAR *)ldata->lpData);
		*/

}

void CWebDialog::OnTimer(UINT nIDEvent)
{
	if(IsWindowVisible() && m_bPlaying)
	{
		if(1 == nIDEvent)
		{
// 			BSTR	sUrl;
// 			browser_->get_LocationURL(&sUrl);		
// 			if( url_ != sUrl)
// 			{
// 				m_edit.SetWindowText(sUrl);
// 				url_ = sUrl;
// 			}
		}
		else if(2 == nIDEvent)
		{
			KillTimer(2);
			SendtoWebMessage(WM_COMMAND, 1001, 0);
		}
		else if(3 == nIDEvent && IsWindowVisible())
		{
			if(m_hIEWnd)
			{
				::SetWindowPos(m_hIEWnd, HWND_TOPMOST, m_IERect.left, m_IERect.top, m_IERect.Width(), m_IERect.Height(), 0);
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CWebDialog::OnKillWEBShow(WPARAM w, LPARAM l)
{
	if(IsWindowVisible())
	{
		if(m_hIEWnd)
		{
			if(w == 1)
				KillTimer(3);
			else
			{
				if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pNetStatusDlg->IsWindowVisible() || ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_AlarmShowDlg->IsWindowVisible() || ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->IsWindowVisible())
					return;
				SetTimer(3, 30, NULL);
			}
		}
	}
}

void CWebDialog::ShowWindow_(BOOL bShow)
{
	if(bShow)
	{
		SetURL("");
		ShowWindow(SW_SHOW);
		if(m_hIEWnd)
		{
			//::SetWindowPos(m_hIEWnd, HWND_TOPMOST, 7, 54, 786, 356, 0);
			::ShowWindow(m_hIEWnd, SW_SHOW);
			::SetForegroundWindow(m_hIEWnd);  
			SetTimer(3, 30, NULL);
		}
	}
	else
	{
		SipShowIM(SIPF_OFF);
		KillTimer(3);
		ShowWindow(SW_HIDE);
		if(m_hIEWnd)
		{
			SendtoWebMessage(WM_BROW_STOP, 0, 0);
			::ShowWindow(m_hIEWnd, SW_HIDE);
		}
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->SetScreenSaveTimer();
	}
}


void CWebDialog::SendtoWebMessage(UINT message, WPARAM w, LPARAM l)
{
	if(m_hIEWnd)
	{
		::SendMessage(m_hIEWnd, message, w, l);
	}
}

void CWebDialog::ExitWeb()
{
	if(m_bPlaying)
	{
		KillTimer(3);
		m_bPlaying = FALSE;
		SendtoWebMessage(WM_CLOSE, 0, 0);
		m_hIEWnd = NULL;
	}
}

/*
void CWebDialog::SetPlayList(TCHAR *dir, int local)
{
	memset(m_chDir, 0, 128*2);
	m_lstPlayList.DeleteAllItems();
	int ncount = 0;
	if(local == 0)
	{
		if(DetectDIR(_T("\\usbdisk")))
			m_lstPlayList.InsertItem(ncount++, _T("usbdisk"), 3);
		if(DetectDIR(_T("\\storagecard")))
			m_lstPlayList.InsertItem(ncount++, _T("storagecard"), 3);
		memcpy(m_chDir, _T("/flashdrv/my_music/"), wcslen(_T("/flashdrv/my_music/"))*2);
	}
	
	else
	{
		CString s = Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM).c_str();
		m_lstPlayList.InsertItem(ncount++, s, 4);
		memcpy(m_chDir, dir, wcslen(dir)*2);
	}
	CString sDir = m_chDir;
	WIN32_FIND_DATA FindFileData;			//查找文件时要使用的数据结构
	HANDLE hFind = INVALID_HANDLE_VALUE;	//定义查找句柄
	
	sDir += "*.*";
	hFind = FindFirstFile(sDir, &FindFileData);//使用FindFirstFile函数来开始文件查找
	
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return;
	} 
	else 
	{
		char filename[128];
		//		Dprintf ("First file name is %s\n", FindFileData.cFileName);
		int i = wcstombs( filename, FindFileData.cFileName, 128);
		filename[i] = '\0';
		if(strstr(filename, ".text1")||strstr(filename, ".text1")||strstr(filename, ".doc")||strstr(filename, ".DOC")||strstr(filename, ".ppt")||strstr(filename, ".PPT"))
		{
			m_lstPlayList.InsertItem(ncount, FindFileData.cFileName, 0);
			m_lstPlayList.SetItemData(ncount++, 0);
		}
		else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			m_lstPlayList.InsertItem(ncount++, FindFileData.cFileName, 2);
		}
		
		//以下是循环使用FindNextFile函数来查找文件
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			//			Dprintf ("Next file name is %s\n", FindFileData.cFileName);
			i = wcstombs( filename, FindFileData.cFileName, 128);
			filename[i] = '\0';
			if(strstr(filename, ".text1")||strstr(filename, ".text1")||strstr(filename, ".doc")||strstr(filename, ".DOC")||strstr(filename, ".ppt")||strstr(filename, ".PPT"))
			{
				m_lstPlayList.InsertItem(ncount, FindFileData.cFileName, 0);
				m_lstPlayList.SetItemData(ncount++, 0);
			}
			else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				m_lstPlayList.InsertItem(ncount++, FindFileData.cFileName, 2);
			}
		}
		
		DWORD dwError = GetLastError();
		if (dwError == ERROR_NO_MORE_FILES) 
		{
			FindClose(hFind);//关闭查找句柄
		} 
		else 
		{
			//			Dprintf ("FindNextFile error. Error is %u\n", dwError);
			return;
		}
	}
	m_lstPlayList.SetScrollRagle(TRUE);
	return;	
}
*/

/*
void CWebDialog::OnClickPlayList(NMHDR* pNMHDR, LRESULT* pResult)
{
	POSITION pos = m_lstPlayList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_lstPlayList.GetNextSelectedItem (pos);
		CString s;
		CString s1 = Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM).c_str();
		s = m_lstPlayList.GetItemText(index, 0);
		//usb
		if(s.Compare(_T("usbdisk")) == 0)
		{
			SetPlayList(_T("\\usbdisk\\"), 1);
		}
		//sd
		else if(s.Compare(_T("storagecard")) == 0)
		{
			SetPlayList(_T("\\storagecard\\"), 1);
		}
		//上一级
		else if(s.Compare(s1) == 0)
		{
			//	m_chDir
			CString sDir = m_chDir;
			int n = sDir.Find(_T("\\"));
			UINT8 old[16];
			int i = 0;
			while(n != -1)
			{
				old[i++] = n;
				n += 1;
				n = sDir.Find(_T("\\"), n);
			}
			sDir = sDir.Mid(0, old[i-2]+1);
			if(sDir == _T("\\"))
				SetPlayList(sDir.GetBuffer(128), 0);
			else
				SetPlayList(sDir.GetBuffer(128), 1);
		}
		
		else
		{
			int n = s.Find(_T(".doc"));
			int n1 = s.Find(_T(".DOC"));
			int n2 = s.Find(_T(".ppt"));
			int n3 = s.Find(_T(".PPT"));
			int n4 = s.Find(_T(".text1"));
			int n5 = s.Find(_T(".text1"));
			if((n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1) && (n4 == -1) && (n5 == -1))
			{
				CString sDir = m_chDir;
				sDir += s; 
				sDir += "\\";
				SetPlayList(sDir.GetBuffer(128), 1);
			}
			else
			{
				CString s1 = m_lstPlayList.GetItemText(index, 0).GetBuffer(128);   
				CString s;
				s = m_chDir;
				s += s1;
				m_edit.SetWindowText(s);
			}
		}
	}
	*pResult = 0;
}
*/
