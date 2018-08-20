// soundsearchdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "soundsearchdlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SoundSegment.h"
// #include "../Data/ContactInfo.h"
// #include "../Data/Contact.h"
#include "SoundDlg.h"
#include "../Data/SkinStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSoundSearchDlg dialog


CSoundSearchDlg::CSoundSearchDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CSoundSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSoundSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSoundSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSoundSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSoundSearchDlg, CCEDialog)
	//{{AFX_MSG_MAP(CSoundSearchDlg)
	ON_MESSAGE(WM_CHANGELANGUAGE,OnChangeLanguage)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SOUNDSEARCH_CANCEL, OnButtonSearchSoundCancel)
	ON_BN_CLICKED(IDC_BUTTON_SOUNDSEARCH_SEARCH, OnButtonSearchSoundSearch)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoundSearchDlg message handlers

BOOL CSoundSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	std::string strTemp;
	CString str;
	
	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TIP);
	str = strTemp.c_str();
	m_sticTip.Create(str, WS_CHILD|WS_VISIBLE, CRect(62, 73*VHEIGHT, 340, 91*VHEIGHT), this);
	m_sticTip.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TIME);
	str = strTemp.c_str();
	m_sticTimeRange.Create(str, WS_CHILD|WS_VISIBLE, CRect(50, 98*VHEIGHT, 144, 116*VHEIGHT), this);
	m_sticTimeRange.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticTimeRange.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TIMETO);
	str = strTemp.c_str();
	str = "-";
	m_sticTo.Create(str, WS_CHILD|WS_VISIBLE, CRect(241, 98*VHEIGHT, 245, 116*VHEIGHT), this);
	m_sticTo.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticTo.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_NAME);
	str = strTemp.c_str();
	m_sticName.Create(str, WS_CHILD|WS_VISIBLE, CRect(50, 123*VHEIGHT, 144, 141*VHEIGHT), this);
	m_sticName.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticName.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_NUMBER);
	str = strTemp.c_str();
	m_sticNumber.Create(str, WS_CHILD|WS_VISIBLE, CRect(50, 148*VHEIGHT, 144, 166*VHEIGHT), this);
	m_sticNumber.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber.SetAlign(SS_CENTER);

	m_edtName.Create(WS_CHILD|WS_VISIBLE, CRect(148, 123*VHEIGHT, 338, 141*VHEIGHT), this, IDC_EDIT_SOUNDSEARCH_NAME);
	m_edtName.SetLimitText(15);

	m_edtNumber.Create(WS_CHILD|WS_VISIBLE, CRect(148, 148*VHEIGHT, 338, 166*VHEIGHT), this, IDC_EDIT_SOUNDSEARCH_NUMBER);
	m_edtNumber.SetLimitText(15);
	m_edtNumber.SetLimitDiagital();

	m_dtFrom.Create(WS_VISIBLE|WS_CHILD|DTS_SHORTDATEFORMAT, CRect(148, 98*VHEIGHT, 240, 116*VHEIGHT), this, IDC_DATETIME_SOUNDSEARCH_FROM);
	m_dtTo.Create(WS_VISIBLE|WS_CHILD|DTS_SHORTDATEFORMAT, CRect(246, 98*VHEIGHT, 338, 116*VHEIGHT), this, IDC_DATETIME_SOUNDSEARCH_TO);
	
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_FINDBTN);
	str = strTemp.c_str();
	m_btnSearch.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78*VHEIGHT, 434, 98*VHEIGHT), this, IDC_BUTTON_SOUNDSEARCH_SEARCH);
	m_btnSearch.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103*VHEIGHT, 434, 123*VHEIGHT), this, IDC_BUTTON_SOUNDSEARCH_CANCEL);
	m_btnCancel.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	m_sticBackground.Create(CRect(34, 40*VHEIGHT, 448, 184*VHEIGHT), this, 1);
	
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_SOUND_FIND_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_SOUND_FIND_TITLE).length());
	
	m_sticBackground.SetTextStruct(ts, 1);
	m_iType = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSoundSearchDlg::OnButtonSearchSoundCancel() 
{
	// TODO: Add your control notification handler code here
 	m_iType = 0;
	ShowWindow_(FALSE);

	KeyBoardOff();
}

void CSoundSearchDlg::OnButtonSearchSoundSearch() 
{
	// TODO: Add your control notification handler code here
	ShowWindow_(FALSE);
	
	KeyBoardOff();
	
    CTime dateFrom;
    CTime dateTo;
	m_dtFrom.GetTime(dateFrom);
	m_dtTo.GetTime(dateTo);
	std::string filter = "startTime BETWEEN '";
	char buff[32];
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d000000", dateFrom.GetYear(), dateFrom.GetMonth(),dateFrom.GetDay());//, dateFrom.GetHour(), dateFrom.GetMinute(), dateFrom.GetSecond());
	CString str1(buff);
	filter += buff;
	filter += "' AND '";
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d235959", dateTo.GetYear(), dateTo.GetMonth(),dateTo.GetDay());//, dateTo.GetHour(), dateTo.GetMinute(), dateTo.GetSecond());
	filter += buff;
	filter += "'";
	if(str1.Compare(CString(buff)) > 0)
	{
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->m_pDeleteTipDlg->SetPasswordModel(false);
		main->m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
		std::string str1 = Data::LanguageResource::Get(Data::RI_DELETETIP_ERRORDATE);
		main->m_pDeleteTipDlg->SetDelTip(str1.c_str());
		main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		return;
	}

	CString csTel;
	m_edtNumber.GetWindowText(csTel);
	CString csName;
	m_edtName.GetWindowText(csName);

	std::string strName = Util::StringOp::FromCString(csName);
	if (!csTel.IsEmpty() && !csName.IsEmpty())
	{
		filter += " AND telephoneNumber Like '%";
		filter += Util::StringOp::FromCString(csTel);
		filter += "%'";
		filter += " AND (name Like '%";
		filter += Util::StringOp::FromCString(csName);
		filter += "%'";
		filter += " OR tipName Like '%";
		filter += Util::StringOp::FromCString(csName);
		filter += "%')";
	}
	else if (!csTel.IsEmpty() && csName.IsEmpty())
	{
		filter += " AND telephoneNumber Like '%";
		filter += Util::StringOp::FromCString(csTel);
		filter += "%'";	
	}
	else if (csTel.IsEmpty() && !csName.IsEmpty())
	{
		filter += " AND (name Like '%";
		filter += Util::StringOp::FromCString(csName);
		filter += "%'";
		filter += " OR tipName Like '%";
		filter += Util::StringOp::FromCString(csName);
		filter += "%')";
	}
	((CSoundDlg*)m_pParentWnd)->Search(filter);
	m_iType = 0;
}
LRESULT CSoundSearchDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CANCEL:
		if(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->m_pSoundSearchDlg->IsWindowVisible())
		{
         OnButtonSearchSoundCancel();
		}
		
		break;
	case WM_OK:
		if(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->m_pSoundSearchDlg->IsWindowVisible())
		{
		  OnButtonSearchSoundSearch();
		}
		
		break;
	}
	return CDialog::WindowProc(message,wParam,lParam);
}

LRESULT CSoundSearchDlg::OnChangeLanguage(WPARAM wParam,LPARAM lParam)
{
	ChangeLanguage();
	return 0;

}
void CSoundSearchDlg::ChangeLanguage()
{
	std::string strTemp;

	CString str;
	
	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TIP);

	str = strTemp.c_str();

	m_sticTip.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TIME);

	str = strTemp.c_str();

	m_sticTimeRange.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TIMETO);

	str = strTemp.c_str();

	str = "-";

	m_sticTo.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_NAME);

	str = strTemp.c_str();

	m_sticName.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_NUMBER);

	str = strTemp.c_str();

	m_sticNumber.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_NUMBER);

	str = strTemp.c_str();

	m_sticNumber.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_FINDBTN);

	str = strTemp.c_str();

	m_btnSearch.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);

	str = strTemp.c_str();

	m_btnCancel.SetWindowText(str);

	TextStruct ts[1];

	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);

	ts[0].txtFontSize = 16;

	ts[0].sAlign = DT_LEFT | DT_BOTTOM;

	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_SOUND_FIND_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_SOUND_FIND_TITLE).length());
	
	m_sticBackground.SetTextStruct(ts, 1);
}

void CSoundSearchDlg::SetSertchCondition(int type)
{
	m_iType = type;

	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	m_dtFrom.SetTime(curtime);
	m_dtTo.SetTime(curtime);
}