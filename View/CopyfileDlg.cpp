// copyfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/SkinStyle.h"
#include "../Data/LanguageResource.h"
#include "../Pblmember.h"
#include "copyfileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const TCHAR *ptchExtDb[] = {_T("/UsbDisk/mediaphone/mmp.db"), _T("/StorageCard/mediaphone/mmp.db")};
const TCHAR *ptchExtMp3[] = {_T("/UsbDisk/mediaphone/my_music"), _T("/StorageCard/mediaphone/my_music")};
const TCHAR *ptchExtPhoto[] = {_T("/UsbDisk/mediaphone/my_photo"), _T("/StorageCard/mediaphone/my_photo")};
const TCHAR *ptchExtBell[] = {_T("/UsbDisk/mediaphone/my_ring"), _T("/StorageCard/mediaphone/my_ring")};
const TCHAR *ptchExtShow[] = {_T("/UsbDisk/mediaphone/my_show"), _T("/StorageCard/mediaphone/my_show")};

static enum {usb_device, sdcard_device, none_device} m_devStatus;
static TCHAR m_chExtPath[64];
BOOL g_bOverWrite = FALSE;
/////////////////////////////////////////////////////////////////////////////
// copyfileDlg dialog

copyfileDlg::copyfileDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(copyfileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(copyfileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nPos = 0;
	m_devStatus = none_device;
	memset(m_chExtPath, 0, sizeof(m_chExtPath));
}


void copyfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(copyfileDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void copyfileDlg::OnDeviceChange(WPARAM w, LPARAM l)
{
	if (w == 0x8000) 
	{
				
	}
	else if (w == 0x8004) //remove
	{
		//BOOL bUsb = DetectDIR(_T("/UsbDisk"));
		//if( bUsb || DetectDIR(L"/StorageCard"))
		//{
		BOOL bExitFlag = FALSE;
		Sleep(2000);
		if(m_devStatus == usb_device)
		{
			BOOL bUsb = DetectDIR(_T("/UsbDisk"));
			if(!bUsb)
			{
				bExitFlag = TRUE;	
			}
		}

		else if(m_devStatus == sdcard_device)
		{
			BOOL bSD = DetectDIR(_T("/StorageCard"));
			if(!bSD)
			{
				bExitFlag = TRUE;	
			}
		}
		Dprintf("copy file is %d\r\n", bExitFlag);

		if(bExitFlag)
		{
			OnButtonCancel();
		}
	}
	
	//GetLogicalDrives()
}

BEGIN_MESSAGE_MAP(copyfileDlg, CCEDialog)
	//{{AFX_MSG_MAP(copyfileDlg)
		// NOTE: the ClassWizard will add message map macros here
	ON_BN_CLICKED(IDC_BUTTON_FIREWALL_CANCEL, OnButtonCancel)
	ON_MESSAGE(WM_USB_MSG, OnDeviceChange)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
END_MESSAGE_MAP()

void copyfileDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	// 	switch(w)
	// 	{
	// 	case 1:
	// 		OnButtonOk();
	// 		break;
	// 	case 2:
	// 		OnButtonCancel();
	// 		break;
	// 	}
	
	switch(w)
	{
	case 1:
	//	OnButtonOk();
		Import(TRUE);
			break;
	case 2:
	case 5:
		OnButtonCancel();	 // no 4
		break;
	case 3:			//替换     no 5
		Import(TRUE);
		break;
	case 4:			//添加     no  6
		Import(FALSE);
		break;		
	}
	
}
BOOL copyfileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	std::string strTemp;
	CString str;
/*
	m_sticTitle.Create(L"", WS_CHILD|WS_VISIBLE, CRect(42 + 54, 70 + 62, 410 + 54, 102 + 62), this);
	m_sticTitle.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
*/
	m_procbarSound.Create(WS_CHILD, CRect(57 + 54, 190 + 62, 522 + 54, 217 + 62), this, 0xFF10);
	m_procbarSound.SetParam(0, 0, 4, 1);
	m_procbarSound.SetPos(0);
	
/*
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78, 434, 98), this, IDC_BUTTON_FIREWALL_OK);
	m_btnOk.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103, 434, 123), this, IDC_BUTTON_FIREWALL_CANCEL);
	m_btnCancel.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	m_sticBackground.Create(CRect(34, 40, 448, 184), this, 1);
	
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 160, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE).c_str(), Data::LanguageResource::Get(Data::RI_SOUND_PLAY_TITLE).length());
	
	m_sticBackground.SetTextStruct(ts, 1);
	*/
		
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54, 62, 746, 358), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\文件备份.xml");
	m_MJPGList.SetMJPGRect(CRect(54, 62, 746, 358));

//	m_MJPGList.SetUnitIsShow(0, FALSE);
	m_MJPGList.SetUnitIsShow(1, FALSE);
	m_MJPGList.SetUnitIsShow(2, FALSE);
	m_MJPGList.SetUnitIsShow(3, FALSE);
	m_MJPGList.SetUnitIsShow(4, FALSE);
	m_MJPGList.SetUnitIsShow(5, FALSE);
	m_MJPGList.SetUnitIsShow(6, FALSE);
	m_MJPGList.SetUnitIsShow(7, FALSE);
	return TRUE;
}

void copyfileDlg::SetType(operater_type type)
{
	type_ = type;
	m_procbarSound.ShowWindow(SW_HIDE);
//	m_MJPGList.SetUnitIsShow(2, FALSE);
	
	BOOL bUsb = DetectDIR(_T("/UsbDisk"));
	if( bUsb || DetectDIR(L"/StorageCard"))
	{
		if (bUsb)
			m_devStatus = usb_device;
		else
			m_devStatus = sdcard_device;

		progress_status_ = isexitfile_status;
		std::string strTemp;
		CString str;
		
		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR2);
		str = strTemp.c_str();
		m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
		
		switch(type_)
		{
		case backup_file:
		case restore_file:
		case copy_file:
		case mp3_out:
		case photo_out:
		case bell_out:
		case show_out:
			m_MJPGList.SetUnitIsShow(3, TRUE);
			m_MJPGList.SetUnitIsShow(4, TRUE);
			m_MJPGList.SetUnitIsShow(5, FALSE);
			m_MJPGList.SetUnitIsShow(6, FALSE);
			m_MJPGList.SetUnitIsShow(7, FALSE);
			m_MJPGList.SetUnitIsDisable(3, FALSE);
			break;
		case mp3_in:
		case photo_in:
		case bell_in:
		case show_in:
			m_MJPGList.SetUnitIsShow(3, FALSE);
			m_MJPGList.SetUnitIsShow(4, FALSE);
			m_MJPGList.SetUnitIsShow(5, TRUE);
			m_MJPGList.SetUnitIsShow(6, TRUE);
			m_MJPGList.SetUnitIsShow(7, TRUE);
			m_MJPGList.SetUnitIsDisable(3, FALSE);
		default:
			break;
		}

	}
	else 
	{
		m_devStatus = none_device;
		memset(m_chExtPath, 0, sizeof(m_chExtPath));

		progress_status_ = nousb_status;
		std::string strTemp;
		CString str;
		
		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR1);
		str = strTemp.c_str();
		m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
		
		m_MJPGList.SetUnitIsDisable(3, FALSE);
		m_MJPGList.SetUnitIsShow(3, TRUE);
		m_MJPGList.SetUnitIsShow(4, TRUE);
		m_MJPGList.SetUnitIsShow(5, FALSE);
		m_MJPGList.SetUnitIsShow(6, FALSE);
		m_MJPGList.SetUnitIsShow(7, FALSE);
	}
}

void copyfileDlg::Import(BOOL bOverflow)
{
	m_MJPGList.SetUnitIsShow(3, TRUE);
	m_MJPGList.SetUnitIsShow(4, TRUE);
	m_MJPGList.SetUnitIsShow(5, FALSE);
	m_MJPGList.SetUnitIsShow(6, FALSE);
	m_MJPGList.SetUnitIsShow(7, FALSE);
	m_MJPGList.Invalidate();

	std::string strTemp;
	CString str;
				
	if(progress_status_ == nousb_status || progress_status_ == finish_status)
	{
		KillTimer(IDT_COPYFILE_TIMER);
		progress_status_ = start_status;
		ShowWindow_(SW_HIDE);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetScreenSaveTimer();
	}
	else if(progress_status_ == isexitfile_status)
	{
		TCHAR tchFile[MAX_PATH] = {0};
		tchLocalFile[0] = L'\0';
		int nFlags = 0;

		switch(type_ )
		{
		case backup_file:
		case copy_file:
		case mp3_out:
		case photo_out:
		case bell_out:
		case show_out:
// 			m_MJPGList.SetUnitIsShow(2, TRUE);
// 			StartCopyFile();
			break;
		case restore_file:
			{		
// 				if(!DetectFile(L"/UsbDisk/mediaphone/backup/mmp.db"))
// 				{
// 					progress_status_ = finish_status;
// 					
// 					strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
// 					str = strTemp.c_str();
// 					m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
// 				}
// 				else			//copy
// 				{
// 					m_MJPGList.SetUnitIsShow(2, TRUE);
// 					StartCopyFile();
// 					
// 				}
				lstrcpy(m_chExtPath, ptchExtDb[m_devStatus]);
				//lstrcpy(tchFile, ptchExtDb);
				nFlags = 1;
			}
			break;
		case mp3_in:
			lstrcpy(m_chExtPath, ptchExtMp3[m_devStatus]);
			//lstrcpy(tchFile, ptchExtMp3);
			lstrcpy(tchLocalFile, ptchLocalMp3);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_mainMp3Dlg_->OnExit_(TRUE);    //退出mp3
			nFlags = 2;
		    break;
		case photo_in:
			lstrcpy(m_chExtPath, ptchExtPhoto[m_devStatus]);
			//lstrcpy(tchFile, ptchExtPhoto);
			lstrcpy(tchLocalFile, ptchLocalPhoto);
			nFlags = 2;
			break;
		case bell_in:
			lstrcpy(m_chExtPath, ptchExtBell[m_devStatus]);
			//lstrcpy(tchFile, ptchExtBell);
			lstrcpy(tchLocalFile, ptchLocalBell);
			nFlags = 2;
		    break;
		case show_in:
			lstrcpy(m_chExtPath, ptchExtShow[m_devStatus]);
			//lstrcpy(tchFile, ptchExtShow);
			lstrcpy(tchLocalFile, ptchLocalShow);
			nFlags = 2;
			break;
		default:
		    return;
		}
		
		if (nFlags == 1)
		{
			if(!DetectFile(m_chExtPath/*tchFile*/))
			{
				progress_status_ = finish_status;
				
				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
				str = strTemp.c_str();
				m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
				return;
			}
		}
		else if (nFlags ==2)
		{
			if (!DetectDIR(m_chExtPath/*tchFile*/))
			{
				progress_status_ = finish_status;
				
				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
				str = strTemp.c_str();
				m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
				return;
			}
		}
		
		if (bOverflow)
		{
			g_bOverWrite = TRUE;
			if(tchLocalFile[0] != L'\0')
				lstrcat(tchLocalFile, _T("\\*"));
			
		/*
			switch(type_)
			{
			case mp3_in:
			case photo_in:
			case bell_in:
			case show_in:
				{
					lstrcat(tchLocalFile, _T("\\*"));
					DeleteFiles(tchLocalFile);
				}
				break;
			}
			*/
		}
		else
		{
			tchLocalFile[0] = L'\0';
			g_bOverWrite = FALSE;
		}
		
//		m_MJPGList.SetUnitIsShow(2, TRUE);
		StartCopyFile();

	}
}

void copyfileDlg::OnButtonCancel()
{
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetScreenSaveTimer();

	TerminateThread(hCopyFileThread, 0);
	CloseHandle(hCopyFileThread);
	hCopyFileThread = NULL;

	KillTimer(IDT_COPYFILE_TIMER);
	progress_status_ = start_status;
	ShowWindow_(SW_HIDE);
}

void copyfileDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == IDT_COPYFILE_TIMER)
	{
		//20090408
		/*
		static int nCount_ =0;
		 if(nCount_++%3 == 0)
		 {
			 CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
			 main->CancelBalckLightSaveTimer();
		 }
		 m_procbarSound.SetPos(nCount_%5);
		 */
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->CancelBalckLightSaveTimer();
	}
	CDialog::OnTimer(nIDEvent);
}

void copyfileDlg::CopyFileProc(void)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	copyfileDlg *pDlg = main->m_pSettingDlg->m_copyfileDlg;
	std::string strTemp;
	CString str;
	pDlg->m_nPos = 0;
	pDlg->SetTimer(IDT_COPYFILE_TIMER, 1000, 0);
	Sleep(1000);
	if(g_bOverWrite)
	{
		g_bOverWrite = FALSE;
		if(pDlg->tchLocalFile[0] != L'\0')
		{
			DeleteFiles(pDlg->tchLocalFile);
		}
	}
	switch(pDlg->type_)
	{
	case backup_file:
		{
			pDlg->m_procbarSound.SetParam(0, 0, 4, 1);
			pDlg->progress_status_ = copying_status;			
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
			str = strTemp.c_str();
			pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
			pDlg->m_procbarSound.ShowWindow(SW_SHOW);
			pDlg->m_procbarSound.SetPos(1);
			pDlg->m_nPos = 1;
			pDlg->SetTimer(IDT_COPYFILE_TIMER, 1000, 0);
			
			if (m_devStatus == usb_device)
			{
				CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
				CopyFile(L"/flashdrv/res_dat/mmp.db", L"/UsbDisk/mediaphone/mmp.db",  FALSE);
			}
			else if (m_devStatus == sdcard_device)
			{
				CreateDirectoryW(L"/StorageCard/mediaphone", 0);
				CopyFile(L"/flashdrv/res_dat/mmp.db", L"/StorageCard/mediaphone/mmp.db",  FALSE);
			}

			pDlg->KillTimer(IDT_COPYFILE_TIMER);
			pDlg->m_procbarSound.SetPos(4);
			
			pDlg->progress_status_ = finish_status;			
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
			str = strTemp.c_str();
			pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
		}
		break;
	case restore_file:
		{
			pDlg->m_procbarSound.SetParam(0, 0, 4, 1);
			BOOL DetectFile(TCHAR *sDir);
			if(!DetectFile(/*L"/UsbDisk/mediaphone/mmp.db"*/m_chExtPath))
			{
				pDlg->progress_status_ = finish_status;
				
				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
				str = strTemp.c_str();
				pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
				pDlg->KillTimer(IDT_COPYFILE_TIMER);
			}
			else			//copy
			{
				pDlg->progress_status_ = copying_status;			
				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
				str = strTemp.c_str();
				pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
				
				pDlg->m_procbarSound.ShowWindow(SW_SHOW);
				pDlg->m_procbarSound.SetPos(1);
				pDlg->m_nPos = 1;
				main->m_pSettingDlg->m_copyfileDlg->SetTimer(IDT_COPYFILE_TIMER, 1000, 0);
				
				//CopyFile(L"/UsbDisk/mediaphone/mmp.db", L"/flashdrv/res_dat/mmp.db", FALSE);
				CopyFile(m_chExtPath, L"/flashdrv/res_dat/mmp.db", FALSE);
				
				::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
			
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowTypeItems();
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->SetTypeListSelected(0);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowItemsInList();
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->ShowItemsInList(-1);
			//	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->ShowItemsInList(-1);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->ResetTypeInfo();

				pDlg->KillTimer(IDT_COPYFILE_TIMER);
				pDlg->m_procbarSound.SetPos(4);
				
				pDlg->progress_status_ = finish_status;			
				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
				str = strTemp.c_str();
				pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
			}
		}
		break;
	case copy_file: 
		{
			//录音备份
			pDlg->progress_status_ = copying_status;			
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
			str = strTemp.c_str();
			pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
			pDlg->m_procbarSound.ShowWindow(SW_SHOW);
			pDlg->m_procbarSound.SetPos(0);
			pDlg->m_nPos = 1;
			pDlg->SetTimer(IDT_COPYFILE_TIMER, 1000, 0);
			
			//在USB上建立目录
			if (m_devStatus == usb_device)
			{
				CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
				CreateDirectoryW(L"/UsbDisk/mediaphone/backup", 0);
				CreateDirectoryW(L"/UsbDisk/mediaphone/backup/flashdrv", 0);
				CreateDirectoryW(L"/UsbDisk/mediaphone/backup/SDcard", 0);
				CreateDirectoryW(L"/UsbDisk/mediaphone/backup/flashdrv/my_record", 0);
				CreateDirectoryW(L"/UsbDisk/mediaphone/backup/SDcard/my_record", 0);
				
				CopyDirFiles(L"/flashdrv/my_record", L"/UsbDisk/mediaphone/backup/flashdrv/my_record");
				//CopyDirFiles(L"/storagecard/my_record", L"/UsbDisk/mediaphone/backup/SDcard/my_record");
			}
			else if (m_devStatus = sdcard_device)
			{
				CreateDirectoryW(L"/StorageCard/mediaphone", 0);
				CreateDirectoryW(L"/StorageCard/mediaphone/backup", 0);
				CreateDirectoryW(L"/StorageCard/mediaphone/backup/flashdrv", 0);
				CreateDirectoryW(L"/StorageCard/mediaphone/backup/SDcard", 0);
				CreateDirectoryW(L"/StorageCard/mediaphone/backup/flashdrv/my_record", 0);
				CreateDirectoryW(L"/StorageCard/mediaphone/backup/SDcard/my_record", 0);
				
				CopyDirFiles(L"/flashdrv/my_record", L"/StorageCard/mediaphone/backup/flashdrv/my_record");
			}
			
			pDlg->KillTimer(IDT_COPYFILE_TIMER);
		//	pDlg->m_procbarSound.SetPos(4);
			
			pDlg->progress_status_ = finish_status;			
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
			str = strTemp.c_str();
			pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
		}
		break;
	case mp3_in:
		{
			if(!DetectDIR(m_chExtPath/*L"/UsbDisk/mediaphone/my_music"*/))
			{
				pDlg->progress_status_ = finish_status;
				
				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
				str = strTemp.c_str();
				pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE);
				pDlg->KillTimer(IDT_COPYFILE_TIMER);
			}
			else			//copy
			{
				pDlg->progress_status_ = copying_status;			
				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
				str = strTemp.c_str();
				pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
				
				pDlg->m_procbarSound.ShowWindow(SW_SHOW);
				pDlg->m_procbarSound.SetPos(0);
				pDlg->m_nPos = 1;
				main->m_pSettingDlg->m_copyfileDlg->SetTimer(IDT_COPYFILE_TIMER, 1000, 0);
				
				//CopyDirFiles(L"/UsbDisk/mediaphone/my_music", L"/flashdrv/my_music", FALSE);
				CopyDirFiles(m_chExtPath, L"/flashdrv/my_music", FALSE);
				
				pDlg->KillTimer(IDT_COPYFILE_TIMER);
			//	pDlg->m_procbarSound.SetPos(4);
				
				pDlg->progress_status_ = finish_status;			
				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
				str = strTemp.c_str();
				pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
			}
		}
		break;
	case mp3_out:
		{
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_mainMp3Dlg_->OnExit_(TRUE);    //退出mp3

			pDlg->progress_status_ = copying_status;			
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
			str = strTemp.c_str();
			pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
			pDlg->m_procbarSound.ShowWindow(SW_SHOW);
			pDlg->m_procbarSound.SetPos(0);
			pDlg->m_nPos = 1;
			pDlg->SetTimer(IDT_COPYFILE_TIMER, 1000, 0);
			
			//在USB上建立目录
			if (m_devStatus == usb_device)
			{
				SECURITY_ATTRIBUTES attribut;
				attribut.nLength = sizeof(SECURITY_ATTRIBUTES);
				attribut.lpSecurityDescriptor = NULL;
				attribut.bInheritHandle=FALSE;

				BOOL ret = CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
				DWORD d;
				if(ret == 0)
				{
					d = GetLastError();
				}
				ret = CreateDirectoryW(L"/UsbDisk/mediaphone/my_music", 0);
				if(ret == 0)
				{
					d = GetLastError();
				}
				
				CopyDirFiles(L"/flashdrv/my_music", L"/UsbDisk/mediaphone/my_music");

// 				if (DetectFile(ptcPlayList[m_devStatus]))
// 				{
// 					CFile::Remove(ptcPlayList[m_devStatus]);
// 				}
			}
			else if (m_devStatus == sdcard_device)
			{
				CreateDirectoryW(L"/StorageCard/mediaphone", 0);
				CreateDirectoryW(L"/StorageCard/mediaphone/my_music", 0);
				
				CopyDirFiles(L"/flashdrv/my_music", L"/StorageCard/mediaphone/my_music");

// 				if (DetectFile(ptcPlayList[m_devStatus]))
// 				{
// 					CFile::Remove(ptcPlayList[m_devStatus]);
// 				}
			}

			pDlg->KillTimer(IDT_COPYFILE_TIMER);
		//	pDlg->m_procbarSound.SetPos(4);
			
			pDlg->progress_status_ = finish_status;			
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
			str = strTemp.c_str();
			pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
		}
		break;
	case photo_in:
		{
			if(!DetectDIR(m_chExtPath/*L"/UsbDisk/mediaphone/my_photo"*/))
			{
				pDlg->progress_status_ = finish_status;
				
				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
				str = strTemp.c_str();
				pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
				pDlg->KillTimer(IDT_COPYFILE_TIMER);
			}
			else			//copy
			{
				pDlg->progress_status_ = copying_status;			
				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
				str = strTemp.c_str();
				pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
				
				pDlg->m_procbarSound.ShowWindow(SW_SHOW);
				pDlg->m_procbarSound.SetPos(0);
				pDlg->m_nPos = 1;
				main->m_pSettingDlg->m_copyfileDlg->SetTimer(IDT_COPYFILE_TIMER, 1000, 0);
				
				//CopyDirFiles(L"/UsbDisk/mediaphone/my_photo", L"/flashdrv/my_photo", FALSE);
				CopyDirFiles(m_chExtPath, L"/flashdrv/my_photo", FALSE);

				pDlg->KillTimer(IDT_COPYFILE_TIMER);
		//		pDlg->m_procbarSound.SetPos(4);
				
				pDlg->progress_status_ = finish_status;			
				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
				str = strTemp.c_str();
				pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
			}
		}
		break;
	case photo_out:
		{
			pDlg->progress_status_ = copying_status;			
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
			str = strTemp.c_str();
			pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
			pDlg->m_procbarSound.ShowWindow(SW_SHOW);
			pDlg->m_procbarSound.SetPos(0);
			pDlg->m_nPos = 1;
			pDlg->SetTimer(IDT_COPYFILE_TIMER, 1000, 0);
			
			//在USB上建立目录
			if (m_devStatus == usb_device)
			{
				CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
				CreateDirectoryW(L"/UsbDisk/mediaphone/my_photo", 0);
				
				CopyDirFiles(L"/flashdrv/my_photo", L"/UsbDisk/mediaphone/my_photo");
			}
			else if (m_devStatus == sdcard_device)
			{
				CreateDirectoryW(L"/StorageCard/mediaphone", 0);
				CreateDirectoryW(L"/StorageCard/mediaphone/my_photo", 0);
				
				CopyDirFiles(L"/flashdrv/my_photo", L"/StorageCard/mediaphone/my_photo");

			}
						
			pDlg->KillTimer(IDT_COPYFILE_TIMER);
	//		pDlg->m_procbarSound.SetPos(4);
			
			pDlg->progress_status_ = finish_status;			
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
			str = strTemp.c_str();
			pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
		}
		break;
	case bell_in:
		{
			if(!DetectDIR(m_chExtPath/*L"/UsbDisk/mediaphone/my_ring"*/))
			{
				pDlg->progress_status_ = finish_status;
				
				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
				str = strTemp.c_str();
				pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
				pDlg->KillTimer(IDT_COPYFILE_TIMER);
			}
			else			//copy
			{
				pDlg->progress_status_ = copying_status;			
				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
				str = strTemp.c_str();
				pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
				
				pDlg->m_procbarSound.ShowWindow(SW_SHOW);
				pDlg->m_procbarSound.SetPos(0);
				pDlg->m_nPos = 1;
				main->m_pSettingDlg->m_copyfileDlg->SetTimer(IDT_COPYFILE_TIMER, 1000, 0);
				
				//CopyDirFiles(L"/UsbDisk/mediaphone/my_ring", L"/flashdrv/my_ring", FALSE);
				CopyDirFiles(m_chExtPath, L"/flashdrv/my_ring", FALSE);
				
				pDlg->KillTimer(IDT_COPYFILE_TIMER);
			//	pDlg->m_procbarSound.SetPos(4);
				
				pDlg->progress_status_ = finish_status;			
				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
				str = strTemp.c_str();
				pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
			}
		}
		break;
	case bell_out:
		{
			pDlg->progress_status_ = copying_status;			
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
			str = strTemp.c_str();
			pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
			pDlg->m_procbarSound.ShowWindow(SW_SHOW);
			pDlg->m_procbarSound.SetPos(0);
			pDlg->m_nPos = 1;
			pDlg->SetTimer(IDT_COPYFILE_TIMER, 1000, 0);
			
			//在USB上建立目录
			if (m_devStatus == usb_device)
			{
				CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
				CreateDirectoryW(L"/UsbDisk/mediaphone/my_ring", 0);
				
				CopyDirFiles(L"/flashdrv/my_ring", L"/UsbDisk/mediaphone/my_ring");

			}
			else if (m_devStatus == sdcard_device)
			{
				CreateDirectoryW(L"/StorageCard/mediaphone", 0);
				CreateDirectoryW(L"/StorageCard/mediaphone/my_ring", 0);
				
				CopyDirFiles(L"/flashdrv/my_ring", L"/StorageCard/mediaphone/my_ring");
			}
					
			pDlg->KillTimer(IDT_COPYFILE_TIMER);
	//		pDlg->m_procbarSound.SetPos(4);
			
			pDlg->progress_status_ = finish_status;			
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
			str = strTemp.c_str();
			pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
		}
		break;
	case show_in:
		{
			if(!DetectDIR(m_chExtPath/*L"/UsbDisk/mediaphone/my_show"*/))
			{
				pDlg->progress_status_ = finish_status;
				
				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
				str = strTemp.c_str();
				pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
				pDlg->KillTimer(IDT_COPYFILE_TIMER);
			}
			else			//copy
			{
				pDlg->progress_status_ = copying_status;			
				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
				str = strTemp.c_str();
				pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
				
				pDlg->m_procbarSound.ShowWindow(SW_SHOW);
				pDlg->m_procbarSound.SetPos(1);
				pDlg->m_nPos = 1;
				main->m_pSettingDlg->m_copyfileDlg->SetTimer(IDT_COPYFILE_TIMER, 1000, 0);
				
				//CopyDirFiles(L"/UsbDisk/mediaphone/my_show", L"/flashdrv/my_show", FALSE);
				CopyDirFiles(m_chExtPath, L"/flashdrv/my_show", FALSE);
				
				pDlg->KillTimer(IDT_COPYFILE_TIMER);
				pDlg->m_procbarSound.SetPos(4);
				
				pDlg->progress_status_ = finish_status;			
				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
				str = strTemp.c_str();
				pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
			}
		}
		break;
	case show_out:
		{
			pDlg->progress_status_ = copying_status;			
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
			str = strTemp.c_str();
			pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
			pDlg->m_procbarSound.ShowWindow(SW_SHOW);
			pDlg->m_procbarSound.SetPos(1);
			pDlg->m_nPos = 1;
			pDlg->SetTimer(IDT_COPYFILE_TIMER, 1000, 0);
			
			//在USB上建立目录
			if (m_devStatus == usb_device)
			{
				CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
				CreateDirectoryW(L"/UsbDisk/mediaphone/my_show", 0);
				
				CopyDirFiles(L"/flashdrv/my_show", L"/UsbDisk/mediaphone/my_show");
			}
			else if (m_devStatus == sdcard_device)
			{
				CreateDirectoryW(L"/StorageCard/mediaphone", 0);
				CreateDirectoryW(L"/StorageCard/mediaphone/my_show", 0);
				
				CopyDirFiles(L"/flashdrv/my_show", L"/StorageCard/mediaphone/my_show");

			}
							
			pDlg->KillTimer(IDT_COPYFILE_TIMER);
			pDlg->m_procbarSound.SetPos(4);
			
			pDlg->progress_status_ = finish_status;			
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
			str = strTemp.c_str();
			pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
		}
		break;
	}
	
	// 	if(pDlg->type_ == backup_file)		//copy
	// 	{
	// 		pDlg->progress_status_ = copying_status;			
	// 		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
	// 		str = strTemp.c_str();
	// 		pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
	// 		pDlg->m_procbarSound.ShowWindow(SW_SHOW);
	// 		pDlg->m_procbarSound.SetPos(1);
	// 		pDlg->m_nPos = 1;
	// 		pDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);
	// 		CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
	// 		CreateDirectoryW(L"/UsbDisk/mediaphone/backup", 0);
	// 		CopyFile(L"/flashdrv/res_dat/mmp.db", L"/UsbDisk/mediaphone/backup/mmp.db",  FALSE);
	// 		
	// 		pDlg->KillTimer(IDT_COPYFILE_TIMER);
	// 		pDlg->m_procbarSound.SetPos(4);
	// 		
	// 		pDlg->progress_status_ = finish_status;			
	// 		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
	// 		str = strTemp.c_str();
	// 		pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
	// 	}
	// 	else if(pDlg->type_ == restore_file)
	// 	{
	// 		BOOL DetectFile(TCHAR *sDir);
	// 		if(!DetectFile(L"/UsbDisk/mediaphone/backup/mmp.db"))
	// 		{
	// 			pDlg->progress_status_ = finish_status;
	// 			
	// 			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
	// 			str = strTemp.c_str();
	// 			pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
	// 		}
	// 		else			//copy
	// 		{
	// 			pDlg->progress_status_ = copying_status;			
	// 			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
	// 			str = strTemp.c_str();
	// 			pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
	// 			
	// 			pDlg->m_procbarSound.ShowWindow(SW_SHOW);
	// 			pDlg->m_procbarSound.SetPos(1);
	// 			pDlg->m_nPos = 1;
	// 			main->m_pSettingDlg->m_copyfileDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);
	// 			
	// 			CopyFile(L"/UsbDisk/mediaphone/backup/mmp.db", L"/flashdrv/res_dat/mmp.db", FALSE);
	// 			
	// 			pDlg->KillTimer(IDT_COPYFILE_TIMER);
	// 			pDlg->m_procbarSound.SetPos(4);
	// 			
	// 			pDlg->progress_status_ = finish_status;			
	// 			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
	// 			str = strTemp.c_str();
	// 			pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
	// 		}
	// 	}
	// 	else if(pDlg->type_ == copy_file)
	// 	{
	// 		//录音备份
	// 		pDlg->progress_status_ = copying_status;			
	// 		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
	// 		str = strTemp.c_str();
	// 		pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
	// 		pDlg->m_procbarSound.ShowWindow(SW_SHOW);
	// 		pDlg->m_procbarSound.SetPos(1);
	// 		pDlg->m_nPos = 1;
	// 		pDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);
	// 		
	// 		//在USB上建立目录
	// 		CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
	// 		CreateDirectoryW(L"/UsbDisk/mediaphone/backup", 0);
	// 		CreateDirectoryW(L"/UsbDisk/mediaphone/backup/flashdrv", 0);
	// 		CreateDirectoryW(L"/UsbDisk/mediaphone/backup/SDcard", 0);
	// 		CreateDirectoryW(L"/UsbDisk/mediaphone/backup/flashdrv/my_record", 0);
	// 		CreateDirectoryW(L"/UsbDisk/mediaphone/backup/SDcard/my_record", 0);
	// 		
	// 		extern void CopyDirFiles(TCHAR *src, TCHAR *des);
	// 		CopyDirFiles(L"/flashdrv/my_record", L"/UsbDisk/mediaphone/backup/flashdrv/my_record");
	// 		CopyDirFiles(L"/storagecard/my_record", L"/UsbDisk/mediaphone/backup/SDcard/my_record");
	// 		
	// 		pDlg->KillTimer(IDT_COPYFILE_TIMER);
	// 		pDlg->m_procbarSound.SetPos(4);
	// 		
	// 		pDlg->progress_status_ = finish_status;			
	// 		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
	// 		str = strTemp.c_str();
	// 		pDlg->m_MJPGList.SetUnitBitmap(0, str, "", TRUE) ;
	// 	}
	pDlg->m_MJPGList.SetUnitIsDisable(3, FALSE);
}

void copyfileDlg::StartCopyFile()
{
	m_MJPGList.SetUnitIsDisable(3, TRUE);
	hCopyFileThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CopyFileProc, NULL, 0, NULL);
//	CloseHandle(hCopyFileThread);
}
/////////////////////////////////////////////////////////////////////////////
// copyfileDlg message handlers
