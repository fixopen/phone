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
	switch(w)
	{
	case 1000:
		OnButtonCancel();
		break;
	case 1001:
		Import(TRUE);
		break;
	}
	
}
BOOL copyfileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	std::string strTemp;
	CString str;
	m_procbarSound.Create(WS_CHILD, CRect(200, 287, 200+403, 287 + 30), this, 0xFF10);
	m_procbarSound.SetParam(0, 0, 4, 1);
	m_procbarSound.SetPos(0);
		
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(190, 120, 190+440, 120+270), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\文件备份.xml");
	m_MJPGList.SetMJPGRect(CRect(190, 120, 190+440, 120+270));
	
	m_MJPGList.SetUnitColor(1, font_black, TRUE);
	m_MJPGList.SetUnitFont(1, font_22);
	m_MJPGList.SetUnitColor(2, font_black, TRUE);
	m_MJPGList.SetUnitFont(2, font_22);

	return TRUE;
}

void copyfileDlg::SetType(operater_type type)
{
	type_ = type;
	m_procbarSound.ShowWindow(SW_HIDE);
	
	BOOL bUsb = DetectDIR(_T("/UsbDisk"));
	if( bUsb || DetectDIR(L"/StorageCard"))
	{
		if (bUsb)
			m_devStatus = usb_device;
		else
			m_devStatus = sdcard_device;

		progress_status_ = isexitfile_status;
		switch(type_)
		{
		case backup_file:
		case mp3_out:
		case photo_out:
		case bell_out: 
			m_MJPGList.SetUnitText(2, L"是否将文件备份到移动设备?", FALSE);
			break;
		case restore_file:
		case mp3_in:
		case photo_in:
		case bell_in:
			m_MJPGList.SetUnitText(2, L"是否将文件导入到本地?", FALSE);
			break;
		}
	}
	else 
	{
		m_devStatus = none_device;
		memset(m_chExtPath, 0, sizeof(m_chExtPath));

		progress_status_ = nousb_status;
		m_MJPGList.SetUnitText(2, L"移动设备不存在,请插入", FALSE);
	}
	m_MJPGList.Invalidate();
}

void copyfileDlg::Import(BOOL bOverflow)
{			
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
		case mp3_out:
		case photo_out:
		case bell_out:
			break;
		case restore_file:
			{
				lstrcpy(m_chExtPath, ptchExtDb[m_devStatus]);
				nFlags = 1;
			}
			break;
		case mp3_in:
			lstrcpy(m_chExtPath, ptchExtMp3[m_devStatus]);
			lstrcpy(tchLocalFile, ptchLocalMp3);
			//((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_mainMp3Dlg_->OnExit_(TRUE);    //退出mp3
			nFlags = 2;
		    break;
		case photo_in:
			lstrcpy(m_chExtPath, ptchExtPhoto[m_devStatus]);
			lstrcpy(tchLocalFile, ptchLocalPhoto);
			nFlags = 2;
			break;
		case bell_in:
			lstrcpy(m_chExtPath, ptchExtBell[m_devStatus]);
			lstrcpy(tchLocalFile, ptchLocalBell);
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
				m_MJPGList.SetUnitText(2, L"文件不存在,无法恢复", TRUE);
				return;
			}
		}
		else if (nFlags ==2)
		{
			if (!DetectDIR(m_chExtPath/*tchFile*/))
			{
				progress_status_ = finish_status;
				
				m_MJPGList.SetUnitText(2, L"路径不存在,无法导入", TRUE);
				return;
			}
		}
		
		if (bOverflow)
		{
			g_bOverWrite = TRUE;
			if(tchLocalFile[0] != L'\0')
				lstrcat(tchLocalFile, _T("\\*"));
		}
		else
		{
			tchLocalFile[0] = L'\0';
			g_bOverWrite = FALSE;
		}
		m_MJPGList.SetUnitText(1, L"此操作会覆盖已有文件", FALSE);
		m_MJPGList.SetUnitText(2, L"", FALSE);
		m_MJPGList.Invalidate();
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
		//if(pDlg->tchLocalFile[0] != L'\0')   //wangzhenxing20100607
		//{
		//	DeleteFiles(pDlg->tchLocalFile);
		//}
	}
	switch(pDlg->type_)
	{
	case backup_file:
		{
			pDlg->m_procbarSound.SetParam(0, 0, 4, 1);
			pDlg->progress_status_ = copying_status;			
			pDlg->m_MJPGList.SetUnitText(2, L"正在备份数据库,请稍后...", TRUE);
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
			pDlg->m_MJPGList.SetUnitText(1, L"", TRUE);
			pDlg->m_MJPGList.SetUnitText(2, L"备份成功", FALSE);
		}
		break;
	case restore_file:
		{
			pDlg->m_procbarSound.SetParam(0, 0, 4, 1);
			BOOL DetectFile(TCHAR *sDir);
			if(!DetectFile(/*L"/UsbDisk/mediaphone/mmp.db"*/m_chExtPath))
			{
				pDlg->progress_status_ = finish_status;
				
				pDlg->m_MJPGList.SetUnitText(2, L"文件不存在,无法恢复数据库", TRUE);
				pDlg->KillTimer(IDT_COPYFILE_TIMER);
			}
			else			//copy
			{
				pDlg->progress_status_ = copying_status;			
				pDlg->m_MJPGList.SetUnitText(2, L"正在恢复数据库,请稍后...", TRUE);
				
				pDlg->m_procbarSound.ShowWindow(SW_SHOW);
				pDlg->m_procbarSound.SetPos(1);
				pDlg->m_nPos = 1;
				main->m_pSettingDlg->m_copyfileDlg->SetTimer(IDT_COPYFILE_TIMER, 1000, 0);
				
				CopyFile(m_chExtPath, L"/flashdrv/res_dat/mmp.db", FALSE);
				
				::SendMessage(main->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
			
				//main->m_pContactDlg->ShowTypeItems();
				//main->m_pContactDlg->ShowItemsInList();
				//main->m_pSoundDlg->ShowItemsInList(-1);
				//main->m_pContactInfoDlg->ResetTypeInfo();

				pDlg->KillTimer(IDT_COPYFILE_TIMER);
				pDlg->m_procbarSound.SetPos(4);
				
				pDlg->progress_status_ = finish_status;			
				pDlg->m_MJPGList.SetUnitText(2, L"数据库恢复成功", FALSE);
			}
		}
		break;
	case copy_file: 
		{
			//录音备份
			pDlg->progress_status_ = copying_status;			
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
			str = strTemp.c_str();
			pDlg->m_MJPGList.SetUnitBitmap(2, str, "", TRUE) ;
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
			pDlg->m_MJPGList.SetUnitBitmap(2, str, "", TRUE) ;
		}
		break;
	case mp3_in:
		{
			if(!DetectDIR(m_chExtPath))
			{
				pDlg->progress_status_ = finish_status;

				pDlg->m_MJPGList.SetUnitText(2, L"路径不存在,无法导入MP3", TRUE);
				pDlg->KillTimer(IDT_COPYFILE_TIMER);
			}
			else			//copy
			{
				pDlg->progress_status_ = copying_status;			
				pDlg->m_MJPGList.SetUnitText(2, L"正在导入MP3,请稍后...", TRUE);
				
				pDlg->m_procbarSound.ShowWindow(SW_SHOW);
				pDlg->m_procbarSound.SetPos(0);
				pDlg->m_nPos = 1;
				main->m_pSettingDlg->m_copyfileDlg->SetTimer(IDT_COPYFILE_TIMER, 1000, 0);
				
				//CopyDirFiles(m_chExtPath, L"/flashdrv/my_music", FALSE);
				CopyDirFiles(m_chExtPath, L"/flashdrv/my_music");
				
				pDlg->KillTimer(IDT_COPYFILE_TIMER);
			//	pDlg->m_procbarSound.SetPos(4);
				
				pDlg->progress_status_ = finish_status;			
				pDlg->m_MJPGList.SetUnitText(2, L"导入完成", FALSE) ;
			}
		}
		break;
	case mp3_out:
		{
			//main->m_pMainDlg->m_mainMp3Dlg_->OnExit_(TRUE);    //退出mp3

			pDlg->progress_status_ = copying_status;			
			pDlg->m_MJPGList.SetUnitText(2, L"正在导出MP3,请稍后...", TRUE);
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
			}
			else if (m_devStatus == sdcard_device)
			{
				CreateDirectoryW(L"/StorageCard/mediaphone", 0);
				CreateDirectoryW(L"/StorageCard/mediaphone/my_music", 0);
				
				CopyDirFiles(L"/flashdrv/my_music", L"/StorageCard/mediaphone/my_music");
			}

			pDlg->KillTimer(IDT_COPYFILE_TIMER);
		//	pDlg->m_procbarSound.SetPos(4);
			
			pDlg->progress_status_ = finish_status;			
			pDlg->m_MJPGList.SetUnitText(2, L"导出完成", FALSE);
		}
		break;
	case photo_in:
		{
			if(!DetectDIR(m_chExtPath))
			{
				pDlg->progress_status_ = finish_status;
				
				pDlg->m_MJPGList.SetUnitText(2, L"路径不存在,无法导入相册", TRUE);
				pDlg->KillTimer(IDT_COPYFILE_TIMER);
			}
			else			//copy
			{
				pDlg->progress_status_ = copying_status;			
				pDlg->m_MJPGList.SetUnitText(2, L"正在导入相册,请稍后...", TRUE);
				
				pDlg->m_procbarSound.ShowWindow(SW_SHOW);
				pDlg->m_procbarSound.SetPos(0);
				pDlg->m_nPos = 1;
				main->m_pSettingDlg->m_copyfileDlg->SetTimer(IDT_COPYFILE_TIMER, 1000, 0);
				
				//CopyDirFiles(m_chExtPath, L"/flashdrv/my_photo", FALSE);
				CopyDirFiles(m_chExtPath, L"/flashdrv/my_photo");  //wangzhenxing20100607

				pDlg->KillTimer(IDT_COPYFILE_TIMER);
				
				pDlg->progress_status_ = finish_status;			
				pDlg->m_MJPGList.SetUnitText(2, L"导入完成", FALSE);
			}
		}
		break;
	case photo_out:
		{
			pDlg->progress_status_ = copying_status;			
			pDlg->m_MJPGList.SetUnitText(2, L"正在导出相册,请稍后...", TRUE);
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
			
			pDlg->progress_status_ = finish_status;			
			pDlg->m_MJPGList.SetUnitText(2, L"导出完成", FALSE);
		}
		break;
	case bell_in:
		{
			if(!DetectDIR(m_chExtPath/*L"/UsbDisk/mediaphone/my_ring"*/))
			{
				pDlg->progress_status_ = finish_status;
				
				pDlg->m_MJPGList.SetUnitText(2, L"路径不存在,无法导入振铃音乐", TRUE);
				pDlg->KillTimer(IDT_COPYFILE_TIMER);
			}
			else			//copy
			{
				pDlg->progress_status_ = copying_status;			
				pDlg->m_MJPGList.SetUnitText(2, L"正在导入振铃音乐,请稍后...", TRUE);
				
				pDlg->m_procbarSound.ShowWindow(SW_SHOW);
				pDlg->m_procbarSound.SetPos(0);
				pDlg->m_nPos = 1;
				main->m_pSettingDlg->m_copyfileDlg->SetTimer(IDT_COPYFILE_TIMER, 1000, 0);
				
				//CopyDirFiles(m_chExtPath, L"/flashdrv/my_ring", FALSE);
				CopyDirFiles(m_chExtPath, L"/flashdrv/my_ring");
				
				pDlg->KillTimer(IDT_COPYFILE_TIMER);
			//	pDlg->m_procbarSound.SetPos(4);
				
				pDlg->progress_status_ = finish_status;			
				pDlg->m_MJPGList.SetUnitText(2, L"导入完成", FALSE);
			}
		}
		break;
	case bell_out:
		{
			pDlg->progress_status_ = copying_status;			
			pDlg->m_MJPGList.SetUnitText(2, L"正在导出振铃音乐,请稍后...", TRUE);
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
			
			pDlg->progress_status_ = finish_status;			
			pDlg->m_MJPGList.SetUnitText(2, L"导出完成", FALSE);
		}
		break;
	}
	if(pDlg->progress_status_ == finish_status)
	{
		pDlg->m_MJPGList.SetUnitText(1, L"", FALSE);
		pDlg->m_MJPGList.Invalidate();
		Sleep(2000);
		pDlg->ShowWindow_(SW_HIDE);
		//wangzhenxing20100608
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetScreenSaveTimer();
	}
}

void copyfileDlg::StartCopyFile()
{
	m_MJPGList.SetUnitIsDisable(3, TRUE);
	hCopyFileThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CopyFileProc, NULL, 0, NULL);
//	CloseHandle(hCopyFileThread);
}
