// SettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "SettingDlg.h"
#include "../Data/LanguageResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog


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


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSettingDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_LIST_CONTACTINFO_TYPE, OnClickListType)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_OK, OnButtonSoundOk)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_CANCEL, OnButtonSoundCancel)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_DEFAULT, OnButtonSoundDefault)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg message handlers

BOOL CSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	std::string strTemp = Data::LanguageResource::Get(Data::RI_RECORD_TYPE);
	CString str = strTemp.c_str();
	m_sticTypeTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0, 0, 69, 20), this);
	m_sticTypeTitle.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticTypeTitle.SetLeftMargin(25);

	m_sticSepLine1.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(69, 0, 70, 204), this);
	m_sticSepLine1.SetColor(RGB(78, 165, 229), RGB(255, 255, 255));
	m_sticSepLine2.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(429, 0, 430, 204), this);
	m_sticSepLine2.SetColor(RGB(255, 255, 255), RGB(255, 255, 255));
	
	m_lsType.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(0, 20, 69, 204), this, IDC_LIST_CONTACTINFO_TYPE, FALSE);
	m_lsType.SetListColor(RGB(255, 255, 255), RGB(235, 252, 255));
	m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsType.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 100);

	m_pImageList = new CImageList();
	m_pImageList->Create(12, 12, ILC_COLOR32|ILC_MASK, 2, 2);   
    HICON hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON1));   
	m_pImageList->Add(hIcon);  
    hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON2));   
	m_pImageList->Add(hIcon);   
	m_lsType.SetImageList(m_pImageList, LVSIL_SMALL);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TELEPHONE);
	str = strTemp.c_str();
	m_lsType.InsertItem(0, str, 0);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_INTERNET);
	str = strTemp.c_str();
	m_lsType.InsertItem(1, str, 1);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SYSTEM);
	str = strTemp.c_str();
	m_lsType.InsertItem(2, str, 0);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_INFORMATION);
	str = strTemp.c_str();
	m_lsType.InsertItem(3, str, 1);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DETAILTITLE);
	str = strTemp.c_str();
	m_sticDetailTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(70, 0, 429, 20), this);
	m_sticDetailTitle.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticDetailTitle.SetLeftMargin(25);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_OPERATION);
	str = strTemp.c_str();
	m_sticOperation.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 0, 480, 20), this);
	m_sticOperation.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticOperation.SetLeftMargin(10);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 20, 480, 40), this, IDC_BUTTON_SETTING_OK);
	SetButtonDefaultColor(&m_btnOk);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 40, 480, 60), this, IDC_BUTTON_SETTING_CANCEL);
	SetButtonDefaultColor(&m_btnCancel);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DEFAULT);
	str = strTemp.c_str();
	m_btnDefault.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 184, 480, 204), this, IDC_BUTTON_SETTING_DEFAULT);
	SetButtonDefaultColor(&m_btnDefault);	

	//tel======================================================================================================
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLERING);
	str = strTemp.c_str();
	m_chbEnableRing.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(84, 25, 220, 40), this, IDC_CHECK_SOUNDDLG_PLAYCONTINUE);
	m_chbEnableRing.SetIcon(IDI_ICON1, CSize(12, 12), IDI_ICON2, CSize(12, 12));
	m_chbEnableRing.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_chbEnableRing.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_chbEnableRing.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLERINGSTC);
	str = strTemp.c_str();
	m_sticDefaultRing.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 45, 186, 60), this);
	m_cmbRing.Create(WS_CHILD|WS_VISIBLE, CRect(190,42,350,150), this, IDC_COMBOBOX_CONTACTGROUP_RING);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLEAUTOLEAVEWORD);
	str = strTemp.c_str();
	m_chbEnableAutoLeaveWord.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(84, 85, 220, 100), this, IDC_CHECK_SOUNDDLG_PLAYCONTINUE);
	m_chbEnableAutoLeaveWord.SetIcon(IDI_ICON1, CSize(12, 12), IDI_ICON2, CSize(12, 12));
	m_chbEnableAutoLeaveWord.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_chbEnableAutoLeaveWord.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_chbEnableAutoLeaveWord.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RINGTIMES);
	str = strTemp.c_str();
	m_sticRingTimes.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 105, 186, 120), this);
	m_sticRingTimes.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_cmbRingTimes.Create(WS_CHILD|WS_VISIBLE, CRect(190,102,260,170), this, IDC_COMBOBOX_CONTACTGROUP_RING);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LEAVEWORD);
	str = strTemp.c_str();
	m_sticAutoLeaveWord.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 125, 186, 140), this);
	m_sticAutoLeaveWord.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_cmbAutoLeaveWordTip.Create(WS_CHILD|WS_VISIBLE, CRect(190,122,350,200), this, IDC_COMBOBOX_CONTACTGROUP_RING);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RECORDLEAVEWORD);
	str = strTemp.c_str();
	m_btnRecardLeaveWordTip.Create(str, WS_CHILD|WS_VISIBLE, CRect(354, 125, 414, 140), this, IDC_BUTTON_SETTING_DEFAULT);
	m_btnRecardLeaveWordTip.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnRecardLeaveWordTip.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnRecardLeaveWordTip.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DEFINESPEEDBUTTON);
	str = strTemp.c_str();
	m_btnDefineSpeedButton.Create(str, WS_CHILD|WS_VISIBLE, CRect(84, 165, 184, 180), this, IDC_BUTTON_SETTING_DEFAULT);
	m_btnDefineSpeedButton.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnDefineSpeedButton.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnDefineSpeedButton.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DEFINESPEEDBUTTONSTC);
	str = strTemp.c_str();
	m_sticDefineSpeedButton.Create(str, WS_CHILD|WS_VISIBLE, CRect(190, 165, 410, 180), this);
	m_sticDefineSpeedButton.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));

	//browser======================================================================================================

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CONNECTAUTO);
	str = strTemp.c_str();
	m_rdoConnectAuto.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(84, 25, 190, 40), this, IDC_CHECK_SOUNDDLG_PLAYCONTINUE);
	m_rdoConnectAuto.SetIcon(IDI_ICON1, CSize(12, 12), IDI_ICON2, CSize(12, 12));
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CONNECTMANUAL);
	str = strTemp.c_str();
	m_rdoConnectManual.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(84, 45, 190, 60), this, IDC_CHECK_SOUNDDLG_PLAYCONTINUE);
	m_rdoConnectManual.SetIcon(IDI_ICON1, CSize(12, 12), IDI_ICON2, CSize(12, 12));
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_HTTP);
	str = strTemp.c_str();
	m_sticHttpAgent.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 65, 170, 80), this);
	m_sticHttpAgent.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SCOKS);
	str = strTemp.c_str();
	m_sticScoksAgent.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 85, 170, 100), this);
	m_sticScoksAgent.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));

	m_edtHttp.Create(WS_CHILD|WS_VISIBLE, CRect(174, 65, 300, 85), this, IDC_EDIT_CONTACTNEW_NAME);
	m_edtHttp.SetLimitText(15);

	m_edtScoks.Create(WS_CHILD|WS_VISIBLE, CRect(174, 85, 300, 105), this, IDC_EDIT_CONTACTNEW_NAME);
	m_edtScoks.SetLimitText(15);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_PORT);
	str = strTemp.c_str();
	m_sticHttpPort.Create(str, WS_CHILD|WS_VISIBLE, CRect(304, 65, 350, 80), this);
	m_sticHttpPort.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));

	m_sticScoksPort.Create(str, WS_CHILD|WS_VISIBLE, CRect(304, 85, 350, 100), this);
	m_sticScoksPort.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	
	m_edtHttpPort.Create(WS_CHILD|WS_VISIBLE, CRect(354, 65, 410, 85), this, IDC_EDIT_CONTACTNEW_NAME);
	m_edtHttpPort.SetLimitText(15);
	
	m_edtScoksPort.Create(WS_CHILD|WS_VISIBLE, CRect(354, 85, 410, 105), this, IDC_EDIT_CONTACTNEW_NAME);
	m_edtScoksPort.SetLimitText(15);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_HOMESITE);
	str = strTemp.c_str();
	m_sticHomeSite.Create(str, WS_CHILD|WS_VISIBLE, CRect(84, 125, 150, 140), this);
	m_sticHomeSite.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));

	m_edtHomeSite.Create(WS_CHILD|WS_VISIBLE, CRect(154, 125, 410, 145), this, IDC_EDIT_CONTACTNEW_NAME);
	m_edtHomeSite.SetLimitText(15);
	
	//systom======================================================================================================
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLESP);
	str = strTemp.c_str();
	m_chbEnnabeScreenProtect.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(84, 25, 190, 40), this, IDC_CHECK_SOUNDDLG_PLAYCONTINUE);
	m_chbEnnabeScreenProtect.SetIcon(IDI_ICON1, CSize(12, 12), IDI_ICON2, CSize(12, 12));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SPTYPE);
	str = strTemp.c_str();
	m_sticScreenProtctType.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 45, 170, 60), this);
	m_sticScreenProtctType.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TIMESP);
	str = strTemp.c_str();
	m_rdoTimeScreen.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(174, 45, 250, 60), this, IDC_CHECK_SOUNDDLG_PLAYCONTINUE);
	m_rdoTimeScreen.SetIcon(IDI_ICON1, CSize(12, 12), IDI_ICON2, CSize(12, 12));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_IMAGESP);
	str = strTemp.c_str();
	m_rdoImageScreen.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(254, 45, 360, 60), this, IDC_CHECK_SOUNDDLG_PLAYCONTINUE);
	m_rdoImageScreen.SetIcon(IDI_ICON1, CSize(12, 12), IDI_ICON2, CSize(12, 12));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_WAITTIME);
	str = strTemp.c_str();
	m_sticWaitTime.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 65, 170, 80), this);
	m_sticWaitTime.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));

	m_cmbWaitTime.Create(WS_CHILD|WS_VISIBLE, CRect(174,62,250,170), this, IDC_COMBOBOX_CONTACTGROUP_RING);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLEPASSWORD);
	str = strTemp.c_str();
	m_chbEnablePassword.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(104, 85, 250, 100), this, IDC_CHECK_SOUNDDLG_PLAYCONTINUE);
	m_chbEnablePassword.SetIcon(IDI_ICON1, CSize(12, 12), IDI_ICON2, CSize(12, 12));
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SETPASSWORD);
	str = strTemp.c_str();
	m_btnSetPassword.Create(str, WS_CHILD|WS_VISIBLE, CRect(254, 85, 330, 100), this, IDC_BUTTON_SETTING_DEFAULT);
	m_btnSetPassword.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnSetPassword.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnSetPassword.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SCREENADJUSTSTC);
	str = strTemp.c_str();
	m_sticTouchAdjust.Create(str, WS_CHILD|WS_VISIBLE, CRect(84, 125, 310, 140), this);
	m_sticTouchAdjust.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SCREENADJUST);
	str = strTemp.c_str();
	m_btnTouchAdjust.Create(str, WS_CHILD|WS_VISIBLE, CRect(314, 125, 410, 140), this, IDC_BUTTON_SETTING_DEFAULT);
	m_btnTouchAdjust.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnTouchAdjust.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnTouchAdjust.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));
	
	//info======================================================================================================
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SYSTOMTITLE);
	str = strTemp.c_str();
	m_sticSystomTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(84, 25, 310, 40), this);
	m_sticSystomTitle.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SOFTVERSION);
	str = strTemp.c_str();
	m_sticSoftwareVersion1.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 49, 170, 64), this);
	m_sticSoftwareVersion1.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticSoftwareVersion.Create(str, WS_CHILD|WS_VISIBLE, CRect(174, 49, 250, 64), this);
	m_sticSoftwareVersion.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_HARDVERSION);
	str = strTemp.c_str();
	m_sticHardwareVersion1.Create(str, WS_CHILD|WS_VISIBLE, CRect(254, 49, 320, 64), this);
	m_sticHardwareVersion1.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticHardwareVersion.Create(str, WS_CHILD|WS_VISIBLE, CRect(324, 49, 400, 64), this);
	m_sticHardwareVersion.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_MEMORYSIZE);
	str = strTemp.c_str();
	m_sticMemorySize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 66, 170, 81), this);
	m_sticMemorySize1.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticMemorySize.Create(str, WS_CHILD|WS_VISIBLE, CRect(174, 66, 270, 81), this);
	m_sticMemorySize.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_STORAGESIZE);
	str = strTemp.c_str();
	m_sticStorageSize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 83, 170, 98), this);
	m_sticStorageSize1.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticStorageSize.Create(str, WS_CHILD|WS_VISIBLE, CRect(174, 83, 270, 98), this);
	m_sticStorageSize.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CONTACTSIZE);
	str = strTemp.c_str();
	m_sticContactSize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(124, 100, 190, 115), this);
	m_sticContactSize1.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticContactSize.Create(str, WS_CHILD|WS_VISIBLE, CRect(194, 100, 270, 115), this);
	m_sticContactSize.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CONTACTINFOSIZE);
	str = strTemp.c_str();
	m_sticContactInfoSize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(124, 117, 190, 132), this);
	m_sticContactInfoSize1.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticContactInfoSize.Create(str, WS_CHILD|WS_VISIBLE, CRect(194, 117, 270, 132), this);
	m_sticContactInfoSize.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SOUNDSIZE);
	str = strTemp.c_str();
	m_sticSoundSize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(124, 134, 190, 149), this);
	m_sticSoundSize1.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticSoundSize.Create(str, WS_CHILD|WS_VISIBLE, CRect(194, 134, 270, 149), this);
	m_sticSoundSize.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_INFORMATIONSIZE);
	str = strTemp.c_str();
	m_sticInformationSize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(124, 151, 190, 166), this);
	m_sticInformationSize1.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticInformationSize.Create(str, WS_CHILD|WS_VISIBLE, CRect(194, 151, 270, 166), this);
	m_sticInformationSize.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARCONTACT);
	str = strTemp.c_str();
	m_btnClearContact.Create(str, WS_CHILD|WS_VISIBLE, CRect(274, 100, 370, 115), this, IDC_BUTTON_SETTING_DEFAULT);
	m_btnClearContact.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnClearContact.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnClearContact.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARCONTACTINFO);
	str = strTemp.c_str();
	m_btnClearContactInfo.Create(str, WS_CHILD|WS_VISIBLE, CRect(274, 117, 370, 132), this, IDC_BUTTON_SETTING_DEFAULT);
	m_btnClearContactInfo.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnClearContactInfo.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnClearContactInfo.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARSOUND);
	str = strTemp.c_str();
	m_btnClearSound.Create(str, WS_CHILD|WS_VISIBLE, CRect(274, 134, 370, 149), this, IDC_BUTTON_SETTING_DEFAULT);
	m_btnClearSound.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnClearSound.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnClearSound.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARINFORMATION);
	str = strTemp.c_str();
	m_btnClearInformation.Create(str, WS_CHILD|WS_VISIBLE, CRect(274, 151, 370, 166), this, IDC_BUTTON_SETTING_DEFAULT);
	m_btnClearInformation.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnClearInformation.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnClearInformation.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_VERSIONRIGHT);
	str = strTemp.c_str();
	m_sticVersionRight1.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 168, 170, 183), this);
	m_sticVersionRight1.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticVersionRight.Create(str, WS_CHILD|WS_VISIBLE, CRect(174, 168, 400, 183), this);
	m_sticVersionRight.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	
	m_pFastDialsDlg = new CFastDialsDlg(this);
	m_pFastDialsDlg->Create(CFastDialsDlg::IDD);

	m_lsType.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	ShowConfigItems();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingDlg::SetButtonDefaultColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CSettingDlg::SetButtonSelectedColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CSettingDlg::OnClickListType(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ShowConfigItems();
	*pResult = 0;
}

void CSettingDlg::OnButtonSoundOk() 
{
	// TODO: Add your control notification handler code here
}

void CSettingDlg::OnButtonSoundCancel() 
{
	// TODO: Add your control notification handler code here
}

void CSettingDlg::OnButtonSoundDefault() 
{
	// TODO: Add your control notification handler code here
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
	m_chbEnableRing.ShowWindow(FALSE);
	m_sticDefaultRing.ShowWindow(FALSE);
	m_cmbRing.ShowWindow(FALSE);
	m_chbEnableAutoLeaveWord.ShowWindow(FALSE);
	m_sticRingTimes.ShowWindow(FALSE);
	m_cmbRingTimes.ShowWindow(FALSE);
	m_sticAutoLeaveWord.ShowWindow(FALSE);
	m_cmbAutoLeaveWordTip.ShowWindow(FALSE);
	m_btnRecardLeaveWordTip.ShowWindow(FALSE);
	m_btnDefineSpeedButton.ShowWindow(FALSE);
	m_sticDefineSpeedButton.ShowWindow(FALSE);
	//browser
	m_rdoConnectAuto.ShowWindow(FALSE);
	m_rdoConnectManual.ShowWindow(FALSE);
	m_sticHttpAgent.ShowWindow(FALSE);
	m_sticScoksAgent.ShowWindow(FALSE);
	m_edtHttp.ShowWindow(FALSE);
	m_edtScoks.ShowWindow(FALSE);
	m_sticHttpPort.ShowWindow(FALSE);
	m_sticScoksPort.ShowWindow(FALSE);
	m_edtHttpPort.ShowWindow(FALSE);
	m_edtScoksPort.ShowWindow(FALSE);
	m_sticHomeSite.ShowWindow(FALSE);
	m_edtHomeSite.ShowWindow(FALSE);
	//systom
	m_chbEnnabeScreenProtect.ShowWindow(FALSE);
	m_sticScreenProtctType.ShowWindow(FALSE);
	m_rdoTimeScreen.ShowWindow(FALSE);
	m_rdoImageScreen.ShowWindow(FALSE);
	m_sticWaitTime.ShowWindow(FALSE);
	m_cmbWaitTime.ShowWindow(FALSE);
	m_chbEnablePassword.ShowWindow(FALSE);
	m_btnSetPassword.ShowWindow(FALSE);
	m_sticTouchAdjust.ShowWindow(FALSE);
	m_btnTouchAdjust.ShowWindow(FALSE);
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

	int type = m_lsType.GetNextSelectedItem(pos);

	switch (type)
	{
	case 0:
		m_chbEnableRing.ShowWindow(TRUE);
		m_sticDefaultRing.ShowWindow(TRUE);
		m_cmbRing.ShowWindow(TRUE);
		m_chbEnableAutoLeaveWord.ShowWindow(TRUE);
		m_sticRingTimes.ShowWindow(TRUE);
		m_cmbRingTimes.ShowWindow(TRUE);
		m_sticAutoLeaveWord.ShowWindow(TRUE);
		m_cmbAutoLeaveWordTip.ShowWindow(TRUE);
		m_btnRecardLeaveWordTip.ShowWindow(TRUE);
		m_btnDefineSpeedButton.ShowWindow(TRUE);
		m_sticDefineSpeedButton.ShowWindow(TRUE);
		break;
	case 1:
		m_rdoConnectAuto.ShowWindow(TRUE);
		m_rdoConnectManual.ShowWindow(TRUE);
		m_sticHttpAgent.ShowWindow(TRUE);
		m_sticScoksAgent.ShowWindow(TRUE);
		m_edtHttp.ShowWindow(TRUE);
		m_edtScoks.ShowWindow(TRUE);
		m_sticHttpPort.ShowWindow(TRUE);
		m_sticScoksPort.ShowWindow(TRUE);
		m_edtHttpPort.ShowWindow(TRUE);
		m_edtScoksPort.ShowWindow(TRUE);
		m_sticHomeSite.ShowWindow(TRUE);
		m_edtHomeSite.ShowWindow(TRUE);
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
		m_sticTouchAdjust.ShowWindow(TRUE);
		m_btnTouchAdjust.ShowWindow(TRUE);
		break;
	case 3:
		m_sticSystomTitle.ShowWindow(TRUE);
		m_sticSoftwareVersion1.ShowWindow(TRUE);
		m_sticHardwareVersion1.ShowWindow(TRUE);
		m_sticMemorySize1.ShowWindow(TRUE);
		m_sticStorageSize1.ShowWindow(TRUE);
		m_sticContactSize1.ShowWindow(TRUE);
		m_sticContactInfoSize1.ShowWindow(TRUE);
		m_sticSoundSize1.ShowWindow(TRUE);
		m_sticInformationSize1.ShowWindow(TRUE);
		m_sticVersionRight1.ShowWindow(TRUE);	
		m_sticSoftwareVersion.ShowWindow(TRUE);
		m_sticHardwareVersion.ShowWindow(TRUE);
		m_sticMemorySize.ShowWindow(TRUE);
		m_sticStorageSize.ShowWindow(TRUE);
		m_sticContactSize.ShowWindow(TRUE);
		m_sticContactInfoSize.ShowWindow(TRUE);
		m_sticSoundSize.ShowWindow(TRUE);
		m_sticInformationSize.ShowWindow(TRUE);
		m_sticVersionRight.ShowWindow(TRUE);
		m_btnClearContact.ShowWindow(TRUE);
		m_btnClearContactInfo.ShowWindow(TRUE);
		m_btnClearSound.ShowWindow(TRUE);
		m_btnClearInformation.ShowWindow(TRUE);
		break;
	}
}