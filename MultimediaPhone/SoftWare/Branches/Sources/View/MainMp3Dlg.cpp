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

extern BOOL m_bIsPausebyEvent;
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
		printf("not find file\n");
	}
	else
	{
		bool finished = false;
		do
		{
			wchar_t wideToName[256];
			//	wchar_t root[256] = {0};
			//	mbstowcs(root, rootPath.c_str(), rootPath.length());
			wsprintf(wideToName, _T("%s/%s"), DesDir, (LPCTSTR)FindFileData.cFileName); //findFileName
			//	wideToName =  
			wchar_t wideFromName[256] = {0};
			//	wchar_t usb[256] = {0};
			wsprintf(wideFromName, _T("%s/%s"), SrcDir, (LPCTSTR)FindFileData.cFileName); //findFileName
			
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//wprintf(_T("create dir %s to %s\n"), wideFromName, wideToName);
				//CopyDirFiles(wideFromName, wideToName);
			}
			else
			{
				if (!::CopyFile(wideFromName, wideToName, false))
				{
				//	printf("current error is %d.", GetLastError());
				}
				//wprintf(_T("copy %s to %s\n"), wideFromName, wideToName);
			}
			
			if (!FindNextFile(hFind, &FindFileData))
			{
				if (GetLastError() == ERROR_NO_MORE_FILES)
				{
					printf("find end.\n");
					finished = true;
				}
				else
				{
					printf("Couldn't find next file.\n");
				}
			}
		} while (!finished);
		FindClose(hFind);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CMainMp3Dlg dialog

CMainMp3Dlg::CMainMp3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainMp3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainMp3Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	memset(m_chDir, 0, 128*2);
	m_IsPlay = 0;
	m_IsSound = TRUE;
	m_Volume = SOUND_DEFAULT_VOLUME;
	m_bIsPausebyEvent = FALSE;
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
	ON_MESSAGE(WM_USB_MSG, OnDeviceChange)
	ON_WM_ACTIVATE()
	ON_MESSAGE(WM_OUTEVENT, OnOutEvent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMainMp3Dlg ::OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized )
{
	if(nState & WA_ACTIVE )
	{
		SetPlayList(_T("/flashdrv/my_music/"), 0);
	}
}


BOOL CMainMp3Dlg::OnInitDialog() 
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

	m_btnExit.Create(L"", Data::g_buttonExitBMPID[0][Data::g_skinstyle], Data::g_buttonExitBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(212, 11, 231, 28), this, IDC_BUTTON_MP3EXIT);
//	m_btnExit.SetIcon(IDI_ICON_EXIT, CSize(12, 12));

	s = Data::LanguageResource::Get(Data::RI_MAIN_MP3BTN).c_str();
	m_stcTitle.Create(s, WS_CHILD|WS_VISIBLE, CRect(14, 10, 231, 28), this);
	m_stcTitle.SetBorder(TRUE);
	m_stcTitle.SetLeftMargin(3);
	m_stcTitle.SetColor(RGB(0, 0, 0), Data::g_staticAppTileBackRGB[Data::g_skinstyle]);

	s = Data::LanguageResource::Get(Data::RI_VEDIO_ALLBTN).c_str();
	m_btnAllSelect.Create(s, Data::g_buttonALLSelectBMPID[0][Data::g_skinstyle], Data::g_buttonALLSelectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE,CRect(12, 178, 43, 197), this, IDC_BUTTON_MP3ALL);
	m_btnAllSelect.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnAllSelect.SetBmpOffSet(0, 0);
	s = Data::LanguageResource::Get(Data::RI_VEDIO_DELETEALLBTN).c_str();
	//清空
	m_btnAllClear.Create(s, Data::g_buttonALLSelectBMPID[0][Data::g_skinstyle], Data::g_buttonALLSelectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(43, 178, 76, 197),this,IDC_BUTTON_MP3CLEAR);
	m_btnAllClear.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnAllClear.SetBmpOffSet(31, 0);
	s = Data::LanguageResource::Get(Data::RI_VEDIO_OPENBTN).c_str();
	m_btnOpenFile.Create(s, Data::g_buttonArcBMPID[0][Data::g_skinstyle], Data::g_buttonArcBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(12, 178, 70, 197), this, IDC_BUTTON_MP3OPEN);
	m_btnOpenFile.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnOpenFile.ShowWindow(SW_HIDE);
	
	m_btnPreMp3.Create(L"", Data::g_buttonPlayBMPID[0][Data::g_skinstyle], Data::g_buttonPlayBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(78, 178, 107, 197), this,IDC_BUTTON_MP3PRE);
	m_btnPreMp3.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnPlayMp3.Create(L"", Data::g_buttonPlayBMPID[0][Data::g_skinstyle], Data::g_buttonPlayBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(107, 178, 141, 197), this,IDC_BUTTON_MP3PLAY);
	m_btnPlayMp3.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnPlayMp3.SetIsToggle(TRUE);
	m_btnPlayMp3.SetBmpOffSet(29,0);
	m_btnBackMp3.Create(L"", Data::g_buttonPlayBMPID[0][Data::g_skinstyle], Data::g_buttonPlayBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(141, 178, 170, 197), this,IDC_BUTTON_MP3BACK);
	m_btnBackMp3.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnBackMp3.SetBmpOffSet(63,0);
	m_btnMuteSound.Create(L"", Data::g_buttonSoundBMPID[0][Data::g_skinstyle], Data::g_buttonSoundBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(191, 180, 204, 195), this,IDC_BUTTON_MP3MUTE);
	m_btnMuteSound.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnMuteSound.SetIsToggle(TRUE);
	
	m_prgPlayTime.Create(WS_CHILD|WS_VISIBLE, CRect(13, 168, 157, 174), this, IDC_PROGRESS_MP3TIME);
	m_prgPlayTime.SetParam(0, 0, 10, 1, 1, Data::g_progressPlayBMPID[1][Data::g_skinstyle], Data::g_progressPlayBMPID[0][Data::g_skinstyle]);
	m_prgPlayTime.SetPos(0, FALSE);
	m_prgPlayTime.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_prgSoundSelect.Create(WS_CHILD|WS_VISIBLE, CRect(208, 180, 231, 195), this,IDC_PROGRESS_MP3SOUND);
	m_prgSoundSelect.SetParam(0, 0, 15, 2, 1, Data::g_progressSoundBMPID[0][Data::g_skinstyle], Data::g_progressSoundBMPID[1][Data::g_skinstyle]);
	m_prgSoundSelect.SetPos(SOUND_DEFAULT_VOLUME, FALSE);
	m_prgSoundSelect.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	m_lstPlayList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(15, 28, 231, 164), this, IDC_LIST_MP3LIST, TRUE, 1);
	m_lstPlayList.SetListColor(Data::g_listctrlBackRGB1[Data::g_skinstyle], Data::g_listctrlBackRGB2[Data::g_skinstyle]);
	m_lstPlayList.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 195);
	
	m_lstPlayList1.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(15, 28, 231, 164), this, 0xFFFF, TRUE, 1);
	m_lstPlayList1.SetListColor(Data::g_listctrlBackRGB1[Data::g_skinstyle], Data::g_listctrlBackRGB2[Data::g_skinstyle]);
	m_lstPlayList1.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 195);
	m_lstPlayList1.ShowWindow_(SW_HIDE);

	m_pImageList = new CImageList();
	m_pImageList->Create(12, 12, ILC_COLOR32|ILC_MASK, 5, 5);   
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

	SetPlayList(_T("/flashdrv/my_music/"), 0);
	m_lstPlayList.SetScrollRagle(FALSE);

	//播放器
	playerDlg_ = new CPlayerDlg();
	playerDlg_->Create(CPlayerDlg::IDD, this);
	playerDlg_->MoveWindow(CRect(15, 28, 15, 28), FALSE);

	m_backGroudStatic.Create(CRect(0, 0, 240, 204), this);
	m_backGroudStatic.SetInRect(CRect(13, 9, 232, 165));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainMp3Dlg ::SetMP3()
{
	playerDlg_->SetParam( "/flashdrv/my_music/playlist.pls", 1);
	playerDlg_->ReSetWindowsRect(CRect(15, 28,15, 28));
	m_bIsPausebyEvent = FALSE;
}

void CMainMp3Dlg::SetPlayList(TCHAR *dir, int local)
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
//		printf ("First file name is %s\n", FindFileData.cFileName);
		int i = wcstombs( filename, FindFileData.cFileName, 128);
		filename[i] = '\0';
		if(strstr(filename, ".mp3")||strstr(filename, ".MP3")||strstr(filename, ".Mp3")||strstr(filename, ".wav")||strstr(filename, ".WAV"))
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
			if(strstr(filename, ".mp3")||strstr(filename, ".MP3")||strstr(filename, ".Mp3")||strstr(filename, ".wav")||strstr(filename, ".WAV"))
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

void CMainMp3Dlg::OnExit_(BOOL isStopMusic)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	//	if(main->player_->type_ && main->player_->isPlaying_)	//停止播放
	if(main->player_->isPlayingVideo && isStopMusic &&  main->player_->isVideoType == 1)
	{
		main->player_->ExitPlayer();
		KillTimer(IDT_GETINFO_TIIMER);
	}
	
	playerDlg_->ShowWindow(SW_HIDE);
	m_lstPlayList1.ShowWindow_(SW_HIDE);
	m_lstPlayList.ShowWindow_(SW_SHOW);
	m_btnAllSelect.ShowWindow(SW_SHOW);
	m_btnAllClear.ShowWindow(SW_SHOW);
	m_btnOpenFile.ShowWindow(SW_HIDE);
	m_btnPlayMp3.SetToggleValue(FALSE);
	
	m_IsPlay = 0;
	//	m_btnPlayMp3.SetIcon(IDI_ICON_PLAY, CSize(24, 24));
	
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
	m_stcFilename.SetTxt(L"");
}

void CMainMp3Dlg::OnExit()
{
	OnExit_(TRUE);
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
		CString s = lvitem.pszText;
		int n = s.Find(_T(".mp3"));
		int n1 = s.Find(_T(".MP3"));
		int n2 = s.Find(_T(".wav"));
		int n3 = s.Find(_T(".WAV"));
		if((n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
			continue;
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
		CString s = lvitem.pszText;
		int n = s.Find(_T(".mp3"));
		int n1 = s.Find(_T(".MP3"));
		int n2 = s.Find(_T(".wav"));
		int n3 = s.Find(_T(".WAV"));
		if((n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
			continue;
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
			int n = s.Find(_T(".mp3"));
			int n1 = s.Find(_T(".MP3"));
			int n2 = s.Find(_T(".wav"));
			int n3 = s.Find(_T(".WAV"));
			if((n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
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

				char file_dir[128];
				n = wcstombs( file_dir, m_chDir, 128);
				file_dir[n] = '\0';
				sprintf(txt, "File%d=%s%s\r\nLength%d=255\r\n", i,file_dir,filename, i);
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
			m_lstPlayList.ShowWindow_(SW_HIDE);
			m_lstPlayList1.ShowWindow_(SW_SHOW);
			m_btnAllSelect.ShowWindow(SW_HIDE);
			m_btnAllClear.ShowWindow(SW_HIDE);
			m_btnOpenFile.ShowWindow(SW_SHOW);

			m_IsPlay = 1;
			m_btnPlayMp3.SetToggleValue(TRUE);
		}
	}
	else if(m_IsPlay == 1)
	{
		m_IsPlay = 2;
		m_btnPlayMp3.SetToggleValue(FALSE);
//		m_btnPlayMp3.SetIcon(IDI_ICON_PLAY, CSize(24, 24));
	
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->player_->ExitPlayer(TRUE);
	}
	else if(m_IsPlay == 2)
	{
		m_IsPlay = 1;
		m_btnPlayMp3.SetToggleValue(TRUE);
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
//	if(main->player_->type_ && main->player_->isPlaying_ == 1)	//停止播放
	if(main->player_->isPlayingVideo)
	{
		main->player_->ExitPlayer();
	}
	playerDlg_->ShowWindow(SW_HIDE);
	m_lstPlayList1.ShowWindow_(SW_HIDE);
	m_lstPlayList.ShowWindow_(SW_SHOW);
	m_btnAllSelect.ShowWindow(SW_SHOW);
	m_btnAllClear.ShowWindow(SW_SHOW);
	m_btnOpenFile.ShowWindow(SW_HIDE);
	m_btnPlayMp3.SetToggleValue(FALSE);

	m_IsPlay = 0;
	KillTimer(IDT_GETINFO_TIIMER);
	char txt[32];
	sprintf(txt, "%d:%02d/%d:%02d", 0, 0, 0, 0);
	CString s = txt;
	m_stcTime.SetWindowText(s);
	m_stcFilename.SetTxt(L"");
	m_prgPlayTime.SetPos(0);

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
		m_stcFilename.SetTxt(sFilename1);

		//m_stcFilename.SetWindowText(info.szFileName);
		char txt[32];
		sprintf(txt, "%d:%02d/%d:%02d", Hour*60+Min, Sec, Hour1*60+Min1, Sec1);
		CString s = txt;
		m_stcTime.SetWindowText(s);
	}

	CDialog::OnTimer(nIDEvent);
}

void CMainMp3Dlg::OnDeviceChange(WPARAM w, LPARAM l)
{
	if (w == 0x8000) //insert
	{
		::Sleep(500);
		if(DetectDIR(_T("/usbdisk")))
			m_stcFilename.SetTxt(L"USB insert");
		else if(DetectDIR(_T("/storage card")))
			m_stcFilename.SetTxt(L"SD insert");

		if(memcmp(m_chDir, _T("/flashdrv/my_music/"), wcslen(m_chDir)*2) == 0)
			SetPlayList(_T("/flashdrv/my_music/"), 0);

	}
	else if (w == 0x8004) //remove
	{
		::Sleep(1000);
		m_stcFilename.SetTxt(L"remove");
		if(memcmp(m_chDir, _T("/flashdrv/my_music/"), wcslen(m_chDir)*2) == 0)
			SetPlayList(_T("/flashdrv/my_music/"), 0);
	}
	
	//GetLogicalDrives()
}

void CMainMp3Dlg::OnOutEvent(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	if(l == 0)  //暂停播放
	{
		//if(main->player_->isPlaying_ == 1)
		if(main->player_->isPlayingVideo && main->player_->isVideoType == 1)
		{
			main->player_->ExitPlayer(TRUE);
			m_bIsPausebyEvent = TRUE;
		}
	}
	else if(l == 1) //恢复播放
	{
		if(m_bIsPausebyEvent && main->player_->isVideoType == 1)
		{
			m_bIsPausebyEvent = FALSE;
			main->player_->SetOwner(playerDlg_);
			main->player_->InitPlayer();
		}
	}
}
