// DetailTip.cpp : implementation file
//

#include "stdafx.h"
#include "DetailTip.h"
#include <string>
#include "..\Data\LanguageResource.h"
#include "..\Data\SkinStyle.h"
#include "windef.h"
#include "..\MultimediaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetailTip dialog


CDetailTip::CDetailTip(CWnd* pParent /*=NULL*/)
	: CCEDialog(CDetailTip::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetailTip)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDetailTip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetailTip)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetailTip, CCEDialog)
	//{{AFX_MSG_MAP(CDetailTip)
	ON_BN_CLICKED(IDC_BUTTON_DETAIL_OK,OnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetailTip message handlers

LRESULT CDetailTip::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{ 
	  case WM_CHANGELANGUAGE:
		OnChangeLanguage(wParam,lParam);
		break;
	  case WM_CANCEL:
		 OnExit();
		 break;
	  case WM_OK:
		 OnExit();
		 break;
	}

  return CCEDialog::WindowProc(message,wParam,lParam);
}

BOOL CDetailTip::OnInitDialog()
{
	CCEDialog::OnInitDialog();

	std::string strTemp;
	CString str;
	CString str1;
	int fontSize = 18;
	int right = 97;
		
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_NAMESTC);
	str=strTemp.c_str()+CString(":");
	m_sticName.Create(str, WS_CHILD|WS_VISIBLE, CRect(55, 73*VHEIGHT, right, 93*VHEIGHT), this, IDC_STATIC_TELEPHONE_NAMENO);
	m_sticName.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticName.SetFontSize(fontSize);

	m_sticDynName.Create(str1, WS_CHILD|WS_VISIBLE, CRect(right+3, 73*VHEIGHT, 360, 93*VHEIGHT), this, IDC_STATIC_TELEPHONE_NAMENO);
	m_sticDynName.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticDynName.SetFontSize(fontSize);

	strTemp=Data::LanguageResource::Get(Data::DETAIL_NUMBER);
	str=strTemp.c_str()+CString(":");
	m_sticNumber.Create(str, WS_CHILD|WS_VISIBLE, CRect(55, 93*VHEIGHT, right, 113*VHEIGHT), this, IDC_STATIC_TELEPHONE_NAMENO);
	m_sticNumber.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber.SetFontSize(fontSize);

	m_sticDynNumber.Create(str, WS_CHILD|WS_VISIBLE, CRect(right+3, 93*VHEIGHT, 360, 113*VHEIGHT), this, IDC_STATIC_TELEPHONE_NAMENO);
	m_sticDynNumber.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticDynNumber.SetFontSize(fontSize);

	strTemp=Data::LanguageResource::Get(Data::RI_RECORD_TIME);
	str=strTemp.c_str()+CString(":");
	m_sticStartime.Create(str, WS_CHILD|WS_VISIBLE, CRect(55, 113*VHEIGHT, right, 133*VHEIGHT), this, IDC_STATIC_TELEPHONE_NAMENO);
	m_sticStartime.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticStartime.SetFontSize(fontSize);

	m_sticDynStartime.Create(str1, WS_CHILD|WS_VISIBLE, CRect(right+3, 113*VHEIGHT, 360, 133*VHEIGHT), this, IDC_STATIC_TELEPHONE_NAMENO);
	m_sticDynStartime.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticDynStartime.SetFontSize(fontSize);

	right = 115 ;
	strTemp=Data::LanguageResource::Get(Data::RI_RECORD_DURATION);
	str=strTemp.c_str()+CString(":");
	m_sticDuration.Create(str, WS_CHILD|WS_VISIBLE, CRect(55, 133*VHEIGHT, right, 153*VHEIGHT), this, IDC_STATIC_TELEPHONE_NAMENO);
	m_sticDuration.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticDuration.SetFontSize(fontSize);

	m_sitcDynDuration.Create(str1, WS_CHILD|WS_VISIBLE, CRect(right+3, 133*VHEIGHT, 360, 153*VHEIGHT), this, IDC_STATIC_TELEPHONE_NAMENO);
	m_sitcDynDuration.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sitcDynDuration.SetFontSize(fontSize);

	right = 97 ;
	strTemp=Data::LanguageResource::Get(Data::RI_RECORD_SOUND);
	str=strTemp.c_str()+CString(":");
	m_sticRecordCaption.Create(str, WS_CHILD|WS_VISIBLE, CRect(55, 153*VHEIGHT, right, 173*VHEIGHT), this, IDC_STATIC_TELEPHONE_NAMENO);
	m_sticRecordCaption.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticRecordCaption.SetFontSize(fontSize);

	m_sticDynRecordCaption.Create(str1, WS_CHILD|WS_VISIBLE, CRect(right+3, 153*VHEIGHT, 360, 173*VHEIGHT), this, IDC_STATIC_TELEPHONE_NAMENO);
	m_sticDynRecordCaption.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticDynRecordCaption.SetFontSize(fontSize-2);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78*VHEIGHT, 434, 98*VHEIGHT), this, IDC_BUTTON_DETAIL_OK);
	m_btnOk.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	m_sticBackground.Create(CRect(34, 40*VHEIGHT, 448, 184*VHEIGHT), this, 1);
	
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::DETAIL_TIP).c_str(), Data::LanguageResource::Get(Data::DETAIL_TIP).length());
	m_sticBackground.SetTextStruct(ts,1);
	m_tip=false;
	
	return TRUE;
}

void CDetailTip::OnExit()
{   
	ShowWindow_(FALSE);

	if (m_tip)
	{
		std::string strTemp=Data::LanguageResource::Get(Data::RI_CARD_NAMESTC);
		
		CString str=strTemp.c_str()+CString(":");
		
	    m_sticName.SetWindowText(str);

		m_tip=false;

	}
	else
	{
		m_tip=false;
	}
}

void CDetailTip::setDetialContent(CString Name,CString Number,CString StartTime,CString Duration,CString RecordCaption,bool flag)
{   
	if(flag)
	{    
		if (m_tip)
		{
			std::string strTemp=Data::LanguageResource::Get(Data::RI_ERROR);
			
			CString str=strTemp.c_str();
			
		    m_sticName.SetWindowText(str);

		}
		m_sticDynName.SetWindowText(Name);
		
		m_sticDynNumber.SetWindowText(Number);
		
		m_sticDynStartime.SetWindowText(StartTime);
		
		m_sitcDynDuration.SetWindowText(Duration);
		
		m_sticDynRecordCaption.SetWindowText(RecordCaption);

	}else
	{ 
		m_sticDynName.SetWindowText(Name);
		
		m_sticDynNumber.SetWindowText(Number);
		
		m_sticDynStartime.SetWindowText(StartTime);
		
		m_sitcDynDuration.SetWindowText(Duration);
		
		m_sticRecordCaption.SetWindowText(_T(""));
	}  
}

void CDetailTip::ChangeLanguage()
{
	std::string strTemp;
	
	CString str;
	
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_NAMESTC);
	
	str=strTemp.c_str()+CString(":");
	
	m_sticName.SetWindowText(str);

	strTemp=Data::LanguageResource::Get(Data::RI_COMN_OKBTN);

	str=strTemp.c_str();

	m_btnOk.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::DETAIL_NUMBER);
	
	str=strTemp.c_str()+CString(":");
	
	m_sticNumber.SetWindowText(str);
	
	strTemp=Data::LanguageResource::Get(Data::RI_RECORD_TIME);
	
	str=strTemp.c_str()+CString(":");
	
	m_sticStartime.SetWindowText(str);
	
	strTemp=Data::LanguageResource::Get(Data::RI_RECORD_DURATION);
	
	str=strTemp.c_str()+CString(":");
	
	m_sticDuration.SetWindowText(str);
	
	strTemp=Data::LanguageResource::Get(Data::RI_RECORD_SOUND);
	
	str=strTemp.c_str()+CString(":");
	
	m_sticRecordCaption.SetWindowText(str);
	
	TextStruct ts[1];
	
	memset(ts, 0, sizeof(TextStruct) * 1);
	   
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
	   
	ts[0].txtFontSize = 16;
	   
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	   
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::DETAIL_TIP).c_str(), Data::LanguageResource::Get(Data::DETAIL_TIP).length());
	
	m_sticBackground.SetTextStruct(ts,1);
 
}

LRESULT CDetailTip::OnChangeLanguage(WPARAM wParam,LPARAM lParam)
{
  ChangeLanguage();

  return 0;
}
void CDetailTip::setHwnd(HWND handle)
{
	m_handle=handle;
}
void CDetailTip::setTIp(bool flag)
{
	m_tip=flag;
}
void CDetailTip::setm_vCurrentResult(std::vector<boost::shared_ptr<Data::SoundSegment> > &tempm_vCurrentResult)
{
	m_vCurrentResult=tempm_vCurrentResult;
}
void CDetailTip::setTipIndex(int nindex)
{
    index=nindex;
}
void CDetailTip::setTypeIndex(int index)
{
	Typeindex=index;
}
