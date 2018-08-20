// CEDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CEDialog.h"
#include "../resource.h"
#include "../Data/SkinStyle.h"
#include "../MultimediaPhone.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace Control {
    /////////////////////////////////////////////////////////////////////////////
    // CCEDialog dialog
    void CCEDialog::ShowWindow_(int nCmdShow)
    {
        if(nCmdShow == SW_HIDE)
            m_bIsFreshMemDC = TRUE;   //test 20071204
        ShowWindow(nCmdShow);
    }

    void CCEDialog ::OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized )
    {
        if(nState == 0)
        {
            /*
            if(theApp.inputDlg_->IsWindowVisible())  //show
            {
            theApp.inputDlg_->hide();
            }
            */
        }
    }

    CCEDialog::CCEDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
        : CDialog(nIDTemplate, pParent)
    {
        //{{AFX_DATA_INIT(CCEDialog)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
        m_Ownerhandle = NULL;
        m_bMemDCCrate = FALSE;
        m_bIsFreshMemDC = FALSE;   //缺省不刷新
    }


    void CCEDialog::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CCEDialog)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }

    BOOL CCEDialog::OnEraseBkgnd(CDC* pDC) 
    {
        // TODO: Add your message handler code here and/or call default
        return TRUE;
        //	return CDialog::OnEraseBkgnd(pDC);
    }

    BEGIN_MESSAGE_MAP(CCEDialog, CDialog)
        //{{AFX_MSG_MAP(CCEDialog)
        ON_WM_PAINT()
        ON_WM_ACTIVATE()
        ON_WM_ERASEBKGND()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CCEDialog message handlers

    BOOL DrawTransparentBmp(HDC hdc, HBITMAP hbmp, RECT &rect, COLORREF colorTrans)
    {
        HDC dcImage, dcTrans, dcImage24;
        HBITMAP holdbmp24, hbmp24;
        HBITMAP holdbmp;
        HBITMAP hbmpTrans, holdbmpTrans;

        // 创建内存DC
        dcImage = CreateCompatibleDC(hdc);
        dcTrans =  CreateCompatibleDC(hdc);
        dcImage24 = CreateCompatibleDC(hdc);

        if (dcImage == NULL || dcTrans == NULL || dcImage24 == NULL)
            // Error: can't create compatible dc
            return FALSE;
        // 获得图像属性
        BITMAP bmp;
        GetObject(hbmp, sizeof(bmp), &bmp);

        // 选择图片到dcImage中
        holdbmp = (HBITMAP)SelectObject(dcImage, hbmp);

        // 创建24色位图，这样才能正确地显示带透明色的位图
        LPBITMAPINFO lpBmpInfo;

        lpBmpInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFOHEADER)];

        lpBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        lpBmpInfo->bmiHeader.biPlanes = 1; 
        lpBmpInfo->bmiHeader.biBitCount = 24; 
        lpBmpInfo->bmiHeader.biCompression = BI_RGB; 
        lpBmpInfo->bmiHeader.biSizeImage = 0; 
        lpBmpInfo->bmiHeader.biClrUsed = 0; 

        lpBmpInfo->bmiHeader.biWidth = bmp.bmWidth;
        lpBmpInfo->bmiHeader.biHeight = bmp.bmHeight;

        HDC dc = CreateCompatibleDC(NULL);

        // 创建新图片
        LPVOID lpBits;
        hbmp24 =::CreateDIBSection(dc,lpBmpInfo,DIB_RGB_COLORS,
            &lpBits,NULL,0);

        DeleteDC(dc);
        delete lpBmpInfo;

        if (hbmp24 == NULL)
            // Error
            return FALSE;

        holdbmp24 = (HBITMAP)SelectObject(dcImage24, hbmp24);

        // 将原图片绘制到24色位图中
        BitBlt(dcImage24, 0, 0, bmp.bmWidth, bmp.bmHeight, dcImage, 0, 0, SRCCOPY);

        // 创建Mask位图
        hbmpTrans = CreateBitmap(bmp.bmWidth, bmp.bmHeight, 1, 1, NULL);
        if (hbmpTrans == NULL)
            // Error
            return FALSE;

        // 选择mask位图到dcTrans中
        holdbmpTrans = (HBITMAP)SelectObject(dcTrans, hbmpTrans);

        // 创建掩码图像（基于指定的颜色）
        COLORREF oldbkcolor = SetBkColor(dcImage24, colorTrans);
        BitBlt(dcTrans, 0, 0, bmp.bmWidth, bmp.bmHeight, dcImage24, 0, 0, SRCCOPY);

        SetBkColor(dcImage24, RGB(0,0,0));
        COLORREF oldtextcolor = SetTextColor(dcImage24, RGB(255,255,255));
        BitBlt(dcImage24, 0, 0, bmp.bmWidth, bmp.bmHeight, dcTrans, 0, 0, SRCAND);

        // 去除指定的颜色
        COLORREF crOldBack, crOldText;
        crOldBack = SetBkColor(hdc, RGB(192,192,192));
        crOldText = SetTextColor(hdc, RGB(0,0,0));

        // 显示透明位图
        StretchBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, dcTrans, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCAND);
        StretchBlt(hdc, rect.left, rect.top, rect.right - rect.left,rect.bottom - rect.top, dcImage24, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCPAINT);

        // 恢复设置及释放资源
        SelectObject(dcImage, holdbmp);

        SelectObject(dcImage24, holdbmp24);
        SelectObject(dcTrans, holdbmpTrans);

        DeleteObject(hbmp24);
        DeleteObject(hbmpTrans);

        SetBkColor(hdc, crOldBack);
        SetTextColor(hdc, crOldText);

        SetBkColor(dcImage24, oldbkcolor);
        SetTextColor(dcImage24, oldtextcolor);

        DeleteDC(dcImage);
        DeleteDC(dcImage24);
        DeleteDC(dcTrans);

        return TRUE;
    }

    void CCEDialog::OnPaint() 
    {
        //	CPaintDC dc(this); // device context for painting
        //CDialog::OnPaint();
        // TODO: Add your message handler code here
        // 	DWORD t, a;
        // 	int ret1 = DMemprintf("CEDialog 0", t, a);

        static HDC hScrDC;   
        static HDC hMemDC;
        static HBITMAP directBmp = NULL;

        if(m_bMemDCCrate && !m_bIsFreshMemDC)
        {
            CDialog::OnPaint();

            HDC pdc = ::GetDC(m_hWnd);
            CRect rt;
            GetClientRect(&rt);
            BitBlt(pdc, 0, 0, rt.Width()-1, rt.Height()-1, hMemDC, 0, 0, SRCCOPY);
            ::ReleaseDC(m_hWnd, pdc);
        }
        else
        {
            int  width, height; 

            width = GetDeviceCaps(hScrDC, HORZRES);
            height = GetDeviceCaps(hScrDC, VERTRES); 

            if(directBmp == NULL)
            {
                hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL); 
                hMemDC = CreateCompatibleDC(hScrDC); 

                BYTE  *lpBitmapBits = NULL;
                BITMAPINFO RGB24BitsBITMAPINFO; 
                ZeroMemory(&RGB24BitsBITMAPINFO, sizeof(BITMAPINFO));
                RGB24BitsBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                RGB24BitsBITMAPINFO.bmiHeader.biWidth = width;
                RGB24BitsBITMAPINFO.bmiHeader.biHeight = height;
                RGB24BitsBITMAPINFO.bmiHeader.biPlanes = 1;
                RGB24BitsBITMAPINFO.bmiHeader.biBitCount = 24;

                directBmp = CreateDIBSection(hMemDC, (BITMAPINFO*)&RGB24BitsBITMAPINFO, DIB_RGB_COLORS, (void **)&lpBitmapBits, NULL, 0);
                SelectObject(hMemDC, directBmp); 
            }

            m_bMemDCCrate = TRUE;
            m_bIsFreshMemDC = FALSE;

            BitBlt(hMemDC, 0, 0, width, height, hScrDC, 0, 0, SRCCOPY);

            CDialog::OnPaint();

            HDC pdc = ::GetDC(m_hWnd);
            CRect rt;
            GetClientRect(&rt);
            BitBlt(pdc, 0, 0, rt.Width()-1, rt.Height()-1, hMemDC, 0, 0, SRCCOPY);
            ::ReleaseDC(m_hWnd, pdc);
        }

        // 	int ret2 = DMemprintf("CEDialog 1", t, a);
        // 	if(ret1 != ret2)
        // 	{
        // 		Dprintf("ret = %d\r\n", ret1-ret2 );
        // 	}
        // Do not call CDialog::OnPaint() for painting messages
    }


    CCEAllFrameDialog::CCEAllFrameDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
        : CDialog(nIDTemplate, pParent)
    {
        //{{AFX_DATA_INIT(CCEDialog)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
        m_nStyle = 0;
    }


    void CCEAllFrameDialog::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CCEDialog)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CCEAllFrameDialog, CDialog)
        //{{AFX_MSG_MAP(CCEDialog)
        ON_WM_PAINT()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CCEDialog message handlers

    void CCEAllFrameDialog::OnPaint() 
    {
        //CPaintDC dc(this); // device context for painting
        CDialog::OnPaint();
        // TODO: Add your message handler code here
        CDC *pdc = GetDC();
        CRect rt;
        GetClientRect(&rt);

        BOOL bHalfwindow = FALSE;
        if(rt.Width() < 480)
            bHalfwindow = TRUE;

        //画top线
        CRect rect1= rt;
        rect1 = CRect(0, 0, 480-1+1, 0+1);
        pdc->FillSolidRect(&rect1, Data::g_allFramTopLineRGB[0][Data::g_skinstyle]);
        rect1 = CRect(1, 1, 480-2+1, 1+1);
        pdc->FillSolidRect(&rect1, Data::g_allFramTopLineRGB[1][Data::g_skinstyle]);
        rect1 = CRect(2, 2, 480-3+1, 2+1);
        pdc->FillSolidRect(&rect1, Data::g_allFramTopLineRGB[2][Data::g_skinstyle]);
        rect1 = CRect(3, 3, 480-4+1, 3+1);
        pdc->FillSolidRect(&rect1, Data::g_allFramTopLineRGB[3][Data::g_skinstyle]);

        if(m_nStyle == 0)
        {
            rect1 = CRect(1, 4, 480-2+1, 12+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramTopLineRGB[4][Data::g_skinstyle]);
            rect1 = CRect(4, 13, 480-5+1, 13+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramBackRGB[Data::g_skinstyle]);
        }

        //画bottom线
        if(!bHalfwindow)
        {
            rect1 = CRect(0, 204-1, 480-1+1, 204-1+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramBottomLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(1, 204-2, 480-2+1, 204-2+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramBottomLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(2, 204-3, 480-3+1, 204-3+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramBottomLineRGB[2][Data::g_skinstyle]);
        }

        //画left线
        rect1 = CRect(0, 0, 0+1, 204-1+1);
        pdc->FillSolidRect(&rect1, Data::g_allFramLeftLineRGB[0][Data::g_skinstyle]);
        rect1 = CRect(1, 1, 1+1, 204-2+1);
        pdc->FillSolidRect(&rect1, Data::g_allFramLeftLineRGB[1][Data::g_skinstyle]);
        rect1 = CRect(2, 2, 2+1, 204-3+1);
        pdc->FillSolidRect(&rect1, Data::g_allFramLeftLineRGB[2][Data::g_skinstyle]);
        rect1 = CRect(3, 2, 3+1, 204-4+1);
        pdc->FillSolidRect(&rect1, Data::g_allFramLeftLineRGB[3][Data::g_skinstyle]);

        //画right线
        if(!bHalfwindow)
        {
            rect1 = CRect(480-1, 0, 480-1+1, 204-1+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramRightLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(480-2, 1, 480-2+1, 204-2+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramRightLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(480-3, 2, 480-3+1, 204-3+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramRightLineRGB[2][Data::g_skinstyle]);
            rect1 = CRect(480-4, 2, 480-4+1, 204-4+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramRightLineRGB[3][Data::g_skinstyle]);
        }


        if(m_nStyle == 0)
        {
            //最后画一条Top线
            rect1 = CRect(1, 14, 480-2+1, 14+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramTopLineRGB[4][Data::g_skinstyle]);

            //画背景
            rect1 = CRect(4, 15, 480-5+1, 204-4+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramBackRGB[Data::g_skinstyle]);
        }
        else if(m_nStyle == 1 || m_nStyle == 2)
        {
            //画背景
            rect1 = CRect(4, 4, 480-4+1, 204-4+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramBackRGB[Data::g_skinstyle]);
        }

        //当 m_nStyle = 1时，画三角形及中间框
        if(m_nStyle == 1)
        {
            //画top
            rect1 = CRect(293, 0, 480-1+1, 0+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramAngleTopLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(294, 1, 480-2+1, 1+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramAngleTopLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(295-2, 2, 480-3+1, 2+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramAngleTopLineRGB[2][Data::g_skinstyle]);
            rect1 = CRect(295-2, 3, 480-4+1, 3+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramAngleTopLineRGB[3][Data::g_skinstyle]);
            rect1 = CRect(295-2, 4, 480-5+1, 4+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramAngleTopLineRGB[4][Data::g_skinstyle]);

            //画right
            rect1 = CRect(480-1, 0, 480-1+1, 188-1+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramAngleRightLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(480-2, 1, 480-2+1, 188-2+1+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramAngleRightLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(480-3, 2, 480-3+1, 188-3+1+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramAngleRightLineRGB[2][Data::g_skinstyle]);
            rect1 = CRect(480-4, 3, 480-4+1, 188-4+1+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramAngleRightLineRGB[3][Data::g_skinstyle]);
            rect1 = CRect(480-5, 4, 480-5+1, 188-5+1+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramAngleRightLineRGB[4][Data::g_skinstyle]);

            int x = 296;
            for(int i = 5; i <= 184; i++)
            {
                rect1 = CRect(x++, i, 475+1, i+1);
                pdc->FillSolidRect(&rect1, Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);
            }

            //画三角形
            CPen pen(PS_SOLID, 1, Data::g_allFramAngleXiexianLineRGB[0][Data::g_skinstyle]);
            CPen *oldpen = pdc->SelectObject(&pen);
            pdc->MoveTo(CPoint(292, 2));
            pdc->LineTo(CPoint(478, 188));
            CPen pen1(PS_SOLID, 1, Data::g_allFramAngleXiexianLineRGB[1][Data::g_skinstyle]);
            pdc->SelectObject(&pen1);
            pdc->MoveTo(CPoint(291, 2));
            pdc->LineTo(CPoint(477, 188));
            pdc->SelectObject(oldpen);

            rect1 = CRect(303, 10, 448+1, 10+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramAngleHerLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(304, 11, 448+1, 11+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramAngleHerLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(306, 13, 448+1, 13+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramAngleHerLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(307, 14, 448+1, 14+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramAngleHerLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(310, 16, 448+1, 16+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramAngleHerLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(311, 17, 448+1, 17+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramAngleHerLineRGB[1][Data::g_skinstyle]);

            //画内框
            //画top
            rect1 = CRect(8, 26, 407+1, 26+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectTopLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(8-1, 26-1, 407+1+1, 26+1-1);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectTopLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(8-2, 26-2, 407+1+2, 26+1-2);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectTopLineRGB[2][Data::g_skinstyle]);

            //画bottom
            rect1 = CRect(8, 197, 407+1, 197+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectBottomLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(8-1, 197+1, 407+1+1, 197+1+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectBottomLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(8-2, 197+2, 407+1+2, 197+1+2);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectBottomLineRGB[2][Data::g_skinstyle]);

            //画left
            rect1 = CRect(7, 26, 7+1, 197+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectLeftLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(7-1, 26-1, 7+1-1, 197+1+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectLeftLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(7-2, 26-2, 7+1-2, 197+1+2);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectLeftLineRGB[2][Data::g_skinstyle]);

            //画right
            rect1 = CRect(408, 26, 408+1, 197+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectRightLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(408+1, 26-1, 408+1+1, 197+1+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectRightLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(408+2, 26-2, 408+1+2, 197+1+2);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectRightLineRGB[2][Data::g_skinstyle]);

            //画Angletop
            rect1 = CRect(316, 26, 407+1, 26+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectAngleTopLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(316-1, 26-1, 407+1+1, 26+1-1);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectAngleTopLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(316-2, 26-2, 407+1+2, 26+1-2);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectAngleTopLineRGB[2][Data::g_skinstyle]);

            //画Angleright
            rect1 = CRect(408, 26, 408+1, 118+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectAngleRightLineRGB[0][Data::g_skinstyle]);
            rect1 = CRect(408+1, 26-1, 408+1+1, 118+1+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectAngleRightLineRGB[1][Data::g_skinstyle]);
            rect1 = CRect(408+2, 26-2, 408+1+2, 118+1+2);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectAngleRightLineRGB[2][Data::g_skinstyle]);

            //画InRect
            rect1 = CRect(8, 27, 407+1, 196+1);
            pdc->FillSolidRect(&rect1, Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        }

        //画上小框
        if(bHalfwindow && m_InRect.Width() != 0)
        {
            CRect rrt = m_InRect;
            rect1 = CRect(rrt.left, rrt.top, rrt.right+1, rrt.top+1);
            pdc->FillSolidRect(&rect1, Data::g_allInRectLine1RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.bottom, rrt.right+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_allInRectLine1RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.top, rrt.left+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_allInRectLine1RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.right, rrt.top, rrt.right+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_allInRectLine1RGB[1][Data::g_skinstyle]);

            rrt.DeflateRect(1,1);
            rect1 = CRect(rrt.left, rrt.top, rrt.right+1, rrt.top+1);
            pdc->FillSolidRect(&rect1, Data::g_allInRectLine2RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.bottom, rrt.right+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_allInRectLine2RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.top, rrt.left+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_allInRectLine2RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.right, rrt.top, rrt.right+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_allInRectLine2RGB[1][Data::g_skinstyle]);
        }

        //通话记录 名片主窗口
        if(m_nStyle == 2)
        {
            CRect rrt = CRect(7, 26, 472, 197);
            rect1 = CRect(rrt.left, rrt.top, rrt.right+1, rrt.top+1);
            pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.bottom, rrt.right+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine2RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.top, rrt.left+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);
            rect1 = CRect(rrt.right, rrt.top, rrt.right+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine2RGB[0][Data::g_skinstyle]);

            rrt.DeflateRect(-1,-1);
            rect1 = CRect(rrt.left, rrt.top, rrt.right+1, rrt.top+1);
            pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine1RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.bottom, rrt.right+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine2RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.top, rrt.left+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine1RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.right, rrt.top, rrt.right+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine2RGB[1][Data::g_skinstyle]);

            rrt.DeflateRect(-1,-1);
            rect1 = CRect(rrt.left, rrt.top, rrt.right+1, rrt.top+1);
            pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine1RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.bottom, rrt.right+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine2RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.left, rrt.top, rrt.left+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine1RGB[1][Data::g_skinstyle]);
            rect1 = CRect(rrt.right, rrt.top, rrt.right+1, rrt.bottom+1);
            pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine2RGB[1][Data::g_skinstyle]);
        }
        ReleaseDC(pdc);

        // Do not call CDialog::OnPaint() for painting messages
    }
}
