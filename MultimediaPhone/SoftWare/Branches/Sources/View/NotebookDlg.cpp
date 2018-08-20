// NotebookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "NotebookDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../MultimediaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNotebookDlg dialog


CNotebookDlg::CNotebookDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNotebookDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNotebookDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNotebookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNotebookDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNotebookDlg, CDialog)
	//{{AFX_MSG_MAP(CNotebookDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_NOTE_OK, OnButtonNoteOk)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_PREV, OnButtonNotePrev)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_NEXT, OnButtonNoteNext)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_CLEAR, OnButtonNoteClear)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_CLOSE, OnButtonNoteClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNotebookDlg message handlers

BOOL CNotebookDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	std::string strTemp;
	CString str;
	
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 26, 472, 46), this, IDC_BUTTON_NOTE_OK);
//	m_btnOk.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_NOTE_BACKBTN);
	str = strTemp.c_str();
	m_btnPrev.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 46, 472, 66), this, IDC_BUTTON_NOTE_PREV);
//	m_btnPrev.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_NOTE_NEXTBTN);
	str = strTemp.c_str();
	m_btnNext.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 66, 472, 86), this, IDC_BUTTON_NOTE_NEXT);
//	m_btnNext.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_NOTE_CLEAR);
	str = strTemp.c_str();
	m_btnClear.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 86, 472, 106), this, IDC_BUTTON_NOTE_CLEAR);
//	m_btnClear.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnClose.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 178, 471, 198), this, IDC_BUTTON_NOTE_CLOSE);
	m_btnClose.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	m_WriteStatic.Create(L"", WS_VISIBLE|WS_CHILD, CRect(8, 27, 416, 198), this, 8950);
	m_WriteStatic.SetWrite(_T("/flashdrv/my_note/1.bmp"));

	m_sticPanel.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(416, 26, 472, 198), this);
	m_sticPanel.SetColor(RGB(0, 0, 0), Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);

	m_sticBackground.Create(CRect(0, 0, 480, 204), this, 2);

	m_nIndex = 0;

	char txt[64];
	sprintf(txt, "%d/20 %s", m_nIndex+1, Data::LanguageResource::Get(Data::RI_NOTE_PAGE).c_str());
	CString s = txt;
	// 	m_sticPage.SetWindowText(s);
	// 	m_sticTitle.SetWindowText(s);
	
	TextStruct ts[2];
	memset(ts, 0, sizeof(TextStruct) * 2);
	ts[0].txtRect = CRect(0, 0, 60, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_BOTTOM;
//	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_MAIN_STOCKINFOBTN).c_str(), Data::LanguageResource::Get(Data::RI_MAIN_STOCKINFOBTN).length());
	
// 	ts[1].txtRect = CRect(220, 4, 300, 24);
	ts[1].txtRect = CRect(200, 0, 280, 20);
	memcpy(ts[1].sTxt, txt, 64);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_CENTER | DT_BOTTOM;
	
	m_sticBackground.SetTextStruct(ts, 2, FALSE);
	//SetPageIndex();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNotebookDlg::SetPageIndex()
{
	char txt[64];
	sprintf(txt, "%d/20 %s", m_nIndex+1, Data::LanguageResource::Get(Data::RI_NOTE_PAGE).c_str());
	CString s = txt;
// 	m_sticPage.SetWindowText(s);
// 	m_sticTitle.SetWindowText(s);
	
	TextStruct ts[2];
	memset(ts, 0, sizeof(TextStruct) * 2);
	ts[0].txtRect = CRect(0, 0, 60, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_BOTTOM;
	//	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_MAIN_STOCKINFOBTN).c_str(), Data::LanguageResource::Get(Data::RI_MAIN_STOCKINFOBTN).length());
	
	// 	ts[1].txtRect = CRect(220, 4, 300, 24);
	ts[1].txtRect = CRect(200, 0, 280, 20);
	memcpy(ts[1].sTxt, txt, 64);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_CENTER | DT_BOTTOM;

	m_sticBackground.ChangeTextStruct(2-1, ts[1], TRUE);
	//m_sticBackground.SetTextStruct(ts, 2);
}

void CNotebookDlg::OnButtonNoteOk()
{
// 	m_WriteStatic.GetWrite(filename[m_nIndex]);
	std::string filename = "/flashdrv/my_note/";
	filename += Util::StringOp::FromInt(m_nIndex + 1);
	filename += ".bmp";
	m_WriteStatic.GetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
// 	ShowWindow(FALSE);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
}

void CNotebookDlg::OnButtonNotePrev()
{
	std::string filename = "/flashdrv/my_note/";
	filename += Util::StringOp::FromInt(m_nIndex + 1);
	filename += ".bmp";
	m_WriteStatic.GetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
//	m_WriteStatic.GetWrite(filename[m_nIndex]);
	m_nIndex--;
	if(m_nIndex < 0)
		m_nIndex = 19;
	
	filename = "/flashdrv/my_note/";
	filename += Util::StringOp::FromInt(m_nIndex + 1);
	filename += ".bmp";
	m_WriteStatic.SetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
// 	m_WriteStatic.SetWrite(filename[m_nIndex]);
	
	SetPageIndex();
}

void CNotebookDlg::OnButtonNoteNext()
{
	std::string filename = "/flashdrv/my_note/";
	filename += Util::StringOp::FromInt(m_nIndex + 1);
	filename += ".bmp";
	m_WriteStatic.GetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
//	m_WriteStatic.GetWrite(filename[m_nIndex]);
	m_nIndex++;
	if(m_nIndex > 19)
		m_nIndex = 0;
	filename = "/flashdrv/my_note/";
	filename += Util::StringOp::FromInt(m_nIndex + 1);
	filename += ".bmp";
	m_WriteStatic.SetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
//	m_WriteStatic.SetWrite(filename[m_nIndex]);
	
	SetPageIndex();
}

void CNotebookDlg::OnButtonNoteClear()
{
	m_WriteStatic.ClearDC();
}

void CNotebookDlg::OnButtonNoteClose()
{
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
}