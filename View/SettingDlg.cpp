// SettingDlg.cpp : implementation file
//
#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "SettingDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "FireWallDlg.h"
#include "../MultimediaPhoneDlg.h"

#include <Storemgr.h>
#include <fatutil.h>
#include "../Util/socket.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#define outputstring_data 1
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#else
#define outputstring_data 0
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog
CString s_VerSion = "1.1.1303261619";
CString CH="中文";
CString EG="English";

extern int g_iContactCount;
extern int g_iContactInfoCount;
extern int g_iRecordCount;
DWORD PreFreeSize = 20 * 1024 * 1024; //预留20M空间
DWORD sdPreFreeSize = 5 * 1024 *1024; //sd卡预留5M空间
//快捷键用上层去实现.
//3.3.0729  屏蔽了光敏值
//3.3.0730  加了背光持续选项

extern BOOL DetectDIR(TCHAR *sDir);
BOOL DeleteFiles(TCHAR* sDir)
{
	CString s = sDir;
	s = s.Left(s.GetLength() - 1);
	
	BOOL flag = FALSE;
	WIN32_FIND_DATA fd; 
	HANDLE hFind = FindFirstFile(sDir, &fd); 
	if ((hFind != INVALID_HANDLE_VALUE)) 
	{ 
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			try
			{
				CFile::Remove(s + fd.cFileName);
			}
			catch (CFileException* pEx)
			{
				pEx->Delete();
			}
		}


		while(FindNextFile(hFind, &fd))
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				try
				{
					CFile::Remove(s + fd.cFileName);
				}
				catch (CFileException* pEx)
				{
					pEx->Delete();
				}
			}
		}
		flag = TRUE;
	} 

	FindClose(hFind);
	return flag;
}

//递归删除文件
void  RecursiveDeleteFiles(TCHAR* sDir)
{
	CString s = sDir;
	s = s.Left(s.GetLength() - 1);
	
	BOOL flag = FALSE;
	WIN32_FIND_DATA fd; 
	HANDLE hFind = FindFirstFile(sDir, &fd); 
	if(hFind != INVALID_HANDLE_VALUE)
	{	
		do 
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{	
				//文件直接删除
				try
				{
					CFile::Remove(s + fd.cFileName);
				}
				catch (CFileException* pEx)
				{
					pEx->Delete();
				}
			}
			else
			{
				//文件夹，删除文件夹下面的文件
				CString dir = s + fd.cFileName;
				dir += L"\\" ;
				dir += L"*"  ;
				RecursiveDeleteFiles((LPTSTR)(LPCTSTR)dir);
			}

		} while (FindNextFile(hFind, &fd));
	
	}	
	FindClose(hFind);
}


DWORD CountFolderSize(CString strPath)
{	
	if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->IsWindowVisible())
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->m_procbarSound.SetParam(0, 0, 4, 1);
	
	if(strPath.Find(L"*", 0) == -1)
		strPath += _T("\\*.*");
	DWORD dwSize = 0;
	WIN32_FIND_DATA FindFileData;
	
	HANDLE hFind = FindFirstFile((LPCTSTR)strPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
	}
	else
	{
		bool finished = false;
		do
		{
			if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				dwSize++;
				if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->IsWindowVisible())
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->m_procbarSound.SetPos(dwSize%5);
			}
			
			if (!FindNextFile(hFind, &FindFileData))
			{
				if (GetLastError() == ERROR_NO_MORE_FILES)
				{
					finished = true;
				}
				else
				{
				}
			}
			Sleep(10);
		} while (!finished);
		FindClose(hFind);
	}
				
		if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->IsWindowVisible())
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->m_procbarSound.SetPos(0);
		return dwSize;
}

void CopyDirFiles(TCHAR *src, TCHAR *des, BOOL bOverflow)
{
	int nFileCount = CountFolderSize(src);
	int nFileIndex = 0;
	
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->m_procbarSound.SetParam(0, 0, nFileCount, 1);
	
	CString SrcDir = src;
	CString DesDir = des;
	CString findFilename = SrcDir + "/*.*";
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile((LPCTSTR)findFilename, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
	}
	else
	{
		bool finished = false;
		do
		{
			wchar_t wideToName[256];
		
			wsprintf(wideToName, _T("%s/%s"), DesDir, (LPCTSTR)FindFileData.cFileName); //findFileName
 
			wchar_t wideFromName[256] = {0};
		
			wsprintf(wideFromName, _T("%s/%s"), SrcDir, (LPCTSTR)FindFileData.cFileName); //findFileName
			
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//wprintf(_T("create dir %s to %s\n"), wideFromName, wideToName);
				//CopyDirFiles(wideFromName, wideToName);
			}
			else{
				if (!::CopyFile(wideFromName, wideToName, !bOverflow))
				{
				}
				nFileIndex++;
				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->m_procbarSound.SetPos(nFileIndex);
			}
			if (!FindNextFile(hFind, &FindFileData))
			{
				DWORD d = GetLastError();
				if (d == ERROR_NO_MORE_FILES)
				{
					finished = true;
				}
			}
			::Sleep(10);
		} while (!finished);
		FindClose(hFind);
	}
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_copyfileDlg->m_procbarSound.SetPos(nFileCount);
}

BOOL WriteSelectLanguagelog(LPCTSTR lpContent)
{
	CStdioFile file;
    LPCTSTR filePath=CString("\\hive\\Language.txt");
	if ( !file.Open(filePath, CStdioFile::modeCreate|CStdioFile::modeNoTruncate|CStdioFile::modeWrite) )
	{
		return FALSE;
	}
	file.SeekToBegin();
	file.Write(lpContent, lstrlen(lpContent)*sizeof(TCHAR));
	file.Close();
	return TRUE;
}
CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


#define IDC_SETTING_BACKUP				9010
#define IDC_SETTING_RESTORE				9011
#define IDC_SETTING_CMBSELECT			9012
#define IDC_SETTING_DELETE				9013
#define IDC_SETTING_ALLSELECT			9014
#define IDC_SETTING_CLEARSELECT			9015
#define IDC_SETTING_IN					9016
#define IDC_SETTING_OUT					9017
#define IDC_SETTING_LSTLOCAL			9018
#define IDC_SETTING_LSTUSB				9019

BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSettingDlg)
//	ON_MESSAGE(WM_CHANGELANGUAGE,OnChangeLanguage)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_LIST_SETTING_TYPE, OnClickListType)
	ON_MESSAGE(WM_LISTCTRL_CLICK, OnListCltrlClick)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_OK, OnButtonSettingOk)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_LOGIN, OnButtonSettingLogin)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_CANCEL, OnButtonSettingCancel)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_DEFAULT, OnButtonSettingDefault)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_PLAYRING, OnButtonSettingPlayRing)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_PLAY, OnButtonSettingListen)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_DEFINEFASTDAIL,OnButtonSettingDefineFastDial)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_ADJUSTTOUCH, OnAdjustTouchPanel)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_SETPASSWORD, OnSettingSaveScreenPassword)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_SETPLAYPASSWORD, OnSetPlayPassword)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_SETADMINPASSWORD, OnSetAdminPassword)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_CLEARCONTACT, OnSettingClearContact)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_CLEARCONTACTINFO, OnSettingClearContactInfo)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_CLEARSOUND, OnSettingClearSound)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_CLEARINFO, OnSettingClearInfo)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_FORMATSD, OnSettingFormatSD)
    ON_BN_CLICKED(IDC_BUTTON_SETTING_LANGCHANGE,OnSettingLanguageChange)
	ON_BN_CLICKED(IDC_SETTING_BACKUP, OnBackup)
	ON_BN_CLICKED(IDC_SETTING_RESTORE, OnRestore)
	ON_BN_CLICKED(IDC_SETTING_OUT, OnFileOut)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUTSTRING_DATA,OnButtonSysInfo)
	ON_BN_CLICKED(IDC_BTN_CLEARRING, OnSettingClearRing)
	ON_BN_CLICKED(IDC_BTN_DELETERING, OnSettingDeleteRing)
	ON_BN_CLICKED(IDB_BTN_USBMODAL, OnSettingUSBModal)
	//add by yy 20111103
	ON_BN_CLICKED(IDC_BUTTON_SETTING_ONEKEYCLEAR, OnButtonMissCallClear)

 #if outputstring_data
	  ON_BN_CLICKED(IDC_CHECK_SETTING_DELETE,OnRegRecordSound)
 #endif
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg message handlers

BOOL CSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int xbegin   = 90  ;
	int xWidth   = 53  ;
	int ybegin   = 60  ;
	int yheight        ;
	int xWidth2  = 80  ; 
	int val       ;
	int nSpace =  20 ;

	m_iTotalSeconds=0;
	m_iCurrentSecond=0;
	m_iSBell=false;
	m_bUSBModal = FALSE;
	// TODO: Add extra initialization here
	std::string strTemp = Data::LanguageResource::Get(Data::RI_RECORD_TYPE);
	CString str = strTemp.c_str();

	m_lsType.m_nLineHeight = 25;
	m_lsType.m_nFontHeight = 16;
	m_lsType.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER,\
		CRect(5, 27*VHEIGHT, 80, 198*VHEIGHT/*218*VHEIGHT*/), this, IDC_LIST_SETTING_TYPE, FALSE);
	m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsType.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 75);
	
	m_pImageList = new CImageList();
	m_pImageList->Create(16, m_lsType.m_nLineHeight, ILC_COLOR32|ILC_MASK, 4, 2);   
	CBitmap bm;
	bm.LoadBitmap(IDB_BITMAP28);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP29);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP30);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP27);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP34);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	
	m_lsType.SetImageList(m_pImageList, LVSIL_SMALL);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TELEPHONE);
	str = strTemp.c_str();
	m_lsType.InsertItem(0, str, 0);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_INTERNET);
	str = strTemp.c_str();
	m_lsType.InsertItem(1, str, 1);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SYSTEM);
	str = strTemp.c_str();
	m_lsType.InsertItem(2, str, 2);


	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_INFORMATION);
	str = strTemp.c_str();
	m_lsType.InsertItem(3, str, 3);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RINGINFO);  //wzx20101224
	str = strTemp.c_str();
	m_lsType.InsertItem(4, str, 4);
	
	//保存
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SAVE);
	str = strTemp.c_str();
	m_btnOk.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 26*VHEIGHT, 471, 45*VHEIGHT), this, IDC_BUTTON_SETTING_OK);
	m_btnOk.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);
	
	//恢复默认
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DEFAULT);
	str = strTemp.c_str();
	m_btnDefault.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 46*VHEIGHT, 471, 65*VHEIGHT), this, IDC_BUTTON_SETTING_DEFAULT);
	m_btnDefault.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);
	
	//管理
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LOGIN);
	str = strTemp.c_str();
	m_btnLogin.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 66*VHEIGHT, 471, 85*VHEIGHT), this, IDC_BUTTON_SETTING_LOGIN);
	m_btnLogin.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);
	
	//关闭
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnCancel.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 178*VHEIGHT, 471, 198*VHEIGHT), this, IDC_BUTTON_SETTING_CANCEL);
	m_btnCancel.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);
	
	//电话通讯 左1======================================================================================================
	//启用外线加拨
	xbegin = 105-nSpace ;
	xWidth = 91  ;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_OUTLINE);
	str = strTemp.c_str();
	m_chbEnableOutLine.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, \
		CRect(xbegin, 28*VHEIGHT, xbegin+xWidth, 44*VHEIGHT), this, IDC_CHECK_SETTING_OUTLINE);
	m_chbEnableOutLine.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	SetButtonDefaultColor(&m_chbEnableOutLine);	
	
	xbegin = 200-nSpace ;
	xWidth = 50  ;
	m_edtOutLine.Create(WS_CHILD|WS_VISIBLE, \
		CRect(xbegin, 28*VHEIGHT, xbegin+xWidth, 44*VHEIGHT), this, IDC_EDIT_SETTING_OUTLINE);
	m_edtOutLine.SetLimitText(15);
	m_edtOutLine.SetLimitDiagital();

	//本机号码
	xbegin = 255-nSpace;
	xWidth = 65 ;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LACALNUMBER);
	str = strTemp.c_str();
	m_sticLocalNumber.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, 28*VHEIGHT, xbegin+xWidth, 44*VHEIGHT), this);
	m_sticLocalNumber.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	xbegin = 325-nSpace;
	xWidth = 95 ;
	m_edtLocalNumber.Create(WS_CHILD|WS_VISIBLE, \
		CRect(xbegin, 28*VHEIGHT, xbegin+xWidth, 44*VHEIGHT), this, IDC_EDIT_SETTING_AREANO);
	m_edtLocalNumber.SetLimitText(15);
	m_edtLocalNumber.SetLimitDiagital();
	
	//启用IP拨号
	xbegin = 105-nSpace;
	xWidth = 91 ;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_IPDIAL);
	str = strTemp.c_str();
	m_chbEnableIPDial.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX,\
		CRect(xbegin, 46*VHEIGHT, xbegin+xWidth, 62*VHEIGHT), this, IDC_CHECK_SETTING_IPDIAL);
	m_chbEnableIPDial.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	SetButtonDefaultColor(&m_chbEnableIPDial);	
	
	xbegin = 200-nSpace;
	xWidth = 80 ;
	m_edtIPDial.Create(WS_CHILD|WS_VISIBLE, \
		CRect(xbegin, 46*VHEIGHT, xbegin+xWidth, 62*VHEIGHT), this, IDC_EDIT_SETTING_IPDIAL);
	m_edtIPDial.SetLimitText(15);
	m_edtIPDial.SetLimitDiagital();

	//本地区号
	xbegin = 121-nSpace;
	xWidth = 65  ;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_AREACODE);
	str = strTemp.c_str();
	m_sticLocalAreaNumber.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, 64*VHEIGHT, xbegin+xWidth, 80*VHEIGHT), this);//64 80
	m_sticLocalAreaNumber.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	xbegin = 200-nSpace ;
	xWidth = 60  ;
	m_edtLocalAreaNumber.Create(WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, 64*VHEIGHT, xbegin+xWidth, 80*VHEIGHT), this, IDC_EDIT_SETTING_AREANO);
	m_edtLocalAreaNumber.SetLimitText(15);
	m_edtLocalAreaNumber.SetLimitDiagital();

	//音量级别
	xbegin = 270-nSpace;
	xWidth = 53 ;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_VOLUME);//音量
	str = strTemp.c_str();
	m_sticVolume.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, 64*VHEIGHT, xbegin+xWidth, 80*VHEIGHT), this);//8298
	m_sticVolume.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	xbegin = xbegin+xWidth +4 ;
	xWidth = 60 ;
	m_cmbVolume.Create(WS_CHILD|WS_VISIBLE,\
		CRect(xbegin,64,xbegin+xWidth,180), this, IDC_COMBOBOX_SETTING_RING);
	
	//启用特色振铃
	xbegin = 105-nSpace ;
	xWidth = 150 ;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLERING);//102118
	str = strTemp.c_str();
	m_chbEnableRing.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX,\
		CRect(xbegin, 84*VHEIGHT, xbegin+xWidth, 100*VHEIGHT), this, IDC_CHECK_SETTING_RING);
	m_chbEnableRing.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	SetButtonDefaultColor(&m_chbEnableRing);
	
	//默认振铃
	xbegin = 121-nSpace;
	xWidth = 75;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLERINGSTC);
	str = strTemp.c_str();
	m_sticDefaultRing.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, 102*VHEIGHT, xbegin+xWidth, 118*VHEIGHT), this);//8298
	m_sticDefaultRing.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	xbegin = 200-nSpace;
	xWidth = 166;
	m_cmbRing.Create(WS_CHILD|WS_VISIBLE, \
		CRect(xbegin,102,xbegin+xWidth,230), this, IDC_COMBOBOX_SETTING_TIP);

	//试听
	xbegin = 370-nSpace;
	xWidth = 56;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LISTEN);
	str = strTemp.c_str();
	m_btnPlayRing.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle],\
		Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, 101*VHEIGHT, xbegin+xWidth, 121*VHEIGHT), this, IDC_BUTTON_SETTING_PLAYRING);
	m_btnPlayRing.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	//启动自动留言  启动前振铃次数
	xbegin = 105-nSpace;
	xWidth = 185;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLEAUTOLEAVEWORD);
	str = strTemp.c_str();
	m_chbEnableAutoLeaveWord.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX,\
		CRect(xbegin, 126*VHEIGHT, xbegin+xWidth, 142*VHEIGHT), this, IDC_CHECK_SETTING_AUTOLEAVEWORD);
	m_chbEnableAutoLeaveWord.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	SetButtonDefaultColor(&m_chbEnableAutoLeaveWord);
	
	//铃声次数
	xbegin = 290-nSpace;
	xWidth = 76;
	m_cmbRingTimes.Create(WS_CHILD|WS_VISIBLE, \
		CRect(xbegin,126,xbegin+xWidth,210), this, IDC_COMBOBOX_SETTING_RINGTIMES);
   
	//启用留言外放
	xbegin = 368-nSpace;
	xWidth = 67;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLESpeakerOut);
	str = strTemp.c_str();
	m_IsSpeakerOut.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX,\
		CRect(xbegin, 126*VHEIGHT, xbegin+67, 142*VHEIGHT), this, IDC_CHECK_SETTING_AUTOLEAVEWORD);
    m_IsSpeakerOut.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	SetButtonDefaultColor(&m_IsSpeakerOut);
	
	//根据语言移动位置
	MoveControl();

	//默认提示语
	xbegin = 121-nSpace ;
	xWidth = 75  ;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RINGTIMES);
	str = strTemp.c_str();
	m_sticRingTimes.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, 146*VHEIGHT, xbegin+75, 162*VHEIGHT), this);
	m_sticRingTimes.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	xbegin = 200-nSpace;
	xWidth = 166;
	m_cmbTip.Create(WS_CHILD|WS_VISIBLE,\
		CRect(xbegin,146,xbegin+xWidth,230), this, IDC_COMBOBOX_SETTING_TIP);
	
	//播放提示语
	xbegin = 370-nSpace;
	xWidth = 56 ;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LISTEN);
	str = strTemp.c_str();
	m_btnPlayLeaveWordTip.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle],\
		Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], \
		WS_CHILD|WS_VISIBLE, CRect(xbegin, 145*VHEIGHT, xbegin+xWidth, 145*VHEIGHT+23), this,\
		IDC_BUTTON_SETTING_PLAY);
	m_btnPlayLeaveWordTip.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	//设置速拨
	xbegin = 120-nSpace;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DEFINESPEEDBUTTON);
	str = strTemp.c_str();
	m_btnDefineSpeedButton.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle],\
		Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, 166*VHEIGHT, xbegin+xWidth, 186*VHEIGHT), this, IDC_BUTTON_SETTING_DEFINEFASTDAIL);
	m_btnDefineSpeedButton.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	//可以设置电话的5个速拨按键
	xbegin = 180-nSpace;
	ybegin = 230;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DEFINESPEEDBUTTONSTC);
	str = strTemp.c_str();
	m_sticDefineSpeedButton.Create(str, WS_CHILD|WS_VISIBLE, \
		CRect(xbegin, 168*VHEIGHT, xbegin+230, 184*VHEIGHT), this);
	m_sticDefineSpeedButton.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	//录音设置
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SETPASSWORD);
	str = strTemp.c_str();
	m_btnSetPlayPassword.Create(str, \
		Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle],\
		Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle],\
		WS_CHILD|WS_VISIBLE,\
		CRect(284, 30*VHEIGHT, 340, 50*VHEIGHT),\
		this, IDC_BUTTON_SETTING_SETPLAYPASSWORD);
	m_btnSetPlayPassword.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLEPLAY);
	str = strTemp.c_str();
	m_chbEnabePlayProtect.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX,\
		CRect(85, 32*VHEIGHT, 280, 48*VHEIGHT), this, IDC_CHECK_SETTING_PLAY);
	m_chbEnabePlayProtect.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	SetButtonDefaultColor(&m_chbEnabePlayProtect);	

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLERECORD);
	str = strTemp.c_str();
	m_chbEnabeRecord.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, \
		CRect(85, 52*VHEIGHT, 210, 68*VHEIGHT), this, IDC_CHECK_SETTING_RECORD);
	m_chbEnabeRecord.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	SetButtonDefaultColor(&m_chbEnabeRecord);	
	
	//启用高质量录音
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLEDELETE);
	str = strTemp.c_str();
	m_chbEnabeDeleteProtect.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, \
		CRect(85, 72*VHEIGHT, 250, 88*VHEIGHT), this, IDC_CHECK_SETTING_DELETE);
	m_chbEnabeDeleteProtect.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	SetButtonDefaultColor(&m_chbEnabeDeleteProtect);	
	
	strTemp = Data::LanguageResource::Get(Data::RI_MAX_REC_TIME);
	str = strTemp.c_str();
	m_sticRecordLimit.Create(str, WS_CHILD|WS_VISIBLE, \
		CRect(86, 92*VHEIGHT, 220, 108*VHEIGHT), this);
	m_sticRecordLimit.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_cmbRecordLimit.Create(WS_CHILD|WS_VISIBLE, CRect(220, 92, 300, 300), this);
	
	//browser======================================================================================================

	//系统参数======================================================================================================
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SETPLAYPASSWORD);
	str = strTemp.c_str();
	m_chbSetAdminPassword.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(85, 30*VHEIGHT, 340, 46*VHEIGHT), this, IDC_CHECK_SETTING_ADMINPASSWORD);
	m_chbSetAdminPassword.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	SetButtonDefaultColor(&m_chbSetAdminPassword);	
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SETPASSWORD);
	str = strTemp.c_str();
	m_btnSetAdminPassword.Create(str,\
		Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle],\
		Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle],\
		WS_CHILD|WS_VISIBLE, CRect(344, 28*VHEIGHT, 400, 48*VHEIGHT-1), \
		this, IDC_BUTTON_SETTING_SETADMINPASSWORD);
	m_btnSetAdminPassword.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLESP);
	str = strTemp.c_str();
	m_chbEnnabeScreenProtect.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(85, 48*VHEIGHT, 210, 64*VHEIGHT), this, IDC_CHECK_SETTING_SCREENPROTECT);
	m_chbEnnabeScreenProtect.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	SetButtonDefaultColor(&m_chbEnnabeScreenProtect);

	//等待时间
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_WAITTIME);
	str = strTemp.c_str();
	m_sticWaitTime.Create(str, WS_CHILD|WS_VISIBLE, CRect(100, 64*VHEIGHT, 200, 80*VHEIGHT), this);
	m_sticWaitTime.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	//combox 
	m_cmbWaitTime.Create(WS_CHILD|WS_VISIBLE, CRect(204, 64, 300, 170), this, IDC_COMBOBOX_SETTING_WAITTIME);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLEPASSWORD);
	str = strTemp.c_str();
	m_chbEnablePassword.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(100, 84*VHEIGHT, 240, 100*VHEIGHT), this, IDC_CHECK_SETTING_SPPASSWORD);
	m_chbEnablePassword.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	SetButtonDefaultColor(&m_chbEnablePassword);	
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SETPASSWORD);
	str = strTemp.c_str();
	m_btnSetPassword.Create(str,\
		Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], \
		Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], \
		WS_CHILD|WS_VISIBLE, CRect(344, 82*VHEIGHT, 400, 102*VHEIGHT-1),\
		this, IDC_BUTTON_SETTING_SETPASSWORD);
	m_btnSetPassword.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	//背光持续时间
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_BACKLIGHT);
	str = strTemp.c_str();
	m_sticBacklight.Create(str, WS_CHILD|WS_VISIBLE, CRect(86, 106*VHEIGHT, 200, 122*VHEIGHT), this);
	m_sticBacklight.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	m_cmbBacklight.Create(WS_CHILD|WS_VISIBLE, CRect(204, 106, 300, 210), this, IDC_COMBOBOX_SETTING_SOUNDSAVEPATH);

	//录音文件存储位置
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SOUNDSAVEPATH);
	str = strTemp.c_str();
	m_sticSoundSavePath.Create(str, WS_CHILD|WS_VISIBLE, CRect(86, 126*VHEIGHT, 200, 142*VHEIGHT), this);
	m_sticSoundSavePath.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	m_cmbSoundSavePath.Create(WS_CHILD|WS_VISIBLE, CRect(204, 126, 300, 190), this, IDC_COMBOBOX_SETTING_SOUNDSAVEPATH);

	//时间设置
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SYSTEMTIME);
	str = strTemp.c_str();
	m_stcTime.Create(str, WS_CHILD|WS_VISIBLE, CRect(86, 146*VHEIGHT, 200, 162*VHEIGHT), this);
	m_stcTime.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_dtDate.Create(WS_VISIBLE|WS_CHILD|DTS_SHORTDATEFORMAT, CRect(204, 146*VHEIGHT, 304, 162*VHEIGHT), this, IDC_SETTING_DATE);
	m_dtTime.Create(WS_VISIBLE|WS_CHILD|DTS_TIMEFORMAT, CRect(310, 144*VHEIGHT, 406, 162*VHEIGHT), this, IDC_SETTING_TIME);
	CTime time1 = CTime(2007, 1, 1, 0, 0, 0);
	CTime time2 = CTime(2031, 1, 1, 0, 0, 0);
	m_dtDate.SetRange(&time1, &time2);
	GetLocalTime(&m_curtime);
	m_dtDate.SetTime(m_curtime);
	m_dtTime.SetTime(m_curtime);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SCREENADJUSTSTC);
	str = strTemp.c_str();
	m_sticTouchAdjust.Create(str, WS_CHILD|WS_VISIBLE, CRect(160, 166*VHEIGHT, 410, 182*VHEIGHT), this);
	m_sticTouchAdjust.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SCREENADJUST);
	str = strTemp.c_str();
	m_btnTouchAdjust.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(100, 164*VHEIGHT, 156, 184*VHEIGHT-1), this, IDC_BUTTON_SETTING_ADJUSTTOUCH);
	m_btnTouchAdjust.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	//系统信息======================================================================================================
	xbegin   = 90  ;
	xWidth   = 53  ;
	ybegin   = 28  ;
	yheight  = 16  ;

	GetSystemInfo(); //wzx 获取系统版本信息
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SYSTOMTITLE);
	str = strTemp.c_str();
	m_sticSystomTitle.Create(str, WS_CHILD/*|WS_VISIBLE*/,\
		CRect(90, 28*VHEIGHT, 170, 44*VHEIGHT), this);
	m_sticSystomTitle.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
    
	//序列号
	val = 4;
	ybegin = 28 ;
	xWidth = 56;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SERIALNO);
	str = strTemp.c_str();
	m_sticSystomSerialNo1.Create(str,WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, ybegin*VHEIGHT, xbegin+xWidth, (ybegin+yheight)*VHEIGHT), this);
	m_sticSystomSerialNo1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticSystomSerialNo.Create(CString(m_sSerialNo),WS_CHILD|WS_VISIBLE,\
		CRect(xbegin+xWidth+4, ybegin*VHEIGHT, xbegin+xWidth+4+200, (ybegin+yheight)*VHEIGHT), this);
	m_sticSystomSerialNo.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	//软件版本
	xWidth = 98;
	ybegin += yheight + val ;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SOFTVERSION);
	str = strTemp.c_str();	
	m_sticSoftwareVersion1.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, ybegin*VHEIGHT, xbegin+xWidth, (ybegin+yheight)*VHEIGHT), this);
	m_sticSoftwareVersion1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	xbegin = xbegin + xWidth+1 ;
	str = s_VerSion;
	m_sticSoftwareVersion.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, ybegin*VHEIGHT, xbegin+200, (ybegin+yheight)*VHEIGHT), this, IDC_SETTING_VERSION);
	m_sticSoftwareVersion.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	m_editVersion.Create(WS_VISIBLE|WS_CHILD|\
		WS_BORDER|ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL,\
		CRect(80, 70*VHEIGHT, 400, 196*VHEIGHT), this, IDC_SETTING_EDITVERSION );
	m_editVersion.ShowWindow(SW_HIDE);
	m_editVersion.SetWindowText(s_VerSion);
	
	//硬件版本
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_HARDVERSION);
	str = strTemp.c_str();
	m_sticHardwareVersion1.Create(str, WS_CHILD/*|WS_VISIBLE*/, CRect(240, 50*VHEIGHT, 293, 66*VHEIGHT), this);
	m_sticHardwareVersion1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	m_sticHardwareVersion.Create(CString(m_sHardwareNo), WS_CHILD/*|WS_VISIBLE*/, CRect(295, 50*VHEIGHT, 410, 66*VHEIGHT), this);
	m_sticHardwareVersion.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	//本地空间
	ybegin += yheight + val ;
	xbegin  = 90 ;
	xWidth  = 95 ;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_FLASHSIZE);
	str = strTemp.c_str();
	m_sticMemorySize1.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, ybegin*VHEIGHT, xbegin+xWidth, (ybegin+yheight)*VHEIGHT),\
		this);m_sticMemorySize1.SetColor(RGB(0, 0, 0),\
		Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	str = "";
	m_sticMemorySize.Create(str, WS_CHILD|WS_VISIBLE, \
		CRect(xbegin+xWidth+4, ybegin*VHEIGHT, xbegin+xWidth+4+200, (ybegin+yheight)*VHEIGHT), this);
	m_sticMemorySize.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	//SD卡空间
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SDSTORAGESIZE);
	str = strTemp.c_str();
	m_sticStorageSize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(90, 86*VHEIGHT, 143, 102*VHEIGHT), this);
	m_sticStorageSize1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	str = "";
	m_sticStorageSize.Create(str, WS_CHILD|WS_VISIBLE, CRect(145, 86*VHEIGHT, 340, 102*VHEIGHT), this);
	m_sticStorageSize.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	//格式化
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_FORMATSD);
	str = strTemp.c_str();
	m_btnFormatSD.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(344, 82, 400, 102), this, IDC_BUTTON_SETTING_FORMATSD);
	m_btnFormatSD.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	//电话名片
	ybegin = 86 ;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CONTACTSIZE);
	str = strTemp.c_str();
	m_sticContactSize1.Create(str, WS_CHILD|WS_VISIBLE, \
		CRect(xbegin, ybegin*VHEIGHT, xbegin+xWidth, (ybegin+yheight)*VHEIGHT), this);
	m_sticContactSize1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	str = "";
	m_sticContactSize.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin+xWidth+4, ybegin*VHEIGHT, xbegin+xWidth+4+xWidth2, (ybegin+yheight)*VHEIGHT), this);
	m_sticContactSize.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	//清空名片按钮
	int nBtnY = 100;
	int nBtnX = 274;
	int nBtnHeight = 23;
	int nBtnWidth  = 56;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARCONTACT);
	str = strTemp.c_str();
	m_btnClearContact.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle],\
		Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle],\
		WS_CHILD|WS_VISIBLE, CRect(nBtnX, nBtnY, nBtnX+nBtnWidth, (nBtnY+nBtnHeight)),\
		this, IDC_BUTTON_SETTING_CLEARCONTACT);
	m_btnClearContact.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	//通话记录
	ybegin += yheight + val ;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CONTACTINFOSIZE);
	str = strTemp.c_str();
	m_sticContactInfoSize1.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, ybegin*VHEIGHT, xbegin+xWidth, (ybegin+yheight)*VHEIGHT), this);
	m_sticContactInfoSize1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	str = "";
	m_sticContactInfoSize.Create(str, WS_CHILD|WS_VISIBLE, \
		CRect(xbegin+xWidth+4, ybegin*VHEIGHT, xbegin+xWidth+4+xWidth2, (ybegin+yheight)*VHEIGHT),\
		this);
	m_sticContactInfoSize.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	//清楚通话记录按钮
	nBtnY += nBtnHeight ;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARCONTACTINFO);
	str = strTemp.c_str();
	m_btnClearContactInfo.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle],\
		Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle],\
		WS_CHILD|WS_VISIBLE, CRect(nBtnX, nBtnY, nBtnX+nBtnWidth,  nBtnY+nBtnHeight),\
		this, IDC_BUTTON_SETTING_CLEARCONTACTINFO);
	m_btnClearContactInfo.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	//录音管理
	ybegin += yheight + val ;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SOUNDSIZE);
	str = strTemp.c_str();
	m_sticSoundSize1.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, ybegin*VHEIGHT, xbegin+xWidth, (ybegin+yheight)*VHEIGHT), this);
	m_sticSoundSize1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	str = "";
	m_sticSoundSize.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin+xWidth+4, ybegin*VHEIGHT, xbegin+xWidth+4+xWidth2, (ybegin+yheight)*VHEIGHT), this);
	m_sticSoundSize.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	//清除录音按钮
	nBtnY += nBtnHeight ;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARSOUND);
	str = strTemp.c_str();
	m_btnClearSound.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle],\
		Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], \
		WS_CHILD|WS_VISIBLE,\
		CRect(nBtnX, nBtnY, nBtnX+nBtnWidth, (nBtnY+nBtnHeight)), \
		this, IDC_BUTTON_SETTING_CLEARSOUND);
	m_btnClearSound.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	//未接电话数
	ybegin += yheight + val ;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_UNCONNECTNUM);
	str = strTemp.c_str();
	m_sticUconnectNum.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, ybegin*VHEIGHT, xbegin+xWidth, (ybegin+yheight)*VHEIGHT), this);
	m_sticUconnectNum.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	str = "";
	m_sticUconnectNum1.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin+xWidth+4, ybegin*VHEIGHT, xbegin+xWidth+4+xWidth2, (ybegin+yheight)*VHEIGHT), this);
	m_sticUconnectNum1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	//清除未接电话按钮
	nBtnY += nBtnHeight;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ONEKEYCLEAR);
	str = strTemp.c_str();
	m_btnSetOneKeyClear.Create(str,\
		Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle],\
		Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle],\
		WS_CHILD|WS_VISIBLE,\
		CRect(nBtnX, nBtnY, nBtnX+nBtnWidth,   (nBtnY+nBtnHeight)),\
		this, IDC_BUTTON_SETTING_ONEKEYCLEAR);
	m_btnSetOneKeyClear.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	//
	
	//系统语言
	ybegin += yheight + val ;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_VERSIONCHANGE);
	str = strTemp.c_str();
	m_versionChange.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, ybegin*VHEIGHT, xbegin+xWidth, (ybegin+yheight)*VHEIGHT), this);
    m_versionChange.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	//语言变换按钮
	nBtnY += nBtnHeight ;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LANG);
	str = strTemp.c_str();
	m_btnLangChange.Create(str, \
		Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle],\
		Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle],\
		WS_CHILD|WS_VISIBLE,\
		CRect(nBtnX, nBtnY, nBtnX+nBtnWidth, (nBtnY+nBtnHeight)),\
		this, IDC_BUTTON_SETTING_LANGCHANGE);
	m_btnLangChange.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_INFORMATIONSIZE);
	str = strTemp.c_str();
	m_sticInformationSize1.Create(str, WS_CHILD/*|WS_VISIBLE*/,\
		CRect(xbegin+xWidth+4, 164*VHEIGHT, xbegin+xWidth+4+xWidth2, 180*VHEIGHT), this);
	m_sticInformationSize1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	str = "";
	m_sticInformationSize.Create(str, WS_CHILD|WS_VISIBLE, CRect(194, 164*VHEIGHT, 270, 180*VHEIGHT), this);
	m_sticInformationSize.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARINFORMATION);
	str = strTemp.c_str();
	m_btnClearInformation.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(274, 162*VHEIGHT, 330, 182*VHEIGHT), this, IDC_BUTTON_SETTING_CLEARINFO);
	m_btnClearInformation.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	//版权所有
	xWidth = 60;
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_VERSIONRIGHT);
	str = strTemp.c_str();
	m_sticVersionRight1.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin, 182*VHEIGHT, xbegin+xWidth, 198*VHEIGHT), this);
	m_sticVersionRight1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	strTemp=Data::LanguageResource::Get(Data::RI_COMPANY);
	str =strTemp.c_str();
	m_sticVersionRight.Create(str, WS_CHILD|WS_VISIBLE,\
		CRect(xbegin+xWidth+4, 182*VHEIGHT, xbegin+xWidth+4+200, 198*VHEIGHT), this);
	m_sticVersionRight.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	//RingInfo
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RINGSIZE);
	str = strTemp.c_str();
	m_sticRingInfo.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 40*VHEIGHT, 170, 56*VHEIGHT), this);
	m_sticRingInfo.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	str = "";
	m_sticRingSize.Create(str, WS_CHILD|WS_VISIBLE, CRect(182, 40*VHEIGHT, 202, 56*VHEIGHT), this);
	m_sticRingSize.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RINGCLEAR);
	str = strTemp.c_str();
	m_btnClearRing.Create(str, \
		Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle],\
		Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle],\
		WS_CHILD|WS_VISIBLE, CRect(248, 40*VHEIGHT, 304, 40*VHEIGHT+23),\
		this, IDC_BTN_CLEARRING);
	m_btnClearRing.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RINGLIST);
	str = strTemp.c_str();
	m_sticRingList.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 60*VHEIGHT, 180, 76*VHEIGHT), this);
	m_sticRingList.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RINGDELETE);
	str = strTemp.c_str();
	m_btnDeleteRing.Create(str,\
		Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle],\
		Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle],\
		WS_CHILD|WS_VISIBLE, CRect(248, 80*VHEIGHT, 304, 80*VHEIGHT+23),\
		this, IDC_BTN_DELETERING);
	m_btnDeleteRing.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	m_cmbDelRingList.Create(WS_CHILD|WS_VISIBLE, CRect(104, 70*VHEIGHT, 244, 360), this, IDC_COMBOBOX_RINGLIST);
	
	//数据导入导出
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TITILEBACKUP);
	str = strTemp.c_str();
	m_stcBackUP.Create(str, WS_VISIBLE|WS_VISIBLE, CRect(24, 64*VHEIGHT, 214, 300*VHEIGHT), this);
	m_stcBackUP.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TITLERESTORE);
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_BACKUP);
	str = strTemp.c_str();
	m_btnBackUP.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(282, 30*VHEIGHT, 338, 50*VHEIGHT), this, IDC_SETTING_BACKUP);
	m_btnBackUP.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RESTORE);
	str = strTemp.c_str();
	m_btnReStore.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(342, 30*VHEIGHT, 398, 50*VHEIGHT), this, IDC_SETTING_RESTORE);
	m_btnReStore.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TITLE);
	str = strTemp.c_str();
	m_stcTitle.Create(str, WS_VISIBLE|WS_VISIBLE, CRect(90, 53*VHEIGHT, 180, 73*VHEIGHT), this);
	m_stcTitle.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_OUT);
	str = strTemp.c_str();
	m_btnOut.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(182, 53*VHEIGHT, 238, 73*VHEIGHT), this, IDC_SETTING_OUT);
	m_btnOut.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	m_sticPanel.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(416, 26*VHEIGHT, 472, 198*VHEIGHT), this);
	m_sticPanel.SetColor(RGB(0, 0, 0), Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);

	m_sticPanel2.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(81, 27*VHEIGHT, 416, 198*VHEIGHT), this);
	m_sticPanel2.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	m_sticBackground.Create(CRect(0, 0, 480, 204*VHEIGHT), this, 2);
	
	TextStruct ts[3];
	memset(ts, 0, sizeof(TextStruct) * 3);
	
	ts[0].txtRect = CRect(0, 0, 80, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_TYPE).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_TYPE).length());
	
	ts[1].txtRect = CRect(81, 0, 415, 20*VHEIGHT);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[1].sTxt, Data::LanguageResource::Get(Data::RI_SETTING_DETAILTITLE).c_str(), Data::LanguageResource::Get(Data::RI_SETTING_DETAILTITLE).length());
	
	ts[2].txtRect = CRect(415, 0, 475, 20*VHEIGHT);
	ts[2].txtFontSize = 16;
	ts[2].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[2].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).length());
		
	m_sticBackground.SetTextStruct(ts, 3);

	m_pFastDialsDlg = new CFastDialsDlg(this);//?
	m_pFastDialsDlg->Create(CFastDialsDlg::IDD);

	m_pPasswordDlg = new CPasswordDlg(this);
	m_pPasswordDlg->Create(CPasswordDlg::IDD);

	m_copyfileDlg = new copyfileDlg(this);
	m_copyfileDlg->Create(copyfileDlg::IDD);

	m_pOggCodec = new OggCodec();
	m_bLogin = false;
	m_iPasswordType = -1;
	m_bTelephoneInUse = false;

	m_bPlaying=false;

	m_bPlayPause=false;

	m_pSetting = Data::Setting::GetCurrentConfig();

	IniCtrlData();

	m_lsType.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

	ShowConfigItems();
	//开机时检测到启用密码屏保的话直接进入屏保
	if(m_pSetting->isUseScreenSaver() && m_pSetting->isUseScreenSaverPassword())
	{
		SetTimer(97, 50, NULL);
	}
	
	#if outputstring_data 
	
	 m_pRegRecordSound=new CRegRecordSound(this);
	 m_pRegRecordSound->Create(CRegRecordSound::IDD);
	
     #endif
  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingDlg::SetButtonDefaultColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
}

void CSettingDlg::SetButtonSelectedColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
}

void CSettingDlg::OnClickListType(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ShowConfigItems();
	*pResult = 0;
}

void CSettingDlg::OnListCltrlClick(WPARAM w, LPARAM l)
{
	LRESULT ret;
	if(w == IDC_LIST_SETTING_TYPE)
		OnClickListType(NULL, &ret);
	//else if(w == IDC_LIST_CONTACT_LIST)
	//	OnClickListList(NULL, &ret);
}

void CSettingDlg::IniCtrlData()
{
	if ((((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsHangOff))
	{
		m_pOggCodec->StopDecode(true);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->StopRing(true);

	}
	m_edtLocalAreaNumber.SetWindowText(Util::StringOp::ToCString(m_pSetting->thisTelephoneNumber().number()));
	m_edtLocalNumber.SetWindowText(Util::StringOp::ToCString(m_pSetting->LocalNumber()));

	if (m_pSetting->isAutoAppendOutlinePrefix())
	{
		m_chbEnableOutLine.SetCheck(BST_CHECKED);
	}
	else
	{
		m_chbEnableOutLine.SetCheck(BST_UNCHECKED);
	}
	m_edtOutLine.SetWindowText(Util::StringOp::ToCString(m_pSetting->outlinePrefix()));

	if (m_pSetting->isAppendIpPrefix())
	{
		m_chbEnableIPDial.SetCheck(BST_CHECKED);
	}
	else
	{
		m_chbEnableIPDial.SetCheck(BST_UNCHECKED);
	}
	m_edtIPDial.SetWindowText(Util::StringOp::ToCString(m_pSetting->ipPrefix()));

	if (m_pSetting->isUseSpecRing())
	{
		m_chbEnableRing.SetCheck(BST_CHECKED);
	}
	else
	{
		m_chbEnableRing.SetCheck(BST_UNCHECKED);
	}

	m_cmbRing.ResetContent();	
	m_cmbDelRingList.ResetContent();
	WIN32_FIND_DATA FindFileData;//查找文件时要使用的数据结构
	HANDLE hFind = INVALID_HANDLE_VALUE;	//定义查找句柄
	
	hFind = FindFirstFile(csFlashRingPath + _T("*.*"), &FindFileData);//使用FindFirstFile函数来开始文件查找
	
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		CString filename;

		filename =  FindFileData.cFileName;

		if ((filename.Right(4).CompareNoCase(_T(".mp3")) == 0)
			|| (filename.Right(4).CompareNoCase(_T(".wav")) == 0))
		{
			m_cmbRing.AddString(filename);
			m_cmbDelRingList.AddString(filename);
		}
		
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			filename =  FindFileData.cFileName;
			if ((filename.Right(4).CompareNoCase(_T(".mp3")) == 0)
				|| (filename.Right(4).CompareNoCase(_T(".wav")) == 0))
			{
				m_cmbRing.AddString(filename);
				m_cmbDelRingList.AddString(filename);
			}
		}
		
		FindClose(hFind);
	}

	CString ringName = Util::StringOp::ToCString(m_pSetting->defaultRingFilename());
	ringName = ringName.Right(ringName.GetLength() - ringName.ReverseFind('\\') - 1);
	int i = 0;
	CString txt;
	for (i = 0; i < m_cmbRing.GetCount(); i++)
	{
		m_cmbRing.GetLBText(i, txt);
		if (ringName == txt)
		{
			m_cmbRing.SetCurSel(i);
			m_cmbDelRingList.SetCurSel(i);
			break;
		}
		else
		{
			m_cmbRing.SetCurSel(0);
			m_cmbDelRingList.SetCurSel(0);
		}
	}
	
	m_cmbVolume.ResetContent();
	m_cmbVolume.AddString(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_LV1)));
	m_cmbVolume.AddString(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_LV2)));
	m_cmbVolume.AddString(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_LV3)));
	m_cmbVolume.AddString(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_LV4)));
	m_cmbVolume.AddString(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_LV5)));
	int v = m_pSetting->soundVolume();
	
	m_cmbVolume.SetCurSel(v);

	DWORD volume[]={0x10001000,0x44004400,0x88008800,0xcc00cc00,0xff00ff00};//调节 音量
	int volume2[] = {-2400, -1800, -1200, -600, -1};
	waveOutSetVolume(NULL, volume[v]);
 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetSoundRingVal(volume2[v]);

	if (m_pSetting->isAutoReply())
	{
		m_chbEnableAutoLeaveWord.SetCheck(BST_CHECKED);
	}                                          
	else
	{
		m_chbEnableAutoLeaveWord.SetCheck(BST_UNCHECKED);
	}
	if (m_pSetting->linkMode()==Data::lmDirect)
	{
		m_IsSpeakerOut.SetCheck(BST_CHECKED);

	}else
	{
		m_IsSpeakerOut.SetCheck(BST_UNCHECKED);
	}

	m_cmbTip.ResetContent();
	hFind = FindFirstFile(csHiveTipPath + _T("*.*"), &FindFileData);//使用FindFirstFile函数来开始文件查找
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		CString filename;
		filename =  FindFileData.cFileName;
		if ((filename.Right(4).CompareNoCase(_T(".mp3")) == 0)
			|| (filename.Right(4).CompareNoCase(_T(".wav")) == 0))
		{
			m_cmbTip.InsertString(-1,filename);
		}
		
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			filename =  FindFileData.cFileName;
			if ((filename.Right(4).CompareNoCase(_T(".mp3")) == 0)
				|| (filename.Right(4).CompareNoCase(_T(".wav")) == 0) )
			{
				m_cmbTip.InsertString(-1,filename);
			}
		}
		
		FindClose(hFind);
	}

	for (i = 0; i < m_cmbTip.GetCount(); i++)
	{
		m_cmbTip.SetItemData(i, 2);
	}
	
	//添加设置的提示语
	if (m_vTipsDir.size())
	{
		m_vTipsDir.clear();
	}

	int pos = 0;
	CString s;
	std::vector<boost::shared_ptr<Data::SoundSegment> > sounds =\
		Data::SoundSegment::GetFromDatabase("type = 2 AND isTip = 1");
	for (i = 0; i < sounds.size(); ++i)
	{	
		//add by qi 20110803
		CString tipsDir;//提示语的目录

		pos = sounds[i]->filename().rfind('\\');
		m_cmbTip.InsertString(-1,Util::StringOp::ToCString(sounds[i]->filename().substr(pos + 1)));
		
		//add by qi 20110803
		tipsDir = Util::StringOp::ToCString(sounds[i]->filename().substr(0,pos+1));
		m_vTipsDir.push_back(tipsDir);
		m_cmbTip.SetItemData(m_cmbTip.GetCount()-1,i);
		
		//

//		change by qi 20110803
// 		if (sounds[i]->filename().substr(0, pos + 1) == ssFlashRecordPath)
// 		{
// 			m_cmbTip.SetItemData(m_cmbTip.GetCount()-1, 0);
// 		}
// 		else
// 		{
// 			m_cmbTip.SetItemData(m_cmbTip.GetCount()-1, 1);
// 		}
	}
    
	//显示提示语
	pos = m_pSetting->defaultTipFilename().rfind('\\');
	CString tipName = Util::StringOp::ToCString(m_pSetting->defaultTipFilename().substr(pos + 1));
	for (i = 0; i < m_cmbTip.GetCount(); i++)
	{
		m_cmbTip.GetLBText(i, txt);

		if (tipName.CompareNoCase(txt) == 0)
		{
			m_cmbTip.SetCurSel(i);
			break;
		}
		else
		{
			m_cmbTip.SetCurSel(0);
		}
	}

	m_cmbRingTimes.ResetContent();
	m_cmbRingTimes.AddString(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_RING_TIME3)));
	m_cmbRingTimes.AddString(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_RING_TIME4)));
	m_cmbRingTimes.AddString(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_RING_TIME5)));
	m_cmbRingTimes.AddString(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_RING_TIME6)));
	int nRingCount = m_pSetting->autoReplyRingCount();
	m_cmbRingTimes.SetCurSel(nRingCount - 3);
	
	if (m_pSetting->isPlayProtect())
	{
		m_chbEnabePlayProtect.SetCheck(BST_CHECKED);
		m_pPasswordDlg->SetFisrtPPassword(TRUE);
	}
	else
	{
		m_chbEnabePlayProtect.SetCheck(BST_UNCHECKED);
	}
	if (m_pSetting->isMustRecord())
	{
		m_chbEnabeRecord.SetCheck(BST_CHECKED);
	}
	else
	{
		m_chbEnabeRecord.SetCheck(BST_UNCHECKED);
	}
	if (m_pSetting->isDeleteProtect())
	{
		m_chbEnabeDeleteProtect.SetCheck(BST_CHECKED);
	}
	else
	{
		m_chbEnabeDeleteProtect.SetCheck(BST_UNCHECKED);
	}
	if (m_pSetting->isAdmin())
	{
		m_chbSetAdminPassword.SetCheck(BST_CHECKED);
		m_btnLogin.ShowWindow(TRUE);
	}
	else
	{
		m_chbSetAdminPassword.SetCheck(BST_UNCHECKED);
		m_btnLogin.ShowWindow(FALSE);
	}

	m_cmbRecordLimit.ResetContent();
	m_cmbRecordLimit.AddString(_T("1")+Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_SYSTEM_MINUTE))));
	m_cmbRecordLimit.AddString(_T("2")+Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_SYSTEM_MINUTE))));
	m_cmbRecordLimit.AddString(_T("5"+Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_SYSTEM_MINUTE)))));
	m_cmbRecordLimit.AddString(_T("50"+Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_SYSTEM_MINUTE)))));//?
	int l = m_pSetting->maxSoundUseSize();
	if(l == 600*5)
		m_cmbRecordLimit.SetCurSel(3);
	else if(l == 120)
		m_cmbRecordLimit.SetCurSel(1);
	else if(l == 300)
		m_cmbRecordLimit.SetCurSel(2);
	else
		m_cmbRecordLimit.SetCurSel(0);

	if (m_pSetting->isUseScreenSaver())
	{
		m_chbEnnabeScreenProtect.SetCheck(BST_CHECKED);
	}
	else
	{
		m_chbEnnabeScreenProtect.SetCheck(BST_UNCHECKED);
	}

	s = Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_SYSTEM_SECOND)));
	CString m = Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_SYSTEM_MINUTE)));
	
	m_cmbWaitTime.ResetContent();
	CString sSecond[5] = {"15", "30", "1", "5", "10"};
	m_cmbWaitTime.AddString(sSecond[0] + s);
	m_cmbWaitTime.AddString(sSecond[1] + s);
	m_cmbWaitTime.AddString(sSecond[2] + m);
	m_cmbWaitTime.AddString(sSecond[3] + m);
	m_cmbWaitTime.AddString(sSecond[4] + m);
	CTimeSpan time = m_pSetting->screenSaverDuration();
	int nSel = 0;
	int nSeconds = time.GetTotalSeconds();
	if(nSeconds == 15)
	{
		nSel = 0;
	}
	else if(nSeconds == 30)
	{
		nSel = 1;
	}
	else if(nSeconds == 60)
	{
		nSel = 2;
	}
	else if(nSeconds == 5*60)
	{
		nSel = 3;
	}
	else if(nSeconds == 10*60)
	{
		nSel = 4;
	}
	m_cmbWaitTime.SetCurSel(nSel);

	if (m_pSetting->isUseScreenSaverPassword())
	{
		m_chbEnablePassword.SetCheck(BST_CHECKED);
		m_pPasswordDlg->SetFisrtSPassword(TRUE);
	}
	else
	{
	   m_chbEnablePassword.SetCheck(BST_UNCHECKED);
	}

    std::string tp = Data::LanguageResource::Get(Data::RI_SETTING_BACK_TIME);
	m_cmbBacklight.ResetContent();
	m_cmbBacklight.AddString(Util::StringOp::ToCString(tp));//?
	m_cmbBacklight.AddString(_T("15") + s);
	m_cmbBacklight.AddString(_T("30") + s);
	m_cmbBacklight.AddString(_T("45") + s);
	m_cmbBacklight.AddString(_T("60") + s);
	time = m_pSetting->backlightDuration();
	nSeconds = time.GetTotalSeconds();
	m_cmbBacklight.SetCurSel(nSeconds / 15);

	m_cmbSoundSavePath.ResetContent();
	std::string temp = Data::LanguageResource::Get(Data::RI_SETTING_LOCAL);
	m_cmbSoundSavePath.AddString(Util::StringOp::ToCString(temp));
	m_cmbSoundSavePath.SetCurSel(0);
}
void CSettingDlg::UpdateDataok()
{
	OnButtonSettingOk();
}

void CSettingDlg::MoveControl()
{
	//
	int xBegin  ;
	int xWidth  ;
	int yBegin  ;
	int yHeight ;
	if (gLangItem == Data::lChinese)
	{
		//自动留言
		xBegin  = 85;
		yBegin  = 126*VHEIGHT ;
		xWidth  = 185;
		yHeight = 142*VHEIGHT - yBegin;
		m_chbEnableAutoLeaveWord.MoveWindow(xBegin,yBegin,xWidth,yHeight);

		//铃声次数
		xBegin  = 270 ;
		yBegin  = 126*VHEIGHT ;
		xWidth  = 76;
		yHeight = 142*VHEIGHT - yBegin;
		m_cmbRingTimes.MoveWindow(xBegin,yBegin,xWidth,yHeight);

		//是否外放
		xBegin  = 348 ;
		yBegin  = 126*VHEIGHT ;
		xWidth  = 67  ;
		yHeight = 142*VHEIGHT - yBegin;
		m_IsSpeakerOut.MoveWindow(xBegin,yBegin,xWidth,yHeight);

	}
	else if (gLangItem == Data::lEnglish)
	{
		//自动留言
		xBegin  = 85;
		yBegin  = 126*VHEIGHT ;
		xWidth  = 80;
		yHeight = 142*VHEIGHT - yBegin;
		m_chbEnableAutoLeaveWord.MoveWindow(xBegin,yBegin,xWidth,yHeight);
		
		//铃声次数
		xBegin = 180 ;
		yBegin = 126*VHEIGHT ;
		xWidth = 76  ;
		yHeight= 144*VHEIGHT-yBegin ; 
		m_cmbRingTimes.MoveWindow(xBegin,yBegin,xWidth,yHeight);
		
		//是否外放
		xBegin  = 270 ;
		yBegin  = 126*VHEIGHT ;
		xWidth  = 100  ;
		yHeight = 142*VHEIGHT - yBegin;
		m_IsSpeakerOut.MoveWindow(xBegin,yBegin,xWidth,yHeight);
	}

}

void CSettingDlg::ChangeTips()
{
	if (gLangItem == Data::lEnglish)
	{
		//
		std::string tips = "\\Hive\\my_tip\\tip4.wav";
		m_pSetting->defaultTipFilename(tips);
		m_pSetting->Update();
	}
	else
	{
		std::string tips = "\\Hive\\my_tip\\tip9.wav";
		m_pSetting->defaultTipFilename(tips);
		m_pSetting->Update();
	}

}

void CSettingDlg::ChangeReg()
{	
	DWORD value ;
	TCHAR szTemp[256] = {0};
	_tcscpy (szTemp, TEXT("nls\\"));
	_tcscat(szTemp,TEXT("overrides\\"));
	
	if (gLangItem == Data::lEnglish)
	{
		value = 0x409;
	}
	else
	{
		value = 0x804;
	}

	HKEY hKey = NULL;
	LONG hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, szTemp, 0, 0, &hKey);
	if ( ERROR_SUCCESS == hRes)
	{	
		DWORD dwDataSize = sizeof(DWORD);
		hRes = RegSetValueEx(hKey, TEXT("LCID"),\
			NULL, REG_DWORD, (LPBYTE)&value, dwDataSize);	
		RegFlushKey(hKey);
	}

	//修改MUI
	memset(szTemp,0,256);
	_tcscpy (szTemp, TEXT("MUI\\"));
	
	hKey = NULL;
	hRes = RegOpenKeyEx (HKEY_CURRENT_USER, szTemp, 0, 0, &hKey);
	if ( ERROR_SUCCESS == hRes)
	{	
		DWORD dwDataSize = sizeof(DWORD);
		hRes = RegSetValueEx(hKey, TEXT("CurLang"),\
			NULL, REG_DWORD, (LPBYTE)&value, dwDataSize);
		
		RegFlushKey(hKey);
		
	}
}

void CSettingDlg::OnButtonSettingOk() 
{
	// TODO: Add your control notification handler code here
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	SYSTEMTIME curtime, curtime1, curtime2;
	m_dtDate.GetTime(&curtime1);
	m_dtTime.GetTime(&curtime2);
	
	if(!(m_curtime.wYear == curtime1.wYear && m_curtime.wMonth == curtime1.wMonth &&m_curtime.wDay == curtime1.wDay &&\
		m_curtime.wHour == curtime2.wHour && m_curtime.wMinute == curtime2.wMinute &&m_curtime.wSecond == curtime2.wSecond))
	{
		memcpy(&curtime, &curtime1, sizeof(SYSTEMTIME));
		
		curtime.wHour = curtime2.wHour;
		
		curtime.wMinute = curtime2.wMinute;
		
		curtime.wSecond = curtime2.wSecond;
		
		SetLocalTime(&curtime);
		
		memcpy(&m_curtime, &curtime, sizeof(SYSTEMTIME));
		//刷新记事
		::PostMessage(main->m_pMainDlg->GetSafeHwnd(), WM_NOTENOTIFY, 0, 0);
	}

	CString str;

	m_edtLocalAreaNumber.GetWindowText(str);
	m_pSetting->thisTelephoneNumber(Data::TelephoneNumber(Util::StringOp::FromCString(str)));
	m_edtLocalNumber.GetWindowText(str);
	m_pSetting->LocalNumber(Util::StringOp::FromCString(str));

	if (m_chbEnableOutLine.GetCheck() == BST_CHECKED)
	{
		m_pSetting->isAutoAppendOutlinePrefix(true);
	}
	else
	{
		m_pSetting->isAutoAppendOutlinePrefix(false);
	}

	m_edtOutLine.GetWindowText(str);

	m_pSetting->outlinePrefix(Util::StringOp::FromCString(str));

	if (m_chbEnableIPDial.GetCheck() == BST_CHECKED)
	{
		m_pSetting->isAppendIpPrefix(true);
	}
	else
	{
		m_pSetting->isAppendIpPrefix(false);
	}

	m_edtIPDial.GetWindowText(str);

	m_pSetting->ipPrefix(Util::StringOp::FromCString(str));

	if (m_chbEnableRing.GetCheck() == BST_CHECKED)
	{
		m_pSetting->isUseSpecRing(true);

        if (!m_pSetting->isFirewall())
        {
			main->phone_->OpenTelRing(FALSE);
        }

	}else
	{
		m_pSetting->isUseSpecRing(false);
    
		if (!m_pSetting->isFirewall())
		{
			main->phone_->OpenTelRing(TRUE);
		}	
	}

	if (m_cmbRing.GetCurSel() != CB_ERR)
	{
		m_cmbRing.GetWindowText(str);

		m_pSetting->defaultRingFilename(Util::StringOp::FromCString(csFlashRingPath + str));
	}

	int v = m_cmbVolume.GetCurSel();

	m_pSetting->soundVolume(v);

	DWORD volume[]={0x10001000,0x44004400,0x88008800,0xcc00cc00,0xff00ff00};
	int volume2[] = {-2400, -1800, -1200, -600, -1};
	waveOutSetVolume(NULL, volume[v]);
	main->phone_->SetSoundRingVal(volume2[v]);//SetSoundValueRing

	if (m_chbEnableAutoLeaveWord.GetCheck() == BST_CHECKED)
	{
		m_pSetting->isAutoReply(true);
	}
	else
	{
		m_pSetting->isAutoReply(false);
	}

	CString s;
	if (m_cmbTip.GetCurSel() != CB_ERR)
	{
		m_cmbTip.GetWindowText(s);
		
		//add by qi 20110803
		if ((s.Right(4).CompareNoCase(_T(".mp3")) == 0)
			|| (s.Right(4).CompareNoCase(_T(".wav")) == 0))
		{  
			s = csHiveTipPath + s;
		}
		else 
		{
			int index = m_cmbTip.GetItemData(m_cmbTip.GetCurSel());
						
			s = m_vTipsDir[index] + s;
					
		}
		//
		
//		change by qi 20110803
// 		if (index == 0)
// 		{
// 			s = csFlashRecordPath + s;
// 		}
// 		else if (index == 1)
// 		{
// 			s = csStorageCardRecordPath + s;
// 		}
// 		else
// 		{
// 			s = csHiveTipPath + s;
// 		}

		m_pSetting->defaultTipFilename(Util::StringOp::FromCString(s));	
	}

	m_pSetting->autoReplyRingCount(m_cmbRingTimes.GetCurSel() + 3);

   //是否启用录音外放,linkMode作为否启用密保的开关来使用了
	if (m_IsSpeakerOut.GetCheck() == BST_CHECKED)
	{
		m_pSetting->linkMode(Data::lmDirect);
 	}
 	else
 	{
 		m_pSetting->linkMode(Data::lmDial);
 	}

	if (m_chbEnabePlayProtect.GetCheck() == BST_CHECKED)
	{
		m_pSetting->isPlayProtect(true);

		main->m_pSoundDlg->m_bPopPasswordDlg=true;

	}else{

		m_pSetting->isPlayProtect(false);

	   main->m_pSoundDlg->m_bPopPasswordDlg=false;
	}
	if (m_chbEnabeRecord.GetCheck() == BST_CHECKED)
	{
		m_pSetting->isMustRecord(true);
	}
	else
	{
		m_pSetting->isMustRecord(false);
	}
	if (m_chbEnabeDeleteProtect.GetCheck() == BST_CHECKED)
	{
		m_pSetting->isDeleteProtect(true);
	}
	else
	{
		m_pSetting->isDeleteProtect(false);
	}

	if (m_chbSetAdminPassword.GetCheck() == BST_CHECKED)
	{
		m_pSetting->isAdmin(true);
		m_btnLogin.ShowWindow(TRUE);
	}
	else
	{
		m_pSetting->isAdmin(false);
		m_bLogin = false;
		m_btnLogin.ShowWindow(FALSE);
		m_btnLogin.SetWindowText(Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_SETTING_LOGIN))));
	}

	//m_pSetting->maxSoundUseSize((m_cmbRecordLimit.GetCurSel() + 1) * 30);
	int l = m_pSetting->maxSoundUseSize();
	if(m_cmbRecordLimit.GetCurSel() == 1)
	{
		m_pSetting->maxSoundUseSize(120);
	}
	else if(m_cmbRecordLimit.GetCurSel() == 2)
	{
		m_pSetting->maxSoundUseSize(300);
	}
	else if(m_cmbRecordLimit.GetCurSel() == 3)
	{
		m_pSetting->maxSoundUseSize(600*5);
	}
	else
	{
		m_pSetting->maxSoundUseSize(60);
	}

	if (m_chbEnnabeScreenProtect.GetCheck() == BST_CHECKED)
	{
		m_pSetting->isUseScreenSaver(true);
	}
	else
	{
		m_pSetting->isUseScreenSaver(false);
	}

	int nSecond[5] = {15, 30, 60, 5*60, 10*60};
    int nSel = m_cmbWaitTime.GetCurSel();
	if (nSel == LB_ERR)
		nSel = 0;
	CTimeSpan time = CTimeSpan(0, 0, nSecond[nSel]/60, nSecond[nSel]%60);
	m_pSetting->screenSaverDuration(time);

	nSel = m_cmbBacklight.GetCurSel();
	if (nSel == LB_ERR)
		nSel = 0;
	nSel = (nSel) * 15;
	time = CTimeSpan(nSel);
	m_pSetting->backlightDuration(time);
	if (m_chbEnablePassword.GetCheck() == BST_CHECKED)
	{
		m_pSetting->isUseScreenSaverPassword(true);
			
	}
	else
	{
		m_pSetting->isUseScreenSaverPassword(false);
		
	}
	m_pSetting->soundPath(ssFlashRecordPath);
	m_pSetting->Update();

	if ((main->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsHangOff))
	{
		m_pOggCodec->StopDecode(true);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->StopRing(true);
	}
	
	//提示操作已成功
	main->m_pDeleteTipDlg->SetPasswordModel(false);
	main->m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
	std::string str1 = Data::LanguageResource::Get(Data::RI_DELETETIP_OPSUCCESS);//是否将Hive和FlashDrv下的文件删除
	main->m_pDeleteTipDlg->SetDelTip(str1.c_str());
	main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);

	ShowConfigItems();
	main->m_pMainDlg->UpdateDiskInfo();
 	main->SetScreenSaveTimer();
}

void CSettingDlg::UpdateTips(void)
{
	m_cmbTip.ResetContent();

	WIN32_FIND_DATA FindFileData;//查找文件时要使用的数据结构
	HANDLE hFind = INVALID_HANDLE_VALUE;	//定义查找句柄
	
	hFind = FindFirstFile(csHiveTipPath + _T("*.*"), &FindFileData);//使用FindFirstFile函数来开始文件查找
	
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		CString filename;
		filename =  FindFileData.cFileName;
		if ((filename.Right(4).CompareNoCase(_T(".mp3")) == 0)
			|| (filename.Right(4).CompareNoCase(_T(".wav")) == 0))
		{
			m_cmbTip.InsertString(-1,filename);
		}
		
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			filename =  FindFileData.cFileName;
			if ((filename.Right(4).CompareNoCase(_T(".mp3")) == 0)
				|| (filename.Right(4).CompareNoCase(_T(".wav")) == 0))
			{
				m_cmbTip.InsertString(-1,filename);
			}
		}
		
		FindClose(hFind);
	}
	for (int i = 0; i < m_cmbTip.GetCount(); i++)
	{
		m_cmbTip.SetItemData(i, 2);
	}
	
	//添加设置的提示语
	if (m_vTipsDir.size())
	{
		m_vTipsDir.clear();
	}


	if (Data::SoundSegment::GetDataCount("type = 2 AND isTip = 1") > 0)
	{
		std::vector<boost::shared_ptr<Data::SoundSegment> > sounds = Data::SoundSegment::GetFromDatabase("type = 2 AND isTip = 1");

		for (int i = 0; i < sounds.size(); ++i)
		{
			//add by qi 20110803
			CString tipsDir;//提示语的目录
			
			int pos = sounds[i]->filename().rfind('\\');
			m_cmbTip.InsertString(-1,Util::StringOp::ToCString(sounds[i]->filename().substr(pos + 1)));
			
			//add by qi 20110803
			tipsDir = Util::StringOp::ToCString(sounds[i]->filename().substr(0,pos+1));
			m_vTipsDir.push_back(tipsDir);
			m_cmbTip.SetItemData(m_cmbTip.GetCount()-1,i);

		}
	}
    
	int pos = m_pSetting->defaultTipFilename().rfind('\\');

	CString tipName = Util::StringOp::ToCString(m_pSetting->defaultTipFilename().substr(pos + 1));

	CString txt;

	for (i = 0; i < m_cmbTip.GetCount(); i++)
	{
		m_cmbTip.GetLBText(i, txt);

		if (tipName.CompareNoCase(txt) == 0)
		{
			m_cmbTip.SetCurSel(i);

			break;
		}
		else
		{
			m_cmbTip.SetCurSel(0);
		}
	}
}

void CSettingDlg::OnButtonSettingLogin()
{	
	//管理
	if (m_bLogin)
	{
		m_bLogin = false;
		IniCtrlData();
		ShowConfigItems();
		m_btnLogin.SetWindowText(Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_SETTING_LOGIN))));

	}else{   
		
		//add by qi 20111104
		//密码为空直接登入
		std::string strTemp = m_pSetting->adminPassword();
		if (!strTemp.empty())
		{
			m_pPasswordDlg->SetType(CHECK_PASSWORD);
			m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
			m_pPasswordDlg->SetHWnd(this->m_hWnd);
			m_pPasswordDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_bLogin = true;
			m_btnLogin.SetWindowText(Util::StringOp::ToCString(\
			(Data::LanguageResource::Get(Data::RI_SETTING_LOGOUT))));
			ShowConfigItems();	
		}
	}

}
void CSettingDlg::OnSetPlayPassword()
{
	m_iPasswordType = 0;
	m_pPasswordDlg->SetType(SETTING_PASSWORD,2);
	std::string strTemp = m_pSetting->playRecordPassword();
	m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
	m_pPasswordDlg->SetHWnd(this->m_hWnd);
	m_pPasswordDlg->ShowWindow(SW_SHOW);	
}
void CSettingDlg::OnSetAdminPassword()
{	
	//管理员密码
	m_iPasswordType = 1;
	m_pPasswordDlg->SetType(SETTING_PASSWORD,1);
	std::string strTemp = m_pSetting->adminPassword();
	m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
	m_pPasswordDlg->SetHWnd(this->m_hWnd);
	m_pPasswordDlg->ShowWindow(SW_SHOW);	
}

void CSettingDlg::OnButtonSettingCancel() 
{
	// TODO: Add your control notification handler code here
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
	if (m_bLogin)
	{
		OnButtonSettingLogin();

	}else{

		IniCtrlData();
		ShowConfigItems();
	}
	m_bPlayPause=false;
}

void CSettingDlg::OnButtonSettingDefault() 
{
	// TODO: Add your control notification handler code here
	m_iDeleteType = 5;
	if (m_pSetting->isAdmin() && !m_bLogin)
	{		
		std::string pw = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->\
			m_pSettingDlg->m_pSetting->adminPassword();
		if (!pw.empty())
		{	
			//add by qi 20111104
			//密码为空不用输入密码
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPasswordModel(true);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
		}
	}

	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
	std::string strTemp = Data::LanguageResource::Get(Data::RI_SETTING_BACKDEFAULT);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp));
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DEFAULT);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(strTemp);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
}

void CSettingDlg::OnButtonSettingPlayRing()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if ((main->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOff))     
		 return;
	m_iSBell=true;  
	std::string strTemp;
	CString str;
	if(m_bPlaying)
    {
		m_pOggCodec->StopDecode(m_bPlaying);
		main->phone_->StopRing(m_bPlaying);
		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LISTEN);
		str = strTemp.c_str();
		m_btnPlayRing.SetWindowText(str);
		
		m_bPlaying=false;
		
	}
	else
	{
		m_pOggCodec->StopDecode(!m_bPlaying);
		main->phone_->StopRing(!m_bPlaying);
		
		CString s; 
		m_cmbRing.GetWindowText(s);
		if (s.IsEmpty())
			return;
		s = csFlashRingPath + s;
		
		CFileStatus status;
		if (CFile::GetStatus(LPCTSTR(s),status))
		{
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_STOP);
			str = strTemp.c_str();
			m_btnPlayRing.SetWindowText(str);
			
			//wzx 20110110  //试听时将音量放小一级,试听完以后再恢复
			int v = m_pSetting->soundVolume();
			DWORD volume[]={0x05000500,0x10001000,0x44004400,0x88008800,0xcc00cc00};
			int volume2[] = {-2400, -1800, -1200, -600, -1};
			waveOutSetVolume(NULL, volume[v]);
			main->phone_->SetSoundRingVal(volume2[v]);

			main->phone_->SetMsgWnd(this);
			main->phone_->StartRing((LPTSTR)(LPCTSTR)s, 1);
			m_bPlaying=true;
		}
		else   //提示文件不存在
		{
			main->m_pDeleteTipDlg->SetPasswordModel(false);
			main->m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
			std::string str1 = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
			main->m_pDeleteTipDlg->SetDelTip(str1.c_str());
			main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		}
	}
}

void CSettingDlg::OnButtonSettingListen()//试听提示语
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if ((main->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOff))
		return;
	std::string strTemp;
	CString str;
	if (m_bPlayPause)
	{
		m_pOggCodec->StopDecode(m_bPlayPause);
		main->phone_->StopRing(m_bPlayPause);
		
		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LISTEN);
		str = strTemp.c_str();
		m_btnPlayLeaveWordTip.SetWindowText(str);
		
		m_bPlayPause = false;
	}
    else
    {	
		m_pOggCodec->StopDecode(!m_bPlayPause);
		main->phone_->StopRing(!m_bPlayPause);
		
		CString s;
		m_cmbTip.GetWindowText(s);
		if (s.IsEmpty() || (m_cmbTip.GetCurSel() == LB_ERR))
			return;
		
		if ((s.Right(4).CompareNoCase(_T(".mp3")) == 0)
			|| (s.Right(4).CompareNoCase(_T(".wav")) == 0))
		{  
			s = csHiveTipPath + s;
		}
		else
		{
			int index = m_cmbTip.GetItemData(m_cmbTip.GetCurSel());
			
			//add by qi 20110803
			s = m_vTipsDir[index] + s;
			//

//			change by qi 20110803			
// 			if (index == 0)
// 			{
// 				s = csFlashRecordPath + s;
// 			}
// 			else if (index == 1)
// 			{
// 				s = csStorageCardRecordPath + s;
// 			}

			std::string filename = Util::StringOp::FromCString(s);
			std::vector<boost::shared_ptr<Data::SoundSegment> > m_vCurrentResult=Data::SoundSegment::GetFromDatabase("type = 2 AND filename = '" + filename + "'");
			if (!m_vCurrentResult.empty())
			{
                m_iTotalSeconds = m_vCurrentResult[0]->duration();
			}
			
		}
		
		CFileStatus   status;
		if (CFile::GetStatus(LPCTSTR(s),status))
		{
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_STOP);
			str = strTemp.c_str();
			m_btnPlayLeaveWordTip.SetWindowText(str);   //当文件能播放时才设置按钮文字
			m_bPlayPause=true;
			//wzx 20110110  //试听时将音量放小一级,试听完以后再恢复
			int v = m_pSetting->soundVolume();
			DWORD volume[]={0x05000500,0x10001000,0x44004400,0x88008800,0xcc00cc00};
			int volume2[] = {-2400, -1800, -1200, -600, -1};
			waveOutSetVolume(NULL, volume[v]);
			main->phone_->SetSoundRingVal(volume2[v]);

			if((s.Right(4).CompareNoCase(_T(".spx")) == 0)||(s.Right(4).CompareNoCase(_T(".spd")) == 0))
			{   
				if (m_pOggCodec->StartDecode(Util::StringOp::FromCString(s)))
				{ 
					KillTimer(100);
					SetTimer(100, 1000, NULL);  
				}
			}
			else
			{   
				main->phone_->SetMsgWnd(this);
				main->phone_->StartRing((LPTSTR)(LPCTSTR)s, 1);
			}
		}
		else   //提示文件不存在
		{
			main->m_pDeleteTipDlg->SetPasswordModel(false);
			main->m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
			std::string str1 = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
			main->m_pDeleteTipDlg->SetDelTip(str1.c_str());
			main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		}
	}
}

typedef BOOL (WINAPI *_TouchCalibrate)();

//触摸屏校正
void CSettingDlg::OnAdjustTouchPanel()
{
	HINSTANCE  hinstance = LoadLibrary(_T("coredll.dll"));
    if  (hinstance == NULL)  
    {  
        AfxMessageBox (L"instance == NULL"); 
        return; 
    }  
    _TouchCalibrate TouchCalibrate = NULL; 
    TouchCalibrate = (_TouchCalibrate)GetProcAddress(hinstance , L"TouchCalibrate"); 
    if (TouchCalibrate == NULL)  
    {  
        AfxMessageBox (L"TouchCalibrate == NULL"); 
        return; 
    }  
    TouchCalibrate (); 
    FreeLibrary(hinstance );
	TCHAR szTemp[256] = {0};
	_tcscpy (szTemp, TEXT("HARDWARE\\DEVICEMAP\\TOUCH"));
	HKEY hKey = NULL;
	LONG hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, szTemp, 0, 0, &hKey);
	if (ERROR_SUCCESS == hRes)
	{
	}
	LONG l = RegFlushKey(HKEY_LOCAL_MACHINE);
	RegCloseKey(hKey);
	
	//写注册表  触摸屏校正
}

void CSettingDlg::OnSetSaveScreenPassWordOK(WPARAM w, LPARAM l)
{
	m_pSetting->screenSaverPassword((char *)w);
}

void CSettingDlg::OnSettingSaveScreenPassword()
{
	m_iPasswordType = 2;
	m_pPasswordDlg->SetType(SETTING_PASSWORD,3);
	std::string strTemp = m_pSetting->screenSaverPassword();
	m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
	m_pPasswordDlg->SetHWnd(this->m_hWnd);
	m_pPasswordDlg->ShowWindow(SW_SHOW);	
}

void CSettingDlg::OnButtonSettingDefineFastDial()
{
	m_pFastDialsDlg->SetFastDialParam(m_pSetting);

	m_pFastDialsDlg->ShowWindow(TRUE);	
}

void CSettingDlg::ShowConfigItems(void)
{
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	//tel
	m_sticLocalAreaNumber.ShowWindow(FALSE);
	m_edtLocalAreaNumber.ShowWindow(FALSE);
	m_sticLocalNumber.ShowWindow(FALSE);
	m_edtLocalNumber.ShowWindow(FALSE);
	m_chbEnableOutLine.ShowWindow(FALSE);
	m_edtOutLine.ShowWindow(FALSE);
	m_chbEnableIPDial.ShowWindow(FALSE);
	m_edtIPDial.ShowWindow(FALSE);
	m_chbEnableRing.ShowWindow(FALSE);
	m_sticDefaultRing.ShowWindow(FALSE);
	m_cmbVolume.ShowWindow(FALSE);
	m_cmbTip.ShowWindow(FALSE);
	m_cmbRing.ShowWindow(FALSE);

	m_chbEnableAutoLeaveWord.ShowWindow(FALSE);
	m_sticRingTimes.ShowWindow(FALSE);
	m_cmbRingTimes.ShowWindow(FALSE);
	m_IsSpeakerOut.ShowWindow(FALSE);
	m_sticVolume.ShowWindow(FALSE);
	m_btnPlayRing.ShowWindow(FALSE);
	m_btnPlayLeaveWordTip.ShowWindow(FALSE);
	m_btnDefineSpeedButton.ShowWindow(FALSE);
	m_sticDefineSpeedButton.ShowWindow(FALSE);

	m_btnSetPlayPassword.ShowWindow(FALSE);
	m_chbEnabeRecord.ShowWindow(FALSE);
	m_chbEnabePlayProtect.ShowWindow(FALSE);
	m_chbEnabeDeleteProtect.ShowWindow(FALSE);
	m_sticRecordLimit.ShowWindow(FALSE);
	m_cmbRecordLimit.ShowWindow(FALSE);
	//browser
	
	//systom
	m_chbSetAdminPassword.ShowWindow(FALSE);
	m_btnSetAdminPassword.ShowWindow(FALSE);
	m_sticBacklight.ShowWindow(FALSE);
	m_cmbBacklight.ShowWindow(FALSE);

	m_chbEnnabeScreenProtect.ShowWindow(FALSE);
	m_sticWaitTime.ShowWindow(FALSE);
	m_cmbWaitTime.ShowWindow(FALSE);
	m_chbEnablePassword.ShowWindow(FALSE);
	m_btnSetPassword.ShowWindow(FALSE);
	m_sticSoundSavePath.ShowWindow(FALSE);
	m_cmbSoundSavePath.ShowWindow(FALSE);
	m_sticTouchAdjust.ShowWindow(FALSE);
	m_btnTouchAdjust.ShowWindow(FALSE);
	m_stcTime.ShowWindow(FALSE);
	m_dtTime.ShowWindow(FALSE);
	m_dtDate.ShowWindow(FALSE);
	//information
	m_sticSystomTitle.ShowWindow(FALSE);
	m_sticSystomSerialNo1.ShowWindow(FALSE);
	m_sticSystomSerialNo.ShowWindow(FALSE);
	m_sticSoftwareVersion1.ShowWindow(FALSE);
	m_sticHardwareVersion1.ShowWindow(FALSE);
	m_sticMemorySize1.ShowWindow(FALSE);
	m_sticStorageSize1.ShowWindow(FALSE);
	m_sticContactSize1.ShowWindow(FALSE);
	m_sticContactInfoSize1.ShowWindow(FALSE);
	m_sticSoundSize1.ShowWindow(FALSE);
	m_versionChange.ShowWindow(FALSE);
	m_sticInformationSize1.ShowWindow(FALSE);
	m_sticVersionRight1.ShowWindow(FALSE);
	m_sticSoftwareVersion.ShowWindow(FALSE);
	m_editVersion.ShowWindow(FALSE);
	m_sticHardwareVersion.ShowWindow(FALSE);
	m_sticMemorySize.ShowWindow(FALSE);
	m_sticStorageSize.ShowWindow(FALSE);
	m_sticContactSize.ShowWindow(FALSE);
	m_sticContactInfoSize.ShowWindow(FALSE);
	m_sticSoundSize.ShowWindow(FALSE);
	m_sticInformationSize.ShowWindow(FALSE);
	m_sticVersionRight.ShowWindow(FALSE);
	m_btnClearContact.ShowWindow(FALSE);
	m_btnClearContactInfo.ShowWindow(FALSE);
	m_btnClearSound.ShowWindow(FALSE);
	m_btnClearInformation.ShowWindow(FALSE);
	m_btnLangChange.ShowWindow(FALSE);
	m_btnFormatSD.ShowWindow(FALSE);
	m_stcBackUP.ShowWindow(FALSE);
//	m_stcReStore.ShowWindow(FALSE);
	m_btnBackUP.ShowWindow(FALSE);
	m_btnReStore.ShowWindow(FALSE);
	m_stcTitle.ShowWindow(FALSE);
	m_btnOut.ShowWindow(FALSE);
	//RingInfo
	m_sticRingInfo.ShowWindow(FALSE);
	m_sticRingSize.ShowWindow(FALSE);
	m_sticRingList.ShowWindow(FALSE);
	m_btnClearRing.ShowWindow(FALSE);
	m_btnDeleteRing.ShowWindow(FALSE);
	m_cmbDelRingList.ShowWindow(FALSE);

	//add by yy 20111103
	m_btnSetOneKeyClear.ShowWindow(FALSE);
	m_sticUconnectNum.ShowWindow(FALSE);
	m_sticUconnectNum1.ShowWindow(FALSE);
	
	
	if (m_pSetting->isAdmin() && !m_bLogin )
		return;

	
	int type = m_lsType.GetNextSelectedItem(pos);

	switch (type)
	{
	case 0:
		m_sticLocalAreaNumber.ShowWindow(TRUE);
		m_edtLocalAreaNumber.ShowWindow(TRUE);
		m_sticLocalNumber.ShowWindow(TRUE);
		m_edtLocalNumber.ShowWindow(TRUE);
		m_chbEnableOutLine.ShowWindow(TRUE);
		m_edtOutLine.ShowWindow(TRUE);
		m_chbEnableIPDial.ShowWindow(TRUE);
		m_edtIPDial.ShowWindow(TRUE);
		m_chbEnableRing.ShowWindow(TRUE);
		m_sticDefaultRing.ShowWindow(TRUE);
		m_cmbVolume.ShowWindow(TRUE);
		m_cmbTip.ShowWindow(TRUE);
		UpdateTips();
		m_cmbRing.ShowWindow(TRUE);	
		m_chbEnableAutoLeaveWord.ShowWindow(TRUE);
		m_IsSpeakerOut.ShowWindow(TRUE);
		m_sticRingTimes.ShowWindow(TRUE);
		m_cmbRingTimes.ShowWindow(TRUE);
		m_sticVolume.ShowWindow(TRUE);
 		m_btnPlayRing.ShowWindow(TRUE);
 		m_btnPlayLeaveWordTip.ShowWindow(TRUE);
		m_btnDefineSpeedButton.ShowWindow(TRUE);
		m_sticDefineSpeedButton.ShowWindow(TRUE);
		break;
	case 1:
		m_btnSetPlayPassword.ShowWindow(TRUE);
		m_chbEnabeRecord.ShowWindow(TRUE);
		m_chbEnabePlayProtect.ShowWindow(TRUE);
		m_chbEnabeDeleteProtect.ShowWindow(TRUE);
		m_sticRecordLimit.ShowWindow(TRUE);
		m_cmbRecordLimit.ShowWindow(TRUE);
		
		break;
	case 2:
		m_chbSetAdminPassword.ShowWindow(TRUE);
		m_btnSetAdminPassword.ShowWindow(TRUE);//管理密码
		m_sticBacklight.ShowWindow(TRUE);
		m_cmbBacklight.ShowWindow(TRUE);
		m_chbEnnabeScreenProtect.ShowWindow(TRUE);
		m_sticWaitTime.ShowWindow(TRUE);
		m_cmbWaitTime.ShowWindow(TRUE);
		m_chbEnablePassword.ShowWindow(TRUE);
		m_btnSetPassword.ShowWindow(TRUE);
		m_sticSoundSavePath.ShowWindow(TRUE);
		m_cmbSoundSavePath.ShowWindow(TRUE);
		{
			m_cmbSoundSavePath.ResetContent();
			std::string temp = Data::LanguageResource::Get(Data::RI_SETTING_LOCAL);
			m_cmbSoundSavePath.AddString(Util::StringOp::ToCString(temp));
			
			BOOL sd = DetectDIR(_T("\\StorageCard"));
			if (sd)
			{
				temp = Data::LanguageResource::Get(Data::RI_SETTING_SDCARD);
				m_cmbSoundSavePath.AddString(Util::StringOp::ToCString(temp));
			}
			
			std::string soundPath = m_pSetting->soundPath();
			if (soundPath.substr(0, 12) == "\\StorageCard" && sd)
			{
				m_cmbSoundSavePath.SetCurSel(1);
			}
			else
			{
				m_cmbSoundSavePath.SetCurSel(0);
			}
		}

		m_sticTouchAdjust.ShowWindow(TRUE);

		m_btnTouchAdjust.ShowWindow(TRUE);

		m_stcTime.ShowWindow(TRUE);

		m_dtTime.ShowWindow(TRUE);

		m_dtDate.ShowWindow(TRUE);

		break;
	case 3:
//		m_sticSystomTitle.ShowWindow(TRUE);
		m_sticSystomSerialNo1.ShowWindow(TRUE);
		m_sticSystomSerialNo.ShowWindow(TRUE);
		m_sticSoftwareVersion1.ShowWindow(TRUE);
	//	m_sticHardwareVersion1.ShowWindow(TRUE);
	//	m_sticHardwareVersion.ShowWindow(TRUE);
		m_sticMemorySize1.ShowWindow(TRUE);
		m_sticContactSize1.ShowWindow(TRUE);
		m_sticContactInfoSize1.ShowWindow(TRUE);
		m_sticSoundSize1.ShowWindow(TRUE);
		m_versionChange.ShowWindow(TRUE);
		m_sticVersionRight1.ShowWindow(TRUE);	
		m_sticSoftwareVersion.ShowWindow(TRUE);
		m_sticUconnectNum.ShowWindow(TRUE);
		m_sticUconnectNum1.ShowWindow(TRUE);
		m_btnSetOneKeyClear.ShowWindow(TRUE);

		{
			std::string temp = Data::LanguageResource::Get(Data::RI_SETTING_AVAILABLESIZE);//?
			CString as = Util::StringOp::ToCString(temp);
			CString text;
			ULARGE_INTEGER freeBytes;
			ULARGE_INTEGER totalBytes;
			ULARGE_INTEGER freeBytesSD;
			ULARGE_INTEGER totalBytesSD;
			DWORD total = 0;

			GetDiskFreeSpaceEx(_T("\\Flashdrv"), &freeBytes, &totalBytes, NULL);
			BOOL sd = DetectDIR(L"\\StorageCard");
			int error = 0;
			if (sd)
			{
				error = GetDiskFreeSpaceEx(_T("\\StorageCard"), &freeBytesSD, &totalBytesSD, NULL);
				if(error != 0)
					total = (totalBytes.QuadPart + totalBytesSD.QuadPart) / (1024*1024);
				else
					total = (totalBytes.QuadPart) / (1024*1024);
			}
			else
			{
				total = (totalBytes.QuadPart) / (1024*1024);
			}

			LONGLONG remain = freeBytes.QuadPart - PreFreeSize;
			if(remain < 0)
			{
				if(sd && error != 0)
				{
					remain = freeBytesSD.QuadPart - sdPreFreeSize;
				}
			}
			else
			{
				if(sd && error != 0)
				{
					LONGLONG fr = freeBytesSD.QuadPart - sdPreFreeSize;
					if(fr > 0)
					{
						remain += fr;
					}
				}
			}
			
			if (remain < 0)
			{
				remain = 0;
			}
			
			if(remain > 0 && remain < 1024*1024)
			{
				text.Format(_T("%dM (0.1M"), total); //容量不足一M时显示0.1M
			}
			else
			{
				text.Format(_T("%dM (%dM"), total, remain/(1024*1024));
			}
			text += as + ")";

			m_sticMemorySize.SetWindowText(text);
			m_sticMemorySize.ShowWindow(TRUE);

			//未接电话	  
			CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
			UINT nUnReadNum = main->m_pTelephoneDlg->GetUnconnectCount();
			text.Format(_T("%d"), nUnReadNum );
			m_sticUconnectNum1.SetWindowText(text);
			
		}
		
		m_sticContactSize.ShowWindow(TRUE);
		m_sticContactInfoSize.ShowWindow(TRUE);
		m_sticSoundSize.ShowWindow(TRUE);
		{
			CString text;
			text.Format(_T("%d"), g_iContactCount);
			m_sticContactSize.SetWindowText(text);

			text.Format(_T("%d"), g_iContactInfoCount);
			m_sticContactInfoSize.SetWindowText(text);

			text.Format(_T("%d"), g_iRecordCount);
			m_sticSoundSize.SetWindowText(text);
		}
		m_sticVersionRight.ShowWindow(TRUE);
		m_btnClearContact.ShowWindow(TRUE);
		m_btnClearContactInfo.ShowWindow(TRUE);
		m_btnClearSound.ShowWindow(TRUE);
		m_btnLangChange.ShowWindow(TRUE);
		//m_btnSystemInfo.ShowWindow(TRUE);
		break;
	case 4:
		{
			CString text;
			text.Format(_T("%d"), m_cmbDelRingList.GetCount());
			m_sticRingSize.SetWindowText(text);
			m_sticRingInfo.ShowWindow(TRUE);
			m_sticRingSize.ShowWindow(TRUE);
			m_sticRingList.ShowWindow(TRUE);
			m_btnClearRing.ShowWindow(TRUE);
			m_btnDeleteRing.ShowWindow(TRUE);
			m_cmbDelRingList.ShowWindow(TRUE);
		}
		break;
	}
}

void CSettingDlg::OnSettingClearContact()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (m_pSetting->isAdmin() && !m_bLogin)
	{	
		std::string pw = main->m_pSettingDlg->m_pSetting->adminPassword();
		if (!pw.empty())
		{	
			//add by qi 20111104
			//密码为空不用输入密码
			main->m_pDeleteTipDlg->SetPasswordModel(true);
			main->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
		}
	}

	if(g_iContactCount > 0)
	{
		main->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
		std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACT2);
		main->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp));
		main->m_pDeleteTipDlg->ShowWindow(TRUE);
		m_iDeleteType = 0;
	}
	else
	{
		std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_ALLCLEAR);
		main->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp));
		main->m_pDeleteTipDlg->ShowWindow(TRUE);
	}
}

void CSettingDlg::OnSettingClearContactInfo()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (m_pSetting->isAdmin() && !m_bLogin)
	{	
		std::string pw = main->m_pSettingDlg->m_pSetting->adminPassword();
		if (!pw.empty())
		{	
			//add by qi 20111104
			//密码为空不用输入密码
			main->m_pDeleteTipDlg->SetPasswordModel(true);
			main->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
		}
	}

	if(g_iContactInfoCount > 0)
	{
		main->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
		std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACTINFO2);
		main->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp));
		main->m_pDeleteTipDlg->ShowWindow(TRUE);
		m_iDeleteType = 1;
	}
	else
	{
		std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_ALLCLEAR);
		main->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp));
		main->m_pDeleteTipDlg->ShowWindow(TRUE);
	}
}

void CSettingDlg::OnSettingClearSound()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (m_pSetting->isAdmin() && !m_bLogin)
	{	
		std::string pw = main->m_pSettingDlg->m_pSetting->adminPassword();
		if (!pw.empty())
		{	
			//add by qi 20111104
			//密码为空不用输入密码
			main->m_pDeleteTipDlg->SetPasswordModel(true);
			main->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
		}

	}

	if(g_iRecordCount > 0)
	{
		main->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
		std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_SOUND2);
		main->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp));
		main->m_pDeleteTipDlg->ShowWindow(TRUE);
		m_iDeleteType = 2;
	}
	else
	{
		std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_ALLCLEAR);
		main->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp));
		main->m_pDeleteTipDlg->ShowWindow(TRUE);
	}
}

void CSettingDlg::OnSettingClearInfo()
{
//	m_iDeleteType = 3;  //wzx 20101225
	if (m_pSetting->isAdmin() && !m_bLogin)
	{	
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		std::string pw = main->m_pSettingDlg->m_pSetting->adminPassword();
		if (!pw.empty())
		{	
			//add by qi 20111104
			//密码为空不用输入密码
			main->m_pDeleteTipDlg->SetPasswordModel(true);
			main->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
		}

	}
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
	  std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp));
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
}

extern void DeleteDirectory(CString SrcDir, BOOL isShow = TRUE);
void CSettingDlg::OnSettingClearRing()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (m_pSetting->isAdmin() && !m_bLogin)
	{
		std::string pw = main->m_pSettingDlg->m_pSetting->adminPassword();
		if (!pw.empty())
		{	
			//add by qi 20111104
			//密码为空不用输入密码
			main->m_pDeleteTipDlg->SetPasswordModel(true);
			main->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
		}	
	}

	CString size(L"");
	m_sticRingSize.GetWindowText(size);
	if(Util::StringOp::ToInt(size) > 0)
	{
		main->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
		std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CLEARRING);
		main->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp));
		main->m_pDeleteTipDlg->ShowWindow(TRUE);
		m_iDeleteType = 3;  //wzx 20101225
	}
	else
	{
		std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_ALLCLEAR);
		main->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp));
		main->m_pDeleteTipDlg->ShowWindow(TRUE);
	}
}

void CSettingDlg::OnSettingDeleteRing()
{ 
	CString name;
	m_cmbDelRingList.GetWindowText(name);
	name = csFlashRingPath + name;
	CFileStatus status;
	if(CFile::GetStatus(LPCTSTR(name),status))
	{
		DeleteFile(name);
	}

	int index = m_cmbDelRingList.GetCurSel();
	if(index >= 0)
	{
		m_cmbDelRingList.DeleteString(index);
		m_cmbRing.DeleteString(index);
	}

	if(m_cmbDelRingList.GetCount() > 0)
	{
		m_cmbDelRingList.SetCurSel(0);
		m_cmbDelRingList.Invalidate();
		m_cmbRing.SetCurSel(0);
		m_cmbRing.Invalidate();
	}
	else
	{
		m_cmbDelRingList.ResetContent();
		m_cmbDelRingList.SetWindowText(L"");
		m_cmbRing.ResetContent();
		m_cmbRing.SetWindowText(L"");
	}
	CString count;
	count.Format(L"%d", m_cmbDelRingList.GetCount());
	m_sticRingSize.SetWindowText(count);
}

void CSettingDlg::OnSettingUSBModal()
{
	extern BOOL m_bInsertUSB;
	if(m_bInsertUSB)
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
		std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_USBMODELTIP);
		main->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp));
		main->m_pDeleteTipDlg->ShowWindow(TRUE);
		return;
	}

	m_bUSBModal = TRUE;

	extern void USBModal();			//lxz 20101228
	USBModal();

	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (m_pSetting->isAdmin() && !m_bLogin)
	{
		std::string pw = main->m_pSettingDlg->m_pSetting->adminPassword();
		if (!pw.empty())
		{	
			//add by qi 20111104
			//密码为空不用输入密码
			main->m_pDeleteTipDlg->SetPasswordModel(true);
			main->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
		}
	}
	main->m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
	std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_USBTIP);
	main->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp));
	main->m_pDeleteTipDlg->ShowWindow(TRUE);
}

void CSettingDlg::OnSettingFormatSD()
{
	if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOff)
		return;

	m_iDeleteType = 4;
	if (m_pSetting->isAdmin() && !m_bLogin)
	{	
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		std::string pw = main->m_pSettingDlg->m_pSetting->adminPassword();
		if (!pw.empty())
		{	
			//add by qi 20111104
			//密码为空不用输入密码
			main->m_pDeleteTipDlg->SetPasswordModel(true);
			main->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
		}
	}
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
	std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_FORMAT);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp));
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
}

//数据库备份
void CSettingDlg::OnBackup()
{
	m_copyfileDlg->SetType(backup_file);
	m_copyfileDlg->ShowWindow_(SW_SHOW);
}

//数据库恢复
void CSettingDlg::OnRestore()
{
	m_copyfileDlg->SetType(restore_file);
	m_copyfileDlg->ShowWindow_(SW_SHOW);
}

//录音文件导出
void CSettingDlg::OnFileOut()
{
	m_copyfileDlg->SetType(copy_file);
	m_copyfileDlg->ShowWindow_(SW_SHOW);
}

void CSettingDlg ::GetIPInfo()
{
}

void CSettingDlg::SetIPConfig(NETWORK_ADPT_INFO& AdptInfo)
{
	TCHAR szTemp[256] = {0};
	_tcscpy (szTemp, TEXT("Comm\\"));
	//_tcscat (szTemp, TEXT("SMSC91181"));
	_tcscat (szTemp, TEXT("JZ47MAC"));
	_tcscat (szTemp, TEXT("\\Parms\\TcpIp"));
	HKEY hKey = NULL;
	LONG hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, szTemp, 0, 0, &hKey);
	if (ERROR_SUCCESS == hRes)
	{
		DWORD dwDataSize = sizeof(DWORD);
		if (AdptInfo.fUseDHCP)
		{
			hRes = RegSetValueEx(hKey, TEXT("EnableDHCP"), NULL, REG_DWORD, (LPBYTE)&AdptInfo.fUseDHCP, dwDataSize);
		}
		else
		{
			hRes = RegSetValueEx(hKey, TEXT("EnableDHCP"), NULL, REG_DWORD, (LPBYTE)&AdptInfo.fUseDHCP, dwDataSize);
			dwDataSize = wcslen((wchar_t*)AdptInfo.IPAddr) * 2+2;
			hRes = RegSetValueEx(hKey, TEXT("IpAddress"), NULL, REG_MULTI_SZ, (LPBYTE)AdptInfo.IPAddr, dwDataSize);
			dwDataSize = wcslen((wchar_t*)AdptInfo.SubnetMask) * 2+2;
			hRes = RegSetValueEx(hKey, TEXT("Subnetmask"), NULL, REG_MULTI_SZ, (LPBYTE)AdptInfo.SubnetMask, dwDataSize);
			dwDataSize = wcslen((wchar_t*)AdptInfo.Gateway) * 2+2;
			hRes = RegSetValueEx(hKey, TEXT("DefaultGateway"), NULL, REG_MULTI_SZ, (LPBYTE)AdptInfo.Gateway, dwDataSize);
			dwDataSize = wcslen((wchar_t*)AdptInfo.DNSAddr) * 2+2;
			hRes = RegSetValueEx(hKey, TEXT("DNS"), NULL, REG_MULTI_SZ, (LPBYTE)AdptInfo.DNSAddr, dwDataSize);
		}
		RegFlushKey(HKEY_LOCAL_MACHINE);
		RegCloseKey(hKey);
	}
}

void CSettingDlg::OnStaticClick(WPARAM w, LPARAM l)
{
	if(w == IDC_SETTING_VERSION)
	{
		if(m_editVersion.IsWindowVisible())
			m_editVersion.ShowWindow(SW_HIDE);
		else
			m_editVersion.ShowWindow(SW_SHOW);
	}
}

BOOL CSettingDlg::FormatSD(void)
{
    STOREINFO StoreInfo={0}; 
    PARTINFO PartInfo = {0};
    HANDLE hFirstStore,hStore,hPart;
    HINSTANCE hFatUtil = NULL;
    BOOL hDismount= FALSE;
	BOOL hMount=FALSE;
	
	
    FORMAT_PARAMS fp={0};
    FORMAT_OPTIONS  pfo={0};
    DWORD dwClusSize = 0; 
    DWORD dwFatVersion = 16; 
	
    typedef DWORD (*PFN_MY_FORMATVOLUME)(HANDLE hVolume, PDISK_INFO  pdi,  PFORMAT_OPTIONS  pfo, PFN_PROGRESS  pfnProgress,PFN_MESSAGE  pfnMessage); 
	
    StoreInfo.cbSize = sizeof(StoreInfo);

    hFirstStore = FindFirstStore( &StoreInfo );
	
	if(!(hFirstStore ))
    {
		return FALSE;
	}
	
	FindNextStore(hFirstStore,&StoreInfo);

	FindNextStore(hFirstStore,&StoreInfo);

	FindNextStore(hFirstStore,&StoreInfo);
	
	hFatUtil = LoadLibrary(L"fatutil.dll");
	
	PFN_MY_FORMATVOLUME  pfnFormatVolume  =  NULL  ;
	
	pfnFormatVolume =  (PFN_MY_FORMATVOLUME)GetProcAddress(hFatUtil, TEXT("FormatVolume"));
	if (!pfnFormatVolume ) 
	{ 
		return FALSE;
	}
	hStore = OpenStore(StoreInfo.szDeviceName);
	
	hPart = OpenPartition(hStore,L"Part00");
	
	hDismount = DismountPartition(hPart);
	if(!hDismount)
	{
		return FALSE;
	}
	
	if(ERROR_SUCCESS  != pfnFormatVolume(hPart, NULL, NULL,  NULL,  NULL))
	{
		return FALSE;
	}
	
	if( !hDismount) 
	{
		return FALSE;
	}
	
	
	hMount = MountPartition(hPart);
	
	if(!( hMount ))
    {
		return FALSE;
	}
	
    return TRUE;
}

void CSettingDlg::bTntext(CString s)
{
	if(s==CH)
	{
		m_btnLangChange.SetWindowText(EG);
		gLangItem=Data::lEnglish;
		WriteSelectLanguagelog((LPCTSTR)CString("0"));
	}
	else if(s=EG)
	{
		m_btnLangChange.SetWindowText(CH);
		gLangItem=Data::lChinese;
		WriteSelectLanguagelog((LPCTSTR)CString("1"));
	}
}

void CSettingDlg::OnSettingLanguageChange()//?
{
   CString text;

   m_btnLangChange.GetWindowText(text);

   bTntext(text);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pMainDlg->m_mainLunarderDlg_);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pMainDlg->m_mainCalucaterDlg_);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pContactDlg);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pContactDlg->m_pContactSearchDlg);

    SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pContactGroupDlg);

    SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pContactNewDlg);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pTelephoneDlg);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pContactInfoDlg);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pContactInfoDlg->m_pSearchContactInfoDlg);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pContactInfoDlg->m_pPasswordDlg);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pContactInfoDlg->m_pPlaySoundDlg);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pContactInfoDlg->m_pDetailTip);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSoundDlg);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSoundDlg->m_pPasswordDlg);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSoundDlg->m_pSoundSearchDlg);
  
   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSoundDlg->m_pRecordSoundDlg);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSoundDlg->m_pPlaySoundDlg);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSoundDlg->m_psoundTipDlg);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSoundDlg->m_pDetailTip);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->m_pFastDialsDlg);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->m_pPasswordDlg);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->m_copyfileDlg);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pMainDlg->m_firewalDlg_);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pInformationdlg);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pMainDlg);

   SendMsg(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg);
 
}

void CSettingDlg::SendMsg(CWnd *pDlg)
{     
	::PostMessage(pDlg->GetSafeHwnd(),WM_CHANGELANGUAGE,0,0);
}

void USBModal()
{
	PROCESS_INFORMATION processInfo;
	STARTUPINFO lpStartupInfo; // 用于在创建子进程时设置各种属性 
	
	memset(&lpStartupInfo, 0, sizeof(lpStartupInfo));
	
	lpStartupInfo.cb = sizeof(lpStartupInfo);
	
	lpStartupInfo.dwX = 0;
	lpStartupInfo.dwY = 0;
	lpStartupInfo.dwXSize = 0;
	lpStartupInfo.dwYSize = 0;
	lpStartupInfo.wShowWindow= SW_SHOWNORMAL; 
	lpStartupInfo.dwFlags= 1|2|4;
	
	memset(&processInfo, 0, sizeof(processInfo));
	
	if (!CreateProcess(L"\\windows\\USBSwitchTool.exe", NULL, NULL, NULL, NULL, CREATE_NEW_CONSOLE, NULL, NULL, /*&lpStartupInfo*/ 0, &processInfo))
	{
		
	}
	
	::Sleep(50);
	
	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);
}

LRESULT CSettingDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	switch (message)
	{
	case WM_DELETESELITEM:
		if(m_iDeleteType == 0 || m_iDeleteType == 1 ||\
			m_iDeleteType == 2 || m_iDeleteType == 3)
		{
			main->m_pDeleteTipDlg->SetPasswordModel(false);
			main->m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
			std::string str1 = Data::LanguageResource::Get(Data::RI_DELETETIP_DELETE);//是否格式化话机进入升级模式
			main->m_pDeleteTipDlg->SetDelTip(str1.c_str());
			main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		}
		SetTimer(99, 50, NULL);
		break;
	case WM_DELETESELITEMCANCEL:
		{
		/*	if(m_bUSBModal)
			{
				USBModal();
				// lxz 20101228
				m_bUSBModal = FALSE;
				Data::Contact::CloseDb();
				Data::Contact::Opendb();
			}*/
			break;
		}

	case WM_SETTINGPASSWORD:
		if (m_iPasswordType == 0)
		{
			m_pSetting->playRecordPassword((char *)wParam);
			//defect 36
			char * p = (char*)wParam;
			m_pSetting->UpdatePlayRecordPassword(p);
			///zzw10824///////////////////////
			std::string space=p;
			if (space!="")
			{
				m_pPasswordDlg->SetFisrtPPassword(TRUE);
			}
			//================================
		}else if (m_iPasswordType == 1)
		{
			m_pSetting->adminPassword((char *)wParam);
			//defect 36
			char * p = (char*)wParam;
			m_pSetting->UpdateAdminPassword(p);
            ////zzw100824///////////////////////////
			std::string space=p;
			if(space!="")
			{
				m_pPasswordDlg->SetFisrtAPassword(TRUE);
			}
			
		}
		else if (m_iPasswordType == 2)
		{
			m_pSetting->screenSaverPassword((char *)wParam);
			//defect 36
			char* p =(char*)wParam;
			m_pSetting->UpdateScreenSaverPassword(p);
			
			//////zzw10824/////////////////
			std::string space=p;
			if(space!="")
			{
              m_pPasswordDlg->SetFisrtSPassword(TRUE);
			}
			//============
		}
		//defect 36
        //m_pSetting->Update();
		break;
	case WM_CHECKPASSWORD:
		m_bLogin = true;
		m_btnLogin.SetWindowText(Util::StringOp::ToCString(\
			(Data::LanguageResource::Get(Data::RI_SETTING_LOGOUT))));
		ShowConfigItems();
		break;
	case WM_TEL_INUSE:
		if (!m_bTelephoneInUse)
		{
			m_bTelephoneInUse = true;
			m_pOggCodec->StopDecode(true);
			main->phone_->StopRing(true);
		}
		break;
	case WM_TEL_NOUSE:
		m_bTelephoneInUse = false;
		break;
	case WM_CHANGELANGUAGE:
          ChangeLanguage();
		  //IniCtrlData();
         main->SwitchPanel_(IDC_BUTTON_MAIN);
     break;

	case WM_CANCEL:
		if (m_pPasswordDlg->IsWindowVisible())
		{
			m_pPasswordDlg->SendMessage(WM_CANCEL,0,0);
		}
		else
		{    
			 ShowWindow(FALSE);

             OnButtonSettingCancel();
		}
		break;
	case WM_OK:
		if (m_pPasswordDlg->IsWindowVisible())
		{
			m_pPasswordDlg->SendMessage(WM_OK,0,0);

		}else{
		    OnButtonSettingOk();
		}
		
		break;
	case WM_ITYPE:
		if (main->m_pSettingDlg->IsWindowVisible())
		{
			POSITION pos = m_lsType.GetFirstSelectedItemPosition();
			if (pos != NULL)
			{
			int type = m_lsType.GetNextSelectedItem(pos);
			type++;
			m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle());
			m_lsType.SetItemState(type,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
			if (type==4)
			{
			type=0;
			m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
            m_lsType.SetItemState(type,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
			}
			}
			ShowConfigItems();
			m_lsType.Invalidate();
		}
		break;
	case WM_STOPTRYRING:
		setButtoName();
		break;
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}

void CSettingDlg::ChangeLanguage()
{	
   
	m_lsType.DeleteAllItems();

	std::string strTemp;

	CString str;

	strTemp=Data::LanguageResource::Get(Data::RI_COMPANY);
	str =strTemp.c_str();
	m_sticVersionRight.SetWindowText(str);

    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TELEPHONE);
	str = strTemp.c_str();
	m_lsType.InsertItem(0, str, 0);  
    
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_INTERNET);
	str = strTemp.c_str();
	m_lsType.InsertItem(1, str, 1);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SYSTEM);
	str = strTemp.c_str();
	m_lsType.InsertItem(2, str, 2);
    
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_INFORMATION);
	str = strTemp.c_str();
	m_lsType.InsertItem(3, str, 3);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RINGINFO);
	str = strTemp.c_str();
	m_lsType.InsertItem(4, str, 4);

    m_lsType.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

	ShowConfigItems();

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SAVE);

	str = strTemp.c_str();

	m_btnOk.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DEFAULT);
	str = strTemp.c_str();
	m_btnDefault.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LOGIN);
	str = strTemp.c_str();
	m_btnLogin.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnCancel.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_AREACODE);
	str = strTemp.c_str();
	m_sticLocalAreaNumber.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_OUTLINE);
	str = strTemp.c_str();
	m_chbEnableOutLine.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LACALNUMBER);
	str = strTemp.c_str();
	m_sticLocalNumber.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_VOLUME);//音量
	str = strTemp.c_str();
	m_sticVolume.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLERING);//102118
	str = strTemp.c_str();
	m_chbEnableRing.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLERINGSTC);
	str = strTemp.c_str();
	m_sticDefaultRing.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLEAUTOLEAVEWORD);
	str = strTemp.c_str();
	m_chbEnableAutoLeaveWord.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLESpeakerOut);
	str=strTemp.c_str();
	m_IsSpeakerOut.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RINGTIMES);
	str = strTemp.c_str();
	m_sticRingTimes.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LISTEN);
	str = strTemp.c_str();
	m_btnPlayRing.SetWindowText(str);
	m_btnPlayLeaveWordTip.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DEFINESPEEDBUTTON);
	str = strTemp.c_str();
	m_btnDefineSpeedButton.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DEFINESPEEDBUTTONSTC);
	str = strTemp.c_str();
	m_sticDefineSpeedButton.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SETPASSWORD);
	str = strTemp.c_str();
	m_btnSetPlayPassword.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLEPLAY);
	str = strTemp.c_str();
	m_chbEnabePlayProtect.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_MAX_REC_TIME);
	str = strTemp.c_str();
	m_sticRecordLimit.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SETPLAYPASSWORD);
	str = strTemp.c_str();
	m_chbSetAdminPassword.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SETPASSWORD);
	str = strTemp.c_str();
	m_btnSetAdminPassword.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLESP);
	str = strTemp.c_str();
	m_chbEnnabeScreenProtect.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_WAITTIME);
	str = strTemp.c_str();
	m_sticWaitTime.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLEPASSWORD);
	str = strTemp.c_str();
	m_chbEnablePassword.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SETPASSWORD);
	str = strTemp.c_str();
	m_btnSetPassword.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_BACKLIGHT);
	str = strTemp.c_str();
	m_sticBacklight.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SOUNDSAVEPATH);
	str = strTemp.c_str();
	m_sticSoundSavePath.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SYSTEMTIME);
	str = strTemp.c_str();
	m_stcTime.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SCREENADJUSTSTC);
	str = strTemp.c_str();
	m_sticTouchAdjust.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SCREENADJUST);
	str = strTemp.c_str();
	m_btnTouchAdjust.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SYSTOMTITLE);
	str = strTemp.c_str();
	m_sticSystomTitle.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SERIALNO);
	str = strTemp.c_str();
	m_sticSystomSerialNo1.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SOFTVERSION);
	str = strTemp.c_str();
	m_sticSoftwareVersion1.SetWindowText(str);

	str = s_VerSion;
	m_sticSoftwareVersion.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_HARDVERSION);
	str = strTemp.c_str();
	m_sticHardwareVersion1.SetWindowText(str);
	str = s_VerSion;
	m_sticHardwareVersion.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SDSTORAGESIZE);
	str = strTemp.c_str();
	m_sticStorageSize1.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SDSTORAGESIZE);
	str = strTemp.c_str();
	m_sticStorageSize1.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_FORMATSD);
	str = strTemp.c_str();
	m_btnFormatSD.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CONTACTSIZE);
	str = strTemp.c_str();
	m_sticContactSize1.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SOUNDSIZE);
	str = strTemp.c_str();
	m_sticSoundSize1.SetWindowText(str);
	
	//未接电话数
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_UNCONNECTNUM);
	str = strTemp.c_str();
	m_sticUconnectNum.SetWindowText(str);
	
	//清除未接电话按钮
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ONEKEYCLEAR);
	str = strTemp.c_str();
	m_btnSetOneKeyClear.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_INFORMATIONSIZE);
	str = strTemp.c_str();
	m_sticInformationSize1.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARCONTACTINFO);
	str = strTemp.c_str();
	m_btnClearContactInfo.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARSOUND);
	str = strTemp.c_str();
	m_btnClearSound.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARINFORMATION);
	str = strTemp.c_str();
	m_btnClearInformation.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_VERSIONRIGHT);
	str = strTemp.c_str();
	m_sticVersionRight1.SetWindowText(str);

    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TITILEBACKUP);
	str = strTemp.c_str();
	m_stcBackUP.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RESTORE);
	str = strTemp.c_str();
	m_btnReStore.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TITLE);
	str = strTemp.c_str();
	m_stcTitle.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_OUT);
	str = strTemp.c_str();
	m_btnOut.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARCONTACT);
	str = strTemp.c_str();
	m_btnClearContact.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_FLASHSIZE);
	str = strTemp.c_str();
	m_sticMemorySize1.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CONTACTINFOSIZE);
	str = strTemp.c_str();
	m_sticContactInfoSize1.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_BACKUP);
	str = strTemp.c_str();
	m_btnBackUP.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLERECORD);
	str = strTemp.c_str();
	m_chbEnabeRecord.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLEDELETE);
	str = strTemp.c_str();
	m_chbEnabeDeleteProtect.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_IPDIAL);
	str = strTemp.c_str();
	m_chbEnableIPDial.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_VERSIONCHANGE);
	str = strTemp.c_str();
	m_versionChange.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RINGSIZE);
	str = strTemp.c_str();
	m_sticRingInfo.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RINGLIST);
	str = strTemp.c_str();
	m_sticRingList.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RINGCLEAR);
	str = strTemp.c_str();
	m_btnClearRing.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RINGDELETE);
	str = strTemp.c_str();
	m_btnDeleteRing.SetWindowText(str);
/*
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_USBPORT);
	str = strTemp.c_str();
	m_sticUSBPort.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_USBMODAL);
	str = strTemp.c_str();
	m_btnUSBModal.SetWindowText(str);
*/
	TextStruct ts[3];
	memset(ts, 0, sizeof(TextStruct) * 3);
	
	ts[0].txtRect = CRect(0, 0, 80, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_TYPE).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_TYPE).length());
	
	ts[1].txtRect = CRect(81, 0, 415, 20*VHEIGHT);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[1].sTxt, Data::LanguageResource::Get(Data::RI_SETTING_DETAILTITLE).c_str(), Data::LanguageResource::Get(Data::RI_SETTING_DETAILTITLE).length());
	
	ts[2].txtRect = CRect(415, 0, 475, 20*VHEIGHT);
	ts[2].txtFontSize = 16;
	ts[2].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[2].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).length());
		
	m_sticBackground.SetTextStruct(ts, 3);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_AVAILABLESIZE);//?
	CString as = Util::StringOp::ToCString( strTemp);
	CString text;
	ULARGE_INTEGER freeBytes;
	ULARGE_INTEGER totalBytes;
	ULARGE_INTEGER totalFreeBytes;
	GetDiskFreeSpaceEx(_T("\\Flashdrv"), &freeBytes, &totalBytes, &totalFreeBytes);
	int m = totalBytes.QuadPart / (1024 * 1024);
	int f = 0;
	if (totalFreeBytes.QuadPart - PreFreeSize > 0)
	{
		f = totalFreeBytes.QuadPart - PreFreeSize;
	}
	else
	{
		f = 0;
	}
	f = f/ (1024 * 1024);
	text.Format(_T("%dM (%dM%s)"), m, f, as);
	m_sticMemorySize.SetWindowText(text);
	m_sticMemorySize.ShowWindow(TRUE);

	//移动控件
	MoveControl();
	ChangeTips();
	ChangeReg();

}

void CSettingDlg::OnButtonSysInfo()
{  //显示系统序列号等信息
	//((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->getOutPutStringData();
}

#if outputstring_data
void CSettingDlg::OnRegRecordSound()
{
	return;  //wzx20101221
	if (m_chbEnabeDeleteProtect.GetCheck() == BST_CHECKED)
	{
		m_pRegRecordSound->ShowWindow(TRUE);
	}
}
#endif
void CSettingDlg::OnTimer(UINT nIDEvent)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (nIDEvent==100)
	{ 
		if(m_bPlayPause)
		{  
			++m_iCurrentSecond;
            if (m_iCurrentSecond>m_iTotalSeconds)
            { 	
				m_bPlayPause = false;
            }
		}else
		{   
			std::string strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LISTEN);	
			CString	str = strTemp.c_str();	
			m_btnPlayLeaveWordTip.SetWindowText(str);
			m_bPlayPause=false;
			m_iCurrentSecond=0;
			KillTimer(100);
			int v = m_pSetting->soundVolume();
			DWORD volume[]={0x10001000,0x44004400,0x88008800,0xcc00cc00,0xff00ff00};
			int volume2[] = {-2400, -1800, -1200, -600, -1};
			waveOutSetVolume(NULL, volume[v]);
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetSoundRingVal(volume2[v]);
		}
	}
	else if(99 == nIDEvent)  //wzx 20101228
	{
		KillTimer(nIDEvent);
		int count = 0;
		CString text;
		if (m_iDeleteType == 0)
		{
			Data::ContactGroup::Remove("");
			main->m_pContactDlg->ShowTypeItems();
			Data::Contact::Remove("");
			//Data::Contact::Compression();  //wangzhenxing20101227
			g_iContactCount = Data::Contact::GetDataCount("");
			text.Format(_T("%d"), g_iContactCount);
			m_sticContactSize.SetWindowText(text);
			main->m_pContactDlg->ShowItemsInList();
		}
		else if (m_iDeleteType == 1)
		{
			Data::ContactInfo::Remove("");
			g_iContactInfoCount = Data::ContactInfo::GetDataCount("");
			text.Format(_T("%d"), g_iContactInfoCount);
			m_sticContactInfoSize.SetWindowText(text);
			main->m_pContactInfoDlg->ShowItemsInList();
			main->m_pMainDlg->m_nUnconnectCount = 0;
			::PostMessage(main->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
		}

		else if (m_iDeleteType == 2)
		{
			Data::SoundSegment::Remove("");
			//DeleteFiles((LPTSTR)(LPCTSTR)CString(csFlashRecordPath + _T("*")));
			RecursiveDeleteFiles((LPTSTR)(LPCTSTR)CString(csFlashRecordPath + _T("*")));
			
			if (DetectDIR((LPTSTR)(LPCTSTR)csStorageCardRecordPath))
			{
			//	DeleteFiles((LPTSTR)(LPCTSTR)CString(csStorageCardRecordPath + _T("*")));
				RecursiveDeleteFiles((LPTSTR)(LPCTSTR)CString(csFlashRecordPath + _T("*")));
			}
			
			g_iRecordCount = Data::SoundSegment::GetDataCount("");
			text.Format(_T("%d"), g_iRecordCount);
			m_sticSoundSize.SetWindowText(text);
			std::vector<boost::shared_ptr<Data::ContactInfo> > result = Data::ContactInfo::GetFromDatabase("isSound = 1");
			if (!result.empty())
			{
				for (std::vector<boost::shared_ptr<Data::ContactInfo> >::iterator iter = result.begin(); iter != result.end(); ++iter)
				{
					(*iter)->isSound(false);
					(*iter)->Update();
				}
			}
			main->m_pSoundDlg->ShowItemsInList();
			main->m_pContactInfoDlg->ShowItemsInList();
			main->m_pMainDlg->m_nRecordCount = 0;
			::PostMessage(main->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
		}
		else if (m_iDeleteType == 3)
		{
			DeleteDirectory(csFlashRingPath);  //清空铃声
			CString delRIngList = ssFlashPath + _T("RingList.txt");
            DeleteFile((LPCTSTR)delRIngList);
			m_cmbDelRingList.ResetContent();
			m_cmbDelRingList.Invalidate();
			m_sticRingSize.SetWindowText(L"0");
			m_cmbRing.ResetContent();
			m_cmbRing.Invalidate();
		}
		else if (m_iDeleteType == 4)
		{
			FormatSD();
		}
		else if (m_iDeleteType == 5)
		{
			//恢复密码  wzx 20110111
			DeleteFile(L"/Hive/startflag.txt");
			m_pSetting->UpdateAdminPassword("");
			m_pSetting->UpdateScreenSaverPassword("");
			m_pSetting->UpdatePlayRecordPassword("");
			m_pPasswordDlg->SetFisrtAPassword(FALSE);
			m_pPasswordDlg->SetFisrtPPassword(FALSE);
			m_pPasswordDlg->SetFisrtSPassword(FALSE);

			int id = m_pSetting->id();
			Data::SettingType type = m_pSetting->type();
			m_pSetting = Data::Setting::GetDefaultConfig();
			m_pSetting->id(id);
			m_pSetting->type(type);
			m_pSetting->Update();
			IniCtrlData();
			ShowConfigItems();
		}
		m_iDeleteType = -1;   //lxz 20101228
		//操作已成功
		main->m_pDeleteTipDlg->SetPasswordModel(false);
		main->m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
		std::string str1 = Data::LanguageResource::Get(Data::RI_DELETETIP_OPSUCCESS);//是否将Hive和FlashDrv下的文件删除
		main->m_pDeleteTipDlg->SetDelTip(str1.c_str());
		main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
	}
	else if(97 == nIDEvent)
	{
		KillTimer(nIDEvent);
		main->KillScreenSaveTimer();
		KeyBoardOff();
		main->HideComboBoxDropDown();
		main->m_pMainDlg->m_mainScreenSaveDlg_->ShowWindow(TRUE);
	}
}
void CSettingDlg::setButtoName()
{
	std::string strTemp;
	CString str;
	if (m_iSBell)
	{
		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LISTEN);
		str = strTemp.c_str();
		m_btnPlayRing.SetWindowText(str);
		m_bPlaying=false;	
	}else
	{   
		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LISTEN);
		str = strTemp.c_str();
		m_btnPlayLeaveWordTip.SetWindowText(str);
		m_bPlayPause = false;
	}
	m_iSBell=false;
	//wzx 20110110
	int v = m_pSetting->soundVolume();
	DWORD volume[]={0x10001000,0x44004400,0x88008800,0xcc00cc00,0xff00ff00};//调节 音量
	int volume2[] = {-2400, -1800, -1200, -600, -1};
	waveOutSetVolume(NULL, volume2[v]);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetSoundRingVal(volume2[v]);
}

void CSettingDlg::SettingDefault()
{
	//恢复密码
	m_pSetting->UpdateAdminPassword("");
	m_pSetting->isAdmin(false);
	m_pSetting->UpdateScreenSaverPassword("");
	m_pSetting->isUseScreenSaverPassword(false);
	m_pSetting->UpdatePlayRecordPassword("");
	m_pSetting->isPlayProtect(false);
	m_bLogin=false;
	/////////////////zzw10824///////////////////////////
	m_pPasswordDlg->SetFisrtAPassword(FALSE);
	m_pPasswordDlg->SetFisrtPPassword(FALSE);
	m_pPasswordDlg->SetFisrtSPassword(FALSE);
	
	//add 20111117
	m_pSetting->Update();
	IniCtrlData();
	ShowConfigItems();

}

void CSettingDlg::GetSystemInfo()
{
	FILE *fp = fopen("\\hive\\productcode.dat", "r");
	if(fp)
	{
		memset(m_sSerialNo, 0, 64);
		memset(m_sHardwareNo, 0, 64);
		char buf[256] = {0};
		fread(buf, sizeof(char), 256, fp);
		
		char *start = NULL;
		char *end = NULL;
		if(strstr(buf, "SerialNo:") != NULL)
		{
			start = strstr(buf, "SerialNo:");
			start += strlen("SerialNo:");
			end = strstr(start, ";");
			memcpy(m_sSerialNo, start, end-start);
		}
		
		if(strstr(buf, "HardwareNo:") != NULL)
		{
			start = strstr(buf, "HardwareNo:");
			start += strlen("HardwareNo:");
			end = strstr(start, ";");
			memcpy(m_sHardwareNo, start, end-start);
		}
		fclose(fp);
	}
}

//add by yy 20111103
void CSettingDlg::OnButtonMissCallClear()
{   
    CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
   	std::string filter("");
	filter +=" SET [played] = 1";
	filter += " WHERE played = 0 AND type = 0";
	Data::ContactInfo::Update(filter);
	
    main->m_pMainDlg->m_nUnconnectCount = 0;
    ::PostMessage(main->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
	
	main->m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
	std::string str1 = Data::LanguageResource::Get(Data::RI_CLEARMISSEDCALL_OPSUCCESS);
	main->m_pDeleteTipDlg->SetDelTip(str1.c_str());
	main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
	
	ShowConfigItems();
}


