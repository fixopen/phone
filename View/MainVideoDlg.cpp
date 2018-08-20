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

BOOL m_bIsPausebyEvent;

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
	m_IsSound = TRUE;
	m_Volume = SOUND_DEFAULT_VOLUME;
	m_bIsPausebyEvent = FALSE;
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
	ON_BN_CLICKED(IDC_BUTTON_VIDEOALL, OnSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOCLEAR, OnClearAll)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOPLAY, OnPlayer)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOPRE, OnPre)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOBACK, OnBack)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOOPEN, OnOpenFile)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOPLAYALL, OnPlayerAll)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOMUTE, OnMute)
	ON_MESSAGE(WM_PROCESS_POS, OnPregress)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PLAYLIST, OnClickPlayList)
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	ON_MESSAGE(WM_USB_MSG, OnDeviceChange)
	ON_MESSAGE(WM_OUTEVENT, OnOutEvent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainVideoDlg message handlers
void CMainVideoDlg ::OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized )
{
	if(nState & WA_ACTIVE )
	{
		SetPlayList(_T("/flashdrv/my_video/"), 0);
	}
}

BOOL CMainVideoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CString s;
	
	m_stcFilename.Create(L"", Data::g_staticAppTileBackRGB[Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(84, 13, 212, 25), this );
	m_stcFilename.SetBackColor(Data::g_staticAppTileBackRGB[Data::g_skinstyle]);
//	m_stcFilename.SetColor(RGB(0, 0, 0), RGB(192, 192, 192));
	s = "00:00/00:00";
	m_stcTime.Create(s, WS_CHILD|WS_VISIBLE, CRect(167, 166, 232, 177), this);
	m_stcTime.SetColor(RGB(0, 0, 0), Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_stcTime.SetFontSize(13);

	m_btnExit.Create(L"", Data::g_buttonExitBMPID[0][Data::g_skinstyle], Data::g_buttonExitBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(212, 11, 231, 28), this, IDC_BUTTON_VIDEOEXIT);
//	m_btnExit.SetIcon(IDI_ICON_EXIT, CSize(12, 12));
	
	//s = Data::LanguageResource::Get(Data::RI_MAIN_VIDEOBTN).c_str();
	m_stcTitle.Create(s, WS_CHILD|WS_VISIBLE, CRect(14, 10, 231, 28), this);
	m_stcTitle.SetBorder(TRUE);
	m_stcTitle.SetLeftMargin(3);
	m_stcTitle.SetColor(RGB(0, 0, 0), Data::g_staticAppTileBackRGB[Data::g_skinstyle]);

	s = Data::LanguageResource::Get(Data::RI_VEDIO_ALLBTN).c_str();
	m_btnAllSelect.Create(s, Data::g_buttonALLSelectBMPID[0][Data::g_skinstyle], Data::g_buttonALLSelectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE,CRect(12, 178, 43, 197), this, IDC_BUTTON_VIDEOALL);
	m_btnAllSelect.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnAllSelect.SetBmpOffSet(0, 0);
	s = Data::LanguageResource::Get(Data::RI_VEDIO_DELETEALLBTN).c_str();
	//清空
	m_btnAllClear.Create(s, Data::g_buttonALLSelectBMPID[0][Data::g_skinstyle], Data::g_buttonALLSelectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(43, 178, 76, 197),this,IDC_BUTTON_VIDEOCLEAR);
	m_btnAllClear.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnAllClear.SetBmpOffSet(31, 0);

	s = Data::LanguageResource::Get(Data::RI_VEDIO_OPENBTN).c_str();
	m_btnOpenFile.Create(s, Data::g_buttonALLSelectBMPID[0][Data::g_skinstyle], Data::g_buttonALLSelectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(12, 178, 43, 197), this, IDC_BUTTON_VIDEOOPEN);
	m_btnOpenFile.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnOpenFile.SetBmpOffSet(0, 0);
	m_btnOpenFile.ShowWindow(SW_HIDE);

	s = Data::LanguageResource::Get(Data::RI_VEDIO_ALLSCREENBTN).c_str();
	m_btnPlayAll.Create(s, Data::g_buttonALLSelectBMPID[0][Data::g_skinstyle], Data::g_buttonALLSelectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(43, 178, 76, 197),this,IDC_BUTTON_VIDEOPLAYALL);
	m_btnPlayAll.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnPlayAll.SetBmpOffSet(31, 0);
	m_btnPlayAll.ShowWindow(SW_HIDE);

	m_btnPreVideo.Create(L"", Data::g_buttonPlayBMPID[0][Data::g_skinstyle], Data::g_buttonPlayBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(78, 178, 107, 197), this,IDC_BUTTON_VIDEOPRE);
	m_btnPreVideo.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnPlayVideo.Create(L"", Data::g_buttonPlayBMPID[0][Data::g_skinstyle], Data::g_buttonPlayBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(107, 178, 141, 197), this,IDC_BUTTON_VIDEOPLAY);
	m_btnPlayVideo.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnPlayVideo.SetIsToggle(TRUE);
	m_btnPlayVideo.SetBmpOffSet(29,0);
	m_btnBackVideo.Create(L"", Data::g_buttonPlayBMPID[0][Data::g_skinstyle], Data::g_buttonPlayBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(141, 178, 170, 197), this,IDC_BUTTON_VIDEOBACK);
	m_btnBackVideo.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnBackVideo.SetBmpOffSet(63,0);
	m_btnMuteSound.Create(L"", Data::g_buttonSoundBMPID[0][Data::g_skinstyle], Data::g_buttonSoundBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(191, 180, 204, 195), this,IDC_BUTTON_VIDEOMUTE);
	m_btnMuteSound.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnMuteSound.SetIsToggle(TRUE);
	
	m_prgPlayTime.Create(WS_CHILD|WS_VISIBLE, CRect(13, 168, 157, 174), this, IDC_PROGRESS_VIDEOTIME);
	m_prgPlayTime.SetParam(0, 0, 10, 1, 1, Data::g_progressPlayBMPID[1][Data::g_skinstyle], Data::g_progressPlayBMPID[0][Data::g_skinstyle]);
	m_prgPlayTime.SetPos(0, FALSE);
	m_prgPlayTime.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_prgSoundSelect.Create(WS_CHILD|WS_VISIBLE, CRect(208, 180, 231, 195), this,IDC_PROGRESS_VIDEOSOUND);
	m_prgSoundSelect.SetParam(0, 0, 15, 2, 1, Data::g_progressSoundBMPID[1][Data::g_skinstyle], Data::g_progressSoundBMPID[0][Data::g_skinstyle]);
	m_prgSoundSelect.SetPos(SOUND_DEFAULT_VOLUME, FALSE);
	m_prgSoundSelect.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	
	m_lstPlayList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(15, 28, 231, 164), this, IDC_LIST_PLAYLIST, TRUE, 1);
	m_lstPlayList.SetListColor(Data::g_listctrlBackRGB1[Data::g_skinstyle], Data::g_listctrlBackRGB2[Data::g_skinstyle]);
	m_lstPlayList.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 195);
	
	m_pImageList = new CImageList();
	m_pImageList->Create(12, 12, ILC_COLOR32|ILC_MASK, 2, 2);   
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

	SetPlayList(_T("/flashdrv/my_video/"), 0);
	m_lstPlayList.SetScrollRagle(FALSE);

	//播放器
	playerDlg_ = new CPlayerDlg();
	playerDlg_->Create(CPlayerDlg::IDD, this);

	m_backGroudStatic.Create(CRect(0, 0, 240, 234), this);
	m_backGroudStatic.SetInRect(CRect(13, 9, 232, 165));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainVideoDlg ::SetVideo()
{
	playerDlg_->SetParam( "/flashdrv/my_video/playlist.pls", 2);
	playerDlg_->ReSetWindowsRect(CRect(15, 28, 231, 164));
	m_bIsPausebyEvent = FALSE;
}

void CMainVideoDlg::SetPlayList(TCHAR *dir, int local)
{
	memset(m_chDir, 0, 128*2);
	m_lstPlayList.DeleteAllItems();
	int ncount = 0;
	if(local == 0)
	{
		if(DetectDIR(_T("/usbdisk")))
			m_lstPlayList.InsertItem(ncount++, _T("usbdisk"), 3);
		if(DetectDIR(_T("/storage card")))
			m_lstPlayList.InsertItem(ncount++, _T("storage card"), 3);
		memcpy(m_chDir, _T("/flashdrv/my_video/"), wcslen(_T("/flashdrv/my_video/"))*2);
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
		//		printf ("First file name is %s\n", FindFileData.cFileName);
		int i = wcstombs( filename, FindFileData.cFileName, 128);
		filename[i] = '\0';
		if(strstr(filename, ".wmv")||strstr(filename, ".WMV") || strstr(filename, ".avi")||strstr(filename, ".AVI")||strstr(filename, ".mpg")||strstr(filename, ".MPG")||strstr(filename, ".mp4"))
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
			//			printf ("Next file name is %s\n", FindFileData.cFileName);
			i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
			filename[i] = '\0';
			if(strstr(filename, ".wmv")||strstr(filename, ".WMV") ||strstr(filename, ".avi")||strstr(filename, ".AVI")||strstr(filename, ".mpg")||strstr(filename, ".MPG")||strstr(filename, ".mp4"))
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
			//			printf ("FindNextFile error. Error is %u\n", dwError);
			return;
		}
	}
	m_lstPlayList.SetScrollRagle(TRUE);
	return;	
}

void CMainVideoDlg::OnExit()
{
	m_bIsPausebyEvent = FALSE;
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
//	if(main->player_->type_ && main->player_->isPlaying_)	//停止播放
	if(main->player_->isPlayingVideo && main->player_->isVideoType == 2 )
	{
		main->player_->ExitPlayer();
		KillTimer(IDT_GETINFO_TIIMER);
	}
	m_btnAllSelect.ShowWindow(SW_SHOW);
	m_btnAllClear.ShowWindow(SW_SHOW);
	m_btnOpenFile.ShowWindow(SW_HIDE);
	m_btnPlayAll.ShowWindow(SW_HIDE);
	m_btnPlayVideo.SetToggleValue(FALSE);

	m_IsPlay = 0;
//	m_btnPlayVideo.SetIcon(IDI_ICON_PLAY, CSize(24, 24));

	playerDlg_->ShowWindow(SW_HIDE);
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
	m_stcFilename.SetTxt(L"");
}

void CMainVideoDlg::OnSelectAll()
{
	int ncount = m_lstPlayList.GetItemCount();
	
	LVITEM lvitem;
	lvitem.mask=LVIF_TEXT   |   LVIF_IMAGE;   
	
	for(int i = 0; i < ncount; i++)
	{
		lvitem.iItem=i;   
		lvitem.iSubItem=0;   
		lvitem.pszText = m_lstPlayList.GetItemText(i, 0).GetBuffer(128);   
		CString s = lvitem.pszText;
		int n = s.Find(_T(".mp4"));
		int n1 = s.Find(_T(".avi"));
		int n2 = s.Find(_T(".AVI"));
		int n3 = s.Find(_T(".wmv"));
		int n4 = s.Find(_T(".WMV"));
		int n5 = s.Find(_T(".MPG"));
		int n6 = s.Find(_T(".mpg"));
		if((n5 == -1) && (n6 == -1) && (n4 == -1) && (n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
			continue;
		lvitem.lParam=i;   
		lvitem.iImage=1;
		m_lstPlayList.SetItem(&lvitem);
		m_lstPlayList.SetItemData(i, 1);
	}
}

void CMainVideoDlg::OnClearAll()
{
	int ncount = m_lstPlayList.GetItemCount();

	LVITEM lvitem;
	lvitem.mask=LVIF_TEXT   |   LVIF_IMAGE;   
	
	for(int i = 0; i < ncount; i++)
	{
		lvitem.iItem=i;   
		lvitem.iSubItem=0;   
		lvitem.pszText = m_lstPlayList.GetItemText(i, 0).GetBuffer(128);   
		CString s = lvitem.pszText;
		int n = s.Find(_T(".mp4"));
		int n1 = s.Find(_T(".avi"));
		int n2 = s.Find(_T(".AVI"));
		int n3 = s.Find(_T(".wmv"));
		int n4 = s.Find(_T(".WMV"));
		int n5 = s.Find(_T(".MPG"));
		int n6 = s.Find(_T(".mpg"));
		if((n5 == -1) && (n6 == -1) && (n4 == -1) && (n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
			continue;
		lvitem.lParam=i;   
		lvitem.iImage=0;
		m_lstPlayList.SetItem(&lvitem);
		m_lstPlayList.SetItemData(i, 0);
	}
}

void CMainVideoDlg::OnClickPlayList(NMHDR* pNMHDR, LRESULT* pResult)
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
			SetPlayList(_T("/usbdisk/"), 1);
		}
		//sd
		else if(s.Compare(_T("storage card")) == 0)
		{
			SetPlayList(_T("/storage card/"), 1);
		}
		//上一级
		else if(s.Compare(s1) == 0)
		{
			//	m_chDir
			CString sDir = m_chDir;
			int n = sDir.Find(_T("/"));
			UINT8 old[16];
			int i = 0;
			while(n != -1)
			{
				old[i++] = n;
				n += 1;
				n = sDir.Find(_T("/"), n);
			}
			sDir = sDir.Mid(0, old[i-2]+1);
			if(sDir == _T("/"))
				SetPlayList(sDir.GetBuffer(128), 0);
			else
				SetPlayList(sDir.GetBuffer(128), 1);
		}
		
		else
		{
			int n = s.Find(_T(".mp4"));
			int n1 = s.Find(_T(".avi"));
			int n2 = s.Find(_T(".AVI"));
			int n3 = s.Find(_T(".wmv"));
			int n4 = s.Find(_T(".WMV"));
			int n5 = s.Find(_T(".MPG"));
			int n6 = s.Find(_T(".mpg"));
			if((n5 == -1) && (n6 == -1) && (n4 == -1) && (n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
			{
				CString sDir = m_chDir;
				sDir += s; 
				sDir += "/";
				SetPlayList(sDir.GetBuffer(128), 1);
			}
			else
			{
				int id = m_lstPlayList.GetItemData(index);
				id=(id == 0)?1:0;
				LVITEM lvitem;
				lvitem.mask=LVIF_TEXT | LVIF_IMAGE;   
				lvitem.iItem=index;   
				lvitem.iSubItem=0;   
				lvitem.pszText = m_lstPlayList.GetItemText(index, 0).GetBuffer(128);   
				lvitem.lParam=index;   
				lvitem.iImage=id;
				m_lstPlayList.SetItem(&lvitem);
				m_lstPlayList.SetItemData(index, id);
			}
		}
	}
	*pResult = 0;
}

//播放
void CMainVideoDlg::OnPlayer()
{
	if(m_IsPlay == 0)
	{
		int ncount = m_lstPlayList.GetItemCount();

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
				
				char file_dir[128];
				n = wcstombs( file_dir, m_chDir, 128);
				file_dir[n] = '\0';
				sprintf(txt, "File%d=%s%s\r\nLength%d=255\r\n", i,file_dir,filename, i);
				strcat(buffer, txt);
				nFileSelected++;
			}
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
				SetVideo();
				playerDlg_->ShowWindow(SW_SHOW);
			}
		
			m_btnAllSelect.ShowWindow(SW_HIDE);
			m_btnAllClear.ShowWindow(SW_HIDE);
			m_btnOpenFile.ShowWindow(SW_SHOW);
			m_btnPlayAll.ShowWindow(SW_SHOW);
			m_IsPlay = 1;
			m_btnPlayVideo.SetToggleValue(TRUE);
//			m_btnPlayVideo.SetIcon(IDI_ICON_STOPPLAY, CSize(24, 24));
		}
	}
	else if(m_IsPlay == 1)	//stop player	
	{
		m_IsPlay = 2;
		m_btnPlayVideo.SetToggleValue(FALSE);
//		m_btnPlayVideo.SetIcon(IDI_ICON_PLAY, CSize(24, 24));
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->player_->ExitPlayer(TRUE);	
	}
	else if(m_IsPlay == 2)	//continue player
	{
		m_IsPlay = 1;
//		m_btnPlayVideo.SetIcon(IDI_ICON_STOPPLAY, CSize(24, 24));
		m_btnPlayVideo.SetToggleValue(TRUE);
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->player_->InitPlayer();
	}
}
//前一条
void CMainVideoDlg::OnPre()
{

}
//后一条
void CMainVideoDlg::OnBack()
{

}
//打开播放列表
void CMainVideoDlg::OnOpenFile()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
//	if(main->player_->type_ && main->player_->isPlaying_ == 1)	//停止播放
	if(main->player_->isPlayingVideo)
	{
		main->player_->ExitPlayer();
	}
	playerDlg_->ShowWindow(SW_HIDE);
	m_btnAllSelect.ShowWindow(SW_SHOW);
	m_btnAllClear.ShowWindow(SW_SHOW);
	m_btnOpenFile.ShowWindow(SW_HIDE);
	m_btnPlayAll.ShowWindow(SW_HIDE);
	m_btnPlayVideo.SetToggleValue(FALSE);
	m_IsPlay = 0;

	KillTimer(IDT_GETINFO_TIIMER);
	char txt[32];
	sprintf(txt, "%d:%02d/%d:%02d", 0, 0, 0, 0);
	CString s = txt;
	m_stcTime.SetWindowText(s);
	m_stcFilename.SetTxt(L"");
	m_prgPlayTime.SetPos(0);
}
//全屏播放
void CMainVideoDlg::OnPlayerAll()
{
	SetCtrlEnable(FALSE);//hide control
	GetParent()->SendMessage(WM_PLAYVIDEO, 2);	
}
//设置声音开关
void CMainVideoDlg::OnMute()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(m_IsSound)
	{
		main->player_->SetVolume(0); 
//		m_btnMuteSound.SetIcon(IDI_ICON_SOUNDCACEL, CSize(12,12));
		m_btnMuteSound.SetToggleValue(TRUE);
	}
	else
	{
		main->player_->SetVolume(m_Volume); 
//		m_btnMuteSound.SetIcon(IDI_ICON_SOUNDOK, CSize(12,12));
		m_btnMuteSound.SetToggleValue(FALSE);
	}
	m_IsSound = !m_IsSound;
}
//
void CMainVideoDlg::OnPregress(WPARAM w, LPARAM l)
{
	//设置声音
	if(l == IDC_PROGRESS_VIDEOSOUND)
	{
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->player_->SetVolume(w);
		m_Volume = w;
	}
	//调整播放时间
	else if(l == IDC_PROGRESS_VIDEOTIME)
	{
		
	}
}

void CMainVideoDlg::SetCtrlEnable(BOOL flag)
{
	m_stcTitle.EnableWindow(flag);
	m_stcTime.EnableWindow(flag);
	m_btnExit.EnableWindow(flag);
	m_btnAllSelect.EnableWindow(flag);
	m_btnAllClear.EnableWindow(flag);
	m_btnOpenFile.EnableWindow(flag);
	m_btnPlayAll.EnableWindow(flag);
	m_btnPlayVideo.EnableWindow(flag);
	m_btnPreVideo.EnableWindow(flag);
	m_btnBackVideo.EnableWindow(flag);
	m_btnMuteSound.EnableWindow(flag);
	m_lstPlayList.EnableWindow(flag);
	m_prgPlayTime.EnableWindow(flag);
	m_prgSoundSelect.EnableWindow(flag);
}

void CMainVideoDlg::StartOpenNewFile()
{
	KillTimer(IDT_GETINFO_TIIMER);
	SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
}

void CMainVideoDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(IDT_GETINFO_TIIMER == nIDEvent)
	{
//		KillTimer(IDT_GETINFO_TIIMER);
		MEDIA_INFO info;
		main->player_->GetVideoFileInfo(&info);
		
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
		
		m_prgPlayTime.SetParam(0, 0, n2, 1, 1, Data::g_progressPlayBMPID[1][Data::g_skinstyle], Data::g_progressPlayBMPID[0][Data::g_skinstyle]);
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
		//m_stcFilename.SetWindowText(sFilename1);
		m_stcFilename.SetTxt(sFilename1);
		//m_stcFilename.SetWindowText(info.szFileName);
		char txt[32];
		sprintf(txt, "%d:%02d/%d:%02d", Hour*60+Min, Sec, Hour1*60+Min1, Sec1);
		CString s = txt;
		m_stcTime.SetWindowText(s);
	}
	else if(IDT_PAUSE_PLAYER == nIDEvent)
	{
		KillTimer(nIDEvent);
		main->player_->ExitPlayer();
	}
	CDialog::OnTimer(nIDEvent);
}

void CMainVideoDlg::OnDeviceChange(WPARAM w, LPARAM l)
{
	if (w == 0x8000) //insert
	{
		::Sleep(500);
		if(DetectDIR(_T("/usbdisk")))
			m_stcFilename.SetTxt(L"USB insert");
		else if(DetectDIR(_T("/storage card")))
			m_stcFilename.SetTxt(L"SD insert");
		
		if(memcmp(m_chDir, _T("/flashdrv/my_video/"), wcslen(m_chDir)*2) == 0)
			SetPlayList(_T("/flashdrv/my_video/"), 0);
		
	}
	else if (w == 0x8004) //remove
	{
		::Sleep(1500);
		m_stcFilename.SetTxt(L"remove");
		if(memcmp(m_chDir, _T("/flashdrv/my_video/"), wcslen(m_chDir)*2) == 0)
			SetPlayList(_T("/flashdrv/my_video/"), 0);
	}
}

void CMainVideoDlg::OnOutEvent(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	if(l == 0)  //暂停播放
	{
	//	if(main->player_->isPlaying_ == 1)
		if(main->player_->isVideoType == 2 && main->player_->isPlayingVideo)
		{
			main->player_->ExitPlayer(TRUE);
			m_bIsPausebyEvent = TRUE;
		}
	}
	else if(l == 1) //恢复播放
	{
		if(m_bIsPausebyEvent)
		{
			if(main->player_->isVideoType == 2 && IsWindowVisible())
			{
				m_bIsPausebyEvent = FALSE;
				main->player_->SetOwner(playerDlg_);
				main->player_->InitPlayer();
			}
		}
	}
}