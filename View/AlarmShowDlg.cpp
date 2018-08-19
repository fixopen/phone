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
	CCEDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54, 62, 746, 358), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\提醒.xml");
	m_MJPGList.SetMJPGRect(CRect(54, 62, 746, 358));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAlarmShowDlg::SetTxt(int nAlarmID, CString sTime, CString sTitle, CString sContent, std::string sRing, BOOL isADDList)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pRecordSoundDlg->CloseSound();
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pPlaySoundDlg->CloseSound();

	//退出屏保
	if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainScreenSaveDlg_->SendMessage(WM_OUTEVENT, 0, 0);
		
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainScreenSaveDlg_->ShowWindow(SW_HIDE);
	}

	if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainPhotoDlg_->IsWindowVisible())
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainPhotoDlg_->SendMessage(WM_OUTEVENT, 0, 0);
	
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	m_nAlarmID = nAlarmID;

	if(isADDList)
	{
		main->m_pMainDlg->m_mainMp3Dlg_->OnTimer(1002); //SendMessage(WM_OUTEVENT, 0, 0);
		//停止试听音乐
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->StopTryRing();
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactGroupDlg->StopTryRing();
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainLunarderDlg_->StopTryRing();
	}

	main->m_pMainDlg->m_mainVideoDlg_->OnTimer(1002); //SendMessage(WM_OUTEVENT, 0, 0);

	m_MJPGList.SetUnitText(1, sTime, FALSE);
//	m_MJPGList.SetUnitText(2, sTitle, FALSE);
	m_MJPGList.SetUnitText(3, sContent, FALSE);
	main->phone_->StartRing((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(sRing), 0xFF);
	m_MJPGList.Invalidate();
	KillTimer(1);
	SetTimer(1, 2*60*1000, NULL);
}

void CAlarmShowDlg::ShowWindow_(int cmdshow)
{
	if(cmdshow > 0)
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_pWebDialog->SendMessage(WM_KILLWEBSHOW, 1, 0);
		::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 800, 480, 0);
		CCEDialog::ShowWindow_(cmdshow);

		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_firewalDlg_->m_cmbTime.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);  //如果commbox打开，让commobox隐藏
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactGroupDlg->m_cmbSoundTip.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactNewDlg->m_cmbType.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactNewDlg->m_cmbGroup.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainLunarderDlg_->m_cmbRing.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
//		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactDlg->m_pSimImportDlg->m_cmbGroup.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_pWebDialog->m_cmbURL.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		
//		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbRing.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbRingTimes.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbAutoRecoedeTimes.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbWaitTime.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbSoundSavePath.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		
//		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbRingVolume.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbSystemVolume.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbContrlBlackLightWaitTime.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
	}
	else
	{
		CCEDialog::ShowWindow_(cmdshow);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_pWebDialog->SendMessage(WM_KILLWEBSHOW, 0, 0);
	}
	
}

LRESULT CAlarmShowDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
    LRESULT result = 0;
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	switch(w)
	{
	case 1:
		KillTimer(1);
		main->phone_->StartRing(L"");
		boost::shared_ptr<Data::Scheduler> m_result = Data::Scheduler::GetFromDatabaseById(m_nAlarmID);
		m_result->tipsType(Data::ttRead);
		m_result->Update();
		main->m_pMainDlg->FindTodayAlarm();
		ShowWindow_(SW_HIDE);
	//	theApp.PostThreadMessage(WM_LBUTTONDOWN, 0, 0);
		main->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
		main->m_pMainDlg->m_mainVideoDlg_->SendMessage(WM_OUTEVENT, 0, 1);
		main->m_pMainDlg->m_mainPhotoDlg_->SendMessage(WM_OUTEVENT, 0, 1);
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
		ShowWindow_(SW_HIDE);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetScreenSaveTimer();
		main->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
		main->m_pMainDlg->m_mainVideoDlg_->SendMessage(WM_OUTEVENT, 0, 1);
		main->m_pMainDlg->m_mainPhotoDlg_->SendMessage(WM_OUTEVENT, 0, 1);
	}
	CCEDialog::OnTimer(nIDEvent);
}
