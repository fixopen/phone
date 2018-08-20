// firewalldlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "firewalldlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../MultimediaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFireWallDlg dialog

CFireWallDlg::CFireWallDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CFireWallDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFireWallDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFireWallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFireWallDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFireWallDlg, CCEDialog)
	//{{AFX_MSG_MAP(CFireWallDlg)
	ON_WM_TIMER()
	ON_MESSAGE(WM_CHANGELANGUAGE,OnChangeLanguage)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_FIREWALL_OK, OnButtonFireWallOk)
	ON_BN_CLICKED(IDC_BUTTON_FIREWALL_CANCEL, OnButtonFireWallCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFireWallDlg message handlers

BOOL CFireWallDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	std::string strTemp;
	CString str;
	
	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_ENABLE);
	str = strTemp.c_str();

	m_chbEnableFireWall.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(62, 73*VHEIGHT, 160, 89*VHEIGHT), this, IDC_CHECKBOX_FIREWALL_ENABLE);
	m_chbEnableFireWall.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	m_chbEnableFireWall.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_chbEnableFireWall.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_chbEnableFireWall.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_BLACKLIST);
	str = strTemp.c_str();
	m_chbRefuseBlacklist.Create(str, WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON, CRect(76, 93*VHEIGHT, 340, 109*VHEIGHT), this, IDC_CHECKBOX_FIREWALL_BLACKLIST);
	m_chbRefuseBlacklist.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_CONTACT);
	str = strTemp.c_str();
	m_chbContactExceptBlacklist.Create(str, WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON, CRect(76, 113*VHEIGHT, 340, 129*VHEIGHT), this, IDC_CHECKBOX_FIREWALL_CONTACT);
	m_chbContactExceptBlacklist.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_VIP);
	str = strTemp.c_str();
	m_chbOnlyVIP.Create(str, WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON, CRect(76, 133*VHEIGHT, 340, 149*VHEIGHT), this, IDC_CHECKBOX_FIREWALL_VIP);
	m_chbOnlyVIP.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_ALL);
	str = strTemp.c_str();
	m_chbRefuseAllByTime.Create(str, WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON, CRect(76, 153*VHEIGHT, 230, 169*VHEIGHT), this, IDC_CHECKBOX_FIREWALL_ALL);
	m_chbRefuseAllByTime.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	CButton *pButton[4];
	pButton[0] = &m_chbRefuseBlacklist;
	pButton[1] = &m_chbContactExceptBlacklist;
	pButton[2] = &m_chbOnlyVIP;
	pButton[3] = &m_chbRefuseAllByTime;
	
	m_chbRefuseBlacklist.SetGroupButton(pButton, 4);
	m_chbContactExceptBlacklist.SetGroupButton(pButton, 4);
	m_chbOnlyVIP.SetGroupButton(pButton, 4);
	m_chbRefuseAllByTime.SetGroupButton(pButton, 4);

// 	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_PART1);
// 	str = strTemp.c_str();
// 	m_sticTimePart1.Create(str, WS_CHILD|WS_VISIBLE, CRect(164, 153, 260, 169), this);

	m_cmbTime.Create(WS_CHILD|WS_VISIBLE, CRect(234, 153, 270, 240), this, IDC_COMBO_FIREWALL_TIME);
	///////////zzw100824////////////////////////////////////////
	m_cmbTime.InsertString(-1,_T("1"));
	m_cmbTime.InsertString(-1,_T("2"));
	m_cmbTime.InsertString(-1,_T("3"));
	m_cmbTime.SetCurSel(0);

	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_PART2);
	str = strTemp.c_str();
	m_sticTimePart2.Create(str, WS_CHILD|WS_VISIBLE, CRect(274, 153*VHEIGHT, 300, 169*VHEIGHT), this);
	m_sticTimePart2.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78*VHEIGHT, 434, 98*VHEIGHT), this, IDC_BUTTON_FIREWALL_OK);
	m_btnOk.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103*VHEIGHT, 434, 123*VHEIGHT), this, IDC_BUTTON_FIREWALL_CANCEL);
	m_btnCancel.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	m_sticBackground.Create(CRect(34, 40*VHEIGHT, 448, 184*VHEIGHT), this, 1);
	
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_FIREWALL_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_FIREWALL_TITLE).length());
	
	m_sticBackground.SetTextStruct(ts, 1);

	m_ISByTimeFireWall=FALSE;

	SetParameters(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall(),
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType(),
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->blockAllTimeDuration().GetTotalSeconds());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
	
LRESULT CFireWallDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
      switch(message)
	  {
	  case WM_OK:
		  OnButtonFireWallOk();
		  break;
	  case WM_CANCEL:
		  OnButtonFireWallCancel();
		  break;
	  }
	 return CDialog::WindowProc( message,  wParam, lParam);
	}
	
void CFireWallDlg::OnButtonFireWallOk()
{
	KillTimer(1);

	if (m_chbEnableFireWall.GetCheck() == BST_CHECKED)
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall(true);
        /////////////////////////////zzw10826//////////////////////////////////////////
		if(!((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isUseSpecRing())
		{
			if (!theApp.m_bIsHungOnFireWall)
			{
			  ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(FALSE);
			}

		}	
       /////////////////////////////zzw10826////////////////////////////////////////////	 
	}else
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall(false);
        //////////////zzw100826//////////////////////////////////////////////////////////
		if(!((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isUseSpecRing())
		{ 
			if (!theApp.m_bIsHungOnFireWall)
			{
		    	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(TRUE);
			}
		}	
	}
    ////////////////////////zzw100826/////////////////////////////////////////
	CString d;
	m_cmbTime.GetLBText(m_cmbTime.GetCurSel(), d);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->blockAllTimeDuration(atoi(d) * 3600);

	int type = 0;
	if (m_chbRefuseBlacklist.GetCheck() == BST_CHECKED)
	{
		type = 0;
	}
	else if (m_chbContactExceptBlacklist.GetCheck() == BST_CHECKED)
	{
		type = 1;
	}
	else if (m_chbOnlyVIP.GetCheck() == BST_CHECKED)
	{
		type = 2;
	}
	else if (m_chbRefuseAllByTime.GetCheck() == BST_CHECKED)
	{
		type = 3;
		SetTimer(1, atoi(d) * 3600 * 1000, 0);
		m_ISByTimeFireWall=TRUE;
	}
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType((Data::FirewallType)type);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->Update();
	 ShowWindow_(FALSE);
   	 OnStart(FALSE);
	::PostMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_FIRENOTIFY, 0, 0);
}

void CFireWallDlg::OnButtonFireWallCancel()
{
	ShowWindow_(FALSE);
	OnStart(FALSE);
	SetParameters(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall(),
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType(),
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->blockAllTimeDuration().GetTotalSeconds());
}

//////////////////////////////////////////////////////////////////////////////////
void CFireWallDlg::SetParameters(bool enable, int type, int duration)
{
	m_chbEnableFireWall.SetCheck(enable);
	m_chbRefuseBlacklist.SetCheck(BST_UNCHECKED);
	m_chbContactExceptBlacklist.SetCheck(BST_UNCHECKED);
	m_chbOnlyVIP.SetCheck(BST_UNCHECKED);
	m_chbRefuseAllByTime.SetCheck(BST_UNCHECKED);
	switch (type)
	{
	case 0:
		m_chbRefuseBlacklist.SetCheck(BST_CHECKED);
		break;
	case 1:
		m_chbContactExceptBlacklist.SetCheck(BST_CHECKED);
		break;
	case 2:
		m_chbOnlyVIP.SetCheck(BST_CHECKED);
		break;
	case 3:
		m_chbRefuseAllByTime.SetCheck(BST_CHECKED);
		break;
	default:
		break;
	}
    int temp=duration;
	int index = duration / 3600;
	m_cmbTime.SetCurSel(index-1);
}

void CFireWallDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		KillTimer(nIDEvent);
		if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType() == 3)
		{
			m_chbEnableFireWall.SetCheck(BST_UNCHECKED);
			m_ISByTimeFireWall=FALSE;
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall(false);
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->Update();
			::PostMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_FIRENOTIFY, 0, 0);
		}
	}

	CCEDialog::OnTimer(nIDEvent);
}

int CFireWallDlg::GetFirewallType()
{
	if(!((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall())
		return -1;
	return ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType();
}

void CFireWallDlg::OnStart(BOOL isStart)
{  ////////////zzw100824//////////////////////////////
   ///((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SendMessage(WM_PLAYVIDEO, !isStart);
	SetParameters(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall(),
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType(),
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->blockAllTimeDuration().GetTotalSeconds());
}

LRESULT CFireWallDlg::OnChangeLanguage(WPARAM wParam,LPARAM lParam)
{
  ChangeLanguage();
  return 0;
}
void CFireWallDlg::ChangeLanguage()
{
	std::string strTemp;
 	CString str;
 	
 	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_ENABLE);
	str = strTemp.c_str();
	m_chbEnableFireWall.SetWindowText(str);
 
	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_BLACKLIST);
 	str = strTemp.c_str();
     m_chbRefuseBlacklist.SetWindowText(str);
 
 	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_CONTACT);
  	str = strTemp.c_str();
  	m_chbContactExceptBlacklist.SetWindowText(str);
 
 	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_VIP);
  	str = strTemp.c_str();
  	m_chbOnlyVIP.SetWindowText(str);
 
  	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_ALL);
  	str = strTemp.c_str();
  	m_chbRefuseAllByTime.SetWindowText(str);
 
  	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_PART2);
  	str = strTemp.c_str();
  	m_sticTimePart2.SetWindowText(str);
  
  	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
  	str = strTemp.c_str();
 	m_btnOk.SetWindowText(str);
  
 	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
  	str = strTemp.c_str();
 	m_btnCancel.SetWindowText(str);
// 	m_sticBackground.Create(CRect(34, 40*VHEIGHT, 448, 184*VHEIGHT), this, 1);
 
     TextStruct ts[1];
 	memset(ts, 0, sizeof(TextStruct) * 1);
	
 	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
 	ts[0].txtFontSize = 16;
 	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
 	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_FIREWALL_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_FIREWALL_TITLE).length());
 	
 	m_sticBackground.SetTextStruct(ts, 1);
}
