// LegendDlg.cpp : implementation file
//

#include "stdafx.h"
#include "multimediaphone.h"
#include "LegendDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLegendDlg

CLegendDlg::CLegendDlg()
{
	m_tHeight = 20/100;
	m_tWidth = 80/100;
}

CLegendDlg::~CLegendDlg()
{
}


BEGIN_MESSAGE_MAP(CLegendDlg, CStatic)
	//{{AFX_MSG_MAP(CLegendDlg)
	ON_NOTIFY(NM_CLICK, IDC_LEG_LIST, OnClickList)
	ON_BN_CLICKED(IDC_LEG_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_LEG_PAGEUP, OnButtonPageUp)
	ON_BN_CLICKED(IDC_LEG_PAGEDOWN, OnButtonPageDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLegendDlg message handlers
BOOL CLegendDlg::Create(DWORD dwStyle, CRect &rect, CWnd *pParentWnd, UINT nID, int fontHeight)
{
	VERIFY(m_font.CreateFont(
	   fontHeight,                // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   FW_NORMAL,                 // nWeight
	   FALSE,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   _T("ÐÂËÎÌå")));            // lpszFacename
	
	CStatic::Create(L"", dwStyle, rect, pParentWnd, nID);
	CRect rt;
	rt = rect;
	rt.right = rect.left + rect.Width()*m_tWidth;
	m_lstContent.Create(WS_CHILD|WS_VISIBLE|LVS_NOCOLUMNHEADER, rt, this, IDC_LEG_LIST);
	m_lstContent.SetFont(&m_font);
	rt.right = rect.left + rect.Width()*m_tHeight;
	rt.bottom = rect.top + rect.Height()*m_tHeight;
	rt.left = rect.left + rect.Width()*m_tWidth;
	m_btnClose.Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, rt, this, IDC_LEG_CLOSE);
	rt.top = rect.top + 3*rect.Height()*m_tHeight;
	m_btnPageUp.Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, rt, this, IDC_LEG_PAGEUP);
	rt.top = rect.top + rect.Height()*m_tWidth;
	m_btnPageDown.Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, rt, this, IDC_LEG_PAGEDOWN);
	
	return TRUE;
}

void CLegendDlg::OnClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	POSITION pos = m_lstContent.GetFirstSelectedItemPosition();
	if(pos != NULL)
	{
	}
}

void CLegendDlg::OnButtonClose()
{
	m_lstContent.ShowWindow(SW_HIDE);
	m_btnClose.ShowWindow(SW_HIDE);
	m_btnPageUp.ShowWindow(SW_HIDE);
	m_btnPageDown.ShowWindow(SW_HIDE);
	ShowWindow(SW_HIDE);
}

void CLegendDlg::OnButtonPageUp()
{
}

void CLegendDlg::OnButtonPageDown()
{
}
