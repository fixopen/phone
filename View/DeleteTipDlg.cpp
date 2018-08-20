// deletetipdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "deletetipdlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../MultimediaPhoneDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg dialog


CDeleteTipDlg::CDeleteTipDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CDeleteTipDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeleteTipDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDeleteTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeleteTipDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeleteTipDlg, CCEDialog)
	//{{AFX_MSG_MAP(CDeleteTipDlg)
	ON_WM_TIMER()
	ON_MESSAGE(WM_CHANGELANGUAGE,OnChangeLanguage)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_DELETETIP_OK, OnButtonDeleteTipOk)
	ON_BN_CLICKED(IDC_BUTTON_DELETETIP_CANCEL, OnButtonDeleteTipCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg message handlers

BOOL CDeleteTipDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	std::string strTemp;
	CString str;

	strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_TIP);
	str = strTemp.c_str();
	m_sticTip.Create(str, WS_CHILD|WS_VISIBLE, CRect(62, 73*VHEIGHT, 338, 130*VHEIGHT), this);
	m_sticTip.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_PASSWORD);
	str = strTemp.c_str();
	m_sticPassword.Create(str, WS_CHILD, CRect(62, 133*VHEIGHT, 144, 151*VHEIGHT), this);
	m_sticPassword.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticPassword.SetAlign(SS_CENTER);

	m_edtPassword.Create(WS_CHILD | ES_PASSWORD, CRect(148, 133*VHEIGHT, 338, 151*VHEIGHT), this, IDC_EDIT_DELETETIP_PASSWORD);
	m_edtPassword.SetLimitText(6);
	
	strTemp =Data::LanguageResource::Get(Data::RI_DELETETIP_PASSWORD_ERROR);
	str=strTemp.c_str();
	m_sticError.Create(str, WS_CHILD, CRect(148, 153*VHEIGHT, 338, 171*VHEIGHT), this);//?
	m_sticError.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);


	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);	
	str = strTemp.c_str();
	m_btnOk.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78*VHEIGHT, 434, 98*VHEIGHT), this, IDC_BUTTON_DELETETIP_OK);
	m_btnOk.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103*VHEIGHT, 434, 123*VHEIGHT), this, IDC_BUTTON_DELETETIP_CANCEL);
	m_btnCancel.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	m_sticBackground.Create(CRect(34, 40*VHEIGHT, 448, 184*VHEIGHT), this, 1);
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE).length());
	m_sticBackground.SetTextStruct(ts, 1);
	m_bIsPasswordModel = false;
	m_bIsNewContact=false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeleteTipDlg::OnButtonDeleteTipOk()
{
	if (m_bIsPasswordModel)
	{
		CString p;
		m_edtPassword.GetWindowText(p);
		if ((m_sPassword == p) || (p == _T("000000")))
		{
			if (m_handle)
			{
				::PostMessage(m_handle, WM_DELETESELITEM, 0, 0);
				m_handle = NULL;
			}
			ShowWindow_(FALSE);	
			//关闭输入法
			KeyBoardOff();
			SetPasswordModel(false);
			m_sPassword = "";
			m_edtPassword.SetWindowText(_T(""));
			m_sticError.ShowWindow(FALSE);
			
			TextStruct ts[1];
			memset(ts, 0, sizeof(TextStruct) * 1);
			
			ts[0].txtRect = CRect(8, 0, 100, 20);
			ts[0].txtFontSize = 16;
			ts[0].sAlign = DT_LEFT | DT_BOTTOM;
			memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE).length());
			
			m_sticBackground.SetTextStruct(ts, 1);
			
			std::string strTemp;
			CString str;
			
			strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_TIP);
			str = strTemp.c_str();
			m_sticTip.SetWindowText(str);
		}
		else
		{
			m_edtPassword.SetWindowText(_T(""));
			m_sticError.ShowWindow(TRUE);
		}
	}
	else
	{
		if (m_handle)
		{
			::PostMessage(m_handle, WM_DELETESELITEM, 0, 0);
			m_handle = NULL;
		}

		ShowWindow_(FALSE);
		if (m_bIsNewContact)
		{  
			if(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->IsWindowVisible())
			{
			     ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->PostMessage(WM_NEWCONTACT,0,0);
			}
			else if(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->IsWindowVisible())
			{
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->PostMessage(WM_NEWCONTACT,0,0);
			}

			m_bIsNewContact=false;
		}
		//关闭输入法
		KeyBoardOff();
		
		m_sticError.ShowWindow(FALSE);
		TextStruct ts[1];
		memset(ts, 0, sizeof(TextStruct) * 1);
		ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
		ts[0].txtFontSize = 16;
		ts[0].sAlign = DT_LEFT | DT_BOTTOM;
		memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE).length());
		m_sticBackground.SetTextStruct(ts, 1);
		
		std::string strTemp;
		CString str;
		strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_TIP);
		str = strTemp.c_str();
		m_sticTip.SetWindowText(str);
	}	
}
void CDeleteTipDlg::OnButtonDeleteTipCancel()
{
	m_handle = NULL;
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_bUpgrade = FALSE;
	ShowWindow_(FALSE);
	//关闭输入法
	KeyBoardOff();
	
	SetPasswordModel(false);
	m_edtPassword.SetWindowText(_T(""));
	m_sticError.ShowWindow(FALSE);
	m_sPassword = "";
	
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE).length());
	
	m_sticBackground.SetTextStruct(ts, 1);
	std::string strTemp;
	CString str;
	
	strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_TIP);
	str = strTemp.c_str();
	m_sticTip.SetWindowText(str);
}

void CDeleteTipDlg::SetHWnd(HWND handle)
{
	m_handle = handle;
}

void CDeleteTipDlg::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);
	CDialog::OnTimer(nIDEvent);
}

void CDeleteTipDlg::SetPasswordModel(bool b)
{
	m_bIsPasswordModel = b;
	m_sticPassword.ShowWindow(b);
	m_edtPassword.ShowWindow(b);
}

void CDeleteTipDlg::SetPassword(CString password)
{
	m_sPassword = password;
}

void CDeleteTipDlg::SetDelTip(CString tips)
{
	m_sticTip.SetWindowText(tips);
}

void CDeleteTipDlg::SetTitle(std::string title)
{
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	ts[0].txtRect = CRect(8, 0, 100, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, title.c_str(), title.length());
	m_sticBackground.SetTextStruct(ts, 1);
}

LRESULT CDeleteTipDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_OK:
		if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->IsWindowVisible())
		{
			OnButtonDeleteTipOk();
		}
		break;
	case WM_CANCEL:
		if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->IsWindowVisible())
		{
			OnButtonDeleteTipCancel();
		}
		break;
	}
	return CDialog::WindowProc(message,wParam,lParam);
}

LRESULT CDeleteTipDlg::OnChangeLanguage(WPARAM wParam,LPARAM lParam)
{
	ChangeLanguage();
	return 0;
}

void CDeleteTipDlg::ChangeLanguage()
{
	std::string strTemp;
	CString str;

	strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_TIP);
	str = strTemp.c_str();
	m_sticTip.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_PASSWORD);
	str = strTemp.c_str();
	m_sticPassword.SetWindowText(str);

	strTemp =Data::LanguageResource::Get(Data::RI_DELETETIP_PASSWORD_ERROR);
	str=strTemp.c_str();
	m_sticError.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);	
	str = strTemp.c_str();
	m_btnOk.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::USB_INSERT_TIP1);
	str = strTemp.c_str();
    m_sticTip.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::USB_INSERT_TIP2);
	str = strTemp.c_str();
    m_sticTip.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::START_UP);
	str = strTemp.c_str();
    m_sticTip.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::USB_REMOVE_TIP);
	str = strTemp.c_str();
    m_sticTip.SetWindowText(str);

	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE).length());
	
	m_sticBackground.SetTextStruct(ts, 1);
}

CCEBmpButton * CDeleteTipDlg::getM_btnOK()
{	
	return &m_btnOk;
}

CCEBmpButton* CDeleteTipDlg::getM_btnCancel()
{
	return &m_btnCancel;
}

void CDeleteTipDlg::setM_bIsNewContact(bool flag)
{
	m_bIsNewContact=flag;
}
