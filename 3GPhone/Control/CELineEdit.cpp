// CELineEdit.cpp : implementation file
//

#include "stdafx.h"
#include "CELineEdit.h"
//#include "../Data/SkinStyle.h"
#include <sipapi.h>

namespace Control {
    /////////////////////////////////////////////////////////////////////////////
    // CCELineEdit

    CCELineEdit::CCELineEdit()
    {
        m_nFontHeight = 28;
        m_isAutoInPut = TRUE;
        m_backRGB = RGB(255, 255, 255);
    }

    CCELineEdit::~CCELineEdit()
    {
    }

    HBRUSH CCELineEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
    {
        //// TODO: Change any attributes of the DC here
        //pDC->SetBkMode(TRANSPARENT);
        //m_brBk.DeleteObject();
        //m_brBk.CreateSolidBrush(Data::g_editBackRGB[Data::g_skinstyle]);
        //return (HBRUSH)m_brBk;   

        //// TODO: Return a non-NULL brush if the parent's handler should not be called
        return NULL;
    }

    BEGIN_MESSAGE_MAP(CCELineEdit, CEdit)
        //{{AFX_MSG_MAP(CCELineEdit)
        ON_WM_TIMER()
        ON_WM_PAINT()
        ON_WM_MOUSEMOVE()
        ON_WM_LBUTTONDOWN()
        ON_WM_LBUTTONUP()
        ON_WM_CTLCOLOR_REFLECT()
        ON_WM_CHAR()
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
        //// TODO: If this is a RICHEDIT control, the control will not
        //// send this notification unless you override the CEdit::OnInitDialog()
        //// function and call CRichEditCtrl().SetEventMask()
        //// with the ENM_CHANGE flag ORed into the mask.
        ////	Invalidate();

        //CDC *pdc = GetDC();
        //CRect rect;
        //this->GetClientRect(&rect);
        //CPen pen(PS_SOLID, 1, Data::g_editLineRGB[Data::g_skinstyle]);
        //CPen *old = pdc->SelectObject(&pen);

        //for(int i = m_nFontHeight; i < rect.bottom; i += m_nFontHeight)
        //{
        //    int x = rect.left;
        //    int y = rect.top + i;
        //    pdc->MoveTo(x, y);
        //    x = rect.right;
        //    pdc->LineTo(x, y);
        //}
        //pdc->SelectObject(old);
        //ReleaseDC(pdc);
        //// 	extern void GlobelSetEditFocus(); //mhq 新建名片切换到桌面
        //// 	GlobelSetEditFocus();

        //// TODO: Add your control notification handler code here
    }

    BOOL CCELineEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
    {
        // TODO: Add your specialized code here and/or call the base class
        BOOL ret = CEdit::Create(dwStyle, rect, pParentWnd, nID);
        VERIFY(m_font.CreateFont(
            m_nFontHeight,                        // nHeight
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
            _T("宋体")));                 // lpszFacename
        this->SetFont(&m_font);
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
            BOOL fRes = FALSE;
            //SIPINFO si;
            //memset (&si, 0, sizeof (SIPINFO));
            //si.cbSize = sizeof(SIPINFO);  //  sizeof (si)= 48
            //if (SipGetInfo(&si))
            if (false)
            {
                //si.fdwFlags &= ~SIPF_ON;
                //si.fdwFlags |=SIPF_DOCKED;

                //if ((rt.top + 40) > (VOIPINPUT_LCD_HEIGHT / 2 + 1))
                {
                    //si.rcSipRect.top = -26;
                }
                //else
                {
                    //si.rcSipRect.top=201; //at least >26 	
                }

                //si.rcSipRect.bottom=si.rcSipRect.top+228; 
                //si.rcSipRect.left = -2;
                //si.rcSipRect.right = 800;

                //fRes = SipSetInfo(&si); 
                //SipShowIM(SIPF_OFF);
                //SipShowIM(SIPF_ON);
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

    void CCELineEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        if(nChar == '\r' || nChar == L'\n')
        {
            Invalidate();
        }
        if (nChar == WM_COPY || nChar == WM_PASTE)
        {
            SendMessage(nChar, 0, 0);
        }
        else
        {
            Invalidate();
            CEdit::OnChar(nChar, nRepCnt, nFlags);
        }
    }


    /////////////////////////////////////////////////////////////////////////////
    // CCERectEdit

    CCERectEdit::CCERectEdit()
    {
        ////	m_brBk.CreateSolidBrush(RGB(200, 206, 238));
        //m_brBk.CreateSolidBrush(Data::g_editBackRGB[Data::g_skinstyle]);
        //m_isAutoInPut = TRUE;
        //m_bDigital = FALSE;
    }

    CCERectEdit::~CCERectEdit()
    {
    }

    BOOL CCERectEdit::Create(DWORD dwStyle, CRect &rect, CWnd *pParentWnd, int nCtrlID, int nFontHeight )
    {
        BOOL ret = CEdit::Create(dwStyle, rect, pParentWnd, nCtrlID);
        ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);
        VERIFY(m_font.CreateFont(
            nFontHeight,                        // nHeight
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
            _T("宋体")));                 // lpszFacename
        this->SetFont(&m_font);
        return ret;
    }

    BEGIN_MESSAGE_MAP(CCERectEdit, CEdit)
        //{{AFX_MSG_MAP(CCERectEdit)
        ON_WM_CTLCOLOR_REFLECT()
        ON_WM_PAINT()
        ON_WM_LBUTTONDOWN()
        ON_WM_CHAR()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CCERectEdit message handlers

    HBRUSH CCERectEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
    {
        //// TODO: Change any attributes of the DC here
        ////	if(nCtlColor == CTLCOLOR_EDIT )
        //pDC->SetBkMode(TRANSPARENT);
        //m_brBk.DeleteObject();
        //m_brBk.CreateSolidBrush(Data::g_editBackRGB[Data::g_skinstyle]);
        ////	return   (HBRUSH)m_brBk;   
        return NULL;
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
            CRect rt;
            GetWindowRect(&rt);
            //		ScreenToClient(&rt);

            BOOL fRes = FALSE; 
            //SIPINFO si; 
            //memset (&si, 0, sizeof (SIPINFO)); 
            //si.cbSize = sizeof(SIPINFO);  //  sizeof (si)= 48 
            //if (SipGetInfo(&si))
            if (false)
            { 
                //si.fdwFlags &= ~SIPF_ON; 
                //si.fdwFlags |=SIPF_DOCKED; 

                //if ((rt.top+40) > (VOIPINPUT_LCD_HEIGHT/2+1))
                {
                    //si.rcSipRect.top = -26;
                }
                //else
                {
                    //si.rcSipRect.top=201; //at least >26 	
                }

                //si.rcSipRect.bottom=si.rcSipRect.top+228; 
                //si.rcSipRect.left = -2;
                //si.rcSipRect.right = 800;

                //fRes = SipSetInfo(&si); 
                //SipShowIM(SIPF_OFF);
                //SipShowIM(SIPF_ON);
            }
        }
    }

    void CCERectEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        //if(nChar == L'\r' || nChar == L'\n')
        //{
        //    GetParent()->SendMessage(WM_EDITENTER, 0, 0);
        //}
        //if (nChar == WM_COPY || nChar == WM_PASTE)
        //{
        //    SendMessage(nChar, 0, 0);
        //}
        //if(m_bDigital)
        //{
        //    if((nChar >= L'0' && nChar <= L'9') || nChar == 0x08 /*|| nChar == 0x25 || nChar == 0x27*/)
        //        CEdit::OnChar(nChar, nRepCnt, nFlags);
        //}
        //else
        //{
        //    CEdit::OnChar(nChar, nRepCnt, nFlags);
        //}
    }
}
