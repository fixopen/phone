// MainMp3Dlg.cpp : implementation file
//
#include "stdafx.h"
#include "MainMp3Dlg.h"
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

/////////////////////////////////////////////////////////////////////////////
// CMainMp3Dlg dialog

CMainMp3Dlg::CMainMp3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainMp3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainMp3Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	memset(m_chDir, 0, 128);
	m_IsPlay = 0;
	m_IsSound = TRUE;
	m_Volume = SOUND_DEFAULT_VOLUME;
}


void CMainMp3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainMp3Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainMp3Dlg, CDialog)
	//{{AFX_MSG_MAP(CMainMp3Dlg)
	ON_BN_CLICKED(IDC_BUTTON_MP3EXIT, OnExit)
	ON_BN_CLICKED(IDC_BUTTON_MP3ALL, OnSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_MP3CLEAR, OnClearAll)
	ON_BN_CLICKED(IDC_BUTTON_MP3PLAY, OnPlayer)
	ON_BN_CLICKED(IDC_BUTTON_MP3PRE, OnPre)
	ON_BN_CLICKED(IDC_BUTTON_MP3BACK, OnBack)
	ON_BN_CLICKED(IDC_BUTTON_MP3OPEN, OnOpenFile)
	ON_BN_CLICKED(IDC_BUTTON_MP3MUTE, OnMute)
	ON_MESSAGE(WM_PROCESS_POS, OnPregress)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MP3LIST, OnClickPlayList)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMainMp3Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CString s;
	s = Data::LanguageResource::Get(Data::RI_MAIN_MP3BTN).c_str();
	m_stcTitle.Create(s, WS_CHILD|WS_VISIBLE, CRect(4, 0, 80, 15), this);
	m_stcTitle.SetColor(RGB(0, 0, 0), RGB(192, 192, 192));
//	m_stcTitle.SetFontSize(24);
	m_stcFilename.Create(L"", WS_CHILD|WS_VISIBLE, CRect(82, 0, 190, 15), this );
//	m_stcFilename.SetColor(RGB(0, 0, 0), RGB(192, 192, 192));
	s = "00:00/00:00";
	m_stcTime.Create(s, WS_CHILD|WS_VISIBLE, CRect(140, 168, 213, 182), this);
	m_stcTime.SetColor(RGB(0, 0, 0), RGB(192, 192, 192));
	m_btnExit.Create(L"", WS_CHILD|WS_VISIBLE, CRect(192, 0, 207, 15), this, IDC_BUTTON_MP3EXIT);
	m_btnExit.SetIcon(IDI_ICON_EXIT, CSize(12, 12));
	
	s = Data::LanguageResource::Get(Data::RI_VEDIO_ALLBTN).c_str();
	m_btnAllSelect.Create(s, Data::g_buttonArcBMPID[0][Data::g_skinstyle], Data::g_buttonArcBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE,CRect(12, 178, 67, 198), this, IDC_BUTTON_MP3ALL);
	m_btnAllSelect.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	s = Data::LanguageResource::Get(Data::RI_VEDIO_DELETEALLBTN).c_str();
	//清空
	m_btnAllClear.Create(s, Data::g_buttonArcBMPID[0][Data::g_skinstyle], Data::g_buttonArcBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0),this,IDC_BUTTON_MP3CLEAR);
	m_btnAllClear.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	s = Data::LanguageResource::Get(Data::RI_VEDIO_OPENBTN).c_str();
	m_btnOpenFile.Create(s, Data::g_buttonArcBMPID[0][Data::g_skinstyle], Data::g_buttonArcBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(12, 178, 67, 198), this, IDC_BUTTON_MP3OPEN);
	m_btnOpenFile.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnOpenFile.ShowWindow(SW_HIDE);
	s = Data::LanguageResource::Get(Data::RI_VEDIO_ALLSCREENBTN).c_str();
	
	m_btnPreMp3.Create(L"", Data::g_buttonPlayBMPID[0][Data::g_skinstyle], Data::g_buttonPlayBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(78, 178, 107, 197), this,IDC_BUTTON_MP3PRE);
	m_btnPreMp3.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnPlayMp3.Create(L"", Data::g_buttonPlayBMPID[0][Data::g_skinstyle], Data::g_buttonPlayBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(107, 178, 141, 197), this,IDC_BUTTON_MP3PLAY);
	m_btnPlayMp3.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnPlayMp3.SetBmpOffSet(29,0);
	m_btnBackMp3.Create(L"", Data::g_buttonPlayBMPID[0][Data::g_skinstyle], Data::g_buttonPlayBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(141, 178, 170, 197), this,IDC_BUTTON_MP3BACK);
	m_btnBackMp3.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnBackMp3.SetBmpOffSet(63,0);
	m_btnMuteSound.Create(L"", Data::g_buttonSoundBMPID[0][Data::g_skinstyle], Data::g_buttonSoundBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(191, 180, 204, 195), this,IDC_BUTTON_MP3MUTE);
	m_btnMuteSound.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnMuteSound.SetIsToggle(TRUE);
	m_prgPlayTime.Create(WS_CHILD|WS_VISIBLE, CRect(13, 166, 157, 172), this, IDC_PROGRESS_MP3TIME);
	m_prgPlayTime.SetParam(0, 0, 10, 1, 1, Data::g_progressPlayBMPID[0][Data::g_skinstyle], Data::g_progressPlayBMPID[1][Data::g_skinstyle]);
	m_prgPlayTime.SetPos(0, FALSE);
	m_prgPlayTime.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_prgSoundSelect.Create(WS_CHILD|WS_VISIBLE, CRect(208, 180, 231, 195), this,IDC_PROGRESS_MP3SOUND);
	m_prgSoundSelect.SetParam(0, 0, 15, 2, 1, Data::g_progressSoundBMPID[1][Data::g_skinstyle], Data::g_progressSoundBMPID[0][Data::g_skinstyle]);
	m_prgSoundSelect.SetPos(SOUND_DEFAULT_VOLUME, FALSE);
	m_prgSoundSelect.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	m_lstPlayList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(2, 16, 206, 168), this, IDC_LIST_MP3LIST, TRUE, 1);
	m_lstPlayList.SetListColor(RGB(255, 255, 255), RGB(255, 255, 255));
	m_lstPlayList.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 133);
	
	m_lstPlayList1.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(2, 16, 206, 168), this, 0xFFFF, TRUE, 1);
	m_lstPlayList1.SetListColor(RGB(255, 255, 255), RGB(255, 255, 255));
	m_lstPlayList1.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 133);
	m_lstPlayList1.ShowWindow(SW_HIDE);

	m_pImageList = new CImageList();
	m_pImageList->Create(12, 12, ILC_COLOR32|ILC_MASK, 2, 2);   
    HICON hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_CHECK0));   
	m_pImageList->Add(hIcon);  
    hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_CHECK1));   
	m_pImageList->Add(hIcon);   
	m_lstPlayList.SetImageList(m_pImageList, LVSIL_SMALL);

	//SetPlayList("/storage card/my_music/");
	SetPlayList("/flashdrv/my_music/");
	m_lstPlayList.SetScrollRagle(FALSE);

	//播放器
	playerDlg_ = new CPlayerDlg();
	playerDlg_->Create(CPlayerDlg::IDD, this);

	m_backGroudStatic.Create(CRect(0, 0, 240, 204), this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainMp3Dlg ::SetMP3()
{
	playerDlg_->SetParam( "/flashdrv/my_music/playlist.pls");
	playerDlg_->ReSetWindowsRect(CRect(2, 16, 2, 16));
}

void CMainMp3Dlg::SetPlayList(char *dir)
{
	m_lstPlayList.DeleteAllItems();
	strcpy(m_chDir, dir);
	WIN32_FIND_DATA FindFileData;			//查找文件时要使用的数据结构
	HANDLE hFind = INVALID_HANDLE_VALUE;	//定义查找句柄
	int ncount = 0;

	char totalfilename[128];
	sprintf(totalfilename, "%s*.*", dir);
	CString sDir = totalfilename;
	hFind = FindFirstFile(sDir, &FindFileData);//使用FindFirstFile函数来开始文件查找
		
	if (hFind == INVALID_HANDLE_VALUE) 
	{
//		printf ("Inval  return 0\n;");
		return;
	} 
	else 
	{
		char filename[128];
//		printf ("First file name is %s\n", FindFileData.cFileName);
		int i = wcstombs( filename, FindFileData.cFileName, 128);
		filename[i] = '\0';
		if(strstr(filename, ".mp3")||strstr(filename, ".MP3")||strstr(filename, ".Mp3")||strstr(filename, ".wav")||strstr(filename, ".WAV"))
		{
			m_lstPlayList.InsertItem(ncount, FindFileData.cFileName, 0);
			m_lstPlayList.SetItemData(ncount++, 0);
		}
	
		//以下是循环使用FindNextFile函数来查找文件
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
//			printf ("Next file name is %s\n", FindFileData.cFileName);
			i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
			filename[i] = '\0';
			if(strstr(filename, ".mp3")||strstr(filename, ".MP3")||strstr(filename, ".Mp3")||strstr(filename, ".wav")||strstr(filename, ".WAV"))
			{
				m_lstPlayList.InsertItem(ncount, FindFileData.cFileName, 0);
				m_lstPlayList.SetItemData(ncount++, 0);
			}
		}

		DWORD dwError = GetLastError();
		if (dwError == ERROR_NO_MORE_FILES) 
		{
			FindClose(hFind);//关闭查找句柄
		} 
		else 
		{
//			printf ("FindNextFile error. Error is %u\n", dwError);
			return;
		}
	}
	
	return;	
}

void CMainMp3Dlg::OnExit()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(main->player_->type_ && main->player_->isPlaying_)	//停止播放
	{
		main->player_->ExitPlayer();
	}
	playerDlg_->ShowWindow(SW_HIDE);
	m_lstPlayList.ShowWindow(SW_SHOW);
	m_lstPlayList1.ShowWindow(SW_HIDE);

	m_IsPlay = 0;
//	m_btnPlayMp3.SetIcon(IDI_ICON_PLAY, CSize(24, 24));

	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
	m_stcFilename.SetTxt(L"");
	KillTimer(IDT_GETINFO_TIIMER);
}

void CMainMp3Dlg::OnSelectAll()
{
	int ncount = m_lstPlayList.GetItemCount();
	
	LVITEM lvitem;
	lvitem.mask=LVIF_TEXT   |   LVIF_IMAGE;   
	
	for(int i = 0; i < ncount; i++)
	{
		lvitem.iItem=i;   
		lvitem.iSubItem=0;   
		lvitem.pszText = m_lstPlayList.GetItemText(i, 0).GetBuffer(128);   
		lvitem.lParam=i;   
		lvitem.iImage=1;
		m_lstPlayList.SetItem(&lvitem);
		m_lstPlayList.SetItemData(i, 1);
	}
}

void CMainMp3Dlg::OnClearAll()
{
	int ncount = m_lstPlayList.GetItemCount();

	LVITEM lvitem;
	lvitem.mask=LVIF_TEXT   |   LVIF_IMAGE;   
	
	for(int i = 0; i < ncount; i++)
	{
		lvitem.iItem=i;   
		lvitem.iSubItem=0;   
		lvitem.pszText = m_lstPlayList.GetItemText(i, 0).GetBuffer(128);   
		lvitem.lParam=i;   
		lvitem.iImage=0;
		m_lstPlayList.SetItem(&lvitem);
		m_lstPlayList.SetItemData(i, 0);
	}
}

void CMainMp3Dlg::OnClickPlayList(NMHDR* pNMHDR, LRESULT* pResult)
{
	POSITION pos = m_lstPlayList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_lstPlayList.GetNextSelectedItem (pos);
		int id = m_lstPlayList.GetItemData(index);
		id=(id == 0)?1:0;
		LVITEM lvitem;
		lvitem.mask=LVIF_TEXT   |   LVIF_IMAGE;   
		lvitem.iItem=index;   
		lvitem.iSubItem=0;   
		lvitem.pszText = m_lstPlayList.GetItemText(index, 0).GetBuffer(128);   
		lvitem.lParam=index;   
		lvitem.iImage=id;
		m_lstPlayList.SetItem(&lvitem);
		m_lstPlayList.SetItemData(index, id);
	}
	*pResult = 0;
}

//播放
void CMainMp3Dlg::OnPlayer()
{
	if(m_IsPlay == 0)
	{
		int ncount = m_lstPlayList.GetItemCount();
		m_lstPlayList1.DeleteAllItems();
		char buffer[1024];
		memset(buffer, 0, 1024);
		int nFileSelected = 0;

		char txt[128];
		strcpy(txt, "[Playlist]\r\n");
		strcpy(buffer, txt);
		
		for(int i = 0; i < ncount; i++)
		{
			int val = m_lstPlayList.GetItemData(i);
			if(val == 1)
			{
				char filename[128];
				CString s = m_lstPlayList.GetItemText(i, 0);
				int n = wcstombs( filename, s.GetBuffer(128), 128/*s.GetLength()*/);
				filename[n] = '\0';
				sprintf(txt, "File%d=/flashdrv/my_music/%s\r\nLength%d=255\r\n", i,filename, i);
				strcat(buffer, txt);
				m_lstPlayList1.InsertItem(nFileSelected, s);
				nFileSelected++;
			}
		}
		m_lstPlayList1.SetScrollRagle(FALSE);

		sprintf(txt, "NumberOfEntries=%d\r\nVersion=2\r\n", nFileSelected);
		strcat(buffer, txt);

		if(nFileSelected != 0)
		{
			FILE *file;
			file = fopen("/flashdrv/my_music/playlist.pls", "w+b");
			if(file)
			{
				fwrite(buffer, sizeof(char), strlen(buffer), file);
				fclose(file);
			}
			//调用播放窗口
			if(playerDlg_)
			{
				SetMP3();
				playerDlg_->ShowWindow(SW_SHOW);
			}
			m_lstPlayList.ShowWindow(SW_HIDE);
			m_lstPlayList1.ShowWindow(SW_SHOW);
			m_btnAllSelect.ShowWindow(SW_HIDE);
			m_btnAllClear.ShowWindow(SW_HIDE);
			m_btnOpenFile.ShowWindow(SW_SHOW);

			m_IsPlay = 1;
//			m_btnPlayMp3.SetIcon(IDI_ICON_STOPPLAY, CSize(24, 24));
		}
	}
	else if(m_IsPlay == 1)
	{
		m_IsPlay = 2;
//		m_btnPlayMp3.SetIcon(IDI_ICON_PLAY, CSize(24, 24));
		m_btnPlayMp3.ShowWindow(SW_SHOW);
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->player_->ExitPlayer(FALSE);
	}
	else if(m_IsPlay == 2)
	{
		m_IsPlay = 1;
//		m_btnPlayMp3.SetIcon(IDI_ICON_STOPPLAY, CSize(24, 24));
		m_btnPlayMp3.ShowWindow(SW_SHOW);
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->player_->InitPlayer();
	}
	
}
//前一条
void CMainMp3Dlg::OnPre()
{

}
//后一条
void CMainMp3Dlg::OnBack()
{

}
//打开播放列表
void CMainMp3Dlg::OnOpenFile()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(main->player_->type_ && main->player_->isPlaying_)	//停止播放
	{
		main->player_->ExitPlayer();
	}
	playerDlg_->ShowWindow(SW_HIDE);
	m_lstPlayList1.ShowWindow(SW_HIDE);
	m_lstPlayList.ShowWindow(SW_SHOW);
	m_btnAllSelect.ShowWindow(SW_SHOW);
	m_btnAllClear.ShowWindow(SW_SHOW);
	m_btnOpenFile.ShowWindow(SW_HIDE);

	m_IsPlay = 0;
//	m_btnPlayMp3.SetIcon(IDI_ICON_PLAY, CSize(24, 24));
}

//设置声音开关
void CMainMp3Dlg::OnMute()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(m_IsSound)
	{
		main->player_->SetVolume(0); 
		m_btnMuteSound.SetToggleValue(TRUE);
//		m_btnMuteSound.SetIcon(IDI_ICON_SOUNDCACEL, CSize(12,12));
	}
	else
	{
		main->player_->SetVolume(m_Volume);
		m_btnMuteSound.SetToggleValue(FALSE);
//		m_btnMuteSound.SetIcon(IDI_ICON_SOUNDOK, CSize(12,12));
	}
	m_IsSound = !m_IsSound;
}
//
void CMainMp3Dlg::OnPregress(WPARAM w, LPARAM l)
{
	//设置声音
	if(l == IDC_PROGRESS_MP3SOUND)
	{
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->player_->SetVolume(w);
		m_Volume = w;
	}
	//调整播放时间
	else if(l == IDC_PROGRESS_MP3TIME)
	{
		
	}
}

void CMainMp3Dlg::SetCtrlEnable(BOOL flag)
{

}

void CMainMp3Dlg::StartOpenNewFile()
{
	/*
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	MEDIA_INFO info;
	main->player_->	GetVideoFileInfo(&info);
	*/
	KillTimer(IDT_GETINFO_TIIMER);
	SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CMainMp3Dlg message handlers
void CMainMp3Dlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(IDT_GETINFO_TIIMER == nIDEvent)
	{
//		KillTimer(IDT_GETINFO_TIIMER);
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		MEDIA_INFO info;
		main->player_->	GetVideoFileInfo(&info);

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
		
		m_prgPlayTime.SetParam(0, 0, n2, 1, 1, Data::g_progressPlayBMPID[0][Data::g_skinstyle], Data::g_progressPlayBMPID[1][Data::g_skinstyle]);
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
		m_stcFilename.SetTxt(sFilename1);

		//m_stcFilename.SetWindowText(info.szFileName);
		char txt[32];
		sprintf(txt, "%d:%02d/%d:%02d", Hour*60+Min, Sec, Hour1*60+Min1, Sec1);
		CString s = txt;
		m_stcTime.SetWindowText(s);
	}

	CDialog::OnTimer(nIDEvent);
}
