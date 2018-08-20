// playsounddlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\MultimediaPhone.h"
#include "playsounddlg.h"
#include "../Data/LanguageResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlaySoundDlg dialog


CPlaySoundDlg::CPlaySoundDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CPlaySoundDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlaySoundDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPlaySoundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlaySoundDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlaySoundDlg, CCEDialog)
	//{{AFX_MSG_MAP(CPlaySoundDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SOUNDDLG_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SOUNDDLG_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_SOUNDDLG_PREV, OnButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_SOUNDDLG_PLAY, OnButtonPlay)
	ON_BN_CLICKED(IDC_CHECK_SOUNDDLG_PLAYCONTINUE, OnCheckPlaycontinue)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlaySoundDlg message handlers

BOOL CPlaySoundDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	//CenterWindow(GetDesktopWindow());	// center to the hpc screen

	int X = 35;
	int Y = 40;
	std::string strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	CString str = strTemp.c_str();
	m_btnClose.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 25 + Y, 402 + X, 45 + Y), this, IDC_BUTTON_SOUNDDLG_CLOSE);
	m_btnClose.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnClose.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnClose.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));
	
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PREV);
	str = strTemp.c_str();
	m_btnPrev.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 50 + Y, 402 + X, 70 + Y), this, IDC_BUTTON_SOUNDDLG_PREV);
	m_btnPrev.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnPrev.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnPrev.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_TEXT);
	str = strTemp.c_str();
	m_btnNext.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 75 + Y, 402 + X, 95 + Y), this, IDC_BUTTON_SOUNDDLG_NEXT);
	m_btnNext.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnNext.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnNext.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));
	
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PLAY);
	str = strTemp.c_str();
	m_btnPlay.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 100 + Y, 402 + X, 120 + Y), this, IDC_BUTTON_SOUNDDLG_PLAY);
	m_btnPlay.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnPlay.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnPlay.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CONTINUE);
	str = strTemp.c_str();
	m_chkPlayContinue.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(25 + X, 100 + Y, 200 + X, 120 + Y), this, IDC_CHECK_SOUNDDLG_PLAYCONTINUE);
	m_chkPlayContinue.SetIcon(IDI_ICON1, CSize(12, 12), IDI_ICON2, CSize(12, 12));
	m_chkPlayContinue.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_chkPlayContinue.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_chkPlayContinue.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_TITLE);
	str = strTemp.c_str();
	m_sticTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0 + X, 0 + Y, 410 + X, 20 + Y), this);
	SetStaticDefaultColor(&m_sticTitle);
	m_sticTitle.SetLeftMargin(180);

	m_sticNameNO.Create(str, WS_CHILD|WS_VISIBLE, CRect(25 + X, 25 + Y, 200 + X, 45 + Y), this);
	SetStaticDefaultColor(&m_sticNameNO);

	m_sticSoundName.Create(str, WS_CHILD|WS_VISIBLE, CRect(25 + X, 50 + Y, 200 + X, 70 + Y), this);
	SetStaticDefaultColor(&m_sticSoundName);

	m_sticSoundDuration.Create(str, WS_CHILD|WS_VISIBLE, CRect(250 + X, 50 + Y, 320 + X, 70 + Y), this);
	SetStaticDefaultColor(&m_sticSoundDuration);

	m_procbarSound.Create(WS_CHILD|WS_VISIBLE, CRect(25 + X, 75 + Y, 320 + X, 95 + Y), this, 0xFF10);
	m_procbarSound.SetParam(0, 0, 50, 1);
	m_procbarSound.SetPos(0);

	m_sticBackground.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(X, Y, 410 + X, 125 + Y), this);
	m_sticBackground.SetColor(RGB(255,255,255), RGB(255,255,255));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlaySoundDlg::SetStaticDefaultColor(CCEStatic* cestatic)
{
 	cestatic->SetColor(RGB(0, 0, 0), RGB(194,235,149));
//	cestatic->SetColor(RGB(0, 0, 0), RGB(240,250,223));
}


void CPlaySoundDlg::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(FALSE);
}

void CPlaySoundDlg::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	
}

void CPlaySoundDlg::OnButtonPrev() 
{
	// TODO: Add your control notification handler code here
	
}

void CPlaySoundDlg::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here
	
}

void CPlaySoundDlg::OnCheckPlaycontinue() 
{
	// TODO: Add your control notification handler code here
	
}
