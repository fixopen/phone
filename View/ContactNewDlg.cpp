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
	ON_WM_CTLCOLOR()
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
	std::string strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWCARDBTN);
	CString str = strTemp.c_str();
	m_sticTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0, 0, 429, 20), this);
	m_sticTitle.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticTitle.SetLeftMargin(25);

	m_sticSepLine1.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(244, 0, 245, 204), this);
	m_sticSepLine1.SetColor(RGB(78, 165, 229), RGB(255, 255, 255));
	m_sticSepLine2.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(429, 0, 430, 204), this);
	m_sticSepLine2.SetColor(RGB(255, 255, 255), RGB(255, 255, 255));

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_NAMESTC);
	str = strTemp.c_str();
	m_sticName.Create(str, WS_CHILD|WS_VISIBLE, CRect(6, 25, 36, 40), this);
	m_sticName.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_MOBILESTC);
	str = strTemp.c_str();
	m_sticMobile1.Create(str, WS_CHILD|WS_VISIBLE, CRect(6, 45, 36, 60), this);
	m_sticMobile1.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_MOBILESTC);
	str = strTemp.c_str();
	m_sticMobile2.Create(str, WS_CHILD|WS_VISIBLE, CRect(6, 65, 36, 80), this);
	m_sticMobile2.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_TELPHONESTC);
	str = strTemp.c_str();
	m_sticTelephone1.Create(str, WS_CHILD|WS_VISIBLE, CRect(6, 85, 36, 100), this);
	m_sticTelephone1.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));
	
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_TELPHONESTC);
	str = strTemp.c_str();
	m_sticTelephone2.Create(str, WS_CHILD|WS_VISIBLE, CRect(6, 105, 36, 120), this);
	m_sticTelephone2.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DIAL);
	str = strTemp.c_str();
	m_btnDialMobile1.Create(str, WS_CHILD|WS_VISIBLE, CRect(190, 46, 240, 64), this, IDC_BUTTON_CONTACTNEW_DIALMOBILE1);
	m_btnDialMobile1.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(194,235,149));
	m_btnDialMobile1.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(194,235,149));
	m_btnDialMobile1.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(194,235,149));

	m_btnDialMobile2.Create(str, WS_CHILD|WS_VISIBLE, CRect(190, 66, 240, 84), this, IDC_BUTTON_CONTACTNEW_DIALMOBILE2);
	m_btnDialMobile2.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(194,235,149));
	m_btnDialMobile2.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(194,235,149));
	m_btnDialMobile2.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(194,235,149));

	m_btnDialTelephone1.Create(str, WS_CHILD|WS_VISIBLE, CRect(190, 86, 240, 104), this, IDC_BUTTON_CONTACTNEW_DIALTEL1);
	m_btnDialTelephone1.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(194,235,149));
	m_btnDialTelephone1.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(194,235,149));
	m_btnDialTelephone1.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(194,235,149));

	m_btnDialTelephone2.Create(str, WS_CHILD|WS_VISIBLE, CRect(190, 106, 240, 124), this, IDC_BUTTON_CONTACTNEW_DIALTEL2);
	m_btnDialTelephone2.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(194,235,149));
	m_btnDialTelephone2.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(194,235,149));
	m_btnDialTelephone2.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(194,235,149));

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_EMAILSTC);
	str = strTemp.c_str();
	m_sticEmail.Create(str, WS_CHILD|WS_VISIBLE, CRect(6, 125, 36, 140), this);
	m_sticEmail.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));
	
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_WEBSTC);
	str = strTemp.c_str();
	m_sticSite.Create(str, WS_CHILD|WS_VISIBLE, CRect(6, 145, 36, 160), this);
	m_sticSite.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_GROUPSTC);
	str = strTemp.c_str();
	m_sticGroup.Create(str, WS_CHILD|WS_VISIBLE, CRect(250, 25, 280, 40), this);
	m_sticGroup.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_COMPANYSTC);
	str = strTemp.c_str();
	m_sticCompany.Create(str, WS_CHILD|WS_VISIBLE, CRect(250, 45, 280, 60), this);
	m_sticCompany.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_COMPANYADDRESSSTC);
	str = strTemp.c_str();
	m_sticCompanyAddress.Create(str, WS_CHILD|WS_VISIBLE, CRect(250, 65, 280, 80), this);
	m_sticCompanyAddress.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_POSTCODESTC);
	str = strTemp.c_str();
	m_sticPostCode.Create(str, WS_CHILD|WS_VISIBLE, CRect(250, 85, 280, 100), this);
	m_sticPostCode.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_DEPARTSTC);
	str = strTemp.c_str();
	m_sticDepartment.Create(str, WS_CHILD|WS_VISIBLE, CRect(250, 105, 280, 120), this);
	m_sticDepartment.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_TITLESTC);
	str = strTemp.c_str();
	m_sticDuty.Create(str, WS_CHILD|WS_VISIBLE, CRect(250, 125, 280, 140), this);
	m_sticDuty.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));
	
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_MOTOSTC);
	str = strTemp.c_str();
	m_sticMemo.Create(str, WS_CHILD|WS_VISIBLE, CRect(250, 145, 280, 160), this);
	m_sticMemo.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	m_cmbType.Create(WS_CHILD|WS_VISIBLE, CRect(170, 26, 240, 106), this, IDC_COMBOBOX_CONTACTNEW_TYPE);
	m_cmbGroup.Create(WS_CHILD|WS_VISIBLE, CRect(280, 26, 429, 126), this, IDC_COMBOBOX_CONTACTNEW_GROUP);
			
	m_cmbType.AddString(CString(Data::LanguageResource::Get(Data::RI_CARD_COMNSTC).c_str()));
	m_cmbType.AddString(CString(Data::LanguageResource::Get(Data::RI_CARD_VIPSTC).c_str()));
	m_cmbType.AddString(CString(Data::LanguageResource::Get(Data::RI_CARD_BLACKLISTSTC).c_str()));

	m_edtName.Create(WS_CHILD|WS_VISIBLE, CRect(40, 25, 168, 45), this, IDC_EDIT_CONTACTNEW_NAME);
	m_edtName.SetLimitText(15);
	m_edtMobile1.Create(WS_CHILD|WS_VISIBLE, CRect(40, 45, 188, 65), this, IDC_EDIT_CONTACTNEW_MOBILE1);
	m_edtMobile1.SetLimitText(15);
	m_edtMobile2.Create(WS_CHILD|WS_VISIBLE, CRect(40, 65, 188, 85), this, IDC_EDIT_CONTACTNEW_MOBILE2);
	m_edtMobile2.SetLimitText(15);
	m_edtTelephone1.Create(WS_CHILD|WS_VISIBLE, CRect(40, 85, 188, 105), this, IDC_EDIT_CONTACTNEW_TELEPHONE1);
	m_edtTelephone1.SetLimitText(15);
	m_edtTelephone2.Create(WS_CHILD|WS_VISIBLE, CRect(40, 105, 188, 125), this, IDC_EDIT_CONTACTNEW_TELEPHONE2);
	m_edtTelephone2.SetLimitText(15);
	m_edtEmail.Create(WS_CHILD|WS_VISIBLE, CRect(40, 125, 188, 145), this, IDC_EDIT_CONTACTNEW_EMAIL);
	m_edtEmail.SetLimitText(15);
	m_edtSite.Create(WS_CHILD|WS_VISIBLE, CRect(40, 145, 188, 204), this, IDC_EDIT_CONTACTNEW_SITE);
	m_edtSite.SetLimitText(15);

	m_edtCompany.Create(WS_CHILD|WS_VISIBLE, CRect(284, 45, 429, 65), this, IDC_EDIT_CONTACTNEW_COMPANY);
	m_edtCompany.SetLimitText(15);
	m_edtCompanyAddress.Create(WS_CHILD|WS_VISIBLE, CRect(284, 65, 429, 85), this, IDC_EDIT_CONTACTNEW_ADDRESS);
	m_edtCompanyAddress.SetLimitText(15);
	m_edtPostCode.Create(WS_CHILD|WS_VISIBLE, CRect(284, 85, 429, 105), this, IDC_EDIT_CONTACTNEW_POSTCODE);
	m_edtPostCode.SetLimitText(15);
	m_edtDepartment.Create(WS_CHILD|WS_VISIBLE, CRect(284, 105, 429, 125), this, IDC_EDIT_CONTACTNEW_DEPARTMENT);
	m_edtDepartment.SetLimitText(15);
	m_edtDuty.Create(WS_CHILD|WS_VISIBLE, CRect(284, 125, 429, 145), this, IDC_EDIT_CONTACTNEW_DUTY);
	m_edtDuty.SetLimitText(15);
	m_edtMemo.Create(WS_CHILD|WS_VISIBLE, CRect(284, 145, 429, 204), this, IDC_EDIT_CONTACTNEW_MEMO);
	m_edtMemo.SetLimitText(15);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_OPERATORSTC);
	str = strTemp.c_str();
	m_sticOperation.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 0, 480, 20), this);
	m_sticOperation.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticOperation.SetLeftMargin(10);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 20, 480, 40), this, IDC_BUTTON_CONTACTNEW_OK);
	SetButtonDefaultColor(&m_btnOk);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 40, 480, 60), this, IDC_BUTTON_CONTACTNEW_CANCEL);
	SetButtonDefaultColor(&m_btnCancel);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_DELETECARDBTN);
	str = strTemp.c_str();
	m_btnDelete.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 184, 480, 204), this, IDC_BUTTON_CONTACTNEW_DELETE);
	SetButtonDefaultColor(&m_btnDelete);

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
}

void CContactNewDlg::OnButtonContactNewDelete() 
{
	// TODO: Add your control notification handler code here
	if (m_pContact->id() != 0)
	{
		ShowWindow(FALSE);
		m_pContact->Remove();
		((CContactDlg*)m_pParentWnd)->ShowItemsInList();
	}
}

HBRUSH CContactNewDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	HBRUSH b=CreateSolidBrush(RGB(255,255,255));
	return b;
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CContactNewDlg::SetContact(boost::shared_ptr<Data::Contact> contact)
{
	m_pContact = contact;
	std::string strTemp;
	if (contact->id() == 0)
	{
		strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWCARDSTC);
	}
	else
	{
		strTemp = Data::LanguageResource::Get(Data::RI_CARD_EDITCARDSTC);
	}
	CString str = strTemp.c_str();
	m_sticTitle.SetWindowText(str);

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
