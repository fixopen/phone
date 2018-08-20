// MainPhotoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainPhotoDlg.h"
#include "../resource.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../MultimediaPhoneDlg.h"
#include "../MultimediaPhone.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include<algorithm>

extern BOOL DetectDIR(TCHAR *sDir);
/////////////////////////////////////////////////////////////////////////////
// CMainPhotoDlg dialog

CMainPhotoDlg::CMainPhotoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainPhotoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainPhotoDlg)
		// NOTE: the ClassWizard will add member initialization here

	//}}AFX_DATA_INIT
	memset(m_chDir, 0, 128*2);
}


void CMainPhotoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainPhotoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainPhotoDlg, CDialog)
	//{{AFX_MSG_MAP(CMainPhotoDlg)
	ON_BN_CLICKED(IDC_BUTTON_PHOTOEXIT, OnExit)
	ON_BN_CLICKED(IDC_BUTTON_PHOTOALL, OnSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_PHOTOCLEAR, OnClearAll)
	ON_BN_CLICKED(IDC_BUTTON_PHOTOPLAY, OnPlayer)
	ON_BN_CLICKED(IDC_BUTTON_PHOTOPRE, OnPre)
	ON_BN_CLICKED(IDC_BUTTON_PHOTOBACK, OnBack)
	ON_BN_CLICKED(IDC_BUTTON_PHOTOOPEN, OnOpenFile)
	ON_BN_CLICKED(IDC_BUTTON_PHOTOPLAYALL, OnPlayerAll)
	ON_BN_CLICKED(IDC_BUTTON_PHOTOSCREENSAVE, OnScreenSave)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PLAYPHOTOLIST, OnClickPlayList)
	ON_WM_ACTIVATE()
	ON_MESSAGE(WM_USB_MSG, OnDeviceChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMainPhotoDlg ::OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized )
{
	if(nState & WA_ACTIVE )
	{
		m_nCountPhoto = SetPlayList(_T("/flashdrv/my_photo/"), 0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMainPhotoDlg message handlers

BOOL CMainPhotoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString s;
	m_btnExit.Create(L"", Data::g_buttonExitBMPID[0][Data::g_skinstyle], Data::g_buttonExitBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(212, 11, 231, 28), this,  IDC_BUTTON_PHOTOEXIT);
	
	s = Data::LanguageResource::Get(Data::RI_MAIN_POTOBTN).c_str();
	m_stcTitle.Create(s, WS_CHILD|WS_VISIBLE, CRect(14, 10, 231, 28), this);
	m_stcTitle.SetBorder(TRUE);
	m_stcTitle.SetLeftMargin(3);
	m_stcTitle.SetColor(RGB(0, 0, 0), Data::g_staticAppTileBackRGB[Data::g_skinstyle]);

	s = Data::LanguageResource::Get(Data::RI_VEDIO_ALLBTN).c_str();
	m_btnAllSelect.Create(s, Data::g_buttonALLSelectBMPID[0][Data::g_skinstyle], Data::g_buttonALLSelectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE,CRect(12, 178, 43, 197), this, IDC_BUTTON_PHOTOALL);
	m_btnAllSelect.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnAllSelect.SetBmpOffSet(0, 0);
	s = Data::LanguageResource::Get(Data::RI_VEDIO_DELETEALLBTN).c_str();
	//清空
	m_btnAllClear.Create(s, Data::g_buttonALLSelectBMPID[0][Data::g_skinstyle], Data::g_buttonALLSelectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(43, 178, 76, 197),this,IDC_BUTTON_PHOTOCLEAR);
	m_btnAllClear.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnAllClear.SetBmpOffSet(31, 0);
	
	s = Data::LanguageResource::Get(Data::RI_VEDIO_OPENBTN).c_str();
	m_btnOpenFile.Create(s, Data::g_buttonALLSelectBMPID[0][Data::g_skinstyle], Data::g_buttonALLSelectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(12, 178, 43, 197), this, IDC_BUTTON_PHOTOOPEN);
	m_btnOpenFile.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnOpenFile.SetBmpOffSet(0, 0);
	m_btnOpenFile.ShowWindow(SW_HIDE);
	
	s = Data::LanguageResource::Get(Data::RI_VEDIO_ALLSCREENBTN).c_str();
	m_btnPlayAll.Create(s, Data::g_buttonALLSelectBMPID[0][Data::g_skinstyle], Data::g_buttonALLSelectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(43, 178, 76, 197),this,IDC_BUTTON_PHOTOPLAYALL);
	m_btnPlayAll.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnPlayAll.SetBmpOffSet(31, 0);
	m_btnPlayAll.ShowWindow(SW_HIDE);
	
	m_btnPrePhoto.Create(L"", Data::g_buttonPlayBMPID[0][Data::g_skinstyle], Data::g_buttonPlayBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(78, 178, 107, 197), this,IDC_BUTTON_PHOTOPRE);
	m_btnPrePhoto.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnPlayPhoto.Create(L"", Data::g_buttonPlayBMPID[0][Data::g_skinstyle], Data::g_buttonPlayBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(107, 178, 141, 197), this,IDC_BUTTON_PHOTOPLAY);
	m_btnPlayPhoto.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnPlayPhoto.SetBmpOffSet(29,0);
	m_btnBackPhoto.Create(L"", Data::g_buttonPlayBMPID[0][Data::g_skinstyle], Data::g_buttonPlayBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(141, 178, 170, 197), this,IDC_BUTTON_PHOTOBACK);
	m_btnBackPhoto.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnBackPhoto.SetBmpOffSet(63,0);

	s = Data::LanguageResource::Get(Data::RI_PHOTO_SCREENSAVE).c_str();
	m_btnScreenSave.Create(s, WS_CHILD|WS_VISIBLE, CRect(173, 178, 236, 197), this,IDC_BUTTON_PHOTOSCREENSAVE);
	m_btnScreenSave.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnScreenSave.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_btnScreenSave.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, Data::g_allFramBackRGB[Data::g_skinstyle]);

	m_lstPlayList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(15, 28, 231, 164), this, IDC_LIST_PLAYPHOTOLIST, TRUE, 1);
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

	m_nCountPhoto = SetPlayList(_T("/flashdrv/my_photo/"), 0);
	m_lstPlayList.SetScrollRagle(FALSE);
	char stxt[32];
	sprintf(stxt, "%d/%d", 0, m_nCountPhoto);
	s = stxt;
	m_stcCount.Create(L"", WS_CHILD|WS_VISIBLE, CRect(208, 165, 232, 178), this);
	m_stcCount.SetColor(RGB(0, 0, 0), Data::g_allFramBackRGB[Data::g_skinstyle]);

	//播放器
	playerDlg_ = new CPlayerDlg();
	playerDlg_->Create(CPlayerDlg::IDD, this);

	m_backGroudStatic.Create(CRect(0, 0, 240, 204), this);
	m_backGroudStatic.SetInRect(CRect(13, 9, 232, 165));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainPhotoDlg ::SetPhoto()
{
	playerDlg_->SetParam("", 0, FALSE);
	playerDlg_->ReSetWindowsRect(CRect(15, 28, 231, 164));
}

int CMainPhotoDlg::SetPlayList(TCHAR *dir, int local)
{
	int filecount = 0;
	memset(m_chDir, 0, 128*2);
	m_lstPlayList.DeleteAllItems();
	int ncount = 0;
	if(local == 0)
	{
		if(DetectDIR(_T("/usbdisk")))
			m_lstPlayList.InsertItem(ncount++, _T("usbdisk"), 3);
		if(DetectDIR(_T("/storage card")))
			m_lstPlayList.InsertItem(ncount++, _T("storage card"), 3);
		memcpy(m_chDir, _T("/flashdrv/my_photo/"), wcslen(_T("/flashdrv/my_photo/"))*2);
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
		return 0;
	} 
	else 
	{
		char filename[128];
		//		printf ("First file name is %s\n", FindFileData.cFileName);
		int i = wcstombs( filename, FindFileData.cFileName, 128);
		filename[i] = '\0';
		if(strstr(filename, ".bmp")||strstr(filename, ".BMP") || strstr(filename, ".jpg")||strstr(filename, ".JPG"))
		{
			//对文件的大小经行限制 < 1024K
			if(((FindFileData.nFileSizeHigh * MAXDWORD) + FindFileData.nFileSizeLow) < 1024*1024)
			{
				m_lstPlayList.InsertItem(ncount, FindFileData.cFileName, 0);
				m_lstPlayList.SetItemData(ncount++, 0);
				filecount++;
			}
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
			if(strstr(filename, ".bmp")||strstr(filename, ".BMP") || strstr(filename, ".jpg")||strstr(filename, ".JPG"))
			{
				//对文件的大小经行限制 < 500K
				if(((FindFileData.nFileSizeHigh * MAXDWORD) + FindFileData.nFileSizeLow) < 1200*1024)
				{
					m_lstPlayList.InsertItem(ncount, FindFileData.cFileName, 0);
					m_lstPlayList.SetItemData(ncount++, 0);
					filecount++;
				}
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
			return 0;
		}
	}
	m_lstPlayList.SetScrollRagle(TRUE);

	return filecount;	
}

void CMainPhotoDlg::OnExit()
{
	playerDlg_->ShowWindow(SW_HIDE);
	m_btnAllSelect.ShowWindow(SW_SHOW);
	m_btnAllClear.ShowWindow(SW_SHOW);
	m_btnOpenFile.ShowWindow(SW_HIDE);
	m_btnPlayAll.ShowWindow(SW_HIDE);
	
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
}

void CMainPhotoDlg::OnSelectAll()
{
	int ncount = m_lstPlayList.GetItemCount();
	
	LVITEM lvitem;
	lvitem.mask=LVIF_TEXT   |   LVIF_IMAGE;   
	
	for(int i = 0; i < ncount; i++)
	{
		lvitem.iItem=i;   
		lvitem.iSubItem=0;   
		lvitem.pszText = m_lstPlayList.GetItemText(i, 0).GetBuffer(128);  
		CString s = lvitem.pszText ;
		int n = s.Find(_T(".bmp"));
		int n1 = s.Find(_T(".BMP"));
		int n2 = s.Find(_T(".jpg"));
		int n3 = s.Find(_T(".JPG"));
		if((n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
			continue;

		lvitem.lParam=i;   
		lvitem.iImage=1;
		m_lstPlayList.SetItem(&lvitem);
		m_lstPlayList.SetItemData(i, 1);
	}
// 	char stxt[32];
// 	sprintf(stxt, "%d/%d", ncount, m_nCountPhoto);
// 	CString s = stxt;
// //	m_stcCount.SetWindowText(s);
}

void CMainPhotoDlg::OnClearAll()
{
	int ncount = m_lstPlayList.GetItemCount();

	LVITEM lvitem;
	lvitem.mask=LVIF_TEXT   |   LVIF_IMAGE;   
	
	for(int i = 0; i < ncount; i++)
	{
		lvitem.iItem=i;   
		lvitem.iSubItem=0;   
		lvitem.pszText = m_lstPlayList.GetItemText(i, 0).GetBuffer(128); 
		CString s = lvitem.pszText ;
		int n = s.Find(_T(".bmp"));
		int n1 = s.Find(_T(".BMP"));
		int n2 = s.Find(_T(".jpg"));
		int n3 = s.Find(_T(".JPG"));
		if((n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
			continue;
		lvitem.lParam=i;   
		lvitem.iImage=0;
		m_lstPlayList.SetItem(&lvitem);
		m_lstPlayList.SetItemData(i, 0);
	}
// 	char stxt[32];
// 	sprintf(stxt, "%d/%d", 0, m_nCountPhoto);
// 	CString s = stxt;
// //	m_stcCount.SetWindowText(s);
}

void CMainPhotoDlg::OnClickPlayList(NMHDR* pNMHDR, LRESULT* pResult)
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
			int n = s.Find(_T(".bmp"));
			int n1 = s.Find(_T(".BMP"));
			int n2 = s.Find(_T(".jpg"));
			int n3 = s.Find(_T(".JPG"));
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

	int ncount = m_lstPlayList.GetItemCount();
	int n = 0;
	for(int i = 0; i < ncount; i++)
	{
		if(m_lstPlayList.GetItemData(i) == 1)
			n++;
	}
	m_nCountPhoto = n;
// 	char stxt[32];
// 	sprintf(stxt, "%d/%d", 1, m_nCountPhoto);
// 	CString s = stxt;
// 	m_stcCount.SetWindowText(s);

	*pResult = 0;
}

//播放
void CMainPhotoDlg::OnPlayer()
{
	m_PhotoList.clear();

	int nFileSelected = 0;
	int ncount = m_lstPlayList.GetItemCount();

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

			char txt[128];
			sprintf(txt, "%s%s",file_dir, filename);

			m_PhotoList.push_back(txt);
			nFileSelected++;
		}
	}

	if(nFileSelected != 0)
	{
		std::sort(m_PhotoList.begin(), m_PhotoList.end());
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->player_->SetImageList(m_PhotoList);

		//调用播放窗口
		if(playerDlg_)
		{
			SetPhoto();
			playerDlg_->ShowWindow(SW_SHOW);
		}

		m_btnAllSelect.ShowWindow(SW_HIDE);
		m_btnAllClear.ShowWindow(SW_HIDE);
		m_btnOpenFile.ShowWindow(SW_SHOW);
		m_btnPlayAll.ShowWindow(SW_SHOW);
	}
}
//前一条
void CMainPhotoDlg::OnPre()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int n = main->player_->Up();
// 	char stxt[32];
// 	sprintf(stxt, "%d/%d", n, m_nCountPhoto);
// 	CString s = stxt;
// //	m_stcCount.SetWindowText(s);
}
//后一条
void CMainPhotoDlg::OnBack()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int n = main->player_->Down();
// 	char stxt[32];
// 	sprintf(stxt, "%d/%d", n, m_nCountPhoto);
// 	CString s = stxt;
// //	m_stcCount.SetWindowText(s);
}
//打开播放列表
void CMainPhotoDlg::OnOpenFile()
{
	playerDlg_->ShowWindow(SW_HIDE);
	m_btnAllSelect.ShowWindow(SW_SHOW);
	m_btnAllClear.ShowWindow(SW_SHOW);
	m_btnOpenFile.ShowWindow(SW_HIDE);
	m_btnPlayAll.ShowWindow(SW_HIDE);
	m_stcCount.SetWindowText(L"");
}

//全屏播放
void CMainPhotoDlg::OnPlayerAll()
{
	GetParent()->SendMessage(WM_PLAYPHOTO, 2);	//全屏
	playerDlg_->SetActiveWindow();
}

void CMainPhotoDlg::OnScreenSave()
{
	int nFileSelected = 0;
	int ncount = m_lstPlayList.GetItemCount();
	FILE *file =NULL;
	for(int i = 0; i < ncount; i++)
	{
		int val = m_lstPlayList.GetItemData(i);
		if(val == 1)
		{
			/*
			char filename[128];
			CString s = m_lstPlayList.GetItemText(i, 0);
			int n = wcstombs( filename, s.GetBuffer(128), 128);
			filename[n] = '\0';
			*/
			char txt[128];
			char filename[128];
			CString s = m_lstPlayList.GetItemText(i, 0);
			int n = wcstombs( filename, s.GetBuffer(128), 128/*s.GetLength()*/);
			filename[n] = '\0';
			
			char file_dir[128];
			n = wcstombs( file_dir, m_chDir, 128);
			file_dir[n] = '\0';
			sprintf(txt, "%s%s\r\n", file_dir, filename);
			
			nFileSelected++;
				if(nFileSelected == 1)
			{
				file = fopen("/flashdrv/my_photo/screensave.pls", "w+b");
				fwrite(txt, sizeof(char), strlen(txt), file);
			}
			else
			{
				fwrite(txt, sizeof(char), strlen(txt), file);
			}
		}
	}
	if(file)
		fclose(file);
}

void CMainPhotoDlg::SetCtrlEnable(BOOL flag)
{
	
}

void CMainPhotoDlg ::SetAllScreenPlayer(BOOL flag)
{
	if(flag)
	{
		CRect rt = CRect(0, 0, 480, 234);
		playerDlg_->MoveWindow(&rt);
	}
	else
	{
		CRect rt = CRect(15, 28, 231, 164);
		playerDlg_->MoveWindow(&rt);
		Invalidate();   //lxz 2007 11 30
	}
}

void CMainPhotoDlg::OnDeviceChange(WPARAM w, LPARAM l)
{
	if (w == 0x8000) //insert
	{
		::Sleep(500);
	
		if(memcmp(m_chDir, _T("/flashdrv/my_photo/"), wcslen(m_chDir)*2) == 0)
			SetPlayList(_T("/flashdrv/my_photo/"), 0);
		
	}
	else if (w == 0x8004) //remove
	{
		::Sleep(1000);
		if(memcmp(m_chDir, _T("/flashdrv/my_photo/"), wcslen(m_chDir)*2) == 0)
			SetPlayList(_T("/flashdrv/my_photo/"), 0);
	}
}

