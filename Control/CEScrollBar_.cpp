// CEScrollBar_.cpp : implementation file
//

#include "stdafx.h"
//#include "testcombox1.h"
#include "CEScrollBar_.h"
#include "../resource.h"
#include "../Data/SkinStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COLORREF SCROLLBAR_DISABLERGB[2] = {RGB(154, 212, 255), RGB(192, 192, 192)};
COLORREF SCROLLBAR_FRAMERGB[2] = {RGB(76,165,229), RGB(128, 128, 128)};

/////////////////////////////////////////////////////////////////////////////
// CCEScrollBar_

CCEScrollBar_::CCEScrollBar_()
{
	m_nMin = 0;
	m_nMax = 0;
	m_nStep = 1;
	m_nPos = 0;
	m_nPage = 0;
	m_BitmapUpID = 0;
	m_BitmapDownID = 0;
	m_BitmapThickID = 0;
	m_nPressed = 0;
	m_bIsActive = FALSE;
	m_nStyle = 0;
	m_Owner = NULL;
}

CCEScrollBar_::~CCEScrollBar_()
{
}


BEGIN_MESSAGE_MAP(CCEScrollBar_, CStatic)
	//{{AFX_MSG_MAP(CCEScrollBar_)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCEScrollBar_ message handlers

void CCEScrollBar_::OnPaint() 
{

	CPaintDC dc(this); // device context for painting
	CRect rt;
	GetClientRect(rt);

	m_RectUpArrow = rt;
	m_RectUpArrow.bottom = SCROLL_ARROW_HEIGHT-1;
	
	m_RectDownArrow = rt;
	m_RectDownArrow.top = rt.bottom - (SCROLL_ARROW_HEIGHT)+1;

	CRect rectall = rt;
	rectall.top = m_RectUpArrow.bottom;  //+1
	rectall.bottom = m_RectDownArrow.top; //-1
	//����
	CBrush br = Data::g_scroolbarFrameRGB[Data::g_skinstyle];
	dc.FrameRect(&rectall, &br);
	//������
	CRect r1 = rectall;
	r1.DeflateRect(1, 1);
	CBrush Bbr = Data::g_scroolbarBackRGB[Data::g_skinstyle];
	dc.FillRect(&r1, &Bbr);	
	//���м���
	r1.left = r1.Width()/2;
	r1.right = r1.left+2;
	CBrush Mbr = Data::g_scroolbarMiddleLineRGB[Data::g_skinstyle];
	dc.FillRect(&r1, &Mbr);	

	if(m_bIsActive)  //active
	{
		int n = 0;
		if((m_nMax-m_nMin-m_nPage) > 0)
			n = m_nPos*(rectall.Height()-SCROLL_THICK_HEIGHT)/(m_nMax-m_nMin-m_nPage);
		else if((m_nMax-m_nMin-m_nPage) == 0)
		{
			if(m_nPos > 0)
			{
				n = (rectall.Height()-SCROLL_THICK_HEIGHT);
				m_nPos = 1;
			}
			else
				m_nPos = 0;
		}	

		m_RectThick = rt;
		m_RectThick.top = rectall.top + n;//-SCROLLBAR_WIDTH/2;
		m_RectThick.bottom = m_RectThick.top + SCROLL_THICK_HEIGHT-1;

		if(m_RectThick.top <= rectall.top)
		{
			m_RectThick.top = rectall.top+1;
			m_RectThick.bottom = m_RectThick.top + SCROLL_THICK_HEIGHT - 1;
		}
		else if(m_RectThick.bottom >= rectall.bottom)
		{
			m_RectThick.bottom = rectall.bottom-1;
			m_RectThick.top = m_RectThick.bottom - SCROLL_THICK_HEIGHT + 1;
		}
	}
	else  // no active
	{
		int n = rectall.Height()/2;
		m_RectThick = rt;
		m_RectThick.top = rectall.top + n-SCROLL_THICK_HEIGHT/2;
		m_RectThick.bottom = m_RectThick.top + SCROLL_THICK_HEIGHT - 1;
		
	}

	m_RectUp = rt;
	m_RectUp.top = rectall.top;
	m_RectUp.bottom = m_RectThick.top;
	
	m_RectDown = rt;
	m_RectDown.top = m_RectThick.bottom;
	m_RectDown.bottom = m_RectDownArrow.top-1;

	//��ͼ��ID��ֵ
	m_BitmapUpID = Data::g_scroolbarUpArrowBmpID[Data::g_skinstyle];
	m_BitmapDownID = Data::g_scroolbarDownArrowBmpID[Data::g_skinstyle];
	m_BitmapThickID = Data::g_scroolbarThickBmpID[Data::g_skinstyle];

	if(m_BitmapUpID > 0)
	{
		CDC memDC;//�ڴ��豸����
		CBitmap cb;//λͼ�����
		cb.LoadBitmap (m_BitmapUpID);//������Դ
		memDC.CreateCompatibleDC (&dc);//�����ڴ��豸������
		CBitmap *oldbitmap=NULL;
		oldbitmap=memDC.SelectObject (&cb);//��λͼѡ�뵱ǰ�豸����
		CRect rect = m_RectUpArrow;
		dc.BitBlt (rect.left, rect.top, rect.Width (), rect.Height(), &memDC, 0, 0, SRCCOPY);//���ڴ泡����ͼ�󿽱�����Ļ��
		memDC.SelectObject (oldbitmap);
	}
	if(m_BitmapDownID > 0)
	{
		CDC memDC;//�ڴ��豸����
		CBitmap cb;//λͼ�����
		cb.LoadBitmap (m_BitmapDownID);//������Դ
		memDC.CreateCompatibleDC (&dc);//�����ڴ��豸������
		CBitmap *oldbitmap=NULL;
		oldbitmap=memDC.SelectObject (&cb);//��λͼѡ�뵱ǰ�豸����
		CRect rect = m_RectDownArrow;
		dc.BitBlt (rect.left, rect.top, rect.Width (), rect.Height(), &memDC, 0, 0, SRCCOPY);//���ڴ泡����ͼ�󿽱�����Ļ��
		memDC.SelectObject (oldbitmap);
	}
	if(m_BitmapThickID > 0)
	{
		CDC memDC;//�ڴ��豸����
		CBitmap cb;//λͼ�����
		cb.LoadBitmap (m_BitmapThickID);//������Դ
		memDC.CreateCompatibleDC (&dc);//�����ڴ��豸������
		CBitmap *oldbitmap=NULL;
		oldbitmap=memDC.SelectObject (&cb);//��λͼѡ�뵱ǰ�豸����
		CRect rect = m_RectThick;
		dc.BitBlt (rect.left+1, rect.top, rect.Width (), rect.Height(), &memDC, 0, 0, SRCCOPY);//���ڴ泡����ͼ�󿽱�����Ļ��
		memDC.SelectObject (oldbitmap);
	}	
	// Do not call CStatic::OnPaint() for painting messages
}

void CCEScrollBar_::SetParam(int style, int nMin, int nMax, int nPage, CWnd *pListCtrl)
{
	if(style == 0)
	{
		m_nStyle = style;	
		
	}
	else
	{
		m_nStyle = 1;
	
	}
	m_nMin = nMin;
	m_nMax = nMax;
//	m_nStep = nStep;
//	m_nPos = 0;
	m_nPage = nPage;
	
	m_Owner = pListCtrl;
	if((m_nMax - m_nMin)<m_nPage)
	{
		m_bIsActive = FALSE;
	}
	else
	{
		m_bIsActive = TRUE;
	}
	ModifyStyle(0, SS_NOTIFY);
}

void CCEScrollBar_::SetRangle(int nMin, int nMax, int nPage)
{
	m_nMin = nMin;
	m_nMax = nMax;
	m_nPage = nPage;

	if((m_nMax - m_nMin)<m_nPage)
	{
		m_bIsActive = FALSE;
	}
	else
	{
		m_bIsActive = TRUE;
	}
}

void CCEScrollBar_::SetPos(int nPos, BOOL redraw)
{
	if(nPos < 0  || nPos > m_nMax)
		return;
	m_nPos = nPos*m_nStep;
	if(redraw)
	{
		Invalidate();
	}
}

void CCEScrollBar_::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bIsActive)
	{
		CStatic::OnLButtonUp(nFlags, point);

		return;
	}

//	CDC *pDC = GetDC();
	//�Ϸ�һ��  PtInRect
	m_nPressed = 0;
	if(m_RectUpArrow.PtInRect(point))
	{
		CDC *pDC = GetDC();
		pDC->InvertRect(m_RectUpArrow);
		ReleaseDC(pDC);
// 		if(m_nPos > 0)
		{
			m_nPos--;
			if(m_Owner)
				m_Owner->SendMessage(WM_VSCROLL, SB_LINEUP, 0  ) ;
			if (m_nPos < 0)//mhq
				m_nPos = 0;
			SetPos(m_nPos);
		}
	}
	else if(m_RectDownArrow.PtInRect(point))	//�·�һ��
	{
		CDC *pDC = GetDC();
		pDC->InvertRect(m_RectDownArrow);
		ReleaseDC(pDC);
		{
			m_nPos++;
			if(m_Owner)
				m_Owner->SendMessage(WM_VSCROLL, SB_LINEDOWN, 0  ) ;
			if (m_nPos > (m_nMax - m_nMin - m_nPage))//mhq
				m_nPos = m_nMax - m_nMin - m_nPage;
			SetPos(m_nPos);
		}
	}
	else if(m_RectUp.PtInRect(point)) //�Ϸ�һҳ
	{
		CDC *pDC = GetDC();
		pDC->InvertRect(m_RectUp);
		ReleaseDC(pDC);
		if(m_nPos > 0)
		{
			m_nPos -= m_nPage;
			if(m_nPos < 0)
				m_nPos = 0;
		//	SetPos(m_nPos);
			if(m_Owner)
			{
			
				m_Owner->SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEUP,0),NULL);
		
			}
			SetPos(m_nPos);
		
		}
	}
	else if(m_RectDown.PtInRect(point)) //�·�һҳ
	{
		CDC *pDC = GetDC();
		pDC->InvertRect(m_RectDown);
		ReleaseDC(pDC);

		if(m_nPos < (m_nMax - m_nMin - m_nPage))  //mhq
		{
			m_nPos += m_nPage;

			if(m_nPos > (m_nMax - m_nMin - m_nPage))
				m_nPos = m_nMax - m_nMin - m_nPage; //mhq
			if(m_Owner)
			{
		
				m_Owner->SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEDOWN,0),NULL);
			}
			SetPos(m_nPos);
			
		}
	}
	CStatic::OnLButtonUp(nFlags, point);
}

void CCEScrollBar_::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bIsActive)
	{
		CStatic::OnLButtonDown(nFlags, point);
		return;
	}

	CDC *pDC = GetDC();
	if(m_RectUpArrow.PtInRect(point)) //�Ϸ�һ��
	{
		m_nPressed = 1;
		pDC->InvertRect(m_RectUpArrow);
	}
	else if(m_RectDownArrow.PtInRect(point))	//�·�һ��
	{
		m_nPressed = 2;
		pDC->InvertRect(m_RectDownArrow);		
	}
	else if(m_RectUp.PtInRect(point)) //�Ϸ�һҳ
	{
		m_nPressed = 3;
		pDC->InvertRect(m_RectUp);
	}
	else if(m_RectDown.PtInRect(point)) //�·�һҳ
	{
		m_nPressed = 4;
		pDC->InvertRect(m_RectDown);
	}
	else if(m_RectThick.PtInRect(point))  // Thick
	{
		m_nPressed = 5;
	}
	ReleaseDC(pDC);
//	Invalidate();
	CStatic::OnLButtonDown(nFlags, point);
}

void CCEScrollBar_::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bIsActive)
	{
		CStatic::OnMouseMove(nFlags, point);
		return;
	}

	if ((GetCapture() != this)) 
	{
		SetCapture();
		//Invalidate();
	}
	else
	{
		CRect rc;
		GetClientRect(&rc);
		if (!rc.PtInRect(point))
		{
			if(m_nPressed == 1)
			{	
				CDC *pDC = GetDC();
				pDC->InvertRect(m_RectUpArrow);
				ReleaseDC(pDC);
			}
			else if(m_nPressed == 2)
			{
				CDC *pDC = GetDC();
				pDC->InvertRect(m_RectDownArrow);
				ReleaseDC(pDC);
			}
			else if(m_nPressed == 3)
			{
				CDC *pDC = GetDC();
				pDC->InvertRect(m_RectUp);
				ReleaseDC(pDC);
			}
			else if(m_nPressed ==4)
			{
				CDC *pDC = GetDC();
				pDC->InvertRect(m_RectDown);
				ReleaseDC(pDC);
			}
			ReleaseCapture();
			m_nPressed = 0;
		}
    }

	if(m_nPressed == 5)	//Thick
	{
		static CPoint oldpoint = point; 
		if(abs(point.y - oldpoint.y) >= 10 )
		{
			int style = 0;
			if(point.y > oldpoint.y)
			{
				style = SB_PAGEDOWN;
				m_nPos += m_nPage;
				if(m_nPos > (m_nMax - m_nMin - m_nPage))//mhq
					m_nPos = m_nMax - m_nMin - m_nPage; 
			}
			else
			{
				style = SB_PAGEUP;
				m_nPos -= m_nPage;
				if(m_nPos < 0)   //mhq
					m_nPos = 0;
			}
			oldpoint = point;
			SetPos(m_nPos);
			if(m_Owner)
				m_Owner->SendMessage(WM_VSCROLL, MAKELONG(style,0), 0 ) ;

		}
	}
 
	CStatic::OnMouseMove(nFlags, point);
}
