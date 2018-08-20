// SettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "SettingDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "FireWallDlg.h"
#include "../MultimediaPhoneDlg.h"

//#include "Calibrui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog
CString s_VerSion = "\
0.8.071225.3\r\n\
\t1.添加股票功能.测试用户名:2945011密码:123456\r\n\
\t2.将便笺功能从下面的状态栏变成了股票.\r\n\
\t3.便笺功能这个版本只能从通话记录中进入.\r\n\
0.8.071220.1\r\n\
\t未完成功能及版本缺陷：\r\n\
\t1.无手写输入\r\n\
\t2.无自动答录提示语和提示音\r\n\
\t3.日期时钟不正确\r\n\
\t4.手写校正未保存\r\n\
\t5.手写笔迹不圆滑\r\n";

extern BOOL DetectDIR(TCHAR *sDir);

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
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_LIST_SETTING_TYPE, OnClickListType)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_OK, OnButtonSettingOk)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_CANCEL, OnButtonSettingCancel)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_DEFAULT, OnButtonSettingDefault)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_RECORD, OnButtonSettingRecord)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_DEFINEFASTDAIL,OnButtonSettingDefineFastDial)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_ADJUSTTOUCH, OnAdjustTouchPanel)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_SETPASSWORD, OnSettingSaveScreenPassword)

	ON_BN_CLICKED(IDC_BUTTON_SETTING_CLEARCONTACT, OnSettingClearContact)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_CLEARCONTACTINFO, OnSettingClearContactInfo)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_CLEARSOUND, OnSettingClearSound)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_CLEARINFO, OnSettingClearInfo)

	ON_BN_CLICKED(IDC_SETTING_BACKUP, OnBackup)
	ON_BN_CLICKED(IDC_SETTING_RESTORE, OnRestore)
	ON_BN_CLICKED(IDC_SETTING_OUT, OnFileOut)
	ON_BN_CLICKED(IDC_SETTING_RINGSELECT, OnRingSelect)
	ON_NOTIFY(NM_CLICK, IDC_SETTING_LSTRING, OnRingLst)

	ON_MESSAGE(WM_SETTINGPASSWORD, OnSetSaveScreenPassWordOK)
	ON_MESSAGE(WM_STATIC_CLICK, OnStaticClick)

	/*
	ON_BN_CLICKED(IDC_SETTING_DELETE, OnDeleteFile)
	ON_BN_CLICKED(IDC_SETTING_ALLSELECT, OnAllSelect)
	ON_BN_CLICKED(IDC_SETTING_CLEARSELECT, OnClearDelect)
	ON_BN_CLICKED(IDC_SETTING_IN, OnFileIn)
	ON_BN_CLICKED(IDC_SETTING_OUT, OnFileOut)
	ON_MESSAGE(WM_COMMBOX_CLICKED, OnFileSelect)
	ON_NOTIFY(NM_CLICK, IDC_SETTING_LSTLOCAL, OnLocalLst)
	ON_NOTIFY(NM_CLICK, IDC_SETTING_LSTUSB, OnUSBLst)
	*/
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg message handlers

BOOL CSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	std::string strTemp = Data::LanguageResource::Get(Data::RI_RECORD_TYPE);
	CString str = strTemp.c_str();
	/*
	m_sticTypeTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0, 0, 69, 20), this);
	m_sticTypeTitle.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticTypeTitle.SetLeftMargin(25);

	m_sticSepLine1.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(69, 0, 70, 204), this);
	m_sticSepLine1.SetColor(RGB(78, 165, 229), RGB(255, 255, 255));
	m_sticSepLine2.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(429, 0, 430, 204), this);
	m_sticSepLine2.SetColor(RGB(255, 255, 255), RGB(255, 255, 255));

	std::string strTemp;
	CString str;
// 	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_TYPE);
// 	str = strTemp.c_str();
// 	m_sticTypeTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0, 0, 69, 20), this);
// 	m_sticTypeTitle.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
// 	m_sticTypeTitle.SetLeftMargin(25);
// 
// 	m_sticSepLine1.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(69, 0, 70, 204), this);
// 	m_sticSepLine1.SetColor(RGB(78, 165, 229), RGB(255, 255, 255));
// 	m_sticSepLine2.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(429, 0, 430, 204), this);
// 	m_sticSepLine2.SetColor(RGB(255, 255, 255), RGB(255, 255, 255));
	*/
	m_lsType.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(8, 27, 80, 198), this, IDC_LIST_SETTING_TYPE, FALSE);
	m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsType.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 80);
	
	m_pImageList = new CImageList();
	m_pImageList->Create(16, 16, ILC_COLOR32|ILC_MASK, 4, 2);   
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
	bm.LoadBitmap(IDB_BITMAP_SETTINGINOUT);
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

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_IN);
	str = strTemp.c_str();
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_OUT);
	str += strTemp.c_str();
	m_lsType.InsertItem(3, str, 4);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_INFORMATION);
	str = strTemp.c_str();
	m_lsType.InsertItem(4, str, 3);

	
// 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DETAILTITLE);
// 	str = strTemp.c_str();
// 	m_sticDetailTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(70, 0, 429, 20), this);
// 	m_sticDetailTitle.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
// 	m_sticDetailTitle.SetLeftMargin(25);

// 	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_OPERATION);
// 	str = strTemp.c_str();
// 	m_sticOperation.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 0, 480, 20), this);
// 	m_sticOperation.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
// 	m_sticOperation.SetLeftMargin(10);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 26, 471, 45), this, IDC_BUTTON_SETTING_OK);
	m_btnOk.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnCancel.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 178, 471, 198), this, IDC_BUTTON_SETTING_CANCEL);
	m_btnCancel.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DEFAULT);
	str = strTemp.c_str();
	m_btnDefault.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 46, 471, 65), this, IDC_BUTTON_SETTING_DEFAULT);
	m_btnDefault.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

	//tel======================================================================================================
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_AREACODE);
	str = strTemp.c_str();
	m_sticLocalAreaNumber.Create(str, WS_CHILD|WS_VISIBLE, CRect(100, 28, 186, 44), this);
	m_sticLocalAreaNumber.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_edtLocalAreaNumber.Create(WS_CHILD|WS_VISIBLE, CRect(190, 28, 340, 44), this, IDC_EDIT_SETTING_AREANO);
	m_edtLocalAreaNumber.SetLimitText(15);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_OUTLINE);
	str = strTemp.c_str();
	m_chbEnableOutLine.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(85, 46, 186, 62), this, IDC_CHECK_SETTING_OUTLINE);
	m_chbEnableOutLine.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	SetButtonDefaultColor(&m_chbEnableOutLine);	
	m_edtOutLine.Create(WS_CHILD|WS_VISIBLE, CRect(190, 46, 340, 62), this, IDC_EDIT_SETTING_OUTLINE);
	m_edtOutLine.SetLimitText(15);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_IPDIAL);
	str = strTemp.c_str();
	m_chbEnableIPDial.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(85, 64, 186, 80), this, IDC_CHECK_SETTING_IPDIAL);
	m_chbEnableIPDial.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	SetButtonDefaultColor(&m_chbEnableIPDial);	
	m_edtIPDial.Create(WS_CHILD|WS_VISIBLE, CRect(190, 64, 340, 80), this, IDC_EDIT_SETTING_IPDIAL);
	m_edtIPDial.SetLimitText(15);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLERING);
	str = strTemp.c_str();
	m_chbEnableRing.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(85, 86, 186, 102), this, IDC_CHECK_SETTING_RING);
	m_chbEnableRing.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	SetButtonDefaultColor(&m_chbEnableRing);	
	
	m_ringEdit.Create(WS_CHILD|WS_VISIBLE, CRect(190,86,340,102), this, 0xFFFF);
	m_ringEdit.SetIsAutoInput(FALSE);
	m_ringButton.Create(L"", Data::g_comboxBMPID[0][Data::g_skinstyle], Data::g_comboxBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(340,86,356,102), this, IDC_SETTING_RINGSELECT);
	m_ringList.Create(WS_CHILD|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(190,104,356,200), this, IDC_SETTING_LSTRING, TRUE, 1);
	m_ringList.SetListColor(Data::g_listctrlBackRGB1[Data::g_skinstyle], Data::g_listctrlBackRGB2[Data::g_skinstyle]);
	m_ringList.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 138);
	m_ringList.ShowWindow_(SW_HIDE);

	m_pImageList1 = new CImageList();
	m_pImageList1->Create(12, 12, ILC_COLOR32|ILC_MASK, 6, 6);   
  	
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[1][Data::g_skinstyle]);
	m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[0][Data::g_skinstyle]);
	m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[2][Data::g_skinstyle]);
	m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[3][Data::g_skinstyle]);
	m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[4][Data::g_skinstyle]);
	m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[5][Data::g_skinstyle]);
	m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	m_ringList.SetImageList(m_pImageList1, LVSIL_SMALL);
	
	m_ringStatic.Create(L"", WS_CHILD, CRect(189,121,349,197), this);
	m_ringStatic.SetBorder(TRUE);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLERINGSTC);
	str = strTemp.c_str();
	m_sticDefaultRing.Create(str, WS_CHILD|WS_VISIBLE, CRect(100, 104, 186, 120), this);
	m_sticDefaultRing.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	m_cmbRing.Create(WS_CHILD|WS_VISIBLE, CRect(190,104,260,200), this, IDC_COMBOBOX_SETTING_RING);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLEAUTOLEAVEWORD);
	str = strTemp.c_str();
	m_chbEnableAutoLeaveWord.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(85, 126, 210, 142), this, IDC_CHECK_SETTING_AUTOLEAVEWORD);
	m_chbEnableAutoLeaveWord.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	SetButtonDefaultColor(&m_chbEnableAutoLeaveWord);	
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RINGTIMES);
	str = strTemp.c_str();
	m_sticRingTimes.Create(str, WS_CHILD|WS_VISIBLE, CRect(100, 144, 186, 160), this);
	m_sticRingTimes.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	m_cmbRingTimes.Create(WS_CHILD|WS_VISIBLE, CRect(190,144,260,200), this, IDC_COMBOBOX_SETTING_RINGTIMES);

// 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LEAVEWORD);
// 	str = strTemp.c_str();
// 	m_sticAutoLeaveWord.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 165, 186, 180), this);
// 	m_sticAutoLeaveWord.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
// 	m_cmbAutoLeaveWordTip.Create(WS_CHILD|WS_VISIBLE, CRect(190,162,350,230), this, IDC_COMBOBOX_SETTING_AUTOLEAVEWORD);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RECORDLEAVEWORD);
	str = strTemp.c_str();
	m_btnRecardLeaveWordTip.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(270, 142, 326, 162), this, IDC_BUTTON_SETTING_RECORD);
	m_btnRecardLeaveWordTip.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RECORDLEAVEWORD);
	str = strTemp.c_str();
	m_btnPlayLeaveWordTip.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD/*|WS_VISIBLE*/, CRect(334, 142, 390, 162), this, IDC_BUTTON_SETTING_PLAY);
	m_btnPlayLeaveWordTip.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);


	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DEFINESPEEDBUTTON);
	str = strTemp.c_str();
	m_btnDefineSpeedButton.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(100, 164, 156, 184), this, IDC_BUTTON_SETTING_DEFINEFASTDAIL);
	m_btnDefineSpeedButton.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DEFINESPEEDBUTTONSTC);
	str = strTemp.c_str();
	m_sticDefineSpeedButton.Create(str, WS_CHILD|WS_VISIBLE, CRect(160, 166, 410, 182), this);
	m_sticDefineSpeedButton.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	//browser======================================================================================================
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CONNECTAUTO);
	str = strTemp.c_str();
	m_rdoConnectAuto.Create(str, WS_CHILD|WS_VISIBLE/*|BS_RADIOBUTTON*/, CRect(85, 28, 210, 44), this, IDC_RADIO_SETTING_CONNECTAUTO);
	m_rdoConnectAuto.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CONNECTMANUAL);
	str = strTemp.c_str();
	m_rdoConnectADSL.Create(str, WS_CHILD|WS_VISIBLE/*|BS_RADIOBUTTON*/, CRect(85, 46, 210, 62), this, IDC_RADIO_SETTINT_CONNECTADSL);
	m_rdoConnectADSL.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	CButton *pButton[2];
	pButton[0] = &m_rdoConnectAuto;
	pButton[1] = &m_rdoConnectADSL;

	m_rdoConnectAuto.SetGroupButton(pButton, 2);
	m_rdoConnectADSL.SetGroupButton(pButton, 2);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ADSLNAME);
	str = strTemp.c_str();
	m_sticADSLName.Create(str, WS_CHILD|WS_VISIBLE, CRect(105, 64, 170, 80), this);
	m_sticADSLName.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	m_edtADSLName.Create(WS_CHILD|WS_VISIBLE, CRect(174, 64, 300, 82), this, IDC_EDIT_SETTING_ADSLNAME);
	m_edtADSLName.SetLimitText(15);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ADSLPASSWORD);
	str = strTemp.c_str();
	m_sticADSLPassword.Create(str, WS_CHILD|WS_VISIBLE, CRect(304, 64, 340, 80), this);
	m_sticADSLPassword.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	m_edtADSLPassword.Create(WS_CHILD|WS_VISIBLE, CRect(344, 64, 410, 82), this, IDC_EDIT_SETTING_ADSLPASSWORD);
	m_edtADSLPassword.SetLimitText(15);
	

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_USEDYNAMICIP);
	str = strTemp.c_str();
	m_rdoGetIPDHCP.Create(str, WS_CHILD|WS_VISIBLE/*|BS_RADIOBUTTON*/, CRect(85, 86, 210, 102), this, IDC_RADIO_SETTING_DHCP);
	m_rdoGetIPDHCP.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_USESTATICIIP);
	str = strTemp.c_str();
	m_rdoGetIPStatic.Create(str, WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON, CRect(85, 104, 210, 120), this, IDC_RADIO_SETTING_STATIC);
	m_rdoGetIPStatic.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	pButton[0] = &m_rdoGetIPDHCP;
	pButton[1] = &m_rdoGetIPStatic;
	
	m_rdoGetIPDHCP.SetGroupButton(pButton, 2);
	m_rdoGetIPStatic.SetGroupButton(pButton, 2);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_IP);
	str = strTemp.c_str();
	m_sticIP.Create(str, WS_CHILD|WS_VISIBLE, CRect(105, 122, 170, 138), this);
	m_sticIP.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_MASK);
	str = strTemp.c_str();
	m_sticMask.Create(str, WS_CHILD|WS_VISIBLE, CRect(105, 140, 170, 156), this);
	m_sticMask.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_GATEWAY);
	str = strTemp.c_str();
	m_sticGateway.Create(str, WS_CHILD|WS_VISIBLE, CRect(105, 158, 170, 174), this);
	m_sticGateway.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DNS);
	str = strTemp.c_str();
	m_sticDNS.Create(str, WS_CHILD|WS_VISIBLE, CRect(105, 176, 170, 192), this);
	m_sticDNS.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	for(int i = 0; i < 4; i++)
	{
		m_edtIP[i].Create(WS_CHILD|WS_VISIBLE, CRect(172+i*36+4, 122, 172+(i+1)*36, 138), this, IDC_EDIT_SETTING_IP);
		m_edtIP[i].SetLimitText(3);
		m_edtMask[i].Create(WS_CHILD|WS_VISIBLE, CRect(172+i*36+4, 140, 172+(i+1)*36, 156), this, IDC_EDIT_SETTING_MASK);
		m_edtMask[i].SetLimitText(3);
		m_edtGateway[i].Create(WS_CHILD|WS_VISIBLE, CRect(172+i*36+4, 158, 172+(i+1)*36, 174), this, IDC_EDIT_SETTING_GATEWAY);
		m_edtGateway[i].SetLimitText(3);
		m_edtDNS[i].Create(WS_CHILD|WS_VISIBLE, CRect(172+i*36+4, 176, 172+(i+1)*36, 192), this, IDC_EDIT_SETTING_DNS);
		m_edtDNS[i].SetLimitText(3);
	}
	

	//systom======================================================================================================
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLESP);
	str = strTemp.c_str();
	m_chbEnnabeScreenProtect.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(85, 28, 210, 44), this, IDC_CHECK_SETTING_SCREENPROTECT);
	m_chbEnnabeScreenProtect.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	SetButtonDefaultColor(&m_chbEnnabeScreenProtect);	

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SPTYPE);
	str = strTemp.c_str();
	m_sticScreenProtctType.Create(str, WS_CHILD|WS_VISIBLE, CRect(100, 46, 170, 62), this);
	m_sticScreenProtctType.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TIMESP);
	str = strTemp.c_str();
	m_rdoTimeScreen.Create(str, WS_CHILD|WS_VISIBLE/*|BS_CHECKBOX*/, CRect(174, 46, 250, 62), this, IDC_RADIO_SETTING_TIMESCREEN);
	m_rdoTimeScreen.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_IMAGESP);
	str = strTemp.c_str();
	m_rdoImageScreen.Create(str, WS_CHILD|WS_VISIBLE/*|BS_CHECKBOX*/, CRect(254, 46, 360, 62), this, IDC_RADIO_SETTING_IMAGESCREEN);
	m_rdoImageScreen.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	pButton[0] = &m_rdoTimeScreen;
	pButton[1] = &m_rdoImageScreen;
	
	m_rdoTimeScreen.SetGroupButton(pButton, 2);
	m_rdoImageScreen.SetGroupButton(pButton, 2);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_WAITTIME);
	str = strTemp.c_str();
	m_sticWaitTime.Create(str, WS_CHILD|WS_VISIBLE, CRect(100, 64, 170, 80), this);
	m_sticWaitTime.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	m_cmbWaitTime.Create(WS_CHILD|WS_VISIBLE, CRect(174, 64, 250, 170), this, IDC_COMBOBOX_SETTING_WAITTIME);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLEPASSWORD);
	str = strTemp.c_str();
	m_chbEnablePassword.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(100, 84, 240, 100), this, IDC_CHECK_SETTING_SPPASSWORD);
	m_chbEnablePassword.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	SetButtonDefaultColor(&m_chbEnablePassword);	
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SETPASSWORD);
	str = strTemp.c_str();
	m_btnSetPassword.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(244, 82, 300, 102), this, IDC_BUTTON_SETTING_SETPASSWORD);
	m_btnSetPassword.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	//录音文件存储位置
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SOUNDSAVEPATH);
	str = strTemp.c_str();
	m_sticSoundSavePath.Create(str, WS_CHILD|WS_VISIBLE, CRect(86, 106, 170, 122), this);
	m_sticSoundSavePath.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	m_cmbSoundSavePath.Create(WS_CHILD|WS_VISIBLE, CRect(174, 106, 250, 170), this, IDC_COMBOBOX_SETTING_SOUNDSAVEPATH);

	//时间设置
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SYSTEMTIME);
	str = strTemp.c_str();
	m_stcTime.Create(str, WS_CHILD|WS_VISIBLE, CRect(86, 128, 170, 146), this);
	m_stcTime.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_dtDate.Create(WS_VISIBLE|WS_CHILD|DTS_SHORTDATEFORMAT, CRect(174, 128, 274, 146), this, IDC_SETTING_DATE);
	m_dtTime.Create(WS_VISIBLE|WS_CHILD|DTS_TIMEFORMAT, CRect(280, 128, 374, 146), this, IDC_SETTING_TIME);
	CTime time1 = CTime(2007, 1, 1, 0, 0, 0);
	CTime time2 = CTime(2031, 1, 1, 0, 0, 0);
	m_dtDate.SetRange(&time1, &time2);
	GetLocalTime(&m_curtime);
	m_dtDate.SetTime(m_curtime);
	m_dtTime.SetTime(m_curtime);
	//

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SCREENADJUSTSTC);
	str = strTemp.c_str();
	m_sticTouchAdjust.Create(str, WS_CHILD|WS_VISIBLE, CRect(160, 166, 410, 182), this);
	m_sticTouchAdjust.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SCREENADJUST);
	str = strTemp.c_str();
	m_btnTouchAdjust.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(100, 164, 156, 184), this, IDC_BUTTON_SETTING_ADJUSTTOUCH);
	m_btnTouchAdjust.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	//info======================================================================================================
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SYSTOMTITLE);
	str = strTemp.c_str();
	m_sticSystomTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(96, 28, 310, 44), this);
	m_sticSystomTitle.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SOFTVERSION);
	str = strTemp.c_str();
	m_sticSoftwareVersion1.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 50, 170, 66), this);
	m_sticSoftwareVersion1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	str = "0.8.071225.3";
	m_sticSoftwareVersion.Create(str, WS_CHILD|WS_VISIBLE, CRect(174, 50, 250, 66), this, IDC_SETTING_VERSION);
	m_sticSoftwareVersion.SetColor(RGB(255, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticSoftwareVersion.SetClicked(TRUE);
	m_sticSoftwareVersion.SetUnderLine(TRUE);
	m_editVersion.Create(WS_VISIBLE|WS_CHILD|WS_BORDER|ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL, CRect(80, 70, 400, 196), this, IDC_SETTING_EDITVERSION );
	m_editVersion.ShowWindow(SW_HIDE);
	m_editVersion.SetWindowText(s_VerSion);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_HARDVERSION);
	str = strTemp.c_str();
	m_sticHardwareVersion1.Create(str, WS_CHILD|WS_VISIBLE, CRect(254, 50, 320, 66), this);
	m_sticHardwareVersion1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	str = "1.0.0.0";
	m_sticHardwareVersion.Create(str, WS_CHILD|WS_VISIBLE, CRect(324, 50, 400, 66), this);
	m_sticHardwareVersion.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_FLASHSIZE);
	str = strTemp.c_str();
	m_sticMemorySize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 68, 170, 84), this);
	m_sticMemorySize1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	str = "";
	m_sticMemorySize.Create(str, WS_CHILD|WS_VISIBLE, CRect(174, 68, 340, 84), this);
	m_sticMemorySize.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SDSTORAGESIZE);
	str = strTemp.c_str();
	m_sticStorageSize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 86, 170, 102), this);
	m_sticStorageSize1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	str = "";
	m_sticStorageSize.Create(str, WS_CHILD|WS_VISIBLE, CRect(174, 86, 340, 102), this);
	m_sticStorageSize.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CONTACTSIZE);
	str = strTemp.c_str();
	m_sticContactSize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(124, 104, 190, 120), this);
	m_sticContactSize1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	str = "";
	m_sticContactSize.Create(str, WS_CHILD|WS_VISIBLE, CRect(194, 104, 270, 120), this);
	m_sticContactSize.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CONTACTINFOSIZE);
	str = strTemp.c_str();
	m_sticContactInfoSize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(124, 124, 190, 140), this);
	m_sticContactInfoSize1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	str = "";
	m_sticContactInfoSize.Create(str, WS_CHILD|WS_VISIBLE, CRect(194, 124, 270, 140), this);
	m_sticContactInfoSize.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SOUNDSIZE);
	str = strTemp.c_str();
	m_sticSoundSize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(124, 144, 190, 160), this);
	m_sticSoundSize1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	str = "";
	m_sticSoundSize.Create(str, WS_CHILD|WS_VISIBLE, CRect(194, 144, 270, 160), this);
	m_sticSoundSize.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_INFORMATIONSIZE);
	str = strTemp.c_str();
	m_sticInformationSize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(124, 164, 190, 180), this);
	m_sticInformationSize1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	str = "";
	m_sticInformationSize.Create(str, WS_CHILD|WS_VISIBLE, CRect(194, 164, 270, 180), this);
	m_sticInformationSize.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARCONTACT);
	str = strTemp.c_str();
	m_btnClearContact.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(274, 102, 330, 122), this, IDC_BUTTON_SETTING_CLEARCONTACT);
	m_btnClearContact.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARCONTACTINFO);
	str = strTemp.c_str();
	m_btnClearContactInfo.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(274, 122, 330, 142), this, IDC_BUTTON_SETTING_CLEARCONTACTINFO);
	m_btnClearContactInfo.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARSOUND);
	str = strTemp.c_str();
	m_btnClearSound.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(274, 142, 330, 162), this, IDC_BUTTON_SETTING_CLEARSOUND);
	m_btnClearSound.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARINFORMATION);
	str = strTemp.c_str();
	m_btnClearInformation.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(274, 162, 330, 182), this, IDC_BUTTON_SETTING_CLEARINFO);
	m_btnClearInformation.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_VERSIONRIGHT);
	str = strTemp.c_str();
	m_sticVersionRight1.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 182, 170, 198), this);
	m_sticVersionRight1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	str = "北京ABC科技有限公司";
	m_sticVersionRight.Create(str, WS_CHILD|WS_VISIBLE, CRect(174, 182, 400, 198), this);
	m_sticVersionRight.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	//数据导入导出
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TITILEBACKUP);
	str = strTemp.c_str();
	m_stcBackUP.Create(str, WS_VISIBLE|WS_VISIBLE, CRect(90, 30, 280, 50), this);
	m_stcBackUP.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TITLERESTORE);
/*	str = strTemp.c_str();
	m_stcReStore.Create(str, WS_VISIBLE|WS_VISIBLE, CRect(90, 53, 300, 73), this);
	m_stcReStore.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
*/
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_BACKUP);
	str = strTemp.c_str();
	m_btnBackUP.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(282, 30, 338, 50), this, IDC_SETTING_BACKUP);
	m_btnBackUP.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RESTORE);
	str = strTemp.c_str();
	m_btnReStore.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(342, 30, 398, 50), this, IDC_SETTING_RESTORE);
	m_btnReStore.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TITLE);
	str = strTemp.c_str();
	m_stcTitle.Create(str, WS_VISIBLE|WS_VISIBLE, CRect(90, 53, 180, 73), this);
	m_stcTitle.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_OUT);
	str = strTemp.c_str();
	m_btnOut.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(182, 53, 238, 73), this, IDC_SETTING_OUT);
	m_btnOut.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

/*
	m_cmbTitle.Create(WS_VISIBLE|WS_CHILD, CRect(152, 51, 252, 173), this, IDC_SETTING_CMBSELECT);
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RECORDE);
	str = strTemp.c_str();
	m_cmbTitle.AddString(str);
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RING);
	str = strTemp.c_str();
	m_cmbTitle.AddString(str);
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_PHOTO);
	str = strTemp.c_str();
	m_cmbTitle.AddString(str);
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_MP3);
	str = strTemp.c_str();
	m_cmbTitle.AddString(str);
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_VIDEO);
	str = strTemp.c_str();
	m_cmbTitle.AddString(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LOCALFILE);
	str = strTemp.c_str();
	m_stcLocalTitle.Create(str, WS_VISIBLE|WS_VISIBLE, CRect(90, 75, 150, 93), this);
	m_stcLocalTitle.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_USBFILE);
	str = strTemp.c_str();
	m_stcUSBTitle.Create(str, WS_VISIBLE|WS_VISIBLE, CRect(282, 75, 405, 93), this);
	m_stcUSBTitle.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	m_lstLocal.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(90, 95, 215, 193), this, IDC_SETTING_LSTLOCAL, TRUE, 1);
	m_lstLocal.SetListColor(Data::g_listctrlBackRGB1[Data::g_skinstyle], Data::g_listctrlBackRGB2[Data::g_skinstyle]);
	m_lstLocal.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 105);
	m_lstUSB.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(282, 95, 405, 193), this, IDC_SETTING_LSTUSB, TRUE, 1);
	m_lstUSB.SetListColor(Data::g_listctrlBackRGB1[Data::g_skinstyle], Data::g_listctrlBackRGB2[Data::g_skinstyle]);
	m_lstUSB.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 105);

	m_stcLocal.Create(L"", WS_VISIBLE|WS_VISIBLE, CRect(89, 94, 216, 194), this);
	m_stcLocal.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_stcLocal.SetBorder(TRUE);
	m_stcUSB.Create(L"", WS_VISIBLE|WS_VISIBLE, CRect(281, 94, 406, 194), this);
	m_stcUSB.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_stcUSB.SetBorder(TRUE);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DELETE);
	str = strTemp.c_str();
	m_btnDelete.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(156, 72, 212, 92), this, IDC_SETTING_DELETE);
	m_btnDelete.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_VEDIO_ALLBTN);
	str = strTemp.c_str();
	m_btnAllSelect.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(221, 104, 277, 124), this, IDC_SETTING_ALLSELECT);
	m_btnAllSelect.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	strTemp = Data::LanguageResource::Get(Data::RI_VEDIO_DELETEALLBTN);
	str = strTemp.c_str();
	m_btnClearSelect.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(221, 127, 277, 147), this, IDC_SETTING_CLEARSELECT);
	m_btnClearSelect.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_IN);
	str = strTemp.c_str();
	m_btnIn.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(221, 150, 277, 170), this, IDC_SETTING_IN);
	m_btnIn.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_OUT);
	str = strTemp.c_str();
	m_btnOut.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(221, 173, 277, 193), this, IDC_SETTING_OUT);
	m_btnOut.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	*/
	m_sticPanel.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(416, 26, 472, 198), this);
	m_sticPanel.SetColor(RGB(0, 0, 0), Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);

	m_sticPanel2.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(81, 27, 416, 198), this);
	m_sticPanel2.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	m_sticBackground.Create(CRect(0, 0, 480, 204), this, 2);
	
	TextStruct ts[3];
	memset(ts, 0, sizeof(TextStruct) * 3);
	
	ts[0].txtRect = CRect(0, 0, 80, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_TYPE).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_TYPE).length());
	
	ts[1].txtRect = CRect(81, 0, 415, 20);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[1].sTxt, Data::LanguageResource::Get(Data::RI_SETTING_DETAILTITLE).c_str(), Data::LanguageResource::Get(Data::RI_SETTING_DETAILTITLE).length());
	
	ts[2].txtRect = CRect(415, 0, 470, 20);
	ts[2].txtFontSize = 16;
	ts[2].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[2].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).length());
		
	m_sticBackground.SetTextStruct(ts, 3);

	m_pFastDialsDlg = new CFastDialsDlg(this);
	m_pFastDialsDlg->Create(CFastDialsDlg::IDD);

	m_passwordDlg = new CPasswordDlg(this);
	m_passwordDlg->Create(CPasswordDlg::IDD);

	m_copyfileDlg = new copyfileDlg(this);
	m_copyfileDlg->Create(copyfileDlg::IDD);

	m_lsType.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
/*
	m_pImageList1 = new CImageList();
	m_pImageList1->Create(12, 12, ILC_COLOR32|ILC_MASK, 5, 5);   
    //HICON hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_CHECK0));   
	//m_pImageList->Add(hIcon);  
    //hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_CHECK1));   
	//m_pImageList->Add(hIcon);   
	
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[1][Data::g_skinstyle]);
	m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[0][Data::g_skinstyle]);
	m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[2][Data::g_skinstyle]);
	m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[3][Data::g_skinstyle]);
	m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[4][Data::g_skinstyle]);
	m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	m_lstLocal.SetImageList(m_pImageList1, LVSIL_SMALL);
	m_lstUSB.SetImageList(m_pImageList1, LVSIL_SMALL);
*/
	m_pSetting = Data::Setting::GetCurrentConfig();
	ShowConfigItems();
	IniCtrlData();
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

void CSettingDlg::IniCtrlData()
{
	if (m_pSetting->thisTelephoneNumber().number() != "")
	{
		m_edtLocalAreaNumber.SetWindowText(Util::StringOp::ToCString(m_pSetting->thisTelephoneNumber().number()));
	}
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
	
	CString s = Util::StringOp::ToCString(m_pSetting->defaultRingFilename());
	int nStart = 0;
	int n = s.Find(_T("/"), nStart);
	while(n != -1)
	{	nStart = n+1;
		n = s.Find(_T("/"), nStart);
	}
	CString filename = s.Mid(nStart);
	CString path = s.Mid(0, nStart);

	m_ringEdit.SetWindowText(filename);
	memset(m_ringDir, 0, 128*2);
	memcpy(m_ringDir, path.GetBuffer(path.GetLength()), path.GetLength()*2);
	
	m_cmbRing.AddString(_T("1"));
	m_cmbRing.AddString(_T("2"));
	m_cmbRing.AddString(_T("3"));
	int v = m_pSetting->soundVolume();
	if (v >= 0 && v <= 2)
	{
		m_cmbRing.SetCurSel(v);
		if (v == 0)
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetSoundValueRing(-3000);
		else if (v == 1)
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetSoundValueRing(-2000);
		else
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetSoundValueRing(-1000);
	}
	else
	{
		m_cmbRing.SetCurSel(0);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetSoundValueRing(-6000);
	}

	if (m_pSetting->isAutoReply())
	{
		m_chbEnableAutoLeaveWord.SetCheck(BST_CHECKED);
	}
	else
	{
		m_chbEnableAutoLeaveWord.SetCheck(BST_UNCHECKED);
	}

	CString sCount[4] = {"3", "4", "5", "6"};
	m_cmbRingTimes.AddString(sCount[0]);
	m_cmbRingTimes.AddString(sCount[1]);
	m_cmbRingTimes.AddString(sCount[2]);
	m_cmbRingTimes.AddString(sCount[3]);
	int nRingCount = m_pSetting->autoReplyRingCount();
	int nn = 0;
	if(nRingCount == 3)
		nn = 0;
	else if(nRingCount == 4)
		nn = 1;
	else if(nRingCount == 5)
		nn = 2;
	else if(nRingCount == 6)
		nn = 3;
	m_cmbRingTimes.SetCurSel(nn);

	if (m_pSetting->linkMode() == Data::lmDirect)
	{
		m_rdoConnectAuto.SetCheck(BST_CHECKED);
		m_rdoConnectADSL.SetCheck(BST_UNCHECKED);
	}
	else
	{
		m_rdoConnectAuto.SetCheck(BST_UNCHECKED);
		m_rdoConnectADSL.SetCheck(BST_CHECKED);
	}

	m_edtADSLName.SetWindowText(Util::StringOp::ToCString(m_pSetting->dialUsername()));
	m_edtADSLPassword.SetWindowText(Util::StringOp::ToCString(m_pSetting->dialPassword()));

	//从注册表中获取IP信息显示

	GetIPInfo();
	if (m_ip.isDHCP)
	{
		m_rdoGetIPDHCP.SetCheck(BST_CHECKED);
		m_rdoGetIPStatic.SetCheck(BST_UNCHECKED);
	}
	else
	{
		m_rdoGetIPDHCP.SetCheck(BST_UNCHECKED);
		m_rdoGetIPStatic.SetCheck(BST_CHECKED);
	}
	
	CString ip = m_ip.ipAddress;
	int index1 = 0;
	for(int i = 0; i < 4; i++)
	{
		int index = ip.Find('.', index1);
		CString ss;
		if(index>0)
		{
			ss = ip.Mid(index1, index-index1);
		}
		else
		{
			ss = ip.Mid(index1);
		}
		index1 = index+1;
		m_edtIP[i].SetWindowText(ss);
	}

	ip = m_ip.mask;
	index1 = 0;
	for(i = 0; i < 4; i++)
	{
		int index = ip.Find('.', index1);
		CString ss;
		if(index>0)
		{
			ss = ip.Mid(index1, index-index1);
		}
		else
		{
			ss = ip.Mid(index1);
		}
		index1 = index+1;
		m_edtMask[i].SetWindowText(ss);
	}

	ip = m_ip.gw;
	index1 = 0;
	for(i = 0; i < 4; i++)
	{
		int index = ip.Find('.', index1);
		CString ss;
		if(index>0)
		{
			ss = ip.Mid(index1, index-index1);
		}
		else
		{
			ss = ip.Mid(index1);
		}
		index1 = index+1;
		m_edtGateway[i].SetWindowText(ss);
	}

	ip = m_ip.dns;
	index1 = 0;
	for(i = 0; i < 4; i++)
	{
		int index = ip.Find('.', index1);
		CString ss;
		if(index>0)
		{
			ss = ip.Mid(index1, index-index1);
		}
		else
		{
			ss = ip.Mid(index1);
		}
		index1 = index+1;
		m_edtDNS[i].SetWindowText(ss);
	}

	/*
	if (m_pSetting->ipMode() == Data::imAuto)
	{
		m_rdoGetIPDHCP.SetCheck(BST_CHECKED);
		m_rdoGetIPStatic.SetCheck(BST_UNCHECKED);
	}
	else
	{
		m_rdoGetIPDHCP.SetCheck(BST_UNCHECKED);
		m_rdoGetIPStatic.SetCheck(BST_CHECKED);
	}

	Data::IPAddr  nIP[4] ;
	nIP[0] = m_pSetting->thisIPAddr();
	nIP[1] = m_pSetting->subnetMask;
	nIP[2] = m_pSetting->gateway();
	nIP[3] = m_pSetting->dns();
	for(int i = 0; i < 4; i++)
	{
		int ip = ((nIP[0]>>(i*8))&&0xFF);
		m_edtIP[i].SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromInt(ip)));
		ip = ((nIP[1]>>(i*8))&&0xFF);
		m_edtMask[i].SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromInt(ip)));
		ip = ((nIP[2]>>(i*8))&&0xFF);
		m_edtGateway[i].SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromInt(ip)));
		ip = ((nIP[3]>>(i*8))&&0xFF);
		m_edtDNS[i].SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromInt(ip)));
	}
	*/

	if (m_pSetting->isUseScreenSaver())
	{
		m_chbEnnabeScreenProtect.SetCheck(BST_CHECKED);
	}
	else
	{
		m_chbEnnabeScreenProtect.SetCheck(BST_UNCHECKED);
	}

	if (m_pSetting->screenSaverContent() == Data::sscClock)
	{
		m_rdoTimeScreen.SetCheck(BST_CHECKED);
		m_rdoImageScreen.SetCheck(BST_UNCHECKED);
	}
	else
	{
		m_rdoTimeScreen.SetCheck(BST_UNCHECKED);
		m_rdoImageScreen.SetCheck(BST_CHECKED);
	}
	
	CString sSecond[5] = {"15秒", "30秒", "一分钟", "五分钟", "十五分钟"};
	m_cmbWaitTime.AddString(sSecond[0]);
	m_cmbWaitTime.AddString(sSecond[1]);
	m_cmbWaitTime.AddString(sSecond[2]);
	m_cmbWaitTime.AddString(sSecond[3]);
	m_cmbWaitTime.AddString(sSecond[4]);
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
	else if(nSeconds == 15*60)
	{
		nSel = 4;
	}
	m_cmbWaitTime.SetCurSel(nSel);

	if (m_pSetting->isUseScreenSaverPassword())
	{
		m_chbEnablePassword.SetCheck(BST_CHECKED);
	}
	else
	{
		m_chbEnablePassword.SetCheck(BST_UNCHECKED);
	}

	m_cmbSoundSavePath.ResetContent();

	std::string temp = Data::LanguageResource::Get(Data::RI_SETTING_LOCAL);
	m_cmbSoundSavePath.AddString(Util::StringOp::ToCString(temp));

	BOOL sd = DetectDIR(_T("/StorageCard"));
	if (sd)
	{
		temp = Data::LanguageResource::Get(Data::RI_SETTING_SDCARD);
		m_cmbSoundSavePath.AddString(Util::StringOp::ToCString(temp));
	}

	std::string soundPath = m_pSetting->soundPath();
	if (soundPath.substr(0, 12) == "/StorageCard" && sd)
	{
		m_cmbSoundSavePath.SetCurSel(1);
	}
	else
	{
		m_cmbSoundSavePath.SetCurSel(0);
	}


// 	m_sticSoundSavePath;
// 	m_cmbSoundSavePath;

// todo: 密码设置
// todo: 速拨设置
// todo: 系统参数显示
// 	m_sticSystomTitle.ShowWindow(TRUE);
// 	m_sticSoftwareVersion.SetWindowText(Util::StringOp::ToCString(m_pSetting->softwareVersion()));
// 	m_sticHardwareVersion.SetWindowText(Util::StringOp::ToCString(m_pSetting->hardwareVersion()));
// 	m_sticMemorySize.SetWindowText(Util::StringOp::ToCString(m_pSetting->memorySize()));
// 	m_sticStorageSize.SetWindowText(Util::StringOp::ToCString(m_pSetting->storageSize()));
// 	m_sticContactSize.SetWindowText(Util::StringOp::ToCString(m_pSetting->contactUseSize()));
// 	m_sticContactInfoSize.SetWindowText(Util::StringOp::ToCString(m_pSetting->contactInfoUseSize()));
// 	m_sticSoundSize.SetWindowText(Util::StringOp::ToCString(m_pSetting->soundUseSize()));
// 	m_sticInformationSize.SetWindowText(Util::StringOp::ToCString(m_pSetting->soundUseSize()));
// 	m_sticVersionRight.SetWindowText(Util::StringOp::ToCString(m_pSetting->copyright()));
}

void CSettingDlg::OnButtonSettingOk() 
{
	//test
	// TODO: Add your control notification handler code here
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	int type = m_lsType.GetNextSelectedItem(pos);
	if(type == 2)		//参数设置
	{
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
		}
	}
	CString str;
	m_edtLocalAreaNumber.GetWindowText(str);
	m_pSetting->thisTelephoneNumber(Data::TelephoneNumber(Util::StringOp::FromCString(str)));
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
	}
	else
	{
		m_pSetting->isUseSpecRing(false);
	}
		
	m_ringEdit.GetWindowText(str);
	CString s = m_ringDir; s += str;
	m_pSetting->defaultRingFilename(Util::StringOp::FromCString(s));

	int v = m_cmbRing.GetCurSel();
	if (v == 0)
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetSoundValueRing(-3000);
	else if (v == 1)
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetSoundValueRing(-2000);
	else
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetSoundValueRing(-1000);
	m_pSetting->soundVolume(v);

	if (m_chbEnableAutoLeaveWord.GetCheck() == BST_CHECKED)
	{
		m_pSetting->isAutoReply(true);
	}
	else
	{
		m_pSetting->isAutoReply(false);
	}

	m_cmbRingTimes.GetWindowText(str);
	m_pSetting->autoReplyRingCount(Util::StringOp::ToInt(Util::StringOp::FromCString(str)));

	if (m_rdoConnectAuto.GetCheck() == BST_CHECKED)
	{
		m_pSetting->linkMode(Data::lmDirect);
	}
	else
	{
		m_pSetting->linkMode(Data::lmDial);
	}

	m_edtADSLName.GetWindowText(str);
	m_pSetting->dialUsername(Util::StringOp::FromCString(str));

	m_edtADSLPassword.GetWindowText(str);
	m_pSetting->dialPassword(Util::StringOp::FromCString(str));

	NETWORK_ADPT_INFO ipConfig;
	memset(&ipConfig, 0, sizeof(NETWORK_ADPT_INFO ));

	if (m_rdoGetIPDHCP.GetCheck() == BST_CHECKED)
	{
		ipConfig.fUseDHCP = TRUE;
		m_pSetting->ipMode(Data::imAuto);
	}
	else
	{
		ipConfig.fUseDHCP = FALSE;
		m_pSetting->ipMode(Data::imManual);
	}
	
	char txt[4][32];
	memset(txt, 0, 4*32);
	Data::IPAddr  nIP[4] = {0, 0, 0, 0};
	for(int i = 0; i < 4; i++)
	{
		m_edtIP[i].GetWindowText(str);
		nIP[0] += ((Util::StringOp::ToInt(Util::StringOp::FromCString(str))) << (i*8));
		m_edtMask[i].GetWindowText(str);
		nIP[1] += ((Util::StringOp::ToInt(Util::StringOp::FromCString(str))) << (i*8));
		m_edtGateway[i].GetWindowText(str);
		nIP[2] += ((Util::StringOp::ToInt(Util::StringOp::FromCString(str))) << (i*8));
		m_edtDNS[i].GetWindowText(str);
		nIP[3] += ((Util::StringOp::ToInt(Util::StringOp::FromCString(str))) << (i*8));
	}

	sprintf(txt[0], "%d.%d.%d.%d", (nIP[0]&0xFF), ((nIP[0] >> 8)&0xFF), ((nIP[0] >> 16)&0xFF), ((nIP[0] >> 24)&0xFF));
	sprintf(txt[1], "%d.%d.%d.%d", (nIP[1]&0xFF), ((nIP[1] >> 8)&0xFF), ((nIP[1] >> 16)&0xFF), ((nIP[1] >> 24)&0xFF));
	sprintf(txt[2], "%d.%d.%d.%d", (nIP[2]&0xFF), ((nIP[2] >> 8)&0xFF), ((nIP[2] >> 16)&0xFF), ((nIP[2] >> 24)&0xFF));
	sprintf(txt[3], "%d.%d.%d.%d", (nIP[3]&0xFF), ((nIP[3] >> 8)&0xFF), ((nIP[3] >> 16)&0xFF), ((nIP[3] >> 24)&0xFF));
	
	CString sIP = txt[0];
	memcpy(ipConfig.IPAddr, (LPCTSTR)sIP, sIP.GetLength()*2);
	sIP = txt[1];
	memcpy(ipConfig.SubnetMask, (LPCTSTR)sIP, sIP.GetLength()*2);
	sIP = txt[2];
	memcpy(ipConfig.Gateway, (LPCTSTR)sIP, sIP.GetLength()*2);
	sIP = txt[3];
	memcpy(ipConfig.DNSAddr, (LPCTSTR)sIP, sIP.GetLength()*2);
	memcpy(ipConfig.WINSAddr, (LPCTSTR)sIP, sIP.GetLength()*2);
	SetIPConfig(ipConfig);

	m_pSetting->thisIPAddr(nIP[0]);
	m_pSetting->subnetMask(nIP[1]);
	m_pSetting->gateway(nIP[2]);
	m_pSetting->dns(nIP[3]);

	if (m_chbEnnabeScreenProtect.GetCheck() == BST_CHECKED)
	{
		m_pSetting->isUseScreenSaver(true);
	}
	else
	{
		m_pSetting->isUseScreenSaver(false);
	}

	if (m_rdoTimeScreen.GetCheck() == BST_CHECKED)
	{
		m_pSetting->screenSaverContent(Data::sscClock);
	}
	else
	{
		m_pSetting->screenSaverContent(Data::sscImage);
	}
	
	int nSecond[5] = {15, 30, 60, 5*60, 15*60};
    int nSel = m_cmbWaitTime.GetCurSel();
	CTimeSpan time = CTimeSpan(0, 0, nSecond[nSel]/60, nSecond[nSel]%60);
	m_pSetting->screenSaverDuration(time);

	if (m_chbEnablePassword.GetCheck() == BST_CHECKED)
	{
		m_pSetting->isUseScreenSaverPassword(true);
	}
	else
	{
		m_pSetting->isUseScreenSaverPassword(false);
	}
		
	std::string soundPath;
	if (m_cmbSoundSavePath.GetCurSel() == 0)
	{
		soundPath = "/StorageCard/MY_RECORD/";
	}
	else
	{
		soundPath = "/UsbDisk/MY_RECORD/";
	}

	BOOL sd = DetectDIR(_T("/storage card"));
	if (!sd)
	{
		CreateDirectory(Util::StringOp::ToCString(soundPath), NULL);
	}

	m_pSetting->soundPath(soundPath);
	m_pSetting->Update();
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
}

void CSettingDlg::OnButtonSettingCancel() 
{
	// TODO: Add your control notification handler code here
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
}

void CSettingDlg::OnButtonSettingDefault() 
{
	// TODO: Add your control notification handler code here
	int id = m_pSetting->id();
	Data::SettingType type = m_pSetting->type();
	m_pSetting = Data::Setting::GetDefaultConfig();
	m_pSetting->id(id);
	m_pSetting->type(type);
	m_pSetting->Update();
	IniCtrlData();
	ShowConfigItems();
}

void CSettingDlg::OnButtonSettingRecord()
{
	/*
	CFireWallDlg* f = new CFireWallDlg(this);
	f->Create(CFireWallDlg::IDD);
	f->ShowWindow(TRUE);
	*/
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
	/*	TCHAR  value[256];
		DWORD dwDataSize;
		DWORD dType;
		memset(value, 0, 256*2);
		hRes = RegQueryValueEx(hKey, TEXT("CalibrationData"), NULL, &dType, (LPBYTE)value, &dwDataSize);
		dwDataSize = wcslen(value);
		hRes = RegSetValueEx(hKey, TEXT("CalibrationData"), NULL, REG_DWORD, (LPBYTE)value, dwDataSize);
	*/
	}
	LONG l = RegFlushKey(HKEY_LOCAL_MACHINE);
	RegCloseKey(hKey);
	
	//写注册表  触摸屏校正
	/*
	TCHAR szTemp[256] = {0};
	_tcscpy (szTemp, TEXT("HARDWARE\\DEVICEMAP\\TOUCH"));
	HKEY hKey = NULL;
	LONG hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, szTemp, 0, 0, &hKey);
	if (ERROR_SUCCESS == hRes)
	{
		TCHAR  value[256];
		DWORD dwDataSize;
		DWORD dType;
		memset(value, 0, 256*2);
		hRes = RegQueryValueEx(hKey, TEXT("CalibrationData"), NULL, &dType, (LPBYTE)value, &dwDataSize);
		dwDataSize = wcslen(value);
		hRes = RegSetValueEx(hKey, TEXT("CalibrationData"), NULL, REG_DWORD, (LPBYTE)value, dwDataSize);
	}
	//RegCloseKey(hKey);
	*/
}

void CSettingDlg::OnSetSaveScreenPassWordOK(WPARAM w, LPARAM l)
{
	m_pSetting->screenSaverPassword((char *)w);
}

void CSettingDlg::OnSettingSaveScreenPassword()
{
	m_passwordDlg->SetType(SETTING_PASSWORD);
	std::string strTemp = m_pSetting->screenSaverPassword();
	m_passwordDlg->SetOldPassWord((char *)strTemp.c_str());
	m_passwordDlg->SetHWnd(this->m_hWnd);
	m_passwordDlg->ShowWindow(SW_SHOW);	
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

	m_ringList.ShowWindow_(SW_HIDE);
	m_ringStatic.ShowWindow(SW_HIDE);

	//tel
	m_sticLocalAreaNumber.ShowWindow(FALSE);
	m_edtLocalAreaNumber.ShowWindow(FALSE);
	m_chbEnableOutLine.ShowWindow(FALSE);
	m_edtOutLine.ShowWindow(FALSE);
	m_chbEnableIPDial.ShowWindow(FALSE);
	m_edtIPDial.ShowWindow(FALSE);
	m_chbEnableRing.ShowWindow(FALSE);
	m_sticDefaultRing.ShowWindow(FALSE);
	m_cmbRing.ShowWindow(FALSE);

	m_ringEdit.ShowWindow(FALSE);
	m_ringButton.ShowWindow(FALSE);


	m_chbEnableAutoLeaveWord.ShowWindow(FALSE);
	m_sticRingTimes.ShowWindow(FALSE);
	m_cmbRingTimes.ShowWindow(FALSE);
//	m_sticAutoLeaveWord.ShowWindow(FALSE);
//	m_cmbAutoLeaveWordTip.ShowWindow(FALSE);
	m_btnRecardLeaveWordTip.ShowWindow(FALSE);
	m_btnDefineSpeedButton.ShowWindow(FALSE);
	m_sticDefineSpeedButton.ShowWindow(FALSE);
	//browser
	m_rdoConnectAuto.ShowWindow(FALSE);
	m_rdoConnectADSL.ShowWindow(FALSE);
	m_sticADSLName.ShowWindow(FALSE);
	m_sticADSLPassword.ShowWindow(FALSE);
	m_edtADSLName.ShowWindow(FALSE);
	m_edtADSLPassword.ShowWindow(FALSE);	
	m_rdoGetIPDHCP.ShowWindow(FALSE);
	m_rdoGetIPStatic.ShowWindow(FALSE);
	m_sticIP.ShowWindow(FALSE);
	m_sticMask.ShowWindow(FALSE);
	m_sticGateway.ShowWindow(FALSE);
	m_sticDNS.ShowWindow(FALSE);
	for(int i = 0; i < 4; i++)
	{
		m_edtIP[i].ShowWindow(FALSE);
		m_edtMask[i].ShowWindow(FALSE);
		m_edtGateway[i].ShowWindow(FALSE);
		m_edtDNS[i].ShowWindow(FALSE);
	}
	
	//systom
	m_chbEnnabeScreenProtect.ShowWindow(FALSE);
	m_sticScreenProtctType.ShowWindow(FALSE);
	m_rdoTimeScreen.ShowWindow(FALSE);
	m_rdoImageScreen.ShowWindow(FALSE);
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
	m_sticSoftwareVersion1.ShowWindow(FALSE);
	m_sticHardwareVersion1.ShowWindow(FALSE);
	m_sticMemorySize1.ShowWindow(FALSE);
	m_sticStorageSize1.ShowWindow(FALSE);
	m_sticContactSize1.ShowWindow(FALSE);
	m_sticContactInfoSize1.ShowWindow(FALSE);
	m_sticSoundSize1.ShowWindow(FALSE);
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
	m_stcBackUP.ShowWindow(FALSE);
//	m_stcReStore.ShowWindow(FALSE);
	m_btnBackUP.ShowWindow(FALSE);
	m_btnReStore.ShowWindow(FALSE);
	m_stcTitle.ShowWindow(FALSE);
	m_btnOut.ShowWindow(FALSE);
	/*
	m_cmbTitle.ShowWindow(FALSE);
	m_lstLocal.ShowWindow_(FALSE);
	m_lstUSB.ShowWindow_(FALSE);
	m_btnIn.ShowWindow(FALSE);
	m_btnOut.ShowWindow(FALSE);
	m_btnDelete.ShowWindow(FALSE);
	m_stcLocal.ShowWindow(FALSE);
	m_stcUSB.ShowWindow(FALSE);
	m_stcLocalTitle.ShowWindow(FALSE);
	m_stcUSBTitle.ShowWindow(FALSE);
	m_btnAllSelect.ShowWindow(FALSE);
	m_btnClearSelect.ShowWindow(FALSE);
*/
	int type = m_lsType.GetNextSelectedItem(pos);

	switch (type)
	{
	case 0:
		m_sticLocalAreaNumber.ShowWindow(TRUE);
		m_edtLocalAreaNumber.ShowWindow(TRUE);
		m_chbEnableOutLine.ShowWindow(TRUE);
		m_edtOutLine.ShowWindow(TRUE);
		m_chbEnableIPDial.ShowWindow(TRUE);
		m_edtIPDial.ShowWindow(TRUE);
		m_chbEnableRing.ShowWindow(TRUE);
		m_sticDefaultRing.ShowWindow(TRUE);
		m_cmbRing.ShowWindow(TRUE);
	
		m_ringEdit.ShowWindow(TRUE);
		m_ringButton.ShowWindow(TRUE);
	
		m_chbEnableAutoLeaveWord.ShowWindow(TRUE);
		m_sticRingTimes.ShowWindow(TRUE);
		m_cmbRingTimes.ShowWindow(TRUE);
//		m_sticAutoLeaveWord.ShowWindow(TRUE);
//		m_cmbAutoLeaveWordTip.ShowWindow(TRUE);
//		m_btnRecardLeaveWordTip.ShowWindow(TRUE);
		m_btnDefineSpeedButton.ShowWindow(TRUE);
		m_sticDefineSpeedButton.ShowWindow(TRUE);
		break;
	case 1:
		m_rdoConnectAuto.ShowWindow(TRUE);
		m_rdoConnectADSL.ShowWindow(TRUE);
		m_sticADSLName.ShowWindow(TRUE);
		m_sticADSLPassword.ShowWindow(TRUE);
		m_edtADSLName.ShowWindow(TRUE);
		m_edtADSLPassword.ShowWindow(TRUE);	
		m_rdoGetIPDHCP.ShowWindow(TRUE);
		m_rdoGetIPStatic.ShowWindow(TRUE);
		m_sticIP.ShowWindow(TRUE);
		m_sticMask.ShowWindow(TRUE);
		m_sticGateway.ShowWindow(TRUE);
		m_sticDNS.ShowWindow(TRUE);
		for(i = 0; i < 4; i++)
		{
			m_edtIP[i].ShowWindow(TRUE);
			m_edtMask[i].ShowWindow(TRUE);
			m_edtGateway[i].ShowWindow(TRUE);
			m_edtDNS[i].ShowWindow(TRUE);
		}
		
		break;
	case 2:
		m_chbEnnabeScreenProtect.ShowWindow(TRUE);
		m_sticScreenProtctType.ShowWindow(TRUE);
		m_rdoTimeScreen.ShowWindow(TRUE);
		m_rdoImageScreen.ShowWindow(TRUE);
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
			
			BOOL sd = DetectDIR(_T("/StorageCard"));
			if (sd)
			{
				temp = Data::LanguageResource::Get(Data::RI_SETTING_SDCARD);
				m_cmbSoundSavePath.AddString(Util::StringOp::ToCString(temp));
			}
			
			std::string soundPath = m_pSetting->soundPath();
			if (soundPath.substr(0, 12) == "/StorageCard" && sd)
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
	case 4:
		m_sticSystomTitle.ShowWindow(TRUE);
		m_sticSoftwareVersion1.ShowWindow(TRUE);
		m_sticHardwareVersion1.ShowWindow(TRUE);
		m_sticMemorySize1.ShowWindow(TRUE);
		m_sticStorageSize1.ShowWindow(TRUE);
		m_sticContactSize1.ShowWindow(TRUE);
		m_sticContactInfoSize1.ShowWindow(TRUE);
		m_sticSoundSize1.ShowWindow(TRUE);
//		m_sticInformationSize1.ShowWindow(TRUE);
		m_sticVersionRight1.ShowWindow(TRUE);	
		m_sticSoftwareVersion.ShowWindow(TRUE);
		m_sticHardwareVersion.ShowWindow(TRUE);
		{
			std::string temp = Data::LanguageResource::Get(Data::RI_SETTING_AVAILABLESIZE);
			CString as = Util::StringOp::ToCString(temp);
			CString text;
			ULARGE_INTEGER freeBytes;
			ULARGE_INTEGER totalBytes;
			ULARGE_INTEGER totalFreeBytes;
			GetDiskFreeSpaceEx(_T("/Flashdrv"), &freeBytes, &totalBytes, &totalFreeBytes);
			int m = totalBytes.QuadPart / (1024 * 1024);
			int f = totalFreeBytes.QuadPart / (1024 * 1024);
			text.Format(_T("%dM (%dM%s)"), m, f, as);
			m_sticMemorySize.SetWindowText(text);
			m_sticMemorySize.ShowWindow(TRUE);			

			freeBytes.QuadPart = 0;
			totalBytes.QuadPart = 0;
			totalFreeBytes.QuadPart = 0;
			GetDiskFreeSpaceEx(_T("/StorageCard"), &freeBytes, &totalBytes, &totalFreeBytes);
			m = totalBytes.QuadPart / (1024 * 1024);
			f = totalFreeBytes.QuadPart / (1024 * 1024);
			text.Format(_T("%dM (%dM%s)"), m, f, as);
			m_sticStorageSize.SetWindowText(text);
			m_sticStorageSize.ShowWindow(TRUE);
		}
		
		m_sticContactSize.ShowWindow(TRUE);
		m_sticContactInfoSize.ShowWindow(TRUE);
		m_sticSoundSize.ShowWindow(TRUE);
//		m_sticInformationSize.ShowWindow(TRUE);
		{
			int count = Data::Contact::GetDataCount("");
			CString text;
			text.Format(_T("%d"), count);
			m_sticContactSize.SetWindowText(text);

			count = Data::ContactInfo::GetDataCount("");
			text.Format(_T("%d"), count);
			m_sticContactInfoSize.SetWindowText(text);

			count = Data::SoundSegment::GetDataCount("");
			text.Format(_T("%d"), count);
			m_sticSoundSize.SetWindowText(text);
		}
		m_sticVersionRight.ShowWindow(TRUE);
		m_btnClearContact.ShowWindow(TRUE);
		m_btnClearContactInfo.ShowWindow(TRUE);
		m_btnClearSound.ShowWindow(TRUE);
//		m_btnClearInformation.ShowWindow(TRUE);
		break;
	case 3:
		m_stcBackUP.ShowWindow(TRUE);
	//	m_stcReStore.ShowWindow(TRUE);
		m_btnBackUP.ShowWindow(TRUE);
		m_btnReStore.ShowWindow(TRUE);
		m_stcTitle.ShowWindow(TRUE);
		m_btnOut.ShowWindow(TRUE);
		/*
		m_cmbTitle.ShowWindow(TRUE);
		m_lstLocal.ShowWindow_(TRUE);
		m_lstUSB.ShowWindow_(TRUE);
		m_btnIn.ShowWindow(TRUE);
		m_btnOut.ShowWindow(TRUE);
		m_btnDelete.ShowWindow(TRUE);
		m_stcLocal.ShowWindow(TRUE);
		m_stcUSB.ShowWindow(TRUE);
		m_stcLocalTitle.ShowWindow(TRUE);
		m_stcUSBTitle.ShowWindow(TRUE);
		m_btnAllSelect.ShowWindow(TRUE);
		m_btnClearSelect.ShowWindow(TRUE);
		*/
		break;
	}
}

void CSettingDlg::OnSettingClearContact()
{
	m_iDeleteType = 0;
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
	std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACT2);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
}

void CSettingDlg::OnSettingClearContactInfo()
{
	m_iDeleteType = 1;
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
	std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACTINFO2);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
}

void CSettingDlg::OnSettingClearSound()
{
	m_iDeleteType = 2;
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
	std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_SOUND2);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
}

void CSettingDlg::OnSettingClearInfo()
{
	m_iDeleteType = 3;
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
	std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
}


//数据库备份
void CSettingDlg::OnBackup()
{
	m_copyfileDlg->SetType(backup_file);
	m_copyfileDlg->ShowWindow(SW_SHOW);
}

//数据库恢复
void CSettingDlg::OnRestore()
{
	m_copyfileDlg->SetType(restore_file);
	m_copyfileDlg->ShowWindow(SW_SHOW);
}

//录音文件导出
void CSettingDlg::OnFileOut()
{
	m_copyfileDlg->SetType(copy_file);
	m_copyfileDlg->ShowWindow(SW_SHOW);
}

void CSettingDlg::OnRingLst(NMHDR* pNMHDR, LRESULT* pResult)
{
	POSITION pos = m_ringList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_ringList.GetNextSelectedItem (pos);
		CString s;
		CString s1 = Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM).c_str();
		s = m_ringList.GetItemText(index, 0);
		if(s.Compare(_T("storage card")) == 0)
		{
			SetRingLst(_T("/storage card/"));
		}
		//上一级
		else if(s.Compare(s1) == 0)
		{
			//	m_chDir
			CString sDir = m_ringDir;
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
				SetRingLst(L"");
			else
				SetRingLst(sDir.GetBuffer(128));
		}
		
		else
		{
			int n = s.Find(_T(".mp3"));
			int n1 = s.Find(_T(".MP3"));
			int n2 = s.Find(_T(".wav"));
			int n3 = s.Find(_T(".WAV"));
			if((n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
			{
				CString sDir = m_ringDir;
				sDir += s; 
				sDir += "/";
				SetRingLst(sDir.GetBuffer(128));
			}
			else
			{
				m_ringEdit.SetWindowText(s);
				m_ringList.ShowWindow_(SW_HIDE);
				m_ringStatic.ShowWindow(SW_HIDE);
			}
		}
	}

	*pResult = 0;
}

void CSettingDlg::OnRingSelect()
{
	if(m_ringStatic.IsWindowVisible())
	{
		m_ringList.ShowWindow_(SW_HIDE);
		m_ringStatic.ShowWindow(SW_HIDE);
	}
	else
	{
		SetRingLst(L"");
		m_ringList.ShowWindow_(SW_SHOW);
		m_ringStatic.ShowWindow(SW_SHOW);
	}
}

void CSettingDlg::SetRingLst(TCHAR *dir)
{
	memset(m_ringDir, 0, 128*2);
	memcpy(m_ringDir, dir, wcslen(dir)*2);
	m_ringList.DeleteAllItems();
	m_ringList.SetScrollPos(0, TRUE);
	int ncount = 0;
	if(wcslen(dir) == 0)
	{
		if(DetectDIR(_T("/storage card")))
			m_ringList.InsertItem(ncount++, _T("storage card"), 3);
		memcpy(m_ringDir, _T("/flashdrv/my_ring/"), wcslen(_T("/flashdrv/my_ring/"))*2);
	}
	
	else
	{
		CString s = Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM).c_str();
		m_ringList.InsertItem(ncount++, s, 4);
		memcpy(m_ringDir, dir, wcslen(dir)*2);
	}

	CString sDir = m_ringDir;
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
		if(strstr(filename, ".mp3")||strstr(filename, ".MP3")||strstr(filename, ".wav")||strstr(filename, ".WAV"))
		{
			m_ringList.InsertItem(ncount, FindFileData.cFileName, 5);
			m_ringList.SetItemData(ncount++, 0);
		}
		else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			m_ringList.InsertItem(ncount++, FindFileData.cFileName, 2);
		}
		
		//以下是循环使用FindNextFile函数来查找文件
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			//printf ("Next file name is %s\n", FindFileData.cFileName);
			i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
			filename[i] = '\0';
			if(strstr(filename, ".mp3")||strstr(filename, ".MP3")||strstr(filename, ".wav")||strstr(filename, ".WAV"))
			{
				m_ringList.InsertItem(ncount, FindFileData.cFileName, 5);
				m_ringList.SetItemData(ncount++, 0);
			}
			else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				m_ringList.InsertItem(ncount++, FindFileData.cFileName, 2);
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
	m_ringList.SetScrollRagle(TRUE);
	return;
}

void CSettingDlg ::GetIPInfo()
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
		DWORD dType;
		DWORD dNcount = 128;
		BYTE  Buffer[128];
		char temp[128];
		memset(Buffer, 0, 128);
		dNcount = 128;
		hRes = RegQueryValueEx(hKey, TEXT("EnableDHCP"), NULL, &dType, Buffer, &dNcount);
		
		if(*(DWORD *)Buffer == 1)
		{
			m_ip.isDHCP = 1;
			memset(Buffer, 0, 64);
			dNcount = 128;
			hRes = RegQueryValueEx(hKey, TEXT("DhcpIPAddress"), NULL, &dType, Buffer, &dNcount);
			memset(temp, 0, 128);
			wcstombs(temp, (const wchar_t *)Buffer, dNcount);
			m_ip.ipAddress = temp;
			memset(Buffer, 0, 64);
			dNcount = 128;
			hRes = RegQueryValueEx(hKey, TEXT("DhcpDefaultGateway"), NULL, &dType, Buffer, &dNcount);
			memset(temp, 0, 128);
			wcstombs(temp, (const wchar_t *)Buffer, dNcount);
			m_ip.gw = temp;
			memset(Buffer, 0, 64);
			dNcount = 128;
			hRes = RegQueryValueEx(hKey, TEXT("DhcpSubnetMask"), NULL, &dType, Buffer, &dNcount);
			memset(temp, 0, 128);
			wcstombs(temp, (const wchar_t *)Buffer, dNcount);
			m_ip.mask = temp;
			memset(Buffer, 0, 64);
			dNcount = 128;
			hRes = RegQueryValueEx(hKey, TEXT("DhcpDNS"), NULL, &dType, Buffer, &dNcount);
			memset(temp, 0, 128);
			wcstombs(temp, (const wchar_t *)Buffer, dNcount);
			m_ip.dns = temp;
		}
		else
		{
			m_ip.isDHCP = 0;
			memset(Buffer, 0, 64);
			dNcount = 128;
			hRes = RegQueryValueEx(hKey, TEXT("IpAddress"), NULL, &dType, Buffer, &dNcount);
			memset(temp, 0, 128);
			wcstombs(temp, (const wchar_t *)Buffer, dNcount);
			m_ip.ipAddress = temp;
			memset(Buffer, 0, 64);
			dNcount = 128;
			hRes = RegQueryValueEx(hKey, TEXT("Subnetmask"), NULL, &dType, Buffer, &dNcount);
			memset(temp, 0, 128);
			wcstombs(temp, (const wchar_t *)Buffer, dNcount);
			m_ip.mask = temp;
			memset(Buffer, 0, 64);
			dNcount = 128;
			hRes = RegQueryValueEx(hKey, TEXT("DefaultGateway"), NULL, &dType, Buffer, &dNcount);
			memset(temp, 0, 128);
			wcstombs(temp, (const wchar_t *)Buffer, dNcount);
			m_ip.gw = temp;
			memset(Buffer, 0, 64);
			dNcount = 128;
			hRes = RegQueryValueEx(hKey, TEXT("DNS"), NULL, &dType, Buffer, &dNcount);
			memset(temp, 0, 128);
			wcstombs(temp, (const wchar_t *)Buffer, dNcount);
			m_ip.dns = temp;
		}
		RegCloseKey(hKey);
		
		//	RegCloseKey(hKey);
	}
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

/*
//删除文件
void CSettingDlg::OnDeleteFile()
{
	
}

//标记文件全部选中
void CSettingDlg::OnAllSelect()
{
	if(m_nListSelect == 0)
	{
		int ncount = m_lstLocal.GetItemCount();
		
		LVITEM lvitem;
		lvitem.mask=LVIF_TEXT   |   LVIF_IMAGE;   
		
		for(int i = 0; i < ncount; i++)
		{
			lvitem.iItem=i;   
			lvitem.iSubItem=0;   
			lvitem.pszText = m_lstLocal.GetItemText(i, 0).GetBuffer(128);  
			CString s = lvitem.pszText;
			lvitem.lParam=i;   
			lvitem.iImage=1;
			m_lstLocal.SetItem(&lvitem);
			m_lstLocal.SetItemData(i, 1);
		}
	}
	else if(m_nListSelect == 1)
	{
		
	}
}

//清除文件选中
void CSettingDlg::OnClearDelect()
{
	if(m_nListSelect == 0)
	{
		int ncount = m_lstLocal.GetItemCount();
		
		LVITEM lvitem;
		lvitem.mask=LVIF_TEXT   |   LVIF_IMAGE;   
		
		for(int i = 0; i < ncount; i++)
		{
			lvitem.iItem=i;   
			lvitem.iSubItem=0;   
			lvitem.pszText = m_lstLocal.GetItemText(i, 0).GetBuffer(128);  
			CString s = lvitem.pszText;
			lvitem.lParam=i;   
			lvitem.iImage=0;
			m_lstLocal.SetItem(&lvitem);
			m_lstLocal.SetItemData(i, 0);
		}
	}
	else if(m_nListSelect == 1)
	{
		
	}
}
//选中的文件导入
void CSettingDlg::OnFileIn()
{

}

void CSettingDlg::SetLocalLst(TCHAR *dir)
{
	memset(m_localDir, 0, 128*2);
	memcpy(m_localDir, dir, wcslen(dir)*2);
	m_lstLocal.DeleteAllItems();
	m_lstLocal.SetScrollPos(0, TRUE);
	int ncount = 0;

	CString sDir = dir;
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
		if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			m_lstLocal.InsertItem(ncount++, FindFileData.cFileName, 0);
		}
		
		//以下是循环使用FindNextFile函数来查找文件
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				m_lstLocal.InsertItem(ncount++, FindFileData.cFileName, 0);
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
	m_lstLocal.SetScrollRagle(FALSE);
	CRect rt;
	m_lstLocal.GetWindowRect(&rt);
	m_lstLocal.InvalidateRect(&rt);
	return;	
}

void CSettingDlg::SetLocalLst()
{
	
}

//选择文件操作种类
void CSettingDlg::OnFileSelect(WPARAM w, LPARAM l)
{
	std::string strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RING);
	CString s1 = strTemp.c_str();
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_PHOTO);
	CString s2 = strTemp.c_str();
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_MP3);
	CString s3 = strTemp.c_str();
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_VIDEO);
	CString s4 = strTemp.c_str();
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RING);
	CString s5 = strTemp.c_str();

	CString s;
	m_cmbTitle.GetWindowText(s);

	//
	if(s == s1)
	{
		m_nFileOperaterType = RING_TYPE;
		SetLocalLst(_T("/flashdrv/my_ring/"));
	}
	else if(s == s2)
	{
		m_nFileOperaterType = PHOTO_TYPE;
		SetLocalLst(_T("/flashdrv/my_photo/"));
	}
	else if(s == s3)
	{
		m_nFileOperaterType = MP3_TYPE;
		SetLocalLst(_T("/flashdrv/my_music/"));
//选中的文件导出
	}
	else if(s == s4)
	{
		m_nFileOperaterType = VIDEO_TYPE;
		SetLocalLst(_T("/flashdrv/my_video/"));
	}
	else if(s == s5)
	{
		m_nFileOperaterType = RECORDE_TYPE;
		SetLocalLst(_T("/flashdrv/my_recorde/"));
	}
}
//点击文件列表
void CSettingDlg::OnLocalLst(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_nListSelect = 0;
}
//点击文件列表
void CSettingDlg::OnUSBLst(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_nListSelect = 1;
}
*/

LRESULT CSettingDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
	case WM_DELETESELITEM:
		if (m_iDeleteType == 0)
		{
			Data::Contact::Remove("");
			Data::ContactGroup::Remove("");
		}
		else if (m_iDeleteType == 1)
		{
			Data::ContactInfo::Remove("");
		}
		else if (m_iDeleteType == 2)
		{
			Data::SoundSegment::Remove("");
		}
		else if (m_iDeleteType == 3)
		{
		}
		break;
	}	
	return CDialog::WindowProc(message, wParam, lParam);
}
