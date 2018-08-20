// CEComboBox_.cpp : implementation file
//

#include "stdafx.h"
#include "../resource.h"
#include "CEComboBox_.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCEComboBox_

CCEComboBox_::CCEComboBox_()
{
}

CCEComboBox_::~CCEComboBox_()
{
}

BOOL CCEComboBox_::Create(int style, CRect &rect, CWnd *pParentWnd, int nCtrlID)
{
	CRect rt = rect;
	rt = CRect(0, 0, rt.Width()-1, rt.Height()-1); 
	CRect rt1 = rt;
	rt.DeflateRect(1,1);

	CStatic::Create(L"", style, CRect(rect.left, rect.top, rect.right, rect.top+20), pParentWnd);
	
	m_Static.Create(L"", WS_CHILD|WS_VISIBLE, CRect(rt1.left, rt1.top, rt1.right-20, rt1.top+20), this);
	m_Static.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));
	m_Static.SetUnderLine(TRUE);
	m_Btn.Create(L"", WS_CHILD|WS_VISIBLE, CRect(rt1.right-20, rt1.top, rt1.right+1, rt1.top+20), this, IDC_COMBO_BTN);
	m_Btn.SetIcon(IDI_ICON_COMBOBOX, CSize(16, 16));
	m_Combo.Create(WS_VISIBLE|WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST, rt, this, IDC_COMBO_COMBO);
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
	CPaintDC dc(this); // device context for painting
	
	CString s;
	m_Combo.GetWindowText(s);
	m_Static.SetWindowText(s);
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
//	Invalidate(TRUE);
}
