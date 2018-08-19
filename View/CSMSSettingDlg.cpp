// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "CSMSSettingDlg.h"
#include "sip.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg dialog


#define  ALL_SENDER  20


CSMSSettingDlg::CSMSSettingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSMSSettingDlg::IDD, pParent) {
}


void CSMSSettingDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(C3GSMSDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CSMSSettingDlg, CDialog)
    //{{AFX_MSG_MAP(C3GSMSDlg)
    ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
    //  ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg message handlers

BOOL CSMSSettingDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    m_MJPGList.Create(L"", WS_VISIBLE | WS_CHILD, CRect(0, 0, 800, 423), this);
    m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\短消息设置.xml");
    m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));

    m_MJPGList.SetUnitIsDownStatus(5, true);
    m_MJPGList.SetUnitIsDownStatus(70, true);

    MoveWindow(0, 57, 800, 423);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CSMSSettingDlg::PreTranslateMessage(MSG* pMsg) {
    return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CSMSSettingDlg::OnClickMJPG(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    CMultimediaPhoneDlg* main = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
    CString icon ;
    switch (w) {
    case 0:
        //写短信
        main->m_pMainDlg->m_p3GSMSDlg->ShowWindow(SW_SHOW);
        ShowWindow(SW_HIDE);
        break;      

    case 1:
        //写彩信
        main->m_pMMSDlg->ShowWindow(SW_SHOW);
        ShowWindow(SW_HIDE);
        break;

    case 2:
        //收件箱
        main->m_pSMSListDlg->ShowWindow_(RECV_TYPE, SMS_TYPE);
        ShowWindow(SW_HIDE);
        break ;

    case 3:
        //发件箱
        main->m_pSMSListDlg->ShowWindow_(SEND_TYPE, SMS_TYPE);
        ShowWindow(SW_HIDE);
        break ;

    case 4:
        //草稿箱
        main->m_pDraftDlg->ShowWindow_(DRAFT_TYPE, SMS_TYPE);
        ShowWindow(SW_HIDE);
        break ;

    case 5:
        //设置
        break;

    case 20:
        //短消息设置
    case 21:
    case 22:
    case 23:
        break;

    case 30:
        //短信回执
    case 31:
        break;

    case 40:
        //已发送消息保存
    case 41:
        break;

    case 50:
        //发件箱保存
    case 51:
        break;

    case 60:
        //是否将短消息存储到草稿箱
    case 61:
        break;

    case 70:
        //短消息设置
        break;

    case 71:
        //彩消息设置
        main->m_pMmsSettingDlg->ShowWindow(SW_SHOW);
        ShowWindow(SW_HIDE);
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

void CSMSSettingDlg::SetUpBtn(int ID) {
    m_MJPGList.SetUnitIsDownStatus(ID, TRUE);
    for (int i = 0 ; i < 6; i++) {
        if (i != ID) {
            m_MJPGList.SetUnitIsDownStatus(i, FALSE);
        }
    }
    m_MJPGList.Invalidate();
}








