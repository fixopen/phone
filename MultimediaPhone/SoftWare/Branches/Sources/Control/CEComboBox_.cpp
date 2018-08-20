// CEComboBox_.cpp : implementation file
//

#include "stdafx.h"
#include "../resource.h"
#include "CEComboBox_.h"
#include "../Data/SkinStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCEComboBox_

CCEComboBox_::CCEComboBox_()
{
	m_backRGB = Data::g_comboxBackRGB[Data::g_skinstyle];
}

CCEComboBox_::~CCEComboBox_()
{
}

BOOL CCEComboBox_::Create(int style, CRect &rect, CWnd *pParentWnd, int nCtrlID)
{
	CRect rt = rect;
	rt = CRect(0, 0, rt.Width()-1, rt.Height()-1); 
//	CRect rt1 = rt;
	rt.DeflateRect(1,1);

	/*
	CStatic::Create(L"", style, CRect(rect.left, rect.top, rect.right, rect.top+20), pParentWnd);
	m_Static.Create(L"", WS_CHILD, CRect(rt1.left, rt1.top, rt1.right-20, rt1.top+20), this);
	//m_Static.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));
	//m_Static.SetUnderLine(TRUE);   //lxz 2007 11 05
	m_Btn.Create(L"", Data::g_comboxBMPID[0][Data::g_skinstyle], Data::g_comboxBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(rt1.right-20, rt1.top+4, rt1.right-2, rt1.top+16), this, IDC_COMBO_BTN);
	m_Combo.Create(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST, rt, this, IDC_COMBO_COMBO);
	*/

	CRect rt1 = CRect(rt.left, rt.top, rt.right, rt.top+18);
	CStatic::Create(L"", style, CRect(rect.left, rect.top, rect.right, rect.top+18), pParentWnd);
	m_Static.Create(L"", WS_CHILD, CRect(rt1.left, rt1.top, rt1.right-14, rt1.bottom), this);
	m_Btn.Create(L"", Data::g_comboxBMPID[0][Data::g_skinstyle], Data::g_comboxBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(rt1.right-15, rt1.top+1, rt1.right, rt1.bottom-2), this, IDC_COMBO_BTN);
	m_Combo.Create(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST, rt, this, IDC_COMBO_COMBO);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CCEComboBox_, CStatic)
	//{{AFX_MSG_MAP(CCEComboBox_)
	ON_BN_CLICKED(IDC_COMBO_BTN, OnButton1)
	ON_CBN_SELCHANGE(IDC_COMBO_COMBO, OnSelchangeCombo1)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCEComboBox_ message handlers

void CCEComboBox_::OnPaint() 
{
	// lxz 2007 11 05
	/*
	CPaintDC dc(this);
	CString s;
	m_Combo.GetWindowText(s);
	m_Static.SetWindowText(s);
	*/

	CStatic::OnPaint();

	CDC *pdc = GetDC();
	
	CString s;
	m_Combo.GetWindowText(s);

	//m_Static.SetWindowText(s);

	CRect rect, rect1;
	GetClientRect(&rect);
	m_Static.GetClientRect(&rect1);
	pdc->FillSolidRect(rect1, Data::g_comboxBackRGB[Data::g_skinstyle]);
	CRect rt = CRect(rect.left, rect.top, rect.right+1, rect.top+1);
	pdc->FillSolidRect(rt, RGB(132, 130, 132));
	rt = CRect(rect.left, rect.top, rect.left+1, rect.bottom+1);
	pdc->FillSolidRect(rt, RGB(132, 130, 132));

	rt = CRect(rect.left, rect.bottom, rect.right+1, rect.bottom+1);
	pdc->FillSolidRect(rt, RGB(255, 255, 255));
	rt = CRect(rect.right, rect.top, rect.right+1, rect.bottom+1);
	pdc->FillSolidRect(rt, RGB(255, 255, 255));

	rect.DeflateRect(1, 1);
	rt = CRect(rect.left, rect.top, rect.right+1, rect.top+1);
	pdc->FillSolidRect(rt, RGB(0, 0, 0));
	rt = CRect(rect.left, rect.top, rect.left+1, rect.bottom+1);
	pdc->FillSolidRect(rt, RGB(0, 0, 0));
	
	rt = CRect(rect.left, rect.bottom, rect.right+1, rect.bottom+1);
	pdc->FillSolidRect(rt, RGB(222, 223, 222));
	rt = CRect(rect.right, rect.top, rect.right+1, rect.bottom+1);
	pdc->FillSolidRect(rt, RGB(222, 223, 222));

	rect1.DeflateRect(2, 2);
	pdc->SetBkMode(TRANSPARENT);
	pdc->DrawText(s, &rect1, DT_LEFT);
	ReleaseDC(pdc);

	// TODO: Add your message handler code here
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CCEComboBox_::OnButton1()
{
	m_Combo.ShowDropDown(TRUE);
	//Invalidate(TRUE);
}

void CCEComboBox_::OnSelchangeCombo1()
{
	CString s;
	m_Combo.GetWindowText(s);
	m_Static.SetWindowText(s);
	Invalidate();
	if(GetParent())
	{
		GetParent()->SendMessage(WM_COMMBOX_CLICKED);
	}
}
