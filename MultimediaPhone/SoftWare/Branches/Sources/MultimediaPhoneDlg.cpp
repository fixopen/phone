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

#include "View/ContactDlg.h"
#include "View/InformationDlg.h"
#include "View/SettingDlg.h"
#include "View/ContactNewDlg.h"

#include "Data/LanguageResource.h"

//#define WM_PLAYVIDEO	8002
#define  KEYTIMEOUT  10 * 1000
#define  RINGTIMEOUT  8 * 1000
extern  BOOL DetectDIR(TCHAR *sDir);
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
	ON_MESSAGE(WM_GEN_EVENT, OnEvent)
	ON_MESSAGE(WM_DEVICECHANGE, OnDeviceChange)
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
	//检测SD ,USB
	m_bIsHaveUSB = DetectDIR(L"UsbDisk");
	m_bIsSD = DetectDIR(L"Storage Card");
//	SetTimer(IDT_DETECTUSB, 500, NULL);

	//init player
	//other wnd
	player_ = new Player(NULL);
	player_->SetPlayerOwner(this);

//	m_pPhone = new Logical::Phone(PickupFunc, HandsfreeFunc, HangupFunc, KeypadDialFunc, ConnectFunc, RingInFunc, RemoteHangupFunc, TimeoutFunc, ErrorFunc);
//	m_pPhone->start();
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
	softkeyval_[0].nAlign = IMAGE_LEFT;
	softkeyval_[0].bToggle = FALSE;

	softkeyval_[1].sKeyRect = CRect(44, 9, 59, 24);
	softkeyval_[1].nKeyValue = 2;
	softkeyval_[1].nTextId = 0;
	softkeyval_[1].nBitmapId = IDB_BITMAP_MAININPUT;
	softkeyval_[1].nAlign = IMAGE_LEFT;
	softkeyval_[1].bToggle = FALSE;

	softkeyval_[2].sKeyRect = CRect(63, 9, 78, 24);
	softkeyval_[2].nKeyValue = 3;
	softkeyval_[2].nTextId = 0;
	softkeyval_[2].nBitmapId = IDB_BITMAP_MAINTEL;
	softkeyval_[2].nAlign = IMAGE_LEFT;
	softkeyval_[2].bToggle = FALSE;

	softkeyval_[3].sKeyRect = CRect(102, 4, 166, 28);
	softkeyval_[3].nKeyValue = 4;
	softkeyval_[3].nTextId = Data::RI_MAIN_MAINBTN;
	softkeyval_[3].nBitmapId = IDB_BITMAP_MAINDESKTOP;
	softkeyval_[3].nAlign = IMAGE_LEFT;
	softkeyval_[3].bToggle = TRUE;

	softkeyval_[4].sKeyRect = CRect(169, 4, 229+1, 28);
	softkeyval_[4].nKeyValue = 5;
	softkeyval_[4].nTextId = Data::RI_MAIN_CARDCASEBTN;
	softkeyval_[4].nBitmapId = IDB_BITMAP_MAINCARD;
	softkeyval_[4].nAlign = IMAGE_LEFT;
	softkeyval_[4].bToggle = TRUE;

	softkeyval_[5].sKeyRect = CRect(232, 4, 292+1, 28);
	softkeyval_[5].nKeyValue = 6;
	softkeyval_[5].nTextId = Data::RI_MAIN_CALLLISTBTN;
	softkeyval_[5].nBitmapId = IDB_BITMAP_MAINCALLLIST;
	softkeyval_[5].nAlign = IMAGE_LEFT;
	softkeyval_[5].bToggle = TRUE;

	softkeyval_[6].sKeyRect = CRect(295, 4, 355+1, 28);
	softkeyval_[6].nKeyValue = 7;
	softkeyval_[6].nTextId = Data::RI_MAIN_SOUND;
	softkeyval_[6].nBitmapId = IDB_BITMAP_MAINRECORD;
	softkeyval_[6].nAlign = IMAGE_LEFT;
	softkeyval_[6].bToggle = TRUE;

	softkeyval_[7].sKeyRect = CRect(358, 4, 416+3, 28);
	softkeyval_[7].nKeyValue = 8;
	softkeyval_[7].nTextId = Data::RI_MAIN_STOCKINFOBTN;
	softkeyval_[7].nBitmapId = IDB_BITMAP_MAININFO;
	softkeyval_[7].nAlign = IMAGE_LEFT;
	softkeyval_[7].bToggle = TRUE;

	softkeyval_[8].sKeyRect = CRect(421, 4, 479, 28);
	softkeyval_[8].nKeyValue = 9;
	softkeyval_[8].nTextId = Data::RI_MAIN_SYSTTEMSETTING;
	softkeyval_[8].nBitmapId = IDB_BITMAP_MAINSETTING;
	softkeyval_[8].nAlign = IMAGE_LEFT;
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
	CDialog* panel; 
	panel = new CInformationDlg();
	panel->Create(CInformationDlg::IDD);
	panels_[IDC_BUTTON_INFORMATION] = panel;

	m_pSettingDlg = new CSettingDlg();
	panel = m_pSettingDlg;
	panel->Create(CSettingDlg::IDD);
	panels_[IDC_BUTTON_SETTING] = panel;
	
	m_pTelephoneDlg = new CTelephoneDlg();
	m_pTelephoneDlg->Create(CTelephoneDlg::IDD);

	m_pMainDlg = new CMainDlg();
	panel = m_pMainDlg;
	panel->Create(CMainDlg::IDD);
	panels_[IDC_BUTTON_MAIN] = panel;
	currentPanel_ = panel;

	panel = new CContactDlg();
	panel->Create(CContactDlg::IDD);
	panels_[IDC_BUTTON_CONTACT] = panel;

	m_pContactInfoDlg = new CContactInfoDlg();
	panel = m_pContactInfoDlg;
	panel->Create(CContactInfoDlg::IDD);
	panels_[IDC_BUTTON_CONTACTINFO] = panel;

	m_pSoundDlg = new CSoundDlg();
	panel = m_pSoundDlg;
	panel->Create(CSoundDlg::IDD);
	panels_[IDC_BUTTON_SOUND] = panel;

	//test CEDlg
/*	m_pTestCEDlg = new CTestCEDlg();
	panel = m_pTestCEDlg;
	panel->Create(CTestCEDlg::IDD);
	panels_[IDC_BUTTON_INFORMATION] = panel;
*/
	m_pContactNewDlg = new CContactNewDlg(panels_[IDC_BUTTON_CONTACT]);
	panel = m_pContactNewDlg;
	panel->Create(CContactNewDlg::IDD);
	panels_[IDC_BUTTON_CONTACTNEW] = panel;

	m_pContactGroupDlg = new CContactGroupDlg(panels_[IDC_BUTTON_CONTACT]);
	panel = m_pContactGroupDlg;
	panel->Create(CContactGroupDlg::IDD);
	panels_[IDC_BUTTON_CONTACTGROUP] = panel;


	m_pDeleteTipDlg = new CDeleteTipDlg(this);
	m_pDeleteTipDlg->Create(CDeleteTipDlg::IDD);

	m_pNotebookDlg = new CNotebookDlg(this);
	m_pNotebookDlg->Create(CNotebookDlg::IDD);

	m_mainLunarderDlg1_ = new CLunarderDlg(panels_[IDC_BUTTON_MAINNOTE]);
	panel = m_mainLunarderDlg1_;
	panel->Create(CContactGroupDlg::IDD);
	panels_[IDC_BUTTON_MAINNOTE] = panel;

/*
	buttons_[IDC_BUTTON_TIME] = &m_btnTime;
	buttons_[IDC_BUTTON_IME] = &m_btnIME;s
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
	m_uiKeyTimer = 1;
	m_uiRingTimer = 2;
	
	//设置进入屏保
	SetScreenSaveTimer();
	
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
	//lxz 20071218 stocks
	//if(panelId == IDC_BUTTON_CONTACT || panelId == IDC_BUTTON_CONTACT || panelId == IDC_BUTTON_CONTACTINFO || panelId == IDC_BUTTON_SOUND)
	//	return;
	
	//关闭输入法
	if(theApp.inputDlg_->IsWindowVisible())
	{
		theApp.inputDlg_->hide();
	}
	
	if(panelId == 0)
	{
		m_pMainDlg->m_mainVideoDlg_->OnExit_();
		if(m_pMainDlg->m_mainLunarderDlg_->IsWindowVisible())
			m_pMainDlg->m_mainLunarderDlg_->OnExit_();
		SwitchPanel_(IDC_BUTTON_MAIN);
//		SwitchPanel_(oldcurrentPanelID_);
		return;
	}
/*
	oldcurrentPanelID_ = currentPanelID_;
	currentPanelID_ = panelId;
*/
	
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
		if(currentPanel_ == panels_[IDC_BUTTON_MAIN])
			((CMainDlg *)currentPanel_)->ShowWindow_(SW_HIDE);
		else if(currentPanel_ == panels_[IDC_BUTTON_INFORMATION])    //20071220 stocks
			((CInformationDlg*)currentPanel_)->ShowWindow_(SW_HIDE);
		else
			currentPanel_->ShowWindow(SW_HIDE);
		currentPanel_ = panels_[panelId];

		if(currentPanel_ == panels_[IDC_BUTTON_MAIN])
			((CMainDlg *)currentPanel_)->ShowWindow_(SW_SHOW);
		else if(currentPanel_ == panels_[IDC_BUTTON_INFORMATION])   //20071220 stocks
			((CInformationDlg*)currentPanel_)->ShowWindow_(SW_SHOW);
		else
			currentPanel_->ShowWindow(SW_SHOW);
	
		if (panelId == IDC_BUTTON_MAIN)
		{
			m_StatusKey.SetToggleKey(3);
		}
		if (panelId == IDC_BUTTON_CONTACT)
		{
			m_StatusKey.SetToggleKey(4);
		}
		if (panelId == IDC_BUTTON_CONTACTINFO)
		{
			m_StatusKey.SetToggleKey(5);
		}
		if (panelId == IDC_BUTTON_INFORMATION)
		{
			m_StatusKey.SetToggleKey(7);
		}
	}
	else if(currentPanel_ == panels_[IDC_BUTTON_MAIN])  //多次点击桌面
	{
		m_pMainDlg->SetMainMenu();
	}

	extern BOOL m_bIsPausebyEvent;
	if(currentPanel_ == panels_[IDC_BUTTON_MAIN] && m_bIsPausebyEvent /*&& player_->type_*/)	//恢复播放消息
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
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->ClearUnconnectCount();
}

void CMultimediaPhoneDlg::OnButtonSound() 
{
	// TODO: Add your control notification handler code here
	SwitchPanel_(IDC_BUTTON_SOUND);	
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->ClearRecordCount();
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
		SendOutEvnet(WM_TEL_RING, 0);
		printf("RING \n");
		m_pFSM->fireEvent(teRing, (void * const)wParam);
		break;
	case WM_TEL_HUNGOFF:
		SendOutEvnet(WM_TEL_HUNGOFF, 1);
		printf("HUNG OFF \n");
		m_pFSM->fireEvent(teHangOff, (void * const)wParam);
		break;
	case WM_TEL_HUNGON:
		printf("HUNG ON \n");
		m_pFSM->fireEvent(teHangOn, (void * const)wParam);
		SendOutEvnet(WM_TEL_HUNGON, 0);
		break;
	case WM_TEL_CALLIDEND:
		printf("CALL ID \n");
		m_pFSM->fireEvent(teCallID, (void * const)wParam);
		break;
	case WM_TEL_KEYCODE:
		SendOutEvnet(WM_TEL_KEYCODE, 0);
		printf("%x ", wParam);
		m_pFSM->fireEvent(teKey, (void * const)wParam);
		break;
	case WM_REDIAL:
		printf("REDIAL \n");
		SendOutEvnet(WM_TEL_HUNGON, 0);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Redial();
		break;
	case WM_MUTE:
		printf("MUTE \n");
//		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Mute();
		break;
	case WM_RKEY:
		printf("R KEY \n");
		break;
	case WM_SPEEDDIAL:
		{
			char c = wParam;
			if (m_pFSM->getCurrentState() == tsHangOn)
			{
				std::map<char, std::string> m = m_pSettingDlg->m_pSetting->speedDials();
				if (m[c] != "")
				{
					m_pTelephoneDlg->DialContact(Data::TelephoneNumber(m[c]));
				}
			}

		}
		break;
	case WM_HUNGONINRECORD:
		printf("HUNG ON IN RECORD \n");
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOnInRecord();
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

	m_pFSM->registerRule(tsHangOff, teCallID, tsRing, (void (__cdecl *const )(void *)) CallID);

	m_pFSM->registerRule(tsRing, teHangOn, tsConnect, (void (__cdecl *const )(void *)) Connect);
	m_pFSM->registerRule(tsRing, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);
	m_pFSM->registerRule(tsRing, teRing, tsRing, (void (__cdecl *const )(void *)) Ring);
	m_pFSM->registerRule(tsRing, teCallID, tsRing, (void (__cdecl *const )(void *)) CallID);	
//  m_pFSM->registerRule(tsRing, teCallID, tsCallID, (void (__cdecl *const )(void *)) CallID);	
// 
//  m_pFSM->registerRule(tsCallID, teHangOn, tsConnect, (void (__cdecl *const )(void *)) Connect);
//  m_pFSM->registerRule(tsCallID, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);
// 	m_pFSM->registerRule(tsCallID, teRing, tsCallID, (void (__cdecl *const )(void *)) Ring);

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
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(FALSE);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOff_(param);

	//未接接听，自动关闭窗口时模拟挂机事件
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->SendOutEvnet(WM_TEL_HUNGOFF, 1);
}

void CMultimediaPhoneDlg::HangOn(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(TRUE);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOn_(param);
}

void CMultimediaPhoneDlg::Key(void* param)
{
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer);
	::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer, KEYTIMEOUT, 0);
	
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
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer);
	::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer, RINGTIMEOUT, 0);

	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(TRUE);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Ring_(param);
}

void CMultimediaPhoneDlg::CallID(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->CallID_(param);
}

void CMultimediaPhoneDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer)
	{
		::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer);
		m_pFSM->fireEvent(teConnect, 0);
	}
	else if (nIDEvent == ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer)
	{
		::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer);
// 		if ((m_pFSM->getCurrentState() == tsCallID) || (m_pFSM->getCurrentState() == tsRing))
		if (m_pFSM->getCurrentState() == tsRing)
		{
			m_pFSM->fireEvent(teHangOff, 0);
		}
	}
	else if(nIDEvent == IDT_MAIN_SAVESCREEN_TIME)		//进入屏保
	{
		KillTimer(IDT_MAIN_SAVESCREEN_TIME);
		CMainDlg *pMainDlg = (CMainDlg *)panels_[IDC_BUTTON_MAIN];
		int type = (int)m_pSettingDlg->m_pSetting->screenSaverContent();
		pMainDlg->m_mainScreenSaveDlg_->SetSaveScreenType(type, TRUE);	  //1 图片屏保   0  时间屏保
		pMainDlg->SendMessage(WM_CHANGEWINDOW, (WPARAM)5, SW_SHOW);   //切换到屏保
	}
	/* 改成消息检测
	else if(nIDEvent == IDT_DETECTUSB)	//检测SD, USB
	{
		BOOL bIsHaveUSB = DetectDIR(L"UsbDisk");
		BOOL bIsSD = DetectDIR(L"Storage Card");
		int flag = 0;
		if(bIsHaveUSB != m_bIsHaveUSB)
		{
			if(bIsHaveUSB)
				flag = 0x8000;
			else 
				flag = 0x8004;
			m_bIsHaveUSB = bIsHaveUSB;
		}

		else if( bIsSD != m_bIsSD)
		{
			if(bIsSD)
				flag = 0x8000;
			else 
				flag = 0x8004;
			m_bIsSD = bIsSD;
		}
		if(flag != 0)
		{
			m_pMainDlg->m_mainPhotoDlg_->SendMessage(WM_DEVICECHANGE, flag, 0);
			m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_DEVICECHANGE, flag, 0);
			m_pMainDlg->m_mainVideoDlg_->SendMessage(WM_DEVICECHANGE, flag, 0);
		}
	}
	*/
	CDialog::OnTimer(nIDEvent);
}

void CMultimediaPhoneDlg::OnMainSoftKey(WPARAM w, LPARAM l)
{
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

//振铃 摘机 按键等外部事情发送给 屏保 MP3 VIDEO 听留言窗口
void CMultimediaPhoneDlg::SendOutEvnet(WPARAM w, LPARAM l)
{
	SendMessage(WM_GEN_EVENT);
	CMainDlg *pWnd = (CMainDlg *)GetPanel(IDC_BUTTON_MAIN);
	BOOL isShow = pWnd->m_mainScreenSaveDlg_->IsWindowVisible();
	switch(w)
	{
		case WM_TEL_KEYCODE:
			{
				pWnd->m_mainScreenSaveDlg_->SendMessage(WM_OUTEVENT, w, isShow);
			}
			break;
		case WM_TEL_RING:
		case WM_TEL_HUNGON:
			{
				pWnd->m_mainScreenSaveDlg_->SendMessage(WM_OUTEVENT, w, isShow);
				pWnd->m_mainVideoDlg_->SendMessage(WM_OUTEVENT, w, l);
				pWnd->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, w, l);
			}
			break;
		case WM_TEL_HUNGOFF:
			{
				pWnd->m_mainVideoDlg_->SendMessage(WM_OUTEVENT, w, l);
				pWnd->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, w, l);
				
			}
			break;
		case WM_TOUCH_CLICK:
			{
	//			pWnd->m_mainScreenSaveDlg_->SendMessage(WM_OUTEVENT, w, isShow);
			}
			break;
	}
}

void CMultimediaPhoneDlg::OnEvent(WPARAM w, LPARAM l)
{
	if(w == 1)
		SendOutEvnet(WM_TOUCH_CLICK, 0);
	SetScreenSaveTimer();
}

void CMultimediaPhoneDlg::SetScreenSaveTimer()
{
	CMainDlg *pWnd = (CMainDlg *)GetPanel(IDC_BUTTON_MAIN);
	HWND hWnd = ::FindWindow(L"csplayer_win", L"csplayer window");
	
	KillTimer(IDT_MAIN_SAVESCREEN_TIME);
	BOOL flag1 = m_pSettingDlg->m_pSetting->isUseScreenSaver();
	BOOL flag2 = ::IsWindowVisible(hWnd);
	BOOL flag3 = pWnd->m_mainVideoDlg_->IsWindowVisible();
	BOOL flag4 = FALSE;
	if(m_pFSM->getCurrentState() != tsHangOff)
		flag4 = TRUE;
	if(flag1)
	{
		CTimeSpan nTimer = m_pSettingDlg->m_pSetting->screenSaverDuration();
		int nSecond = nTimer.GetTotalSeconds();
		if(flag2 && flag3)    //video
		{
			Dprintf("UnSet ScreenSave Timer \r\n");
		}
		else if(flag4)  //电话正在使用
		{
			Dprintf("UnSet ScreenSave Timer \r\n");
		}
		else
		{
			SetTimer(IDT_MAIN_SAVESCREEN_TIME, nSecond*1000, NULL);
			static int gTimerCount = 0;
			Dprintf("Set ScreenSave Timer = %d \r\n", gTimerCount++);
		}
	}
	else
	{
		Dprintf("UnSet ScreenSave Timer \r\n");
	}
}

void CMultimediaPhoneDlg::KillScreenSaveTimer()
{
	KillTimer(IDT_MAIN_SAVESCREEN_TIME);
	Dprintf("Kill ScreenSave Timer \r\n");
}

void CMultimediaPhoneDlg::OnDeviceChange(WPARAM w, LPARAM l)
{
	m_pMainDlg->m_mainPhotoDlg_->SendMessage(WM_USB_MSG, w, 0);
	m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_USB_MSG, w, 0);
	m_pMainDlg->m_mainVideoDlg_->SendMessage(WM_USB_MSG, w, 0);
}

void PickupFunc()
{
	printf("\nPickupFunc\n");
}
void HandsfreeFunc()
{
	printf("\nHandsfreeFunc\n");
}
void HangupFunc()
{
	printf("\nHangupFunc\n");
}
void KeypadDialFunc(unsigned char const& keyCode)
{
	printf("\nKeypadDialFunc=%d\n", keyCode);
}
void ConnectFunc()
{
	printf("\nConnectFunc\n");
}
void RingInFunc(Data::TelephoneNumber number)
{
	printf("\nRingInFunc=%s\n", number.number().c_str());
}
void RemoteHangupFunc()
{
	printf("\nRemoteHangupFunc\n");
}
void TimeoutFunc()
{
	printf("\nTimeoutFunc\n");
}
void ErrorFunc()
{
	printf("\nErrorFunc\n");
}