// MultimediaPhoneDlg.cpp : implementation file
//


#include "stdafx.h"
#include "MultimediaPhone.h"
#include "MultimediaPhoneDlg.h"
#include "./View/ViewConstants.h"
#include "./Data/SkinStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "View/MainDlg.h"
#include "View/ContactDlg.h"
#include "View/ContactInfoDlg.h"
#include "View/SoundDlg.h"
#include "View/InformationDlg.h"
#include "View/SettingDlg.h"
#include "View/ContactNewDlg.h"

#include "Data/LanguageResource.h"

//#define WM_PLAYVIDEO	8002


void CopyDirFiles()
{
	CString SrcDir = "/usbdisk";
	CString DesDir = "/flashdrv/my_photo";
	CString findFilename = SrcDir + "/*.*";
	//wprintf(_T("find file name is %s\n"), (LPCTSTR)findFilename);
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
					printf("current error is %d.", GetLastError());
				}
				wprintf(_T("copy %s to %s\n"), wideFromName, wideToName);
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
// CMultimediaPhoneDlg dialog

CMultimediaPhoneDlg::CMultimediaPhoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultimediaPhoneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultimediaPhoneDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMultimediaPhoneDlg::~CMultimediaPhoneDlg()
{
	for (std::map<int, CDialog*>::iterator i = panels_.begin(); i != panels_.end(); ++i)
		delete i->second;
}

void CMultimediaPhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultimediaPhoneDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

//  	DDX_Control(pDX, IDC_BUTTON_TIME, m_btnTime);
//  	DDX_Control(pDX, IDC_BUTTON_IME, m_btnIME);
//  	DDX_Control(pDX, IDC_BUTTON_LINE, m_btnLine);
//  	DDX_Control(pDX, IDC_BUTTON_MAIN, m_btnMain);
//  	DDX_Control(pDX, IDC_BUTTON_CONTACT, m_btnContact);
//  	DDX_Control(pDX, IDC_BUTTON_CONTACTINFO, m_btnContactInfo);
//  	DDX_Control(pDX, IDC_BUTTON_SOUND, m_btnSound);
//  	DDX_Control(pDX, IDC_BUTTON_INFORMATION, m_btnInformation);
//  	DDX_Control(pDX, IDC_BUTTON_SETTING, m_btnSetting);
}

BEGIN_MESSAGE_MAP(CMultimediaPhoneDlg, CDialog)
	//{{AFX_MSG_MAP(CMultimediaPhoneDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SOFTKEY, OnMainSoftKey)
	/*
	ON_BN_CLICKED(IDC_BUTTON_TIME, OnButtonTime)
	ON_BN_CLICKED(IDC_BUTTON_IME, OnButtonIME)
	ON_BN_CLICKED(IDC_BUTTON_LINE, OnButtonLine)
	ON_BN_CLICKED(IDC_BUTTON_MAIN, OnButtonMain)
	ON_BN_CLICKED(IDC_BUTTON_CONTACT, OnButtonContact)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO, OnButtonContactinfo)
	ON_BN_CLICKED(IDC_BUTTON_SOUND, OnButtonSound)
	ON_BN_CLICKED(IDC_BUTTON_INFORMATION, OnButtonInformation)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, OnButtonSetting)
	*/
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneDlg message handlers

BOOL CMultimediaPhoneDlg::OnInitDialog()
{
	//lxz test jpg
	//CopyDirFiles();

	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CenterWindow(GetDesktopWindow());	// center to the hpc screen

	// TODO: Add extra initialization here
	//step1: global variable init

	//step2: global workthread init

	//step3: config data read and fill

	//step4: device and environment check and init

	//step5: init UI
	
	//init player
	//other wnd
	player_ = new Player(NULL);
	player_->SetPlayerOwner(this);

	phone_ = new Phone();
	//mainVideoDlg_ = new CMainVideoDlg();
	//mainVideoDlg_->Create(CMainVideoDlg::IDD);
	//playerDlg_ = new CPlayerDlg();
	//playerDlg_->Create(CPlayerDlg::IDD);

	ICONKEYSTRUCT softkeyval_[9];
	softkeyval_[0].sKeyRect = CRect(3, 9, 40, 24);
	softkeyval_[0].nKeyValue = 1;
	softkeyval_[0].nTextId = 0;
	softkeyval_[0].nBitmapId = 0;
	softkeyval_[0].nAlign = DT_LEFT;
	softkeyval_[0].bToggle = FALSE;

	softkeyval_[1].sKeyRect = CRect(44, 9, 59, 24);
	softkeyval_[1].nKeyValue = 2;
	softkeyval_[1].nTextId = 0;
	softkeyval_[1].nBitmapId = IDB_BITMAP_MAININPUT;
	softkeyval_[1].nAlign = DT_LEFT;
	softkeyval_[1].bToggle = FALSE;

	softkeyval_[2].sKeyRect = CRect(63, 9, 78, 24);
	softkeyval_[2].nKeyValue = 3;
	softkeyval_[2].nTextId = 0;
	softkeyval_[2].nBitmapId = IDB_BITMAP_MAINTEL;
	softkeyval_[2].nAlign = DT_LEFT;
	softkeyval_[2].bToggle = FALSE;

	softkeyval_[3].sKeyRect = CRect(102, 4, 166, 28);
	softkeyval_[3].nKeyValue = 4;
	softkeyval_[3].nTextId = Data::RI_MAIN_MAINBTN;
	softkeyval_[3].nBitmapId = IDB_BITMAP_MAINDESKTOP;
	softkeyval_[3].nAlign = DT_LEFT;
	softkeyval_[3].bToggle = TRUE;

	softkeyval_[4].sKeyRect = CRect(169, 4, 229+1, 28);
	softkeyval_[4].nKeyValue = 5;
	softkeyval_[4].nTextId = Data::RI_MAIN_CARDCASEBTN;
	softkeyval_[4].nBitmapId = IDB_BITMAP_MAINCARD;
	softkeyval_[4].nAlign = DT_LEFT;
	softkeyval_[4].bToggle = TRUE;

	softkeyval_[5].sKeyRect = CRect(232, 4, 292+1, 28);
	softkeyval_[5].nKeyValue = 6;
	softkeyval_[5].nTextId = Data::RI_MAIN_CALLLISTBTN;
	softkeyval_[5].nBitmapId = IDB_BITMAP_MAINCALLLIST;
	softkeyval_[5].nAlign = DT_LEFT;
	softkeyval_[5].bToggle = TRUE;

	softkeyval_[6].sKeyRect = CRect(295, 4, 355+1, 28);
	softkeyval_[6].nKeyValue = 7;
	softkeyval_[6].nTextId = Data::RI_MAIN_SOUND;
	softkeyval_[6].nBitmapId = IDB_BITMAP_MAINRECORD;
	softkeyval_[6].nAlign = DT_LEFT;
	softkeyval_[6].bToggle = TRUE;

	softkeyval_[7].sKeyRect = CRect(358, 4, 416+3, 28);
	softkeyval_[7].nKeyValue = 8;
	softkeyval_[7].nTextId = Data::RI_MAIN_STOCKINFOBTN;
	softkeyval_[7].nBitmapId = IDB_BITMAP_MAININFO;
	softkeyval_[7].nAlign = DT_LEFT;
	softkeyval_[7].bToggle = TRUE;

	softkeyval_[8].sKeyRect = CRect(421, 4, 479, 28);
	softkeyval_[8].nKeyValue = 9;
	softkeyval_[8].nTextId = Data::RI_MAIN_SYSTTEMSETTING;
	softkeyval_[8].nBitmapId = IDB_BITMAP_MAINSETTING;
	softkeyval_[8].nAlign = DT_LEFT;
	softkeyval_[8].bToggle = TRUE;
	
	m_StatusKey.Create(IDB_BITMAP_MAIN, IDB_BITMAP_MAINTOGGLE, 3, CPoint(0, 0), 9,  softkeyval_, CRect(0, 204, 479, 233), this, WS_VISIBLE|WS_CHILD, IDC_BUTTON_TIME);
	SetSkinStyle();
	m_StatusKey.SetLeftTimer();
	/*
	m_btnTime.Create(str, WS_CHILD|WS_VISIBLE, CRect(0, 205, 41, 234), this, IDC_BUTTON_TIME);
	m_btnIME.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(41, 205, 65, 234), this, IDC_BUTTON_IME);
	m_btnIME.SetIcon(IDI_ICON_IME, CSize(16, 16));
	m_btnLine.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(65, 205, 89, 234), this, IDC_BUTTON_LINE);
	m_btnLine.SetIcon(IDI_ICON_LINE, CSize(16, 16));

	strTemp = Data::LanguageResource::Get(Data::RI_MAIN_MAINBTN);
	str = strTemp.c_str();		
	m_btnMain.Create(str, WS_CHILD|WS_VISIBLE, CRect(91, 205, 154, 234), this, IDC_BUTTON_MAIN);
	m_btnMain.SetWindowText(str);
	m_btnMain.SetIcon(IDI_ICON_DESK, CSize(16, 16));
	
	strTemp = Data::LanguageResource::Get(Data::RI_MAIN_CARDCASEBTN);
	str = strTemp.c_str();
	m_btnContact.Create(str, WS_CHILD|WS_VISIBLE, CRect(156, 205, 219, 234), this, IDC_BUTTON_CONTACT);
	m_btnContact.SetWindowText(str);
	m_btnContact.SetIcon(IDI_ICON_CONTACT, CSize(16, 16));
	
	strTemp = Data::LanguageResource::Get(Data::RI_MAIN_CALLLISTBTN);
	str = strTemp.c_str();
	m_btnContactInfo.Create(str, WS_CHILD|WS_VISIBLE, CRect(221, 205, 284, 234), this, IDC_BUTTON_CONTACTINFO);
	m_btnContactInfo.SetWindowText(str);
	m_btnContactInfo.SetIcon(IDI_ICON_CONTACTINFO, CSize(16, 16));

	strTemp = Data::LanguageResource::Get(Data::RI_MAIN_SOUND);
	str = strTemp.c_str();
	m_btnSound.Create(str, WS_CHILD|WS_VISIBLE, CRect(286, 205, 349, 234), this, IDC_BUTTON_SOUND);
	m_btnSound.SetWindowText(str);
	m_btnSound.SetIcon(IDI_ICON_SOUND, CSize(16, 16));

	strTemp = Data::LanguageResource::Get(Data::RI_MAIN_STOCKINFOBTN);
	str = strTemp.c_str();
	m_btnInformation.Create(str, WS_CHILD|WS_VISIBLE, CRect(351, 205, 414, 234), this, IDC_BUTTON_INFORMATION);
	m_btnInformation.SetWindowText(str);
	m_btnInformation.SetIcon(IDI_ICON_INFO, CSize(16, 16));

	strTemp = Data::LanguageResource::Get(Data::RI_MAIN_SYSTTEMSETTING);
	str = strTemp.c_str();
	m_btnSetting.Create(str, WS_CHILD|WS_VISIBLE, CRect(416, 205, 480, 234), this, IDC_BUTTON_SETTING);
	m_btnSetting.SetWindowText(str);
	m_btnSetting.SetIcon(IDI_ICON_SETTING, CSize(16, 16));
	*/

	CDialog* panel = new CMainDlg();
	panel->Create(CMainDlg::IDD);
	panels_[IDC_BUTTON_MAIN] = panel;
	currentPanel_ = panel;

	panel = new CContactDlg();
	panel->Create(CContactDlg::IDD);
	panels_[IDC_BUTTON_CONTACT] = panel;

	panel = new CContactInfoDlg();
	panel->Create(CContactInfoDlg::IDD);
	panels_[IDC_BUTTON_CONTACTINFO] = panel;

	panel = new CSoundDlg();
	panel->Create(CSoundDlg::IDD);
	panels_[IDC_BUTTON_SOUND] = panel;

	panel = new CInformationDlg();
	panel->Create(CInformationDlg::IDD);
	panels_[IDC_BUTTON_INFORMATION] = panel;

	//test CEDlg
/*	m_pTestCEDlg = new CTestCEDlg();
	panel = m_pTestCEDlg;
	panel->Create(CTestCEDlg::IDD);
	panels_[IDC_BUTTON_INFORMATION] = panel;
*/
	panel = new CSettingDlg();
	panel->Create(CSettingDlg::IDD);
	panels_[IDC_BUTTON_SETTING] = panel;

	m_pContactNewDlg = new CContactNewDlg(panels_[IDC_BUTTON_CONTACT]);
	panel = m_pContactNewDlg;
	panel->Create(CContactNewDlg::IDD);
	panels_[IDC_BUTTON_CONTACTNEW] = panel;

	m_pContactGroupDlg = new CContactGroupDlg(panels_[IDC_BUTTON_CONTACT]);
	panel = m_pContactGroupDlg;
	panel->Create(CContactGroupDlg::IDD);
	panels_[IDC_BUTTON_CONTACTGROUP] = panel;

	m_pTelephoneDlg = new CTelephoneDlg();
	m_pTelephoneDlg->Create(CTelephoneDlg::IDD);
/*
	buttons_[IDC_BUTTON_TIME] = &m_btnTime;
	buttons_[IDC_BUTTON_IME] = &m_btnIME;
	buttons_[IDC_BUTTON_LINE] = &m_btnLine;
	buttons_[IDC_BUTTON_MAIN] = &m_btnMain;
	buttons_[IDC_BUTTON_CONTACT] = &m_btnContact;
	buttons_[IDC_BUTTON_CONTACTINFO] = &m_btnContactInfo;
	buttons_[IDC_BUTTON_SOUND] = &m_btnSound;
	buttons_[IDC_BUTTON_INFORMATION] = &m_btnInformation;
	buttons_[IDC_BUTTON_SETTING] = &m_btnSetting;

	for (std::map<int, CCEButtonST*>::iterator iter = buttons_.begin(); iter != buttons_.end(); ++iter)
	{
		SetButtonDefaultColor((*iter).second);
	}
	*/
	currentPanel_->ShowWindow(SW_SHOW);
//	m_pBtnCurrent = &m_btnMain;
//	SetButtonSelectedColor(m_pBtnCurrent);

	//and so on
	//m_btnLine.EnableWindow(FALSE);
	m_pFSM = new Util::FSM;
	AddFSMRules();
	m_uiTimer = 0;

	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CMultimediaPhoneDlg::SetButtonDefaultColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(154,212,255));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(154,212,255));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(154,212,255));
}
void CMultimediaPhoneDlg::SetButtonSelectedColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(235,252,255));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(235,252,255));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(225,252,255));
}
void CMultimediaPhoneDlg::SwitchPanel_(int panelId)
{
	if(currentPanel_ == panels_[IDC_BUTTON_MAIN])	//停止播放
	{
		panels_[IDC_BUTTON_MAIN]->SendMessage(WM_PLAYVIDEO, 0);
	}
	
	if (panels_[panelId] != currentPanel_)
	{
		/*
		SetButtonDefaultColor(m_pBtnCurrent);
		m_pBtnCurrent = buttons_[panelId];
		SetButtonSelectedColor(m_pBtnCurrent);
		*/
		currentPanel_->ShowWindow(SW_HIDE);
		currentPanel_ = panels_[panelId];
		currentPanel_->ShowWindow(SW_SHOW);
	}

	if(currentPanel_ == panels_[IDC_BUTTON_MAIN] && player_->type_)	//恢复播放消息
	{
		panels_[IDC_BUTTON_MAIN]->SendMessage(WM_PLAYVIDEO, 1);
	}
}

void CMultimediaPhoneDlg::OnButtonMain() 
{
	// TODO: Add your control notification handler code here
	SwitchPanel_(IDC_BUTTON_MAIN);
}

void CMultimediaPhoneDlg::OnButtonContact() 
{
	// TODO: Add your control notification handler code here
	SwitchPanel_(IDC_BUTTON_CONTACT);
}

void CMultimediaPhoneDlg::OnButtonContactinfo() 
{
	// TODO: Add your control notification handler code here
	SwitchPanel_(IDC_BUTTON_CONTACTINFO);
}

void CMultimediaPhoneDlg::OnButtonSound() 
{
	// TODO: Add your control notification handler code here
	SwitchPanel_(IDC_BUTTON_SOUND);	
}

void CMultimediaPhoneDlg::OnButtonInformation() 
{
	// TODO: Add your control notification handler code here
	SwitchPanel_(IDC_BUTTON_INFORMATION);
	//test CEDialog
//	m_pTestCEDlg->ShowWindow(TRUE);
}

void CMultimediaPhoneDlg::OnButtonSetting() 
{
	// TODO: Add your control notification handler code here
	SwitchPanel_(IDC_BUTTON_SETTING);	
}

void CMultimediaPhoneDlg::OnButtonTime() 
{
	// TODO: Add your control notification handler code here
}

void CMultimediaPhoneDlg::OnButtonIME() 
{
	// TODO: Add your control notification handler code here
}

void CMultimediaPhoneDlg::OnButtonLine() 
{
	// TODO: Add your control notification handler code here
	m_pTelephoneDlg->ShowWindow(TRUE);	
}

BOOL CMultimediaPhoneDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CMultimediaPhoneDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_LBUTTONDOWN:
		panels_[IDC_BUTTON_MAIN]->SendMessage(WM_PLAYVIDEO, 3); //退出全屏
		break;
	case WM_APP + 88: //WM_PlayEnded:
		{
		//	printf("player file start!\n");
	//		Sleep(100);
			panels_[IDC_BUTTON_MAIN]->SendMessage(WM_PLAYVIDEO, 4); //播放下一个文件
		}
		break;
	case WM_APP+60:
		printf("0:");
		break;
	case WM_TEL_RING:
		printf("RING \n");
		m_pFSM->fireEvent(teRing, 0);
		break;
	case WM_TEL_HUNGOFF:
		printf("HUNG OFF \n");
		m_pFSM->fireEvent(teHangOff, 0);
		break;
	case WM_TEL_HUNGON:
		printf("HUNG ON \n");
		m_pFSM->fireEvent(teHangOn, 0);
		break;
	case WM_TEL_CALLIDEND:
		printf("CALL ID \n");
		m_pFSM->fireEvent(teCallID, 0);
		break;
	case WM_TEL_TELCODE:
		printf("%x ", wParam);
		m_pFSM->fireEvent(teCode, (void * const)wParam);
		break;
	case WM_TEL_KEYCODE:
		printf("%x ", wParam);
		m_pFSM->fireEvent(teKey, (void * const)wParam);
		break;
	default:
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

void CMultimediaPhoneDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CMultimediaPhoneDlg::EnableLine(BOOL bEnable)
{
	m_StatusKey.SetEnableKey(2, bEnable);
//	m_btnLine.EnableWindow(bEnable);
}

void CMultimediaPhoneDlg::AddFSMRules(void)
{
	m_pFSM->registerRule(tsHangOff, teHangOn, tsHangOn, (void (__cdecl *const )(void *)) HangOn);
	m_pFSM->registerRule(tsHangOff, teRing, tsRing, (void (__cdecl *const )(void *)) Ring);

	m_pFSM->registerRule(tsRing, teHangOn, tsConnect, (void (__cdecl *const )(void *)) Connect);
	m_pFSM->registerRule(tsRing, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);
	m_pFSM->registerRule(tsRing, teCode, tsCode, (void (__cdecl *const )(void *)) Code);

 	m_pFSM->registerRule(tsCode, teHangOn, tsConnect, (void (__cdecl *const )(void *)) Connect);
 	m_pFSM->registerRule(tsCode, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);
 	m_pFSM->registerRule(tsCode, teCallID, tsCallID, (void (__cdecl *const )(void *)) CallID);	

 	m_pFSM->registerRule(tsCallID, teHangOn, tsConnect, (void (__cdecl *const )(void *)) Connect);
 	m_pFSM->registerRule(tsCallID, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);

	m_pFSM->registerRule(tsConnect, teCode, tsConnect, (void (__cdecl *const )(void *)) Code);
	m_pFSM->registerRule(tsConnect, teCallID, tsConnect, (void (__cdecl *const )(void *)) CallID);
	m_pFSM->registerRule(tsConnect, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);

	m_pFSM->registerRule(tsConnect, teKey, tsConnect, (void (__cdecl *const )(void *)) SubKey);

	m_pFSM->registerRule(tsHangOn, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);
	m_pFSM->registerRule(tsHangOn, teKey, tsKey, (void (__cdecl *const )(void *)) Key);
	m_pFSM->registerRule(tsKey, teConnect, tsConnect, (void (__cdecl *const )(void *)) Connect);//拨打时，超时即为接通
	m_pFSM->registerRule(tsKey, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);
	m_pFSM->registerRule(tsKey, teKey, tsKey, (void (__cdecl *const )(void *)) Key);
		
	m_pFSM->setStartState(tsHangOff);
}

void CMultimediaPhoneDlg::HangOff(void* param)
{
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiTimer);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(FALSE);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOff_(param);
}

void CMultimediaPhoneDlg::HangOn(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(TRUE);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOn_(param);
}

void CMultimediaPhoneDlg::Key(void* param)
{
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiTimer);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiTimer = ::SetTimer((theApp.m_pMainWnd)->m_hWnd, 1, 10 * 1000, 0);
	
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Key_(param);
}

void CMultimediaPhoneDlg::SubKey(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->SubKey_(param);
}

void CMultimediaPhoneDlg::Connect(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Connect_(param);
}

void CMultimediaPhoneDlg::Ring(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(TRUE);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Ring_(param);
}

void CMultimediaPhoneDlg::Code(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Code_(param);
}

void CMultimediaPhoneDlg::CallID(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->CallID_(param);
}

void CMultimediaPhoneDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiTimer);
		m_pFSM->fireEvent(teConnect, 0);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CMultimediaPhoneDlg::OnMainSoftKey(WPARAM w, LPARAM l)
{
	/*
	afx_msg void OnButtonTime();
	afx_msg void OnButtonIME();
	afx_msg void OnButtonLine();
	afx_msg void OnButtonMain();
	afx_msg void OnButtonContact();
	afx_msg void OnButtonContactinfo();
	afx_msg void OnButtonSound();
	afx_msg void OnButtonInformation();
	afx_msg void OnButtonSetting();
	*/
	switch(w)
	{
	case 1:
		OnButtonTime();
		break;
	case 2:
		OnButtonIME();
		break;
	case 3:
		OnButtonLine();
		break;
	case 4:
		OnButtonMain();
		break;
	case 5:
		OnButtonContact();
		break;
	case 6:
		OnButtonContactinfo();
		break;
	case 7:
		OnButtonSound();
		break;
	case 8:
		OnButtonInformation();
		break;
	case 9:
		OnButtonSetting();
		break;
	}

}

void CMultimediaPhoneDlg::SetSkinStyle()
{
	m_StatusKey.m_pKeys[1].nBitmapId = Data::g_mainstatusIMEBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[2].nBitmapId = Data::g_mainstatusTELBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[3].nBitmapId = Data::g_mainstatusDesktopBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[4].nBitmapId = Data::g_mainstatusCardBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[5].nBitmapId = Data::g_mainstatusCalllistBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[6].nBitmapId = Data::g_mainstatusRecordBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[7].nBitmapId = Data::g_mainstatusInfoBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[8].nBitmapId = Data::g_mainstatusSettingBmpID[Data::g_skinstyle];

	m_StatusKey.m_BitmapToggleID = Data::g_mainstatusToggleBmpID[Data::g_skinstyle];
	m_StatusKey.m_BitmapID = Data::g_mainstatusMainBmpID[Data::g_skinstyle];
	m_StatusKey.m_clrBack = Data::g_mainstatusBackRGB[Data::g_skinstyle];
}