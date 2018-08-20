// CEDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CEDialog.h"
#include "../resource.h"
#include "../Data/SkinStyle.h"
#include	"../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
	
	//暂时去掉时间
	//SetTimer(IDT_SETLEFTTIME,1000,NULL);
	
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


BEGIN_MESSAGE_MAP(CCEDialog, CDialog)
	//{{AFX_MSG_MAP(CCEDialog)
	ON_WM_PAINT()
	ON_WM_ACTIVATE()
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
	DWORD t, a;
	int ret1 = DMemprintf("CEDialog 0", t, a);

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
	
	int ret2 = DMemprintf("CEDialog 1", t, a);
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
	rect1 = CRect(0, 0, 480-1+1, (0+1)*VHEIGHT);
	pdc->FillSolidRect(&rect1, Data::g_allFramTopLineRGB[0][Data::g_skinstyle]);
	rect1 = CRect(1, 1*VHEIGHT, 480-2+1, (1+1)*VHEIGHT);
	pdc->FillSolidRect(&rect1, Data::g_allFramTopLineRGB[1][Data::g_skinstyle]);
	rect1 = CRect(2, 2*VHEIGHT, 480-3+1, (2+1)*VHEIGHT);
	pdc->FillSolidRect(&rect1, Data::g_allFramTopLineRGB[2][Data::g_skinstyle]);
	rect1 = CRect(3, 3*VHEIGHT, 480-4+1, (3+1)*VHEIGHT);
	pdc->FillSolidRect(&rect1, Data::g_allFramTopLineRGB[3][Data::g_skinstyle]);

	if(m_nStyle == 0)
	{
		rect1 = CRect(1, 4*VHEIGHT, 480-2+1, (12+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramTopLineRGB[4][Data::g_skinstyle]);
		rect1 = CRect(4, 13*VHEIGHT, 480-5+1, (13+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramBackRGB[Data::g_skinstyle]);
	}
	
	//画bottom线
	if(!bHalfwindow)
	{
		rect1 = CRect(0, (204-1)*VHEIGHT, 480-1+1, (204-1+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramBottomLineRGB[0][Data::g_skinstyle]);
		rect1 = CRect(1, (204-2)*VHEIGHT, 480-2+1, (204-2+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramBottomLineRGB[1][Data::g_skinstyle]);
		rect1 = CRect(2, (204-3)*VHEIGHT, 480-3+1, (204-3+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramBottomLineRGB[2][Data::g_skinstyle]);
	}
	
	//画left线
	rect1 = CRect(0, 0, 0+1, (204-1+1)*VHEIGHT);
	pdc->FillSolidRect(&rect1, Data::g_allFramLeftLineRGB[0][Data::g_skinstyle]);
	rect1 = CRect(1, 1*VHEIGHT, 1+1, (204-2+1)*VHEIGHT);
	pdc->FillSolidRect(&rect1, Data::g_allFramLeftLineRGB[1][Data::g_skinstyle]);
	rect1 = CRect(2, 2*VHEIGHT, 2+1, (204-3+1)*VHEIGHT);
	pdc->FillSolidRect(&rect1, Data::g_allFramLeftLineRGB[2][Data::g_skinstyle]);
	rect1 = CRect(3, 2*VHEIGHT, 3+1, (204-4+1)*VHEIGHT);
	pdc->FillSolidRect(&rect1, Data::g_allFramLeftLineRGB[3][Data::g_skinstyle]);

	//画right线
	if(!bHalfwindow)
	{
		rect1 = CRect(480-1, 0, 480-1+1, 204-1+1);
		pdc->FillSolidRect(&rect1, Data::g_allFramRightLineRGB[0][Data::g_skinstyle]);
		rect1 = CRect(480-2, 1*VHEIGHT, 480-2+1, (204-2+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramRightLineRGB[1][Data::g_skinstyle]);
		rect1 = CRect(480-3, 2*VHEIGHT, 480-3+1, (204-3+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramRightLineRGB[2][Data::g_skinstyle]);
		rect1 = CRect(480-4, 2*VHEIGHT, 480-4+1, (204-4+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramRightLineRGB[3][Data::g_skinstyle]);
	}
	
	
	if(m_nStyle == 0)
	{
		//最后画一条Top线
		rect1 = CRect(1, 14*VHEIGHT, 480-2+1, (14+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramTopLineRGB[4][Data::g_skinstyle]);
		
		//画背景
		rect1 = CRect(4, 15*VHEIGHT, 480-5+1, (204-4+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramBackRGB[Data::g_skinstyle]);
	}
	else if(m_nStyle == 1 || m_nStyle == 2)
	{
		//画背景
		rect1 = CRect(4, 4*VHEIGHT, 480-4+1, (204-4+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramBackRGB[Data::g_skinstyle]);
	}

	//当 m_nStyle = 1时，画三角形及中间框
	if(m_nStyle == 1)
	{
		//画top
		rect1 = CRect(293, 0*VHEIGHT, 480-1+1, (0+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramAngleTopLineRGB[0][Data::g_skinstyle]);
		rect1 = CRect(294, 1*VHEIGHT, 480-2+1,(1+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramAngleTopLineRGB[1][Data::g_skinstyle]);
		rect1 = CRect(295-2, 2*VHEIGHT, 480-3+1, (2+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramAngleTopLineRGB[2][Data::g_skinstyle]);
		rect1 = CRect(295-2, 3*VHEIGHT, 480-4+1, (3+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramAngleTopLineRGB[3][Data::g_skinstyle]);
		rect1 = CRect(295-2, 4*VHEIGHT, 480-5+1, (4+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramAngleTopLineRGB[4][Data::g_skinstyle]);

		//画right
		rect1 = CRect(480-1, 0, 480-1+1, (188-1+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramAngleRightLineRGB[0][Data::g_skinstyle]);
		rect1 = CRect(480-2, 1*VHEIGHT, 480-2+1, (188-2+1+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramAngleRightLineRGB[1][Data::g_skinstyle]);
		rect1 = CRect(480-3, 2*VHEIGHT, 480-3+1, (188-3+1+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramAngleRightLineRGB[2][Data::g_skinstyle]);
		rect1 = CRect(480-4, 3*VHEIGHT, 480-4+1, (188-4+1+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramAngleRightLineRGB[3][Data::g_skinstyle]);
		rect1 = CRect(480-5, 4*VHEIGHT, 480-5+1, (188-5+1+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramAngleRightLineRGB[4][Data::g_skinstyle]);

		int x = 296;
		for(int i = 5; i <= 184; i++)
		{
			rect1 = CRect(x++, i*VHEIGHT, 475+1, (i+1)*VHEIGHT);
			pdc->FillSolidRect(&rect1, Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);
		}

		//画三角形
		CPen pen(PS_SOLID, 1, Data::g_allFramAngleXiexianLineRGB[0][Data::g_skinstyle]);
		CPen *oldpen = pdc->SelectObject(&pen);
		pdc->MoveTo(CPoint(292, 2*VHEIGHT));
		pdc->LineTo(CPoint(478, 188*VHEIGHT));
		CPen pen1(PS_SOLID, 1, Data::g_allFramAngleXiexianLineRGB[1][Data::g_skinstyle]);
		pdc->SelectObject(&pen1);
		pdc->MoveTo(CPoint(291, 2*VHEIGHT));
		pdc->LineTo(CPoint(477, 188*VHEIGHT));
		pdc->SelectObject(oldpen);

		rect1 = CRect(303, 10*VHEIGHT, 448+1, (10+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramAngleHerLineRGB[0][Data::g_skinstyle]);
		rect1 = CRect(304, 11*VHEIGHT, 448+1, (11+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramAngleHerLineRGB[1][Data::g_skinstyle]);
		rect1 = CRect(306, 13*VHEIGHT, 448+1, (13+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramAngleHerLineRGB[0][Data::g_skinstyle]);
		rect1 = CRect(307, 14*VHEIGHT, 448+1, (14+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramAngleHerLineRGB[1][Data::g_skinstyle]);
		rect1 = CRect(310, 16*VHEIGHT, 448+1, (16+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramAngleHerLineRGB[0][Data::g_skinstyle]);
		rect1 = CRect(311, 17*VHEIGHT, 448+1, (17+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramAngleHerLineRGB[1][Data::g_skinstyle]);

		//画内框
		//画top
		rect1 = CRect(8, 26*VHEIGHT, 407+1, (26+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectTopLineRGB[0][Data::g_skinstyle]);
		rect1 = CRect(8-1, (26-1)*VHEIGHT, 407+1+1, (26+1-1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectTopLineRGB[1][Data::g_skinstyle]);
		rect1 = CRect(8-2, (26-2)*VHEIGHT, 407+1+2, (26+1-2)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectTopLineRGB[2][Data::g_skinstyle]);

		//画bottom
		rect1 = CRect(8, 197*VHEIGHT, 407+1, (197+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectBottomLineRGB[0][Data::g_skinstyle]);
		rect1 = CRect(8-1, (197+1)*VHEIGHT, 407+1+1, (197+1+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectBottomLineRGB[1][Data::g_skinstyle]);
		rect1 = CRect(8-2, (197+2)*VHEIGHT, 407+1+2,(197+1+2)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectBottomLineRGB[2][Data::g_skinstyle]);

		//画left
		rect1 = CRect(7, 26*VHEIGHT, 7+1, (197+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectLeftLineRGB[0][Data::g_skinstyle]);
		rect1 = CRect(7-1, (26-1)*VHEIGHT, 7+1-1, (197+1+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectLeftLineRGB[1][Data::g_skinstyle]);
		rect1 = CRect(7-2, (26-2)*VHEIGHT, 7+1-2, (197+1+2)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectLeftLineRGB[2][Data::g_skinstyle]);

		//画right
		rect1 = CRect(408, 26*VHEIGHT, 408+1, (197+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectRightLineRGB[0][Data::g_skinstyle]);
		rect1 = CRect(408+1, (26-1)*VHEIGHT, 408+1+1, (197+1+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectRightLineRGB[1][Data::g_skinstyle]);
		rect1 = CRect(408+2, (26-2)*VHEIGHT, 408+1+2, (197+1+2)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectRightLineRGB[2][Data::g_skinstyle]);

		//画Angletop
		rect1 = CRect(316, 26*VHEIGHT, 407+1, (26+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectAngleTopLineRGB[0][Data::g_skinstyle]);
		rect1 = CRect(316-1, (26-1)*VHEIGHT, 407+1+1, (26+1-1));
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectAngleTopLineRGB[1][Data::g_skinstyle]);
		rect1 = CRect(316-2, (26-2)*VHEIGHT, 407+1+2, (26+1-2)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectAngleTopLineRGB[2][Data::g_skinstyle]);

		//画Angleright
		rect1 = CRect(408, 26*VHEIGHT, 408+1, (118+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectAngleRightLineRGB[0][Data::g_skinstyle]);
		rect1 = CRect(408+1, (26-1)*VHEIGHT, 408+1+1, (118+1+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectAngleRightLineRGB[1][Data::g_skinstyle]);
		rect1 = CRect(408+2, (26-2)*VHEIGHT, 408+1+2, (118+1+2)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectAngleRightLineRGB[2][Data::g_skinstyle]);

		//画InRect
		rect1 = CRect(8, 27*VHEIGHT, 407+1, (196+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	}

	//画上小框
	if(bHalfwindow && m_InRect.Width() != 0)
	{
		CRect rrt = m_InRect;
		rect1 = CRect(rrt.left, rrt.top*VHEIGHT, rrt.right+1, (rrt.top+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allInRectLine1RGB[0][Data::g_skinstyle]);
		rect1 = CRect(rrt.left, rrt.bottom*VHEIGHT, rrt.right+1, (rrt.bottom+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allInRectLine1RGB[1][Data::g_skinstyle]);
		rect1 = CRect(rrt.left, rrt.top*VHEIGHT, rrt.left+1, (rrt.bottom+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allInRectLine1RGB[0][Data::g_skinstyle]);
		rect1 = CRect(rrt.right, rrt.top*VHEIGHT, rrt.right+1, (rrt.bottom+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allInRectLine1RGB[1][Data::g_skinstyle]);

		rrt.DeflateRect(1,1);
		rect1 = CRect(rrt.left, rrt.top*VHEIGHT, rrt.right+1, (rrt.top+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allInRectLine2RGB[0][Data::g_skinstyle]);
		rect1 = CRect(rrt.left, rrt.bottom*VHEIGHT, rrt.right+1, (rrt.bottom+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allInRectLine2RGB[1][Data::g_skinstyle]);
		rect1 = CRect(rrt.left, rrt.top*VHEIGHT, rrt.left+1, (rrt.bottom+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allInRectLine2RGB[0][Data::g_skinstyle]);
		rect1 = CRect(rrt.right, rrt.top*VHEIGHT, rrt.right+1, (rrt.bottom+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allInRectLine2RGB[1][Data::g_skinstyle]);
	}

	//通话记录 名片主窗口
	if(m_nStyle == 2)
	{
		CRect rrt = CRect(7, 26*VHEIGHT, 472, 197*VHEIGHT);
		rect1 = CRect(rrt.left, rrt.top*VHEIGHT, rrt.right+1, (rrt.top+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);
		rect1 = CRect(rrt.left, rrt.bottom*VHEIGHT, rrt.right+1, (rrt.bottom+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine2RGB[0][Data::g_skinstyle]);
		rect1 = CRect(rrt.left, rrt.top*VHEIGHT, rrt.left+1, (rrt.bottom+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);
		rect1 = CRect(rrt.right, rrt.top*VHEIGHT, rrt.right+1, (rrt.bottom+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine2RGB[0][Data::g_skinstyle]);
		
		rrt.DeflateRect(-1,-1);
		rect1 = CRect(rrt.left, rrt.top*VHEIGHT, rrt.right+1, (rrt.top+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine1RGB[1][Data::g_skinstyle]);
		rect1 = CRect(rrt.left, rrt.bottom*VHEIGHT, rrt.right+1, (rrt.bottom+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine2RGB[1][Data::g_skinstyle]);
		rect1 = CRect(rrt.left, rrt.top*VHEIGHT, rrt.left+1, (rrt.bottom+1));
		pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine1RGB[1][Data::g_skinstyle]);
		rect1 = CRect(rrt.right, rrt.top*VHEIGHT, rrt.right+1, (rrt.bottom+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine2RGB[1][Data::g_skinstyle]);

		rrt.DeflateRect(-1,-1);
		rect1 = CRect(rrt.left, rrt.top*VHEIGHT, rrt.right+1, (rrt.top+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine1RGB[1][Data::g_skinstyle]);
		rect1 = CRect(rrt.left, rrt.bottom*VHEIGHT, rrt.right+1, (rrt.bottom+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine2RGB[1][Data::g_skinstyle]);
		rect1 = CRect(rrt.left, rrt.top*VHEIGHT, rrt.left+1, (rrt.bottom+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine1RGB[1][Data::g_skinstyle]);
		rect1 = CRect(rrt.right, rrt.top*VHEIGHT, rrt.right+1, (rrt.bottom+1)*VHEIGHT);
		pdc->FillSolidRect(&rect1, Data::g_allFrameInFrameLine2RGB[1][Data::g_skinstyle]);
	}
	ReleaseDC(pdc);

	// Do not call CDialog::OnPaint() for painting messages
}

void CCEDialog::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==IDT_SETLEFTTIME)
	{
		//add by qi 20111025
		//有输入法下面，不显示时间
		CRect rect ;
		theApp.inputDlg_->GetWindowRect(&rect);
		if (theApp.inputDlg_->IsWindowVisible() && \
			rect.top > 128)
		{
			return ;
		}
		//

		//存放当前系统时间
		SYSTEMTIME curtime;
		//获取当前系统时间
		GetLocalTime(&curtime);
		//存放格式化时间的数组
		char time[24];
		//格式化时间，只要小时和分
		sprintf(time, "%02d:%02d", curtime.wHour, curtime.wMinute);
		//存放时间的字符串
		CString sTime = time;
		
		setLeft_Time(sTime,RGB(0xFF, 0xFF, 0xFF));	
			
	}
	
}
void CCEDialog::setLeft_Time(CString sTime,COLORREF cr)
{   
	if(!((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
	{   
		
		HDC hdc=::GetDC(NULL);
						
		::SetBkMode(hdc,TRANSPARENT);		
		COLORREF oldcr = ::SetTextColor(hdc,cr);		
		
		HDC hMemoyDc;	
		hMemoyDc = ::CreateCompatibleDC(hdc);
		
		CBitmap cb;
		
		CRect rect=CRect(0, 203*VHEIGHT+1, 40,231*VHEIGHT);	
		cb.LoadBitmap (8933);//载入资源
		
		HBITMAP hOldBitmap=NULL;	
		hOldBitmap = (HBITMAP) ::SelectObject(hMemoyDc,cb.m_hObject);//将位图选入当前设备场景
		
		::BitBlt(hdc,rect.left,rect.top, rect.Width (), rect.Height(), hMemoyDc,0 , 0, SRCCOPY);//将内存场景的图象拷贝到屏幕上
				
		::SelectObject(hMemoyDc,hOldBitmap);	
		cb.DeleteObject();
	
		CRect rect2=CRect(3,204*VHEIGHT+6,40,232*VHEIGHT);	
		::DrawText(hdc,sTime, sTime.GetLength(), rect2, DT_CENTER|DT_VCENTER);
		
		::SetTextColor(hdc,oldcr);

		::DeleteDC(hMemoyDc);
		::ReleaseDC(NULL,hdc);	
	}
	//pWnd->ReleaseDC(pdc);	
}