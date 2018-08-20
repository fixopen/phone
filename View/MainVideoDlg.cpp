// MainVideoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainVideoDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../MultimediaPhoneDlg.h"
#include "../MultimediaPhone.h"
#include "player.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int gVideo3PuaseCount = 0;
extern int gPlayIndex;
extern BOOL DetectDIR(TCHAR *sDir);
//#define IDT_GETINFO_TIIMER	8000
/////////////////////////////////////////////////////////////////////////////
// CMainVideoDlg dialog
//#define WM_CHANGEWINDOW		8001
//#define WM_PLAYVIDEO		8002    //WPARAM 0 暂停播放 1 恢复播放 2 全屏播放 3 退出全屏
CMainVideoDlg::CMainVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainVideoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainVideoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	memset(m_chDir, 0, 128*2);
	m_IsPlay = 0;
	m_ClickType = 0;
	m_IsSound = TRUE;
	m_Volume = SOUND_DEFAULT_VOLUME;
	m_bIsPausebyEvent = FALSE;
	m_pageSize = 5;
}


void CMainVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainVideoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainVideoDlg, CDialog)
	//{{AFX_MSG_MAP(CMainVideoDlg)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOEXIT, OnExit)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOPLAY, OnPlayer)
//	ON_BN_CLICKED(IDC_BUTTON_VIDEOSTOP, OnStop)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOPRE, OnPre)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOBACK, OnBack)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOOPEN, OnOpenFile)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOPLAYALL, OnPlayerAll)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOMUTE, OnMute)
	ON_MESSAGE(WM_PROCESS_POS, OnPregress)
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	ON_MESSAGE(WM_OUTEVENT, OnOutEvent)
	ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMainVideoDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == 'U')
	{
		OnPre();
	}
	else if(nChar == 'D')
	{
		OnBack();
	}
}

/////////////////////////////////////////////////////////////////////////////
void CMainVideoDlg::SetVolume()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int index = main->m_pSettingDlg->m_pTempSetting->sysVolume();
	int Volume[] = {10, 8, 6, 4, 2};
	main->playervideo_->SetVolume(Volume[index]);
	m_Volume = Volume[index];
}

BOOL CMainVideoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_MoveText = new CCEMoveTxtStatic();
	m_MoveText->Create(L"", RGB(0,0,0), WS_CHILD|WS_VISIBLE, CRect(36,22,474,52), this);
	m_MoveText->SetTxt(L"", 22, RGB(0,0,0));
	m_MoveText->MoveWindow(CRect(36,22,474,52));

	m_prgPlayTime.Create(WS_CHILD|WS_VISIBLE, CRect(38, 327, 474, 327+9), this, IDC_PROGRESS_VIDEOTIME);
	m_prgPlayTime.SetParam(0, 0, 10, 1, 1, IDB_PROGRESS_TIME1, IDB_PROGRESS_TIME2);
	m_prgPlayTime.SetPos(0, FALSE);
	m_prgPlayTime.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	//播放器
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	playerDlg_ = new CPlayerDlg(main->playervideo_);
	playerDlg_->Create(CPlayerDlg::IDD, this);
	playerDlg_->ReSetWindowsRect(CRect(36, 52, 474, 320));

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\视频播放.xml");

	MoveWindow(0, 57, 800, 423);
	m_MJPGList.SetUnitColor(0, font_white, TRUE);
	m_MJPGList.SetUnitColor(40, font_white, TRUE);
	m_MJPGList.SetUnitColor(41, font_white, TRUE);
	m_MJPGList.SetUnitFont(40, font_16);
	m_MJPGList.SetUnitFont(41, font_16);
	for(int i=21; i<=25; i++)
	{
		m_MJPGList.SetUnitFont(i, font_18);
	}
	m_MJPGList.SetUnitIsShow(50, TRUE);
	ChangeVolume(m_Volume+50);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->playervideo_->SetVolume(m_Volume);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainVideoDlg::SetVideo(CString filename)
{
	char filename_[128];
	std::string str = Util::StringOp::FromCString(filename);
	sprintf(filename_, "%s", str.c_str());
	playerDlg_->SetParam(filename_, mtVideo);
	playerDlg_->ReSetWindowsRect(CRect(36, 109, 474, 377));
	
	if ((gPlayIndex > 0) && (0 == gPlayIndex%m_pageSize))
	{
		m_MJPGList.SetUnitColor(25, font_blue, TRUE);
	}
	else
	{
		m_MJPGList.SetUnitColor((gPlayIndex%m_pageSize)+20, font_blue, TRUE);
	}
}

void CMainVideoDlg::OnExit()
{
	m_bIsPausebyEvent = FALSE;
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->playervideo_->ExitPlayer();
	KillTimer(IDT_GETINFO_TIIMER);
	
	m_MoveText->KillTimer(0xFFF0);
	m_MoveText->m_nCount = 0;
	m_MoveText->SetTxt(L"", 22, RGB(255,255,255));
	
	m_prgPlayTime.SetPos(0);
	m_bIsPausebyEvent = FALSE; 
	m_IsPlay = 0;
	gPlayIndex = 0;
	playerDlg_->ShowWindow(SW_HIDE);
	m_MJPGList.SetUnitIsDownStatus(3, FALSE);
	m_MJPGList.SetUnitIsShow(3, TRUE);
	ClearAll();
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
}

//播放
void CMainVideoDlg::OnPlayer()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(main->m_pSoundDlg->m_pRecordSoundDlg->m_bISRecording || main->m_pTelephoneDlg->m_bRecording)
		return;

	if(main->m_pTelephoneDlg->GetIsRecordStatus())
		return;
	
	if(m_IsPlay == 0)
	{
		
		char *buffer = new char[1024*10];
		memset(buffer, 0, 1024*10);
		int nFileSelected = m_MovieList.size();
		
		char txt[128];
		strcpy(txt, "[Playlist]\r\n");
		strcpy(buffer, txt);
		for (int i=0; i<m_MovieList.size(); i++)
		{
			std::string str = Util::StringOp::FromCString(m_MovieList[i]);
			sprintf(txt, "File%d=%s\r\nLength%d=255\r\n", i, str.c_str(), i);
			if(strlen(buffer) < (1024*8))
				strcat(buffer, txt);
		}
		sprintf(txt, "NumberOfEntries=%d\r\nVersion=2\r\n", nFileSelected);
		strcat(buffer, txt);
		
		if(nFileSelected != 0)
		{
			FILE *file;
			file = fopen("/flashdrv/my_video/playlist.pls", "w+b");
			if(file)
			{
				fwrite(buffer, sizeof(char), strlen(buffer), file);
				fclose(file);
			}
			//调用播放窗口
			if(playerDlg_)
			{
				m_MoveText->KillTimer(0xFFF0);
				m_MoveText->SetTxt(L"", 22, RGB(255,255,255));
				m_MoveText->m_nCount = 0;
				SetVideo(m_MovieList[gPlayIndex-1]);
				playerDlg_->Show();
				m_MoveText->SetTxt(m_ShowList[gPlayIndex-1], 22, RGB(255,255,255));
				m_MoveText->SetTimer(0xFFF0, 100, NULL);
				SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
			}
			delete []buffer;
			m_IsPlay = 1;
			m_MJPGList.SetUnitIsDownStatus(3, TRUE);
			m_MJPGList.SetUnitIsShow(3, TRUE);

			if(0 == main->phone_->m_BatteryStatus.batteryType)
			{
				main->phone_->m_BatteryStatus.typeChange = 1;
				::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_CHANGE_DCBATTERY, 0, 0);
			}
		}
	}
	else if(m_IsPlay == 1)
	{
		m_MJPGList.SetUnitIsDownStatus(3, FALSE);
		m_MJPGList.SetUnitIsShow(3, TRUE);
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->playervideo_->PausePlayer(TRUE);
		m_IsPlay = 2;
		KillTimer(IDT_GETINFO_TIIMER);
		m_MoveText->KillTimer(0xFFF0);
	}
	else if(m_IsPlay == 2)
	{
		if(m_bIsPausebyEvent)
		{
			Dprintf("Video resume!\r\n");
			
			CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
			m_bIsPausebyEvent = FALSE;
			main->playervideo_->SetOwner(playerDlg_);
			main->playervideo_->InitPlayer();
			
			main->playervideo_->isActiveMode_ = 1;
			main->playervideo_->ResumePlayer();
			gVideo3PuaseCount = 0;
		}
		
		m_MJPGList.SetUnitIsDownStatus(3, TRUE);
		m_MJPGList.SetUnitIsShow(3, TRUE);
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->playervideo_->PausePlayer(FALSE);
		m_IsPlay = 1;
		SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
		m_MoveText->SetTimer(0xFFF0, 100, NULL);
	}
}

void CMainVideoDlg::OnStop()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (1 == m_IsPlay||2 == m_IsPlay)
	{
		m_prgPlayTime.SetPos(0);
		m_IsPlay = 0;
		m_bIsPausebyEvent = TRUE;
		m_MJPGList.SetUnitIsDownStatus(3, FALSE);
		m_MJPGList.SetUnitIsShow(3, TRUE);
		m_MJPGList.SetUnitText(8, L"0:00/0:00", TRUE);
		main->playervideo_->StopPlayer();
		KillTimer(IDT_GETINFO_TIIMER);
		m_MoveText->KillTimer(0xFFF0);
		m_MoveText->SetTxt(L"", 22, RGB(255,255,255));
		m_MoveText->m_nCount = 0;
	}
}

//前一条
void CMainVideoDlg::OnPre()
{
	if(m_IsPlay == 2 || ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->m_pRecordSoundDlg->m_bISRecording || ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->m_bRecording)
		return;
	KillTimer(IDT_GETINFO_TIIMER);
	m_MoveText->KillTimer(0xFFF0);
	m_MoveText->SetTxt(L"", 22, RGB(255,255,255));
	m_MoveText->m_nCount = 0;
	
	m_MJPGList.SetUnitColor(((gPlayIndex)%m_pageSize)+20, font_red, TRUE);
	m_MJPGList.SetUnitText(((gPlayIndex)%m_pageSize)+20, m_ShowList[gPlayIndex-1], TRUE);
	gPlayIndex--;
	
	if(0 == gPlayIndex)
	{
		OnLast();
	}
	else if (0 == gPlayIndex%m_pageSize)
	{
		PageUp();
	}
	SetVideo(m_MovieList[gPlayIndex-1]);
	playerDlg_->Show();
	m_IsPlay = 1;
	m_MoveText->SetTxt(m_ShowList[gPlayIndex-1], 22, RGB(255,255,255));
	SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
	m_MoveText->SetTimer(0xFFF0, 100, NULL);
}

//后一条
void CMainVideoDlg::OnBack()
{
	if(m_IsPlay == 2 || ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->m_pRecordSoundDlg->m_bISRecording || ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->m_bRecording)
		return;
	KillTimer(IDT_GETINFO_TIIMER);
	m_MoveText->KillTimer(0xFFF0);
	m_MoveText->SetTxt(L"", 22, RGB(255,255,255));
	m_MoveText->m_nCount = 0;
	
	m_MJPGList.SetUnitColor(((gPlayIndex)%m_pageSize)+20, font_red, TRUE);
	m_MJPGList.SetUnitText(((gPlayIndex)%m_pageSize)+20, m_ShowList[gPlayIndex-1], TRUE);
	gPlayIndex++;
	
	if ((gPlayIndex > 1) && (1 == gPlayIndex%m_pageSize))
	{
		PageDown();
	}
	if(m_MovieList.size() < gPlayIndex)
	{
		OnFirst();
	}
	
	SetVideo(m_MovieList[gPlayIndex-1]);
	playerDlg_->Show();
	m_IsPlay = 1;
	m_MoveText->SetTxt(m_ShowList[gPlayIndex-1], 22, RGB(255,255,255));
	SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
	m_MoveText->SetTimer(0xFFF0, 100, NULL);
}

void CMainVideoDlg::OnFirst()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	KillTimer(IDT_GETINFO_TIIMER);
	m_MoveText->KillTimer(0xFFF0);
	m_MoveText->SetTxt(L"", 22, RGB(255,255,255));
	m_MoveText->m_nCount = 0;
	
	main->playervideo_->First();
	m_selectCurrentPage = 1;
	CString str;
	str.Format(L"%d", m_selectCurrentPage);
	m_MJPGList.SetUnitText(40, str, TRUE);
	
	gPlayIndex = 1;
	m_IsPlay = 1;
	m_MoveText->SetTxt(m_ShowList[gPlayIndex-1], 22, RGB(255,255,255));
	m_MJPGList.SetUnitIsDisable(9, TRUE);
	if (m_selectCurrentPage < m_selectPageCount)
	{
		m_MJPGList.SetUnitIsDisable(10, FALSE);
	}
	ShowArrayInList(m_ShowList);
	m_MoveText->SetTimer(0xFFF0, 100, NULL);
	SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
}

void CMainVideoDlg::OnLast()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	KillTimer(IDT_GETINFO_TIIMER);
	m_MoveText->KillTimer(0xFFF0);
	m_MoveText->SetTxt(L"", 22, RGB(255,255,255));
	m_MoveText->m_nCount = 0;
	
	main->playervideo_->Last();
	m_selectCurrentPage = m_selectPageCount;
	CString str;
	str.Format(L"%d", m_selectCurrentPage);
	m_MJPGList.SetUnitText(40, str, TRUE);
	
	gPlayIndex = m_MovieList.size();
	m_IsPlay = 1;
	m_MoveText->SetTxt(m_ShowList[gPlayIndex-1], 22, RGB(255,255,255));
	m_MJPGList.SetUnitIsDisable(10, TRUE);
	if (m_selectCurrentPage > 1)
	{
		m_MJPGList.SetUnitIsDisable(9, FALSE);
	}
	ShowArrayInList(m_ShowList);
	m_MoveText->SetTimer(0xFFF0, 100, NULL);
	SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
}

//打开播放列表
void CMainVideoDlg::OnOpenFile()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	m_bIsPausebyEvent = FALSE;
	main->playervideo_->ExitPlayer();

	playerDlg_->ShowWindow(SW_HIDE);
	m_IsPlay = 0;
	KillTimer(IDT_GETINFO_TIIMER);
	m_prgPlayTime.SetPos(0);
}
//全屏播放
void CMainVideoDlg::OnPlayerAll()
{
	if(m_IsPlay == 2)
		return;
	GetParent()->SendMessage(WM_PLAYVIDEO, 2);	

	::SetCursorPos(799, 479);
}
//设置声音开关
void CMainVideoDlg::OnMute()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(m_IsSound)
	{
		//wangzhenxing1019
		ChangeVolume(0);
		main->playervideo_->SetVolume(0); 
		m_MJPGList.SetUnitIsDownStatus(50, TRUE);
		m_MJPGList.SetUnitIsShow(50, TRUE);
	}
	else
	{
		//wangzhenxing1019
		ChangeVolume(m_Volume+50);
		main->playervideo_->SetVolume(m_Volume);
		m_MJPGList.SetUnitIsDownStatus(50, FALSE);
		m_MJPGList.SetUnitIsShow(50, TRUE);
	}
	m_IsSound = !m_IsSound;
}

void CMainVideoDlg::ChangeVolume(int w)
{
	for (int i=1; i<=10; i++)
	{
		int Volume = i + 50;
		if (Volume <= w)
		{
			m_MJPGList.SetUnitIsDownStatus(Volume, TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDownStatus(Volume, FALSE);
		}
	}
	m_MJPGList.Invalidate();
}

void CMainVideoDlg::OnPregress(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	//设置声音
	if(l == IDC_PROGRESS_VIDEOSOUND)
	{
		if(m_IsSound)
			main->playervideo_->SetVolume(w);
		m_Volume = w;
	}
	//调整播放时间
	else if(l == IDC_PROGRESS_VIDEOTIME)
	{
		int percent = m_prgPlayTime.GetPercent();
		main->playervideo_->PlayPos(percent);	
	}
}

void CMainVideoDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (0x10 == nIDEvent)
	{
		KillTimer(0x10);
		if (0 == gPlayIndex%m_pageSize)
		{
			m_MJPGList.SetUnitIsDownStatus(25, FALSE);
			m_MJPGList.SetUnitIsShow(25, TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDownStatus((gPlayIndex%m_pageSize)+20, FALSE);
			m_MJPGList.SetUnitIsShow((gPlayIndex%m_pageSize)+20, TRUE);
		}	
	}
	
	if(IDT_GETINFO_TIIMER == nIDEvent && m_MJPGList.GetUnitIsDownStatus(3))
	{
		MEDIA_INFO info;
		main->playervideo_->GetVideoFileInfo(&info);
		
		int Hour,Min,Sec;
		int tick = info.nTick;
		Hour = tick / 3600 / TICKSPERSEC;
		tick -= Hour * 3600 * TICKSPERSEC;
		Min = tick / 60 / TICKSPERSEC;
		tick -= Min * 60 * TICKSPERSEC;
		Sec = tick / TICKSPERSEC;

		int Hour1,Min1,Sec1;
		tick = info.nDuration;
		Hour1 = tick / 3600 / TICKSPERSEC;
		tick -= Hour1 * 3600 * TICKSPERSEC;
		Min1 = tick / 60 / TICKSPERSEC;
		tick -= Min1 * 60 * TICKSPERSEC;
		Sec1 = tick / TICKSPERSEC;

		int n1 = Hour*3600+Min*60+Sec;
		int n2 = Hour1*3600+Min1*60+Sec1;
		
		m_prgPlayTime.SetParam(0, 0, n2, 1, 1, IDB_PROGRESS_TIME1, IDB_PROGRESS_TIME2);

		m_prgPlayTime.SetPos(n1);

		CString sFilename = info.szFileName;
		int nstart = 0;
		int noldstart = 0;
		nstart = sFilename.Find('/', noldstart);
		while(nstart >= 0)
		{
			noldstart = nstart+1;
			nstart = sFilename.Find('/', noldstart);
		}

		CString sFilename1 = sFilename.Mid(noldstart);
		static int gNReFresh = 0;
		if(gNReFresh++%3)
			sFilename1 = "";
		m_MJPGList.SetUnitText(0, sFilename1, TRUE);
		
		char txt[32];
		sprintf(txt, "%02d:%02d:%02d", Hour, Min, Sec);
		CString s = txt;
		m_MJPGList.SetUnitText(31, s, TRUE);
		sprintf(txt, "%02d:%02d:%02d", Hour1, Min1, Sec1);
		s = txt;
		m_MJPGList.SetUnitText(30, s, TRUE);

		if(((n1 >= (n2 - 3)) && n1 != 0) || n2 == 0)		//换到下一首
		{
			if(main->playervideo_->isPlaying_)
			{	
				m_MJPGList.SetUnitColor(((gPlayIndex)%m_pageSize)+20, font_red, TRUE);
				m_MJPGList.SetUnitText(((gPlayIndex)%m_pageSize)+20, m_ShowList[gPlayIndex-1], TRUE);
				gPlayIndex++;
				
				if ((1 == gPlayIndex%m_pageSize) && (gPlayIndex > 1))
				{
					PageDown();
				}
				if(gPlayIndex > m_MovieList.size())
				{
					gPlayIndex = 1;
					if (m_selectCurrentPage == m_selectPageCount)
					{
						OnFirst();
					}
				}
				std::string s = Util::StringOp::FromCString(m_MovieList[gPlayIndex-1]);
				char filename_[128];
				sprintf(filename_, "%s", s.c_str());
				SetVideo(filename_);
				playerDlg_->Show();
				m_MoveText->m_nCount = 0;
				m_MoveText->SetTxt(m_ShowList[gPlayIndex-1], 22, RGB(255,255,255));
			}
		}
		main->CancelBalckLightSaveTimer();
	}
	else if(IDT_PAUSE_PLAYER == nIDEvent)
	{
		KillTimer(nIDEvent);
		main->playervideo_->ExitPlayer();
	}

	else if(nIDEvent == 1001)   //恢复播放
	{
		gVideo3PuaseCount--;
		KillTimer(1001);
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		if(m_bIsPausebyEvent)
		{
			if(IsWindowVisible() && m_bIsPausebyEvent && !main->m_AlarmShowDlg->IsWindowVisible() && !main->m_pTelephoneDlg->IsWindowVisible() && !main->m_pSoundDlg->m_pRecordSoundDlg->m_bISRecording && !((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->m_bRecording/* && (gVideo3PuaseCount<=0)*/)
			{
				gVideo3PuaseCount = 0;
				m_bIsPausebyEvent = FALSE;
				main->playervideo_->SetOwner(playerDlg_);
				main->playervideo_->InitPlayer();
				main->playervideo_->ResumePlayer();

				if(m_IsPlay == 2)
				{
					main->playervideo_->PausePlayer(TRUE);
				}
				else
				{
					SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
				}

				main->KillScreenSaveTimer();  //恢复播放时,停止屏保
			}
		}
	}
	else if(nIDEvent == 1002)	//暂停播放
	{
		if(gVideo3PuaseCount < 0)
			gVideo3PuaseCount = 0;
		gVideo3PuaseCount++;
		KillTimer(1002);
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		if(main->playervideo_->isPlaying_)
		{
			KillTimer(IDT_GETINFO_TIIMER);
			main->playervideo_->ExitPlayer(TRUE);
			m_bIsPausebyEvent = TRUE;
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CMainVideoDlg::OnOutEvent(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	if(l == 0)  //暂停播放
	{
		//SetTimer(1002, 5, NULL);
		OnTimer(1002);
	}
	else if(l == 1) //恢复播放
	{
		SetTimer(1001, 50, NULL);
	}
}

void CMainVideoDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	static unsigned int firstClick = 0;
	static unsigned int lastClick = 0;
	switch(w)
	{
	case 1:			//第一首
		m_MJPGList.SetUnitIsDownStatus(3, FALSE);
		m_MJPGList.SetUnitIsShow(3, TRUE);
		OnFirst();
		m_MJPGList.SetUnitIsDownStatus(3, TRUE);
		m_MJPGList.SetUnitIsShow(3, TRUE);
		break;
	case 2:         //上一首
		m_MJPGList.SetUnitIsDownStatus(3, FALSE);
		m_MJPGList.SetUnitIsShow(3, TRUE);
		OnPre();
		m_MJPGList.SetUnitIsDownStatus(3, TRUE);
		m_MJPGList.SetUnitIsShow(3, TRUE);
		break;
	case 3:			//播放
		OnPlayer();
		break;      
	case 4:			//停止
		OnStop();
		break;
	case 5:			//下一首
		m_MJPGList.SetUnitIsDownStatus(3, FALSE);
		m_MJPGList.SetUnitIsShow(3, TRUE);
		OnBack();
		m_MJPGList.SetUnitIsDownStatus(3, TRUE);
		m_MJPGList.SetUnitIsShow(3, TRUE);
		break;
	case 6:			//最后一首
		m_MJPGList.SetUnitIsDownStatus(3, FALSE);
		m_MJPGList.SetUnitIsShow(3, TRUE);
		OnLast();
		m_MJPGList.SetUnitIsDownStatus(3, TRUE);
		m_MJPGList.SetUnitIsShow(3, TRUE);
		break;
	case 7:			//全屏
		OnPlayerAll();
		break;
	case 9:		   //上翻
		PageUp();
		break;
	case 10:		   //下翻
		PageDown();
		break;
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
		if (m_ClickType == w)
		{
			lastClick = GetTickCount();
			if ((lastClick - firstClick) < 1000)
			{
				OnDBClickShowList(w);
			}
			else
			{
				firstClick = GetTickCount();
			}
		}
		else
		{
			m_ClickType = w;
			firstClick = GetTickCount();
			OnClickShowList(w);
		}
		break;
	case 50:
		OnMute();
		break;
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
	case 60:
		if (m_IsSound)
		{
			m_Volume = w - 50;
			ChangeVolume(w);
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->playervideo_->SetVolume(m_Volume);
		}
		break;
	case 1000:		//退出
		OnExit();
		break;
	}
}

void CMainVideoDlg::CalculatePage(int dataCount)
{
	if (0 == dataCount)
	{
		m_selectCurrentPage = 0;
	}
	else
	{
		m_selectCurrentPage = 1;
	}
	
	if (0 == dataCount%m_pageSize)
	{
		m_selectPageCount = dataCount/m_pageSize;
	}
	else
	{
		m_selectPageCount = dataCount/m_pageSize + 1;
	}
	
	CString str;
	str.Format(L"/%d",m_selectPageCount);
	m_MJPGList.SetUnitText(41, str, TRUE);
	str.Empty();
	str.Format(L"%d",m_selectCurrentPage);
	m_MJPGList.SetUnitText(40, str, TRUE);
	
	m_MJPGList.SetUnitIsDisable(9, TRUE);
	if (m_selectPageCount <= 1)		//当总数小于pagesize时设置下翻按钮不可用
	{
		m_MJPGList.SetUnitIsDisable(10, TRUE);
	}		
	else
	{
		m_MJPGList.SetUnitIsDisable(10, FALSE); 
	}
}

void CMainVideoDlg::ShowArrayInList(std::vector<CString> fileName)
{
	ClearAll();
	if (gPlayIndex <= m_selectCurrentPage*m_pageSize && gPlayIndex >= (m_selectCurrentPage-1)*m_pageSize)
	{
		if ((gPlayIndex > 0) && (0 == gPlayIndex%m_pageSize))
		{
			m_MJPGList.SetUnitColor(25, font_blue, FALSE);
		}
		else
		{
			m_MJPGList.SetUnitColor((gPlayIndex%m_pageSize)+20, font_blue, FALSE);
		}
	}
	if (fileName.size() > 0)
	{
		int index = 21;
		for (int i=(m_selectCurrentPage - 1)*m_pageSize; i<m_selectCurrentPage*m_pageSize; i++)
		{
			if (fileName.begin() + i < fileName.end())
			{
				m_MJPGList.SetUnitIsDisable(index, FALSE);
				m_MJPGList.SetUnitText(index, fileName[i], FALSE);
			}
			else
			{
				m_MJPGList.SetUnitText(index, L"", FALSE);
				m_MJPGList.SetUnitIsDisable(index, TRUE);
			}
			
			index++;
		}
		m_MJPGList.Invalidate();
	}
}

void CMainVideoDlg::HandleAudio(bool bt)
{	
	if (bt)
	{
		m_Volume++;
		if (m_Volume > 10)
		{
			m_Volume = 10;
			return ;
		}
	}
	else
	{	
		m_Volume--;
		if (m_Volume < 0)
		{
			m_Volume = 0 ;
			return ;
		}
	}
	
	ChangeVolume(m_Volume + 50);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->playervideo_->SetVolume(m_Volume);
	
}

void CMainVideoDlg::PageUp()
{
	m_selectCurrentPage--;
	
	if (m_selectCurrentPage <= 1)
	{
		m_selectCurrentPage = 1;
		m_MJPGList.SetUnitIsDisable(9, TRUE);
	}
	if (m_selectCurrentPage < m_selectPageCount)
	{
		m_MJPGList.SetUnitIsDisable(10, FALSE);
	}
	
	CString str;
	str.Format(L"%d", m_selectCurrentPage);
	m_MJPGList.SetUnitText(40, str, TRUE);
	str.Empty();
	ShowArrayInList(m_ShowList);
}

void CMainVideoDlg::PageDown()
{
	m_selectCurrentPage++;   
	
	if (m_selectCurrentPage > 1)
	{
		m_MJPGList.SetUnitIsDisable(9, FALSE);
	}
	if (m_selectCurrentPage >= m_selectPageCount)  
	{
		m_MJPGList.SetUnitIsDisable(10, TRUE);
	}
	
	CString str;
	str.Format(L"%d", m_selectCurrentPage);
	m_MJPGList.SetUnitText(40, str, TRUE);
	str.Empty();
	ShowArrayInList(m_ShowList);
}

void CMainVideoDlg::ClearAll()
{
	for (int i=21; i<=25; i++)
	{
		m_MJPGList.SetUnitText(i, L"", FALSE);
		m_MJPGList.SetUnitIsDownStatus(i, FALSE);
		m_MJPGList.SetUnitColor(i, font_red, FALSE);
	}
	m_MJPGList.Invalidate();
}

void CMainVideoDlg::OnClickShowList(int unitNO)
{
	for (int i=21; i<=25; i++)
	{
		if (i != unitNO)
		{
			m_MJPGList.SetUnitIsDownStatus(i, FALSE);
		}
		else
		{
			m_MJPGList.SetUnitIsDownStatus(i, TRUE);
		}
	}
	m_MJPGList.Invalidate();
	SetTimer(0x10, 10*1000, NULL);
}

void CMainVideoDlg::OnDBClickShowList(int unitNO)
{
	m_ClickType = 0;
	m_MJPGList.SetUnitIsDownStatus(3, FALSE);
	m_MJPGList.SetUnitIsShow(3, TRUE);
	KillTimer(IDT_GETINFO_TIIMER);
	m_MJPGList.SetUnitColor((gPlayIndex%m_pageSize)+20, font_red, TRUE);
	gPlayIndex = (m_selectCurrentPage-1)*m_pageSize + unitNO - 20;
	SetVideo(m_MovieList[gPlayIndex-1]);
	playerDlg_->Show();
	m_IsPlay = 1;
	m_MoveText->m_nCount = 0;
	m_MoveText->SetTxt(m_ShowList[gPlayIndex-1], 22, RGB(255,255,255));
	SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
	m_MJPGList.SetUnitIsDownStatus(3, TRUE);
	m_MJPGList.SetUnitIsShow(3, TRUE);
}