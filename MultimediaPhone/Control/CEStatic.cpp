// CEStatic.cpp : implementation file
//

#include "stdafx.h"
#include "CEStatic.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../Logical/LunarderDate.h"
#include "../Data/SkinStyle.h"
#include "../resource.h"

namespace Control {
#define TIMER_TIMESTATIC		0xFFF0

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif
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
    /*CCEStatic*/

    CCEStatic::CCEStatic()
    {
        m_bIsClicked = TRUE;
        m_bIsUnderLine = FALSE;
        m_bIsToggle = FALSE;
        m_txtRGB = RGB(0, 0, 0);
        m_bkRGB = RGB(255, 255, 255);
        m_toggleRGB = RGB(0, 0, 0);
        m_nFontHeight = 28;
        m_nLeftMargin = 0;
        m_sDate1 = "";
        m_sDate2 = ""; 
        m_nBitmapID = 0;
        m_nAlign = DT_LEFT;
        m_bIsBorder = FALSE;
    }

    CCEStatic::~CCEStatic()
    {
        m_Font.DeleteObject();
    }


    BEGIN_MESSAGE_MAP(CCEStatic, CStatic)
        //{{AFX_MSG_MAP(CCEStatic)
        ON_WM_LBUTTONDOWN()
        //	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
        ON_WM_PAINT()
        ON_WM_CTLCOLOR_REFLECT()
        ON_WM_ERASEBKGND()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CCEStatic message handlers
    void CCEStatic::OnLButtonDown(UINT nFlags, CPoint point)
    {
        if((GetDlgCtrlID() != 0xFFFF) && m_bIsClicked)
        {
            GetParent()->SendMessage(STN_CLICKED, GetDlgCtrlID(), point.x); //WM_STATIC_CLICK
        }
    }

    void CCEStatic::OnClicked() 
    {
        // TODO: Add your control notification handler code here
        if((GetDlgCtrlID() != 0xFFFF) && m_bIsClicked)
        {
            GetParent()->SendMessage(STN_CLICKED, GetDlgCtrlID()); //WM_STATIC_CLICK
        }
    }

    void CCEStatic::PreSubclassWindow() 
    {
        // TODO: Add your specialized code here and/or call the base class
        DWORD dwStyle = GetStyle();
        ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY | BS_OWNERDRAW);

        // Create the font
        VERIFY(m_Font.CreateFont(
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
        SetFont(&m_Font);

        CStatic::PreSubclassWindow();
    }

    void CCEStatic::SetFontSize(int size)
    {
        m_nFontHeight = size;
        m_Font.DeleteObject();
        VERIFY(m_Font.CreateFont(
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

        //SetFont(&m_Font);
        //CDC *pdc = GetDC();
        //pdc->SelectObject(&m_Font);
        //ReleaseDC(pdc);
    }

    void CCEStatic::OnPaint() 
    {
        CPaintDC dc(this); // device context for painting

        //	CStatic::OnPaint();

        CDC *pdc = GetDC();
        pdc->SetBkMode(TRANSPARENT);
        pdc->SelectObject(&m_Font);
        CRect rt;
        GetClientRect(&rt);
        if(m_bIsToggle && m_toggleRGB != RGB(0, 0, 0))
        {
            pdc->FillSolidRect(&rt, m_toggleRGB);
        }
        else
            pdc->FillSolidRect(&rt, m_bkRGB);
        CString s;
        GetWindowText(s);

        //贴图
        if(m_nBitmapID > 0)
        {
            CDC memDC;//内存设备场景
            CBitmap cb;//位图类对象
            CRect rect;
            GetClientRect(&rect);
            cb.LoadBitmap (m_nBitmapID);//载入资源
            memDC.CreateCompatibleDC (pdc);//创建内存设备上下文
            CBitmap *oldbitmap=NULL;
            oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
            pdc->BitBlt (rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
            memDC.SelectObject (oldbitmap);
            cb.DeleteObject();
        }
        //

        pdc->SetTextColor(m_txtRGB);
        if(m_sDate1 != "")
        {
            int size = 18;
            if(m_sDate2.GetLength() == 3)
            {
                size = 16;
            }

            SetFontSize(size);
            rt.right = size-1;
            pdc->SelectObject(&m_Font);
            pdc->DrawText(m_sDate1, &rt, DT_WORDBREAK | DT_CENTER);

            int size1 = 14;  //14
            SetFontSize(size1);
            pdc->SelectObject(&m_Font);
            rt.top=0;
            rt.bottom = size1-1;
            rt.left = size;
            rt.right = size+size1;
            //	pdc->FillSolidRect(rt, Data::g_listctrlBackRGB2[Data::g_skinstyle]);
            if(m_sDate2.GetLength() == 3)
            {
                CString s1 = m_sDate2.Mid(1, 1);
                pdc->DrawText(s1, &rt, DT_WORDBREAK | DT_LEFT);
            }
            else
            {
                CString s1 = m_sDate2.Left(1);
                pdc->DrawText(s1, &rt, DT_WORDBREAK | DT_LEFT);
            }

            rt.top=size1-2;    //-1
            rt.bottom = size1*2-1;
            CString s2 = m_sDate2.Right(1);
            //	pdc->FillSolidRect(rt, Data::g_listctrlBackRGB2[Data::g_skinstyle]);
            pdc->DrawText(s2, &rt, DT_WORDBREAK | DT_LEFT);

            if(m_sDate2.GetLength() == 3)
            {
                rt.left = 2;
                rt.right = size-1;		
                rt.top=size1-2;			//-1
                rt.bottom = size1*2-1;
                CString s3 = m_sDate2.Left(1);
                //		pdc->FillSolidRect(rt, Data::g_listctrlBackRGB2[Data::g_skinstyle]);
                pdc->DrawText(s3, &rt, DT_WORDBREAK | DT_LEFT);
            }
        }
        else if(s != "")
        {
            if(rt.Height() > m_nFontHeight)
            {
                int n = (rt.Height() - m_nFontHeight)/2;
                rt.top +=  n;
                rt.bottom -= n;
            }
            if(m_nLeftMargin < rt.Width())
            {
                rt.left = m_nLeftMargin;
            }
            pdc->DrawText(s, &rt, DT_WORDBREAK | m_nAlign);
        }
        if(m_bIsUnderLine)
        {		
            CPen pen (PS_SOLID, 1, m_txtRGB);
            CPen* pOldPen = pdc->SelectObject (&pen);
            pdc->MoveTo(rt.left, rt.bottom-1);
            pdc->LineTo(rt.right, rt.bottom-1);
            pdc->SelectObject(pOldPen);

        }

        if(m_bIsToggle)
        {
            GetClientRect(&rt);
            if(m_toggleRGB == RGB(0, 0, 0))
                pdc->InvertRect(&rt);
        }

        if(m_bIsBorder)
        {
            GetClientRect(&rt);
            CRect rrt = rt;
            CRect rect1;
            rect1 = CRect(rrt.left, rrt.top, rrt.right+1, rrt.top+1);
            pdc->FillSolidRect(&rect1, Data::g_staticLine1RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.bottom-1, rrt.right+1, rrt.bottom);
            pdc->FillSolidRect(&rect1, Data::g_staticLine1RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.top, rrt.left+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_staticLine1RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.right, rrt.top, rrt.right+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_staticLine1RGB[1][Data::g_skinstyle]);

            rrt.DeflateRect(1, 1);
            rect1 = CRect(rrt.left, rrt.top, rrt.right+1, rrt.top+1);
            pdc->FillSolidRect(&rect1, Data::g_staticLine2RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.bottom-1, rrt.right+1, rrt.bottom);
            pdc->FillSolidRect(&rect1, Data::g_staticLine2RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.top, rrt.left+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_staticLine2RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.right, rrt.top, rrt.right+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_staticLine2RGB[1][Data::g_skinstyle]);

            rrt.DeflateRect(1, 1);
            rect1 = CRect(rrt.left, rrt.top, rrt.right+1, rrt.top+1);
            pdc->FillSolidRect(&rect1, Data::g_staticLine3RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.bottom-1, rrt.right+1, rrt.bottom);
            pdc->FillSolidRect(&rect1, Data::g_staticLine3RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.top, rrt.left+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_staticLine3RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.right, rrt.top, rrt.right+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_staticLine3RGB[1][Data::g_skinstyle]);
        }
        ReleaseDC(pdc);
        // Do not call CStatic::OnPaint() for painting messages
    }

    void CCEStatic::SetUnderLine(BOOL isUnderLine)
    {
        m_bIsUnderLine = isUnderLine;
    }

    void CCEStatic::SetClicked(BOOL isClicked)
    {
        m_bIsClicked = isClicked;
    }

    void CCEStatic::SetColor(COLORREF txtClr, COLORREF bkClr)
    {
        m_txtRGB = txtClr;
        m_bkRGB = bkClr;
    }

    HBRUSH CCEStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
    {
        // TODO: Change any attributes of the DC here
        /*
        pDC->SetTextColor(m_txtRGB);
        //  pDC->SetTextAlign(TA_BOTTOM);
        // transparent text.
        pDC->SetBkMode(TRANSPARENT);
        BOOL isFirst = TRUE;
        if(isFirst)
        {
        isFirst = FALSE;
        m_brush.CreateSolidBrush(m_bkRGB);
        }
        */
        // TODO: Return a different brush if the default is not desired
        return NULL;

    }

    BOOL CCEStatic::OnEraseBkgnd(CDC* pDC) 
    {
        // TODO: Add your message handler code here and/or call default
        CRect rt;
        GetClientRect(&rt);
        pDC->FillSolidRect(&rt, m_bkRGB);
        return CStatic::OnEraseBkgnd(pDC);
    }


    void CCEStatic::DrawItem(LPDRAWITEMSTRUCT lpDIS)
    {
    }

    CCETimeStatic::CCETimeStatic()
    {
        m_bChineseVersion = TRUE;
        //	m_nFontHeight = 16;
        m_backRGB = RGB(192, 192, 192);
    }

    CCETimeStatic::~CCETimeStatic()
    {
    }

    void CCETimeStatic::PreSubclassWindow() 
    { 
        DWORD dwStyle = GetStyle();
        ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | BS_OWNERDRAW);
        SetTimer(TIMER_TIMESTATIC, 1000, NULL);
        CButton::PreSubclassWindow();
    } // End of PreSubclassWindow

    void CCETimeStatic::DrawItem(LPDRAWITEMSTRUCT lpDIS)
    {
        CDC*	pDC_ = CDC::FromHandle(lpDIS->hDC);
        CRect rt;
        GetClientRect(&rt);

        SYSTEMTIME curtime;
        GetLocalTime(&curtime);
        CString str1;
        char datetime[64];

        int nWeekDay  = Logical::LunarderDate::WeekDay(curtime.wYear, curtime.wMonth, curtime.wDay);
        sprintf(datetime, "%04d%s%02d%s%02d%s %s", curtime.wYear, Data::LanguageResource::Get(Data::RI_COMN_YEARSTC).c_str(), curtime.wMonth, Data::LanguageResource::Get(Data::RI_COMN_MONTHSTC).c_str(), curtime.wDay, Data::LanguageResource::Get(Data::RI_COMN_DAYSTC).c_str(), Data::LanguageResource::Get(Data::RI_COMN_SUNSTC+nWeekDay).c_str());
        str1 = datetime;

        CDC pDC;
        pDC.CreateCompatibleDC(pDC_);   
        CBitmap bitmap;
        bitmap.CreateCompatibleBitmap(pDC_,  195,   85);   
        CBitmap *pOldBitmap = pDC.SelectObject(&bitmap);   

        pDC.SetBkMode(TRANSPARENT);
        //	pDC->SetTextAlign(TA_CENTER|TA_BOTTOM);
        pDC.FillSolidRect(&rt, m_backRGB);
        CRect rect = rt;
        rect.bottom = rect.top+16;
        pDC.DrawText(str1, rect, DT_CENTER);

        if(m_bChineseVersion)	//if chinese language
        {
            UINT16 nY, nM, nD;
            int nHoliday = Logical::LunarderDate::GetLunarDate(curtime.wYear, curtime.wMonth, curtime.wDay,  nY, nM, nD);
            int nLeapMonth = Logical::LunarderDate::GetLeapMonth(nY);
            char chYear[16], chMonth[16], chDay[16], chHoliday[16],  chLeapMonth[16];

            memset(chYear, 0, 16);
            memset(chMonth, 0, 16);
            memset(chDay, 0, 16);
            memset(chHoliday, 0, 16);
            memset(chLeapMonth, 0, 16);

            int nYear = Logical::LunarderDate::FormatLunarYear(nY, chYear);
            Logical::LunarderDate::FormatLunarMonth(nM, chMonth);
            Logical::LunarderDate::FormatLunarDay(nD, chDay);
            if(nHoliday)
            {
                Logical::LunarderDate::FormatHolDay(nHoliday, chHoliday);
            }
            if(nLeapMonth == nM)
            {
                strcpy(chLeapMonth, "润");
            }
            sprintf(datetime, "%s %s%s %s %s%s %s", Data::LanguageResource::Get(Data::RI_MAIN_LUNARCALENDARSTC).c_str(), chYear, Data::LanguageResource::Get(Data::RI_COMN_YEARSTC).c_str(), chLeapMonth, chMonth, chDay, chHoliday);
            str1 = datetime;
            rect.top = rect.bottom+1;
            rect.bottom = rect.top+18;
            pDC.DrawText(str1, rect, DT_CENTER);
        }

        rect.top = 41;
        rect.left = 19;
        rect.right = 176;
        rect.bottom = 78;

        CRect rt3 = rect;
        //画外框
        rt3 = CRect(19, 41, 176+1, 41+1);
        pDC.FillSolidRect(rt3, Data::g_mainTimeFrameRGB[0][Data::g_skinstyle]);
        rt3 = CRect(19, 78, 176+1, 78+1);
        pDC.FillSolidRect(rt3, Data::g_mainTimeFrameRGB[1][Data::g_skinstyle]);
        rt3 = CRect(19, 41, 19+1, 78+1);
        pDC.FillSolidRect(rt3, Data::g_mainTimeFrameRGB[0][Data::g_skinstyle]);
        rt3 = CRect(176, 41, 176+1, 78+1);
        pDC.FillSolidRect(rt3, Data::g_mainTimeFrameRGB[1][Data::g_skinstyle]);
        //画内框
        rt3 = CRect(20, 42, 175+1, 77+1);
        pDC.FillSolidRect(rt3, Data::g_mainTimeInFrameRGB[0][Data::g_skinstyle]);
        rt3 = CRect(21, 43, 174+1, 76+1);
        pDC.FillSolidRect(rt3, Data::g_mainTimeInFrameRGB[1][Data::g_skinstyle]);
        rt3 = CRect(22, 44, 173, 75);
        for(int i = 0; i < 6; i++)
        {
            rt3 = CRect(22, 47+i*5, 173+1, 47+i*5+1);
            pDC.FillSolidRect(rt3, Data::g_mainTimeInFrameRGB[2][Data::g_skinstyle]);
        }

        for(int j = 0; j < 30; j++)
        {
            rt3 = CRect(25+j*5, 44, 25+j*5+1, 75+1);
            pDC.FillSolidRect(rt3, Data::g_mainTimeInFrameRGB[2][Data::g_skinstyle]);
        }

        sprintf(datetime, "%02d:%02d:%02d", curtime.wHour, curtime.wMinute, curtime.wSecond);
        int len = strlen(datetime);

        CDC memDC;//内存设备场景
        CBitmap cb;//位图类对象
        cb.LoadBitmap (Data::g_mainTimeBmpID[Data::g_skinstyle]);//载入资源
        memDC.CreateCompatibleDC (pDC_);//创建内存设备上下文
        CBitmap *oldbitmap=NULL;
        oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
        rt3 = CRect(21, 43, 176, 76);
        //	pDC.BitBlt (rect.left, rect.top, rect.Width (), rect.Height(), &memDC, 0, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
        for(int n = 0; n < len; n++)
        {
            char c = datetime[n];
            int index = c - '0';
            TransparentBlt2(pDC.m_hDC, rt3.left+12+n*16, rt3.top+5, 14, 23, memDC.m_hDC, index*14, 0, 14, 23,  RGB(255, 0, 255));
        }
        pDC_->BitBlt (rt.left, rt.top, rt.Width (), rt.Height(), &pDC, 0, 0, SRCCOPY);
        pDC.SelectObject(pOldBitmap);   
        memDC.SelectObject (oldbitmap);
        cb.DeleteObject();
        ReleaseDC(pDC_);
    }

    void CCETimeStatic::OnShow()
    {
        //	CStatic::OnPaint();

        CRect rt;
        GetClientRect(&rt);

        SYSTEMTIME curtime;
        GetLocalTime(&curtime);
        CString str1;
        char datetime[64];

        int nWeekDay  = Logical::LunarderDate::WeekDay(curtime.wYear, curtime.wMonth, curtime.wDay);
        sprintf(datetime, "%04d%s%02d%s%02d%s %s", curtime.wYear, Data::LanguageResource::Get(Data::RI_COMN_YEARSTC).c_str(), curtime.wMonth, Data::LanguageResource::Get(Data::RI_COMN_MONTHSTC).c_str(), curtime.wDay, Data::LanguageResource::Get(Data::RI_COMN_DAYSTC).c_str(), Data::LanguageResource::Get(Data::RI_COMN_SUNSTC+nWeekDay).c_str());
        str1 = datetime;

        CDC *pDC_  = GetDC();
        CDC pDC;
        pDC.CreateCompatibleDC(pDC_);   
        CBitmap bitmap;
        bitmap.CreateCompatibleBitmap(pDC_,  195,   85);   
        CBitmap *pOldBitmap = pDC.SelectObject(&bitmap);   

        pDC.SetBkMode(TRANSPARENT);
        //	pDC->SetTextAlign(TA_CENTER|TA_BOTTOM);
        pDC.FillSolidRect(&rt, m_backRGB);
        CRect rect = rt;
        rect.bottom = rect.top+16;
        pDC.DrawText(str1, rect, DT_CENTER);

        if(m_bChineseVersion)	//if chinese language
        {
            UINT16 nY, nM, nD;
            int nHoliday = Logical::LunarderDate::GetLunarDate(curtime.wYear, curtime.wMonth, curtime.wDay,  nY, nM, nD);
            int nLeapMonth = Logical::LunarderDate::GetLeapMonth(nY);
            char chYear[16], chMonth[16], chDay[16], chHoliday[16],  chLeapMonth[16];

            memset(chYear, 0, 16);
            memset(chMonth, 0, 16);
            memset(chDay, 0, 16);
            memset(chHoliday, 0, 16);
            memset(chLeapMonth, 0, 16);

            int nYear = Logical::LunarderDate::FormatLunarYear(nY, chYear);
            Logical::LunarderDate::FormatLunarMonth(nM, chMonth);
            Logical::LunarderDate::FormatLunarDay(nD, chDay);
            if(nHoliday)
            {
                Logical::LunarderDate::FormatHolDay(nHoliday, chHoliday);
            }
            if(nLeapMonth == nM)
            {
                strcpy(chLeapMonth, "润");
            }
            sprintf(datetime, "%s %s%s %s %s%s %s", Data::LanguageResource::Get(Data::RI_MAIN_LUNARCALENDARSTC).c_str(), chYear, Data::LanguageResource::Get(Data::RI_COMN_YEARSTC).c_str(), chLeapMonth, chMonth, chDay, chHoliday);
            str1 = datetime;
            rect.top = rect.bottom+1;
            rect.bottom = rect.top+18;
            pDC.DrawText(str1, rect, DT_CENTER);
        }

        rect.top = 41;
        rect.left = 19;
        rect.right = 176;
        rect.bottom = 78;

        CRect rt3 = rect;
        //画外框
        rt3 = CRect(19, 41, 176+1, 41+1);
        pDC.FillSolidRect(rt3, Data::g_mainTimeFrameRGB[0][Data::g_skinstyle]);
        rt3 = CRect(19, 78, 176+1, 78+1);
        pDC.FillSolidRect(rt3, Data::g_mainTimeFrameRGB[1][Data::g_skinstyle]);
        rt3 = CRect(19, 41, 19+1, 78+1);
        pDC.FillSolidRect(rt3, Data::g_mainTimeFrameRGB[0][Data::g_skinstyle]);
        rt3 = CRect(176, 41, 176+1, 78+1);
        pDC.FillSolidRect(rt3, Data::g_mainTimeFrameRGB[1][Data::g_skinstyle]);
        //画内框
        rt3 = CRect(20, 42, 175+1, 77+1);
        pDC.FillSolidRect(rt3, Data::g_mainTimeInFrameRGB[0][Data::g_skinstyle]);
        rt3 = CRect(21, 43, 174+1, 76+1);
        pDC.FillSolidRect(rt3, Data::g_mainTimeInFrameRGB[1][Data::g_skinstyle]);
        rt3 = CRect(22, 44, 173, 75);
        for(int i = 0; i < 6; i++)
        {
            rt3 = CRect(22, 47+i*5, 173+1, 47+i*5+1);
            pDC.FillSolidRect(rt3, Data::g_mainTimeInFrameRGB[2][Data::g_skinstyle]);
        }

        for(int j = 0; j < 30; j++)
        {
            rt3 = CRect(25+j*5, 44, 25+j*5+1, 75+1);
            pDC.FillSolidRect(rt3, Data::g_mainTimeInFrameRGB[2][Data::g_skinstyle]);
        }

        sprintf(datetime, "%02d:%02d:%02d", curtime.wHour, curtime.wMinute, curtime.wSecond);
        int len = strlen(datetime);

        CDC memDC;//内存设备场景
        CBitmap cb;//位图类对象
        cb.LoadBitmap (Data::g_mainTimeBmpID[Data::g_skinstyle]);//载入资源
        memDC.CreateCompatibleDC (pDC_);//创建内存设备上下文
        CBitmap *oldbitmap=NULL;
        oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
        rt3 = CRect(21, 43, 176, 76);
        //	pDC.BitBlt (rect.left, rect.top, rect.Width (), rect.Height(), &memDC, 0, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
        for(int n = 0; n < len; n++)
        {
            char c = datetime[n];
            int index = c - '0';
            TransparentBlt2(pDC.m_hDC, rt3.left+12+n*16, rt3.top+5, 14, 23, memDC.m_hDC, index*14, 0, 14, 23,  RGB(255, 0, 255));
        }
        pDC_->BitBlt (rt.left, rt.top, rt.Width (), rt.Height(), &pDC, 0, 0, SRCCOPY);
        pDC.SelectObject(pOldBitmap);   
        memDC.SelectObject (oldbitmap);
        cb.DeleteObject();
        ReleaseDC(pDC_);
    }

    void CCETimeStatic::OnTimer(UINT nIDEvent) 
    {
        // TODO: Add your message handler code here and/or call default
        if(nIDEvent == TIMER_TIMESTATIC)
        {
            if(this->IsWindowVisible())
            {
                OnShow();
                //	Invalidate();
            }
        }
        CButton::OnTimer(nIDEvent);
    }


    BEGIN_MESSAGE_MAP(CCETimeStatic, CButton)
        //{{AFX_MSG_MAP(CCEStatic)
        ON_WM_TIMER()
        //	ON_WM_PAINT()
        ON_WM_DRAWITEM()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    CCEMoveTxtStatic::CCEMoveTxtStatic()
    {
        m_nCount = 0;
        m_backColor = RGB(192, 192, 192);
    }

    CCEMoveTxtStatic::~CCEMoveTxtStatic()
    {
        memDC.DeleteDC();   
        bmp.DeleteObject();   
    }

    void CCEMoveTxtStatic::OnTimer(UINT nIDEvent)
    {
        if(nIDEvent == TIMER_TIMESTATIC)
        {
            m_nCount++;
            if(m_nCount*16 >= m_nW)
                m_nCount = 0;
            Invalidate();
        }
    }

    BOOL CCEMoveTxtStatic::Create(LPCTSTR sTxt, COLORREF backcl, int style, CRect &rect, CWnd *pParentWnd, int nCtrlID)
    {
        m_backColor = backcl;
        BOOL ret = CStatic::Create(sTxt, style, rect, pParentWnd);
        SetTimer(TIMER_TIMESTATIC, 1000, NULL);
        m_font = GetFont();

        CRect   rc;   
        GetClientRect(rc);   
        CString   strText;   
        GetWindowText(strText);
        CDC *pdc = GetDC();
        memDC.CreateCompatibleDC(pdc); 

        memDC.SelectObject(m_font);  
        CSize   size   =   pdc->GetTextExtent(strText);
        int w = rc.Width()*2+size.cx;
        m_nW = rc.Width()+size.cx;;
        int h = rc.Height();
        bmp.CreateCompatibleBitmap(pdc,   w,   h);   
        memDC.SelectObject(&bmp);   
        memDC.FillSolidRect(CRect(0, 0, w, h),   m_backColor);   
        memDC.DrawText(strText, CRect(0, 0, w-1, h-1), DT_CENTER|DT_VCENTER);
        ReleaseDC(pdc);
        return ret;
    }

    void CCEMoveTxtStatic::OnPaint()     
    {   
        //   TODO:   Add   your   message   handler   code   here   

        CPaintDC   dc(this); 
        CRect   rc;   
        GetClientRect(rc);  
        dc.BitBlt(rc.left,   rc.top,   rc.Width(),   rc.Height(),   &memDC,   m_nCount*16,   rc.top,   SRCCOPY);
    }

    void CCEMoveTxtStatic::SetTxt(LPCTSTR sTxt)
    {
        SetWindowText(sTxt);

        memDC.DeleteDC();   
        bmp.DeleteObject(); 

        CRect   rc;   
        GetClientRect(rc);   
        CString   strText;   
        GetWindowText(strText);
        CDC *pdc = GetDC();
        memDC.CreateCompatibleDC(pdc); 

        memDC.SelectObject(m_font);  
        CSize   size   =   pdc->GetTextExtent(strText);
        int w = rc.Width()*2+size.cx;
        m_nW = rc.Width()+size.cx;
        int h = rc.Height();
        bmp.CreateCompatibleBitmap(pdc,   w,   h);   
        memDC.SelectObject(&bmp);   
        memDC.FillSolidRect(CRect(0, 0, w, h),   m_backColor);   
        memDC.DrawText(strText, CRect(0, 0, w-1, h-1), DT_CENTER|DT_VCENTER);
        ReleaseDC(pdc);
    }

    BEGIN_MESSAGE_MAP(CCEMoveTxtStatic, CStatic)
        //{{AFX_MSG_MAP(CCEStatic)
        ON_WM_TIMER()
        ON_WM_PAINT()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /*CCEWriteStatic*/

    CCEWriteStatic::CCEWriteStatic()
    {
        ResetPoint();
        m_bChanged = FALSE;
        WRITE_TYPE = 1;
        DOWITH_WRITE = 1;
    }

    CCEWriteStatic::~CCEWriteStatic()
    {
    }


    BEGIN_MESSAGE_MAP(CCEWriteStatic, CStatic)
        //{{AFX_MSG_MAP(CStatic)
        ON_WM_LBUTTONDOWN()
        ON_WM_LBUTTONUP()
        ON_WM_MOUSEMOVE()
        ON_WM_PAINT()
        ON_WM_CTLCOLOR_REFLECT()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CCEWriteStatic message handlers

    void CCEWriteStatic::PreSubclassWindow() 
    {
        // TODO: Add your specialized code here and/or call the base class
        DWORD dwStyle = GetStyle();
        ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
        CStatic::PreSubclassWindow();

        m_pDC = GetDC();
        m_Pen.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
        CPen* pOldPen1 = m_pDC->SelectObject (&m_Pen);

        /*
        CDC *pdc = GetDC();
        m_MemDC.CreateCompatibleDC(pdc);
        CRect rt;
        GetWindowRect(&rt);
        m_Bitmap.CreateCompatibleBitmap(&m_MemDC, rt.Width(), rt.Height());
        CBitmap *oldbitmap=m_MemDC.SelectObject (&m_Bitmap);//将位图选入当前设备场景
        ReleaseDC(pdc);
        */
    }

    void CCEWriteStatic::DrawMemDC()
    {
        /*
        CRect rt;
        GetWindowRect(&rt);
        int w = rt.Width();
        int h = rt.Height();

        UINT16 w1 = *(UINT16 *)m_data;
        UINT16 h1 = *(UINT16 *)(m_data+2);

        char *ptr = (m_data+4);

        for(int r = 0; r < h; r++)
        {
        for(int c = 0; c < w; c++)
        {
        if(c >= w1 )
        m_MemDC.SetPixel(CPoint(c, r), RGB(255, 255, 255));	//白色
        else
        {
        UINT8 data = *(ptr+ (w/8+1)*r + c/8);
        UINT8 bit = ((data >> (7-c%8))&1);
        if(bit == 1)
        m_MemDC.SetPixel(CPoint(c, r), RGB(0, 0, 0));		//黑色
        else 
        m_MemDC.SetPixel(CPoint(c, r), RGB(255, 255, 255));	//白色
        }
        }
        }
        */
    }

    void CCEWriteStatic::OnPaint() 
    {
        CPaintDC dc(this); // device context for painting
        CRect rt;
        GetWindowRect(&rt);
        CDC *pdc = GetDC();
        pdc->BitBlt(0, 0, rt.Width(), rt.Height(), m_MemDC, 0, 0, SRCCOPY);
        ReleaseDC(pdc);
        // Do not call CStatic::OnPaint() for painting messages
    }

    HBRUSH CCEWriteStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
    {
        // TODO: Change any attributes of the DC here
        /*
        CBrush m_brush;
        m_brush.CreateSolidBrush(RGB(0, 0, 0));
        pDC->SelectObject(m_brush);
        */
        HBRUSH b=CreateSolidBrush(RGB(255,255,255));
        return b;

        // TODO: Return a different brush if the default is not desired
        return NULL;

    }

    void CCEWriteStatic::SetWrite(char *ptr)
    {
        m_data = ptr;
        DrawMemDC();
        Invalidate();
    }

    void CCEWriteStatic::SetWrite(TCHAR *filename)
    {
        m_bChanged = FALSE;
        aDibSectionCE.Load(filename);
        m_MemDC = aDibSectionCE.GetMemoryDC();

        CPen* pOldPen1 = m_MemDC->SelectObject(&m_Pen);

        Invalidate();
    }

    void CCEWriteStatic::GetWrite(TCHAR *filename)
    {
        if(m_bChanged)
            aDibSectionCE.Save(filename);
    }

    void CCEWriteStatic::GetWrite(char *ptr)
    {
        /*
        CRect rt;
        GetWindowRect(&rt);
        int w = rt.Width();
        int h = rt.Height();

        *(UINT16 *)ptr = w;
        *(UINT16 *)(ptr +2) = h;

        char *ptr1 = (ptr+4);
        for(int r = 0; r < h; r++)
        {
        for(int c = 0; c < w; c++)
        {
        char *temp = ptr1+ (w/8+1)*r + c/8;
        UINT data = *(temp);
        COLORREF color = m_MemDC.GetPixel(CPoint(c, r));		
        char bit = 0;
        if(color == 0)  //黑色
        {
        bit = 1;
        }
        else			//白色	
        {
        bit = 0;
        }
        data |= (bit<<(7-c%8));
        *(temp) = data;
        }
        }
        */
    }

    void CCEWriteStatic::OnLButtonDown(UINT nFlags, CPoint point)
    {
        //Dprintf("Down Pt rest\n");
        ResetPoint();
        m_bChanged = TRUE;
        /*
        CPen pen (PS_SOLID, 1, RGB(0, 0, 0));
        CPen* pOldPen = m_MemDC->SelectObject (&pen);
        if(m_OldPoint.x != 0xFFFF && m_OldPoint.y != 0xFFFF)
        {
        m_MemDC->MoveTo(m_OldPoint);
        m_MemDC->LineTo(point);
        }
        m_MemDC->SelectObject(pOldPen);
        Invalidate();
        */

        m_OldPoint = point;
        CStatic::OnLButtonDown(nFlags, point);
    }

    void CCEWriteStatic::OnLButtonUp(UINT nFlags, CPoint point)
    {
        //Dprintf("Up Pt rest\n");

        ResetPoint();
        CStatic::OnLButtonUp(nFlags, point);
    }

    CPoint CCEWriteStatic::DoWithWrite(CPoint pt)
    {
        if (DOWITH_WRITE == 1)
        {
            CPoint ret = m_PrePoint[0];
            m_PrePoint[0] = m_PrePoint[1];
            m_PrePoint[1] = m_PrePoint[2];

            if (m_PrePoint[0].x != 0xFFFF || m_PrePoint[0].y != 0xFFFF)
            {
                m_PrePoint[2].x = (m_PrePoint[0].x + pt.x) / 2;
                m_PrePoint[2].y = (m_PrePoint[0].y + pt.y) / 2;
            }
            else
            {
                m_PrePoint[2] = pt;
            }
            if (ret.x == 0xFFFF || ret.y == 0xFFFF)
                return pt;
            return ret;
        }
        else
            return pt;
    }

    void CCEWriteStatic::OnAddPoint(CPoint pt1, CPoint point)
    {
        if(pt1.x != 0xFFFF && pt1.y != 0xFFFF)
        {   
            unsigned int add_x, add_y, dx,dy;
            unsigned int l_long;
            int sign_x,sign_y;
            int x,y;
            int nx,ny;
            nx=pt1.x;
            ny=pt1.y;
            int oldx = pt1.x;
            int oldy = pt1.y;
            if(point.x>=pt1.x)
            { 
                dx=point.x-pt1.x;
                sign_x=1;
            }
            else 
            {
                dx=pt1.x-point.x;
                sign_x=-1;
            }
            if(point.y>=pt1.y) 
            {
                dy=point.y-pt1.y;
                sign_y=1;
            }
            else 
            {
                dy=pt1.y-point.y;
                sign_y=-1;
            }
            l_long=dx+dy;
            if(l_long==0)
            {
                return;
            }
            add_x=l_long/2;
            add_y=add_x;
            //Dprintf("add %d\r\n", add_x);
            while(x !=point.x||y !=point.y)
            {
                add_x+=dx;
                add_y+=dy;
                if(x !=point.x)
                    x=pt1.x+(add_x/l_long)*sign_x;
                if(y !=point.y)
                    y=pt1.y+(add_y/l_long)*sign_y;
                if(nx !=x||ny !=y) 
                {
                    nx=x;
                    ny=y;
                    m_MemDC->MoveTo(oldx, oldy);
                    m_MemDC->LineTo(nx, ny);

                    m_pDC->MoveTo(oldx, oldy);
                    m_pDC->LineTo(nx, ny);

                    oldx = nx;
                    oldy = ny;
                }
            }
        }
    }

    void CCEWriteStatic::OnMouseMove(UINT nFlags, CPoint point) 
    {
        if (nFlags & MK_LBUTTON)
        {
            if (WRITE_TYPE == 1)
            {
                if (m_OldPoint.x <= 800 * X_XISHU && m_OldPoint.y <= 480 * Y_XISHU)
                {
                    if (m_OldPoint.x != point.x || m_OldPoint.y != point.y)
                    {
                        MoveToEx(m_MemDC->m_hDC, m_OldPoint.x, m_OldPoint.y, NULL);
                        LineTo(m_MemDC->m_hDC, point.x, point.y);

                        MoveToEx(m_pDC->m_hDC, m_OldPoint.x, m_OldPoint.y, NULL);
                        LineTo(m_pDC->m_hDC, point.x, point.y);
                    }
                }

                m_OldPoint.x = point.x;
                m_OldPoint.y = point.y;
            }
            else
            {
                OnAddPoint(m_OldPoint, point);
                m_OldPoint = point;
            }
            Invalidate();
        }
    }

    void CCEWriteStatic::ResetPoint()
    {
        m_OldPoint = CPoint(0xFFFF, 0xFFFF);
        m_PrePoint[0] = CPoint(0xFFFF, 0xFFFF);
        m_PrePoint[1] = CPoint(0xFFFF, 0xFFFF);
        m_PrePoint[2] = CPoint(0xFFFF, 0xFFFF);
    }

    void CCEWriteStatic::ClearDC()
    {
        CRect rt;
        GetClientRect(&rt);
        m_MemDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(255, 255, 255));
        m_bChanged = TRUE;
        Invalidate();
    }

    //用于窗口的整个边框风格
    CCEFrameStatic::CCEFrameStatic()
    {
        m_pTxtStruct = NULL;
        m_ntxtCount = 0;
        m_nInRectWidth = 0;
    }

    CCEFrameStatic::~CCEFrameStatic()
    {
        if (m_pTxtStruct)
            delete m_pTxtStruct;

        //memDC.DeleteDC();
        //bmp.DeleteObject();
    }

    BEGIN_MESSAGE_MAP(CCEFrameStatic, CStatic)
        //{{AFX_MSG_MAP(CCEFrameStatic)
        ON_WM_PAINT()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    BOOL CCEFrameStatic::Create(CRect &rect, CWnd *pParentWnd, int nStyle)
    {
        m_nStyle = nStyle;
        BOOL ret = CStatic::Create(L"", WS_CHILD|WS_VISIBLE, rect, pParentWnd);
        /*
        CRect   rc;   
        GetClientRect(rc);  
        CDC *pdc = GetDC();
        memDC.CreateCompatibleDC(pdc); 
        bmp.CreateCompatibleBitmap(pdc,   rc.Width(),  rc.Height());   
        memDC.SelectObject(&bmp);   
        ReleaseDC(pdc);
        */
        return ret;
    }

    void CCEFrameStatic::OnDrawRect()
    {
        //CStatic::OnPaint();

        CDC *pdc = GetDC();
        CRect rt;
        GetClientRect(&rt);

        CDC memDC;
        CBitmap bmp;
        memDC.CreateCompatibleDC(pdc); 
        bmp.CreateCompatibleBitmap(pdc,   rt.Width(),  rt.Height());   
        memDC.SelectObject(&bmp);   

        BOOL bHalfwindow = FALSE;
        if(rt.Width() < 480)
            bHalfwindow = TRUE;

        //画top线
        CRect rect1= rt;
        rect1 = CRect(0, 0, 480-1+1, 0+1);
        memDC.FillSolidRect(&rect1, Data::g_allFramTopLineRGB[0][Data::g_skinstyle]);
        rect1 = CRect(1, 1, 480-2+1, 1+1);
        memDC.FillSolidRect(&rect1, Data::g_allFramTopLineRGB[1][Data::g_skinstyle]);
        rect1 = CRect(2, 2, 480-3+1, 2+1);
        memDC.FillSolidRect(&rect1, Data::g_allFramTopLineRGB[2][Data::g_skinstyle]);
        rect1 = CRect(3, 3, 480-4+1, 3+1);
        memDC.FillSolidRect(&rect1, Data::g_allFramTopLineRGB[3][Data::g_skinstyle]);

        if(m_nStyle == 0)
        {
            rect1 = CRect(1, 4, 480-2+1, 12+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramTopLineRGB[4][Data::g_skinstyle]);
            rect1 = CRect(4, 13, 480-5+1, 13+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramBackRGB[Data::g_skinstyle]);
        }

        //画bottom线
        if(!bHalfwindow)
        {
            rect1 = CRect(0, 204-1, 480-1+1, 204-1+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramBottomLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(1, 204-2, 480-2+1, 204-2+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramBottomLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(2, 204-3, 480-3+1, 204-3+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramBottomLineRGB[2][Data::g_skinstyle]);
        }

        //画left线
        rect1 = CRect(0, 0, 0+1, 204-1+1);
        memDC.FillSolidRect(&rect1, Data::g_allFramLeftLineRGB[0][Data::g_skinstyle]);
        rect1 = CRect(1, 1, 1+1, 204-2+1);
        memDC.FillSolidRect(&rect1, Data::g_allFramLeftLineRGB[1][Data::g_skinstyle]);
        rect1 = CRect(2, 2, 2+1, 204-3+1);
        memDC.FillSolidRect(&rect1, Data::g_allFramLeftLineRGB[2][Data::g_skinstyle]);
        rect1 = CRect(3, 2, 3+1, 204-4+1);
        memDC.FillSolidRect(&rect1, Data::g_allFramLeftLineRGB[3][Data::g_skinstyle]);

        //画right线
        if(!bHalfwindow)
        {
            rect1 = CRect(480-1, 0, 480-1+1, 204-1+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramRightLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(480-2, 1, 480-2+1, 204-2+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramRightLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(480-3, 2, 480-3+1, 204-3+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramRightLineRGB[2][Data::g_skinstyle]);
            rect1 = CRect(480-4, 2, 480-4+1, 204-4+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramRightLineRGB[3][Data::g_skinstyle]);
        }


        if(m_nStyle == 0)
        {
            //最后画一条Top线
            rect1 = CRect(1, 14, 480-2+1, 14+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramTopLineRGB[4][Data::g_skinstyle]);

            //画背景
            rect1 = CRect(4, 15, 480-5+1, 204-4+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramBackRGB[Data::g_skinstyle]);
        }
        else if(m_nStyle == 1 || m_nStyle == 2)
        {
            //画背景
            rect1 = CRect(4, 4, 480-4+1, 204-4+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramBackRGB[Data::g_skinstyle]);
        }

        //当 m_nStyle = 1时，画三角形及中间框
        if(m_nStyle == 1)
        {
            //画top
            rect1 = CRect(293, 0, 480-1+1, 0+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramAngleTopLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(294, 1, 480-2+1, 1+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramAngleTopLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(295-2, 2, 480-3+1, 2+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramAngleTopLineRGB[2][Data::g_skinstyle]);
            rect1 = CRect(295-2, 3, 480-4+1, 3+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramAngleTopLineRGB[3][Data::g_skinstyle]);
            rect1 = CRect(295-2, 4, 480-5+1, 4+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramAngleTopLineRGB[4][Data::g_skinstyle]);

            //画right
            rect1 = CRect(480-1, 0, 480-1+1, 188-1+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramAngleRightLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(480-2, 1, 480-2+1, 188-2+1+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramAngleRightLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(480-3, 2, 480-3+1, 188-3+1+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramAngleRightLineRGB[2][Data::g_skinstyle]);
            rect1 = CRect(480-4, 3, 480-4+1, 188-4+1+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramAngleRightLineRGB[3][Data::g_skinstyle]);
            rect1 = CRect(480-5, 4, 480-5+1, 188-5+1+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramAngleRightLineRGB[4][Data::g_skinstyle]);

            int x = 296;
            for(int i = 5; i <= 184; i++)
            {
                rect1 = CRect(x++, i, 475+1, i+1);
                memDC.FillSolidRect(&rect1, Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);
            }

            //画三角形
            CPen pen(PS_SOLID, 1, Data::g_allFramAngleXiexianLineRGB[0][Data::g_skinstyle]);
            CPen *oldpen = memDC.SelectObject(&pen);
            memDC.MoveTo(CPoint(292, 2));
            memDC.LineTo(CPoint(478, 188));
            CPen pen1(PS_SOLID, 1, Data::g_allFramAngleXiexianLineRGB[1][Data::g_skinstyle]);
            memDC.SelectObject(&pen1);
            memDC.MoveTo(CPoint(291, 2));
            memDC.LineTo(CPoint(477, 188));
            memDC.SelectObject(oldpen);

            rect1 = CRect(303, 10, 448+1, 10+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramAngleHerLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(304, 11, 448+1, 11+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramAngleHerLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(306, 13, 448+1, 13+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramAngleHerLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(307, 14, 448+1, 14+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramAngleHerLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(310, 16, 448+1, 16+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramAngleHerLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(311, 17, 448+1, 17+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramAngleHerLineRGB[1][Data::g_skinstyle]);

            //画内框
            //画top
            rect1 = CRect(8, 26, 407+1, 26+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectTopLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(8-1, 26-1, 407+1+1, 26+1-1);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectTopLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(8-2, 26-2, 407+1+2, 26+1-2);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectTopLineRGB[2][Data::g_skinstyle]);

            //画bottom
            rect1 = CRect(8, 197, 407+1, 197+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectBottomLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(8-1, 197+1, 407+1+1, 197+1+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectBottomLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(8-2, 197+2, 407+1+2, 197+1+2);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectBottomLineRGB[2][Data::g_skinstyle]);

            //画left
            rect1 = CRect(7, 26, 7+1, 197+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectLeftLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(7-1, 26-1, 7+1-1, 197+1+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectLeftLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(7-2, 26-2, 7+1-2, 197+1+2);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectLeftLineRGB[2][Data::g_skinstyle]);

            //画right
            rect1 = CRect(408, 26, 408+1, 197+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectRightLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(408+1, 26-1, 408+1+1, 197+1+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectRightLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(408+2, 26-2, 408+1+2, 197+1+2);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectRightLineRGB[2][Data::g_skinstyle]);

            //画Angletop
            rect1 = CRect(316, 26, 407+1, 26+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectAngleTopLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(316-1, 26-1, 407+1+1, 26+1-1);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectAngleTopLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(316-2, 26-2, 407+1+2, 26+1-2);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectAngleTopLineRGB[2][Data::g_skinstyle]);

            //画Angleright
            rect1 = CRect(408, 26, 408+1, 118+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectAngleRightLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(408+1, 26-1, 408+1+1, 118+1+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectAngleRightLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(408+2, 26-2, 408+1+2, 118+1+2);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectAngleRightLineRGB[2][Data::g_skinstyle]);

            //画InRect
            rect1 = CRect(8, 27, 407+1, 196+1);
            memDC.FillSolidRect(&rect1, Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        }

        //画上小框
        if(bHalfwindow && m_InRect.Width() != 0)
        {
            CRect rrt = m_InRect;
            rect1 = CRect(rrt.left, rrt.top, rrt.right+1, rrt.top+1);
            memDC.FillSolidRect(&rect1, Data::g_allInRectLine1RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.bottom, rrt.right+1, rrt.bottom+1);
            memDC.FillSolidRect(&rect1, Data::g_allInRectLine1RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.top, rrt.left+1, rrt.bottom+1);
            memDC.FillSolidRect(&rect1, Data::g_allInRectLine1RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.right, rrt.top, rrt.right+1, rrt.bottom+1);
            memDC.FillSolidRect(&rect1, Data::g_allInRectLine1RGB[1][Data::g_skinstyle]);

            rrt.DeflateRect(1,1);
            rect1 = CRect(rrt.left, rrt.top, rrt.right+1, rrt.top+1);
            memDC.FillSolidRect(&rect1, Data::g_allInRectLine2RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.bottom, rrt.right+1, rrt.bottom+1);
            memDC.FillSolidRect(&rect1, Data::g_allInRectLine2RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.top, rrt.left+1, rrt.bottom+1);
            memDC.FillSolidRect(&rect1, Data::g_allInRectLine2RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.right, rrt.top, rrt.right+1, rrt.bottom+1);
            memDC.FillSolidRect(&rect1, Data::g_allInRectLine2RGB[1][Data::g_skinstyle]);

            if(m_nInRectWidth == 1)
            {
                memDC.FillSolidRect(&rrt, Data::g_allInRectBackRGB[Data::g_skinstyle]);

                rrt.DeflateRect(1,1);
                rect1 = CRect(rrt.left, rrt.top, rrt.right+1, rrt.top+1);
                memDC.FillSolidRect(&rect1, Data::g_allInRectLine3RGB[0][Data::g_skinstyle]);
                rect1 = CRect(rrt.left, rrt.bottom, rrt.right+1, rrt.bottom+1);
                memDC.FillSolidRect(&rect1, Data::g_allInRectLine3RGB[1][Data::g_skinstyle]);

                rect1 = CRect(rrt.left, rrt.top, rrt.left+1, rrt.bottom+1);
                memDC.FillSolidRect(&rect1, Data::g_allInRectLine3RGB[0][Data::g_skinstyle]);
                rect1 = CRect(rrt.right, rrt.top, rrt.right+1, rrt.bottom+1);
                memDC.FillSolidRect(&rect1, Data::g_allInRectLine3RGB[1][Data::g_skinstyle]);

                rrt.DeflateRect(1,1);
                rect1 = CRect(rrt.left, rrt.top, rrt.right+1, rrt.top+1);
                memDC.FillSolidRect(&rect1, Data::g_allInRectLine4RGB[0][Data::g_skinstyle]);
                rect1 = CRect(rrt.left, rrt.bottom, rrt.right+1, rrt.bottom+1);
                memDC.FillSolidRect(&rect1, Data::g_allInRectLine4RGB[1][Data::g_skinstyle]);

                rect1 = CRect(rrt.left, rrt.top, rrt.left+1, rrt.bottom+1);
                memDC.FillSolidRect(&rect1, Data::g_allInRectLine4RGB[0][Data::g_skinstyle]);
                rect1 = CRect(rrt.right, rrt.top, rrt.right+1, rrt.bottom+1);
                memDC.FillSolidRect(&rect1, Data::g_allInRectLine4RGB[1][Data::g_skinstyle]);
            }		
        }

        //通话记录 名片主窗口
        if(m_nStyle == 2)
        {
            CRect rrt = CRect(7, 26, 472, 197);
            rect1 = CRect(rrt.left, rrt.top, rrt.right+1, rrt.top+1);
            memDC.FillSolidRect(&rect1, Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.bottom, rrt.right+1, rrt.bottom+1);
            memDC.FillSolidRect(&rect1, Data::g_allFrameInFrameLine2RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.top, rrt.left+1, rrt.bottom+1);
            memDC.FillSolidRect(&rect1, Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.right, rrt.top, rrt.right+1, rrt.bottom+1);
            memDC.FillSolidRect(&rect1, Data::g_allFrameInFrameLine2RGB[0][Data::g_skinstyle]);

            rrt.DeflateRect(-1,-1);
            rect1 = CRect(rrt.left, rrt.top, rrt.right+1, rrt.top+1);
            memDC.FillSolidRect(&rect1, Data::g_allFrameInFrameLine1RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.bottom, rrt.right+1, rrt.bottom+1);
            memDC.FillSolidRect(&rect1, Data::g_allFrameInFrameLine2RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.top, rrt.left+1, rrt.bottom+1);
            memDC.FillSolidRect(&rect1, Data::g_allFrameInFrameLine1RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.right, rrt.top, rrt.right+1, rrt.bottom+1);
            memDC.FillSolidRect(&rect1, Data::g_allFrameInFrameLine2RGB[1][Data::g_skinstyle]);

            rrt.DeflateRect(-1,-1);
            rect1 = CRect(rrt.left, rrt.top, rrt.right+1, rrt.top+1);
            memDC.FillSolidRect(&rect1, Data::g_allFrameInFrameLine1RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.bottom, rrt.right+1, rrt.bottom+1);
            memDC.FillSolidRect(&rect1, Data::g_allFrameInFrameLine2RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.top, rrt.left+1, rrt.bottom+1);
            memDC.FillSolidRect(&rect1, Data::g_allFrameInFrameLine1RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.right, rrt.top, rrt.right+1, rrt.bottom+1);
            memDC.FillSolidRect(&rect1, Data::g_allFrameInFrameLine2RGB[1][Data::g_skinstyle]);
        }

        //在背景上画上文字
        for(int nIndex = 0; nIndex < m_ntxtCount; nIndex++)
        {
            VERIFY(m_Font.CreateFont(
                m_pTxtStruct[nIndex].txtFontSize,                        // nHeight
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
                _T("宋体")));              // lpszFacename

            rect1= m_pTxtStruct[nIndex].txtRect;
            CFont *oldfont = memDC.SelectObject(&m_Font);

            int oldMode = memDC.SetBkMode(TRANSPARENT);
            int oldcolor = memDC.SetTextColor(m_pTxtStruct[nIndex].txtColor);
            CString s = Util::StringOp::ToCString(m_pTxtStruct[nIndex].sTxt);
            memDC.DrawText(s, &rect1, DT_WORDBREAK |m_pTxtStruct[nIndex].sAlign);

            memDC.SetTextColor(oldcolor);
            memDC.SetBkMode(oldMode);
            memDC.SelectObject(oldfont);
            m_Font.DeleteObject();
        }

        pdc->BitBlt(rt.left, rt.top, rt.Width(), rt.Height(), &memDC, rt.left, rt.top, SRCCOPY);

        memDC.DeleteDC();   
        bmp.DeleteObject();
        ReleaseDC(pdc);
    }

    void CCEFrameStatic::OnPaint()
    {
        CStatic::OnPaint();
        OnDrawRect();	
    }

    void CCEFrameStatic::SetTextStruct(TextStruct *pTxtStruct, int ncount, BOOL reDraw)
    {
        if(m_pTxtStruct)
        {
            delete m_pTxtStruct;
        }
        m_ntxtCount = ncount;
        m_pTxtStruct  = new TextStruct[ncount];
        memset(m_pTxtStruct, 0, sizeof(TextStruct) * m_ntxtCount);
        memcpy(m_pTxtStruct, pTxtStruct, sizeof(TextStruct) * m_ntxtCount);
        if(reDraw)
        {
            Invalidate();
        }
    }

    void CCEFrameStatic ::ChangeTextStruct(int nIndex, TextStruct txtStruct, BOOL reDraw)
    {
        if(nIndex < m_ntxtCount)
        {
            memcpy(&m_pTxtStruct[nIndex], &txtStruct, sizeof(TextStruct));
        }
        if(reDraw)
        {
            //InvalidateRect(&m_pTxtStruct[nIndex].txtRect);
            OnDrawRect();
        }
    }

    void CCEFrameStatic::SetSkinStyle()
    {

    }

    //用于部分窗口的边框风格
    CCEFramePartStatic::CCEFramePartStatic()
    {
        m_pTxtStruct = NULL;
        m_ntxtCount = 0;
    }

    CCEFramePartStatic::~CCEFramePartStatic()
    {
        if(m_pTxtStruct)
            delete m_pTxtStruct;

        //	memDC.DeleteDC();   
        //	bmp.DeleteObject();   
    }

    BEGIN_MESSAGE_MAP(CCEFramePartStatic, CStatic)
        //{{AFX_MSG_MAP(CCEFramePartStatic)
        ON_WM_PAINT()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    BOOL CCEFramePartStatic::Create(CRect &rect, CWnd *pParentWnd, int nStyle)
    {
        m_nStyle = nStyle;
        BOOL ret = CStatic::Create(L"", WS_CHILD|WS_VISIBLE, rect, pParentWnd);
        /*
        CRect   rc;   
        GetClientRect(rc);   
        CDC *pdc = GetDC();
        memDC.CreateCompatibleDC(pdc); 
        bmp.CreateCompatibleBitmap(pdc,   rc.Width(),  rc.Height());   
        memDC.SelectObject(&bmp);   
        ReleaseDC(pdc);
        */
        return ret;
    }

#define FRAMEPARTWIDTH		(415+1)
#define FRAMEPARTHEIGHT		(144+1)

    void CCEFramePartStatic::OnPaint()
    {
        CStatic::OnPaint();
        CDC *pdc = GetDC();
        CRect rt;
        GetClientRect(&rt);

        CDC memDC;
        CBitmap bmp;
        memDC.CreateCompatibleDC(pdc); 
        bmp.CreateCompatibleBitmap(pdc,   rt.Width(),  rt.Height());   
        memDC.SelectObject(&bmp); 

        CRect rect1= rt;

        //擦背景色
        rect1 = CRect(0, 0, 415, 144);
        memDC.FillSolidRect(&rect1, Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        //画左边Title top
        rect1 = CRect(0, 0, 136+4+1, 0+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle1TopRGB[0][Data::g_skinstyle]);
        rect1 = CRect(1, 1, 137+4+1, 1+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle1TopRGB[1][Data::g_skinstyle]);
        rect1 = CRect(2, 2, 138+4+1, 2+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle1TopRGB[2][Data::g_skinstyle]);
        rect1 = CRect(3, 3, 139+4+1, 3+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle1TopRGB[3][Data::g_skinstyle]);

        //画左边Title bottom
        rect1 = CRect(9, 25, 159+4+1, 25+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle1BottomRGB[0][Data::g_skinstyle]);
        rect1 = CRect(9-1, 25-1, 159+4-1+1, 25-1+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle1BottomRGB[1][Data::g_skinstyle]);
        rect1 = CRect(9-2, 25-2, 159+4-2+1, 25-2+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle1BottomRGB[2][Data::g_skinstyle]);
        rect1 = CRect(9-3, 25-3, 159+4-2+1, 25-3+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle1BottomRGB[3][Data::g_skinstyle]);

        //画左边Title left
        rect1 = CRect(0, 0, 0+1, 103+4+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle1LeftRGB[0][Data::g_skinstyle]);
        rect1 = CRect(0+1, 0+1, 0+1+1, 103+4-1+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle1LeftRGB[1][Data::g_skinstyle]);
        rect1 = CRect(0+2, 0+2, 0+2+1, 103+4-2+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle1LeftRGB[2][Data::g_skinstyle]);
        rect1 = CRect(0+3, 0+3, 0+3+1, 103+4-3+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle1LeftRGB[3][Data::g_skinstyle]);

        //画左边Title right
        rect1 = CRect(9, 25, 9+1, 98);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle1RightRGB[0][Data::g_skinstyle]);
        rect1 = CRect(9-1, 25-1, 9-1+1, 98+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle1RightRGB[1][Data::g_skinstyle]);
        rect1 = CRect(9-2, 25-2, 9-2+1, 98+2);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle1RightRGB[2][Data::g_skinstyle]);
        rect1 = CRect(9-3, 25-3, 9-3+1, 98+3);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle1RightRGB[3][Data::g_skinstyle]);

        //画左边Title 背景
        int x = 139;
        int i;
        for(i = 4; i < 22; i++)
        {
            rect1 = CRect(4, i, (x)+1, i+1);
            memDC.FillSolidRect(&rect1, Data::g_partFrameTitle1BackRGB[Data::g_skinstyle]);
            x++;
        }
        x = 100;
        for(i = 4; i < 6; i++)
        {
            rect1 = CRect(i, 22, i+1, x+1);
            memDC.FillSolidRect(&rect1, Data::g_partFrameTitle1BackRGB[Data::g_skinstyle]);
            x--;
        }
        //画Tile 斜线
        CPen pen(PS_SOLID, 1, Data::g_partFrameTitle1XiexianRGB[0][Data::g_skinstyle]);
        CPen *oldpen = memDC.SelectObject(&pen);
        memDC.MoveTo(CPoint(1, 104));
        memDC.LineTo(CPoint(9, 96));
        CPen pen1(PS_SOLID, 1, Data::g_partFrameTitle1XiexianRGB[1][Data::g_skinstyle]);
        memDC.SelectObject(&pen1);
        memDC.MoveTo(CPoint(1, 105));
        memDC.LineTo(CPoint(9, 97));
        CPen pen2(PS_SOLID, 1, Data::g_partFrameTitle1XiexianRGB[2][Data::g_skinstyle]);
        memDC.SelectObject(&pen2);
        memDC.MoveTo(CPoint(1, 106));
        memDC.LineTo(CPoint(10, 97));
        CPen pen3(PS_SOLID, 1, Data::g_partFrameTitle1XiexianRGB[3][Data::g_skinstyle]);
        memDC.SelectObject(&pen3);
        memDC.MoveTo(CPoint(1, 107));
        memDC.LineTo(CPoint(10, 98));

        CPen pen4(PS_SOLID, 1, Data::g_partFrameTitle2XiexianRGB[0][Data::g_skinstyle]);
        memDC.SelectObject(&pen4);
        memDC.MoveTo(CPoint(137, 1));
        memDC.LineTo(CPoint(160, 24));
        CPen pen5(PS_SOLID, 1, Data::g_partFrameTitle2XiexianRGB[1][Data::g_skinstyle]);
        memDC.SelectObject(&pen5);
        memDC.MoveTo(CPoint(138, 1));
        memDC.LineTo(CPoint(161, 24));
        CPen pen6(PS_SOLID, 1, Data::g_partFrameTitle2XiexianRGB[2][Data::g_skinstyle]);
        memDC.SelectObject(&pen6);
        memDC.MoveTo(CPoint(139, 1));
        memDC.LineTo(CPoint(162, 24));
        CPen pen7(PS_SOLID, 1, Data::g_partFrameTitle2XiexianRGB[3][Data::g_skinstyle]);
        memDC.SelectObject(&pen7);
        memDC.MoveTo(CPoint(140, 1));
        memDC.LineTo(CPoint(163, 24));
        memDC.SelectObject(oldpen);

        //画右边Title top
        rect1 = CRect(140, 0, 414+1, 0+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2TopRGB[0][Data::g_skinstyle]);
        rect1 = CRect(140+1, 1, 414-1+1, 1+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2TopRGB[1][Data::g_skinstyle]);
        rect1 = CRect(140+2, 2, 414-2+1, 2+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2TopRGB[2][Data::g_skinstyle]);
        rect1 = CRect(140+3, 3, 414-3+1, 3+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2TopRGB[3][Data::g_skinstyle]);

        //画右边Title bottom
        rect1 = CRect(164, 25, 414+1, 25+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2BottomRGB[0][Data::g_skinstyle]);
        rect1 = CRect(164-1, 25-1, 414-1+1, 25-1+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2BottomRGB[1][Data::g_skinstyle]);
        rect1 = CRect(164-2, 25-2, 414-2+1, 25-2+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2BottomRGB[2][Data::g_skinstyle]);
        rect1 = CRect(164-3, 25-3, 414-3+1, 25-3+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2BottomRGB[3][Data::g_skinstyle]);

        //画右边Title right
        rect1 = CRect(414, 0, 414+1, 25+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2RightRGB[0][Data::g_skinstyle]);
        rect1 = CRect(414-1, 1, 414-1+1, 25-1+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2RightRGB[0][Data::g_skinstyle]);   //1
        rect1 = CRect(414-2, 2, 414-2+1, 25-2+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2RightRGB[2][Data::g_skinstyle]);
        rect1 = CRect(414-3, 3, 414-3+1, 25-3+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2RightRGB[3][Data::g_skinstyle]);

        //画右边背景
        //画右边Title 背景
        x = 144;
        for(i = 4; i < 22; i++)
        {
            rect1 = CRect(x++, i, 410+1, i+1);
            memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2BackRGB[Data::g_skinstyle]);
        }
        //画线
        rect1 = CRect(150, 10, 387+1, 10+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2LineRGB[0][Data::g_skinstyle]);
        rect1 = CRect(151, 11, 387+1, 11+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2LineRGB[1][Data::g_skinstyle]);
        rect1 = CRect(153, 13, 387+1, 13+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2LineRGB[0][Data::g_skinstyle]);
        rect1 = CRect(154, 14, 387+1, 14+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2LineRGB[1][Data::g_skinstyle]);
        rect1 = CRect(156, 16, 387+1, 16+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2LineRGB[0][Data::g_skinstyle]);
        rect1 = CRect(157, 17, 387+1, 17+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameTitle2LineRGB[1][Data::g_skinstyle]);

        //画主窗口的框
        //left
        rect1 = CRect(0, 108, 0+1, 143+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameMainLeftRGB[0][Data::g_skinstyle]);
        rect1 = CRect(0+1, 108-1, 0+1+1, 143-1+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameMainLeftRGB[1][Data::g_skinstyle]);
        rect1 = CRect(0+2, 108-2, 0+2+1, 143-2+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameMainLeftRGB[2][Data::g_skinstyle]);
        rect1 = CRect(0+3, 108-3, 0+3+1, 143-3+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameMainLeftRGB[3][Data::g_skinstyle]);

        //bottom
        rect1 = CRect(0, 143, 414+1, 143+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameMainBottomRGB[0][Data::g_skinstyle]);
        rect1 = CRect(0+1, 143-1, 414-1+1, 143-1+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameMainBottomRGB[1][Data::g_skinstyle]);
        rect1 = CRect(0+2, 143-2, 414-2+1, 143-2+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameMainBottomRGB[2][Data::g_skinstyle]);
        rect1 = CRect(0+3, 143-3, 414-3+1, 143-3+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameMainBottomRGB[3][Data::g_skinstyle]);
        rect1 = CRect(0+4, 143-4, 414-4+1, 143-4+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameMainBottomRGB[4][Data::g_skinstyle]);
        //right
        rect1 = CRect(414, 26, 414+1, 143+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameMainRightRGB[0][Data::g_skinstyle]);
        rect1 = CRect(414-1, 26, 414-1+1, 143-1+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameMainRightRGB[0][Data::g_skinstyle]);  //1
        rect1 = CRect(414-2, 26, 414-2+1, 143-2+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameMainRightRGB[2][Data::g_skinstyle]);
        rect1 = CRect(414-3, 26, 414-3+1, 143-3+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameMainRightRGB[3][Data::g_skinstyle]);
        rect1 = CRect(414-4, 26, 414-4+1, 143-4+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameMainRightRGB[4][Data::g_skinstyle]);

        //画InRect
        //top
        rect1 = CRect(15, 31, 333+1, 31+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectTopRGB[0][Data::g_skinstyle]);
        rect1 = CRect(15-1, 31-1, 333+1+1, 31-1+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectTopRGB[1][Data::g_skinstyle]);
        rect1 = CRect(15-2, 31-2, 333+2+1, 31-2+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectTopRGB[2][Data::g_skinstyle]);
        rect1 = CRect(11, 27, 409+1, 27+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectTopRGB[3][Data::g_skinstyle]);

        //bottom
        rect1 = CRect(15, 134, 333+1, 134+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectBottomRGB[0][Data::g_skinstyle]);
        rect1 = CRect(15-1, 134+1, 333+1+1, 134+1+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectBottomRGB[1][Data::g_skinstyle]);
        rect1 = CRect(15-2, 134+2, 333+2+1, 134+2+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectBottomRGB[2][Data::g_skinstyle]);

        //left
        rect1 = CRect(15, 32,15+1, 133+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectLeftRGB[0][Data::g_skinstyle]);
        rect1 = CRect(15-1, 32-1, 15-1+1, 133+1+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectLeftRGB[1][Data::g_skinstyle]);
        rect1 = CRect(15-2, 32-2, 15-2+1, 133+2+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectLeftRGB[2][Data::g_skinstyle]);
        rect1 = CRect(11, 28, 11+1, 98+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectLeftRGB[3][Data::g_skinstyle]);

        //right
        rect1 = CRect(329, 32, 329+1, 133+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectRightRGB[0][Data::g_skinstyle]);
        rect1 = CRect(329+1, 32, 329+1+1, 133+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectRightRGB[1][Data::g_skinstyle]);
        rect1 = CRect(329+2, 32, 329+2+1, 133+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectRightRGB[2][Data::g_skinstyle]);
        rect1 = CRect(329+3, 32, 329+3+1, 133+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectRightRGB[2][Data::g_skinstyle]);
        rect1 = CRect(333, 32, 333+1, 133+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectLeftRGB[0][Data::g_skinstyle]);
        rect1 = CRect(333+1, 32-1, 333+1+1, 133+1+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectLeftRGB[1][Data::g_skinstyle]);
        rect1 = CRect(333+2, 32-2, 333+2+1, 133+2+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectLeftRGB[2][Data::g_skinstyle]);

        //背景色
        rect1 = CRect(16, 32, 328+1, 133+1);
        memDC.FillSolidRect(&rect1, Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

        //在背景上画上文字
        for(int nIndex = 0; nIndex < m_ntxtCount; nIndex++)
        {
            VERIFY(m_Font.CreateFont(
                m_pTxtStruct[nIndex].txtFontSize,                        // nHeight
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
                _T("宋体")));              // lpszFacename

            rect1= m_pTxtStruct[nIndex].txtRect;
            CFont *oldfont = memDC.SelectObject(&m_Font);

            int oldMode = memDC.SetBkMode(TRANSPARENT);
            int oldcolor = memDC.SetTextColor(m_pTxtStruct[nIndex].txtColor);
            CString s = Util::StringOp::ToCString(m_pTxtStruct[nIndex].sTxt);
            memDC.DrawText(s, &rect1, DT_WORDBREAK |m_pTxtStruct[nIndex].sAlign);

            memDC.SetTextColor(oldcolor);
            memDC.SetBkMode(oldMode);
            memDC.SelectObject(oldfont);
            m_Font.DeleteObject();
        }

        pdc->BitBlt(rt.left, rt.top, rt.Width(), rt.Height(), &memDC, rt.left, rt.top, SRCCOPY);

        memDC.DeleteDC();   
        bmp.DeleteObject();

        ReleaseDC(pdc);
    }

    void CCEFramePartStatic::SetSkinStyle()
    {

    }
    void CCEFramePartStatic::SetTextStruct(TextStruct *pTxtStruct, int ncount, BOOL reDraw)
    {
        if(m_pTxtStruct)
        {
            delete[] m_pTxtStruct;
        }
        m_ntxtCount = ncount;
        m_pTxtStruct  = new TextStruct[ncount];
        memset(m_pTxtStruct, 0, sizeof(TextStruct) * m_ntxtCount);
        memcpy(m_pTxtStruct, pTxtStruct, sizeof(TextStruct) * m_ntxtCount);
        if(reDraw)
            Invalidate();
    }

    void CCEFramePartStatic ::ChangeTextStruct(int nIndex, TextStruct txtStruct, BOOL reDraw)
    {
        if(nIndex < m_ntxtCount)
        {
            memcpy(&m_pTxtStruct[nIndex], &txtStruct, sizeof(TextStruct));
        }
        if(reDraw)
        {
            InvalidateRect(&m_pTxtStruct[nIndex].txtRect);
        }
    }
}
