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
#include "sip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContactNewDlg dialog


CContactNewDlg::CContactNewDlg(CWnd* pParent /*=NULL*/)
: CDialog(CContactNewDlg::IDD, pParent) {
    m_vSimID.resize(300);
    m_bSIMGroup = false ;
}


void CContactNewDlg::DoDataExchange(CDataExchange* pDX) {
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
    ON_MESSAGE(WM_COMMBOX_CLICKED, OnComboSelect)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContactNewDlg message handlers
LRESULT CContactNewDlg::OnClickMJPG(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    CString icon ;
    switch (w) {
    case 10:
        //手机1
        OnButtonContactNewDialMobile1();
        break;

    case 11:
        //视频
        break;

    case 12:
        //发信息
        {
            CString s;
            m_edtMobile1.GetWindowText(s);
            if (s != "") {
                std::vector<CString> telNum ;
                std::vector<CString> vname ;
                CSMSDlg* pWnd_ = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_p3GSMSDlg;
                //pWnd_->initDataBase(SMS_NEW, -1, FALSE);
                //pWnd_->SetSMSDetail(s, "");

                CString name;
                m_edtName.GetWindowText(name);
                if (!name.IsEmpty()) {
                    vname.push_back(name);
                } else {
                    vname.push_back(s);
                }

                telNum.push_back(s);
                pWnd_->SetSender(vname);
                pWnd_->SetAppend(telNum);
                pWnd_->ShowWindow(SW_SHOW);

                icon = Allicon[1];
                main->AddIcon(icon);
            }
        }
        break;

    case 13:
        //手机2
        OnButtonContactNewDialMobile2();
        break;

    case 14:
        //固话1
        OnButtonContactNewDialTelephone1();
        break;

    case 15:
        //固话2
        OnButtonContactNewDialTelephone2();
        break;

    case 1000:
        //返回
        OnButtonContactNewCancel();
        break;

    case 1001:
        //确定
        OnButtonContactNewOk();
        break;

    default:
        break;
    }
    return result;
}
BOOL CContactNewDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here

    std::string strTemp;
    CString str;

    int xbegin, ybegin;
    int yspace;
    int width, height;
    xbegin = 92;
    ybegin = 45;
    width = 157;
    height = 34;
    yspace = 16; 

    m_cmbType.Create(WS_CHILD | WS_VISIBLE, CRect(xbegin + width, ybegin - 2, xbegin + width + 148, ybegin - 2 + 342), this, IDC_COMBOBOX_CONTACTNEW_TYPE);
    m_cmbGroup.Create(WS_CHILD | WS_VISIBLE, CRect(397, ybegin + yspace + height - 3, 397 + 237, ybegin + yspace + height - 3 + 342), this, IDC_COMBOBOX_CONTACTNEW_GROUP);

    m_cmbType.AddString(CString(Data::LanguageResource::Get(Data::RI_CARD_COMNSTC).c_str()));
    m_cmbType.AddString(CString(Data::LanguageResource::Get(Data::RI_CARD_VIPSTC).c_str()));
    m_cmbType.AddString(CString(Data::LanguageResource::Get(Data::RI_CARD_BLACKLISTSTC).c_str()));

    //姓名
    m_edtName.Create(WS_CHILD | WS_VISIBLE, CRect(xbegin, ybegin, xbegin + width, ybegin + height), this, IDC_EDIT_CONTACTNEW_NAME);
    m_edtName.SetWindowText(L"");
    m_edtName.SetLimitText(15);

    //移动电话1
    ybegin += yspace + height ; 
    m_edtMobile1.Create(WS_CHILD | WS_VISIBLE, CRect(xbegin, ybegin, xbegin + width, ybegin + height), this, IDC_EDIT_CONTACTNEW_MOBILE1);
    m_edtMobile1.SetLimitText(15);
    m_edtMobile1.SetLimitDiagital();

    //移动电话2
    ybegin += yspace + height ;
    m_edtMobile2.Create(WS_CHILD | WS_VISIBLE, CRect(xbegin, ybegin, xbegin + width, ybegin + height), this, IDC_EDIT_CONTACTNEW_MOBILE2);
    m_edtMobile2.SetLimitText(15);
    m_edtMobile2.SetLimitDiagital();

    //固定电话1
    ybegin += yspace + height ;
    m_edtTelephone1.Create(WS_CHILD | WS_VISIBLE, CRect(xbegin, ybegin, xbegin + width, ybegin + height), this, IDC_EDIT_CONTACTNEW_TELEPHONE1);
    m_edtTelephone1.SetLimitText(15);
    m_edtTelephone1.SetLimitDiagital();

    //固定电话2
    ybegin += yspace + height ;
    m_edtTelephone2.Create(WS_CHILD | WS_VISIBLE, CRect(xbegin, ybegin, xbegin + width, ybegin + height), this, IDC_EDIT_CONTACTNEW_TELEPHONE2);
    m_edtTelephone2.SetLimitText(15);
    m_edtTelephone2.SetLimitDiagital();

    //网址
    ybegin += yspace + height ;
    m_edtSite.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(xbegin, ybegin, xbegin + width, ybegin + height), this, IDC_EDIT_CONTACTNEW_SITE);
    m_edtSite.SetLimitText(30);

    //右边
    //单位
    ybegin = 45 + (height + yspace) * 2;
    xbegin = 398;
    width = 233;
    m_edtCompany.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(xbegin, ybegin, xbegin + width, ybegin + height), this, IDC_EDIT_CONTACTNEW_COMPANY);
    m_edtCompany.SetLimitText(30);

    //地址
    ybegin += yspace + height ;
    m_edtCompanyAddress.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(xbegin, ybegin, xbegin + width, ybegin + height), this, IDC_EDIT_CONTACTNEW_ADDRESS);
    m_edtCompanyAddress.SetLimitText(30);

    //邮编
    ybegin += yspace + height ;
    m_edtPostCode.Create(WS_CHILD | WS_VISIBLE, CRect(xbegin, ybegin, xbegin + width, ybegin + height), this, IDC_EDIT_CONTACTNEW_POSTCODE);
    m_edtPostCode.SetLimitText(6);
    m_edtPostCode.SetLimitDiagital();

    //邮件
    ybegin += yspace + height ;
    m_edtEmail.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(xbegin, ybegin, xbegin + width, ybegin + height), this, IDC_EDIT_CONTACTNEW_EMAIL);
    m_edtEmail.SetLimitText(30);

    //存储控件
    //  ybegin += yspace+height ;
    //  m_comStorage.Create(WS_CHILD|WS_VISIBLE,
    //  CRect(xbegin, ybegin, xbegin+width, ybegin+height), this, IDC_COMBOBOX_SETTING_RINGTIMES);

    //备注
    ybegin += yspace + height ;
    xbegin = 92;
    width = 157;
    m_edtMemo.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(xbegin, ybegin, xbegin + width, ybegin + height), this, IDC_EDIT_CONTACTNEW_MEMO);
    m_edtMemo.SetLimitText(30);


    m_edtDepartment.Create(WS_CHILD | ES_AUTOHSCROLL, CRect(475 - 8, 271 - 4, 675 - 8, 301 - 4), this, IDC_EDIT_CONTACTNEW_DEPARTMENT);
    m_edtDepartment.SetLimitText(30);

    m_edtDuty.Create(WS_CHILD, CRect(475 - 8, 320 - 4, 675 - 8, 350 - 4), this, IDC_EDIT_CONTACTNEW_DUTY);
    m_edtDuty.SetLimitText(15); 

    m_MJPGList.Create(L"", WS_VISIBLE | WS_CHILD, CRect(0, 0, 800, 423), this);
    m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\新建名片.xml");
    m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));

    m_MJPGList.SetUnitFont(20, font_24);

    MoveWindow(0, 57, 800, 423);

    m_pPinYinIndex = LoadPinYinIndex();
    m_pContact = boost::shared_ptr<Data::Contact>();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CContactNewDlg::SetButtonDefaultColor(CCEButtonST* button) {
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248, 214, 147));
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248, 214, 147));
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248, 214, 147));
}

void CContactNewDlg::SetButtonSelectedColor(CCEButtonST* button) {
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248, 214, 147));
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248, 214, 147));
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248, 214, 147));
}

void CContactNewDlg::AddGroupItems(void) {
    m_cmbGroup.ResetContent();
    std::string filter = "[name] <> 'SIM卡'" ;
    if (0 == m_pContact->id())//新建
    {
        filter = "" ;
    }

    if (m_bSIMGroup) {
        m_cmbGroup.AddString(L"SIM卡");
        filter = "[name] = 'SIM卡'";
        std::vector<boost::shared_ptr<Data::ContactGroup> > result = Data::ContactGroup::GetFromDatabase(filter);
        if (!result.empty()) {
            m_cmbGroup.SetItemData(0, result[0]->id());
        }
    } else {
        std::string strTemp;
          strTemp = Data::LanguageResource::Get(Data::RI_CARD_ALLCARDSTC);
        CString str = Util::StringOp::ToCString(strTemp);
          m_cmbGroup.AddString(str);
          m_cmbGroup.SetItemData(0, 0);

        std::vector<boost::shared_ptr<Data::ContactGroup> > result = Data::ContactGroup::GetFromDatabase(filter);
        for (size_t i = 0; i < result.size(); ++i) {
            m_cmbGroup.AddString(CString(result[i]->name().c_str()));
            m_cmbGroup.SetItemData(i + 1, result[i]->id());
        }
    }
}

void CContactNewDlg::OnButtonContactNewOk() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd; 
    std::vector<Util::ATCommandWarp::SIM_FORMAT> vsim ;
    Util::ATCommandWarp::SIM_FORMAT sim ;
    std::string tel;

    CString s;
    m_edtName.GetWindowText(s);
    if (s.IsEmpty()) {
        return;
    }

    if (!m_pContact) {
        m_pContact = boost::shared_ptr<Data::Contact>(new Data::Contact);
    }   

    m_pContact->name(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));
    m_pContact->alias(GetAlias(m_pContact->name()));
    sim.name = m_pContact->name();

    m_edtMobile1.GetWindowText(s);
    m_pContact->mobilesTelephone(Data::TelephoneNumber(ReplaceSingleQuotation(Util::StringOp::FromCString(s))));
    tel = m_pContact->mobilesTelephone().number();

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

    //  m_edtDepartment.GetWindowText(s);
    //  m_pContact->department(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));

    //  m_edtDuty.GetWindowText(s);
    //  m_pContact->duty(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));

    m_edtMemo.GetWindowText(s);
    m_pContact->memo(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));

    //类型
    if (m_cmbType.GetCurSel() == CB_ERR) {
        m_pContact->type(Data::ctNormal);
    } else {
        m_pContact->type(Data::ContactType(m_cmbType.GetCurSel()));
    }

    // 类别
    //add by qi 2009_09_26
    CString groupname(L"");
    if (m_cmbGroup.GetCurSel() == CB_ERR) {
        m_pContact->groupId(0);
    } else {
        m_pContact->groupId(m_cmbGroup.GetItemData(m_cmbGroup.GetCurSel()));

        //add by qi 2009_09_26
        m_cmbGroup.GetWindowText(groupname);
    }

    //处理SIM卡组
    int id ;
    if (m_bSIMGroup) {
        if (tel.empty()) {
            return ;
        }

        sim.telnum = tel;

        if (m_pContact->id() == 0)//插入
        {
            //SIM卡也同步
            for (size_t i = 0 ; i < m_vSimID.size() ; i++) {
                m_vSimID[i] = 0 ;
            }

            std::string filter = "groupId = 1";//sim卡
            std::vector<boost::shared_ptr<Data::Contact> > result;
            result = Data::Contact::GetFromDatabase(filter);
            if (!result.empty())//查找sim卡中的空闲序号
            {
                for (size_t i = 0 ; i < result.size() ; i++) {
                    CString memo = result[i]->memo().c_str();
                    int id = atoi(memo);
                    m_vSimID[id - 1] = id ;
                }
            }

            for (size_t i = 0 ; i < m_vSimID.size() ; i++) {
                if (0 == m_vSimID[i]) {
                    id = i + 1;
                    sim.id = Util::StringOp::FromInt(id);
                    break   ;
                }
            }

            //获得SIM卡的容量，如果容量满了,警告
            int capacity = main->m_pATCommandWarp1->GetSimCapacity();
            if (id >= capacity) {
                main->m_pWarningNoFlashDlg->SetTitle(L"SIM卡已满!");
                main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
                return ;
            }
        }
    }

    if (m_pContact->id() == 0)//插入
    {
        if (m_bSIMGroup) {
            vsim.push_back(sim);
            m_pContact->memo(Util::StringOp::FromInt(id));//现在备注记录信息在名片中的ID
            m_pContact->Insert();

            //插入sim
            main->m_pATCommandWarp1->Save2Sim(vsim);
        } else {
            m_pContact->Insert();
        }
    } else//更新
    {
          //sim卡组中的电话只有一项

        if (m_bSIMGroup) {
            //先删除，后插入
            int id = atoi(CString(m_pContact->memo().c_str()));
            sim.id = Util::StringOp::FromInt(id);
            vsim.push_back(sim);

            main->m_pATCommandWarp1->DeleteSim(id);
            main->m_pATCommandWarp1->Save2Sim(vsim);

            m_pContact->memo(Util::StringOp::FromInt(id));//现在备注记录信息在名片中的ID
        }
          m_pContact->Update();
    }

    //显示新建的信息
    main->m_pContactDlg->ShowNewContact(m_pContact->groupId());

    ShowWindow(FALSE);
    SipShowIM(SIPF_OFF);
}

void CContactNewDlg::OnButtonContactNewCancel() {
    // TODO: Add your control notification handler code here
    ShowWindow(FALSE);
    SipShowIM(SIPF_OFF);
}

void CContactNewDlg::OnButtonContactNewDelete() {
    // TODO: Add your control notification handler code here
    if (m_pContact->id() > 0) {
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
        std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACT);
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
    }
}

void CContactNewDlg::SetContact(boost::shared_ptr<Data::Contact> contact) {
    m_pContact = contact;
    std::string strTemp;

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

    if (0 == contact->id())//新建 add by qi 2009_11_2
    {
        m_bSIMGroup = false ;
        m_MJPGList.SetUnitText(20, L"", true);
    } else {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd; 
        if (contact->groupId() == main->m_pContactDlg->GetSimID()) {
            m_MJPGList.SetUnitText(20, L"SIM卡", true);
            m_bSIMGroup = true ;
        } else {
            m_MJPGList.SetUnitText(20, L"本地", true);
            m_bSIMGroup = false ;
        }
    }

    if (m_cmbType.SetCurSel(contact->type()) == CB_ERR) {
        m_cmbType.SetCurSel(Data::ctNormal);
    }
    AddGroupItems();

    int id = contact->groupId();
    std::string filter;
    filter = "id = " + Util::StringOp::FromInt(id);
    std::vector<boost::shared_ptr<Data::ContactGroup> > result = Data::ContactGroup::GetFromDatabase(filter);
    /*  if (!result.empty())
    {
    int index = m_cmbGroup.SelectString(0, CString(result[0]->name().c_str()));
    if (index == CB_ERR)
    {
    m_cmbGroup.SetCurSel(0);
    }
    }
    else
    {
    m_cmbGroup.SetCurSel(0);
    }
    */
    if (!result.empty()) {
        CString group = Util::StringOp::ToCString(result[0]->name());
        CString txt;
        for (int i = 0; i < m_cmbGroup.GetCount(); i++) {
            m_cmbGroup.GetLBText(i, txt);
            if (group == txt) {
                m_cmbGroup.SetCurSel(i);
                break;
            } else {
                m_cmbGroup.SetCurSel(0);
            }
        }
    } else {
        m_cmbGroup.SetCurSel(0);
    }
}

/*
void CContactNewDlg::SetContact(boost::shared_ptr<Data::Contact> contact)

if (!result.empty())
{
CString group = Util::StringOp::ToCString(result[0]->name());
CString txt;
for (int i = 0; i < m_cmbGroup.GetCount(); i++)
{
m_cmbGroup.GetLBText(i, txt);
if (group == txt)
{
m_cmbGroup.SetCurSel(i);
break;
}
else
{
m_cmbGroup.SetCurSel(0);
}
}
}
else
{
m_cmbGroup.SetCurSel(0);
}
}
*/
char* CContactNewDlg::LoadPinYinIndex(void) {
    CFile m_pHexFile;   


    m_pHexFile.Open(hive_res_firstchar, CFile::modeRead | CFile::typeBinary);   

    if (!m_pHexFile) {
        return NULL;
    }   
    int nFilelen = (int)m_pHexFile.GetLength();   
    char* szBuf;   
    szBuf = new char[nFilelen];   
    int nRet = m_pHexFile.Read(szBuf, nFilelen);   
    if (nRet <= 0) {
        return NULL;
    }   
    m_pHexFile.Close();  
    return szBuf;
}

std::string CContactNewDlg::GetAlias(std::string name) {
    if (m_pPinYinIndex == NULL) {
        return "";
    }

    std::string alias;
    for (size_t i = 0; i < name.size(); ++i) {
        if ((unsigned char)name[i] > (unsigned char)127) {
            unsigned int index = ((unsigned char)name[i] - 0xb0) * 94 + ((unsigned char)name[i + 1] - 0xa1);
            if (index >= 0 && index < 6768) {
                alias += m_pPinYinIndex[index];
            } else {
            }
            ++i;
        } else {
            if (name[i] >= 'A' && name[i] <= 'Z') {
                alias += char(name[i] + 32);
            } else {
                alias += name[i];
            }
        }
    }
    return alias;
}

std::string CContactNewDlg::ReplaceSingleQuotation(std::string& field) {
    for (size_t i = 0; i < field.length(); ++i) {
        if (field[i] == '\'') {
            field[i] = '\"';
        }
    }
    return field;
}

void CContactNewDlg::OnButtonContactNewDialMobile1() {
    CString mobile;
    m_edtMobile1.GetWindowText(mobile);
    if (mobile.IsEmpty()) {
        return;
    }
    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(Util::StringOp::FromCString(mobile), m_pContact->id());
}

void CContactNewDlg::OnButtonContactNewDialMobile2() {
    CString mobile;
    m_edtMobile2.GetWindowText(mobile);
    if (mobile.IsEmpty()) {
        return;
    }
    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(Util::StringOp::FromCString(mobile), m_pContact->id());
}

void CContactNewDlg::OnButtonContactNewDialTelephone1() {
    CString telephone;
    m_edtTelephone1.GetWindowText(telephone);
    if (telephone.IsEmpty()) {
        return;
    }
    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(Util::StringOp::FromCString(telephone), m_pContact->id());
}

void CContactNewDlg::OnButtonContactNewDialTelephone2() {
    CString telephone;
    m_edtTelephone2.GetWindowText(telephone);
    if (telephone.IsEmpty()) {
        return;
    }
    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(Util::StringOp::FromCString(telephone), m_pContact->id());
}

LRESULT CContactNewDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
    // TODO: Add your specialized code here and/or call the base class
    switch (message) {
    case WM_DELETESELITEM:
        m_pContact->Remove();
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pContactDlg->ShowItemsInList();
        ShowWindow(FALSE);
        break;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CContactNewDlg::PreTranslateMessage(MSG* pMsg) {
    return CDialog::PreTranslateMessage(pMsg);
}


void CContactNewDlg::ClearEdit() {
    //  m_edtName;
    //  m_edtMobile1;

    m_edtMobile2.SetWindowText(L"");
    m_edtMobile2.SetReadOnly(true);

    m_edtTelephone1.SetWindowText(L"");
    m_edtTelephone1.SetReadOnly(true);

    m_edtTelephone2.SetWindowText(L"");
    m_edtTelephone2.SetReadOnly(true);

    m_edtEmail.SetWindowText(L"");
    m_edtEmail.SetReadOnly(true);

    m_edtSite.SetWindowText(L"");
    m_edtSite.SetReadOnly(true);

    m_edtCompany.SetWindowText(L"");
    m_edtCompany.SetReadOnly(true);

    m_edtCompanyAddress.SetWindowText(L"");
    m_edtCompanyAddress.SetReadOnly(true);

    m_edtPostCode.SetWindowText(L"");
    m_edtPostCode.SetReadOnly(true);

    //  m_edtTelephone1;
    //  m_edtTelephone2;
    //  m_edtEmail;
    //  m_edtSite;
    //  m_edtCompany;
    //  m_edtCompanyAddress;
    //  m_edtPostCode;
    //  
    //  m_edtDepartment;    
    //  m_edtDuty;
    //  m_edtMemo;
}

void CContactNewDlg::SetSim(bool btrue) {
    m_bSIMGroup = btrue ;

    m_edtMobile2.SetWindowText(L"");
    m_edtMobile2.SetReadOnly(btrue);

    m_edtTelephone1.SetWindowText(L"");
    m_edtTelephone1.SetReadOnly(btrue);

    m_edtTelephone2.SetWindowText(L"");
    m_edtTelephone2.SetReadOnly(btrue);
}
LRESULT CContactNewDlg::OnComboSelect(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    CString content;
    m_cmbGroup.GetWindowText(content);
    if (0 == content.Compare(L"SIM卡")) {
        m_MJPGList.SetUnitText(20, L"SIM卡", TRUE);
        SetSim(true);
    } else {
        m_MJPGList.SetUnitText(20, L"本地", TRUE);
        SetSim(false);
    }
    return result;
}

void CContactNewDlg::ShowWindow_() {
    m_bSIMGroup = false ;

    m_edtName.SetWindowText(L"");
    m_edtName.SetReadOnly(false);

    m_edtMobile1.SetWindowText(L"");
    m_edtMobile1.SetReadOnly(false);

    m_edtMobile2.SetWindowText(L"");
    m_edtMobile2.SetReadOnly(false);

    m_edtTelephone1.SetWindowText(L"");
    m_edtTelephone1.SetReadOnly(false);

    m_edtTelephone2.SetWindowText(L"");
    m_edtTelephone2.SetReadOnly(false);

    m_edtEmail.SetWindowText(L"");
    m_edtEmail.SetReadOnly(false);

    m_edtSite.SetWindowText(L"");
    m_edtSite.SetReadOnly(false);

    m_edtCompany.SetWindowText(L"");
    m_edtCompany.SetReadOnly(false);

    m_edtCompanyAddress.SetWindowText(L"");
    m_edtCompanyAddress.SetReadOnly(false);

    m_edtPostCode.SetWindowText(L"");
    m_edtPostCode.SetReadOnly(false);

    m_edtMemo.SetWindowText(L"");//备注
    m_edtMemo.SetReadOnly(false);

    m_MJPGList.SetUnitText(20, L"", TRUE);
    ShowWindow(SW_SHOW);
}