// AlarmShowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "AlarmShowDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace View {
    /////////////////////////////////////////////////////////////////////////////
    // CAlarmShowDlg dialog


    CAlarmShowDlg::CAlarmShowDlg(CWnd* pParent /*=NULL*/)
        : CCEDialog(CAlarmShowDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CAlarmShowDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
    }


    void CAlarmShowDlg::DoDataExchange(CDataExchange* pDX)
    {
        CCEDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CAlarmShowDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CAlarmShowDlg, CCEDialog)
        //{{AFX_MSG_MAP(CAlarmShowDlg)
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
        ON_WM_TIMER()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CAlarmShowDlg message handlers

    BOOL CAlarmShowDlg::OnInitDialog() 
    {
        CCEDialog::OnInitDialog();2

        // TODO: Add extra initialization here
        m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54*X_XISHU, 62*Y_XISHU, 746*X_XISHU, 358*Y_XISHU), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\ÖÐÎÄ\\ÌáÐÑ.xml"));
        m_MJPGList.SetMJPGRect(CRect(54*X_XISHU, 62*Y_XISHU, 746*X_XISHU, 358*Y_XISHU));

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    void CAlarmShowDlg::SetTxt(int nAlarmID, CString sTime, CString sTitle, CString sContent, std::string sRing, BOOL isADDList)
    {
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pRecordSoundDlg->CloseSound();
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pPlaySoundDlg->CloseSound();

        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        m_nAlarmID = nAlarmID;
        main->m_pMainDlg->m_mainMp3Dlg_->OnTimer(1002); //SendMessage(WM_OUTEVENT, 0, 0);
        main->m_pMainDlg->m_mainVideoDlg_->OnTimer(1002); //SendMessage(WM_OUTEVENT, 0, 0);

        //Í£Ö¹ÊÔÌýÒôÀÖ
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->OnStopTryRing(0, 0);
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactGroupDlg->OnStopTryRing(0, 0);
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainLunarderDlg_->OnStopTryRing(0, 0);

        m_MJPGList.SetUnitText(1, sTime, FALSE);
        //	m_MJPGList.SetUnitText(2, sTitle, FALSE);
        m_MJPGList.SetUnitText(3, sContent, FALSE);
        main->phone_->StartRing((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(sRing), 3);
        m_MJPGList.Invalidate();
        SetTimer(1, 3*60*1000, NULL);
    }

    void CAlarmShowDlg::ShowWindow_(int cmdshow)
    {
        if(cmdshow > 0)
        {
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_pWebDialog->SendMessage(WM_KILLWEBSHOW, 1, 0);
            ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 800, 480, 0);
        }
        else
        {
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_pWebDialog->SendMessage(WM_KILLWEBSHOW, 0, 0);
        }
        CCEDialog::ShowWindow_(cmdshow);
    }

    LRESULT CAlarmShowDlg::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        switch(w)
        {
        case 1:
            main->phone_->StartRing(L"");
            Util::shared_ptr<Data::Scheduler> m_result = Data::Scheduler::GetFromDatabaseById(m_nAlarmID);
            m_result->tipsType(Data::ttRead);
            m_result->Update();
            main->m_pMainDlg->FindTodayAlarm();
            ShowWindow(SW_HIDE);
            main->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
            main->m_pMainDlg->m_mainVideoDlg_->SendMessage(WM_OUTEVENT, 0, 1);
            break;
        }
        return result;
    }

    void CAlarmShowDlg::OnTimer(UINT nIDEvent) 
    {
        // TODO: Add your message handler code here and/or call default
        if(nIDEvent == 1)
        {
            KillTimer(1);
            CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
            main->phone_->StartRing(L"");
            main->m_pMainDlg->FindTodayAlarm();
            ShowWindow(SW_HIDE);
            main->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
            main->m_pMainDlg->m_mainVideoDlg_->SendMessage(WM_OUTEVENT, 0, 1);
        }
        CCEDialog::OnTimer(nIDEvent);
    }
}
