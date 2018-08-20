// SoftKey.cpp : implementation file
//

#include "stdafx.h"
#include "SoftKey.h"
#include "../Data/LanguageResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

namespace Control {
#define IDC_REG_TIMER		0x1000

#define TOGGLEBMP_WIDTH	90
#define MARGIN_PIXEL	2
#define MARGINDOWN_PIXEL 4
#define ICON_PIXEL		16

    //手写
    static int m_nWriteLinePoint;			//记录当前点数
    static int m_nWriteLineBuff;			//记录当前记录的BUFF
    static int m_nRecognizeBuff;			//记录当前识别的BUFF
    static WriteLineBuff *m_pWriteLineBuff;	//buff
    static HANDLE hWriteThread = NULL;
    //手写相关函数
    static void HwWritePrc(void); 

    void InitWriteLineBuff();
    void AddWriteLineBuff(CPoint pt);
    void AddIndexBuff();
    void FreeWriteLineBuff();

    void TransparentBlt2( HDC hdcDest,      // 目标DC
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
        )
    {
        HBITMAP hOldImageBMP, hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);	// 创建兼容位图
        HBITMAP hOldMaskBMP, hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);			// 创建单色掩码位图
        HDC		hImageDC = CreateCompatibleDC(hdcDest);
        HDC		hMaskDC = CreateCompatibleDC(hdcDest);
        hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);
        hOldMaskBMP = (HBITMAP)SelectObject(hMaskDC, hMaskBMP);

        // 将源DC中的位图拷贝到临时DC中
        if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
            BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
        else
            StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, 
            hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);

        // 设置透明色
        SetBkColor(hImageDC, crTransparent);

        // 生成透明区域为白色，其它区域为黑色的掩码位图
        BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);

        // 生成透明区域为黑色，其它区域保持不变的位图
        SetBkColor(hImageDC, RGB(0,0,0));
        SetTextColor(hImageDC, RGB(255,255,255));
        BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

        // 透明部分保持屏幕不变，其它部分变成黑色
        SetBkColor(hdcDest,RGB(0xff,0xff,0xff));
        SetTextColor(hdcDest,RGB(0,0,0));
        BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

        // "或"运算,生成最终效果
        BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCPAINT);

        SelectObject(hImageDC, hOldImageBMP);
        DeleteDC(hImageDC);
        SelectObject(hMaskDC, hOldMaskBMP);
        DeleteDC(hMaskDC);
        DeleteObject(hImageBMP);
        DeleteObject(hMaskBMP);

    }

    void PutIconPartial(HDC pdc, int x, int y, int iNO, CRect r)
    {
        if(iNO <= 0)
            return;

        HDC memDC = CreateCompatibleDC(pdc);//创建内存设备上下文
        HBITMAP hBmp = LoadBitmap( AfxGetResourceHandle(), MAKEINTRESOURCE(iNO));
        CRect rect;
        HBITMAP oldbitmap;
        oldbitmap = (HBITMAP)SelectObject(memDC, hBmp);//将位图选入当前设备场景
        BitBlt(pdc, x, y, r.Width (), r.Height(), memDC, r.left, r.top, SRCCOPY);//将内存场景的图象拷贝到屏幕上
        SelectObject (memDC, oldbitmap);
    }

    void PutIconPartial(CDC *pdc, int x, int y, int iNO, CRect r)
    {
        if(iNO <= 0)
            return;

        CDC memDC;//内存设备场景
        CBitmap cb;//位图类对象
        CRect rect;
        cb.LoadBitmap (iNO);//载入资源
        memDC.CreateCompatibleDC (pdc);//创建内存设备上下文
        CBitmap *oldbitmap=NULL;
        oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
        pdc->BitBlt(x, y, r.Width (), r.Height(), &memDC, r.left, r.top, SRCCOPY);//将内存场景的图象拷贝到屏幕上
        memDC.SelectObject (oldbitmap);
        cb.DeleteObject();
    }

    /////////////////////////////////////////////////////////////////////////////
    // CSoftKey

    CSoftKey::CSoftKey()
    {
        m_pWriteLineBuff = NULL;
    }

    CSoftKey::~CSoftKey()
    {
    }


    BEGIN_MESSAGE_MAP(CSoftKey, CStatic)
        //{{AFX_MSG_MAP(CSoftKey)
        ON_WM_LBUTTONDOWN()
        ON_WM_LBUTTONUP()
        ON_WM_MOUSEMOVE()
        ON_WM_CREATE()
        ON_WM_PAINT()
        ON_WM_TIMER()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CSoftKey message handlers

    void CSoftKey::OnLButtonDown(UINT nFlags, CPoint point) 
    {
        // TODO: Add your message handler code here and/or call default
        INT32 sel = GetPenKey(point);

        if(	m_bHaveHandWrite)
        {
            if((m_nHWPenDown = FindHWRect_(point)) > 0)
            {
                if(m_nOldHWPenDown > 0 && m_nOldHWPenDown!= m_nHWPenDown)
                {
                    //笔迹采集结束.开始识别
                    //todo:
                    AddIndexBuff();   //创建新的buff

                    KillTimer(IDC_REG_TIMER);

                    extern void PutIconPartial(CDC *pdc, int x, int y, int iNO, CRect r);
                    CDC *pDC = GetDC();
                    CRect rect = m_HWRect[m_nOldHWPenDown-1];
                    rect.DeflateRect(-2, -2);
                    PutIconPartial(pDC, rect.left, rect.top, m_BitmapID, rect);
                    ReleaseDC(pDC);

                    m_OldPoint = point;
                    m_PrePoint[0] = CPoint(0xFFFF, 0xFFFF);
                    m_PrePoint[1] = CPoint(0xFFFF, 0xFFFF);
                    m_PrePoint[2] = CPoint(0xFFFF, 0xFFFF);

                    m_nOldHWPenDown = m_nHWPenDown;
                }
                else if(m_nOldHWPenDown == 0)	//开始记录新的采样
                {
                    m_OldPoint = point;
                    m_PrePoint[0] = CPoint(0xFFFF, 0xFFFF);
                    m_PrePoint[1] = CPoint(0xFFFF, 0xFFFF);
                    m_PrePoint[2] = CPoint(0xFFFF, 0xFFFF);

                    m_nOldHWPenDown = m_nHWPenDown;

                }
                else							//继续采集
                {
                    m_OldPoint = point;
                    m_PrePoint[0] = CPoint(0xFFFF, 0xFFFF);
                    m_PrePoint[1] = CPoint(0xFFFF, 0xFFFF);
                    m_PrePoint[2] = CPoint(0xFFFF, 0xFFFF);
                    KillTimer(IDC_REG_TIMER);
                }
            }
        }

        if(sel == m_nCurSel)
            return;
        CDC *pdc = GetDC();
        if(m_nCurSel >= 0)
        {
            pdc->InvertRect(m_pKeys[m_nCurSel].sKeyRect);
            m_nCurSel = -1;
        }
        if(sel >= 0)
        {
            pdc->InvertRect(m_pKeys[sel].sKeyRect);
            m_nCurSel = sel;
            m_nLocal = m_nCurSel;
        }

        CStatic::OnLButtonDown(nFlags, point);
    }

    void CSoftKey::OnLButtonUp(UINT nFlags, CPoint point) 
    {
        // TODO: Add your message handler code here and/or call default
        if(m_nCurSel >= 0)
        {
            CDC *pdc = GetDC();
            pdc->InvertRect(m_pKeys[m_nCurSel].sKeyRect);
            ReleaseDC(pdc);
            //SendMessage(WM_SOFTKEY, GetDlgCtrlID(), 0);
            //NMHDR hm = {this,CN_COMMAND,UINT32(m_pKeys[sel].nKeyValue),sel};
            this->GetParent()->SendMessage(WM_SOFTKEY, UINT32(m_pKeys[m_nCurSel].nKeyValue), GetDlgCtrlID());
            m_nCurSel = -1;
        }

        if(	m_bHaveHandWrite)
        {
            if((m_nHWPenDown = FindHWRect_(point)) > 0 && m_nOldHWPenDown > 0)
            {
                if(m_nOldHWPenDown!= m_nHWPenDown)
                {
                    extern void PutIconPartial(CDC *pdc, int x, int y, int iNO, CRect r);
                    CDC *pDC = GetDC();
                    CRect rect = m_HWRect[m_nOldHWPenDown-1];
                    rect.DeflateRect(-2, -2);
                    PutIconPartial(pDC, rect.left, rect.top, m_BitmapID, rect);
                    ReleaseDC(pDC);
                }
                else	//设置超时识别
                {
                    //设置采集点 抬笔
                    CPoint pt;
                    pt.x = -1;
                    pt.y = 0;
                    AddWriteLineBuff(pt);

                    SetTimer(IDC_REG_TIMER, m_RegTimer, NULL);
                }
            }
        }

        CStatic::OnLButtonUp(nFlags, point);
    }

    void CSoftKey::OnMouseMove(UINT nFlags, CPoint point) 
    {
        // TODO: Add your message handler code here and/or call default
        if(	m_bHaveHandWrite && nFlags == MK_LBUTTON)
        {
            if(((m_nHWPenDown = FindHWRect_(point)) > 0) && m_nOldHWPenDown > 0)
            {
                if(m_nHWPenDown == m_nOldHWPenDown)	//在同一个框中写子
                {
                    KillTimer(IDC_REG_TIMER);
                    SetTimer(IDC_REG_TIMER, m_RegTimer, NULL);

                    CPoint pt = DoWithWrite(point);
                    //采点
                    AddWriteLineBuff(pt);

                    CPen pen(PS_SOLID, 8, RGB(0, 0, 0));
                    CDC	*pDC = GetDC();
                    CPen *old = pDC->SelectObject(&pen);
                    pDC->MoveTo(m_OldPoint);
                    pDC->LineTo(pt);
                    pDC->SelectObject(old);
                    m_OldPoint = pt;

                    if(pt.x == 0xFFFF || m_OldPoint.x == 0xFFFF)
                    {
                        int i = 0;
                        i++;
                    }
                    ReleaseDC(pDC);


                    // 				采点
                    // 								AddWriteLineBuff(point);
                    // 								
                    // 								CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
                    // 								CDC	*pDC = GetDC();
                    // 								CPen *old = pDC->SelectObject(&pen);
                    // 								pDC->MoveTo(m_OldPoint);
                    // 								pDC->LineTo(point);
                    // 								pDC->SelectObject(old);
                    // 								m_OldPoint = point;
                    // 								ReleaseDC(pDC);
                }
            }
        }

        CStatic::OnMouseMove(nFlags, point);
    }

#define DOWITH_WRITE		0
    CPoint CSoftKey::DoWithWrite(CPoint pt)
    {
        if(DOWITH_WRITE == 1)
        { 
            CPoint ret;
            ret = m_PrePoint[0];
            m_PrePoint[0] = m_PrePoint[1];
            m_PrePoint[1] = m_PrePoint[2];

            if(m_PrePoint[0].x != 0xFFFF || m_PrePoint[0].y != 0xFFFF)
            {
                m_PrePoint[2].x =( m_PrePoint[0].x+pt.x)/2;
                m_PrePoint[2].y =( m_PrePoint[0].y+pt.y)/2;
            }
            else
            {
                m_PrePoint[2] = pt;
            }
            if(ret.x == 0xFFFF || ret.y == 0xFFFF)
                return pt;
            return ret;
        }
        else
            return pt;	
    }

    BOOL CSoftKey::PreCreateWindow(CREATESTRUCT& cs) 
    {
        // TODO: Add your specialized code here and/or call the base class
        return CStatic::PreCreateWindow(cs);
    }

    int CSoftKey::OnCreate(LPCREATESTRUCT lpCreateStruct) 
    {
        if (CStatic::OnCreate(lpCreateStruct) == -1)
            return -1;

        // TODO: Add your specialized creation code here
        ModifyStyle(0, SS_NOTIFY);
        return 0;
    }

    void CSoftKey::OnPaint() 
    {
        CPaintDC dc(this); // device context for painting

        if(m_BitmapID <= 0)
            return;

        CDC memDC;//内存设备场景
        CBitmap cb;//位图类对象
        CRect rect;
        //GetWindowRect(&rect);
        this->GetClientRect(&rect);
        cb.LoadBitmap (m_BitmapID);//载入资源
        memDC.CreateCompatibleDC (&dc);//创建内存设备上下文
        CBitmap *oldbitmap=NULL;
        oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
        dc.BitBlt (m_pPoint.x,m_pPoint.y, rect.Width (), rect.Height(), &memDC, 0, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
        memDC.SelectObject (oldbitmap);
        cb.DeleteObject();

        // TODO: Add your message handler code here

        // Do not call CStatic::OnPaint() for painting messages
    }

    BOOL CSoftKey::Create(int		iconNO, 
        CPoint	point,
        UINT32      nKeyCount, 
        CRect     * pKeyRectArray, 
        UINT16    * pKeyValueArray, 
        CRect		  &rect,
        CWnd * pParent, 
        int style,
        UINT32      nCtrlId)
    {
        //2005.5.8
        m_BitmapID = iconNO;
        m_pPoint = point;
        m_nLocal = 0;
        m_bHaveHandWrite = FALSE;
        m_nHWPenDown = 0;
        m_nOldHWPenDown = 0;
        m_pKeys  = new KEYSTRUCT[nKeyCount];
        memset(m_pKeys, 0, sizeof(KEYSTRUCT) * nKeyCount);
        if(!m_pKeys)
            return FALSE;
        CRect ctrRect(9999, 9999, 0,0);
        for(int i=0; i < (int)nKeyCount; i++)
        {
            if(ctrRect.left > pKeyRectArray[i].left) 
                ctrRect.left = pKeyRectArray[i].left;

            if(ctrRect.top  > pKeyRectArray[i].top)
                ctrRect.top  = pKeyRectArray[i].top;

            if(ctrRect.right < pKeyRectArray[i].right)
                ctrRect.right = pKeyRectArray[i].right;

            if(ctrRect.bottom < pKeyRectArray[i].bottom)
                ctrRect.bottom = pKeyRectArray[i].bottom; 

            m_pKeys[i].sKeyRect  = pKeyRectArray[i];
            m_pKeys[i].nKeyValue = pKeyValueArray[i];
        }

        CWnd* pWnd = this;

        CRect r_;
        if(rect.Width() == 0)
        {
            r_ = ctrRect;
        }
        else
        {
            r_ = rect;
        }
        //	return pWnd->Create(_T("BUTTON"), lpszCaption, dwStyle, rect, pParentWnd, nID);
        if(!pWnd->CWnd::Create(L"STATIC", L"", WS_CHILD|WS_VISIBLE|style, r_, pParent, nCtrlId))
        {
            delete[] m_pKeys;
            return FALSE;
        } 
        m_nKeyCount = nKeyCount;
        m_nCurSel   = -1;


        VERIFY(m_font.CreateFont(
            22,                        // nHeight
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
        SetFont(&m_font);

        ModifyStyle(0, SS_NOTIFY);
        return TRUE;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //

    void CSoftKey::SetParameters(int iconNO, UINT16 *pKeyValueArray, BOOL bRedraw)
    {
        m_BitmapID = iconNO;

        if(pKeyValueArray)
        {
            for(int i=0; i< (int)m_nKeyCount; i++)
                m_pKeys[i].nKeyValue = pKeyValueArray[i];
        }
        if(bRedraw)
            this->Invalidate();
        //OnPaint();
    }

    void CSoftKey ::SetSoftKeyRect(CRect *pKeyRectArray, int x, int y)
    {
        CRect ctrRect = CRect(999, 999, 0, 0);
        for(int i=0; i < (int)m_nKeyCount; i++)
        {
            if(ctrRect.left > pKeyRectArray[i].left) 
                ctrRect.left = pKeyRectArray[i].left;

            if(ctrRect.top  > pKeyRectArray[i].top)
                ctrRect.top  = pKeyRectArray[i].top;

            if(ctrRect.right < pKeyRectArray[i].right)
                ctrRect.right = pKeyRectArray[i].right;

            if(ctrRect.bottom < pKeyRectArray[i].bottom)
                ctrRect.bottom = pKeyRectArray[i].bottom; 

            m_pKeys[i].sKeyRect  = pKeyRectArray[i];
        }
        //	m_sRect = ctrRect;
        m_pPoint.x = x;
        m_pPoint.y = y;
    }

    INT32   CSoftKey::GetPenKey(CPoint &pt)
    {
        KEYSTRUCT * pKey = m_pKeys;
        for(int i=0; i < (int)m_nKeyCount; i++, pKey++)
            if(pKey->sKeyRect.PtInRect(pt))
                return i;
        return -1;
    }


    //手写相关的函数
    void CSoftKey::SetHandWriteRect(CRect rect1, CRect rect2)
    {
        m_RegTimer = 1000; //1s
        m_bHaveHandWrite = TRUE;
        m_HWRect[0] = rect1;
        m_HWRect[1] = rect2;
    }

    int	CSoftKey::FindHWRect_(CPoint pt)
    {
        int ret = 0;

        if(m_HWRect[0].PtInRect(pt))
        {
            ret = 1;
        }
        else if(m_HWRect[1].PtInRect(pt))
        {
            ret = 2;
        }
        return ret;
    }

    void CSoftKey::OnTimer(UINT nIDEvent) 
    {
        // TODO: Add your message handler code here and/or call default
        if(IDC_REG_TIMER == nIDEvent)	//超时识别
        {
            KillTimer(IDC_REG_TIMER);
            if(m_bHaveHandWrite)
            {
                extern void PutIconPartial(CDC *pdc, int x, int y, int iNO, CRect r);
                CDC *pDC = GetDC();
                CRect rect = m_HWRect[m_nOldHWPenDown-1];
                rect.DeflateRect(-2, -2);
                PutIconPartial(pDC, rect.left, rect.top, m_BitmapID, rect);
                ReleaseDC(pDC);

                AddIndexBuff();   //创建新的buff

                GetParent()->SendMessage(WM_SOFTKEY_REG, m_nOldHWPenDown-1, 0);
            }
        }

        CStatic::OnTimer(nIDEvent);
    }

    //手写
    unsigned char *g_writedic;
    long g_pWriteRam[HWRERAMSIZE / 4];
    extern int HWRE_LoadDict(unsigned char** hDic);
    extern void HWRE_FreeDict( unsigned char** hDic );
    void HwWritePrc(void)
    {
        unsigned short Result[MAXCANDNUM];
        THWAttribute attr;
        attr.pRam = ( unsigned char* )g_pWriteRam;
        attr.iCandidateNum = MAXCANDNUM;			//set the number of candidate you want to be returned
        attr.dwRange = CHARSET_CNDEFAULT;			//set range
        while(TRUE)
        {
            if(m_nRecognizeBuff != m_nWriteLineBuff)
            {
                attr.pRom = g_writedic;
                int iCodeNum = 0; //HWRecognize((const THWPoint *)(m_pWriteLineBuff+m_nRecognizeBuff), &attr, Result);
                m_nRecognizeBuff++;
                if(m_nRecognizeBuff >= WRITE_LINE_BUFFMAX)
                    m_nRecognizeBuff = 0;
                //给手写窗口发送消息
                if(iCodeNum > 0)
                {
                    // 				extern CWnd *GetInputDlg();
                    // 				GetInputDlg()->SendMessage(WM_SOFTKEY_REG, (WPARAM)Result, (LPARAM)iCodeNum );
                }
            }
            else
            {
                ::Sleep(50);
            }
        }
    }

    void InitWriteLineBuff()
    {
        if(m_pWriteLineBuff == NULL)
        {
            m_nWriteLinePoint = 0;			
            m_nWriteLineBuff = -1;
            m_nRecognizeBuff = 0;
            if(m_pWriteLineBuff)
                delete m_pWriteLineBuff;
            m_pWriteLineBuff = (WriteLineBuff *)malloc(sizeof(WriteLineBuff)*WRITE_LINE_BUFFMAX);
            AddIndexBuff();

            if(m_pWriteLineBuff)
            {
                if(hWriteThread == NULL)
                    hWriteThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HwWritePrc, NULL, 0, NULL);
                if(hWriteThread)
                {
                    int ret = HWRE_LoadDict(&g_writedic);
                    if(ret != 0)
                    {
                        delete m_pWriteLineBuff;
                        m_pWriteLineBuff = NULL;			
                    }
                    else
                    {
                        //Dprintf("init write ok\n");
                        return;
                    }
                }
                else
                {
                    delete m_pWriteLineBuff;
                    m_pWriteLineBuff = NULL;
                }
            }
            //Dprintf("init write error\n");
        }
    }

    void AddWriteLineBuff(CPoint pt)
    {
        if(m_pWriteLineBuff)
        {
            if(m_nWriteLinePoint < WRITE_LINE_POINTMAX)
            {
                m_pWriteLineBuff[m_nWriteLineBuff].pt[m_nWriteLinePoint].x = pt.x;
                m_pWriteLineBuff[m_nWriteLineBuff].pt[m_nWriteLinePoint].y = pt.y;
                //Dprintf("%d %d (%d %d) ", m_nWriteLineBuff, m_nWriteLinePoint, pt.x, pt.y);
            }
            m_nWriteLinePoint++;
        }
        else
        {
            InitWriteLineBuff();
        }
    }

    void AddIndexBuff()
    {	
        if(m_pWriteLineBuff)
        {
            m_nWriteLinePoint = 0;
            m_nWriteLineBuff++; 
            //Dprintf("\nBuff Index %d\n", m_nWriteLineBuff );
            if(m_nWriteLineBuff >= WRITE_LINE_BUFFMAX)
                m_nWriteLineBuff = 0;
            memset((void *)(m_pWriteLineBuff+m_nWriteLineBuff), 0xFF, sizeof(WriteLineBuff));
        }
    }

    void FreeWriteLineBuff()
    {
        if(m_pWriteLineBuff)
        {
            delete m_pWriteLineBuff;
            m_pWriteLineBuff = NULL;

            HWRE_FreeDict( &g_writedic );
            //Dprintf("Free write\n");
        }
    }
    /////////////////////////////////////////////////////////////////////////////
    // CCEIconSoftKey

    CCEIconSoftKey::CCEIconSoftKey()
    {
        m_clrBack = RGB(255, 255, 255);
        m_nOldCurSel = -1;
    }

    CCEIconSoftKey::~CCEIconSoftKey()
    {
    }


    BEGIN_MESSAGE_MAP(CCEIconSoftKey, CStatic)
        //{{AFX_MSG_MAP(CSoftKey)
        ON_WM_LBUTTONDOWN()
        ON_WM_LBUTTONUP()
        ON_WM_CREATE()
        ON_WM_PAINT()
        ON_WM_TIMER()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CSoftKey message handlers

    void CCEIconSoftKey::OnLButtonDown(UINT nFlags, CPoint point) 
    {
        // TODO: Add your message handler code here and/or call default
        INT32 sel = GetPenKey(point);

        if(sel == m_nCurSel)
            return;

        CDC *pdc = GetDC();
        if(m_nCurSel >= 0 )
        {
            if(!m_pKeys[m_nCurSel].bToggle)
            {
                pdc->InvertRect(m_pKeys[m_nCurSel].sKeyRect);
            }
            m_nCurSel = -1;
        }
        if(sel >= 0)
        {
            m_nCurSel = sel;
            if(m_pKeys[m_nCurSel].bToggle)
                m_nToggle = m_nCurSel;
            if(!m_pKeys[m_nCurSel].bToggle)
            {
                pdc->InvertRect(m_pKeys[sel].sKeyRect);
            }
            else
            {
                Invalidate();
            }

        }
        ReleaseDC(pdc);
        CStatic::OnLButtonDown(nFlags, point);
    }

    void CCEIconSoftKey::OnLButtonUp(UINT nFlags, CPoint point) 
    {
        // TODO: Add your message handler code here and/or call default
        if(m_nCurSel >= 0)
        {
            if(!m_pKeys[m_nCurSel].bToggle)
            {
                CDC *pdc = GetDC();
                pdc->InvertRect(m_pKeys[m_nCurSel].sKeyRect);
                ReleaseDC(pdc);
            }
            //SendMessage(WM_SOFTKEY, GetDlgCtrlID(), 0);
            //NMHDR hm = {this,CN_COMMAND,UINT32(m_pKeys[sel].nKeyValue),sel};
            if(m_pKeys[m_nCurSel].bEnable)
                this->GetParent()->SendMessage(WM_SOFTKEY, UINT32(m_pKeys[m_nCurSel].nKeyValue), GetDlgCtrlID());
            m_nOldCurSel = m_nCurSel;
            m_nCurSel = -1;
        }

        CStatic::OnLButtonUp(nFlags, point);
    }

    BOOL CCEIconSoftKey::PreCreateWindow(CREATESTRUCT& cs) 
    {
        // TODO: Add your specialized code here and/or call the base class
        CStatic::PreCreateWindow(cs);
        return TRUE;
    }

    int CCEIconSoftKey::OnCreate(LPCREATESTRUCT lpCreateStruct) 
    {
        if (CStatic::OnCreate(lpCreateStruct) == -1)
            return -1;

        // TODO: Add your specialized creation code here
        ModifyStyle(0, SS_NOTIFY);
        // Create the font
        return 0;
    }

    void CCEIconSoftKey::OnPaint() 
    {
        CPaintDC dc(this); // device context for painting
        CFont m_Font;
        VERIFY(m_Font.CreateFont(
            16,                        // nHeight
            0,                         // nWidth
            0,                         // nEscapement
            0,                         // nOrientation
            FW_NORMAL,                 // nWeight
            FALSE,                     // bItalic
            0,						   // bUnderline
            0,                         // cStrikeOut
            ANSI_CHARSET,              // nCharSet
            OUT_DEFAULT_PRECIS,        // nOutPrecision
            CLIP_DEFAULT_PRECIS,       // nClipPrecision
            DEFAULT_QUALITY,           // nQuality
            DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
            _T("宋体")));              // lpszFacename
        dc.SelectObject(&m_Font);
        dc.SetBkMode(TRANSPARENT);
        CDC memDC;//内存设备场景
        CBitmap cb;//位图类对象
        CRect rect;
        GetClientRect(&rect);
        memDC.CreateCompatibleDC (&dc);//创建内存设备上下文

        //贴上背景图
        if(m_BitmapID > 0)
        {
            cb.LoadBitmap (m_BitmapID);//载入资源
            CBitmap *oldbitmap=NULL;
            oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
            dc.BitBlt(m_pPoint.x,m_pPoint.y, rect.Width (), rect.Height(), &memDC, 0, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
            memDC.SelectObject (oldbitmap);
            cb.DeleteObject();
        }
        else
        {
            dc.FillSolidRect(rect, m_clrBack);
        }

        //贴上toggle图
        if(m_nToggle != -1)
        {
            rect = m_pKeys[m_nToggle].sKeyRect;

            cb.LoadBitmap (m_BitmapToggleID);//载入资源
            CBitmap *oldbitmap=NULL;
            oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
            int n;
            int x = rect.left;
            int y = rect.top;
            if(m_nToggle == 3)
            {
                x = rect.left-25;
                n = 90;
            }
            else
            {
                n = rect.Width();
            }

            dc.BitBlt(x, y, n, rect.Height(), &memDC, TOGGLEBMP_WIDTH-n+1, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
            memDC.SelectObject (oldbitmap);
            cb.DeleteObject();
        }

        for(int i = 0; i < m_nKeyCount; i++)
        {
            rect = m_pKeys[i].sKeyRect;
            CRect rectTxt = rect;
            CRect rectBmp = rect;
            if(m_pKeys[i].nBitmapId > 0)
            {
                if(m_pKeys[i].nTextId > 0)
                {
                    if(m_pKeys[i].nAlign == IMAGE_LEFT)
                    {
                        rectBmp.left += MARGIN_PIXEL;
                        rectBmp.right = rectBmp.left+ICON_PIXEL-1;
                        rectBmp.top += MARGINDOWN_PIXEL;

                        rectTxt.left = rectBmp.right+1;
                        rectTxt.top += MARGINDOWN_PIXEL;
                    }
                    else if(m_pKeys[i].nAlign == IMAGE_RIGHT)
                    {
                        rectBmp.left = rectBmp.right - ICON_PIXEL +1;
                        rectTxt.right = rectBmp.left - MARGIN_PIXEL;
                        rectBmp.top += MARGINDOWN_PIXEL;
                        rectTxt.top += MARGINDOWN_PIXEL;
                    }
                    else if(m_pKeys[i].nAlign == IMAGE_TOP)
                    {
                        rectBmp.top += MARGIN_PIXEL;
                        rectBmp.bottom = rectBmp.top+ICON_PIXEL-1;
                        rectBmp.left += (rect.Width()-16)/2;
                        rectTxt.top = rectBmp.bottom+1;
                    }
                    else if(m_pKeys[i].nAlign == IMAGE_BOTTOM)
                    {
                        rectBmp.top = rectBmp.bottom - ICON_PIXEL +1;
                        rectTxt.bottom = rectBmp.top - MARGIN_PIXEL;
                        rectBmp.left += (rect.Width()-16)/2;
                    }
                    else
                    {
                        rectBmp.left += MARGIN_PIXEL;
                        rectBmp.right = rectBmp.left+ICON_PIXEL-1;
                        rectBmp.top += MARGINDOWN_PIXEL;
                        rectTxt.left = rectBmp.right+1;
                        rectTxt.top += MARGINDOWN_PIXEL;
                    }
                }
                //贴小图标

                cb.LoadBitmap (m_pKeys[i].nBitmapId);//载入资源
                CBitmap *oldbitmap=NULL;
                oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
                //	dc.BitBlt(rectBmp.left, rectBmp.top, rectBmp.Width (), rectBmp.Height(), &memDC, 0, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
                TransparentBlt2(dc.m_hDC, rectBmp.left, rectBmp.top, 16, 16, memDC.m_hDC, 0, 0, 16, 16, RGB(0xFF, 0, 0xFF));
                memDC.SelectObject (oldbitmap);
                cb.DeleteObject();

                //再画文字
                if(m_pKeys[i].nTextId > 0)
                {
                    CString s = Util::StringOp::ToCString(Data::LanguageResource::Get(m_pKeys[i].nTextId).c_str());
                    dc.DrawText(s, s.GetLength(), rectTxt, DT_WORDBREAK | m_pKeys[i].nAlign);
                }
            }
            else 
            {
                //居中显示文字
                if(m_pKeys[i].nTextId > 0)
                {
                    CString s = Util::StringOp::ToCString(Data::LanguageResource::Get(m_pKeys[i].nTextId).c_str());
                    dc.DrawText(s, s.GetLength(), rectTxt, DT_CENTER |DT_VCENTER );
                }
            }

        }
        // TODO: Add your message handler code here

        // Do not call CStatic::OnPaint() for painting messages
    }

    BOOL CCEIconSoftKey::Create(int		iconNO, 
        int			toggleIconNo,
        int			nToggleKey,
        CPoint		point,
        UINT32      nKeyCount, 
        ICONKEYSTRUCT	*softkeyval, 
        CRect		  &rect,
        CWnd * pParent, 
        int style,
        UINT32      nCtrlId)
    {
        //2005.5.8
        m_BitmapID = iconNO;
        m_pPoint = point;
        m_BitmapToggleID = toggleIconNo;
        m_nToggle = nToggleKey;
        m_pKeys  = new ICONKEYSTRUCT[nKeyCount];
        memset(m_pKeys, 0, sizeof(ICONKEYSTRUCT) * nKeyCount);
        if(!m_pKeys)
            return FALSE;
        CRect ctrRect(9999, 9999, 0,0);
        for(int i=0; i < (int)nKeyCount; i++)
        {
            if(ctrRect.left > softkeyval[i].sKeyRect.left) 
                ctrRect.left = softkeyval[i].sKeyRect.left;

            if(ctrRect.top  > softkeyval[i].sKeyRect.top)
                ctrRect.top  = softkeyval[i].sKeyRect.top;

            if(ctrRect.right < softkeyval[i].sKeyRect.right)
                ctrRect.right = softkeyval[i].sKeyRect.right;

            if(ctrRect.bottom < softkeyval[i].sKeyRect.bottom)
                ctrRect.bottom = softkeyval[i].sKeyRect.bottom; 

            m_pKeys[i].sKeyRect  = softkeyval[i].sKeyRect;
            m_pKeys[i].nKeyValue = softkeyval[i].nKeyValue;
            m_pKeys[i].nTextId = softkeyval[i].nTextId;
            m_pKeys[i].nBitmapId = softkeyval[i].nBitmapId;
            m_pKeys[i].nAlign = softkeyval[i].nAlign;
            m_pKeys[i].bToggle = softkeyval[i].bToggle;
            m_pKeys[i].bEnable = TRUE;
        }

        CWnd* pWnd = this;

        CRect r_;
        if(rect.Width() == 0)
        {
            r_ = ctrRect;
        }
        else
        {
            r_ = rect;
        }
        if(!pWnd->CWnd::Create(L"STATIC", L"", WS_CHILD|WS_VISIBLE|SS_NOTIFY|style, r_, pParent, nCtrlId))
        {
            delete[] m_pKeys;
            return FALSE;
        } 
        m_nKeyCount = nKeyCount;
        m_nCurSel   = -1;

        return TRUE;
    }


    INT32   CCEIconSoftKey::GetPenKey(CPoint &pt)
    {
        ICONKEYSTRUCT* pKey = m_pKeys;
        for(int i=0; i < (int)m_nKeyCount; i++, pKey++)
            if(pKey->sKeyRect.PtInRect(pt))
                return i;
        return -1;
    }

    INT8 CCEIconSoftKey::GetToggleKey()
    {
        return m_nToggle;
    }

    void CCEIconSoftKey ::SetKeyText(CString s, int nkey, COLORREF cr)
    {
        if(nkey > m_nKeyCount)
            return;
        CDC *pdc = GetDC(); // device context for painting
        pdc->SetBkMode(TRANSPARENT);
        COLORREF oldcr = pdc->SetTextColor(cr);
        CDC memDC;//内存设备场景
        CBitmap cb;//位图类对象
        CRect rect;
        rect = m_pKeys[nkey].sKeyRect;
        memDC.CreateCompatibleDC (pdc);//创建内存设备上下文

        //贴上背景图
        if(m_BitmapID > 0)
        {
            cb.LoadBitmap (m_BitmapID);//载入资源
            CBitmap *oldbitmap=NULL;
            oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
            pdc->BitBlt(rect.left, rect.top, rect.Width (), rect.Height(), &memDC, rect.left, rect.top, SRCCOPY);//将内存场景的图象拷贝到屏幕上
            memDC.SelectObject (oldbitmap);
            cb.DeleteObject();
        }
        else
        {
            pdc->FillSolidRect(rect, m_clrBack);
        }

        //贴上toggle图
        if(m_nToggle != -1 && nkey == m_nToggle)
        {
            cb.LoadBitmap (m_BitmapToggleID);//载入资源
            CBitmap *oldbitmap=NULL;
            oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
            pdc->BitBlt(rect.right-rect.Width()+1, rect.top, rect.Width (), rect.Height(), &memDC, 0, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
            memDC.SelectObject (oldbitmap);
            cb.DeleteObject();
        }

        rect = m_pKeys[nkey].sKeyRect;
        CRect rectTxt = rect;
        CRect rectBmp = rect;
        if(m_pKeys[nkey].nBitmapId > 0)
        {
            //if(m_pKeys[nkey].nTextId > 0)
            {
                if(m_pKeys[nkey].nAlign == IMAGE_LEFT)
                {
                    rectBmp.left += MARGIN_PIXEL;
                    rectBmp.right = rectBmp.left+ICON_PIXEL-1;
                    rectTxt.left = rectBmp.right+1;
                }
                else if(m_pKeys[nkey].nAlign == IMAGE_RIGHT)
                {
                    rectBmp.left = rectBmp.right - ICON_PIXEL +1;
                    rectTxt.right = rectBmp.left - MARGIN_PIXEL;
                }
                else if(m_pKeys[nkey].nAlign == IMAGE_TOP)
                {
                    rectBmp.top += MARGIN_PIXEL;
                    rectBmp.bottom = rectBmp.top+ICON_PIXEL-1;
                    rectTxt.top = rectBmp.bottom+1;
                }
                else if(m_pKeys[nkey].nAlign == IMAGE_BOTTOM)
                {
                    rectBmp.top = rectBmp.bottom - ICON_PIXEL +1;
                    rectTxt.bottom = rectBmp.top - MARGIN_PIXEL;
                }
                else
                {
                    rectBmp.left += MARGIN_PIXEL;
                    rectBmp.right = rectBmp.left+ICON_PIXEL-1;
                    rectTxt.left = rectBmp.right+1;
                }
            }
            //贴小图标
            cb.LoadBitmap (m_pKeys[nkey].nBitmapId);//载入资源
            CBitmap *oldbitmap=NULL;
            oldbitmap=memDC.SelectObject (&cb);//将位图选入当前设备场景
            //pdc->BitBlt(rectBmp.left, rectBmp.top, 16, 16, &memDC, 0, 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
            TransparentBlt2(pdc->m_hDC, rectBmp.left, rectBmp.top, 16, 16, memDC.m_hDC, 0, 0, 16, 16, RGB(0xFF, 0, 0xFF));
            memDC.SelectObject (oldbitmap);
            cb.DeleteObject();

            //再画文字
            pdc->DrawText(s, s.GetLength(), rectTxt, DT_WORDBREAK |  m_pKeys[nkey].nAlign);
        }
        else
        {
            //画文字
            pdc->DrawText(s, s.GetLength(), rectTxt, DT_CENTER |DT_VCENTER);
        }
        pdc->SetTextColor(oldcr);	
        ReleaseDC(pdc);
    }

    void CCEIconSoftKey::OnTimer(UINT nIDEvent) 
    {
        if(nIDEvent == 10)
        {
            CString s;
            char time[24];
            SYSTEMTIME curtime;
            GetLocalTime(&curtime);
            sprintf(time, "%02d:%02d", curtime.wHour, curtime.wMinute);
            s = time;

            //test lxz memory 20071109
            //	DMemprintf("CSoftKey 0");
            SetKeyText(s, 0, RGB(0xFF, 0xFF, 0xFF));
            //	DMemprintf("CSoftKey 1");
        }
    }

    void CCEIconSoftKey::SetLeftTimer()
    {
        SetTimer(10, 1000, NULL);	
    }

    //===========================================================================
    //手写库
    //===========================================================================
    //load dictionary
    // "CHS_Gbkcode_LittleEndian.dic"
#define	WRITE_INPUT_TABLE	"C:/flashdrv/res_dat/write.dic"
    int HWRE_LoadDict(unsigned char** hDic)
    {
        FILE *in;
        long nLen;
        unsigned char* pDic;

        in = fopen( WRITE_INPUT_TABLE, "rb" );
        if( in == NULL )return -1;

        fseek( in, 0, SEEK_END );
        nLen = ftell(in);
        fseek( in, 0, SEEK_SET );

        pDic = (unsigned char*)malloc(nLen);
        if( pDic == NULL )
        {
            fclose(in);
            return -2;
        }

        fread( pDic, 1, nLen, in );
        fclose(in);

        *hDic = pDic;

        return 0;
    }

    void HWRE_FreeDict( unsigned char** hDic )
    {
        if( hDic != NULL && *hDic != NULL )
        {
            free(*hDic);
            *hDic = NULL;
        }

        return;
    }
}
