// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "3GTelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3GTelDlg dialog


C3GTelDlg::C3GTelDlg(CWnd* pParent /*=NULL*/)
: CDialog(C3GTelDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(C3GTelDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

}


void C3GTelDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(C3GTelDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(C3GTelDlg, CDialog)
    //{{AFX_MSG_MAP(C3GTelDlg)
    ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    //  ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GTelDlg message handlers
/*
void C3GTelDlg::OnTimer(UINT nIDEvent)
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

BOOL C3GTelDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    m_MJPGList.Create(L"", WS_VISIBLE | WS_CHILD, CRect(0, 0, 600, 420), this);
    m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\3g_视频电话.xml");
    m_MJPGList.SetMJPGRect(CRect(0, 0, 600, 420));
    //  SetTimer(1, 1000, NULL);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT C3GTelDlg::OnClickMJPG(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    CMultimediaPhoneDlg* pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
    switch (w) {
    case 1:
        if (pMainDlg->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOff) {
            CString sTel = ".\\adv\\mjpg\\k1\\common\\电话1.bmp";
            pMainDlg->m_MJPGList.SetUnitBitmap(1, sTel, "", TRUE);
            pMainDlg->m_pTelephoneDlg->ShowWindow_(TRUE);
        } else {
            pMainDlg->SendMessage(WM_TEL_HUNGON);
        }
        break;
    case 2:
        pMainDlg->m_pContactDlg->ShowWindow(SW_SHOW);
        break;
    case 3:
        pMainDlg->m_pContactInfoDlg->ShowWindow(SW_SHOW);
        //  pMainDlg->m_pSoundDlg->ShowWindow(SW_SHOW);
        break;
    case 4:
        pMainDlg->m_pMainDlg->m_firewalDlg_->SetParameters(((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall(), ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType(), (int)((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->blockAllTimeDuration().GetTotalSeconds());
        pMainDlg->m_pMainDlg->m_firewalDlg_->ShowWindow(SW_SHOW);
        //  pMainDlg->m_pSettingDlg->ShowWindow(SW_SHOW);
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        break;
    case 100:
        ShowWindow(SW_HIDE);
    default:
        break;
    }
    return result;
}
