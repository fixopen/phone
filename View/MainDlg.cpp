// MainDlg.cpp : implementation file
//
#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../Logical/Telephone.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

#define TIMER_MAINDLG		0xFFF3
#define SECONDBYTES8           2048
#define SECONDBYTES2           790
extern DWORD PreFreeSize;
extern DWORD sdPreFreeSize;
//检测路径是否存在
BOOL DetectDIR(TCHAR *sDir)
{
	BOOL flag = FALSE;
	WIN32_FIND_DATA fd; 
	HANDLE hFind = FindFirstFile(sDir, &fd); 
	if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
	{ 
		flag = TRUE;
	} 
	FindClose(hFind);
	return flag;
}

BOOL DetectFile(TCHAR *sDir)
{
	BOOL flag = FALSE;
	WIN32_FIND_DATA fd; 
	HANDLE hFind = FindFirstFile(sDir, &fd); 
	if (hFind != INVALID_HANDLE_VALUE) 
	{ 
		flag = TRUE;
	} 
	FindClose(hFind);
	return flag;
}

void CopyDirFiles(TCHAR *src, TCHAR *des)
{
	CString SrcDir = src;
	CString DesDir = des;
	CString findFilename = SrcDir + "/*.*";
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile((LPCTSTR)findFilename, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
	}

	else
	{
		bool finished = false;
		do
		{
			wchar_t wideToName[256];
			wsprintf(wideToName, _T("%s/%s"), DesDir, (LPCTSTR)FindFileData.cFileName); //findFileName
			wchar_t wideFromName[256] = {0};
			wsprintf(wideFromName, _T("%s/%s"), SrcDir, (LPCTSTR)FindFileData.cFileName); //findFileName
			
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
			}
			else
			{
				if (!::CopyFile(wideFromName, wideToName, false))
				{
				}
			}
			
			if (!FindNextFile(hFind, &FindFileData))
			{
				if (GetLastError() == ERROR_NO_MORE_FILES)
				{
					finished = true;
				}
			}
		} while (!finished);
		FindClose(hFind);
	}
}

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
	//}}AFX_DATA_INIT
	m_nRdTime = 0;
}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
//	ON_MESSAGE(WM_CHANGEWINDOW, OnChangeWindow)
	ON_MESSAGE(WM_STATIC_CLICK, OnStaticClick)
	ON_MESSAGE(WM_SOFTKEY, OnAppSoftKey)
	ON_MESSAGE(WM_CHANGELANGUAGE,OnChangeLanguage)
//	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void CMainDlg::OnAppSoftKey(WPARAM wParam, LPARAM lParam)
{
	if(wParam == 4)
	{
		OnLunarderBtn();
	}
	else if(wParam == 5)
	{
		OnCalculBtn();
	}
	else if(wParam == 6)
	{
		OnScreenSaveBtn();
	}
}

void CMainDlg ::OnScreenSaveBtn()
{
	m_mainScreenSaveDlg_->ShowWindow(TRUE);
}

void CMainDlg::OnCalculBtn()
{
	m_mainCalucaterDlg_->ShowWindow(TRUE);
}

void CMainDlg::OnLunarderBtn()
{
	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	m_mainLunarderDlg_->SetData(curtime.wYear, curtime.wMonth, curtime.wDay);
	m_mainLunarderDlg_->ShowWindow(TRUE);
}

// CMainDlg message handlers
void CMainDlg::SetCtrlEnable(BOOL flag)
{
//	m_LineStatic.EnableWindow(flag);
//	m_TodayStatic.EnableWindow(flag);
	m_TelStatusStatic.EnableWindow(flag);
	m_CallWallStatic.EnableWindow(flag);
	m_TimeStatic.EnableWindow(flag);
}

BOOL CMainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	// TODO: Add extra initialization here
	
	//未接电话
	m_TelStatusStatic.Create(L"", WS_CHILD|WS_VISIBLE, CRect(263, 112*VHEIGHT, 263+203-1, 133*VHEIGHT), this, IDC_STATIC_TELSTATUS);
	m_TelStatusStatic.SetColor(Data::g_mainTxtColor[0][Data::g_skinstyle], RGB(0, 0, 0));
//	m_TelStatusStatic.SetUnderLine(TRUE);
	m_TelStatusStatic.SetBitmapID(Data::g_mainTxtBmpID[Data::g_skinstyle]);
	m_TelStatusStatic.SetAlign(DT_CENTER);
	m_TelStatusStatic.SetClicked(TRUE);
	m_TelStatusStatic.SetFontSize(18);

	//防火墙信息
	m_CallWallStatic.Create(L"", WS_CHILD|WS_VISIBLE,\
		CRect(263, 141*VHEIGHT, 263+203-1, 163*VHEIGHT), this, IDC_STATIC_CALLWALL);
	
	m_CallWallStatic.SetColor(Data::g_mainTxtColor[1][Data::g_skinstyle], RGB(0, 0, 0));
	m_CallWallStatic.SetBitmapID(Data::g_mainTxtBmpID[Data::g_skinstyle]);
	m_CallWallStatic.SetAlign(DT_CENTER);
	m_CallWallStatic.SetClicked(TRUE);
	m_CallWallStatic.SetFontSize(18);

	//记事
	m_TodayNoteStatic.Create(L"", WS_CHILD|WS_VISIBLE, CRect(263, 170*VHEIGHT, 263+203, 192*VHEIGHT), this, IDC_STATIC_NOTETATUS);
	m_TodayNoteStatic.SetBitmapID(Data::g_mainTxtBmpID[Data::g_skinstyle]);
	m_TodayNoteStatic.SetAlign(DT_CENTER);
	m_TodayNoteStatic.SetColor(Data::g_mainTxtColor[2][Data::g_skinstyle], RGB(0, 0, 0));
	m_TodayNoteStatic.SetClicked(TRUE);
	m_TodayNoteStatic.SetFontSize(18);

	//左边
	//录音模式
	int xbegin  = 24   ;
	int ybegin  = 19   ;
	int xWidth  = 206  ;
	int yHeight = 18   ; 
	int nVal    = 24   ;
	std::string strTemp;
	CString str;
	strTemp=Data::LanguageResource::Get(Data::RI_REC_MODE);
	str=strTemp.c_str();
	m_sticRecordMode.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, ybegin*VHEIGHT, xbegin+xWidth, (ybegin+yHeight)*VHEIGHT), this);
	m_sticRecordMode.SetColor(RGB(0, 0, 0), Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_sticRecordMode.SetFontSize(18);

	//录音质量
	strTemp=Data::LanguageResource::Get(Data::RI_RECORD_QUALITY);
	str=strTemp.c_str();
	m_sticRecordQuality.Create(str, WS_CHILD/*|WS_VISIBLE*/,\
		CRect(24, 40*VHEIGHT, 230, 58*VHEIGHT), this);
	m_sticRecordQuality.SetColor(RGB(0, 0, 0), Data::g_allFramBackRGB[Data::g_skinstyle]);
	
	//剩余录音时长
	ybegin += nVal; 
	strTemp=Data::LanguageResource::Get(Data::RI_REMAIN_TIME);
    str=strTemp.c_str();
	m_sticRecordRemain.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, ybegin*VHEIGHT, xbegin+xWidth, (ybegin+yHeight)*VHEIGHT), this);
	m_sticRecordRemain.SetColor(RGB(0, 0, 0), Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_sticRecordRemain.SetFontSize(18);

	//磁盘总容量
	ybegin += nVal;
	strTemp=Data::LanguageResource::Get(Data::RI_TOTAL_CAPACITY);
	str=strTemp.c_str();
	m_sticDiskVolume.Create(str, WS_CHILD|WS_VISIBLE, \
		CRect(xbegin, ybegin*VHEIGHT, xbegin+xWidth,(ybegin+yHeight)*VHEIGHT), this);
	m_sticDiskVolume.SetColor(RGB(0, 0, 0), Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_sticDiskVolume.SetFontSize(18);

	//录音可用空间
	ybegin += nVal;
	strTemp=Data::LanguageResource::Get(Data::RI_REC_SPACE);
	str=strTemp.c_str();
	m_sticUnUsedDiskVolume.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, ybegin*VHEIGHT, xbegin+xWidth, (ybegin+yHeight)*VHEIGHT), this);
	m_sticUnUsedDiskVolume.SetColor(RGB(0, 0, 0), Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_sticUnUsedDiskVolume.SetFontSize(18);

	//录音存储空间
	ybegin += nVal;
	m_procbarDiskUse.Create(WS_CHILD|WS_VISIBLE,\
		CRect(xbegin-1, ybegin*VHEIGHT, xbegin+xWidth, (ybegin+yHeight)*VHEIGHT), this, 0xFFFF);
	m_procbarDiskUse.SetParam(0, 0, 100, 1);
	m_procbarDiskUse.SetPos(0);
	m_procbarDiskUse.SetDisable(true);
	
	ICONKEYSTRUCT softkeyval_[3];
	softkeyval_[0].sKeyRect = CRect(0, 23*VHEIGHT, 69, 64*VHEIGHT);//
	softkeyval_[0].nKeyValue = 4;
//	softkeyval_[0].nTextId = Data::RI_MAIN_CALENDARBTN;
	softkeyval_[0].nTextId = 0;
	softkeyval_[0].nBitmapId = IDB_BITMAP_MAINCAND;
	softkeyval_[0].nAlign = IMAGE_TOP;
	softkeyval_[0].bToggle = FALSE;
	
	softkeyval_[1].sKeyRect = CRect(70, 23*VHEIGHT, 139, 64*VHEIGHT);
	softkeyval_[1].nKeyValue = 5;
//	softkeyval_[1].nTextId = Data::RI_MAIN_CALCULATORBTN;
	softkeyval_[1].nTextId = 0;
	softkeyval_[1].nBitmapId = IDB_BITMAP_MAINCALU;
	softkeyval_[1].nAlign = IMAGE_TOP;
	softkeyval_[1].bToggle = FALSE;
	
	softkeyval_[2].sKeyRect = CRect(140, 23*VHEIGHT, 209, 64*VHEIGHT);
	softkeyval_[2].nKeyValue = 6;
//	softkeyval_[2].nTextId = Data::RI_MAIN_SCREENSAVERBTN;
	softkeyval_[2].nTextId = 0;
	softkeyval_[2].nBitmapId = IDB_BITMAP_MAINSAVE;
	softkeyval_[2].nAlign = IMAGE_TOP;
	softkeyval_[2].bToggle = FALSE;
	
	m_AppSoftKey.Create(0, 0, -1, CPoint(0, 0),\
		3, softkeyval_, CRect(15, 123*VHEIGHT, 15+210-1, 188*VHEIGHT), this, WS_VISIBLE|WS_CHILD, IDC_BUTTON1);
	m_AppSoftKey.m_clrBack = Data::g_allFramBackRGB[Data::g_skinstyle];

	m_mainLunarderDlg_ = new CLunarderDlg;
	m_mainLunarderDlg_->Create(CLunarderDlg::IDD, this);
	m_mainLunarderDlg_->ShowWindow(SW_HIDE);

	
	m_mainScreenSaveDlg_ = new CScreenSaveDlg();
	m_mainScreenSaveDlg_->Create(CScreenSaveDlg::IDD);
	m_mainScreenSaveDlg_->ShowWindow(SW_HIDE);
	
	m_mainCalucaterDlg_ = new CCalculaterDlg();
	m_mainCalucaterDlg_->Create(CCalculaterDlg::IDD, this);
	m_mainCalucaterDlg_->ShowWindow(SW_HIDE);
	
	
	m_firewalDlg_ = new CFireWallDlg();
	m_firewalDlg_->Create(CFireWallDlg::IDD, this);
	m_firewalDlg_->ShowWindow(SW_HIDE);

	//时间控件
	m_TimeStatic.Create(L"", WS_CHILD|WS_VISIBLE,\
		CRect(262, 24, 262+194, 108),this, IDC_STATIC_TIME);
	m_TimeStatic.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	m_FrameStatic.Create(CRect(0, 0, 480, 204*VHEIGHT), this, 0);

	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	m_nUnconnectCount = main->m_pTelephoneDlg->GetUnconnectCount();
	m_nRecordCount = main->m_pTelephoneDlg->GetRecordCount();

	OnShowNoteStatic();
	OnShowCallWallStatic();
	OnShowTelStatusStatic();
	UpdateDiskInfo();
	SetTimer(1, 60*60*1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//add function
//获取未接电话个数
int	CMainDlg::GetMissedTelInfo()
{
	//todo :: Missed finish!
	return 3;
}
//获取留言电话个数
int	CMainDlg::GetRecordedTelInfo()
{
	//todo :: Missed finish!
	return 2;
}

//获取是否设置启动防火墙
BOOL CMainDlg::GetCallWallInfo()
{
	//todo :: Missed finish!
	return TRUE;
}

void CMainDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting message
}

VOID WriteMyLog_(char *ptr, int size)
{
	FILE* fp = fopen("\\hive\\mylog.txt", "a+");
	if (fp != NULL) 
	{
		fwrite(ptr, sizeof(char), size, fp);
		fwrite("\n", strlen("\n"), 1, fp);
		fclose(fp);
	}
}

void CMainDlg::OnTimer(UINT nIDEvent)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	if(1 == nIDEvent)
	{
		PostMessage(WM_NOTENOTIFY, 0, 0);
	}
	else if(2 == nIDEvent)  //wzx test
	{
		if(main->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOff)
		{
			main->phone_->HungOff();
			SetTimer(3, 2000, NULL);
		}
		else
		{
			main->phone_->HungOn();
			Sleep(1000);
			main->phone_->DialNumber("807");
		}
	}
	else if(3 == nIDEvent)
	{
		KillTimer(3);
		main->phone_->HungOn();
		Sleep(1000); 
		main->phone_->DialNumber("807");
	}
	CDialog::OnTimer(nIDEvent);
}


void CMainDlg::OnStaticClick(WPARAM w, LPARAM l)
{
//	SetTimer(2, 90*1000, NULL);
	if(w == IDC_STATIC_CALLWALL)		//防火墙
	{
		m_firewalDlg_->OnStart();
		m_firewalDlg_->ShowWindow(TRUE);
	}
	else if(w == IDC_STATIC_TELSTATUS)	//未接电话、留言电话
	{
		//todo: 进入未读电话记录
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
		main->m_pContactInfoDlg->ShowUnconnectItems();
		main->m_pContactInfoDlg->ShowWindow(SW_SHOW);	
	}
	else if(w == IDC_STATIC_NOTETATUS)		//今天是否有记事
	{	
		SYSTEMTIME curtime;
		GetLocalTime(&curtime);
		m_mainLunarderDlg_->SetData(curtime.wYear, curtime.wMonth, curtime.wDay);
		m_mainLunarderDlg_->ShowWindow(TRUE);
	}
}

void CMainDlg::OnShowNoteStatic()
{
	SYSTEMTIME curtime;

	GetLocalTime(&curtime);

	CString s;

	if(!m_mainLunarderDlg_->m_lunarder.IsHaveNote(curtime.wYear, curtime.wMonth, curtime.wDay, s))
	{
		s = Data::LanguageResource::Get(Data::RI_MAIN_NONOTE).c_str(); 
	}
	m_TodayNoteStatic.SetWindowText(s);
}

void CMainDlg::OnShowCallWallStatic()
{
	CString s;

	int type = m_firewalDlg_->GetFirewallType();

	if(type == -1)
	{
		s = Data::LanguageResource::Get(Data::RI_MAIN_PROTECTEDWALL_CLOSE_STC).c_str();

	}else{

		char txt[128];
		const char *strtemp[] = {\
		Data::LanguageResource::Get(Data::RI_FIREWALL_BLACKLIST).c_str(),\
		Data::LanguageResource::Get(Data::RI_FIREWALL_CONTACT).c_str(),\
		Data::LanguageResource::Get(Data::RI_FIREWALL_VIP).c_str(),\
		Data::LanguageResource::Get(Data::RI_FIREWALL_ALL).c_str()\
		};
		sprintf(txt, "%s%s", Data::LanguageResource::Get(Data::RI_MAIN_PROTECTEDWALL_OPEN_STC).c_str(), strtemp[type]);
		s = txt;
	}
	m_CallWallStatic.SetWindowText(s);
}

void CMainDlg::OnShowTelStatusStatic()
{
	//todo: 获取未接电话条数
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	int n1 = m_nUnconnectCount;
	int n2 = m_nRecordCount;
	if(n1 > 0)
	{
		main->phone_->OpenCallLight(TRUE); 
	}
	else
	{
		main->phone_->OpenCallLight(FALSE);
	}

	if(n2 > 0)
	{
		main->phone_->OpenLeveaLight(TRUE);
	}
	else
	{
		main->phone_->OpenLeveaLight(FALSE);
	}

	char txt[64] = {0};
	sprintf(txt, "%d%s%s(%d%s%s)", n1, Data::LanguageResource::Get(Data::RI_COMN_ENTRIESSTC).c_str(), Data::LanguageResource::Get(Data::RI_COMN_MISSTELSTC).c_str(),\
		n2, Data::LanguageResource::Get(Data::RI_COMN_ENTRIESSTC).c_str(), Data::LanguageResource::Get(Data::RI_COMN_RECORDETELSTC).c_str());
	CString s = txt;
	m_TelStatusStatic.SetWindowText(s);
}

LRESULT CMainDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
	case WM_TELNOTIFY:
		OnShowTelStatusStatic();
		UpdateDiskInfo();
		break;
	case WM_NOTENOTIFY:
		OnShowNoteStatic();
		break;
	case WM_FIRENOTIFY:
		OnShowCallWallStatic();
		break;
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}

void CMainDlg::UpdateDiskInfo(void)
{
	CString text;

	std::string strTemp;

	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isMustRecord())
	{
		strTemp=Data::LanguageResource::Get(Data::RI_REC_MODE_AUTO);
		text = strTemp.c_str();
	}
	else
	{
		strTemp=Data::LanguageResource::Get(Data::RI_REC_MODE_MT);
		text = strTemp.c_str();
	}
	
	m_sticRecordMode.SetWindowText(text);
	
	bool q = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isDeleteProtect();
	if (q)
	{
		strTemp=Data::LanguageResource::Get(Data::RI_REC_HQUALITY);
		text = strTemp.c_str();
	}
	else
	{
		strTemp=Data::LanguageResource::Get(Data::RI_REC_STATARD);
		text = strTemp.c_str();
	}
	
	m_sticRecordQuality.SetWindowText(text);

	ULARGE_INTEGER freeBytes;
	ULARGE_INTEGER totalBytes;
	ULARGE_INTEGER freeBytesSD;
	ULARGE_INTEGER totalBytesSD;
	DWORD total = 0;
	GetDiskFreeSpaceEx(csFlashRecordPath, &freeBytes, &totalBytes, NULL);
	BOOL sd = DetectDIR(L"\\StorageCard");
	int error = 0;
	if (sd)
	{
		error = GetDiskFreeSpaceEx(L"\\StorageCard", &freeBytesSD, &totalBytesSD, NULL);
		//error = GetLastError(); //1617 设备已被删除
		if(error != 0)
			total = (totalBytes.QuadPart + totalBytesSD.QuadPart) / (1024*1024);
		else
			total = (totalBytes.QuadPart) / (1024*1024);
	}
	else
	{
		total = (totalBytes.QuadPart) / (1024*1024);
	}

	CString str2;
	std::string s;
	CString str1;
	CString str;
	s=Data::LanguageResource::Get(Data::RI_TOTAL_SPACE);
	str1=s.c_str();
	str2.Format(_T(" %d M"), total);
	m_sticDiskVolume.SetWindowText(str1+str2);

	LONGLONG remain = freeBytes.QuadPart - PreFreeSize;
	if(remain < 0)
	{
		if(sd && error != 0)
		{
			remain = freeBytesSD.QuadPart - sdPreFreeSize;
		}
	}
	else
	{
		if(sd && error != 0)
		{
			LONGLONG fr = freeBytesSD.QuadPart - sdPreFreeSize;
			if(fr > 0)
			{
				remain += fr;
			}
		}
	}

	if (remain < 0)
	{
		remain = 0;
	}

	s=Data::LanguageResource::Get(Data::RI_REC_REMAIN_SPACE);
	str1=s.c_str();
	if(remain > 0 && remain < 1024*1024)
	{
		str2 = L"0.1 M";  //容量不足一M时显示0.1M
	}
	else
	{
		str2.Format(_T(" %d M"), remain/(1024*1024));
	}
	m_sticUnUsedDiskVolume.SetWindowText(str1+str2);
	m_procbarDiskUse.SetParam(0, 0, total, 1);
	if(remain > 0 && remain < 1024*1024)
	{
		m_procbarDiskUse.SetPos(total - 2);
	}
	else
	{
		m_procbarDiskUse.SetPos(total - remain/(1024*1024));
	}
	
	LONGLONG t = 0;
	if (remain > 0)
	{
		if (q)
		{
			t = remain / SECONDBYTES8;
		}
		else
		{
			t = remain / SECONDBYTES2;
		}
	}
	
	m_nRdTime = t;
	CString duration;
	if (t > 3600)
	{
		if(t/3600 > 100)
			t += (3600*10);
		else if(t/3600 > 50)
			t += (3600*5);
		if (gLangItem == 1)
		{
			duration.Format(_T("%02d小时%02d分"), t / 3600, t % 3600 / 60);
		}
		else
		{
	    	duration.Format(_T("%02dh%02dm"), t / 3600, t % 3600 / 60);
		}
	}
	else
	{
		int second = t % 60;
		int minute = t / 60;
		if (gLangItem == 1)
		{
			duration.Format(_T("%02d分%02d秒"), minute, second);
		}
		else
		{
			duration.Format(_T("%02dm%02ds"), minute, second);
		}
	}

	s = Data::LanguageResource::Get(Data::RI_REC_REMAIN_TIME);
	str1 = s.c_str();
	m_sticRecordRemain.SetWindowText(str1+duration);
}
void CMainDlg::ShowWindow_(int nCmdShow)
{
	if (nCmdShow > 0)
	{
		UpdateDiskInfo();
	}
	ShowWindow(nCmdShow);
}

LRESULT CMainDlg::OnChangeLanguage(WPARAM wParam,LPARAM lParam)//?
{
	ChangeLanguage();
    OnShowNoteStatic();
	OnShowCallWallStatic();
	OnShowTelStatusStatic();
	UpdateDiskInfo();
	((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->ShowWindow(SW_SHOW);
	((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pMainDlg->ShowWindow(SW_SHOW);
	return 0;
}

void CMainDlg::ChangeLanguage()
{  
 	
	m_mainLunarderDlg_->ShowWindow(SW_HIDE);

	std::string strTemp;
	CString str;
	strTemp=Data::LanguageResource::Get(Data::RI_REC_MODE_EG);
	str=strTemp.c_str();
	m_sticRecordMode.SetWindowText(str);

	strTemp=Data::LanguageResource::Get(Data::RI_RECORD_QUALITY);
	str=strTemp.c_str();
	m_sticRecordQuality.SetWindowText(str);

	strTemp=Data::LanguageResource::Get(Data::RI_REMAIN_TIME);
    str=strTemp.c_str();
	m_sticRecordRemain.SetWindowText(str);

	strTemp=Data::LanguageResource::Get(Data::RI_TOTAL_CAPACITY);
	str=strTemp.c_str();
	m_sticDiskVolume.SetWindowText(str);
	
	strTemp=Data::LanguageResource::Get(Data::RI_REC_SPACE);
	str=strTemp.c_str();
	m_sticUnUsedDiskVolume.SetWindowText(str);	
}
