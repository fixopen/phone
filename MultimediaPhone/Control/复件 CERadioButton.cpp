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

namespace Control {
    /////////////////////////////////////////////////////////////////////////////
    // CCERadioButton
#define RADIOBUTTONSIZE    32

    CCERadioButton::CCERadioButton()
    {
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
        m_nGroup = n;
        for(int i = 0; i < m_nGroup; i++)
            m_GroupButton[i] = dynamic_cast<Control::CCERadioButton*>(pButton[i]);
    }

    BOOL CCERadioButton::Create(LPCTSTR lpszCaption, DWORD dwStyle,
        const RECT& rect, CWnd* pParentWnd, UINT nID)
    {
        BOOL ret = CButton::Create(lpszCaption, dwStyle|BS_RADIOBUTTON , rect, pParentWnd, nID);
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
            _T("宋体")));                 // lpszFacename
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

    extern void TransparentBlt2( HDC hdcDest,      // 目标DC
    int nXOriginDest,   // 目标X偏移
    int nYOriginDest,   // 目标Y偏移
    int nWidthDest,     // 目标宽度
    int nHeightDest,    // 目标高度
    HDC hdcSrc,         // 源DC
    int nXOriginSrc,    // 源X起点
    int nYOriginSrc,    // 源Y起点
    int nWidthSrc,      // 源宽度
    int nHeightSrc,     // 源高度
    UINT crTransparent  // 透明色,COLORREF类型
    );

    void CCERadioButton::OnPaint() 
    {
        CPaintDC dc(this); // device context for painting
        //	CButton::OnPaint();

        CDC *pdc = GetDC();
        CRect rt;
        GetClientRect(&rt);
        pdc->FillSolidRect(&rt, m_bkRGB);

        CDC memDC;//内存设备场景
        CBitmap cb;//位图类对象
        int iNo;
        if(GetCheck())
            iNo	= IDB_BITMAP_RADIO1;
        else
            iNo	= IDB_BITMAP_RADIO2;
        cb.LoadBitmap (iNo);//载入资源
        memDC.CreateCompatibleDC (pdc);//创建内存设备上下文
        CBitmap *oldbitmap=NULL;
        oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
        //pdc->BitBlt (rt.left, rt.top+2, 16, 16, &memDC, 0, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
        TransparentBlt2(pdc->m_hDC, rt.left, rt.top+2, RADIOBUTTONSIZE, RADIOBUTTONSIZE, memDC.m_hDC, 0, 0, RADIOBUTTONSIZE, RADIOBUTTONSIZE, RGB(255, 0, 255));
        memDC.SelectObject (oldbitmap);

        CString s;
        GetWindowText(s);
        pdc->SetBkMode(TRANSPARENT);
        CFont *pOldFont = pdc->SelectObject(&m_font);
        pdc->SetTextColor(m_txtRGB);
        pdc->DrawText(s, CRect(rt.left+RADIOBUTTONSIZE+6, rt.top+2, rt.right, rt.bottom), 0);
        pdc->SelectObject(pOldFont);
        ReleaseDC(pdc);

        // TODO: Add your message handler code here

        // Do not call CButton::OnPaint() for painting messages
    }

    void CCERadioButton::OnLButtonDown(UINT nFlags, CPoint point) 
    {
        // TODO: Add your message handler code here and/or call default

        //	CButton::OnLButtonDown(nFlags, point);
        CRect rect ;
        GetClientRect(&rect);
        CDC *pdc = GetDC();
        rect.left += (RADIOBUTTONSIZE+6);
        pdc->InvertRect(&rect);
        ReleaseDC(pdc);

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
                m_GroupButton[i]->SetCheck(1);
                m_GroupButton[i]->Invalidate();
            }
            else if(m_GroupButton[i]->GetCheck())
            {
                m_GroupButton[i]->SetCheck(0);
                m_GroupButton[i]->Invalidate();
            }	
        }
    }
    /*
    void CCERadioButton::PreSubclassWindow()
    {
    //DWORD style = GetWindowLong();
    ModifyStyle(0x0000001FL, BS_OWNERDRAW|BS_RADIOBUTTON, SWP_FRAMECHANGED);
    CButton::PreSubclassWindow();
    }

    void CCERadioButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
    {

    }
    */
}
