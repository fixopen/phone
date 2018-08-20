// CWarningDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "deletetipdlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../MultimediaPhoneDlg.h"
#include "CWarningDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCWarningDlg dialog


CCWarningDlg::CCWarningDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CCWarningDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCWarningDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCWarningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCWarningDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCWarningDlg, CCEDialog)
	//{{AFX_MSG_MAP(CCWarningDlg)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CHANGELANGUAGE,ChangeLanguage)
	ON_BN_CLICKED(IDC_BUTTON_DELETETIP_OK, OnButtonWarningOK)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCWarningDlg message handlers


void CCWarningDlg::SetTitle(std::string title)
{
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	ts[0].txtRect = CRect(8, 0, 100, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, title.c_str(), title.length());
	m_sticBackground.SetTextStruct(ts, 1);
}

BOOL CCWarningDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	std::string strTemp;
	CString str;

	strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_TIP);
	str = strTemp.c_str();
	m_sticTip.Create(str, WS_CHILD|WS_VISIBLE, CRect(62, 73*VHEIGHT, 338, 130*VHEIGHT), this);
	m_sticTip.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);	
	str = strTemp.c_str();
	m_btnOk.Create(str,\
		Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], \
		Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], \
		WS_CHILD|WS_VISIBLE, CRect(378, 78*VHEIGHT, 434, 98*VHEIGHT),\
		this, IDC_BUTTON_DELETETIP_OK);
	m_btnOk.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	m_sticBackground.Create(CRect(34, 40*VHEIGHT, 448, 184*VHEIGHT), this, 1);
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(\
		Data::RI_WARNING).c_str(),\
		Data::LanguageResource::Get(Data::RI_WARNING).length());
	m_sticBackground.SetTextStruct(ts, 1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCWarningDlg::OnButtonWarningOK()
{
	ShowWindow_(FALSE);
}

LRESULT CCWarningDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	switch(message)
	{
	case WM_OK:
		if (IsWindowVisible())
		{
			OnButtonWarningOK();
		}
	}
	return CCEDialog::WindowProc(message, wParam, lParam);
}

void CCWarningDlg::ChangeLanguage()
{
	std::string strTemp;
	CString str;
	
	
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);	
	str = strTemp.c_str();
	m_btnOk.SetWindowText(str);
	

	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt,\
		Data::LanguageResource::Get(Data::RI_WARNING).c_str(),\
		Data::LanguageResource::Get(Data::RI_WARNING).length());
	
	m_sticBackground.SetTextStruct(ts, 1);
}


void CCWarningDlg::SetDelTip(CString tips)
{
	m_sticTip.SetWindowText(tips);
}
