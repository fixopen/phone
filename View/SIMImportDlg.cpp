// SIMImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "SIMImportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSIMImportDlg dialog

#define IDC_STATIC_STATUS   1120

CSIMImportDlg::CSIMImportDlg(CWnd* pParent /*=NULL*/)
: CCEDialog(CSIMImportDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CSIMImportDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

}

void CSIMImportDlg::DoDataExchange(CDataExchange* pDX) {
    CCEDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSIMImportDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(CSIMImportDlg, CCEDialog)
    //{{AFX_MSG_MAP(CSIMImportDlg)
    ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

LRESULT CSIMImportDlg::OnClickMJPG(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    WPARAM p = 0 ; 
    switch (w) {
    case 3:
        //本地->SIM
        break;
    case 4:
        //SIM->本地
        break;
    case 1000:
        //返回
        ShowWindow_(SW_HIDE);
        break;

    case 1001:
        //确定
        OnBtnOK();
        break;

    case 1002:
        //删除
        break;
    }
    return result;
}
/////////////////////////////////////////////////////////////////////////////
// CSIMImportDlg message handlers

BOOL CSIMImportDlg::OnInitDialog() {
    CCEDialog::OnInitDialog();

    // TODO: Add extra initialization here

    m_procbar.Create(WS_CHILD, CRect(180 + 35, 105 + 216, 180 + 35 + 185, 105 + 216 + 27), this, IDC_PBAR_SOUNDDLG_PROCESS);

    m_MJPGList.Create(L"", WS_VISIBLE | WS_CHILD, CRect(180, 105, 180 + 440, 105 + 270), this);
    m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\SIM导入.xml");
    m_MJPGList.SetMJPGRect(CRect(180, 105, 180 + 440, 105 + 270));

    m_MJPGList.SetUnitText(2, L"", FALSE);
    m_MJPGList.SetUnitIsShow(3, FALSE);

    m_pContact = boost::shared_ptr<Data::Contact>(new Data::Contact);   

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CSIMImportDlg::SetUnit(Destination des) {
    if (SIMtoLocal == des) {
        m_MJPGList.SetUnitIsDownStatus(4, TRUE);    
        m_MJPGList.SetUnitIsDownStatus(3, FALSE);
    }

    if (LocaltoSIM == des) {
        m_MJPGList.SetUnitIsDownStatus(4, FALSE);   
        m_MJPGList.SetUnitIsDownStatus(3, TRUE);
    }
    m_enumDes = des ;
    m_MJPGList.SetUnitIsDisable(4, true);
    m_MJPGList.SetUnitIsDisable(3, true);
    m_MJPGList.SetUnitIsShow(4, true, true);
    m_MJPGList.SetUnitIsShow(3, true, true);
}

void CSIMImportDlg::SetHWnd(HWND handle) {
    m_handle = handle ;
}

LRESULT CSIMImportDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
    return CCEDialog::WindowProc(message, wParam, lParam);
}

void CSIMImportDlg::InitSIM() {
}

void CSIMImportDlg::OnTimer(UINT nIDEvent) {
    if (1 == nIDEvent) {
        KillTimer(1);
        m_procbar.ShowWindow(SW_HIDE);
        ShowWindow_(SW_HIDE);
    }
    CCEDialog::OnTimer(nIDEvent);
}

std::string GetAlias(std::string name) {
    if (((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pContactNewDlg->m_pPinYinIndex == NULL) {
        return "";
    }

    std::string alias;
    for (size_t i = 0; i < name.size(); ++i) {
        if ((unsigned char)name[i] > (unsigned char)127) {
            unsigned int index = ((unsigned char)name[i] - 0xb0) * 94 + ((unsigned char)name[i + 1] - 0xa1);

            if (index >= 0 && index < 6768) {
                alias += ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pContactNewDlg->m_pPinYinIndex[index];
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

void CSIMImportDlg::OnBtnOK() {
    m_procbar.ShowWindow(SW_SHOW);
    ::PostMessage(m_handle, WM_SIM_IMPORT, m_enumDes, 0);
}

void CSIMImportDlg::SetProcessMax(int max) {
    m_procbar.ShowWindow(SW_HIDE);
    m_procbar.SetParam(0, 0, max, 1);
    m_procbar.SetPos(0);
    m_iMaxPos = max;
}

void CSIMImportDlg::SetProcessPos(int npos) {
    m_procbar.SetPos(npos);
    if (npos == m_iMaxPos) {
        SetTimer(1, 100, NULL);
    }
}
