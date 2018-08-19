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
: CCEDialog(CNoteDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CNoteDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    //  m_nIndex = 0;
}


void CNoteDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CNoteDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CNoteDlg, CCEDialog)
    //{{AFX_MSG_MAP(CNoteDlg)
    //}}AFX_MSG_MAP
    //  ON_BN_CLICKED(IDC_BUTTON_NOTE_OK, OnButtonNoteOk)
    //  ON_BN_CLICKED(IDC_BUTTON_NOTE_PREV, OnButtonNotePrev)
    //  ON_BN_CLICKED(IDC_BUTTON_NOTE_NEXT, OnButtonNoteNext)
    //  ON_BN_CLICKED(IDC_BUTTON_NOTE_CLEAR, OnButtonNoteClear)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoteDlg message handlers

BOOL CNoteDlg::OnInitDialog() {
    CDialog::OnInitDialog();


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

// void CNoteDlg::SetPageIndex()
// {
//  char txt[64];
//  sprintf(txt, "%d/3 %s", m_nIndex+1, Data::LanguageResource::Get(Data::RI_NOTE_PAGE).c_str());
//  CString s = txt;
//  m_sticPage.SetWindowText(s);
// }
// 
// void CNoteDlg::OnButtonNoteOk()
// {
//  m_WriteStatic.GetWrite(filename[m_nIndex]);
//  ShowWindow(FALSE);
// }
// 
// void CNoteDlg::OnButtonNotePrev()
// {
//  m_WriteStatic.GetWrite(filename[m_nIndex]);
//  m_nIndex--;
//  if(m_nIndex < 0)
//      m_nIndex = 2;
//  m_WriteStatic.SetWrite(filename[m_nIndex]);
// 
//  SetPageIndex();
// }
// 
// void CNoteDlg::OnButtonNoteNext()
// {
//  m_WriteStatic.GetWrite(filename[m_nIndex]);
//  m_nIndex++;
//  if(m_nIndex > 2)
//      m_nIndex = 0;
//  m_WriteStatic.SetWrite(filename[m_nIndex]);
// 
//  SetPageIndex();
// }
// 
// void CNoteDlg::OnButtonNoteClear()
// {
//  m_WriteStatic.ClearDC();
// }
