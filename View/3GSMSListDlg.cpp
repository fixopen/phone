// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "3GSMSListDlg.h"
#include "3GSMSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3GSMSListDlg dialog
void ExtractNumber(CString content, std::vector<CString>& vnum)//提取串里的数字串
{
    for (int i = 0 ; i < content.GetLength() ; i++)//提取内容里的数字串
    {
        if (content.GetAt(i) >= '0' && content.GetAt(i) <= '9') {
            content = content.Mid(i);
            for (int j = 0 ; j < content.GetLength() ; j++) {
                if (content.GetAt(j) < '0' || content.GetAt(j) > '9') {
                    CString tel ;
                    tel = content.Mid(0, j);
                    if (tel.GetLength() > 3)//大于3位才保存
                    {
                        vnum.push_back(tel);
                    }
                    content = content.Mid(j);
                    i = 0 ;
                    break ;
                }
            }
        }
    }
}

bool Warning(std::vector<int>vclick, CString tip) {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    //只能选择一个
    int num = 0;
    for (size_t i = 0 ; i < vclick.size() ; i++) {
        if (1 == vclick[i]) {
            num++;
        }
    }

    if (num > 1) {
        main->m_pWarningNoFlashDlg->SetTitle(tip);
        main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
        return true ;
    }
    return false ;
}

#define PAGE_COUNT  6
C3GSMSListDlg::C3GSMSListDlg(CWnd* pParent /*=NULL*/)
: CDialog(C3GSMSListDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(C3GSMSListDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

    m_iTurnOverPages = 0 ;
    m_bSelectAll = false ;
    m_nListCountTotal = 0;
    m_nPageSize = PAGE_COUNT;
    m_strHomeRecordeTelCode = ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->speCode3_.c_str();
}


void C3GSMSListDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(C3GSMSListDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(C3GSMSListDlg, CDialog)
    //{{AFX_MSG_MAP(C3GSMSListDlg)
    ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
    ON_MESSAGE(WM_LISTCTRL_CLICKSUBITEM, OnListCltrlClick)
    //  ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GSMSListDlg message handlers
/*
void C3GSMSListDlg::OnTimer(UINT nIDEvent)
{
if(IsWindowVisible())
{
if(nIDEvent == 1)
{

}
}
CDialog::OnTimer(nIDEvent);
}
*/

void C3GSMSListDlg::initType(SMSBOX_TYPE box_type, SMSMMS_TYPE sms_type, BOOL reDraw) {
    m_nBoxType = box_type;
    m_nSMSType = sms_type;

    char* listTitle[] = {
        ".\\adv\\mjpg\\k1\\common\\3g\\短信-发件人标题.bmp", ".\\adv\\mjpg\\k1\\common\\3g\\短信-收件人标题.bmp",
    };

    if (RECV_TYPE == m_nBoxType)//收件箱
    {
        SetUpBtn(2);
        m_MJPGList.SetUnitBitmap(11, listTitle[0], L"", reDraw);//设置收件人
    }

    if (SEND_TYPE == m_nBoxType)//发件箱
    {
        SetUpBtn(3);
        m_MJPGList.SetUnitBitmap(11, listTitle[1], L"", reDraw);//设置收件人
    }

    if (DRAFT_TYPE == m_nBoxType)//草稿箱
    {
        SetUpBtn(4);
    }

    if (m_nSMSType == SMS_TYPE) {
        m_MJPGList.SetUnitIsDownStatus(100, TRUE);
        m_MJPGList.SetUnitIsDownStatus(101, FALSE);
    }
    if (m_nSMSType == MMS_TYPE) {
        m_MJPGList.SetUnitIsDownStatus(100, FALSE);
        m_MJPGList.SetUnitIsDownStatus(101, TRUE);
    }

    FromDataBase();
}

void C3GSMSListDlg::DoDeleteSMSAll() {
    if (m_nBoxType == HOME_RECORD_TYPE)   //家庭留言   短信中心特殊号
    {
        m_sListFilter = "[group] = " + Util::StringOp::FromInt(Data::Message::gReMoteSMS); //"smscAddress = '" + m_strHomeRecordeTelCode + "'";
    } else {
        if (m_nSMSType == MMS_TYPE) {
            m_sListFilter = "[type] = " + Util::StringOp::FromInt(m_nBoxType);
        } else {
              m_sListFilter = "[group] = " + Util::StringOp::FromInt(m_nBoxType);
        }
    }

    if (m_nSMSType == MMS_TYPE)         //彩信
    {
        if (Data::MMSData::GetDataCount("") > 0) {
            Data::MMSData::Remove(m_sListFilter);
        }
    } else          //短信及家庭留言
    {
        if (Data::Message::GetDataCount("") > 0) {
            Data::Message::Remove(m_sListFilter);
        }
    }
}

BOOL C3GSMSListDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    m_pSmsReadDlg = new CSMSReadDlg();
    m_pSmsReadDlg->Create(CSMSReadDlg::IDD);
    m_pSmsReadDlg->ShowWindow(SW_HIDE);

    m_pMmsReadDlg = new CMMSReadDlg();
    m_pMmsReadDlg->Create(CMMSReadDlg::IDD);
    m_pMmsReadDlg->ShowWindow(SW_HIDE);

    m_pNumberExtractDlg = new CNumberExtractDlg();
    m_pNumberExtractDlg->Create(CNumberExtractDlg::IDD);
    m_pNumberExtractDlg->ShowWindow_(SW_HIDE);

    m_MJPGList.Create(L"", WS_VISIBLE | WS_CHILD, CRect(0, 0, 800, 423), this);
    m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\收件箱.xml");
    m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));

    m_MJPGList.SetUnitIsDownStatus(2, true);

    SetPagefont();

    MoveWindow(0, 57, 800, 423);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT C3GSMSListDlg::OnClickMJPG(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    CMultimediaPhoneDlg* main = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
    CString icon ;
    switch (w) {
    case 0:
        //写消息    
        main->m_pMainDlg->m_p3GSMSDlg->ShowWindow(SW_SHOW);
        ShowWindow(SW_HIDE);
        break;

    case 1:
        //写彩信息          
        main->m_pMMSDlg->ShowWindow(SW_SHOW);
        ShowWindow(SW_HIDE);
        break ;

    case 2:
        //收件箱    
        main->m_pSMSListDlg->ShowWindow_(RECV_TYPE, SMS_TYPE);
        break;

    case 3:
        //发件箱    
        main->m_pSMSListDlg->ShowWindow_(SEND_TYPE, SMS_TYPE);
        break;

    case 4:
        //草稿箱
        main->m_pDraftDlg->ShowWindow_(DRAFT_TYPE, SMS_TYPE);
        ShowWindow(SW_HIDE);
        break ;

    case 5:
        //设置
        main->m_pSmsSettingDlg->ShowWindow(SW_SHOW);
        ShowWindow(SW_HIDE);
        break;

    case 10:
        //删除
        ShowDeleteDlg();
        break;

    case 11:
        //回复
        Replay();
        break;

    case 12:
        //转发
        Transit();
        break;

    case 13:
        //详情
        Details();
        icon = Allicon[1];
        main->AddIcon(icon);
        break;

    case 14:
        //提取号码
        NumberExtract();
        break;

    case 30:
        //列表第1行
    case 40:
        //列表第2行
    case 50:
        //列表第3行
    case 60:
        //列表第4行
    case 70:
        //列表第5行
    case 80:
        //列表第6行
        ClickedOneItem(w, w / 10 - 3);
        break;

    case 31:
        //点击某行
    case 32:
    case 33:
    case 34:
    case 41:
    case 42:
    case 43:
    case 44:
    case 51:
    case 52:
    case 53:
    case 54:
    case 61:
    case 62:
    case 63:
    case 64:
    case 71:
    case 72:
    case 73:
    case 74:
    case 81:
    case 82:
    case 83:
    case 84:
        ReadOneItem(w / 10 - 3);
        icon = Allicon[1];
        main->AddIcon(icon);
        break;

    case 100:
        //短消息
        ShowSMS(SMS_TYPE);
        break;

    case 101:
        //彩信息
        ShowSMS(MMS_TYPE);
        break;

    case 110:
        //页数显示
        break;

    case 120:
        //上一页
        PageSwitch(up_page);
        break;

    case 121:
        //下一页
        PageSwitch(down_page);
        break;

    case 140:
        //全选
        SelectAll();
        break;

    case 1000:
        ShowWindow(SW_HIDE);        
        main->PopbackIcon();
        break;

    case 1001:
        ShowWindow(SW_HIDE);    
        main->PopbackIcon();        
        break;  

    default:
        break;
    }
    return result;
}


void C3GSMSListDlg::FromDataBase() {
    //设置查询条件
    if (m_nSMSType == MMS_TYPE) {
        m_sListFilter = "[type] = " + Util::StringOp::FromInt(m_nBoxType);
    } else {
        m_sListFilter = "[group] = " + Util::StringOp::FromInt(m_nBoxType);
    }


    if (m_nSMSType == MMS_TYPE)//彩信
    {
        if (Data::MMSData::GetDataCount("") > 0) {
            m_iTotalPages = Data::MMSData::GetDataCount(m_sListFilter) / m_nPageSize;//总页数
            if (Data::MMSData::GetDataCount(m_sListFilter) % m_nPageSize != 0) {
                m_iTotalPages += 1;
            }

            m_vMMSDataCurrentResult = Data::MMSData::GetFromDatabaseByTypeOffsetLength(m_sListFilter, (m_iCurrentPage - 1) * m_nPageSize + 1, m_nPageSize);

            //是否往容器里添加数据
            if (m_iCurrentPage > GetVectorPages()) {
                for (size_t i = 0 ; i < m_vMMSDataCurrentResult.size() ; i++) {
                    if (m_bSelectAll) {
                        m_vClick.push_back(1);
                    } else {
                        m_vClick.push_back(0);
                    }
                    m_vID.push_back(m_vMMSDataCurrentResult[i]->id());
                }
            }
        } else {
            m_iTotalPages = 0;
            m_vMMSDataCurrentResult.clear();
        }
    } else//短信及家庭留言
    {
        if (Data::Message::GetDataCount("") > 0) {
            m_iTotalPages = Data::Message::GetDataCount(m_sListFilter) / m_nPageSize;//总页数
            if (Data::Message::GetDataCount(m_sListFilter) % m_nPageSize != 0) {
                m_iTotalPages += 1;
            }

            m_vMessageCurrentResult = Data::Message::GetFromDatabaseByTypeOffsetLength(m_sListFilter, (m_iCurrentPage - 1) * m_nPageSize + 1, m_nPageSize);

            //是否往容器里添加数据
            if (m_iCurrentPage > GetVectorPages()) {
                for (size_t i = 0 ; i < m_vMessageCurrentResult.size() ; i++) {
                    if (m_bSelectAll) {
                        m_vClick.push_back(1);
                    } else {
                        m_vClick.push_back(0);
                    }
                    m_vID.push_back(m_vMessageCurrentResult[i]->id());
                }
            }
        } else {
            m_vMessageCurrentResult.clear();
            m_iTotalPages = 0;
        }
    }
}

void C3GSMSListDlg::SetUpPages() {
    CString page ;
    page.Format(L"%d/%d", m_iCurrentPage, m_iTotalPages);
    if (0 == m_iTotalPages) {
        page.Format(L"%d/%d", 0, m_iTotalPages);
    }
    m_MJPGList.SetUnitText(110, page, TRUE);//设置当前的页
}


void C3GSMSListDlg::DeleteRefreshList() {
}

void C3GSMSListDlg::ScrollItemsInList(int step, int nPos) {
}

std::string C3GSMSListDlg::GetContactName(std::string const number) {
    if (number == "") {
        return number;
    }
    std::string filter;
    filter += "mobilesTelephone = '";
    filter += Data::TelephoneNumber(number).ToString();
    filter += "' OR mobilesTelephone2 = '";
    filter += Data::TelephoneNumber(number).ToString();
    filter += "' OR worksTelephone = '";
    filter += Data::TelephoneNumber(number).ToString();
    filter += "' OR worksTelephone2 = '";
    filter += Data::TelephoneNumber(number).ToString();
    filter += "'";
    std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase(filter);
    if (!result.empty()) {
        return result[0]->name() ;
    }
    return "";
}

void C3GSMSListDlg::ShowArrayInList() {
    int ibeginID = 30 ;
    int addItems = 0;
    CString temp;
    CString noreadType = L".\\adv\\mjpg\\k5\\common\\短信\\短信未读.png";//没有阅读图标
    CString readType = L".\\adv\\mjpg\\k5\\common\\短信\\短信已读.png";//已经阅读图标
    CString noSend = L".\\adv\\mjpg\\k5\\common\\短信\\短信发送成功失败.png";//没有发送成功的小图标
    CString send = L".\\adv\\mjpg\\k5\\common\\短信\\短信发送成功.png";;//发送成功小图标

    SetUpPages();
    SetUnitStatus();
    if (m_nSMSType == MMS_TYPE) //彩信
    {
        for (size_t i = 0; i < m_vMMSDataCurrentResult.size(); ++i) {
            if (m_vMMSDataCurrentResult[i]->isRead)//在发件箱里，表示是否发送成功。收件箱里表示是否阅读
            {
                if (RECV_TYPE == m_nBoxType)//收件箱
                {
                    m_MJPGList.SetUnitBitmap(ibeginID + addItems + 1, readType, L"", false);
                }

                if (SEND_TYPE == m_nBoxType)//发件箱
                {
                    m_MJPGList.SetUnitBitmap(ibeginID + addItems + 1, send, L"", false);
                }
            } else //已读不标记
            {
                if (RECV_TYPE == m_nBoxType)//收件箱
                {
                    m_MJPGList.SetUnitBitmap(ibeginID + addItems + 1, noreadType, L"", false);
                }

                if (SEND_TYPE == m_nBoxType)//发件箱
                {
                    m_MJPGList.SetUnitBitmap(ibeginID + addItems + 1, noSend, L"", false);
                }
            }

            std::string number ;
            if (m_nBoxType == SEND_TYPE || m_nBoxType == DRAFT_TYPE) {
                number = m_vMMSDataCurrentResult[i]->RecipientAddress;
            } else {
                number = m_vMMSDataCurrentResult[i]->SenderAddress;
            }

            std::string name = GetContactName(number);
            if (!name.empty()) {
                temp = Util::StringOp::ToCString(name);
            } else {
                temp = Util::StringOp::ToCString(number);
            }
            m_MJPGList.SetUnitText(ibeginID + addItems + 2, temp, false);//收件人或者发件人


            temp = Util::StringOp::ToCString(m_vMMSDataCurrentResult[i]->Subject);
            m_MJPGList.SetUnitText(ibeginID + addItems + 3, temp, false);//内容

            CTime tm = CTime(1970, 1, 1, 0, 0, 0);
            tm += CTimeSpan(0, 0, 0, m_vMMSDataCurrentResult[i]->DateAndTime);
            temp = Util::StringOp::FromTimestamp(tm).c_str();
            m_MJPGList.SetUnitText(ibeginID + addItems + 4, temp, false);//时间

            addItems += 10;
        }
    } else //短信
    {
        for (size_t i = 0; i < m_vMessageCurrentResult.size(); ++i) {
            if (m_vMessageCurrentResult[i]->state == Data::Message::sNoRead) {
                if (RECV_TYPE == m_nBoxType)//收件箱
                {
                    m_MJPGList.SetUnitBitmap(ibeginID + addItems + 1, noreadType, L"", false);
                }

                if (SEND_TYPE == m_nBoxType)//发件箱
                {
                    m_MJPGList.SetUnitBitmap(ibeginID + addItems + 1, noSend, L"", false);
                }
            } else {
                if (RECV_TYPE == m_nBoxType)//收件箱
                {
                    m_MJPGList.SetUnitBitmap(ibeginID + addItems + 1, readType, L"", false);
                }

                if (SEND_TYPE == m_nBoxType)//发件箱
                {
                    m_MJPGList.SetUnitBitmap(ibeginID + addItems + 1, send, L"", false);
                }
            }

            std::string number = m_vMessageCurrentResult[i]->remote.address;//在发件箱里，表示是否发送成功。收件箱里表示是否阅读            
            std::string name = GetContactName(number);
            if (!name.empty()) {
                temp = Util::StringOp::ToCString(name);
            } else {
                temp = Util::StringOp::ToCString(number);
            }
            m_MJPGList.SetUnitText(ibeginID + addItems + 2, temp, false);//收件人或者发件人

            temp = Util::StringOp::ToCString(m_vMessageCurrentResult[i]->unicodeData);
            m_MJPGList.SetUnitText(ibeginID + addItems + 3, temp, false);

            temp = Util::StringOp::ToCString(m_vMessageCurrentResult[i]->timestamp.ToString_());
            m_MJPGList.SetUnitText(ibeginID + addItems + 4, temp, false);//时间

            addItems += 10;
        }
    }
}

LRESULT C3GSMSListDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
    // TODO: Add your specialized code here and/or call the base class
    switch (message) {
    case WM_DELETESELITEM:
        {
            DeleteItems();
        }
        break;
    case WM_SCROLL_EDO:
        ScrollItemsInList(wParam, lParam);
        break;
    default:
        break;
    }

    return CDialog::WindowProc(message, wParam, lParam);
}

LRESULT C3GSMSListDlg::OnListCltrlClick(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    CMultimediaPhoneDlg* pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
    if (l == 7)    //详情
    {
        if (w >= 0 && w <= (PAGE_COUNT - 1)) {
            C3GSMSDetailDlg* pWnd_ = pMainDlg->m_pMainDlg->m_p3GSMSDlg->m_pSMSDetailDlg;
            if ((m_nBoxType == DRAFT_TYPE) || (m_nBoxType == SEND_TYPE)) {
                if (m_nSMSType == MMS_TYPE) {
                    pWnd_->initDataBase(MMS_READ, m_vMMSDataCurrentResult[w]->id(), FALSE);
                    pWnd_->ShowWindow(SW_SHOW);
                } else {
                      pWnd_->initDataBase(SMS_READ, m_vMessageCurrentResult[w]->id(), FALSE);
                      pWnd_->ShowWindow(SW_SHOW);
                }
            } else {
                if (m_nSMSType == MMS_TYPE) {
                    pWnd_->initDataBase(MMS_READ, m_vMMSDataCurrentResult[w]->id(), FALSE);
                    m_vMMSDataCurrentResult[w]->isRead = TRUE;
                    m_vMMSDataCurrentResult[w]->Update();
                    pWnd_->ShowWindow(SW_SHOW);
                    ShowArrayInList();
                } else {
                      pWnd_->initDataBase(SMS_READ, m_vMessageCurrentResult[w]->id(), FALSE);
                      m_vMessageCurrentResult[w]->state = Data::Message::sReaded;
                      m_vMessageCurrentResult[w]->Update();
                      pWnd_->ShowWindow(SW_SHOW);
                      ShowArrayInList();
                }
            }
        }
    } else if (l == 6)  //删除
    {
        if (w >= 0 && w <= (PAGE_COUNT - 1)) {
            m_nSelectItem = w;
            ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
            std::string strTemp = ".\\adv\\mjpg\\k1\\common\\确定删除吗.bmp";
            ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
            ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
        }
    } else if (l == 5)  //回复  发送
    {
        if (w >= 0 && w <= (PAGE_COUNT - 1)) {
            CString sTelcode = "";
            CString sContent = "";
            C3GSMSDetailDlg* pWnd_ = pMainDlg->m_pMainDlg->m_p3GSMSDlg->m_pSMSDetailDlg;
            if ((m_nBoxType == DRAFT_TYPE) || (m_nBoxType == SEND_TYPE))  //发送
            {
                if (m_nSMSType == MMS_TYPE) {
                    //todo:彩信
                } else {
                      sTelcode = m_vMessageCurrentResult[w]->remote.address.c_str();
                      sContent = m_vMessageCurrentResult[w]->unicodeData.c_str();

                      pWnd_->initDataBase(SMS_NEW, -1, FALSE);
                      pWnd_->SetSMSDetail(sTelcode, sContent);
                      pWnd_->ShowWindow(SW_SHOW);
                }
            } else {
                if (m_nSMSType == MMS_TYPE) {
                    sTelcode = m_vMMSDataCurrentResult[w]->SenderAddress.c_str();
                } else {
                      sTelcode = m_vMessageCurrentResult[w]->remote.address.c_str();
                }   

                  pWnd_->initDataBase(SMS_NEW, -1, FALSE);
                  pWnd_->SetSMSDetail(sTelcode, sContent);
                  pWnd_->ShowWindow(SW_SHOW);
            }
        }
    } else if (l == 4)  //转发
    {
        if (w >= 0 && w <= (PAGE_COUNT - 1)) {
            CString sTelcode = "";
            CString sContent = "";
            C3GSMSDetailDlg* pWnd_ = pMainDlg->m_pMainDlg->m_p3GSMSDlg->m_pSMSDetailDlg;
            if (m_nSMSType == MMS_TYPE) {
                //  sTelcode = m_vMMSDataCurrentResult[w]->unicodeData.c_str();
            } else {
                sContent = m_vMessageCurrentResult[w]->unicodeData.c_str();
                pWnd_->initDataBase(SMS_NEW, -1, FALSE);
                pWnd_->SetSMSDetail(sTelcode, sContent);
                pWnd_->ShowWindow(SW_SHOW);
            }
        }
    } else if (l == 3)  //回电
    {
        if (w >= 0 && w <= (PAGE_COUNT - 1)) {
            CString sTelcode = "";
            if (m_nSMSType == MMS_TYPE) {
                sTelcode = m_vMMSDataCurrentResult[w]->SenderAddress.c_str();
            } else {
                sTelcode = m_vMessageCurrentResult[w]->remote.address.c_str();
            }
            if (sTelcode != "") {
                ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(Util::StringOp::FromCString(sTelcode), -1);
            }
        }
    }
    return result;
}

void C3GSMSListDlg::SetUpBtn(int ID) {
    m_MJPGList.SetUnitIsDownStatus(ID, TRUE);
    for (int i = 0 ; i < 6; i++) {
        if (i != ID) {
            m_MJPGList.SetUnitIsDownStatus(i, FALSE);
        }
    }
    m_MJPGList.Invalidate();
}

void C3GSMSListDlg::ClearCurrentPage() {
    CString empty(L"");
    int ibegin = 30 ;
    int items = 0 ;
    for (int i = 0 ; i < m_nPageSize ; i++) {
        m_MJPGList.SetUnitIsShow(ibegin + items, false, false);
        m_MJPGList.SetUnitIsDownStatus(ibegin + items, false);

        m_MJPGList.SetUnitBitmap(ibegin + items + 1, L"", L"", false);//状态
        m_MJPGList.SetUnitIsShow(ibegin + items + 1, false, false);

        m_MJPGList.SetUnitText(ibegin + items + 2, empty, false);//发件人
        m_MJPGList.SetUnitIsShow(ibegin + items + 2, false, false);

        m_MJPGList.SetUnitText(ibegin + items + 3, empty, false);//内容
        m_MJPGList.SetUnitIsShow(ibegin + items + 3, false, false);

        m_MJPGList.SetUnitText(ibegin + items + 4, empty, false);//接受时间
        m_MJPGList.SetUnitIsShow(ibegin + items + 4, false, false);

        items = items + 10;
    }   

    m_MJPGList.SetUnitIsShow(140, false, false);

    m_MJPGList.Invalidate();
}

void C3GSMSListDlg::SelectAll() {
    int unitID = 30 ;
    if (!m_bSelectAll) {
        m_bSelectAll = true ;
        for (int i = 0 ; i < GetCurrentItems(); i++) {
            m_MJPGList.SetUnitIsDownStatus(140, true);
            m_MJPGList.SetUnitIsDownStatus(unitID, true);       

            unitID += 10;
        }
    } else {
        m_bSelectAll = false ;
        for (int i = 0 ; i < GetCurrentItems(); i++) {
            m_MJPGList.SetUnitIsDownStatus(140, false);
            m_MJPGList.SetUnitIsDownStatus(unitID, false);      

            unitID += 10;
        }
    }

    //所有项全部全选
    for (size_t i = 0 ; i < m_vClick.size(); i++) {
        if (m_bSelectAll) {
            m_vClick[i] = 1;
        } else {
            m_vClick[i] = 0;
        }
    }

    m_MJPGList.Invalidate();
}

void C3GSMSListDlg::DeleteItems() {
    int npos = 0 ;
    //删除打勾的行，目前只删除当前的行
    for (size_t i = 0 ; i < m_vClick.size() ; i++) {
        if (1 == m_vClick[(m_iCurrentPage - 1) * m_nPageSize + i]) {
            if (SMS_TYPE == m_nSMSType) {
                Data::Message::Remove("id = " + Util::StringOp::FromInt(m_vID[i]));
            }

            if (MMS_TYPE == m_nSMSType) {
                std::string filter ;
                filter = "id = ";
                filter += Util::StringOp::FromInt(m_vID[i]); 
                std::vector<boost::shared_ptr<Data::MMSData> >result ;
                result = Data::MMSData::GetFromDatabase(filter);
                if (!result.empty()) {
                    CString s = result[0]->SavePath.c_str();                
                    CString filename = L"";
                    CString path = L"";

                    extern int FindFileEx(CString s, CString & sPath, CString & sFilename);
                    FindFileEx(s, path, filename);
                    extern void DeleteDirectory(CString SrcDir, BOOL isShow = TRUE);
                    DeleteDirectory(path);

                    Data::MMSData::Remove("id = " + Util::StringOp::FromInt(m_vID[i]));
                }
            }

            npos++;
            ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetProcessPos(npos);
        }
    }

    //删除容器里的数据
    std::vector<int>::iterator it;
    it = m_vClick.begin();  
    std::vector<int>::iterator itID;
    itID = m_vID.begin();
    for (it ; it < m_vClick.end() ; it) {
        if (1 == (*it)) {
            m_vClick.erase(it);
            m_vID.erase(itID);
        } else {
            itID++ ;
            it++;
        }
    }   

    //容器的末尾值也去掉，保留为页的整数
    if (m_vClick.size() % m_nPageSize != 0) {
        it = m_vClick.end() - m_vClick.size() % m_nPageSize ;
        itID = m_vID.end() - m_vID.size() % m_nPageSize ;
        for (it ; it < m_vClick.end() ; it) {
            m_vClick.erase(it);
            m_vID.erase(itID);
        }
    }

    //重新获得当前的总页数
    if (SMS_TYPE == m_nSMSType) {
        m_iTotalPages = Data::Message::GetDataCount(m_sListFilter) / m_nPageSize;
        if (Data::Message::GetDataCount(m_sListFilter) % m_nPageSize != 0) {
            m_iTotalPages += 1;
        }
    } else {
          m_iTotalPages = Data::MMSData::GetDataCount(m_sListFilter) / m_nPageSize;
        if (Data::MMSData::GetDataCount(m_sListFilter) % m_nPageSize != 0) {
            m_iTotalPages += 1;
        }
    }


    if (m_iCurrentPage > m_iTotalPages) {
        m_iCurrentPage = m_iTotalPages ;
    }

    if (0 == m_iTotalPages) {
        m_iCurrentPage = 1;
    }

    ClearCurrentPage();
    FromDataBase();
    ShowArrayInList();
} 

void C3GSMSListDlg::PageSwitch(Action action) {
    if (down_page == action) {
        m_iCurrentPage++;
        if (m_iCurrentPage > m_iTotalPages) {
            m_iCurrentPage-- ;
            return ;
        }
    }

    if (up_page == action) {
        m_iCurrentPage--;
        if (m_iCurrentPage < 1) {
            m_iCurrentPage++;
            return ;
        }
    }

    ClearCurrentPage();
    FromDataBase();
    ShowArrayInList();
}

int C3GSMSListDlg::GetCurrentItems() {
    int icurrent ;//当前页的行数

    if (SMS_TYPE == m_nSMSType) {
        icurrent = m_vMessageCurrentResult.size() ;
    }

    if (MMS_TYPE == m_nSMSType) {
        icurrent = m_vMMSDataCurrentResult.size();
    }

    return icurrent ;
}

void C3GSMSListDlg::Clear() {
    if (m_vMessageCurrentResult.size() > 0) {
        m_vMessageCurrentResult.clear();
    }

    if (m_vMMSDataCurrentResult.size() > 0) {
        m_vMMSDataCurrentResult.clear();
    }

    if (m_vClick.size()) {
        m_vClick.clear();
    }

    if (m_vID.size()) {
        m_vID.clear();
    }

    m_bSelectAll = false ;
}

void C3GSMSListDlg::ClickedOneItem(int unitID, int item) {
    if (m_vClick[(m_iCurrentPage - 1) * m_nPageSize + item] == 0)//被点击了
    {
        m_MJPGList.SetUnitIsDownStatus(unitID, true);       
        m_vClick[(m_iCurrentPage - 1) * m_nPageSize + item] = 1;
    } else {
        m_MJPGList.SetUnitIsDownStatus(unitID, false);
        m_vClick[(m_iCurrentPage - 1) * m_nPageSize + item] = 0;
    }

    m_MJPGList.SetUnitIsShow(unitID, true, true);
}

void C3GSMSListDlg::ReadOneItem(int item) {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
    if (SMS_TYPE == m_nSMSType) {
        main->m_pSMSListDlg->m_pSmsReadDlg->SetSMSInfo(m_vMessageCurrentResult[item]->id(), m_nBoxType);
        main->m_pSMSListDlg->m_pSmsReadDlg->ShowWindow(SW_SHOW);
    } else {
          main->m_pSMSListDlg->m_pMmsReadDlg->SetMMSInfo(m_vMMSDataCurrentResult[item]->id(), m_nBoxType);
          main->m_pSMSListDlg->m_pMmsReadDlg->ShowWindow(SW_SHOW);
    }

    if (RECV_TYPE == m_nBoxType) {
        CString readType = L".\\adv\\mjpg\\k5\\common\\短信\\短信已读.png";//已经阅读图标
        m_MJPGList.SetUnitBitmap((item + 3) * 10 + 1, readType, L"", true);
    }
}

void C3GSMSListDlg::ShowSMS(SMSMMS_TYPE smsType) {
    if (smsType != m_nSMSType) {
        m_nSMSType = smsType ;
        m_iCurrentPage = 1 ;
        Clear();
        ClearCurrentPage();
        FromDataBase();
        ShowArrayInList();
    }
}
void C3GSMSListDlg::SetUnitStatus() {
    int ibegin = 30 ;
    int items = 0 ;
    for (int i = 0 ; i < GetCurrentItems(); i++) {
        for (int j = 0 ; j < 5 ; j++) {
            m_MJPGList.SetUnitIsShow(ibegin + items + j, true, false);
        }

        if (1 == m_vClick[(m_iCurrentPage - 1) * m_nPageSize + i]) {
            m_MJPGList.SetUnitIsDownStatus(ibegin + items, true);
        }

        items = items + 10;
    }

    if (GetCurrentItems() > 0) {
        m_MJPGList.SetUnitIsShow(140, true, false);
    }

    m_MJPGList.Invalidate();
}

int C3GSMSListDlg::GetVectorPages() {
    int pages ;
    pages = m_vClick.size() / m_nPageSize ;
    if (m_vClick.size() % m_nPageSize != 0) {
        pages += 1;
    }
    return pages ;
}
void C3GSMSListDlg::SetPagefont() {
    int begin = 30 ;
    for (int i = 0 ; i < m_nPageSize ; i++) {
        for (int j = 2; j < 5; j++) {
            m_MJPGList.SetUnitColor(begin + i * 10 + j, font_white, true);
        }
    }
}

void C3GSMSListDlg::ShowDeleteDlg() {
    bool enble = false ;
    int count = 0 ;

    for (size_t i = 0 ; i < m_vClick.size() ; i++) {
        if (1 == m_vClick[i]) {
            enble = true ;
            count++;
        }
    }

    if (enble) {
        CString title = "确认删除已选的内容吗?";
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(title, 0);
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetProcessMax(count);
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
    }
}

void C3GSMSListDlg::ShowWindow_(SMSBOX_TYPE type, SMSMMS_TYPE smsType) {
    CString sender = L".\\adv\\mjpg\\k5\\common\\短信\\发件人.bmp";//发件人图标
    CString sendTime = L".\\adv\\mjpg\\k5\\common\\短信\\发件时间.bmp";//发送时间
    CString receive = L".\\adv\\mjpg\\k5\\common\\短信\\收件人.bmp";//收件人图标
    CString receiveTime = L".\\adv\\mjpg\\k5\\common\\短信\\接收时间.bmp";//接受时间

    m_nBoxType = type ;
    m_nSMSType = smsType;
    m_iCurrentPage = 1 ;

    Clear();
    ClearCurrentPage();

    if (SEND_TYPE == type) {
        m_MJPGList.SetUnitBitmap(130, receive, L"", false);
        m_MJPGList.SetUnitBitmap(131, sendTime, L"", true);
        SetUpBtn(3);
    }

    if (RECV_TYPE == type) {
        m_MJPGList.SetUnitBitmap(130, sender, L"", false);
        m_MJPGList.SetUnitBitmap(131, receiveTime, L"", false);
        SetUpBtn(2);
    }

    FromDataBase();
    ShowArrayInList();
    ShowWindow(SW_SHOW);
}

void C3GSMSListDlg::Replay()//回复
{
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    //只能选择一个
    CString tip = L"只能选择一项去回复";
    if (Warning(m_vClick, tip)) {
        return ;
    }

    int index ;
    if (GetFirstClickID(index) && RECV_TYPE == m_nBoxType)//只有收件的时候才能回复
    {
        std::vector<CString> vtel;
        std::vector<CString> append;
        std::string sender ;
        std::string filter ;
        filter = "id =";
        filter += Util::StringOp::FromInt(m_vID[index]);
        if (SMS_TYPE == m_nSMSType) {
            std::vector<boost::shared_ptr<Data::Message> > result ;
            result = Data::Message::GetFromDatabase(filter);
            if (!result.empty()) {
                sender = result[0]->remote.address; 
                append.push_back(Util::StringOp::ToCString(sender));
            }

            std::string name = GetContactName(sender);//判断息名片是否有该人
            if (!name.empty()) {
                sender = name ;
            }

            vtel.push_back(Util::StringOp::ToCString(sender));

            main->m_pMainDlg->m_p3GSMSDlg->SetSender(vtel);
            main->m_pMainDlg->m_p3GSMSDlg->SetAppend(append);
            main->m_pMainDlg->m_p3GSMSDlg->ShowWindow(true);
        } else {
            std::vector<boost::shared_ptr<Data::MMSData> > result ;
            result = Data::MMSData::GetFromDatabase(filter);
            if (!result.empty()) {
                sender = result[0]->SenderAddress;  
                append.push_back(Util::StringOp::ToCString(sender));
            }

            std::string name = GetContactName(sender);//判断息名片是否有该人
            if (!name.empty()) {
                sender = name ;
            }

            vtel.push_back(Util::StringOp::ToCString(sender));
            main->m_pMMSDlg->SetSender(vtel);
            main->m_pMMSDlg->SetAppend(append);
            main->m_pMMSDlg->ShowWindow(true);
        }
    }
}

void C3GSMSListDlg::Transit() {
    //只能选择一个
    CString tip = L"只能选择一个去转发";
    if (Warning(m_vClick, tip)) {
        return ;
    }

    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;   
    int index ;
    if (GetFirstClickID(index))//只有收件的时候才能回复
    {
        boost::shared_ptr<Data::MMSData> pMmsdata ;
        std::string data ;
        if (SMS_TYPE == m_nSMSType) {
            boost::shared_ptr<Data::Message> pmessage ;
            pmessage = Data::Message::GetDataById(m_vID[index]);
            if (pmessage) {
                data = pmessage->unicodeData;
            }
        } else {
            pMmsdata = Data::MMSData::GetDataById(m_vID[index]);
        }

        if (SMS_TYPE == m_nSMSType)//短信
        {
            main->m_pMainDlg->m_p3GSMSDlg->SetSmsContent(data); 
            main->m_pMainDlg->m_p3GSMSDlg->ShowWindow(SW_SHOW);
            main->AddIcon(Allicon[1]);
        }

        if (MMS_TYPE == m_nSMSType)//彩信
        {
            main->m_pMMSDlg->SetMmsContent(pMmsdata);
            main->m_pMMSDlg->ShowWindow(SW_SHOW);
            main->AddIcon(Allicon[1]);
        }
    }
}

void C3GSMSListDlg::Details() {
    CString tip = L"只能选择一项查看详情";
    if (Warning(m_vClick, tip)) {
        return ;
    }

    int index ;
    if (GetFirstClickID(index, true)) {
        if (MMS_TYPE == m_nSMSType) {
            m_pMmsReadDlg->SetMMSInfo(m_vID[index], m_nBoxType);
            m_pMmsReadDlg->ShowWindow(SW_SHOW);
        } else {
            m_pSmsReadDlg->SetSMSInfo(m_vID[index], m_nBoxType);    
            m_pSmsReadDlg->ShowWindow(SW_SHOW);
        }

        if (RECV_TYPE == m_nBoxType) {
            CString readType = L".\\adv\\mjpg\\k5\\common\\短信\\短信已读.png";//已经阅读图标
            m_MJPGList.SetUnitBitmap((index % m_nPageSize) * 10 + 30 + 1, readType, L"", true);
        }
    }
}

void C3GSMSListDlg::NumberExtract() {
    std::vector<CString> vNumber ;
    CString contact ;//收件人或者发件人
    CString content ;
    int index ;

    CString tip = L"只能选择一项去提取号码!";
    if (Warning(m_vClick, tip)) {
        return ;
    }

    if (GetFirstClickID(index)) {
        if (SMS_TYPE == m_nSMSType) {
            boost::shared_ptr<Data::Message> pmessge;
            pmessge = Data::Message::GetDataById(m_vID[index]);
            contact = Util::StringOp::ToCString(pmessge->remote.address);
            content = Util::StringOp::ToCString(pmessge->unicodeData);
        } else {
            boost::shared_ptr<Data::MMSData> pMmsData;
              pMmsData = Data::MMSData::GetDataById(m_vID[index]);
            if (RECV_TYPE == m_nBoxType) {
                contact = Util::StringOp::ToCString(pMmsData->SenderAddress);
            } else {
                  contact = Util::StringOp::ToCString(pMmsData->RecipientAddress);
            }
              content = Util::StringOp::ToCString(pMmsData->ContentType);
        }

        //号码有可能是中文，现在按数字串计算
        vNumber.push_back(contact);
        void ExtractNumber(CString content, std::vector<CString> & vnum);
        ExtractNumber(content, vNumber);

        m_pNumberExtractDlg->SetNumber(vNumber);
        m_pNumberExtractDlg->ShowNumber();
        m_pNumberExtractDlg->ShowWindow_(SW_SHOW);
    }

    //号码提取界面
}

bool C3GSMSListDlg::GetFirstClickID(int& index, bool bcurrent) {
    bool enble = false ;
    if (!bcurrent)//从头开始
    {
        for (size_t i = 0 ; i < m_vClick.size() ; i++) {
            if (1 == m_vClick[i]) {
                enble = true ;
                index = i;
            }
        }
    } else//从当前页计算
    {
        int icurrent = GetCurrentItems();
        for (int i = 0 ; i < icurrent ; i++) {
            if (1 == m_vClick[(m_iCurrentPage - 1) * m_nPageSize + i]) {
                enble = true ;
                index = i ;
                break ;
            }
        }
    }
    return enble ;
}
