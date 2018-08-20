// CELineEdit.cpp : implementation file
//

#include "stdafx.h"
#include "CELineEdit.h"
#include	"../MultimediaPhone.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCELineEdit

CCELineEdit::CCELineEdit()
{
	m_nFontHeight = 18;
	m_isAutoInPut = TRUE;
}

CCELineEdit::~CCELineEdit()
{
}

BEGIN_MESSAGE_MAP(CCELineEdit, CEdit)
	//{{AFX_MSG_MAP(CCELineEdit)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
	//ON_CONTROL_REFLECT(EN_CHANGE, OnChange)

/////////////////////////////////////////////////////////////////////////////
// CCELineEdit message handlers

void CCELineEdit::OnPaint() 
{
	CEdit::OnPaint();
	OnChange();

//	CPaintDC dc(this); // device context for painting

}

void CCELineEdit::OnChange() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
//	Invalidate();
	
	CDC *pdc = GetDC();
	CRect rect;
	this->GetClientRect(&rect);

	for(int i = m_nFontHeight; i < rect.bottom; i += m_nFontHeight)
	{
		int x = rect.left;
		int y = rect.top + i;
		pdc->MoveTo(x, y);
		x = rect.right;
	    pdc->LineTo(x, y);
	}
	ReleaseDC(pdc);
	extern void GlobelSetEditFocus();
	GlobelSetEditFocus();
	
	// TODO: Add your control notification handler code here
}

BOOL CCELineEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL ret = CEdit::Create(dwStyle, rect, pParentWnd, nID);
	CFont font;
	VERIFY(font.CreateFont(
	   m_nFontHeight,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   FW_NORMAL,                 // nWeight
	   TRUE,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   _T("宋体")));                 // lpszFacename
	this->SetFont(&font);
	CEdit::OnPaint();
	return ret;
}

void CCELineEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CEdit::OnMouseMove(nFlags, point);
	if(nFlags & MK_LBUTTON)
	{
		//OnChange();
		SetTimer(0xFF00, 3, NULL);
		//Invalidate();
	}
}

void CCELineEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CEdit::OnLButtonDown(nFlags, point);
	//OnChange();
	//Invalidate();
	SetTimer(0xFF00, 3, NULL);

	if(m_isAutoInPut)
	{
		CRect rt;
		GetWindowRect(&rt);
//		ScreenToClient(&rt);
		theApp.inputDlg_->SetOwnerEdit(this);
		if(rt.top > VOIPINPUT_LCD_HEIGHT/2+1)
		{
			theApp.inputDlg_->MoveTo(TOP_INPUT);
		}
		else
		{
			theApp.inputDlg_->MoveTo(BOTTOM_INPUT);
		}

		if(!theApp.inputDlg_->IsWindowVisible())  //show
		{
			theApp.inputDlg_->show();
		}
	}
}

void CCELineEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CEdit::OnLButtonUp(nFlags, point);
	//OnChange();
	//Invalidate();
	SetTimer(0xFF00, 3, NULL);
}

void CCELineEdit::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 0xFF00)
	{
		KillTimer(0xFF00);
		OnChange();
	}
    CEdit::OnTimer(nIDEvent);
}

//bug ,当input显示的时候，点击edit，edit光标消失，不显示
//
void GlobelSetEditFocus()
{
	if(theApp.inputDlg_->m_pOwnerEdit)
		theApp.inputDlg_->m_pOwnerEdit->SetFocus();
}