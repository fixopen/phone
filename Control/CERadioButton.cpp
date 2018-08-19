// CERadioButton.cpp : implementation file
//

#include "stdafx.h"
#include "../resource.h"
#include "CERadioButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCERadioButton
#define RADIOBUTTONSIZE    32

CCERadioButton::CCERadioButton()
{
	m_nCheck = 0;
	m_txtRGB = RGB(0, 0, 0);
	m_bkRGB = RGB(192, 192, 192);
	m_nGroup = 0;
//	nCheck = 0;
}

CCERadioButton::~CCERadioButton()
{
}

void CCERadioButton::SetGroupButton(CButton *pButton[], int n)
{
	if(n <= 12)
		m_nGroup = n;
	else 
		m_nGroup = 12;
	for(int i = 0; i < m_nGroup; i++)
		m_GroupButton[i] = (CCERadioButton *)pButton[i]; 
}

BOOL CCERadioButton::Create(LPCTSTR lpszCaption, DWORD dwStyle,
				const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL ret = CButton::Create(lpszCaption, dwStyle, rect, pParentWnd, nID);
	VERIFY(m_font.CreateFont(
		28,                        // nHeight
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
		_T("����")));                 // lpszFacename
	this->SetFont(&m_font);
	return ret;
}

BEGIN_MESSAGE_MAP(CCERadioButton, CButton)
	//{{AFX_MSG_MAP(CCERadioButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCERadioButton message handlers

extern void TransparentBlt2( HDC hdcDest,      // Ŀ��DC
							int nXOriginDest,   // Ŀ��Xƫ��
							int nYOriginDest,   // Ŀ��Yƫ��
							int nWidthDest,     // Ŀ�����
							int nHeightDest,    // Ŀ��߶�
							HDC hdcSrc,         // ԴDC
							int nXOriginSrc,    // ԴX���
							int nYOriginSrc,    // ԴY���
							int nWidthSrc,      // Դ����
							int nHeightSrc,     // Դ�߶�
							UINT crTransparent  // ͸��ɫ,COLORREF����
					 );

void CCERadioButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
//	CButton::OnPaint();

	CRect rt;
	GetClientRect(&rt);
	dc.FillSolidRect(&rt, m_bkRGB);

	CDC memDC;//�ڴ��豸����
	CBitmap cb;//λͼ�����
	int iNo;
	if(GetCheck_())
		iNo	= IDB_BITMAP_RADIO1;
	else
		iNo	= IDB_BITMAP_RADIO2;
	cb.LoadBitmap (iNo);//������Դ
	memDC.CreateCompatibleDC (&dc);//�����ڴ��豸������
	CBitmap *oldbitmap=NULL;
	oldbitmap=memDC.SelectObject (&cb);//��λͼѡ�뵱ǰ�豸����
	//dc.BitBlt (rt.left, rt.top+2, 16, 16, &memDC, 0, 0, SRCCOPY);//���ڴ泡����ͼ�󿽱�����Ļ��
	TransparentBlt2(dc.m_hDC, rt.left, rt.top+2, RADIOBUTTONSIZE, RADIOBUTTONSIZE, memDC.m_hDC, 0, 0, RADIOBUTTONSIZE, RADIOBUTTONSIZE, RGB(255, 0, 255));
	memDC.SelectObject (oldbitmap);

	CString s;
	GetWindowText(s);
	dc.SetBkMode(TRANSPARENT);
	CFont *pOldFont = dc.SelectObject(&m_font);
	dc.SetTextColor(m_txtRGB);
	dc.DrawText(s, CRect(rt.left+RADIOBUTTONSIZE+6, rt.top+2, rt.right, rt.bottom), 0);
	dc.SelectObject(pOldFont);

	// TODO: Add your message handler code here
	
	// Do not call CButton::OnPaint() for painting messages
}

void CCERadioButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
//	CButton::OnLButtonDown(nFlags, point);
	CRect rect ;
	GetClientRect(&rect);
	HDC pdc = ::GetDC(m_hWnd);
	rect.left += (RADIOBUTTONSIZE+6);
	::InvertRect(pdc, &rect);
	::ReleaseDC(m_hWnd, pdc);

	//Invalidate();
}

void CCERadioButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
//	CButton::OnLButtonUp(nFlags, point);
	if(m_nGroup < 2)
		return;
	for(int i = 0; i < m_nGroup; i++)
	{
		if(m_GroupButton[i] == this)
		{
			BOOL ret = 	m_GroupButton[i]->GetCheck_();
			m_GroupButton[i]->SetCheck_(1);
			m_GroupButton[i]->Invalidate();
			if(!ret)
			{
				CWnd *pWnd = GetParent();
				if(pWnd)
				{
					pWnd->SendMessage(WM_BTNST_CLICK, GetDlgCtrlID(), 0);
				}
			}
		}
		else if(m_GroupButton[i]->GetCheck_())
		{
			m_GroupButton[i]->SetCheck_(0);
			m_GroupButton[i]->Invalidate();
		}	
	}
}

BOOL CCERadioButton::GetCheck_()
{
	return m_nCheck = m_nCheck?1:0;
}

void CCERadioButton::SetCheck_(BOOL nCheck)
{
	m_nCheck = nCheck;
	SetCheck(nCheck);
}

void CCERadioButton::PreSubclassWindow()
{
	//DWORD style = GetWindowLong();
	ModifyStyle(0x0000001FL, BS_OWNERDRAW, SWP_FRAMECHANGED);
	CButton::PreSubclassWindow();
}

void CCERadioButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{

}