// copyfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/SkinStyle.h"
#include "../Data/LanguageResource.h"
#include "copyfileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// copyfileDlg dialog


copyfileDlg::copyfileDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(copyfileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(copyfileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nPos = 0;
}


void copyfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(copyfileDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(copyfileDlg, CCEDialog)
	//{{AFX_MSG_MAP(copyfileDlg)
		// NOTE: the ClassWizard will add message map macros here
	ON_BN_CLICKED(IDC_BUTTON_FIREWALL_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_FIREWALL_CANCEL, OnButtonCancel)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL copyfileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	std::string strTemp;
	CString str;

	m_sticTitle.Create(L"", WS_CHILD|WS_VISIBLE, CRect(62, 76, 340, 94), this);
	m_sticTitle.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	m_procbarSound.Create(WS_CHILD, CRect(62, 123, 338, 139), this, 0xFF10);
	m_procbarSound.SetParam(0, 0, 4, 1);
	m_procbarSound.SetPos(0);
	
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
	return TRUE;
}

void copyfileDlg::SetType(operater_type type)
{
	type_ = type;
	m_procbarSound.ShowWindow(SW_HIDE);
	extern BOOL DetectDIR(TCHAR *sDir);
	if(!DetectDIR(L"/UsbDisk"))
	{
		progress_status_ = nousb_status;
		std::string strTemp;
		CString str;
		
		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR1);
		str = strTemp.c_str();
		m_sticTitle.SetWindowText(str);

	}
	else 
	{
		progress_status_ = isexitfile_status;
		std::string strTemp;
		CString str;
		
		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR2);
		str = strTemp.c_str();
		m_sticTitle.SetWindowText(str);
	}
}

void copyfileDlg::OnButtonOk()
{
	std::string strTemp;
	CString str;
				
	if(progress_status_ == nousb_status || progress_status_ == finish_status)
	{
		KillTimer(IDT_COPYFILE_TIMER);
		progress_status_ = start_status;
		ShowWindow(SW_HIDE);
	}
	else if(progress_status_ == isexitfile_status)
	{
		if(type_ == backup_file)		//copy
		{
			StartCopyFile();
			/*
			progress_status_ = copying_status;			
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
			str = strTemp.c_str();
			m_sticTitle.SetWindowText(str);
			m_procbarSound.ShowWindow(SW_SHOW);
			m_procbarSound.SetPos(1);
			m_nPos = 1;
			SetTimer(IDT_COPYFILE_TIMER, 300, 0);
			CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
			CreateDirectoryW(L"/UsbDisk/mediaphone/backup", 0);
			CopyFile(L"/flashdrv/res_dat/mmp.db", L"/UsbDisk/mediaphone/backup/mmp.db",  FALSE);
			
			KillTimer(IDT_COPYFILE_TIMER);
			m_procbarSound.SetPos(4);
			
			progress_status_ = finish_status;			
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
			str = strTemp.c_str();
			m_sticTitle.SetWindowText(str);
			*/
		}
		else if(type_ == restore_file)
		{
			BOOL DetectFile(TCHAR *sDir);
			if(!DetectFile(L"/UsbDisk/mediaphone/backup/mmp.db"))
			{
				progress_status_ = finish_status;
				
				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
				str = strTemp.c_str();
				m_sticTitle.SetWindowText(str);
			}
			else			//copy
			{
				StartCopyFile();
// 				progress_status_ = copying_status;			
// 				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
// 				str = strTemp.c_str();
// 				m_sticTitle.SetWindowText(str);
// 
// 				m_procbarSound.ShowWindow(SW_SHOW);
// 				m_procbarSound.SetPos(1);
// 				m_nPos = 1;
// 				SetTimer(IDT_COPYFILE_TIMER, 300, 0);
// 
// 				CopyFile(L"/UsbDisk/mediaphone/backup/mmp.db", L"/flashdrv/res_dat/mmp.db", FALSE);
// 				
// 				KillTimer(IDT_COPYFILE_TIMER);
// 				m_procbarSound.SetPos(4);
// 
// 				progress_status_ = finish_status;			
// 				strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
// 				str = strTemp.c_str();
// 				m_sticTitle.SetWindowText(str);
			}
		}
		else if(type_ == copy_file)
		{
			StartCopyFile();
			//录音备份
// 			progress_status_ = copying_status;			
// 			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
// 			str = strTemp.c_str();
// 			m_sticTitle.SetWindowText(str);
// 			m_procbarSound.ShowWindow(SW_SHOW);
// 			m_procbarSound.SetPos(1);
// 			m_nPos = 1;
// 			SetTimer(IDT_COPYFILE_TIMER, 300, 0);
// 
// 			//在USB上建立目录
// 			CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
// 			CreateDirectoryW(L"/UsbDisk/mediaphone/backup", 0);
// 			CreateDirectoryW(L"/UsbDisk/mediaphone/backup/flashdrv", 0);
// 			CreateDirectoryW(L"/UsbDisk/mediaphone/backup/SDcard", 0);
// 			CreateDirectoryW(L"/UsbDisk/mediaphone/backup/flashdrv/my_record", 0);
// 			CreateDirectoryW(L"/UsbDisk/mediaphone/backup/SDcard/my_record", 0);
// 
// 			extern void CopyDirFiles(TCHAR *src, TCHAR *des);
// 			CopyDirFiles(L"/flashdrv/my_record", L"/UsbDisk/mediaphone/backup/flashdrv/my_record");
// 			CopyDirFiles(L"/storage card/my_record", L"/UsbDisk/mediaphone/backup/SDcard/my_record");
// 			
// 			KillTimer(IDT_COPYFILE_TIMER);
// 			m_procbarSound.SetPos(4);
// 
// 			progress_status_ = finish_status;			
// 			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
// 			str = strTemp.c_str();
// 			m_sticTitle.SetWindowText(str);
		}
	}
}

void copyfileDlg::OnButtonCancel()
{
	KillTimer(IDT_COPYFILE_TIMER);
	progress_status_ = start_status;
	ShowWindow(SW_HIDE);
}

void copyfileDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == IDT_COPYFILE_TIMER)
	{
		m_procbarSound.SetPos(m_nPos);
	}
	CDialog::OnTimer(nIDEvent);
}

void copyfileDlg::CopyFileProc(void)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	copyfileDlg *pDlg = main->m_pSettingDlg->m_copyfileDlg;
	std::string strTemp;
	CString str;
	if(pDlg->type_ == backup_file)		//copy
	{
		pDlg->progress_status_ = copying_status;			
		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
		str = strTemp.c_str();
		pDlg->m_sticTitle.SetWindowText(str);
		pDlg->m_procbarSound.ShowWindow(SW_SHOW);
		pDlg->m_procbarSound.SetPos(1);
		pDlg->m_nPos = 1;
		pDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);
		CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
		CreateDirectoryW(L"/UsbDisk/mediaphone/backup", 0);
		CopyFile(L"/flashdrv/res_dat/mmp.db", L"/UsbDisk/mediaphone/backup/mmp.db",  FALSE);
		
		pDlg->KillTimer(IDT_COPYFILE_TIMER);
		pDlg->m_procbarSound.SetPos(4);
		
		pDlg->progress_status_ = finish_status;			
		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
		str = strTemp.c_str();
		pDlg->m_sticTitle.SetWindowText(str);
	}
	else if(pDlg->type_ == restore_file)
	{
		BOOL DetectFile(TCHAR *sDir);
		if(!DetectFile(L"/UsbDisk/mediaphone/backup/mmp.db"))
		{
			pDlg->progress_status_ = finish_status;
			
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
			str = strTemp.c_str();
			pDlg->m_sticTitle.SetWindowText(str);
		}
		else			//copy
		{
			pDlg->progress_status_ = copying_status;			
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
			str = strTemp.c_str();
			pDlg->m_sticTitle.SetWindowText(str);
			
			pDlg->m_procbarSound.ShowWindow(SW_SHOW);
			pDlg->m_procbarSound.SetPos(1);
			pDlg->m_nPos = 1;
			main->m_pSettingDlg->m_copyfileDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);
			
			CopyFile(L"/UsbDisk/mediaphone/backup/mmp.db", L"/flashdrv/res_dat/mmp.db", FALSE);
			
			pDlg->KillTimer(IDT_COPYFILE_TIMER);
			pDlg->m_procbarSound.SetPos(4);
			
			pDlg->progress_status_ = finish_status;			
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
			str = strTemp.c_str();
			pDlg->m_sticTitle.SetWindowText(str);
		}
	}
	else if(pDlg->type_ == copy_file)
	{
		//录音备份
		pDlg->progress_status_ = copying_status;			
		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
		str = strTemp.c_str();
		pDlg->m_sticTitle.SetWindowText(str);
		pDlg->m_procbarSound.ShowWindow(SW_SHOW);
		pDlg->m_procbarSound.SetPos(1);
		pDlg->m_nPos = 1;
		pDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);
		
		//在USB上建立目录
		CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
		CreateDirectoryW(L"/UsbDisk/mediaphone/backup", 0);
		CreateDirectoryW(L"/UsbDisk/mediaphone/backup/flashdrv", 0);
		CreateDirectoryW(L"/UsbDisk/mediaphone/backup/SDcard", 0);
		CreateDirectoryW(L"/UsbDisk/mediaphone/backup/flashdrv/my_record", 0);
		CreateDirectoryW(L"/UsbDisk/mediaphone/backup/SDcard/my_record", 0);
		
		extern void CopyDirFiles(TCHAR *src, TCHAR *des);
		CopyDirFiles(L"/flashdrv/my_record", L"/UsbDisk/mediaphone/backup/flashdrv/my_record");
		CopyDirFiles(L"/storage card/my_record", L"/UsbDisk/mediaphone/backup/SDcard/my_record");
		
		pDlg->KillTimer(IDT_COPYFILE_TIMER);
		pDlg->m_procbarSound.SetPos(4);
		
		pDlg->progress_status_ = finish_status;			
		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
		str = strTemp.c_str();
		pDlg->m_sticTitle.SetWindowText(str);
	}
}

void copyfileDlg::StartCopyFile()
{
	hCopyFileThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CopyFileProc, NULL, 0, NULL);
}
/////////////////////////////////////////////////////////////////////////////
// copyfileDlg message handlers
