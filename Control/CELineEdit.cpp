// CELineEdit.cpp : implementation file
//

#include "stdafx.h"
#include "CELineEdit.h"
#include	"../MultimediaPhone.h"
#include "../Data/SkinStyle.h"
#include "sip.h"
#include "Data/LanguageResource.h"

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
	m_backRGB = RGB(255, 255, 255);
}

CCELineEdit::~CCELineEdit()
{
}

HBRUSH CCELineEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	pDC->SetBkMode(TRANSPARENT);
	m_brBk.DeleteObject();
	m_brBk.CreateSolidBrush(Data::g_editBackRGB[Data::g_skinstyle]);
	return (HBRUSH)m_brBk;   
  
   // TODO: Return a non-NULL brush if the parent's handler should not be called
   //return NULL;
}

BEGIN_MESSAGE_MAP(CCELineEdit, CEdit)
	//{{AFX_MSG_MAP(CCELineEdit)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_KEYDOWN()
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
	CPen pen(PS_SOLID, 1, Data::g_editLineRGB[Data::g_skinstyle]);
	CPen *old = pdc->SelectObject(&pen);

	for(int i = m_nFontHeight; i < rect.bottom; i += m_nFontHeight)
	{
		int x = rect.left;
		int y = rect.top + i;
		pdc->MoveTo(x, y);
		x = rect.right;
	    pdc->LineTo(x, y);
	}
	pdc->SelectObject(old);
	ReleaseDC(pdc);
	///zzw100823////////////////////////
//	extern void GlobelSetEditFocus();
//	GlobelSetEditFocus();
	
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
		if (gLangItem == Data::lEnglish)
		{
			SipShowIM(SIPF_ON);
		}
		else
		{
			CRect rt;
			GetWindowRect(&rt);
			rt.top += point.y;
	//		ClientToScreen(&rt);
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
}

void CCELineEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CEdit::OnLButtonUp(nFlags, point);
	//OnChange();
	//Invalidate();
	SetTimer(0xFF00, 3, NULL);
}
void CCELineEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	
	if((nChar >= '0' && nChar <= '9') || nChar == '*' || nChar == '#')
	{
		char txt[2] = {0};
		txt[0] = nChar;
		CString s = txt;
		ReplaceSel(s);
	}
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
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



/////////////////////////////////////////////////////////////////////////////
// CCERectEdit

CCERectEdit::CCERectEdit()
{
//	m_brBk.CreateSolidBrush(RGB(200, 206, 238));
	m_brBk.CreateSolidBrush(Data::g_editBackRGB[Data::g_skinstyle]);
	m_isAutoInPut = TRUE;
	m_bDigital=FALSE;
}

CCERectEdit::~CCERectEdit()
{
}

BOOL CCERectEdit::Create(DWORD dwStyle, CRect &rect, CWnd *pParentWnd, int nCtrlID)
{
	BOOL ret = CEdit::Create(dwStyle, rect, pParentWnd, nCtrlID);
	ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);
	return ret;
}

BEGIN_MESSAGE_MAP(CCERectEdit, CEdit)
//{{AFX_MSG_MAP(CCERectEdit)
ON_WM_CTLCOLOR_REFLECT()
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_KEYDOWN()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCERectEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   	if((nChar >= '0' && nChar <= '9') || nChar == '*' || nChar == '#')
	{
		
		char txt[2] = {0};		
		txt[0] = nChar;
		CString s = txt;
		ReplaceSel(s);
	}
	//	CEdit::OnChar(nChar, nRepCnt, nFlags);
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////
// CCERectEdit message handlers

HBRUSH CCERectEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	//	if(nCtlColor == CTLCOLOR_EDIT )
	pDC->SetBkMode(TRANSPARENT);
	m_brBk.DeleteObject();
	m_brBk.CreateSolidBrush(Data::g_editBackRGB[Data::g_skinstyle]);
	return   (HBRUSH)m_brBk;   
   // TODO: Return a non-NULL brush if the parent's handler should not be called
   //return NULL;
}

void CCERectEdit::OnPaint() 
{
//	CPaintDC dc(this); // device context for painting
	CEdit::OnPaint();
	// TODO: Add your message handler code here
	
	// Do not call CEdit::OnPaint() for painting messages
}

void CCERectEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CEdit::OnLButtonDown(nFlags, point);

	if(m_isAutoInPut)
	{
		if (gLangItem == Data::lEnglish)
		{
			SipShowIM(SIPF_ON);
		}
		else
		{
			CRect rt;
			GetWindowRect(&rt);
			if (m_bDigital)
			{
				//	ScreenToClient(&rt);
				theApp.inputDlg_->SetOwnerEdit(this);
				theApp.inputDlg_->SetLimitDiagital(m_bDigital);
				
				//  change by qi 20111025
				//	if(rt.top > VOIPINPUT_LCD_HEIGHT/2+1)
				if(rt.top > 128)
				{
					theApp.inputDlg_->MoveTo(TOP_INPUT);
				}
				else
				{
					theApp.inputDlg_->MoveTo(BOTTOM_INPUT);
				}
				
				//隐藏手写
				if (theApp.handWriteDlg_->IsWindowVisible())
				{
					theApp.handWriteDlg_->hide();
				}
				
				if(rt.left > 200)
				{
					theApp.handWriteDlg_->MoveTo(LEFT);
				}
				else
				{
					theApp.handWriteDlg_->MoveTo(RIGHT);
				}
				//
				
				if(!theApp.inputDlg_->IsWindowVisible()) 
				{
					theApp.inputDlg_->show();   
				}
			}
			else
			{
				//手写区域
				theApp.handWriteDlg_->SetOwnerEdit(this);
				theApp.handWriteDlg_->SetLimitDiagital(m_bDigital);
				
				if(rt.left > 200)
				{
					theApp.handWriteDlg_->MoveTo(LEFT);
				}
				else
				{
					theApp.handWriteDlg_->MoveTo(RIGHT);
				}
				
				if(rt.top > 128)
				{
					theApp.inputDlg_->MoveTo(TOP_INPUT);
				}
				else
				{
					theApp.inputDlg_->MoveTo(BOTTOM_INPUT);
				}
				
				//隐藏普通输入法
				if (theApp.inputDlg_->IsWindowVisible())
				{
					theApp.inputDlg_->hide();
				}
				
				if(!theApp.handWriteDlg_->IsWindowVisible())
				{
					theApp.handWriteDlg_->show();   
				}
				
			}
		}

	}
}

//bug ,当input显示的时候，点击edit，edit光标消失，不显示
//
void GlobelSetEditFocus()
{
	if(theApp.inputDlg_->m_pOwnerEdit)
		theApp.inputDlg_->m_pOwnerEdit->SetFocus();
}

CWnd *GetInputDlg()
{
	return (CWnd *)theApp.inputDlg_;
}

