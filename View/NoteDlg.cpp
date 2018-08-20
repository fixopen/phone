// notedlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "notedlg.h"
#include "../Data/LanguageResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNoteDlg dialog


CNoteDlg::CNoteDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CNoteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNoteDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nIndex = 0;
}


void CNoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNoteDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNoteDlg, CCEDialog)
	//{{AFX_MSG_MAP(CNoteDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_NOTE_OK, OnButtonNoteOk)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_PREV, OnButtonNotePrev)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_NEXT, OnButtonNoteNext)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_CLEAR, OnButtonNoteClear)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoteDlg message handlers

BOOL CNoteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int X = 35;
	int Y = 12;
	std::string strTemp;
	CString str;

	strTemp = Data::LanguageResource::Get(Data::RI_NOTE_TITLE);
	str = strTemp.c_str();
	m_sticTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0 + X, 0 + Y, 340 + X, 20 + Y), this);
	m_sticTitle.SetColor(RGB(0, 0, 0), RGB(115,210,138));
	m_sticTitle.SetLeftMargin(160);
	//lxz 2007.10.17
	strTemp = Data::LanguageResource::Get(Data::RI_NOTE_PAGE);
	str = strTemp.c_str();
	m_sticPageNumber.Create(L"", WS_CHILD|WS_VISIBLE, CRect(340 + X, 0 + Y, 360 + X, 20 + Y), this, IDC_STATIC_NOTE_PAGENUMBER);
	m_sticPageNumber.SetColor(RGB(0, 0, 0), RGB(115,210,138));
	m_sticPageNumber.SetLeftMargin(160);

	strTemp = Data::LanguageResource::Get(Data::RI_NOTE_PAGE);
	str = strTemp.c_str();
	m_sticPage.Create(str, WS_CHILD|WS_VISIBLE, CRect(360 + X, 0 + Y, 410 + X, 20 + Y), this);
	m_sticPage.SetColor(RGB(0, 0, 0), RGB(115,210,138));
	m_sticPage.SetLeftMargin(160);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 25 + Y, 402 + X, 45 + Y), this, IDC_BUTTON_NOTE_OK);
	m_btnOk.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnOk.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnOk.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_NOTE_BACKBTN);
	str = strTemp.c_str();
	m_btnPrev.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 50 + Y, 402 + X, 70 + Y), this, IDC_BUTTON_NOTE_PREV);
	m_btnPrev.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnPrev.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnPrev.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_NOTE_NEXTBTN);
	str = strTemp.c_str();
	m_btnNext.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 75 + Y, 402 + X, 95 + Y), this, IDC_BUTTON_NOTE_NEXT);
	m_btnNext.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnNext.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnNext.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_NOTE_CLEAR);
	str = strTemp.c_str();
	m_btnClear.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 150 + Y, 402 + X, 170 + Y), this, IDC_BUTTON_NOTE_CLEAR);
	m_btnClear.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnClear.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnClear.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));
	
	m_WriteStatic.Create(L"", WS_VISIBLE|WS_CHILD, CRect(4+X, 34+Y, 336+X, 176+Y), this, 8950);
	int n = (336-4+1/8+1)*(176-34+1);
	pbuffer[0] = new char[n];
	pbuffer[1] = new char[n];
	pbuffer[2] = new char[n];
	
	memset(filename[0], 0, 128*2);
	memset(filename[1], 0, 128*2);
	memset(filename[2], 0, 128*2);
	mbstowcs(filename[0], "/flashdrv/my_note/1.bmp", strlen("/flashdrv/my_note/1.bmp"));
	mbstowcs(filename[1], "/flashdrv/my_note/2.bmp", strlen("/flashdrv/my_note/2.bmp"));
	mbstowcs(filename[2], "/flashdrv/my_note/3.bmp", strlen("/flashdrv/my_note/3.bmp"));
	
	m_WriteStatic.SetWrite(/*pbuffer[0]*/filename[0]);
	SetPageIndex();

	m_sticBackground.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(X, Y, 410 + X, 180 + Y), this);
	m_sticBackground.SetColor(RGB(255,255,255), RGB(255,255,255));

	
//	m_WriteStatic.ModifyStyle(0, SS_NOTIFY);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNoteDlg::SetPageIndex()
{
	char txt[64];
	sprintf(txt, "%d/3 %s", m_nIndex+1, Data::LanguageResource::Get(Data::RI_NOTE_PAGE).c_str());
	CString s = txt;
	m_sticPage.SetWindowText(s);
}

void CNoteDlg::OnButtonNoteOk()
{
	m_WriteStatic.GetWrite(filename[m_nIndex]);
	ShowWindow(FALSE);
}

void CNoteDlg::OnButtonNotePrev()
{
	m_WriteStatic.GetWrite(filename[m_nIndex]);
	m_nIndex--;
	if(m_nIndex < 0)
		m_nIndex = 2;
	m_WriteStatic.SetWrite(filename[m_nIndex]);

	SetPageIndex();
}

void CNoteDlg::OnButtonNoteNext()
{
	m_WriteStatic.GetWrite(filename[m_nIndex]);
	m_nIndex++;
	if(m_nIndex > 2)
		m_nIndex = 0;
	m_WriteStatic.SetWrite(filename[m_nIndex]);

	SetPageIndex();
}

void CNoteDlg::OnButtonNoteClear()
{
	m_WriteStatic.ClearDC();
}
