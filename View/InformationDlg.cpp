// InformationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "InformationDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../MultimediaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInformationDlg dialog


CInformationDlg::CInformationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInformationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInformationDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CInformationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInformationDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInformationDlg, CDialog)
	//{{AFX_MSG_MAP(CInformationDlg)
		ON_MESSAGE(WM_CHANGELANGUAGE,OnChangeLanguage)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_NOTE_OK, OnButtonNoteOk)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_PREV, OnButtonNotePrev)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_NEXT, OnButtonNoteNext)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_CLEAR, OnButtonNoteClear)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_CLOSE, OnButtonNoteClose)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInformationDlg message handlers

BOOL CInformationDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	std::string strTemp;
	CString str;
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SAVE);
	str = strTemp.c_str();
	m_btnOk.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 26*VHEIGHT, 472, 46*VHEIGHT), this, IDC_BUTTON_NOTE_OK);

	
	strTemp = Data::LanguageResource::Get(Data::RI_NOTE_BACKBTN);
	str = strTemp.c_str();
	m_btnPrev.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 46*VHEIGHT, 472, 66*VHEIGHT), this, IDC_BUTTON_NOTE_PREV);

	
	strTemp = Data::LanguageResource::Get(Data::RI_NOTE_NEXTBTN);
	str = strTemp.c_str();
	m_btnNext.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 66*VHEIGHT, 472, 86*VHEIGHT), this, IDC_BUTTON_NOTE_NEXT);

	
	strTemp = Data::LanguageResource::Get(Data::RI_NOTE_CLEAR);
	str = strTemp.c_str();
	m_btnClear.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 86*VHEIGHT, 472, 106*VHEIGHT), this, IDC_BUTTON_NOTE_CLEAR);


	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnClose.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 178*VHEIGHT, 471, 198*VHEIGHT), this, IDC_BUTTON_NOTE_CLOSE);
	m_btnClose.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	m_WriteStatic.Create(L"", WS_VISIBLE|WS_CHILD, CRect(8, 27*VHEIGHT, 416, 198*VHEIGHT), this, 8950);
	//m_WriteStatic.SetWrite(_T("/flashdrv/my_note/1.bmp"));
    m_WriteStatic.SetWrite(_T("/hive/my_note/1.bmp"));

	m_sticPanel.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(416, 26*VHEIGHT, 472, 198*VHEIGHT), this);
	m_sticPanel.SetColor(RGB(0, 0, 0), Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);

	m_sticBackground.Create(CRect(0, 0, 480, 204*VHEIGHT), this, 2);

	m_nIndex = 0;

	char txt[64];
	sprintf(txt, "%d/20 %s", m_nIndex+1, Data::LanguageResource::Get(Data::RI_NOTE_PAGE).c_str());
	CString s = txt;
	// 	m_sticPage.SetWindowText(s);
	// 	m_sticTitle.SetWindowText(s);
	
	TextStruct ts[2];
	memset(ts, 0, sizeof(TextStruct) * 2);
	ts[0].txtRect = CRect(0, 0, 60, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_BOTTOM;	
// 	ts[1].txtRect = CRect(220, 4, 300, 24);
	ts[1].txtRect = CRect(200, 0, 280, 20*VHEIGHT);
	memcpy(ts[1].sTxt, txt, 64);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_CENTER | DT_BOTTOM;
	
	m_sticBackground.SetTextStruct(ts, 2, FALSE);
//	SetPageIndex();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInformationDlg::SetPageIndex()
{
	char txt[64];
	sprintf(txt, "%d/20 %s", m_nIndex+1, Data::LanguageResource::Get(Data::RI_NOTE_PAGE).c_str());//?
	CString s = txt;
// 	m_sticPage.SetWindowText(s);
// 	m_sticTitle.SetWindowText(s);
	
	TextStruct ts[2];
	memset(ts, 0, sizeof(TextStruct) * 2);
	ts[0].txtRect = CRect(0, 0, 60, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_BOTTOM;	
// 	ts[1].txtRect = CRect(220, 4, 300, 24);
	ts[1].txtRect = CRect(200, 0, 280, 20*VHEIGHT);
	memcpy(ts[1].sTxt, txt, 64);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_CENTER | DT_BOTTOM;

	m_sticBackground.ChangeTextStruct(2-1, ts[1], TRUE);
	m_sticBackground.SetTextStruct(ts, 2);
}

void CInformationDlg::OnButtonNoteOk()
{
// 	m_WriteStatic.GetWrite(filename[m_nIndex]);
	std::string filename = "/hive/my_note/";
	filename += Util::StringOp::FromInt(m_nIndex + 1);
	filename += ".bmp";
	m_WriteStatic.GetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
// 	ShowWindow(FALSE);

//	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
}

void CInformationDlg::OnButtonNotePrev()
{
	std::string filename = "/hive/my_note/";
	filename += Util::StringOp::FromInt(m_nIndex + 1);
	filename += ".bmp";
	m_WriteStatic.GetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
//	m_WriteStatic.GetWrite(filename[m_nIndex]);
	m_nIndex--;
	if(m_nIndex < 0)
		m_nIndex = 19;
	
	filename = "/hive/my_note/";
	filename += Util::StringOp::FromInt(m_nIndex + 1);
	filename += ".bmp";
	m_WriteStatic.SetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
// 	m_WriteStatic.SetWrite(filename[m_nIndex]);
	
	SetPageIndex();
}

void CInformationDlg::OnButtonNoteNext()
{
	std::string filename = "/hive/my_note/";
	filename += Util::StringOp::FromInt(m_nIndex + 1);
	filename += ".bmp";
	m_WriteStatic.GetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
//	m_WriteStatic.GetWrite(filename[m_nIndex]);
	m_nIndex++;
	if(m_nIndex > 19)
		m_nIndex = 0;
	filename = "/hive/my_note/";
	filename += Util::StringOp::FromInt(m_nIndex + 1);
	filename += ".bmp";
	m_WriteStatic.SetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
//	m_WriteStatic.SetWrite(filename[m_nIndex]);
	
	SetPageIndex();
}

void CInformationDlg::OnButtonNoteClear()
{
	m_WriteStatic.ClearDC();
}

void CInformationDlg::OnButtonNoteClose()
{
    ShowWindow(SW_HIDE);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
}
LRESULT CInformationDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_OK:
		OnButtonNoteOk();
		break;
	case WM_DELETE:
		OnButtonNoteClear();
		break;
	case WM_CANCEL:
		OnButtonNoteClose();
		break;
	case WM_NEWCONTACT:
         ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_CONTACT);
		break;
	case WM_PLAYLEAVEWORD:
         ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_SOUND);
		break;
	case WM_KEYDOWN:
		if ('D'==wParam)
		{
			OnButtonNoteNext();
		}
		if ('U'==wParam)
		{
			OnButtonNotePrev();
		}
		break;
	}
	return CDialog::WindowProc(message,wParam,lParam);
}
LRESULT CInformationDlg::OnChangeLanguage(WPARAM wParam,LPARAM lParam)
{
 ChangeLanguage();
 return 0;
}
void CInformationDlg::ChangeLanguage()
{
	std::string strTemp;
	CString str;
	
	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SAVE);
	str = strTemp.c_str();
	m_btnOk.SetWindowText(str);
	
	strTemp = Data::LanguageResource::Get(Data::RI_NOTE_BACKBTN);
	str = strTemp.c_str();
	m_btnPrev.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_NOTE_NEXTBTN);
	str = strTemp.c_str();
	m_btnNext.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_NOTE_CLEAR);
	str = strTemp.c_str();
 	m_btnClear.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnClose.SetWindowText(str);

    char txt[64];
	sprintf(txt, "%d/20 %s", m_nIndex+1, Data::LanguageResource::Get(Data::RI_NOTE_PAGE).c_str());
	CString s = txt;
	
	TextStruct ts[2];
	memset(ts, 0, sizeof(TextStruct) * 2);
	ts[0].txtRect = CRect(0, 0, 60, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_BOTTOM;

	ts[1].txtRect = CRect(200, 0, 280, 20*VHEIGHT);
	memcpy(ts[1].sTxt, txt, 64);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_CENTER | DT_BOTTOM;
	
	m_sticBackground.SetTextStruct(ts, 2, FALSE);
}
