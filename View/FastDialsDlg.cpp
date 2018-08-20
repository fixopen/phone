// fastdialsdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "fastdialsdlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "..\MultimediaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFastDialsDlg dialog


CFastDialsDlg::CFastDialsDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CFastDialsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFastDialsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Data = NULL;
}


void CFastDialsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFastDialsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFastDialsDlg, CCEDialog)
	//{{AFX_MSG_MAP(CFastDialsDlg)
ON_MESSAGE(WM_CHANGELANGUAGE,OnChangeLanguage)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_FASTDIALS_OK, OnButtonFastDialsOk)
	ON_BN_CLICKED(IDC_BUTTON_FASTDIALS_CANCEL, OnButtonFastDialsCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFastDialsDlg message handlers

BOOL CFastDialsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	std::string strTemp;
	CString str;
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER01);
	str = strTemp.c_str();
	m_sticNumber01.Create(str, WS_CHILD|WS_VISIBLE, CRect(52, 74*VHEIGHT, 102, 92*VHEIGHT), this);
	m_sticNumber01.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber01.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER02);
	str = strTemp.c_str();
	m_sticNumber02.Create(str, WS_CHILD|WS_VISIBLE, CRect(52, 94*VHEIGHT, 102, 112*VHEIGHT), this);
	m_sticNumber02.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber02.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER03);
	str = strTemp.c_str();
	m_sticNumber03.Create(str, WS_CHILD|WS_VISIBLE, CRect(52, 114*VHEIGHT, 102, 132*VHEIGHT), this);
	m_sticNumber03.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber03.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER04);
	str = strTemp.c_str();
	m_sticNumber04.Create(str, WS_CHILD|WS_VISIBLE, CRect(52, 134*VHEIGHT, 102, 152*VHEIGHT), this);
	m_sticNumber04.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber04.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER05);
	str = strTemp.c_str();
	m_sticNumber05.Create(str, WS_CHILD|WS_VISIBLE, CRect(52, 154*VHEIGHT, 102, 172*VHEIGHT), this);
	m_sticNumber05.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber05.SetAlign(SS_CENTER);
	
	m_edtNumber[0].Create(WS_CHILD|WS_VISIBLE|WS_HSCROLL|ES_AUTOHSCROLL, CRect(104, 74*VHEIGHT, 204, 92*VHEIGHT), this, IDC_EDIT_FASTDIALS_NUMBER01);
	m_edtNumber[0].SetLimitText(32);
	m_edtNumber[0].SetLimitDiagital();

	m_edtNumber[1].Create(WS_CHILD|WS_VISIBLE|WS_HSCROLL|ES_AUTOHSCROLL, CRect(104, 94*VHEIGHT, 204, 112*VHEIGHT), this, IDC_EDIT_FASTDIALS_NUMBER02);
	m_edtNumber[1].SetLimitText(32);
	m_edtNumber[1].SetLimitDiagital();

	m_edtNumber[2].Create(WS_CHILD|WS_VISIBLE|WS_HSCROLL|ES_AUTOHSCROLL, CRect(104, 114*VHEIGHT, 204, 132*VHEIGHT), this, IDC_EDIT_FASTDIALS_NUMBER03);
	m_edtNumber[2].SetLimitText(32);
	m_edtNumber[2].SetLimitDiagital();

	m_edtNumber[3].Create(WS_CHILD|WS_VISIBLE|WS_HSCROLL|ES_AUTOHSCROLL, CRect(104, 134*VHEIGHT, 204, 152*VHEIGHT), this, IDC_EDIT_FASTDIALS_NUMBER04);
	m_edtNumber[3].SetLimitText(32);
	m_edtNumber[3].SetLimitDiagital();

	m_edtNumber[4].Create(WS_CHILD|WS_VISIBLE|WS_HSCROLL|ES_AUTOHSCROLL, CRect(104, 154*VHEIGHT, 204, 172*VHEIGHT), this, IDC_EDIT_FASTDIALS_NUMBER05);
	m_edtNumber[4].SetLimitText(32);
	m_edtNumber[4].SetLimitDiagital();

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78*VHEIGHT, 434, 98*VHEIGHT), this, IDC_BUTTON_FASTDIALS_OK);
	m_btnOk.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103*VHEIGHT, 434, 123*VHEIGHT), this, IDC_BUTTON_FASTDIALS_CANCEL);
	m_btnCancel.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	m_sticBackground.Create(CRect(34, 40*VHEIGHT, 448, 184*VHEIGHT), this, 1);
	
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_FASTDIALS_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_FASTDIALS_TITLE).length());
	m_sticBackground.SetTextStruct(ts, 1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFastDialsDlg::OnButtonFastDialsOk()
{
	std::map<char, std::string> diallist;
	for (int i = 0; i < 12; i++)
	{
		CString s;
		m_edtNumber[i].GetWindowText(s);
		diallist[i+1] = Util::StringOp::FromCString(s);
		if(i == 4)
			break;
	}
	if(m_Data)
	{
		m_Data->speedDials(diallist);
		m_Data->Update();
	}
//====================================
	KeyBoardOff();
	ShowWindow_(FALSE);
}

void CFastDialsDlg::OnButtonFastDialsCancel()
{
	ShowWindow_(FALSE);
	
	KeyBoardOff();
}

void CFastDialsDlg::SetFastDialParam(boost::shared_ptr<Data::Setting> data)
{
	m_Data = data;
	std::map<char, std::string>  diallist = m_Data->speedDials();
	int idx = 0;
	for (std::map<char, std::string>::const_iterator i = diallist.begin(); i != diallist.end(); ++i)
	{
		std::string str = i->second;
		m_edtNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
		if(idx == 5)
			break;
	}
}
LRESULT CFastDialsDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
		switch(message)
		{
		case WM_OK:
			 OnButtonFastDialsOk();
			break;
		case WM_CANCEL:
			 OnButtonFastDialsCancel();
			break;
		}
		return CDialog::WindowProc(message,wParam,lParam);
}
LRESULT CFastDialsDlg::OnChangeLanguage(WPARAM wParam,LPARAM lParam)
{
  ChangeLanguage();
  return 0;
}
void CFastDialsDlg::ChangeLanguage()
{
	std::string strTemp;
	CString str;

	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER01);
	str = strTemp.c_str();
	m_sticNumber01.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER02);
	str = strTemp.c_str();
	m_sticNumber02.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER03);
	str = strTemp.c_str();
	m_sticNumber03.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER04);
	str = strTemp.c_str();
	m_sticNumber04.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER05);
	str = strTemp.c_str();
	m_sticNumber05.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.SetWindowText(str);

	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_FASTDIALS_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_FASTDIALS_TITLE).length());
	
	m_sticBackground.SetTextStruct(ts, 1);
}