// ContactNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ContactNewDlg.h"
#include "ContactDlg.h"

#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/ContactGroup.h"
#include "../Data/ContactInfo.h"
#include "../Data/SkinStyle.h"
//#include <sip.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace View {
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
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CContactNewDlg message handlers
    LRESULT CContactNewDlg::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        switch (w)
        {
        case 1:
            OnButtonContactNewDialMobile1();
            break;
        case 2:
            OnButtonContactNewDialMobile2();
            break;
        case 3:
            OnButtonContactNewDialTelephone1();
            break;
        case 4:
            OnButtonContactNewDialTelephone2();
            break;
        case 5:
            OnButtonContactNewOk();
            break;
        case 6:
            OnButtonContactNewCancel();
            break;
        case 7:
            OnButtonContactNewDelete();
            break;
        }
        return result;
    }

    BOOL CContactNewDlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();

        // TODO: Add extra initialization here

        std::string strTemp;
        CString str;

		m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0*X_XISHU, 0*Y_XISHU, 800*X_XISHU, 420*Y_XISHU), this);
		m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\新建名片.xml"));
		m_MJPGList.SetMJPGRect(CRect(0*X_XISHU, 0*Y_XISHU, 800*X_XISHU, 420*Y_XISHU));

        m_cmbType.Create(WS_CHILD|WS_VISIBLE, CRect(292*X_XISHU, 68*Y_XISHU, 410*X_XISHU, 410*Y_XISHU), &m_MJPGList, IDC_COMBOBOX_CONTACTNEW_TYPE);
        m_cmbGroup.Create(WS_CHILD|WS_VISIBLE, CRect(468*X_XISHU, 68*Y_XISHU, 682*X_XISHU, 410*Y_XISHU), &m_MJPGList, IDC_COMBOBOX_CONTACTNEW_GROUP);

        m_cmbType.AddString(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_COMNSTC)));
        m_cmbType.AddString(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_VIPSTC)));
        m_cmbType.AddString(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_BLACKLISTSTC)));

        m_edtName.Create(WS_CHILD|WS_VISIBLE, CRect(80*X_XISHU, 75*Y_XISHU, 280*X_XISHU, 105*Y_XISHU), &m_MJPGList, IDC_EDIT_CONTACTNEW_NAME);
        m_edtName.SetWindowText(L"");
        m_edtName.SetLimitText(15);
        m_edtMobile1.Create(WS_CHILD|WS_VISIBLE, CRect(80*X_XISHU, 123*Y_XISHU, 280*X_XISHU, 153*Y_XISHU), &m_MJPGList, IDC_EDIT_CONTACTNEW_MOBILE1);
        m_edtMobile1.SetLimitText(15);
        m_edtMobile2.Create(WS_CHILD|WS_VISIBLE, CRect(80*X_XISHU, 173*Y_XISHU, 280*X_XISHU, 203*Y_XISHU), &m_MJPGList, IDC_EDIT_CONTACTNEW_MOBILE2);
        m_edtMobile2.SetLimitText(15);
        m_edtTelephone1.Create(WS_CHILD|WS_VISIBLE, CRect(80*X_XISHU, 222*Y_XISHU, 280*X_XISHU, 252*Y_XISHU), &m_MJPGList, IDC_EDIT_CONTACTNEW_TELEPHONE1);
        m_edtTelephone1.SetLimitText(15);
        m_edtTelephone2.Create(WS_CHILD|WS_VISIBLE, CRect(80*X_XISHU, 271*Y_XISHU, 280*X_XISHU, 301*Y_XISHU), &m_MJPGList, IDC_EDIT_CONTACTNEW_TELEPHONE2);
        m_edtTelephone2.SetLimitText(15);

        m_edtMobile1.SetLimitDiagital();
        m_edtMobile2.SetLimitDiagital();
        m_edtTelephone1.SetLimitDiagital();
        m_edtTelephone2.SetLimitDiagital();

        m_edtEmail.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(80*X_XISHU, 320*Y_XISHU, 280*X_XISHU, 350), &m_MJPGList, IDC_EDIT_CONTACTNEW_EMAIL);
        m_edtEmail.SetLimitText(30);
        m_edtSite.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(80*X_XISHU, 369*Y_XISHU, 280*X_XISHU, 399*Y_XISHU), &m_MJPGList, IDC_EDIT_CONTACTNEW_SITE);
        m_edtSite.SetLimitText(30);

        m_edtCompany.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(475*X_XISHU, 123*Y_XISHU, 675*X_XISHU, 153*Y_XISHU), &m_MJPGList, IDC_EDIT_CONTACTNEW_COMPANY);
        m_edtCompany.SetLimitText(30);
        m_edtCompanyAddress.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(475*X_XISHU, 173*Y_XISHU, 675*X_XISHU, 203*Y_XISHU), &m_MJPGList, IDC_EDIT_CONTACTNEW_ADDRESS);
        m_edtCompanyAddress.SetLimitText(30);
        m_edtPostCode.Create(WS_CHILD|WS_VISIBLE, CRect(475*X_XISHU, 222*Y_XISHU, 675*X_XISHU, 252*Y_XISHU), &m_MJPGList, IDC_EDIT_CONTACTNEW_POSTCODE);
        m_edtPostCode.SetLimitText(6);
        m_edtPostCode.SetLimitDiagital();

        m_edtDepartment.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(475*X_XISHU, 271*Y_XISHU, 675*X_XISHU, 301*Y_XISHU), &m_MJPGList, IDC_EDIT_CONTACTNEW_DEPARTMENT);
        m_edtDepartment.SetLimitText(30);
        m_edtDuty.Create(WS_CHILD|WS_VISIBLE, CRect(475*X_XISHU, 320*Y_XISHU, 675*X_XISHU, 350*Y_XISHU), &m_MJPGList, IDC_EDIT_CONTACTNEW_DUTY);
        m_edtDuty.SetLimitText(15);
        m_edtMemo.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(475*X_XISHU, 369*Y_XISHU, 675*X_XISHU, 399*Y_XISHU), &m_MJPGList, IDC_EDIT_CONTACTNEW_MEMO);
        m_edtMemo.SetLimitText(30);

        m_pPinYinIndex = LoadPinYinIndex();
        m_pContact = Util::shared_ptr<Data::Contact>();

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
        std::string strTemp;
        strTemp = Data::LanguageResource::Get(Data::RI_CARD_ALLCARDSTC);
        CString str = Util::StringOp::ToCString(strTemp);
        m_cmbGroup.AddString(str);
        m_cmbGroup.SetItemData(0, 0);
        std::vector<Util::shared_ptr<Data::ContactGroup> > result = Data::ContactGroup::GetFromDatabase("");
        for (std::size_t i = 0; i < result.size(); ++i)
        {
            m_cmbGroup.AddString(Util::StringOp::ToCString(result[i]->name()));
            m_cmbGroup.SetItemData(i + 1, result[i]->id());
        }
    }

    void CContactNewDlg::OnButtonContactNewOk() 
    {
        // TODO: Add your control notification handler code here
        CString s;
        m_edtName.GetWindowText(s);
        if (s.IsEmpty())
        {
            return;
        }

        if (!m_pContact)
        {
            m_pContact = Util::shared_ptr<Data::Contact>(new Data::Contact);
        }	

        m_pContact->name(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));
        m_pContact->alias(GetAlias(m_pContact->name()));

        m_edtMobile1.GetWindowText(s);
        m_pContact->mobilesTelephone(Data::TelephoneNumber(ReplaceSingleQuotation(Util::StringOp::FromCString(s))));
        m_edtMobile2.GetWindowText(s);
        m_pContact->mobilesTelephone2(Data::TelephoneNumber(ReplaceSingleQuotation(Util::StringOp::FromCString(s))));
        m_edtTelephone1.GetWindowText(s);
        m_pContact->worksTelephone(Data::TelephoneNumber(ReplaceSingleQuotation(Util::StringOp::FromCString(s))));
        m_edtTelephone2.GetWindowText(s);
        m_pContact->worksTelephone2(Data::TelephoneNumber(ReplaceSingleQuotation(Util::StringOp::FromCString(s))));
        m_edtEmail.GetWindowText(s);
        m_pContact->email(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));
        m_edtSite.GetWindowText(s);
        m_pContact->homesite(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));
        m_edtCompany.GetWindowText(s);
        m_pContact->company(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));
        m_edtCompanyAddress.GetWindowText(s);
        m_pContact->companyAddress(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));
        m_edtPostCode.GetWindowText(s);
        m_pContact->companyPostCode(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));
        m_edtDepartment.GetWindowText(s);
        m_pContact->department(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));
        m_edtDuty.GetWindowText(s);
        m_pContact->duty(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));
        m_edtMemo.GetWindowText(s);
        m_pContact->memo(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));
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

        ShowWindow(FALSE);
        //SipShowIM(SIPF_OFF);

        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowItemsInList();
    }

    void CContactNewDlg::OnButtonContactNewCancel() 
    {
        ShowWindow(FALSE);
        //SipShowIM(SIPF_OFF);
    }

    void CContactNewDlg::OnButtonContactNewDelete() 
    {
        // TODO: Add your control notification handler code here
        if (m_pContact->id() > 0)
        {
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
            std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACT);
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp));
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
        }
    }

    void CContactNewDlg::SetContact(Util::shared_ptr<Data::Contact> contact)
    {
        m_pContact = contact;
        std::string strTemp;
        if (contact->id() == 0)
        {
            //strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWCARDSTC);
            //m_btnDelete.ShowWindow(FALSE);
            m_MJPGList.SetUnitIsShow(7, TRUE);
            m_MJPGList.SetUnitIsShow(8, FALSE);
        }
        else
        {
            //strTemp = Data::LanguageResource::Get(Data::RI_CARD_EDITCARDSTC);
            //m_btnDelete.ShowWindow(TRUE);
            m_MJPGList.SetUnitIsShow(8, TRUE);
            m_MJPGList.SetUnitIsShow(7, FALSE);
        }

        m_edtName.SetWindowText(CString(contact->name().c_str()));
        m_edtMobile1.SetWindowText(CString(contact->mobilesTelephone().number().c_str()));
        m_edtMobile2.SetWindowText(CString(contact->mobilesTelephone2().number().c_str()));
        m_edtTelephone1.SetWindowText(CString(contact->worksTelephone().number().c_str()));
        m_edtTelephone2.SetWindowText(CString(contact->worksTelephone2().number().c_str()));
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

        int id = contact->groupId();
        std::string filter;
        filter = "id = " + Util::StringOp::FromInt(id);
        std::vector<Util::shared_ptr<Data::ContactGroup> > result = Data::ContactGroup::GetFromDatabase(filter);
        if (!result.empty())
        {
            int index = m_cmbGroup.SelectString(0, Util::StringOp::ToCString(result[0]->name()));
            if (index == CB_ERR)
            {
                m_cmbGroup.SetCurSel(0);
            }
        }
        else
        {
            m_cmbGroup.SetCurSel(0);
        }
    }

    char* CContactNewDlg::LoadPinYinIndex(void)
    {
        CFile m_pHexFile;   

        m_pHexFile.Open(_T("C:/flashdrv/res_dat/firstchar.txt"), CFile::modeRead | CFile::typeBinary);   

        if (!m_pHexFile)   
        {   
            return NULL;   
        }   
        ULONGLONG nFilelen = m_pHexFile.GetLength();   
        char* szBuf;   
        szBuf = new char[nFilelen];   
        int nRet = m_pHexFile.Read(szBuf, (UINT)nFilelen);   
        if (nRet <= 0)   
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
        for (std::size_t i = 0; i < name.size(); ++i)
        {
            if ((unsigned char)name[i] > (unsigned char)127)
            {
                unsigned int index = ((unsigned char)name[i] - 0xb0) * 94 + ((unsigned char)name[i+1] - 0xa1);
                if(index >= 0 && index < 6768)
                {
                    alias += m_pPinYinIndex[index];	
                }
                else
                {
                }
                ++i;
            }
            else
            {
                if (name[i] >= 'A' && name[i] <= 'Z')
                {
                    alias += char(name[i] + 32);
                }
                else
                {
                    alias += name[i];
                }
            }
        }
        return alias;
    }

    std::string CContactNewDlg::ReplaceSingleQuotation(std::string& field)
    {
        for (std::size_t i = 0; i < field.length(); ++i)
        {
            if (field[i] == '\'')
            {
                field[i] = '\"';
            }
        }
        return field;
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

    LRESULT CContactNewDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
    {
        // TODO: Add your specialized code here and/or call the base class
        switch (message)
        {
        case WM_DELETESELITEM:
            m_pContact->Remove();
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowItemsInList();
            ShowWindow(FALSE);
            break;
        }
        return CDialog::WindowProc(message, wParam, lParam);
    }
}
