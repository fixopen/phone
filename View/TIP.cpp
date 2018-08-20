// TIP.cpp : implementation file
//

#include "stdafx.h"
#include "TIP.h"
#include <string>
#include "..\Data\LanguageResource.h"
#include "..\Data\SkinStyle.h"
#include "..\Data\SoundSegment.h"
#include "..\MultimediaPhoneDlg.h"
#include "..\Util\socket.h"
#include "SoundDlg.h"
#include "..\Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTIP dialog
#define  CBS_SIMPLE 0x0001L

CTIP::CTIP(CWnd* pParent /*=NULL*/)
	: CCEDialog(CTIP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTIP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CTIP::OnInitDialog()
{
	CCEDialog::OnInitDialog();

	std::string strTemp;
	CString str;
    
	strTemp = Data::LanguageResource::Get(Data::RI_FILE_NMAE);
	str = strTemp.c_str();
	m_sticTip.Create(str, WS_CHILD|WS_VISIBLE, CRect(80, 100*VHEIGHT, 160, 125*VHEIGHT), this);
	m_sticTip.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticTip.SetFontSize(18);

	m_cmbTip.Create(WS_CHILD|WS_VISIBLE, CRect(160,105*VHEIGHT,340,125*VHEIGHT), this, IDC_EDIT_CONTACTNEW_NAME);
	m_cmbTip.SetWindowText(L"");
	m_cmbTip.SetLimitText(20);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);	
	str = strTemp.c_str();
	m_btnOk.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78*VHEIGHT, 434, 98*VHEIGHT), this, IDC_BUTTON_TIP_OK);
	m_btnOk.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103*VHEIGHT, 434, 123*VHEIGHT), this, IDC_BUTTON_TIP_CANCEL);
	m_btnCancel.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	m_sticBackground.Create(CRect(34, 40*VHEIGHT, 448, 184*VHEIGHT), this, 1);
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);  
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);  
	ts[0].txtFontSize = 16;  
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;  
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_MODIFY_FILE_NMAE).c_str(), Data::LanguageResource::Get(Data::RI_MODIFY_FILE_NMAE).length());
	
	m_sticBackground.SetTextStruct(ts,1);
	index=0;

	//AddTIpItems();
	return TRUE;

}
void CTIP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTIP)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTIP, CCEDialog)
	//{{AFX_MSG_MAP(CTIP)
		// NOTE: the ClassWizard will add message map macros here
	ON_BN_CLICKED(IDC_BUTTON_TIP_OK,OnButtonTipOk)
	ON_BN_CLICKED(IDC_BUTTON_TIP_CANCEL,OnButtonCancel)
	//ON_MESSAGE(WM_CHANGELANGUAGE,OnChangeLanguage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTIP message handlers

LRESULT CTIP::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	switch(message)
	{ 
	case WM_CHANGELANGUAGE:
		OnChangeLanguage(wParam,lParam);
		break;
		
	case WM_CANCEL:
		OnButtonCancel();
		break;
	 case WM_OK:
		 OnButtonTipOk();
		 break;
	}	
	return CCEDialog::WindowProc(message, wParam, lParam);
}
LRESULT CTIP::OnChangeLanguage(WPARAM wParam,LPARAM lParam)
{

  ChangeLanguage();

  return 0;
}
void CTIP::OnButtonCancel()
{
	  ShowWindow_(FALSE);

	  KeyBoardOff();
	  
	  m_cmbTip.SetWindowText(_T(""));
	  
      std::string  strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);	
	  
      CString str = strTemp.c_str();
	  
	  m_btnOk.SetWindowText(str);
}
void CTIP::OnButtonTipOk()
{ 
	ShowWindow_(FALSE);
	
	KeyBoardOff();
	
	m_cmbTip.GetWindowText(result);
	result.TrimLeft();
	result.TrimRight();
	m_vCurrentResult[index]->name(Util::StringOp::FromCString(result));
	m_vCurrentResult[index]->Update();
	m_cmbTip.SetWindowText(_T(""));
	std::string  strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);	
	CString str = strTemp.c_str();
	m_btnOk.SetWindowText(str);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->\
		ShowArrayInList(m_vCurrentResult);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->UpdateTips();

}
void CTIP::ChangeLanguage()
{
	std::string strTemp;
	CString str;

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_LEAVEWORDTIP);
	str = strTemp.c_str();
	m_sticTip.SetWindowText(str);
	
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);	
	str = strTemp.c_str();
    m_btnOk.SetWindowText(str);
	
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.SetWindowText(str);
	
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);  
	ts[0].txtFontSize = 18;  
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;  
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::DETAIL_TIP).c_str(), Data::LanguageResource::Get(Data::DETAIL_TIP).length());
	m_sticBackground.SetTextStruct(ts,1);

}

void CTIP::setTipIndex(int nindex)
{
    index=nindex;
}
void CTIP::setm_vCurrentResult(\
		std::vector<boost::shared_ptr<Data::SoundSegment> > &tempm_vCurrentResult)
{
	m_vCurrentResult=tempm_vCurrentResult;
}
void CTIP::setCurrentTIp(std::string currentTip)
{
	m_cmbTip.SetWindowText(Util::StringOp::ToCString(currentTip));
}
void CTIP::setTypeIndex(int index)
{
  Typeindex=index;
}
std::string CTIP::ReplaceSingleQuotation(std::string& field)
{
	for (int i = 0; i < field.length(); ++i)
	{
		if (field[i] == '\'')
		{
			field[i] = '\"';
		}
	}
	return field;
}