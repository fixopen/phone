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

static BOOL gbPhotoPauseCount = 0;
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
	m_nPhotoType = local_type;
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
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
//	ON_MESSAGE(WM_LISTCTRL_CLICK, OnListCltrlClick)
	ON_MESSAGE(WM_OUTEVENT, OnOutEvent)
	ON_MESSAGE(WM_LISTCTRL_CLICKSUBITEM, OnListCltrlClick)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMainPhotoDlg::OnListCltrlClick(WPARAM w, LPARAM l)
{
	LRESULT ret;
//	if(w == IDC_LIST_PLAYPHOTOLIST)
	{
		if(m_nPhotoType == net_type)
		{
			if(l == 1)
			{
				/*
				if(w >= 0 && w <= (8-1))
				{
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
					std::string strTemp = ".\\adv\\mjpg\\k1\\common\\ȷ��ɾ����.bmp";
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
				}
				*/
			}
			else
			{
				OnClickPlayList(NULL, &ret);
			}
		}
		else
			OnClickPlayList(NULL, &ret);
	}
}

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
	
	m_lstPlayList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(18, 56, 582, 296), this, IDC_LIST_PLAYPHOTOLIST, TRUE, 1);
	m_lstPlayList.SetExtendedStyle(m_lstPlayList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lstPlayList.SetListColor(Data::g_listctrlBackRGB1[Data::g_skinstyle], Data::g_listctrlBackRGB2[Data::g_skinstyle]);
	m_lstPlayList.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 564-34-60);
	m_lstPlayList.InsertColumn(1, _T("delete"), LVCFMT_LEFT, 60);
	
	m_pImageList = new CImageList();
	m_pImageList->Create(32, 32, ILC_COLOR32|ILC_MASK, 2, 2);   
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

	/*
	char stxt[32];
	sprintf(stxt, "%d/%d", 0, m_nCountPhoto);
	s = stxt;
	m_stcCount.Create(L"", WS_CHILD|WS_VISIBLE, CRect(208, 165, 232, 178), this);
	m_stcCount.SetColor(RGB(0, 0, 0), Data::g_allFramBackRGB[Data::g_skinstyle]);
	*/

	//������
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	playerDlg_ = new CPlayerDlg(main->playerimage_);
	playerDlg_->Create(CPlayerDlg::IDD, this);

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 600, 420), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\����\\���1.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 600, 420));

	m_MJPGList.SetUnitIsShow(3, TRUE);
	m_MJPGList.SetUnitIsShow(4, TRUE);
	m_MJPGList.SetUnitIsShow(11, FALSE);
	m_MJPGList.SetUnitIsShow(10, FALSE);

//	SetTimer(1, 10*1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainPhotoDlg ::SetPhoto()
{
	playerDlg_->SetParam("", mtImage, FALSE);
	playerDlg_->ReSetWindowsRect(CRect(/*7, 50, 7+522, 50+213*/18, 56, 582, 296));
}

int CMainPhotoDlg::SetPlayList(TCHAR *dir, int local)
{
	int filecount = 0;
	memset(m_chDir, 0, 128*2);
	m_lstPlayList.DeleteAllItems();
	int ncount = 0;
/*
	if(m_nPhotoType == net_type)   //������Ƭ
	{
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

		std::string spCode = "spCode = '";
		spCode += main->m_pSettingDlg->m_pSetting->speCode2_;
		spCode += "'";
		m_pMediaFileresult = Data::MultimediaDownload::GetFromDatabase(spCode);
		filecount = m_pMediaFileresult.size();
		if(filecount > 0);
		{
			for(int i = 0; i < filecount; i++)
			{
				CString s = m_pMediaFileresult[i]->multimediaInfos.filename.c_str();
				
				CString filename = "";
				CString path = "";
				
				extern int FindFileEx(CString s, CString &sPath, CString &sFilename); 
				FindFileEx(s, path, filename);
				m_lstPlayList.InsertItem(i, filename, 0);
				m_lstPlayList.SetItemText(i, 1, L"ɾ��");
				m_lstPlayList.SetItemData(i, 0);
			}

		}	
		m_lstPlayList.SetScrollRagle(TRUE);
		return filecount;	
	}
*/
	
	if(local == 0)
	{
		if(m_nPhotoType == net_type)
		{
			memcpy(m_chDir, _T("/flashdrv/my_net_photo/"), wcslen(_T("/flashdrv/my_net_photo/"))*2);
		}
		else
		{
			if(DetectDIR(_T("/usbdisk")))
				m_lstPlayList.InsertItem(ncount++, _T("usbdisk"), 3);
			if(DetectDIR(_T("/storagecard")))
				m_lstPlayList.InsertItem(ncount++, _T("storagecard"), 3);
			memcpy(m_chDir, _T("/flashdrv/my_photo/"), wcslen(_T("/flashdrv/my_photo/"))*2);
		}
	}
	
	else
	{
		CString s = Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM).c_str();
		m_lstPlayList.InsertItem(ncount++, s, 4);
		memcpy(m_chDir, dir, wcslen(dir)*2);
	}
	CString sDir = m_chDir;
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
		if(/*strstr(filename, ".bmp")||strstr(filename, ".BMP") ||*/ strstr(filename, ".jpg")||strstr(filename, ".JPG"))
		{
			//���ļ��Ĵ�С�������� < 1024K
			//if(((FindFileData.nFileSizeHigh * MAXDWORD) + FindFileData.nFileSizeLow) < 1024*1024)
			{
				m_lstPlayList.InsertItem(ncount, FindFileData.cFileName, 0);
			//	m_lstPlayList.SetItemText(ncount, 1, L"ɾ��");
				m_lstPlayList.SetItemData(ncount++, 0);

				filecount++;
			}
		}
		else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			CString sS;
			CString s = Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM).c_str();
			sS = m_lstPlayList.GetItemText(0, 0);
			if(sS == s)
				m_lstPlayList.InsertItem(1, FindFileData.cFileName, 2);
			else 
				m_lstPlayList.InsertItem(0, FindFileData.cFileName, 2);
			ncount++;
		}
		
		//������ѭ��ʹ��FindNextFile�����������ļ�
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			//			Dprintf ("Next file name is %s\n", FindFileData.cFileName);
			i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
			filename[i] = '\0';
			if(/*strstr(filename, ".bmp")||strstr(filename, ".BMP") ||*/ strstr(filename, ".jpg")||strstr(filename, ".JPG"))
			{
				//���ļ��Ĵ�С�������� < 500K
				//if(((FindFileData.nFileSizeHigh * MAXDWORD) + FindFileData.nFileSizeLow) < 1200*1024)
				{
					m_lstPlayList.InsertItem(ncount, FindFileData.cFileName, 0);
				//	m_lstPlayList.SetItemText(ncount, 1, L"ɾ��");
					m_lstPlayList.SetItemData(ncount++, 0);
					filecount++;
				}
			}
			else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				CString sS;
				CString s = Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM).c_str();
				sS = m_lstPlayList.GetItemText(0, 0);
				if(sS == s)
					m_lstPlayList.InsertItem(1, FindFileData.cFileName, 2);
				else 
					m_lstPlayList.InsertItem(0, FindFileData.cFileName, 2);
				ncount++;
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
	m_lstPlayList.SetScrollRagle(TRUE);

	return filecount;	
}

void CMainPhotoDlg::OnExit()
{
	KillTimer(0x100);
	playerDlg_->StopTimer();
	playerDlg_->ShowWindow(SW_HIDE);
	playerDlg_->player_->ExitPlayer(TRUE);

	m_MJPGList.SetUnitIsShow(3, TRUE);
	m_MJPGList.SetUnitIsShow(4, TRUE);
	m_MJPGList.SetUnitIsShow(11, FALSE);
	m_MJPGList.SetUnitIsShow(10, FALSE);

	m_MJPGList.SetUnitIsShow(9, TRUE);

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
		lvitem.pszText = (LPTSTR)(LPCTSTR)m_lstPlayList.GetItemText(i, 0)/*.GetBuffer(128)*/;  
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
		lvitem.pszText = (LPTSTR)(LPCTSTR)m_lstPlayList.GetItemText(i, 0)/*.GetBuffer(128)*/; 
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
/*
		if(m_nPhotoType == net_type)			//������Ƭ
		{
			int id = m_lstPlayList.GetItemData(index);
			id=(id == 0)?1:0;
			LVITEM lvitem;
			lvitem.mask=LVIF_TEXT | LVIF_IMAGE;   
			lvitem.iItem=index;   
			lvitem.iSubItem=0;   
			lvitem.pszText = (LPTSTR)(LPCTSTR)s;//(LPTSTR)(LPCTSTR)m_lstPlayList.GetItemText(index, 0);  
			lvitem.lParam=index;   
			lvitem.iImage=id;
			m_lstPlayList.SetItem(&lvitem);
			m_lstPlayList.SetItemData(index, id);
			
			return;
		}
*/
		//usb
		if(s.Compare(_T("usbdisk")) == 0)
		{
			SetPlayList(_T("/usbdisk/"), 1);
		}
		//sd
		else if(s.Compare(_T("storagecard")) == 0)
		{
			SetPlayList(_T("/storagecard/"), 1);
		}
		//��һ��
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
				SetPlayList((TCHAR *)(LPCTSTR)sDir/*.GetBuffer(128)*/, 0);
			else
				SetPlayList((TCHAR *)(LPCTSTR)sDir/*.GetBuffer(128)*/, 1);
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
				SetPlayList((TCHAR *)(LPCTSTR)sDir/*.GetBuffer(128)*/, 1);
			}
			else
			{
				int id = m_lstPlayList.GetItemData(index);
				id=(id == 0)?1:0;
				LVITEM lvitem;
				lvitem.mask=LVIF_TEXT | LVIF_IMAGE;   
				lvitem.iItem=index;   
				lvitem.iSubItem=0;   
				lvitem.pszText = (LPTSTR)(LPCTSTR)s;//(LPTSTR)(LPCTSTR)m_lstPlayList.GetItemText(index, 0)/*.GetBuffer(128)*/;  
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

//����
void CMainPhotoDlg::OnPlayer(int index)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->CancelBalckLightSaveTimer();

	m_PhotoList.clear();

	int nFileSelected = 0;
	int ncount = m_lstPlayList.GetItemCount();

	for(int i = 0; i < ncount; i++)
	{
		int val = m_lstPlayList.GetItemData(i);
		if(val == 1)
		{
		//	if(m_nPhotoType == local_type)			//������Ƭ
			{
				char filename[128];
				CString s = m_lstPlayList.GetItemText(i, 0);
				int n = wcstombs( filename, (LPTSTR)(LPCTSTR)s/*.GetBuffer(128)*/, 128/*s.GetLength()*/);
				filename[n] = '\0';
				
				char file_dir[128];
				n = wcstombs( file_dir, m_chDir, 128);
				file_dir[n] = '\0';
				
				char txt[128];
				sprintf(txt, "%s%s",file_dir, filename);
				
				m_PhotoList.push_back(txt);
				nFileSelected++;
			}
			/*
			else
			{
				CString s = m_pMediaFileresult[i]->multimediaInfos.filename.c_str();
				m_PhotoList.push_back(s);
				nFileSelected++;
			}
			*/
		}
	}

	if(nFileSelected != 0)
	{
		gbPhotoPauseCount = 0;
	//	std::sort(m_PhotoList.begin(), m_PhotoList.end());
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->playerimage_->SetImageList(m_PhotoList, index);
		
		m_MJPGList.SetUnitIsShow(9, FALSE);
		m_MJPGList.Invalidate();

		//���ò��Ŵ���
		if(playerDlg_)
		{
			SetPhoto();
			playerDlg_->ShowWindow(SW_SHOW);
		}

		m_MJPGList.SetUnitIsShow(3, FALSE);
		m_MJPGList.SetUnitIsShow(4, FALSE);
		m_MJPGList.SetUnitIsShow(11, TRUE);
		m_MJPGList.SetUnitIsShow(10, TRUE);
	}
	playerDlg_->SetResumeTimer();
}
//ǰһ��
void CMainPhotoDlg::OnPre()
{
	if(m_MJPGList.GetUnitIsShow(11))
	{
		gbPhotoPauseCount = 0;
		playerDlg_->OnKeyDown('U', 0 , 0);
	}
}
//��һ��
void CMainPhotoDlg::OnBack()
{
	if(m_MJPGList.GetUnitIsShow(11))
	{
		gbPhotoPauseCount = 0;
		playerDlg_->OnKeyDown('D', 0 , 0);
	}
}
//�򿪲����б�
void CMainPhotoDlg::OnOpenFile()
{
	playerDlg_->player_->ExitPlayer(TRUE);

	playerDlg_->StopTimer();
	playerDlg_->ShowWindow(SW_HIDE);

	m_MJPGList.SetUnitIsShow(3, TRUE);
	m_MJPGList.SetUnitIsShow(4, TRUE);
	m_MJPGList.SetUnitIsShow(11, FALSE);
	m_MJPGList.SetUnitIsShow(10, FALSE);

	m_MJPGList.SetUnitIsShow(9, TRUE);

	//m_stcCount.SetWindowText(L"");
}

//ȫ������
void CMainPhotoDlg::OnPlayerAll()
{
	GetParent()->SendMessage(WM_PLAYPHOTO, 2);	//ȫ��
	playerDlg_->SetActiveWindow();

	::SetCursorPos(799, 479);
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
			int n = wcstombs( filename, (LPTSTR)(LPCTSTR)s/*.GetBuffer(128)*/, 128/*s.GetLength()*/);
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
		CRect rt = CRect(0, 0, 800, 480);
		playerDlg_->MoveWindow(&rt);
	}
	else
	{
		CRect rt = CRect(18, 56, 582, 296);
		playerDlg_->MoveWindow(&rt);
	}
	
	playerDlg_->player_->SetAllScreenPlayer(flag);
	Invalidate();   //lxz 2007 11 30
}

void CMainPhotoDlg::OnDeviceChange(WPARAM w, LPARAM l)
{
	if (w == 0x8000) //insert
	{
		SetTimer(100, 2000, NULL);

// 		::Sleep(500);
// 	
// 		if(memcmp(m_chDir, _T("/flashdrv/my_photo/"), wcslen(m_chDir)*2) == 0)
// 			SetPlayList(_T("/flashdrv/my_photo/"), 0);
// 		
	}
	else if (w == 0x8004) //remove
	{
		SetTimer(100, 2000, NULL);
// 		::Sleep(1000);
// 		if(memcmp(m_chDir, _T("/flashdrv/my_photo/"), wcslen(m_chDir)*2) == 0)
// 			SetPlayList(_T("/flashdrv/my_photo/"), 0);
	}
}

void CMainPhotoDlg ::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 1:			//ȫѡ
		OnSelectAll();
		break;
	case 2:         //��һ��
		OnPre();
		break;
	case 3:			//����
		OnPlayer();
		break;      
	case 4:			//��һ��
		OnBack();
		break;
	case 5:			//���
		OnClearAll();
		break;
	case 6:			//����������
		OnScreenSave();
		break;
	case 7:			//�˳�
		OnExit();
		break;
	case 8:			//��
		OnOpenFile();
		break;
	case 9:			//ȫ��
		OnPlayerAll();
		break;
	case 100:		//������Ƭ&������Ƭ�����л�
		{
			/*
			BOOL flag = m_MJPGList.GetUnitIsDownStatus(9);
			m_MJPGList.SetUnitIsDownStatus(9, !flag);
			m_MJPGList.SetUnitIsShow(9, TRUE, TRUE);
			m_nPhotoType = (PhotoType)(!flag);
			SetPlayList(L"/flashdrv/my_photo/", 0);
			break;
			*/
		}
	}
}


void CMainPhotoDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1 && IsWindowVisible() && playerDlg_->IsWindowVisible())
	{
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->playerimage_->Down();
	}
	else if(nIDEvent == 0x100)
	{
		KillTimer(0x100);
		if(IsWindowVisible())
		{
			OnPlayer(-1);
			//CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
			//playerDlg_->ShowWindow(SW_SHOW);
			//main->playerimage_->Down();
		}
	}
	else if(nIDEvent == 100)
	{
		KillTimer(100);
		if(memcmp(m_chDir, _T("/flashdrv/my_photo/"), wcslen(m_chDir)*2) == 0)
 			SetPlayList(_T("/flashdrv/my_photo/"), 0);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CMainPhotoDlg::OnOutEvent(WPARAM w, LPARAM l)
{
	if(IsWindowVisible())
	{
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		if(l == 0 && main->playerimage_->isPlaying_)  //��ͣ����
		{
			gbPhotoPauseCount = 1;
			//OnOpenFile();
			playerDlg_->StopTimer();
			playerDlg_->ShowWindow(SW_HIDE);
			playerDlg_->player_->ExitPlayer(TRUE);	
		}
		else if(l == 1) //�ָ�����
		{
			//if(gnPhotoPauseCount <= 0)    //���ܽ����ͻ����.   //20090223
			if(gbPhotoPauseCount && !main->m_pNetStatusDlg->IsWindowVisible() && !main->m_AlarmShowDlg->IsWindowVisible() && !main->m_pTelephoneDlg->IsWindowVisible())
			{
				gbPhotoPauseCount = 0;
				SetTimer(0x100, 5, NULL);
				/*   �Ƶ�timer 0x100
				playerDlg_->ShowWindow(SW_SHOW);
				main->playerimage_->Down();
				*/
			}
		}
	}
}

LRESULT CMainPhotoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_DELETESELITEM:
		{
			
			POSITION pos = m_lstPlayList.GetFirstSelectedItemPosition();
			if (pos != NULL)
			{
				int index = m_lstPlayList.GetNextSelectedItem (pos);
				m_pMediaFileresult[index]->Remove();
				CString s = m_pMediaFileresult[index]->multimediaInfos.filename.c_str();
			
				CString filename = "";
				CString path = "";
				extern int FindFileEx(CString s, CString &sPath, CString &sFilename);
				FindFileEx(s, path, filename);
				extern void DeleteDirectory(CString SrcDir, BOOL isShow = TRUE);
				DeleteDirectory(path);

				SetPlayList(L"/flashdrv/my_photo/", 0);
			}
		}
		break;
	default:
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}