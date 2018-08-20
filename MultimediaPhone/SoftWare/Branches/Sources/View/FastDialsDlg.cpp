// fastdialsdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "fastdialsdlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"

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
	m_sticNumber01.Create(str, WS_CHILD|WS_VISIBLE, CRect(52, 72, 102, 88), this);
	m_sticNumber01.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber01.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER02);
	str = strTemp.c_str();
	m_sticNumber02.Create(str, WS_CHILD|WS_VISIBLE, CRect(52, 89, 102, 105), this);
	m_sticNumber02.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber02.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER03);
	str = strTemp.c_str();
	m_sticNumber03.Create(str, WS_CHILD|WS_VISIBLE, CRect(52, 106, 102, 122), this);
	m_sticNumber03.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber03.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER04);
	str = strTemp.c_str();
	m_sticNumber04.Create(str, WS_CHILD|WS_VISIBLE, CRect(52, 123, 102, 139), this);
	m_sticNumber04.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber04.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER05);
	str = strTemp.c_str();
	m_sticNumber05.Create(str, WS_CHILD|WS_VISIBLE, CRect(52, 140, 102, 156), this);
	m_sticNumber05.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber05.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER06);
	str = strTemp.c_str();
	m_sticNumber06.Create(str, WS_CHILD|WS_VISIBLE, CRect(52, 157, 102, 173), this);
	m_sticNumber06.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber06.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER07);
	str = strTemp.c_str();
	m_sticNumber07.Create(str, WS_CHILD|WS_VISIBLE, CRect(206, 72, 258, 88), this);
	m_sticNumber07.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber07.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER08);
	str = strTemp.c_str();
	m_sticNumber08.Create(str, WS_CHILD|WS_VISIBLE, CRect(206, 89, 258, 105), this);
	m_sticNumber08.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber08.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER09);
	str = strTemp.c_str();
	m_sticNumber09.Create(str, WS_CHILD|WS_VISIBLE, CRect(206, 106, 258, 122), this);
	m_sticNumber09.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber09.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER10);
	str = strTemp.c_str();
	m_sticNumber10.Create(str, WS_CHILD|WS_VISIBLE, CRect(206, 123, 258, 139), this);
	m_sticNumber10.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber10.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER11);
	str = strTemp.c_str();
	m_sticNumber11.Create(str, WS_CHILD|WS_VISIBLE, CRect(206, 140, 258, 156), this);
	m_sticNumber11.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber11.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER12);
	str = strTemp.c_str();
	m_sticNumber12.Create(str, WS_CHILD|WS_VISIBLE, CRect(206, 157, 258, 173), this);
	m_sticNumber12.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber12.SetAlign(SS_CENTER);
	
	m_edtNumber[0].Create(WS_CHILD|WS_VISIBLE, CRect(104, 72, 204, 88), this, IDC_EDIT_FASTDIALS_NUMBER01);
	m_edtNumber[0].SetLimitText(15);

	m_edtNumber[1].Create(WS_CHILD|WS_VISIBLE, CRect(104, 89, 204, 105), this, IDC_EDIT_FASTDIALS_NUMBER02);
	m_edtNumber[1].SetLimitText(15);

	m_edtNumber[2].Create(WS_CHILD|WS_VISIBLE, CRect(104, 106, 204, 122), this, IDC_EDIT_FASTDIALS_NUMBER03);
	m_edtNumber[2].SetLimitText(15);

	m_edtNumber[3].Create(WS_CHILD|WS_VISIBLE, CRect(104, 123, 204, 139), this, IDC_EDIT_FASTDIALS_NUMBER04);
	m_edtNumber[3].SetLimitText(15);

	m_edtNumber[4].Create(WS_CHILD|WS_VISIBLE, CRect(104, 140, 204, 156), this, IDC_EDIT_FASTDIALS_NUMBER05);
	m_edtNumber[4].SetLimitText(15);

	m_edtNumber[5].Create(WS_CHILD|WS_VISIBLE, CRect(104, 157, 204, 173), this, IDC_EDIT_FASTDIALS_NUMBER06);
	m_edtNumber[5].SetLimitText(15);

	m_edtNumber[6].Create(WS_CHILD|WS_VISIBLE, CRect(260, 72, 360, 88), this, IDC_EDIT_FASTDIALS_NUMBER07);
	m_edtNumber[6].SetLimitText(15);

	m_edtNumber[7].Create(WS_CHILD|WS_VISIBLE, CRect(260, 89, 360, 105), this, IDC_EDIT_FASTDIALS_NUMBER08);
	m_edtNumber[7].SetLimitText(15);

	m_edtNumber[8].Create(WS_CHILD|WS_VISIBLE, CRect(260, 106, 360, 122), this, IDC_EDIT_FASTDIALS_NUMBER09);
	m_edtNumber[8].SetLimitText(15);

	m_edtNumber[9].Create(WS_CHILD|WS_VISIBLE, CRect(260, 123, 360, 139), this, IDC_EDIT_FASTDIALS_NUMBER10);
	m_edtNumber[9].SetLimitText(15);

	m_edtNumber[10].Create(WS_CHILD|WS_VISIBLE, CRect(260, 140, 360, 156), this, IDC_EDIT_FASTDIALS_NUMBER11);
	m_edtNumber[10].SetLimitText(15);

	m_edtNumber[11].Create(WS_CHILD|WS_VISIBLE, CRect(260, 157, 360, 173), this, IDC_EDIT_FASTDIALS_NUMBER12);
	m_edtNumber[11].SetLimitText(15);
	
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78, 434, 98), this, IDC_BUTTON_FASTDIALS_OK);
	m_btnOk.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103, 434, 123), this, IDC_BUTTON_FASTDIALS_CANCEL);
	m_btnCancel.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	m_sticBackground.Create(CRect(34, 40, 448, 184), this, 1);
	
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20);
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
	}
	if(m_Data)
		m_Data->speedDials(diallist);

	if(theApp.inputDlg_->IsWindowVisible())  //show
	{
		theApp.inputDlg_->hide();
	}
	ShowWindow_(FALSE);
	theApp.inputDlg_->ShowWindow(FALSE);
}

void CFastDialsDlg::OnButtonFastDialsCancel()
{
	if(theApp.inputDlg_->IsWindowVisible())  //show
	{
		theApp.inputDlg_->hide();
	}
	ShowWindow_(FALSE);
	theApp.inputDlg_->ShowWindow(FALSE);
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
	}
}
