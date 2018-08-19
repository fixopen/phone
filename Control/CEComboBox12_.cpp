// CEComboBox_.cpp : implementation file
//

#include "stdafx.h"
#include "../resource.h"
#include "CEComboBox12_.h"
#include "../Data/SkinStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCEComboBox12_

CCEComboBox12_::CCEComboBox12_()
{
	m_backRGB = m_backRGB = RGB(255, 255, 255);/*Data::g_comboxBackRGB[Data::g_skinstyle];*/
	m_nFontHeight = 28;
}

CCEComboBox12_::~CCEComboBox12_()
{
}

BOOL CCEComboBox12_::CreateEx(int style, CRect &rect, CWnd *pParentWnd, int nCtrlID, int fontSize, int btnWidth, int btnHeight, int bitmapType)
{
	CRect rt = rect;
	rt = CRect(0, 0, rt.Width(), rt.Height()); 

	CRect rt1 = CRect(rt.left, rt.top, rt.right, rt.top+btnHeight);
	CStatic::Create(L"", style, CRect(rect.left, rect.top, rect.right, rect.top+btnHeight), pParentWnd);
	m_Static.Create(L"", WS_CHILD, CRect(rt1.left, rt1.top, rt1.right-btnWidth+1, rt1.bottom), this);
	if(bitmapType > 0)
	{
		m_Btn.Create(L"", IDB_BITMAP_COMBOX1_FORE, IDB_BITMAP_COMBOX1_BACK, WS_CHILD|WS_VISIBLE, CRect(rt1.right-btnWidth, rt1.top, rt1.right, rt1.bottom), this, IDC_COMBO_BTN);
	}
	else if( bitmapType == -1)
	{
		m_Btn.Create(L"", IDB_BITMAP_CONTACTNEW, IDB_BITMAP_CONTACTNEW1, WS_CHILD|WS_VISIBLE, CRect(rt1.right-btnWidth, rt1.top+1, rt1.right, rt1.bottom+1), this, IDC_COMBO_BTN);
	}
	else
	{
		m_Btn.Create(L"", IDB_BITMAP_COMBOX_FORE, IDB_BITMAP_COMBOX_BACK, WS_CHILD|WS_VISIBLE, CRect(rt1.right-btnWidth, rt1.top, rt1.right, rt1.bottom), this, IDC_COMBO_BTN);
	}
	m_pCombo = new CComboBox();
	m_rt = rt;
	m_pCombo->Create(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST|WS_TABSTOP, rt, this, IDC_COMBO_COMBO);
	m_nCtrlID = nCtrlID;
	VERIFY(m_font.CreateFont(
		fontSize,                  // nHeight
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
		_T("ËÎÌו")));                 // lpszFacename
	SetFont(&m_font);
	m_pCombo->SetFont(&m_font);
	
	return TRUE;
}

BOOL CCEComboBox12_::Create(int style, CRect &rect, CWnd *pParentWnd, int nCtrlID)
{
	CRect rt = rect;
	rt = CRect(0, 0, rt.Width(), rt.Height()); 
	//	CRect rt1 = rt;
	//rt.DeflateRect(1,1);

	/*
	CStatic::Create(L"", style, CRect(rect.left, rect.top, rect.right, rect.top+20), pParentWnd);
	m_Static.Create(L"", WS_CHILD, CRect(rt1.left, rt1.top, rt1.right-20, rt1.top+20), this);
	//m_Static.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));
	//m_Static.SetUnderLine(TRUE);   //lxz 2007 11 05
	m_Btn.Create(L"", Data::g_comboxBMPID[0][Data::g_skinstyle], Data::g_comboxBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(rt1.right-20, rt1.top+4, rt1.right-2, rt1.top+16), this, IDC_COMBO_BTN);
	m_pCombo->Create(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST, rt, this, IDC_COMBO_COMBO);
	*/

	CRect rt1 = CRect(rt.left, rt.top, rt.right, rt.top+40);
	CStatic::Create(L"", style, CRect(rect.left, rect.top, rect.right, rect.top+40), pParentWnd);
	m_Static.Create(L"", WS_CHILD, CRect(rt1.left, rt1.top, rt1.right-57, rt1.bottom), this);
	m_Btn.Create(L"", Data::g_comboxBMPID[0][Data::g_skinstyle], Data::g_comboxBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(rt1.right-58, rt1.top, rt1.right, rt1.bottom), this, IDC_COMBO_BTN);
	m_pCombo = new CComboBox();
	m_rt = rt;
	m_pCombo->Create(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST, rt, this, IDC_COMBO_COMBO);
//	m_pCombo->SetDroppedWidth(100);
	m_nCtrlID = nCtrlID;
	VERIFY(m_font.CreateFont(
		m_nFontHeight,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapementu 
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
		_T("ËÎÌו")));                 // lpszFacename
	SetFont(&m_font);

	m_pCombo->SetFont(&m_font);
	/*	
	m_Static.SetFont(&m_font);
	*/
	return TRUE;
}

BEGIN_MESSAGE_MAP(CCEComboBox12_, CStatic)
	//{{AFX_MSG_MAP(CCEComboBox12_)
	ON_BN_CLICKED(IDC_COMBO_BTN, OnButton1)
	ON_CBN_SELCHANGE(IDC_COMBO_COMBO, OnSelchangeCombo1)
	ON_CBN_SELENDCANCEL(IDC_COMBO_COMBO, OnSelendcancelCombo1)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCEComboBox12_ message handlers

void CCEComboBox12_::OnPaint() 
{
	// lxz 2007 11 05
	ReCreateComm();

	CPaintDC dc(this);
	/*	CString s;
	m_pCombo->GetWindowText(s);
	m_Static.SetWindowText(s);
	*/

	CFont *pFont = dc.SelectObject(&m_font);
	
	CString s;
	m_pCombo->GetWindowText(s);

	//m_Static.SetWindowText(s);

	CRect rect, rect1;
	GetClientRect(&rect);
	m_Static.GetClientRect(&rect1);
	dc.FillSolidRect(rect1, Data::g_comboxBackRGB[Data::g_skinstyle]);
	CRect rt = CRect(rect.left, rect.top, rect.right+1, rect.top+1);
	dc.FillSolidRect(rt, RGB(132, 130, 132));
	rt = CRect(rect.left, rect.top, rect.left+1, rect.bottom+1);
	dc.FillSolidRect(rt, RGB(132, 130, 132));

	rt = CRect(rect.left, rect.bottom, rect.right+1, rect.bottom+1);
	dc.FillSolidRect(rt, RGB(255, 255, 255));
	rt = CRect(rect.right, rect.top, rect.right+1, rect.bottom+1);
	dc.FillSolidRect(rt, RGB(255, 255, 255));

	rect.DeflateRect(1, 1);
	rt = CRect(rect.left, rect.top, rect.right+1, rect.top+1);
	dc.FillSolidRect(rt, RGB(0, 0, 0));
	rt = CRect(rect.left, rect.top, rect.left+1, rect.bottom+1);
	dc.FillSolidRect(rt, RGB(0, 0, 0));
	
	rt = CRect(rect.left, rect.bottom, rect.right+1, rect.bottom+1);
	dc.FillSolidRect(rt, RGB(222, 223, 222));
	rt = CRect(rect.right, rect.top, rect.right+1, rect.bottom+1);
	dc.FillSolidRect(rt, RGB(222, 223, 222));

	rect1.DeflateRect(2, 2);
	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(s, &rect1, DT_LEFT);
	dc.SelectObject(pFont);


	// TODO: Add your message handler code here
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CCEComboBox12_::OnButton1()
{
//	::ShowCursor(FALSE);
	m_pCombo->ShowDropDown(TRUE);
//	::SetCursorPos(25, 25);
	//Invalidate(TRUE);
}

void CCEComboBox12_::OnSelchangeCombo1()
{
	CString s;
	m_pCombo->GetWindowText(s);
	m_Static.SetWindowText(s);
	Invalidate();
	if(GetParent())
	{
		GetParent()->SendMessage(WM_COMMBOX_CLICKED, m_nCtrlID, m_nCtrlID);
//		m_pCombo->SendMessage(WM_ACTIVATE, 0, 0);
//		::ShowCursor(TRUE);
//		m_pCombo->EndWaitCursor();
	}
}

void CCEComboBox12_::OnSelendcancelCombo1()
{
	Invalidate();
}

void CCEComboBox12_::SetWindowText_(CString s)
{
	int nn = -1;
	int ncount = m_pCombo->GetCount();
	for(int i = 0; i < ncount; i++)
	{
		CString rString;
		m_pCombo->GetLBText(i, rString);
		if(rString == s)
		{
			m_pCombo->SetCurSel(i);
			m_Static.SetWindowText(s);
		}
	}
}

void CCEComboBox12_ ::GetWindowText(CString &string)
{
	m_pCombo->GetWindowText(string);
	//m_Static.GetWindowText(string);
}

void CCEComboBox12_:: ReCreateComm()
{
	if(m_pCombo)
	{
		CComboBox *m_pTempComm = new CComboBox();
	//	m_pCombo->GetClientRect(&rt);
		m_pTempComm->Create(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST, m_rt, this, IDC_COMBO_COMBO);
		m_pTempComm->SetFont(&m_font);
		
		int ncount = m_pCombo->GetCount();
		for(int i = 0; i < ncount; i++)
		{
			CString rString;
			m_pCombo->GetLBText(i, rString);
			m_pTempComm->AddString(rString);
			
			//add by qi 20100901
			m_pTempComm->SetItemData(i,m_pCombo->GetItemData(i));
			//

			if(i == m_pCombo->GetCurSel())
				m_pTempComm->SetWindowText(rString);
		}
		int cursel = m_pCombo->GetCurSel();
		m_pTempComm->SetCurSel(m_pCombo->GetCurSel());
		delete m_pCombo;
		m_pCombo = m_pTempComm;
	}
}