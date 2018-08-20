// ContactNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "ContactNewDlg.h"
#include "../Data/LanguageResource.h"
#include "ContactDlg.h"
#include "../Data/ContactGroup.h"
#include "../MultimediaPhoneDlg.h"

#include "../Data/ContactInfo.h"
#include "../Data/SkinStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContactNewDlg dialog


CContactNewDlg::CContactNewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CContactNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContactNewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CContactNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContactNewDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContactNewDlg, CDialog)
	//{{AFX_MSG_MAP(CContactNewDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CONTACTNEW_OK, OnButtonContactNewOk)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTNEW_CANCEL, OnButtonContactNewCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTNEW_DELETE, OnButtonContactNewDelete)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTNEW_DIALMOBILE1, OnButtonContactNewDialMobile1)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTNEW_DIALMOBILE2, OnButtonContactNewDialMobile2)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTNEW_DIALTEL1, OnButtonContactNewDialTelephone1)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTNEW_DIALTEL2, OnButtonContactNewDialTelephone2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContactNewDlg message handlers

BOOL CContactNewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	std::string strTemp;
	CString str;

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_NAMESTC);
	strTemp += ":";
	str = strTemp.c_str();
	m_sticName.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(18, 40, 60, 58), this);
	m_sticName.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticName.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_MOBILESTC);
	str = strTemp.c_str();
	m_sticMobile1.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(18, 61, 60, 79), this);
	m_sticMobile1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticMobile1.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_MOBILESTC);
	str = strTemp.c_str();
	m_sticMobile2.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(18, 82, 60, 100), this);
	m_sticMobile2.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticMobile2.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_TELPHONESTC);
	str = strTemp.c_str();
	m_sticTelephone1.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(18, 103, 60, 121), this);
	m_sticTelephone1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticTelephone1.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_TELPHONESTC);
	str = strTemp.c_str();
	m_sticTelephone2.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(18, 124, 60, 142), this);
	m_sticTelephone2.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticTelephone2.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DIAL);
	str = strTemp.c_str();
	m_btnDialMobile1.Create(str, Data::g_buttonDialBMPID[0][Data::g_skinstyle], Data::g_buttonDialBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(184, 61, 230, 80), this, IDC_BUTTON_CONTACTNEW_DIALMOBILE1);

	m_btnDialMobile2.Create(str, Data::g_buttonDialBMPID[0][Data::g_skinstyle], Data::g_buttonDialBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(184, 82, 230, 101), this, IDC_BUTTON_CONTACTNEW_DIALMOBILE2);

	m_btnDialTelephone1.Create(str, Data::g_buttonDialBMPID[0][Data::g_skinstyle], Data::g_buttonDialBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(184, 103, 230, 122), this, IDC_BUTTON_CONTACTNEW_DIALTEL1);

	m_btnDialTelephone2.Create(str, Data::g_buttonDialBMPID[0][Data::g_skinstyle], Data::g_buttonDialBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(184, 124, 230, 143), this, IDC_BUTTON_CONTACTNEW_DIALTEL2);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_EMAILSTC);
	str = strTemp.c_str();
	m_sticEmail.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(18, 145, 60, 163), this);
	m_sticEmail.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticEmail.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_WEBSTC);
	str = strTemp.c_str();
	m_sticSite.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(18, 166, 60, 184), this);
	m_sticSite.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticSite.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_GROUPSTC);
	str = strTemp.c_str();
	m_sticGroup.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(243, 40, 285, 58), this);
	m_sticGroup.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticGroup.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_COMPANYSTC);
	strTemp += ":";
	str = strTemp.c_str();
	m_sticCompany.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(243, 61, 285, 79), this);
	m_sticCompany.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticCompany.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_COMPANYADDRESSSTC);
	strTemp += ":";
	str = strTemp.c_str();
	m_sticCompanyAddress.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(243, 82, 285, 100), this);
	m_sticCompanyAddress.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticCompanyAddress.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_POSTCODESTC);
	str = strTemp.c_str();
	m_sticPostCode.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(243, 103, 285, 121), this);
	m_sticPostCode.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticPostCode.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_DEPARTSTC);
	strTemp += ":";
	str = strTemp.c_str();
	m_sticDepartment.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(243, 124, 285, 142), this);
	m_sticDepartment.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticDepartment.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_TITLESTC);
	strTemp += ":";
	str = strTemp.c_str();
	m_sticDuty.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(243, 145, 285, 163), this);
	m_sticDuty.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticDuty.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_MOTOSTC);
	str = strTemp.c_str();
	m_sticMemo.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(243, 166, 285, 184), this);
	m_sticMemo.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticMemo.SetAlign(SS_CENTER);

	m_cmbType.Create(WS_CHILD|WS_VISIBLE, CRect(180, 40, 241, 120), this, IDC_COMBOBOX_CONTACTNEW_TYPE);
	m_cmbGroup.Create(WS_CHILD|WS_VISIBLE, CRect(286, 40, 400, 130), this, IDC_COMBOBOX_CONTACTNEW_GROUP);
			
	m_cmbType.AddString(CString(Data::LanguageResource::Get(Data::RI_CARD_COMNSTC).c_str()));
	m_cmbType.AddString(CString(Data::LanguageResource::Get(Data::RI_CARD_VIPSTC).c_str()));
	m_cmbType.AddString(CString(Data::LanguageResource::Get(Data::RI_CARD_BLACKLISTSTC).c_str()));

	m_edtName.Create(WS_CHILD|WS_VISIBLE, CRect(61, 40, 175, 58), this, IDC_EDIT_CONTACTNEW_NAME);
	m_edtName.SetWindowText(L"");
	m_edtName.SetLimitText(15);
	m_edtMobile1.Create(WS_CHILD|WS_VISIBLE, CRect(61, 61, 175, 79), this, IDC_EDIT_CONTACTNEW_MOBILE1);
	m_edtMobile1.SetLimitText(15);
	m_edtMobile2.Create(WS_CHILD|WS_VISIBLE, CRect(61, 82, 175, 100), this, IDC_EDIT_CONTACTNEW_MOBILE2);
	m_edtMobile2.SetLimitText(15);
	m_edtTelephone1.Create(WS_CHILD|WS_VISIBLE, CRect(61, 103, 175, 121), this, IDC_EDIT_CONTACTNEW_TELEPHONE1);
	m_edtTelephone1.SetLimitText(15);
	m_edtTelephone2.Create(WS_CHILD|WS_VISIBLE, CRect(61, 124, 175, 142), this, IDC_EDIT_CONTACTNEW_TELEPHONE2);
	m_edtTelephone2.SetLimitText(15);
	m_edtEmail.Create(WS_CHILD|WS_VISIBLE, CRect(61, 145, 175, 163), this, IDC_EDIT_CONTACTNEW_EMAIL);
	m_edtEmail.SetLimitText(15);
	m_edtSite.Create(WS_CHILD|WS_VISIBLE, CRect(61, 166, 175, 184), this, IDC_EDIT_CONTACTNEW_SITE);
	m_edtSite.SetLimitText(15);

	m_edtCompany.Create(WS_CHILD|WS_VISIBLE, CRect(286, 61, 400, 79), this, IDC_EDIT_CONTACTNEW_COMPANY);
	m_edtCompany.SetLimitText(15);
	m_edtCompanyAddress.Create(WS_CHILD|WS_VISIBLE, CRect(286, 82, 400, 100), this, IDC_EDIT_CONTACTNEW_ADDRESS);
	m_edtCompanyAddress.SetLimitText(15);
	m_edtPostCode.Create(WS_CHILD|WS_VISIBLE, CRect(286, 103, 400, 121), this, IDC_EDIT_CONTACTNEW_POSTCODE);
	m_edtPostCode.SetLimitText(15);
	m_edtDepartment.Create(WS_CHILD|WS_VISIBLE, CRect(286, 124, 400, 142), this, IDC_EDIT_CONTACTNEW_DEPARTMENT);
	m_edtDepartment.SetLimitText(15);
	m_edtDuty.Create(WS_CHILD|WS_VISIBLE, CRect(286, 145, 400, 163), this, IDC_EDIT_CONTACTNEW_DUTY);
	m_edtDuty.SetLimitText(15);
	m_edtMemo.Create(WS_CHILD|WS_VISIBLE, CRect(286, 166, 400, 184), this, IDC_EDIT_CONTACTNEW_MEMO);
	m_edtMemo.SetLimitText(15);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.Create(str, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(415, 41, 471, 61), this, IDC_BUTTON_CONTACTNEW_OK);
	m_btnOk.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(415, 66, 471, 86), this, IDC_BUTTON_CONTACTNEW_CANCEL);
	m_btnCancel.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_DELETECARDBTN);
	str = strTemp.c_str();
	m_btnDelete.Create(str, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(415, 91, 471, 111), this, IDC_BUTTON_CONTACTNEW_DELETE);
	m_btnDelete.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

	m_sticBackground.Create(CRect(0, 0, 480, 204), this, 1);

	m_pPinYinIndex = LoadPinYinIndex();
	m_pContact = boost::shared_ptr<Data::Contact>();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CContactNewDlg::SetButtonDefaultColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CContactNewDlg::SetButtonSelectedColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CContactNewDlg::AddGroupItems(void)
{
	m_cmbGroup.ResetContent();
	std::vector<boost::shared_ptr<Data::ContactGroup> > result = Data::ContactGroup::GetFromDatabase("");
	for (int i = 0; i < result.size(); ++i)
	{
		m_cmbGroup.AddString(CString(result[i]->name().c_str()));
		m_cmbGroup.SetItemData(i, result[i]->id());
	}
}

void CContactNewDlg::OnButtonContactNewOk() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(FALSE);
	(CMultimediaPhoneDlg*)theApp.inputDlg_->ShowWindow(FALSE);
	
	if (!m_pContact)
	{
		m_pContact = boost::shared_ptr<Data::Contact>(new Data::Contact);
	}	
	CString s;
	m_edtName.GetWindowText(s);

	m_pContact->name(Util::StringOp::FromCString(s));
	m_pContact->alias(GetAlias(m_pContact->name()));

	m_edtMobile1.GetWindowText(s);
	m_pContact->mobilesTelephone(Data::TelephoneNumber(Util::StringOp::FromCString(s)));
	m_edtMobile2.GetWindowText(s);
	m_pContact->mobilesTelephone2(Data::TelephoneNumber(Util::StringOp::FromCString(s)));
	m_edtTelephone1.GetWindowText(s);
	m_pContact->worksTelephone(Data::TelephoneNumber(Util::StringOp::FromCString(s)));
	m_edtTelephone2.GetWindowText(s);
	m_pContact->worksTelephone2(Data::TelephoneNumber(Util::StringOp::FromCString(s)));
	m_edtEmail.GetWindowText(s);
	m_pContact->email(Util::StringOp::FromCString(s));
	m_edtSite.GetWindowText(s);
	m_pContact->homesite(Util::StringOp::FromCString(s));
	m_edtCompany.GetWindowText(s);
	m_pContact->company(Util::StringOp::FromCString(s));
	m_edtCompanyAddress.GetWindowText(s);
	m_pContact->companyAddress((Util::StringOp::FromCString(s)));
	m_edtPostCode.GetWindowText(s);
	m_pContact->companyPostCode((Util::StringOp::FromCString(s)));
	m_edtDepartment.GetWindowText(s);
	m_pContact->department(Util::StringOp::FromCString(s));
	m_edtDuty.GetWindowText(s);
	m_pContact->duty(Util::StringOp::FromCString(s));
	m_edtMemo.GetWindowText(s);
	m_pContact->memo(Util::StringOp::FromCString(s));
	if (m_cmbType.GetCurSel() == CB_ERR)
	{
		m_pContact->type(Data::ctNormal);
	}
	else
	{
		m_pContact->type(Data::ContactType(m_cmbType.GetCurSel()));
	}

	if (m_cmbGroup.GetCurSel() == CB_ERR)
	{
		m_pContact->groupId(0);
	}
	else
	{
		m_pContact->groupId(m_cmbGroup.GetItemData(m_cmbGroup.GetCurSel()));
	}

	if (m_pContact->id() == 0)
	{
		m_pContact->Insert();
	}
	else
	{
		m_pContact->Update();
	}
	((CContactDlg*)m_pParentWnd)->ShowItemsInList();
}

void CContactNewDlg::OnButtonContactNewCancel() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(FALSE);
	(CMultimediaPhoneDlg*)theApp.inputDlg_->ShowWindow(FALSE);
}

void CContactNewDlg::OnButtonContactNewDelete() 
{
	// TODO: Add your control notification handler code here
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_pParentWnd->GetSafeHwnd());
	std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACT);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
}

void CContactNewDlg::SetContact(boost::shared_ptr<Data::Contact> contact)
{
	m_pContact = contact;
	std::string strTemp;
	if (contact->id() == 0)
	{
		strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWCARDSTC);
		m_btnDelete.ShowWindow(FALSE);
	}
	else
	{
		strTemp = Data::LanguageResource::Get(Data::RI_CARD_EDITCARDSTC);
		m_btnDelete.ShowWindow(TRUE);
	}
// 	CString str = strTemp.c_str();
// 	m_sticTitle.SetWindowText(str);

	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, strTemp.c_str(), strTemp.length());
	
	m_sticBackground.SetTextStruct(ts, 1);


	m_edtName.SetWindowText(CString(contact->name().c_str()));
// 	m_edtMobile1.SetWindowText(CString(contact->mobilesTelephone().ToString().c_str()));
// 	m_edtMobile2.SetWindowText(CString(contact->mobilesTelephone2().ToString().c_str()));
// 	m_edtTelephone1.SetWindowText(CString(contact->worksTelephone().ToString().c_str()));
// 	m_edtTelephone2.SetWindowText(CString(contact->worksTelephone2().ToString().c_str()));
	m_edtMobile1.SetWindowText(CString(contact->mobilesTelephone().number().c_str()));
// 	if (contact->mobilesTelephone().number() != "")
// 	{
// 		m_btnDialMobile1.ShowWindow(TRUE);
// 	}
// 	else
// 	{
// 		m_btnDialMobile1.ShowWindow(FALSE);
// 	}
	m_edtMobile2.SetWindowText(CString(contact->mobilesTelephone2().number().c_str()));
// 	if (contact->mobilesTelephone2().number() != "")
// 	{
// 		m_btnDialMobile2.ShowWindow(TRUE);
// 	}
// 	else
// 	{
// 		m_btnDialMobile2.ShowWindow(FALSE);
// 	}

	m_edtTelephone1.SetWindowText(CString(contact->worksTelephone().number().c_str()));
// 	if (contact->worksTelephone().number() != "")
// 	{
// 		m_btnDialTelephone1.ShowWindow(TRUE);
// 	}
// 	else
// 	{
// 		m_btnDialTelephone1.ShowWindow(FALSE);
// 	}

	m_edtTelephone2.SetWindowText(CString(contact->worksTelephone2().number().c_str()));
// 	if (contact->worksTelephone2().number() != "")
// 	{
// 		m_btnDialTelephone2.ShowWindow(TRUE);
// 	}
// 	else
// 	{
// 		m_btnDialTelephone2.ShowWindow(FALSE);
// 	}

	m_edtEmail.SetWindowText(CString(contact->email().c_str()));
	m_edtSite.SetWindowText(CString(contact->homesite().c_str()));
	m_edtCompany.SetWindowText(CString(contact->company().c_str()));
	m_edtCompanyAddress.SetWindowText(CString(contact->companyAddress().c_str()));
	m_edtPostCode.SetWindowText(CString(contact->companyPostCode().c_str()));
	m_edtDepartment.SetWindowText(CString(contact->department().c_str()));
	m_edtDuty.SetWindowText(CString(contact->duty().c_str()));
	m_edtMemo.SetWindowText(CString(contact->memo().c_str()));
	
	if (m_cmbType.SetCurSel(contact->type()) == CB_ERR)
	{
		m_cmbType.SetCurSel(Data::ctNormal);
	}
	AddGroupItems();
// 	int id = 0;
// 	if (contact->group())
// 	{
// 		id = contact->group()->id();
// 	}
	int id = contact->groupId();
	std::string filter;
	filter = "id = " + Util::StringOp::FromInt(id);
	std::vector<boost::shared_ptr<Data::ContactGroup> > result = Data::ContactGroup::GetFromDatabase(filter);
	if (!result.empty())
	{
		m_cmbGroup.SelectString(0, CString(result[0]->name().c_str()));
	}
	else
	{
		m_cmbGroup.SetCurSel(-1);
	}
}

char* CContactNewDlg::LoadPinYinIndex(void)
{
	CFile m_pHexFile;   

	m_pHexFile.Open(_T("/flashdrv/res_dat/firstchar.txt"), CFile::modeRead | CFile::typeBinary);   

	if(!m_pHexFile)   
	{   
		return NULL;   
	}   
	int nFilelen = m_pHexFile.GetLength();   
	char* szBuf;   
	szBuf = new char[nFilelen];   
	int nRet = m_pHexFile.Read(szBuf,nFilelen);   
	if(nRet <= 0)   
	{   
		return NULL;   
	}   
	m_pHexFile.Close();  
	return szBuf;
}

std::string CContactNewDlg::GetAlias(std::string name)
{
	if (m_pPinYinIndex == NULL)
	{
		return "";
	}

	std::string alias;
	for (int i = 0; i < name.size(); ++i)
	{
		if ((unsigned char)name[i] > (unsigned char)127)
		{
			unsigned int index = ((unsigned char)name[i] - 0xb0) * 94 + ((unsigned char)name[i+1] - 0xa1);
			alias += m_pPinYinIndex[index];	
			++i;
		}
		else
		{
			alias += name[i];
		}
	}
	return alias;
}

void CContactNewDlg::OnButtonContactNewDialMobile1()
{
	CString mobile;
	m_edtMobile1.GetWindowText(mobile);
	if (mobile.IsEmpty())
	{
		return;
	}
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(Util::StringOp::FromCString(mobile), m_pContact->id());
}

void CContactNewDlg::OnButtonContactNewDialMobile2()
{
	CString mobile;
	m_edtMobile2.GetWindowText(mobile);
	if (mobile.IsEmpty())
	{
		return;
	}
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(Util::StringOp::FromCString(mobile), m_pContact->id());
}

void CContactNewDlg::OnButtonContactNewDialTelephone1()
{
	CString telephone;
	m_edtTelephone1.GetWindowText(telephone);
	if (telephone.IsEmpty())
	{
		return;
	}
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(Util::StringOp::FromCString(telephone), m_pContact->id());
}

void CContactNewDlg::OnButtonContactNewDialTelephone2()
{
	CString telephone;
	m_edtTelephone2.GetWindowText(telephone);
	if (telephone.IsEmpty())
	{
		return;
	}
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(Util::StringOp::FromCString(telephone), m_pContact->id());
}
