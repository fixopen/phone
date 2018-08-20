// CEProcessBar.cpp : implementation file
//

#include "stdafx.h"
//#include "EditLine.h"
#include "CEProcessBar.h"
#include "../Data/SkinStyle.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace Control {
    /////////////////////////////////////////////////////////////////////////////
    // CCEProcessBar
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
#define    MIDDLEBMP_SIZE		14/2
    CCEProcessBar::CCEProcessBar()
    {
        m_nMin = 0;
        m_nMax = 10;
        m_nStep = 1;
        m_nPos = 0;
        m_BitmapID = 0;
        m_BitmapIDTop = 0;
        m_BitmapIDBottom = 0;
        m_nStyle = 0;		//0.横条画区域  1.横条bitmap  2.竖条画区域	3.竖条画区域 
        m_bSelectedThick = FALSE;
        m_nThinckWidth = MIDDLEBMP_SIZE;
        m_bIsDisable = FALSE;
    }

    CCEProcessBar::~CCEProcessBar()
    {
    }

    BEGIN_MESSAGE_MAP(CCEProcessBar, CStatic)
        //{{AFX_MSG_MAP(CCEProcessBar)
        ON_WM_PAINT()
        ON_WM_LBUTTONDOWN()
        ON_WM_LBUTTONUP()
        ON_WM_MOUSEMOVE()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CCEProcessBar message handlers
    void CCEProcessBar::SetParam(int nbmpID, int MIN, int MAX, int nStep, int nStyle, int nBmpTop, int nBmpBottom)
    {
        m_nStyle = nStyle;
        m_BitmapIDTop = nBmpTop;
        m_BitmapIDBottom = nBmpBottom;
        m_nMin = MIN;
        m_nMax = MAX;
        m_nStep = nStep;
        if(m_nStep < 1)
            m_nStep = 1;
        m_nPos = 0;
        if(nbmpID > 0)
            m_BitmapID = nbmpID;
        ModifyStyle(0, SS_NOTIFY);
    }

    int CCEProcessBar::GetPercent()
    {
        return m_nPos*100/(m_nMax-m_nMin + 1);
    }

    void CCEProcessBar::SetPos(int nPos, BOOL redraw)
    {
        m_nPos = (nPos*m_nStep)%(m_nMax-m_nMin + 1);
        if(redraw)
            OnDrawBar();
    }

    void CCEProcessBar::OnDrawBar()
    {
        if(!IsWindowVisible())
            return;
        CDC *pdc = GetDC();
        CRect rt;
        GetClientRect(rt);
        if(m_nStyle <= 1)
        {
            if(m_nStyle == 0)
            {
                CBrush bBr = Data::g_progressBackRGB[Data::g_skinstyle]; 
                //	pdc->FillRect(&rt, &bBr);

                CRect lRect = rt;
                lRect.left += 4;
                lRect.right -= 4;
                lRect.top += 3;
                lRect.bottom -= 3;
                if((m_nMax-m_nMin) == 0)
                    lRect.right = lRect.left;
                else
                    lRect.right = lRect.left + m_nPos*lRect.Width()/(m_nMax-m_nMin);

                CRect rRect = rt;
                rRect.left += 4;
                rRect.right -= 4;
                rRect.top += 3;
                rRect.bottom -= 3;
                if((m_nMax-m_nMin) != 0)
                    rRect.left = rRect.left + m_nPos*rRect.Width()/(m_nMax-m_nMin);

                CBrush rBr = Data::g_progressRightRGB[Data::g_skinstyle]; 
                CBrush lBr = Data::g_progressLeftRGB[Data::g_skinstyle]; 
                pdc->FillRect(&lRect, &lBr);
                pdc->FillRect(&rRect, &rBr);

                if(m_BitmapID <= 0)
                {
                    return;
                }

                if(m_BitmapID > 0)
                {
                    CDC memDC;//内存设备场景
                    CBitmap cb;//位图类对象
                    cb.LoadBitmap (m_BitmapID);//载入资源
                    memDC.CreateCompatibleDC (pdc);//创建内存设备上下文
                    CBitmap *oldbitmap=NULL;
                    oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
                    CRect rect = rt;
                    rect.left = lRect.right - MIDDLEBMP_SIZE;
                    rect.right = lRect.right + MIDDLEBMP_SIZE;
                    pdc->BitBlt (rect.left, rect.top, rect.Width (), rect.Height(), &memDC, 0, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
                    memDC.SelectObject (oldbitmap);
                }
            }
            else
            {
                CRect lRect = rt;

                if((m_nMax-m_nMin) == 0)
                    lRect.right = lRect.left;
                else
                    lRect.right = lRect.left + m_nPos*lRect.Width()/(m_nMax-m_nMin);

                if(m_BitmapIDBottom > 0)
                {
                    CDC memDC;//内存设备场景
                    CBitmap cb;//位图类对象
                    cb.LoadBitmap (m_BitmapIDBottom);//载入资源
                    memDC.CreateCompatibleDC (pdc);//创建内存设备上下文
                    CBitmap *oldbitmap=NULL;
                    oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
                    CRect rect = rt;
                    //pdc->BitBlt (rect.left, rect.top, rect.Width (), rect.Height(), &memDC, 0, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
                    pdc->BitBlt (lRect.right+MIDDLEBMP_SIZE, lRect.top, rect.Width()-lRect.right, lRect.Height(), &memDC, lRect.right+MIDDLEBMP_SIZE, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
                    //TransparentBlt2(pdc->m_hDC, rect.left, rect.top, rect.Width (), rect.Height(), memDC.m_hDC, 0, 0, rect.Width (), rect.Height(), RGB(0xFF, 0, 0xFF));
                    memDC.SelectObject (oldbitmap);
                }
                if(m_BitmapIDTop > 0)
                {
                    CDC memDC;//内存设备场景
                    CBitmap cb;//位图类对象
                    cb.LoadBitmap (m_BitmapIDTop);//载入资源
                    memDC.CreateCompatibleDC (pdc);//创建内存设备上下文
                    CBitmap *oldbitmap=NULL;
                    oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
                    CRect rect = rt;

                    if(lRect.Width()-MIDDLEBMP_SIZE > 0)
                        pdc->BitBlt (rect.left, rect.top, lRect.Width()-MIDDLEBMP_SIZE, lRect.Height(), &memDC, 0, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
                    //TransparentBlt2(pdc->m_hDC, rect.left, rect.top, lRect.Width(), lRect.Height(), memDC.m_hDC, 0, 0, lRect.Width(), lRect.Height(), RGB(0xFF, 0, 0xFF));
                    memDC.SelectObject (oldbitmap);
                }

                if(m_BitmapID > 0)
                {
                    CDC memDC;//内存设备场景
                    CBitmap cb;//位图类对象
                    cb.LoadBitmap (m_BitmapID);//载入资源
                    memDC.CreateCompatibleDC (pdc);//创建内存设备上下文
                    CBitmap *oldbitmap=NULL;
                    oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
                    CRect rect = rt;
                    rect.left = lRect.right - MIDDLEBMP_SIZE;
                    rect.right = lRect.right + MIDDLEBMP_SIZE;
                    if(rect.left < 0)
                    {
                        rect.left = 0;
                        rect.right = MIDDLEBMP_SIZE*2;
                    }
                    else if(rect.right > rt.right)
                    {
                        rect.left = rt.right - MIDDLEBMP_SIZE*2;
                        rect.right = rt.right;
                    }
                    pdc->BitBlt (rect.left, rect.top, rect.Width (), rect.Height(), &memDC, 0, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
                    memDC.SelectObject (oldbitmap);
                }
            }
        }
        else      //竖条
        {
            if(m_nStyle == 2) 
            {
                CBrush bBr = Data::g_progressBackRGB[Data::g_skinstyle]; 
                pdc->FillRect(&rt, &bBr);

                CRect bRect = rt;

                //	tRect.left += 4;
                //	tRect.right -= 4;
                //	tRect.top += MIDDLEBMP_SIZE;
                //	tRect.bottom -= MIDDLEBMP_SIZE;

                if((m_nMax-m_nMin) == 0)
                    bRect.top = bRect.bottom;
                else
                    bRect.top = bRect.bottom - m_nPos*bRect.Height()/(m_nMax-m_nMin);

                CRect tRect = rt;

                //	bRect.left += 4;
                //	bRect.right -= 4;
                //	bRect.top += MIDDLEBMP_SIZE;
                //	bRect.bottom -= MIDDLEBMP_SIZE;

                if((m_nMax-m_nMin) != 0)
                    tRect.bottom = tRect.bottom - m_nPos*tRect.Height()/(m_nMax-m_nMin);

                CBrush rBr = Data::g_progressRightRGB[Data::g_skinstyle]; 
                CBrush b_Br = Data::g_progressLeftRGB[Data::g_skinstyle]; 
                pdc->FillRect(&tRect, &rBr);
                pdc->FillRect(&bRect, &b_Br);

                if(m_BitmapID <= 0)
                {
                    return;
                }

                if(m_BitmapID > 0)
                {
                    CDC memDC;//内存设备场景
                    CBitmap cb;//位图类对象
                    cb.LoadBitmap (m_BitmapID);//载入资源
                    memDC.CreateCompatibleDC (pdc);//创建内存设备上下文
                    CBitmap *oldbitmap=NULL;
                    oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
                    CRect rect = rt;
                    rect.left = bRect.top - MIDDLEBMP_SIZE;
                    rect.right = bRect.top + MIDDLEBMP_SIZE;
                    pdc->BitBlt (rect.left, rect.top, rect.Width (), rect.Height(), &memDC, 0, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
                    memDC.SelectObject (oldbitmap);
                }
            }
            else
            {
                CRect bRect = rt;

                if((m_nMax-m_nMin) == 0)
                    bRect.top = bRect.bottom;
                else
                    bRect.top = bRect.bottom - m_nPos*bRect.Height()/(m_nMax-m_nMin);

                CRect tRect = rt;
                if((m_nMax-m_nMin) != 0)
                    tRect.bottom = tRect.bottom - m_nPos*tRect.Height()/(m_nMax-m_nMin);

                //lxz 20080708 屏幕闪动
                //			pdc->FillSolidRect(rt, m_backRGB);

                if(m_BitmapIDBottom > 0)
                {
                    CDC memDC;//内存设备场景
                    CBitmap cb;//位图类对象
                    cb.LoadBitmap (m_BitmapIDBottom);//载入资源
                    memDC.CreateCompatibleDC (pdc);//创建内存设备上下文
                    CBitmap *oldbitmap=NULL;
                    oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
                    CRect rect = rt;
                    pdc->BitBlt (tRect.left, tRect.top, tRect.Width (), tRect.Height(), &memDC, 0, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
                    //TransparentBlt2(pdc->m_hDC, tRect.left, tRect.top, tRect.Width (), tRect.Height(), memDC.m_hDC, 0, 0, tRect.Width (), tRect.Height(), RGB(0xFF, 0, 0xFF));
                    memDC.SelectObject (oldbitmap);
                }
                if(m_BitmapIDTop > 0)
                {
                    CDC memDC;//内存设备场景
                    CBitmap cb;//位图类对象
                    cb.LoadBitmap (m_BitmapIDTop);//载入资源
                    memDC.CreateCompatibleDC (pdc);//创建内存设备上下文
                    CBitmap *oldbitmap=NULL;
                    oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
                    CRect rect = rt;
                    pdc->BitBlt (bRect.left, bRect.top, bRect.Width(), bRect.Height(), &memDC, rt.Width()-bRect.Width(), rt.Height()-bRect.Height(), SRCCOPY);//将内存场景的图象拷贝到屏幕上
                    //TransparentBlt2(pdc->m_hDC, bRect.left, bRect.top, bRect.Width(), bRect.Height(), memDC.m_hDC, rt.Width()-bRect.Width(), rt.Height()-bRect.Height(), bRect.Width(), bRect.Height(), RGB(0xFF, 0, 0xFF));
                    memDC.SelectObject (oldbitmap);
                }

                if(m_BitmapID > 0)
                {
                    CDC memDC;//内存设备场景
                    CBitmap cb;//位图类对象
                    cb.LoadBitmap (m_BitmapID);//载入资源
                    memDC.CreateCompatibleDC (pdc);//创建内存设备上下文
                    CBitmap *oldbitmap=NULL;
                    oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
                    CRect rect = rt;
                    rect.top = tRect.bottom - MIDDLEBMP_SIZE;
                    rect.bottom = tRect.bottom + MIDDLEBMP_SIZE;

                    if(rect.top < 0)
                    {
                        rect.top = 0;
                        rect.bottom = MIDDLEBMP_SIZE*2;
                    }
                    else if(rect.bottom >  rt.bottom)
                    {
                        rect.bottom = rt.bottom;
                        rect.top = rect.bottom - MIDDLEBMP_SIZE*2;
                    }
                    pdc->BitBlt (rect.left, rect.top, rect.Width (), rect.Height(), &memDC, 0, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
                    memDC.SelectObject (oldbitmap);
                }
            }
        }
        ReleaseDC(pdc);	
    }

    void CCEProcessBar::OnPaint() 
    {
        //CPaintDC dc(this); // device context for painting
        CStatic::OnPaint();
        OnDrawBar();
        // TODO: Add your message handler code here

        // Do not call CStatic::OnPaint() for painting messages
    }

    void CCEProcessBar::OnLButtonDown(UINT nFlags, CPoint point) 
    {
        // TODO: Add your message handler code here and/or call default
        if(m_bIsDisable)
            return;

        CRect rt;
        GetClientRect(rt);
        int npos;
        if(m_nStyle <= 1)
            npos = ((point.x - rt.left)*(m_nMax-m_nMin))/(rt.Width()-4);
        else
            npos = ((rt.bottom - point.y)*(m_nMax-m_nMin))/(rt.Height()-4);
        if(abs(npos - m_nPos) < 3)
            m_bSelectedThick = TRUE;
        CStatic::OnLButtonDown(nFlags, point);
    }

    void CCEProcessBar::OnLButtonUp(UINT nFlags, CPoint point)
    {
        // TODO: Add your message handler code here and/or call default
        if(m_bIsDisable)
            return;

        CRect rt;
        GetClientRect(rt);

        int npos;
        if(m_nStyle <= 1)
        {
            npos = ((point.x - rt.left)*(m_nMax-m_nMin))/(rt.Width()-4);
        }
        else
        {
            npos = ((rt.bottom - point.y)*(m_nMax-m_nMin))/(rt.Height()-4);
        }
        //	m_nPos = npos;

        if(npos > m_nMax)
            npos = m_nMax;

        SetPos(npos);
        m_bSelectedThick = FALSE; 
        CWnd *wnd = GetParent();
        int nId = GetDlgCtrlID();
        if(wnd)
            wnd->SendMessage(WM_PROCESS_POS, npos, nId);

        CStatic::OnLButtonUp(nFlags, point);
    }

    BOOL CCEProcessBar::Create(int style, CRect r, CWnd *pParent, int ctrlID)
    {
        return CStatic::Create(L"", style, r, pParent, ctrlID);
    }

    void CCEProcessBar::OnMouseMove(UINT nFlags, CPoint point) 
    {
        // TODO: Add your message handler code here and/or call default
        if(m_bIsDisable)
            return;

        if(nFlags & MK_LBUTTON)
        {
            static CPoint oldpoint = point; 
            CRect rt;
            GetClientRect(rt);
            if(m_nStyle <= 1)
            {
                if(abs(point.x - oldpoint.x) >= 5 && (m_bSelectedThick))
                {
                    int npos = ((point.x - rt.left)*(m_nMax-m_nMin))/(rt.Width()-4);
                    SetPos(npos);
                    oldpoint = point;
                }
            }
            else
            {
                if(abs(point.y - oldpoint.y) >= 5 && (m_bSelectedThick))
                {
                    int npos = ((rt.bottom -point.y)*(m_nMax-m_nMin))/(rt.Height()-4);
                    SetPos(npos);
                    oldpoint = point;
                }
            }
        }

        //	m_nPos = npos;
        CStatic::OnMouseMove(nFlags, point);
    }
}
