// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/LanguageResource.h"
#include "../Logical/LunarderDate.h"
#include "../Data/SkinStyle.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

#define TIMER_MAINDLG		0xFFF3
extern BOOL DetectDIR(TCHAR *sDir);

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
	m_mainmenuDlg_ = NULL;
	m_nUnTel = 0;
	m_nLeaveSound = 0;
	m_bIsFireWall = FALSE;
	m_bIsAlarm = FALSE;
	//}}AFX_DATA_INIT
}

CMainDlg::~CMainDlg()
{
	if(m_mainmenuDlg_)
		delete m_mainmenuDlg_;
	m_mainmenuDlg_ = NULL;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	//}}AFX_DATA_MAP
	/*
	DDX_Control(pDX, IDC_STATIC_LINE, m_LineStatic);
	DDX_Control(pDX, IDC_STATIC_TODAY, m_TodayStatic);
	DDX_Control(pDX, IDC_STATIC_TELSTATUS, m_TelStatusStatic);
	DDX_Control(pDX, IDC_STATIC_CALLWALL, m_CallWallStatic);
	DDX_Control(pDX, IDC_STATIC_TIME, m_TimeStatic);
	*/
}

void CMainDlg ::OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized )
{
	if(nState)
	{
	/*	OnShowCallWallStatic();
		OnShowNoteStatic();
		OnShowTelStatusStatic();
		*/
	}
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_MESSAGE(WM_CHANGEWINDOW, OnChangeWindow)
	ON_MESSAGE(WM_PLAYVIDEO, OnPlayVideo)
	ON_MESSAGE(WM_PLAYPHOTO, OnPlayPhoto)
	ON_MESSAGE(WM_STATIC_CLICK, OnStaticClick)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPGToApp)
	ON_MESSAGE(WM_MJPGSHOWHALF, OnMJPGShowHalf)
	ON_WM_ACTIVATE()
	ON_MESSAGE(WM_CHECKPASSWORD, OnSetAdmin)
	ON_MESSAGE(WM_DELETESELITEM, OnUnSetAdmin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers
void CMainDlg::SetCtrlEnable(BOOL flag)
{
//	m_LineStatic.EnableWindow(flag);
//	m_TodayStatic.EnableWindow(flag);
	/*
	m_TelStatusStatic.EnableWindow(flag);
	m_CallWallStatic.EnableWindow(flag);
	m_TimeStatic.EnableWindow(flag);
	*/
}

BOOL CMainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
//	m_LineStatic.Create(L"", WS_CHILD|WS_VISIBLE, CRect(247-MAIN_MARGIN_OFFSET,9, 248-MAIN_MARGIN_OFFSET, 195), this, IDC_STATIC_LINE);
//	m_LineStatic.SetColor(RGB(0, 0, 0), DIVIDEDLINE_RGB);
	
//	m_TodayStatic.Create(L"", WS_CHILD|WS_VISIBLE, CRect(252-MAIN_MARGIN_OFFSET,9, 471-MAIN_MARGIN_OFFSET, 29), this, IDC_STATIC_TODAY);
//	m_TodayStatic.SetColor(RGB(0, 0, 0), STATIC_RGB1);
//	CString s = Data::LanguageResource::Get(Data::RI_MAIN_TODAYSTC).c_str();
//	m_TodayStatic.SetWindowText(s);

	m_pWebDialog = new CWebDialog;
	m_pWebDialog->Create(CWebDialog::IDD);
	m_pWebDialog->ShowWindow_(SW_HIDE);

	
/*
//	m_TelStatusStatic.Create(L"", WS_CHILD|WS_VISIBLE, CRect(263, 111, 263+203-1, 111+23-1), this, IDC_STATIC_TELSTATUS);
	m_TelStatusStatic.Create(L"", WS_CHILD|WS_VISIBLE, CRect(278+3, 110+2, 278+3+158-1, 110+21-1), this, IDC_STATIC_TELSTATUS);
	m_TelStatusStatic.SetColor(Data::g_mainTxtColor[0][Data::g_skinstyle], RGB(0, 0, 0));
//	m_TelStatusStatic.SetUnderLine(TRUE);
	m_TelStatusStatic.SetBitmapID(IDB_BITMAP_WHSTATIC Data::g_mainTxtBmpID[Data::g_skinstyle] );
	m_TelStatusStatic.SetAlign(DT_CENTER);
	m_TelStatusStatic.SetClicked(TRUE);

//	m_CallWallStatic.Create(L"", WS_CHILD|WS_VISIBLE, CRect(263, 141, 263+203-1, 141+23-1), this, IDC_STATIC_CALLWALL);
	m_CallWallStatic.Create(L"", WS_CHILD|WS_VISIBLE, CRect(278+3, 140+2, 278+3+158-1, 140+21-1), this, IDC_STATIC_CALLWALL);
	m_CallWallStatic.SetColor(Data::g_mainTxtColor[1][Data::g_skinstyle], RGB(0, 0, 0));
	m_CallWallStatic.SetBitmapID(IDB_BITMAP_WHSTATIC Data::g_mainTxtBmpID[Data::g_skinstyle] );
	m_CallWallStatic.SetAlign(DT_CENTER);
	m_CallWallStatic.SetClicked(TRUE);

	//m_TodayNoteStatic.Create(L"", WS_CHILD|WS_VISIBLE, CRect(263, 170, 263+203-1, 170+23-1), this, IDC_STATIC_NOTETATUS);
	m_TodayNoteStatic.Create(L"", WS_CHILD|WS_VISIBLE, CRect(278+3, 168+2, 278+3+158-1, 168+21-1), this, IDC_STATIC_NOTETATUS);
	m_TodayNoteStatic.SetBitmapID(IDB_BITMAP_WHSTATIC  Data::g_mainTxtBmpID[Data::g_skinstyle]);
	m_TodayNoteStatic.SetAlign(DT_CENTER);
	m_TodayNoteStatic.SetColor(Data::g_mainTxtColor[2][Data::g_skinstyle], RGB(0, 0, 0));

	m_TodayNoteStatic.SetClicked(TRUE);

	m_TimeStatic.Create(L"", WS_CHILD|WS_VISIBLE, CRect(262, 25, 262+194, 25+84), this, IDC_STATIC_TIME);
	*/

//	m_TimeStatic.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	//	m_TimeStatic.SetBackRGB(RGB(8, 150, 198));  blue
	//	m_TimeStatic.SetBackRGB(RGB(255, 255, 255));  //white
/*
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	if(main->n_StyleMain == 0)
		m_TimeStatic.SetBackRGB(RGB(255, 255, 255)); 
	else if(main->n_StyleMain == 1)
		m_TimeStatic.SetBackRGB(RGB(174, 179, 185));  //gray
	else if(main->n_StyleMain == 2)
		m_TimeStatic.SetBackRGB(RGB(8, 150, 198));  //blue
*/

	m_mainmenuDlg_ = new CMainMenuDlg;
	m_mainmenuDlg_->Create(CMainMenuDlg::IDD, this);
	m_mainmenuDlg_->ShowWindow_(SW_HIDE);
	m_currentWnd = (CWnd *)m_mainmenuDlg_;
		
	m_mainVideoDlg_ = new CMainVideoDlg;
	m_mainVideoDlg_->Create(CMainVideoDlg::IDD, this);
	m_mainVideoDlg_->ShowWindow(SW_HIDE);
	
	m_mainPhotoDlg_ = new CMainPhotoDlg;
	m_mainPhotoDlg_->Create(CMainPhotoDlg::IDD, this);
	m_mainPhotoDlg_->ShowWindow(SW_HIDE);
	
	m_mainMp3Dlg_ = new CMainMp3Dlg;
	m_mainMp3Dlg_->Create(CMainMp3Dlg::IDD, this);
	m_mainMp3Dlg_->ShowWindow(SW_HIDE);
	
	m_mainScreenSaveDlg_ = new CScreenSaveDlg;
	m_mainScreenSaveDlg_->Create(CScreenSaveDlg::IDD);
	m_mainScreenSaveDlg_->ShowWindow(SW_HIDE);
	
	m_mainCalucaterDlg_ = new CCalculaterDlg;
	m_mainCalucaterDlg_->Create(CCalculaterDlg::IDD, this);
	m_mainCalucaterDlg_->ShowWindow(SW_HIDE);

	m_mainLunarderDlg_ = new CLunarderDlg;
	m_mainLunarderDlg_->Create(CLunarderDlg::IDD, this);
	m_mainLunarderDlg_->ShowWindow(SW_HIDE);
	
	
	m_firewalDlg_ = new CFireWallDlg();
	m_firewalDlg_->Create(CFireWallDlg::IDD, this);
	m_firewalDlg_->ShowWindow(SW_HIDE);

	m_pHuangLiDlg_ = new CHuangliDlg;
	m_pHuangLiDlg_->Create(CHuangliDlg::IDD, this);
	m_pHuangLiDlg_->ShowWindow(SW_HIDE);

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 420), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\����\\����.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 420));

	CString s = ".\\adv\\mjpg\\k1\\common\\����¼������.bmp";
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	std::string sPath = main->m_pSettingDlg->m_pSetting->soundPath();
	if (sPath == ssStorageCardRecordPath && DetectDIR((LPTSTR)(LPCTSTR)csStarageCard))
		s = ".\\adv\\mjpg\\k1\\common\\SD��¼������.bmp";
	else
		sPath = ssFlashRecordPath;
	m_MJPGList.SetUnitBitmap(300, s, s, TRUE);
	s = "�ֶ�";
	if(main->m_pSettingDlg->m_pSetting->isMustRecord())
		s = "�Զ�";
	m_MJPGList.SetUnitText(301, s, TRUE);
	ULARGE_INTEGER freeBytes;
	ULARGE_INTEGER totalBytes;
	GetDiskFreeSpaceEx(Util::StringOp::ToCString(sPath), &freeBytes, &totalBytes, NULL);
	int nProcess = (totalBytes.QuadPart*10/9-freeBytes.QuadPart)*100/(totalBytes.QuadPart*10/9);
	char txt[64];
	sprintf(txt, ".\\adv\\mjpg\\k1\\common\\�ٷֱ�%d.bmp", nProcess/10);
	m_MJPGList.SetUnitBitmap(23, txt, txt, TRUE);

	SetDateTime(FALSE);
	SetTimer(1, 1000, NULL);
	SetTimer(2, 5000, NULL);
	SetTimer(4, 1000, NULL);

	OnShowNoteStatic();
	OnShowCallWallStatic();
	OnShowTelStatusStatic(0, 0);
//	OnTimer(2);

	SetPhotoList();

//	for(int i = 12; i < 20; i++)
//		m_MJPGList.SetUnitIsTranslate(i, TRUE);

//	m_FrameStatic.Create(CRect(0, 0, 480, 204), this, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMainDlg::ShowTodayAlarm()
{
	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	CTime time = CTime(curtime.wYear, curtime.wMonth, curtime.wDay, 0, 0, 0);
	
	std::string filter = "startTime BETWEEN '";
	char buff[32];
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d000000", time.GetYear(), time.GetMonth(),time.GetDay());//, dateFrom.GetHour(), dateFrom.GetMinute(), dateFrom.GetSecond());
	filter += buff;
	filter += "' AND '";
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d235959", time.GetYear(), time.GetMonth(),time.GetDay());//, dateTo.GetHour(), dateTo.GetMinute(), dateTo.GetSecond());
	filter += buff;
	filter += "'";
	std::string order = "startTime";
	std::vector<boost::shared_ptr<Data::Scheduler> > m_result = Data::Scheduler::GetFromDatabaseByOrder(filter, order); 
	if (!m_result.empty())
	{
		for(int i = 0; i < m_result.size(); i++)
		{
			CTime tm = m_result[i]->startTime();
			GetLocalTime(&curtime);
			CTime nowtm = CTime(curtime.wYear, curtime.wMonth, curtime.wDay, curtime.wHour, curtime.wMinute, curtime.wSecond);
			if(m_result[i]->tipsType() == Data::ttAlarmSound && (nowtm > tm))
			{
				std::string content = m_result[i]->what();
				std::string ring = "/flashdrv/my_ring/"; 
				ring += m_result[i]->ring();
				CString content_ = "����: ";
				content_ += content.c_str();
				CTime tm = m_result[i]->startTime();
				CString sTime;
				sTime.Format(_T("ʱ��: %04d-%02d-%02d %02d:%02d:%02d"), tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
				m_result[i]->tipsType(Data::ttRead);
				m_result[i]->Update();
				((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_AlarmShowDlg->SetTxt(m_result[i]->id(), sTime, "����: ", content_, "", FALSE);
				((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_AlarmShowDlg->ShowWindow_(SW_SHOW);
				return TRUE;
			}
		}
	}
	return FALSE;
}

// // 
BOOL CMainDlg::FindTodayAlarm()
{
	m_bIsAlarm = FALSE;

	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	CTime time(curtime.wYear, curtime.wMonth, curtime.wDay, 0, 0, 0);
	
	std::string filter = "startTime BETWEEN '";
	char buff[32];
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d000000", time.GetYear(), time.GetMonth(),time.GetDay());//, dateFrom.GetHour(), dateFrom.GetMinute(), dateFrom.GetSecond());
	filter += buff;
	filter += "' AND '";
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d235959", time.GetYear(), time.GetMonth(),time.GetDay());//, dateTo.GetHour(), dateTo.GetMinute(), dateTo.GetSecond());
	filter += buff;
	filter += "'";
 	std::string order = "startTime";
//	std::string filter = "startTime BETWEEN '20090201000000' AND '20090201235959'";
	std::vector<boost::shared_ptr<Data::Scheduler> > m_result = Data::Scheduler::GetFromDatabaseByOrder(filter, order); 
	if (!m_result.empty())
	{
		for(int i = 0; i < m_result.size(); i++)
		{
			CTime tm = m_result[i]->startTime();
			GetLocalTime(&curtime);
			CTime nowtm = CTime(curtime.wYear, curtime.wMonth, curtime.wDay, curtime.wHour, curtime.wMinute, curtime.wSecond);
			if(m_result[i]->tipsType() == Data::ttAlarmSound && (nowtm > tm))
			{
				m_bIsAlarm = TRUE;
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CMainDlg::SetStatusAll(BOOL flag)
{
	  
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	main->m_MJPGList.SetUnitIsDownStatus(2, FALSE);
	main->m_MJPGList.SetUnitIsDownStatus(3, FALSE);
	main->m_MJPGList.SetUnitIsDownStatus(4, FALSE);
	main->m_MJPGList.SetUnitIsDownStatus(5, FALSE);
	main->m_MJPGList.SetUnitIsDownStatus(6, FALSE);
	main->m_MJPGList.SetUnitIsDownStatus(7, FALSE);
	if(!flag)
		main->m_MJPGList.SetUnitIsDownStatus(2, TRUE);
	main->m_MJPGList.Invalidate();
}

void CMainDlg::SetDateTime(BOOL isDraw)
{
	CString sTime;
	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	static int gHour1, gHour2, gMinute1, gMinute2, gSecond1, gSecond2;
	gHour1=gHour2=gMinute1=gMinute2=gSecond1=gSecond2 = -1;
	CString sFile;
	char filename[64];
	if(gHour1 != curtime.wHour/10)
	{
		sprintf(filename, ".\\adv\\mjpg\\k1\\common\\%d.bmp", curtime.wHour/10);
		sFile = filename;
		m_MJPGList.SetUnitBitmap(12, filename, "", isDraw);
		gHour1 = curtime.wHour/10;
	}
	if(gHour2 != curtime.wHour%10)
	{
		sprintf(filename, ".\\adv\\mjpg\\k1\\common\\%d.bmp", curtime.wHour%10);
		sFile = filename;
		m_MJPGList.SetUnitBitmap(13, filename, "", isDraw);
		gHour2 = curtime.wHour%10;
	}
	if(gMinute1 != curtime.wMinute/10)
	{
		sprintf(filename, ".\\adv\\mjpg\\k1\\common\\%d.bmp", curtime.wMinute/10);
		sFile = filename;
		m_MJPGList.SetUnitBitmap(15, filename, "", isDraw);
		gMinute1 = curtime.wMinute/10;
	}
	if(gMinute2 !=curtime.wMinute%10)
	{
		sprintf(filename, ".\\adv\\mjpg\\k1\\common\\%d.bmp", curtime.wMinute%10);
		sFile = filename;
		m_MJPGList.SetUnitBitmap(16, filename, "", isDraw);
		gMinute2 =curtime.wMinute%10;
	}

	if(gSecond1 != curtime.wSecond/10)
	{
		sprintf(filename, ".\\adv\\mjpg\\k1\\common\\%d.bmp", curtime.wSecond/10);
		sFile = filename;
		m_MJPGList.SetUnitBitmap(18, filename, "", isDraw);
		gSecond1 = curtime.wSecond/10;
	}
	
	sprintf(filename, ".\\adv\\mjpg\\k1\\common\\%d.bmp", curtime.wSecond%10);
	sFile = filename;
	m_MJPGList.SetUnitBitmap(19, filename, "", isDraw);

	static int nFresh = 0;
	CString sTel = ".\\adv\\mjpg\\k1\\common\\δ�ӵ绰1.bmp";
	CString sRecorde = ".\\adv\\mjpg\\k1\\common\\δ������1.bmp";
	CString sAlarm = ".\\adv\\mjpg\\k1\\common\\δ������1.bmp";
	CString sFireWall = ".\\adv\\mjpg\\k1\\common\\����ǽ����1.bmp";
	m_bIsFireWall = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall();
	if(nFresh++ % 2)
	{
		if(m_nUnTel > 0)
			sTel = ".\\adv\\mjpg\\k1\\common\\δ�ӵ绰.bmp";
		if(m_nLeaveSound > 0)
			sRecorde = ".\\adv\\mjpg\\k1\\common\\δ������.bmp";
		if(m_bIsFireWall)
			sFireWall = ".\\adv\\mjpg\\k1\\common\\����ǽ����.bmp";
		if(m_bIsAlarm)
			sAlarm = ".\\adv\\mjpg\\k1\\common\\δ������.bmp";
	}

	static CString gsTel = "";
	static CString gsRecorde = "";
	static CString gsAlarm = "";
	static CString gsFireWall = "";

	if(gsTel != sTel)
	{
		m_MJPGList.SetUnitBitmap(9, sTel, "", isDraw);
		gsTel = sTel;
	}
	if(gsRecorde != sRecorde)
	{
		m_MJPGList.SetUnitBitmap(8, sRecorde, "", isDraw);
		gsRecorde = sRecorde;
	}
	if(gsAlarm != sAlarm)
	{
		m_MJPGList.SetUnitBitmap(59, sAlarm, "", isDraw);
		gsAlarm = sAlarm;
	}
	if(gsFireWall != sFireWall)
	{
		m_MJPGList.SetUnitBitmap(58, sFireWall, "", isDraw);
		gsFireWall = sFireWall;
	}
	
	int nWeekDay = Logical::LunarderDate::WeekDay(curtime.wYear, curtime.wMonth, curtime.wDay);
	char txt[64];
	sprintf(txt, "%s %04d-%02d-%02d", Data::LanguageResource::Get(Data::RI_COMN_SUNSTC+nWeekDay).c_str(), curtime.wYear, curtime.wMonth, curtime.wDay);
	sTime = txt;
	static CString oldTime = "";
	//if(oldTime != sTime)    //2008
	{
		m_MJPGList.SetUnitText(10, sTime, isDraw);
		oldTime = sTime;
	}
}

//add function
//��ȡδ�ӵ绰����
int	CMainDlg::GetMissedTelInfo()
{
	//todo :: Missed finish!
	return 3;
}
//��ȡ���Ե绰����
int	CMainDlg::GetRecordedTelInfo()
{
	//todo :: Missed finish!
	return 2;
}

//��ȡ�Ƿ�������������ǽ
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

int CMainDlg::SetPhotoList()
{
	m_PhotoList.clear();
	CString sDir = "\\flashdrv\\my_show\\";
	WIN32_FIND_DATA FindFileData;			//�����ļ�ʱҪʹ�õ����ݽṹ
	HANDLE hFind = INVALID_HANDLE_VALUE;	//������Ҿ��
	
	sDir += "*.*";
	hFind = FindFirstFile(sDir, &FindFileData);//ʹ��FindFirstFile��������ʼ�ļ�����
	
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return 0;
	} 
	else 
	{
		char filename[128];
		//		Dprintf ("First file name is %s\n", FindFileData.cFileName);
		int i = wcstombs( filename, FindFileData.cFileName, 128);
		filename[i] = '\0';
		if(strstr(filename, ".bmp")||strstr(filename, ".BMP") || strstr(filename, ".jpg")||strstr(filename, ".JPG"))
		{
			//���ļ��Ĵ�С�������� < 1024K
			if(((FindFileData.nFileSizeHigh * MAXDWORD) + FindFileData.nFileSizeLow) < 1024*1024)
			{
				CString s = "\\flashdrv\\\\my_show\\";
				s += FindFileData.cFileName;
				m_PhotoList.push_back(s);
			}
		}
	
		//������ѭ��ʹ��FindNextFile�����������ļ�
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			//			Dprintf ("Next file name is %s\n", FindFileData.cFileName);
			i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
			filename[i] = '\0';
			if(strstr(filename, ".bmp")||strstr(filename, ".BMP") || strstr(filename, ".jpg")||strstr(filename, ".JPG"))
			{
				//���ļ��Ĵ�С�������� < 500K
				if(((FindFileData.nFileSizeHigh * MAXDWORD) + FindFileData.nFileSizeLow) < 1200*1024)
				{
					CString s = "\\flashdrv\\my_show\\";
					s += FindFileData.cFileName;
					m_PhotoList.push_back(s);
				}
			}
		}
		
		DWORD dwError = GetLastError();
		if (dwError == ERROR_NO_MORE_FILES) 
		{
			FindClose(hFind);//�رղ��Ҿ��
		} 
		else 
		{
			//			Dprintf ("FindNextFile error. Error is %u\n", dwError);
			return 0;
		}
	}
	return 1;	
}

void CMainDlg::OnTimer(UINT nIDEvent)
{
	
#ifdef _DEBUG
	
#else
	if(nIDEvent == 4)
	{
		extern BOOL watchdog_MFC;
		watchdog_MFC = TRUE;
	}
	
#endif
	extern BOOL g_IsRun;
    if(!g_IsRun)
		g_IsRun = TRUE;
//	extern BOOL g_bAdjustPanel;
//	if(g_bAdjustPanel)
//		g_bAdjustPanel = FALSE;
	if(IsWindowVisible() && !((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->IsWindowVisible())
	{
		if(nIDEvent == 1)
		{
			SetDateTime(TRUE);
		}
		else if(nIDEvent == 2)				//�ұߵĹ����
		{
			std::string path = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->soundPath();
				
			if (path == ssStorageCardRecordPath)//"\\StorageCard\\MY_RECORD\\")
			{
				BOOL sd = DetectDIR((LPTSTR)(LPCTSTR)csStarageCard);
				if(!sd)
				{
					CString s = ".\\adv\\mjpg\\k1\\common\\����¼������.bmp";
					m_MJPGList.SetUnitBitmap(300, s, s, TRUE);
					path = ssFlashRecordPath;
				}
				else
				{
					CString s = ".\\adv\\mjpg\\k1\\common\\SD��¼������.bmp";
					m_MJPGList.SetUnitBitmap(300, s, s, TRUE);
				}
			
			}

			else
			{
				CString s = ".\\adv\\mjpg\\k1\\common\\����¼������.bmp";
				m_MJPGList.SetUnitBitmap(300, s, s, TRUE);
			}

			ULARGE_INTEGER freeBytes;
			ULARGE_INTEGER totalBytes;
			GetDiskFreeSpaceEx(Util::StringOp::ToCString(path), &freeBytes, &totalBytes, NULL);
			
			int nProcess = (totalBytes.QuadPart*10/9-freeBytes.QuadPart)*100/(totalBytes.QuadPart*10/9);

			char txt[64];
			sprintf(txt, ".\\adv\\mjpg\\k1\\common\\�ٷֱ�%d.bmp", nProcess/10);
			m_MJPGList.SetUnitBitmap(23, txt, txt, TRUE);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CMainDlg::OnChangeWindow(WPARAM w, LPARAM l)
{
	if(l == SW_HIDE)
	{
		if((CWnd *)w != m_mainmenuDlg_)
		{
			if(w != 0xFFFFFFFF)
				((CWnd *)w)->ShowWindow(SW_HIDE);
			m_mainmenuDlg_->SetVideo();
			if((CWnd *)w != (CWnd *)m_mainScreenSaveDlg_ || (m_currentWnd == m_mainmenuDlg_ && IsWindowVisible()))
			{
				if((CWnd *)w == (CWnd *)m_mainScreenSaveDlg_)
					m_mainmenuDlg_->ShowWindow_(SW_SHOW, 1);
				else
					m_mainmenuDlg_->ShowWindow_(SW_SHOW);
		//		m_currentWnd = (CWnd *)m_mainmenuDlg_ ;
			}
			else if((CWnd *)w == (CWnd *)m_mainScreenSaveDlg_ && m_currentWnd == m_mainPhotoDlg_ )
			{
				//��playDlg���ó�photoDlg
				((CMultimediaPhoneDlg *)GetParent())->playerimage_->SetOwner(m_mainPhotoDlg_->playerDlg_);
			}
			else if((CWnd *)w == (CWnd *)m_mainScreenSaveDlg_)   //���ص�������
			{
				m_mainScreenSaveDlg_->ShowWindow(SW_HIDE);
			//	return;
			}

			//lxz 0531
	    	m_mainmenuDlg_->ShowWindow_(SW_HIDE);

			m_mainVideoDlg_->playerDlg_->ShowWindow(SW_HIDE);
			m_mainVideoDlg_->ShowWindow(SW_HIDE);
			m_mainPhotoDlg_->playerDlg_->StopTimer();
			m_mainPhotoDlg_->playerDlg_->ShowWindow(SW_HIDE);
		    m_mainPhotoDlg_->ShowWindow(SW_HIDE);
			m_mainMp3Dlg_->ShowWindow(SW_HIDE);
			m_mainLunarderDlg_->ShowWindow(SW_HIDE);
			m_mainScreenSaveDlg_->ShowWindow(SW_HIDE);
			m_mainCalucaterDlg_->ShowWindow(SW_HIDE);
			m_pWebDialog->ShowWindow_(SW_HIDE);	
			m_MJPGList.ShowWindow(SW_SHOW);
			
			if((CWnd *)w != (CWnd *)m_mainScreenSaveDlg_ && (w != 6))    //��������ˢ�£��±ߵ�״̬��
				SetStatusAll(FALSE);
		}
	}
	else if(l == SW_SHOW)
	{
		/*
		if(m_currentWnd == m_mainVideoDlg_)
			m_mainVideoDlg_->OnExit_();
		else if(m_currentWnd != m_mainScreenSaveDlg_ )
			m_currentWnd->ShowWindow(SW_HIDE);
		*/
		m_mainmenuDlg_->ShowWindow_(SW_HIDE);
		if(w == 0)	//��ʾ����ӰԺ
		{
			m_mainVideoDlg_->SetVideo("/flashdrv/my_video/playlist.pls");
			m_mainVideoDlg_->ShowWindow(SW_SHOW);
			m_currentWnd = (CWnd *)m_mainVideoDlg_ ;
		}
		else if(w == 1)	//��ʾPhoto
		{
			m_mainPhotoDlg_->SetPhoto();
			m_mainPhotoDlg_->ShowWindow(SW_SHOW);
			m_currentWnd = (CWnd *)m_mainPhotoDlg_ ;
		}
		else if(w == 2)	//��ʾMp3
		{
			m_mainMp3Dlg_->SetMP3("");
			m_mainMp3Dlg_->ShowWindow(SW_SHOW);
			m_currentWnd = (CWnd *)m_mainMp3Dlg_ ;
		}
		else if(w == 3)		//��ʾ����
		{
			m_mainLunarderDlg_->ShowWindow(SW_SHOW);
			m_currentWnd = (CWnd *)m_mainLunarderDlg_ ;
		}
		else if(w == 4)
		{
			m_mainCalucaterDlg_->ShowWindow(SW_SHOW);
			m_currentWnd = (CWnd *)m_mainCalucaterDlg_ ;
		}
		else if(w == 5)	//��ʾ����
		{
			//0530 lxz
		//	m_pWebDialog->SetPlayList(_T("\\storagecard\\"), 1);
		//	m_pWebDialog->ShowWindow(SW_SHOW);
		//	m_mainScreenSaveDlg_->ShowWindow(SW_SHOW);
		//	StartWeb();
		}
		else if(w == 6) //��ʾ����
		{
			//lxz 20090207
			if(m_currentWnd == m_mainVideoDlg_)
				m_mainVideoDlg_->OnExit_();
			else if(m_currentWnd == m_mainPhotoDlg_)
				m_mainPhotoDlg_->OnExit_();

			m_mainScreenSaveDlg_->ShowWindow(SW_SHOW);
		}
	}
}

void CMainDlg::OnPlayPhoto(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	if(m_mainPhotoDlg_->IsWindowVisible())
	{	
	//	if(!main->playerimage_->type_)		//ͼƬ
		{
			if(w == 2)	//ȫ������
			{
				if(main->playerimage_->isPlaying_ )
				{
					m_mainPhotoDlg_->SetAllScreenPlayer(TRUE);
				}
			}
			else if(w == 3) //�ָ�����
			{
				if(main->playerimage_->isPlaying_)
				{
					m_mainPhotoDlg_->SetAllScreenPlayer(FALSE);
				}
			}
		}
	}
	else if(m_mainScreenSaveDlg_->IsWindowVisible())
	{
		if(w == 3)	//�˳�����
		{
			if(main->playerimage_->isPlaying_ )
			{
				m_mainScreenSaveDlg_->OnExit();
			}
		}
	}
}

void CMainDlg::SetTimer_(BOOL flag)
{
	if(flag)
	{
		//������ʱ��
		SetTimer(1, 1000, NULL);
		SetTimer(2, 5000, NULL);
	}
	else
	{
		KillTimer(1);
		KillTimer(2);
	}
}
void CMainDlg::OnPlayVideo(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	{

		if(w == 0  )	//��ͣ��Ƶ
		{
			//��Ƶ��ͣ
			if(main->playervideo_->isPlaying_)
				m_mainVideoDlg_->SendMessage(WM_OUTEVENT, 0, 0);
			if(main->playerimage_->isPlaying_)
				m_mainPhotoDlg_->SendMessage(WM_OUTEVENT, 0, 0);
			if(m_mainPhotoDlg_->IsWindowVisible())
				m_mainPhotoDlg_->SendMessage(WM_OUTEVENT, 0, 0);
		}
		else if(w ==  1)//������Ƶ
		{
			{
				if(m_mainMp3Dlg_->IsWindowVisible())
				{
					//main->playeraudio_->InitPlayer();
				}
				else if(m_mainVideoDlg_->IsWindowVisible())
				{
					m_mainVideoDlg_->SendMessage(WM_OUTEVENT, 0, 1);						
				}
				else if(m_mainPhotoDlg_->IsWindowVisible())
				{
		//			m_mainPhotoDlg_->SendMessage(WM_OUTEVENT, 0, 1);						
				}
			}
		}
		else if(w == 2)	//ȫ������
		{
			if(main->playervideo_->isPlaying_)
			{
				main->playervideo_->SetAllScreenPlayer(TRUE);
			}
			if(main->playerimage_->isPlaying_)
				main->playerimage_->SetAllScreenPlayer(TRUE);
		}
		else if(w == 3)	//�˳�ȫ��
		{
			if(m_mainScreenSaveDlg_->playerDlg_->player_->owner_ == m_mainScreenSaveDlg_->playerDlg_ && m_mainScreenSaveDlg_->playerDlg_->player_->isPlaying_)
				m_mainScreenSaveDlg_->SendMessage(WM_OUTEVENT, 0, 0);
			if(main->playervideo_->isPlaying_)
			{
				main->playervideo_->SetAllScreenPlayer(FALSE);
			}
			if(m_mainPhotoDlg_->playerDlg_->player_->owner_ == m_mainPhotoDlg_->playerDlg_  && main->playerimage_->isPlaying_)
				m_mainPhotoDlg_->SetAllScreenPlayer(FALSE);
		}
		else if(w == 4 ) //�ļ���ʼ����
		{
			//������
			/*
			if(m_mainVideoDlg_->IsWindowVisible() )//|| m_mainVideoDlg_->IsWindowVisible())	//��Ƶ
			{
				m_mainVideoDlg_->StartOpenNewFile();
			}
			else if(m_mainMp3Dlg_->IsWindowVisible())
			{
				m_mainMp3Dlg_->StartOpenNewFile();
			}
			*/
		}
	}
}

void CMainDlg::OnStaticClick(WPARAM w, LPARAM l)
{
	if(w == IDC_STATIC_CALLWALL)		//����ǽ
	{
		m_firewalDlg_->OnStart();
		m_firewalDlg_->ShowWindow(TRUE);
	}
	else if(w == IDC_STATIC_TELSTATUS)	//δ�ӵ绰�����Ե绰
	{
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
		//todo: ����δ���绰��¼
		main->m_pContactInfoDlg->ShowUnconnectItems();
		main->m_pContactInfoDlg->ShowWindow(SW_SHOW);
		//lxz 20080617
	//	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->ClearUnconnectCount();
	}
	else if(w == IDC_STATIC_NOTETATUS)		//�����Ƿ��м���
	{	
		SYSTEMTIME curtime;
		GetLocalTime(&curtime);
//		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_mainLunarderDlg1_->SetIn();		//�Ƿ���Ҫ��ͣ��Ƶ����
//		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_mainLunarderDlg1_->SetData(curtime.wYear, curtime.wMonth, curtime.wDay);
//		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAINNOTE);
	}
}

void CMainDlg::OnShowNoteStatic()
{
	/*
	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	CString s;
	if(!m_mainLunarderDlg_->m_lunarder.IsHaveNote(curtime.wYear, curtime.wMonth, curtime.wDay, s))
	{
		s = Data::LanguageResource::Get(Data::RI_MAIN_NONOTE).c_str(); 
	}
	m_TodayNoteStatic.SetWindowText(s);
	*/
}

void CMainDlg::OnShowCallWallStatic()
{
	/*
	CString s;
	int type = m_firewalDlg_->GetFirewallType();
	if(type == -1)
	{
		s = Data::LanguageResource::Get(Data::RI_MAIN_PROTECTEDWALL_CLOSE_STC).c_str();
	}
	else
	{
		char txt[128];
		const char *strtemp[] = {\
		Data::LanguageResource::Get(Data::RI_FIREWALL_BLACKLIST).c_str(),\
		Data::LanguageResource::Get(Data::RI_FIREWALL_CONTACT).c_str(),\
		Data::LanguageResource::Get(Data::RI_FIREWALL_VIP).c_str(),\
		Data::LanguageResource::Get(Data::RI_FIREWALL_ALL).c_str()\
		};
		//sprintf(txt, "%s(%s)", Data::LanguageResource::Get(Data::RI_MAIN_PROTECTEDWALL_OPEN_STC).c_str(), strtemp[type]);
		sprintf(txt, "%s", strtemp[type]);
		s = txt;
	}
	m_CallWallStatic.SetWindowText(s);
	*/
}

void CMainDlg::OnShowTelStatusStatic(WPARAM wParam, LPARAM lParam)
{
	//todo: ��ȡδ�ӵ绰����
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	if(wParam == 0)
	{
		m_nUnTel = main->m_pTelephoneDlg->GetUnconnectCount();
		m_nLeaveSound = main->m_pTelephoneDlg->GetRecordCount();
	}
	else if(wParam == 1)    //ͨ����¼
	{
		if(lParam == 0)     //���
		{
			m_nUnTel = 0;
		}
		else if(lParam == 1)   //����һ��
		{
			m_nUnTel++;
		}
		else if(lParam == 2)   //����һ��
		{
			m_nUnTel--;
			if(m_nUnTel < 0)
				m_nUnTel = 0;
		}
	}
	else if(wParam == 2)	//������¼
	{
		if(lParam == 0)     //���
		{
			m_nLeaveSound = 0;	
		}
		else if(lParam == 1)   //����һ��
		{
			m_nLeaveSound++;
		}
		else if(lParam == 2)   //����һ��
		{
			m_nLeaveSound--;
			if(m_nLeaveSound < 0)
				m_nLeaveSound = 0;
		}
	}
	
	CString s;
	s.Format(_T("%d"), m_nLeaveSound);
	m_MJPGList.SetUnitText(302, s, TRUE);
	s.Format(_T("%d"), m_nUnTel);
	m_MJPGList.SetUnitText(303, s, TRUE);

// 	CString sTel = "";
// 	CString sRecorde = "";
// 	CString sAlarm = "";
// 	CString sFireWall = "";
// 	
// 	if(m_nUnTel > 0)
// 		sTel = ".\\adv\\mjpg\\k1\\common\\δ�ӵ绰.bmp";
// 	if(m_nLeaveSound > 0)
// 		sRecorde = ".\\adv\\mjpg\\k1\\common\\δ��¼��.bmp";
	
//	m_MJPGList.SetUnitBitmap(9, sTel, "", TRUE);
//	m_MJPGList.SetUnitBitmap(8, sRecorde, "", TRUE);

}

LRESULT CMainDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
	case WM_TELNOTIFY:
		OnShowCallWallStatic();
		OnShowNoteStatic();
		OnShowTelStatusStatic(wParam, lParam);
		break;
	case WM_FIRENOTIFY:
		OnShowCallWallStatic();
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CMainDlg::ShowWindow_(int nCmdShow)
{
	SetTimer_(FALSE);

	//�����������л� main����ʾbug
	if(m_currentWnd == m_mainmenuDlg_ /* && nCmdShow == SW_SHOW*/)
	{
//		m_mainmenuDlg_->ShowWindow_(nCmdShow);
	}
	else if(m_currentWnd == m_mainVideoDlg_)
	{
		HWND hWnd = ::FindWindow(L"csplayer_win0", L"csplayer window0");
		BOOL flag2 = ::IsWindowVisible(hWnd);
		if(flag2 && nCmdShow == SW_SHOW)
			m_mainVideoDlg_->playerDlg_->ShowWindow(nCmdShow);
		else if(nCmdShow == SW_HIDE)
			m_mainVideoDlg_->playerDlg_->ShowWindow(nCmdShow);
		m_mainVideoDlg_->ShowWindow(nCmdShow);
	}
	else if(m_currentWnd == m_mainPhotoDlg_)
	{
		m_mainPhotoDlg_->playerDlg_->ShowWindow(nCmdShow);
		m_mainPhotoDlg_->ShowWindow(nCmdShow);
	}

	DWORD dwStart   =   GetTickCount();
		
	ShowWindow(nCmdShow);

	DWORD offset = GetTickCount() - dwStart; 
	Dprintf("Main show %d\n", offset);

	//��ʾ MJPG
	if(nCmdShow == SW_SHOW)
	{
		m_mainmenuDlg_->ShowWindow_(SW_HIDE);
		m_mainVideoDlg_->ShowWindow(SW_HIDE);
		m_mainPhotoDlg_->playerDlg_->ShowWindow(SW_HIDE);
		m_mainPhotoDlg_->ShowWindow(SW_HIDE);
		m_mainMp3Dlg_->ShowWindow(SW_HIDE);
		m_mainLunarderDlg_->ShowWindow(SW_HIDE);
		m_mainScreenSaveDlg_->ShowWindow(SW_HIDE);
		m_mainCalucaterDlg_->ShowWindow(SW_HIDE);
		m_pWebDialog->ShowWindow_(SW_HIDE);	
		((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pNotebookDlg->ShowWindow(SW_HIDE);
		m_MJPGList.ShowWindow(SW_SHOW);
	}
	SetTimer_(TRUE);
}

void CMainDlg::SetMainMenu()
{
	((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pNotebookDlg->ShowWindow(SW_HIDE);
	m_firewalDlg_->ShowWindow(SW_HIDE);
	m_pHuangLiDlg_->ShowWindow(SW_HIDE);
	if(m_currentWnd == m_mainVideoDlg_)
		m_mainVideoDlg_->OnExit_();
	else if(m_currentWnd == m_mainMp3Dlg_)
		m_mainMp3Dlg_->OnExit_(FALSE);
	else if(m_currentWnd == m_mainPhotoDlg_)
		m_mainPhotoDlg_->OnExit_();
	else if(m_currentWnd == m_mainLunarderDlg_)
	{
		m_mainLunarderDlg_->OnExit_();
		ShowWindow(SW_SHOW);
	}
	else if(m_currentWnd == m_mainCalucaterDlg_)
		m_mainCalucaterDlg_->OnExit_();
//	else if(m_currentWnd == m_pWebDialog)
//	{
//		m_pWebDialog->OnButtonClose();
//	}
	else if(m_MJPGList.IsWindowVisible())
	{
	//	CString s = ".\\adv\\mjpg\\k1\\����\\����.xml";
	//	m_MJPGList.SetCurrentLinkFile(s);
		SetDateTime(FALSE);
		Invalidate();
		//m_MJPGList.DrawMJPGPage_HDC(s);
	}
}

void CMainDlg::OnClickMJPGToApp(WPARAM w, LPARAM l)
{
	/*
	void OnVideoBtn();
	void OnPhotoBtn();
	void OnMp3Btn();
	void OnScreenSaveBtn();
	void OnCalculBtn();
	void OnLunarderBtn();
	*/
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
//	m_MJPGList.ShowWindow(SW_HIDE);   //lxz 20080602
	switch (w)
	{
	case 1:					//��ǩ
		{
		//m_mainmenuDlg_->OnMp3Btn();
		((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pNotebookDlg->ShowWindow(SW_SHOW);
		}
		break;
	case 2:					//��������						
	//	m_mainmenuDlg_->OnVideoBtn();
		{
			SYSTEMTIME curtime;
			GetLocalTime(&curtime);
			m_pHuangLiDlg_->SetData(curtime.wYear, curtime.wMonth, curtime.wDay, TRUE);
			m_pHuangLiDlg_->ShowWindow(SW_SHOW);    //�ϻ���
		}
		break;
	case 3:
		m_mainPhotoDlg_->OnOpenFile();
		m_mainmenuDlg_->OnPhotoBtn();
		break;
	case 4:			//����ǽ
		if(!((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isAdmin() || ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_bLogin)
		{
			m_firewalDlg_->SetParameters(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall(),
				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType(),
				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->blockAllTimeDuration().GetTotalSeconds());
			
			m_firewalDlg_->ShowWindow(SW_SHOW);
		}
		else
		{
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pWarningNoFlashDlg->SetType(4);
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		}
		break;
	case 5:							
		m_mainmenuDlg_->OnLunarderBtn();    //�ճ�
		break;
	case 6:								//
		m_mainmenuDlg_->OnCalculBtn();
		break;
	case 7:								
		if(m_MJPGList.GetUnitIsDownStatus(170))   //ע������Ա
		{
			std::string strTemp = ".\\adv\\mjpg\\k1\\common\\ȡ������Ա��¼.bmp";
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
		}
		else									 //����Ա��¼
		{
			if(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isAdmin())
			{
				((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->m_passwordDlg->SetType(CHECK_SUPPERPASSWORD);
				std::string strTemp = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->adminPassword();
				((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->m_passwordDlg->SetOldPassWord((char *)strTemp.c_str());
				((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->m_passwordDlg->SetHWnd(this->m_hWnd);
				((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->m_passwordDlg->ShowWindow_(SW_SHOW);	
			}
			else
			{
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->SetType(5);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
			}
		}
		break;
	case 8:									//���
		{
			//((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pNotebookDlg->ShowWindow(SW_SHOW);
			main->OnMainSoftKey(5, 0);
		}
		break;
	case 9:								
	case 10:										//��ʾδ�ӵ绰
		{
			CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
			//todo: ����δ���绰��¼
			main->m_pContactInfoDlg->ShowUnconnectItems();
			main->m_pContactInfoDlg->ShowWindow(SW_SHOW);
		}
		break;
	case 11:										//��ʾ¼���绰
		{
			CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
			main->m_pSoundDlg->ShowItemsInList(0);
			main->OnMainSoftKey(7, 0);
		}
		break;
	case 30:		//����
		{
			if(m_bIsAlarm)
			{
				SetMainMenu();
				ShowTodayAlarm();
			}
			break;
		}
	case 31:	//����ǽ
		{
			if(m_bIsFireWall)
			{
				SetMainMenu();
				OnClickMJPGToApp(4, 0);
			}
			break;
		}
	default:
		break;
	}

	if(w >= 1 && w <= 7)
		SetStatusAll(TRUE);

	m_MJPGList.m_nIndexSelectUnit = -1;
}

void CMainDlg::ShowRightCtrl(int nCmdShow)
{
	/*
	m_TimeStatic.ShowWindow(nCmdShow);
	m_TodayNoteStatic.ShowWindow(nCmdShow);
	m_CallWallStatic.ShowWindow(nCmdShow);
	m_TelStatusStatic.ShowWindow(nCmdShow);
	*/
}

void CMainDlg::OnMJPGShowHalf(WPARAM w, LPARAM l)
{
	if(w == 0)
		ShowRightCtrl(SW_HIDE);
	else
		ShowRightCtrl(SW_SHOW);

}

void CMainDlg::OnSetAdmin(WPARAM w, LPARAM l)
{
	if(w == 1)
	{
		m_MJPGList.SetUnitIsDownStatus(170, TRUE);
		SetStatusAll(FALSE);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin = TRUE;
	}
}

void CMainDlg::OnUnSetAdmin(WPARAM w, LPARAM l)
{
	m_MJPGList.SetUnitIsDownStatus(170, FALSE);
	SetStatusAll(FALSE);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin = FALSE;
}

void  CMainDlg::StartWeb()
{
	MEMORYSTATUS   mem;     
	mem.dwLength=sizeof(mem);     
	GlobalMemoryStatus(&mem);     
//     if( mem.dwAvailPhys < 32*1024*1024)
// 	{
// 		return;
// 	}

	if(m_pWebDialog)
	{
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
		main->CancelBalckLightSaveTimer();
		m_pWebDialog->ShowWindow_(SW_SHOW);
	}

// 	PROCESS_INFORMATION processInfo;
// 	STARTUPINFO lpStartupInfo; // �����ڴ����ӽ���ʱ���ø������� 
// 	memset(&lpStartupInfo, 0, sizeof(lpStartupInfo));
// 
// 	lpStartupInfo.cb = sizeof(lpStartupInfo);
// 	lpStartupInfo.dwX = 0;
// 	lpStartupInfo.dwY = 0;
// 	lpStartupInfo.dwXSize = 800;
// 	lpStartupInfo.dwYSize = 420;
// 	lpStartupInfo.wShowWindow= SW_MINIMIZE; 
// //	lpStartupInfo.dwFlags= 1|2|4;
// 	
// 
// //	memset(&processInfo, 0, sizeof(processInfo));
// 	if (!CreateProcess(L"\\windows\\iesample.exe", L"http://www.google.com", NULL, NULL, NULL, CREATE_NEW_CONSOLE, NULL, NULL, &lpStartupInfo, &processInfo))
// 	{
// 	//	GetWindowThreadProcessId
// 	//	::FindWindow()
// 	}
// 	HWND hwnd = ::FindWindow(NULL, L"���ڱ���");
// //	HWND hwnd = ::GetWin(processInfo.dwProcessId);
//   //  Sleep(1000); 
//     ::MoveWindow(hwnd, 0,  0,   800,   420,   TRUE); 
// 
// 
// 	CloseHandle(processInfo.hThread);
// 	CloseHandle(processInfo.hProcess);
// 	return;

	/*

	PROCESS_INFORMATION processInfo;
// 	CString exeFilename = "\\windows\\presviewer.exe";
// 	CString filename = "\\storagecard\\����.ppt";
	CString exeFilename = "\\windows\\docviewer.exe";
	CString filename = "\\storagecard\\�ϻ�������.doc";

	STARTUPINFO lpStartupInfo; // �����ڴ����ӽ���ʱ���ø������� 
	memset(&lpStartupInfo, 0, sizeof(lpStartupInfo));
	lpStartupInfo.cb = sizeof(lpStartupInfo);
	lpStartupInfo.dwX = 20;
	lpStartupInfo.dwY = 20;
	lpStartupInfo.dwXSize = 200;
	lpStartupInfo.dwYSize = 200;
	lpStartupInfo.wShowWindow=SW_SHOW; 
	lpStartupInfo.dwFlags= 1|2|4;
	
	if (!CreateProcess(exeFilename, filename, NULL, NULL, NULL, CREATE_NEW_CONSOLE, NULL, NULL, &lpStartupInfo, &processInfo))
	{
		printf("Ӧ�ó�������ʧ�ܣ���ԭ�¸��������Ĳ��㣬�뾡����ϵ�豸��Ӧ��!\n");
		//::MessageBox(NULL, _T("��������ʧ�ܣ����Ժ����ԡ���"), _T("��������ʧ��"), 0);
	}
	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);
	*/
}
