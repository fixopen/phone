// MainPhotoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainPhotoDlg.h"
#include "../resource.h"
#include "../Data/LanguageResource.h"
#include "../MultimediaPhoneDlg.h"
#include "../MultimediaPhone.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include<algorithm>
/////////////////////////////////////////////////////////////////////////////
// CMainPhotoDlg dialog

CMainPhotoDlg::CMainPhotoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainPhotoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainPhotoDlg)
		// NOTE: the ClassWizard will add member initialization here

	//}}AFX_DATA_INIT
	memset(m_chDir, 0, 128);
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainPhotoDlg message handlers

BOOL CMainPhotoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString s;
	s = Data::LanguageResource::Get(Data::RI_MAIN_POTOBTN).c_str();
	m_stcTitle.Create(s, WS_CHILD|WS_VISIBLE, CRect(4, 0, 80, 15), this);
	m_stcTitle.SetColor(RGB(0, 0, 0), RGB(192, 192, 192));
	
	m_btnExit.Create(L"", WS_CHILD|WS_VISIBLE, CRect(192, 0, 207, 15), this, IDC_BUTTON_PHOTOEXIT);
	
	m_btnExit.SetIcon(IDI_ICON_EXIT, CSize(12, 12));
	s = Data::LanguageResource::Get(Data::RI_VEDIO_ALLBTN).c_str();
	m_btnAllSelect.Create(s, WS_CHILD|WS_VISIBLE,CRect(14, 182, 41, 197), this, IDC_BUTTON_PHOTOALL);
	s = Data::LanguageResource::Get(Data::RI_VEDIO_DELETEALLBTN).c_str();
	m_btnAllClear.Create(s, WS_CHILD|WS_VISIBLE, CRect(44, 182, 71, 197),this,IDC_BUTTON_PHOTOCLEAR);
	s = Data::LanguageResource::Get(Data::RI_VEDIO_OPENBTN).c_str();
	m_btnOpenFile.Create(s, WS_CHILD|WS_VISIBLE,CRect(14, 182, 41, 197), this, IDC_BUTTON_PHOTOOPEN);
	m_btnOpenFile.ShowWindow(SW_HIDE);
	s = Data::LanguageResource::Get(Data::RI_VEDIO_ALLSCREENBTN).c_str();
	m_btnPlayAll.Create(s, WS_CHILD|WS_VISIBLE, CRect(44, 182, 71, 197),this,IDC_BUTTON_PHOTOPLAYALL);
	m_btnPlayAll.ShowWindow(SW_HIDE);
	m_btnPrePhoto.Create(L"", WS_CHILD|WS_VISIBLE, CRect(75, 182, 99, 198), this,IDC_BUTTON_PHOTOPRE);
	m_btnPrePhoto.SetIcon(IDI_ICON_PRE, CSize(24, 16));
	m_btnPlayPhoto.Create(L"", WS_CHILD|WS_VISIBLE, CRect(99, 178, 123, 202), this,IDC_BUTTON_PHOTOPLAY);
	m_btnPlayPhoto.SetIcon(IDI_ICON_PLAY, CSize(24, 24));
	m_btnBackPhoto.Create(L"", WS_CHILD|WS_VISIBLE, CRect(123, 182, 147, 198), this,IDC_BUTTON_PHOTOBACK);
	m_btnBackPhoto.SetIcon(IDI_ICON_NEXT, CSize(24, 16));
	s = Data::LanguageResource::Get(Data::RI_PHOTO_SCREENSAVE).c_str();
	m_btnScreenSave.Create(s, WS_CHILD|WS_VISIBLE, CRect(150, 182, 220, 198), this,IDC_BUTTON_PHOTOSCREENSAVE);
	
	m_lstPlayList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(2, 16, 206, 168), this, IDC_LIST_PLAYPHOTOLIST, TRUE, 1);
	m_lstPlayList.SetListColor(RGB(255, 255, 255), RGB(255, 255, 255));
	m_lstPlayList.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 133);
	m_pImageList = new CImageList();
	m_pImageList->Create(12, 12, ILC_COLOR32|ILC_MASK, 2, 2);   
    HICON hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_CHECK0));   
	m_pImageList->Add(hIcon);  
    hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_CHECK1));   
	m_pImageList->Add(hIcon);   
	m_lstPlayList.SetImageList(m_pImageList, LVSIL_SMALL);

	m_nCountPhoto = SetPlayList("/flashdrv/my_photo/");
	m_lstPlayList.SetScrollRagle(FALSE);
	char stxt[32];
	sprintf(stxt, "%d/%d", 0, m_nCountPhoto);
	s = stxt;
	m_stcCount.Create(s, WS_CHILD|WS_VISIBLE, CRect(140, 168, 213, 182), this);
	m_stcCount.SetColor(RGB(0, 0, 0), RGB(192, 192, 192));

	//播放器
	playerDlg_ = new CPlayerDlg();
	playerDlg_->Create(CPlayerDlg::IDD, this);

	m_backGroudStatic.Create(CRect(0, 0, 204, 240), this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainPhotoDlg ::SetPhoto()
{
	playerDlg_->SetParam("", FALSE, FALSE);
	playerDlg_->ReSetWindowsRect(CRect(2, 16, 206, 168));
}

int CMainPhotoDlg::SetPlayList(char *dir)
{
	m_lstPlayList.DeleteAllItems();
	strcpy(m_chDir, dir);
	strcpy(m_chDir, "/flashdrv/my_photo/");
	WIN32_FIND_DATA FindFileData;			//查找文件时要使用的数据结构
	HANDLE hFind = INVALID_HANDLE_VALUE;	//定义查找句柄
	int ncount = 0;

	char totalfilename[128];
	sprintf(totalfilename, "%s*.*", m_chDir);
	CString sDir = totalfilename;
	hFind = FindFirstFile(sDir, &FindFileData);//使用FindFirstFile函数来开始文件查找
		
	if (hFind == INVALID_HANDLE_VALUE) 
	{
//		printf ("Inval  return 0\n;");
		return 0;
	} 
	else 
	{
		char filename[128];
//		printf ("First file name is %s\n", FindFileData.cFileName);
		int i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
		filename[i] = '\0';
		if(strstr(filename, ".jpg")||strstr(filename, ".JPG")||strstr(filename, ".bmp")||strstr(filename, ".BMP"))
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
			if(strstr(filename, ".jpg")||strstr(filename, ".JPG")||strstr(filename, ".bmp")||strstr(filename, ".BMP"))
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
			return 0;
		}
	}
	
	return ncount;	
}

void CMainPhotoDlg::OnExit()
{
	playerDlg_->ShowWindow(SW_HIDE);
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
		lvitem.lParam=i;   
		lvitem.iImage=1;
		m_lstPlayList.SetItem(&lvitem);
		m_lstPlayList.SetItemData(i, 1);
	}
	char stxt[32];
	sprintf(stxt, "%d/%d", ncount, m_nCountPhoto);
	CString s = stxt;
	m_stcCount.SetWindowText(s);
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
		lvitem.lParam=i;   
		lvitem.iImage=0;
		m_lstPlayList.SetItem(&lvitem);
		m_lstPlayList.SetItemData(i, 0);
	}
	char stxt[32];
	sprintf(stxt, "%d/%d", 0, m_nCountPhoto);
	CString s = stxt;
	m_stcCount.SetWindowText(s);
}

void CMainPhotoDlg::OnClickPlayList(NMHDR* pNMHDR, LRESULT* pResult)
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

	int ncount = m_lstPlayList.GetItemCount();
	int n = 0;
	for(int i = 0; i < ncount; i++)
	{
		if(m_lstPlayList.GetItemData(i) == 1)
			n++;
	}
	char stxt[32];
	sprintf(stxt, "%d/%d", n, m_nCountPhoto);
	CString s = stxt;
	m_stcCount.SetWindowText(s);

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
			int n = wcstombs( filename, s.GetBuffer(128), /*s.GetLength()*/128);
			filename[n] = '\0';
			char txt[128];
			sprintf(txt, "%s%s",m_chDir, filename);
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
	main->player_->Up();
}
//后一条
void CMainPhotoDlg::OnBack()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->player_->Down();
}
//打开播放列表
void CMainPhotoDlg::OnOpenFile()
{
	m_nCountPhoto = SetPlayList("/flashdrv/my_photo/");
	m_lstPlayList.SetScrollRagle(FALSE);

	playerDlg_->ShowWindow(SW_HIDE);
	m_btnAllSelect.ShowWindow(SW_SHOW);
	m_btnAllClear.ShowWindow(SW_SHOW);
	m_btnOpenFile.ShowWindow(SW_HIDE);
	m_btnPlayAll.ShowWindow(SW_HIDE);
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
			char filename[128];
			CString s = m_lstPlayList.GetItemText(i, 0);
			int n = wcstombs( filename, s.GetBuffer(128), /*s.GetLength()*/128);
			filename[n] = '\0';
			char txt[128];
			sprintf(txt, "%s%s",m_chDir, filename);
			
			nFileSelected++;
			char txt1[128];
			sprintf(txt1, "%s\r\n", txt);
			if(nFileSelected == 1)
			{
				file = fopen("/flashdrv/my_photo/screensave.pls", "w+b");
				fwrite(txt1, sizeof(char), strlen(txt1), file);
			}
			else
			{
				fwrite(txt1, sizeof(char), strlen(txt1), file);
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
		CRect rt = CRect(2, 16, 206, 168);
		playerDlg_->MoveWindow(&rt);
	}
}
