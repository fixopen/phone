// CEDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CEDialog.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCEDialog dialog


CCEDialog::CCEDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CCEDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
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
	StretchBlt(hdc, rect.left, rect.top, rect.right - rect.left,rect.bottom - rect.top,
	dcTrans, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCAND);
	StretchBlt(hdc, rect.left, rect.top, rect.right - rect.left,rect.bottom - rect.top,
	dcImage24, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCPAINT);

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
	//CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	HDC  hScrDC, hMemDC;         
    int  width, height; 
    
	//creates a device context for the screen device
    hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL); 

    //get the screen point size
    width = GetDeviceCaps(hScrDC, HORZRES);
    height = GetDeviceCaps(hScrDC, VERTRES); 

    //creates a memory device context (DC) compatible with the screen device(hScrDC)  
    hMemDC = CreateCompatibleDC(hScrDC); 

	BYTE  *lpBitmapBits = NULL;
	BITMAPINFO RGB24BitsBITMAPINFO; 
    ZeroMemory(&RGB24BitsBITMAPINFO, sizeof(BITMAPINFO));
    RGB24BitsBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    RGB24BitsBITMAPINFO.bmiHeader.biWidth = width;
    RGB24BitsBITMAPINFO.bmiHeader.biHeight = height;
    RGB24BitsBITMAPINFO.bmiHeader.biPlanes = 1;
    RGB24BitsBITMAPINFO.bmiHeader.biBitCount = 24;
   
    HBITMAP directBmp = CreateDIBSection(hMemDC, (BITMAPINFO*)&RGB24BitsBITMAPINFO, DIB_RGB_COLORS, (void **)&lpBitmapBits, NULL, 0);
	HGDIOBJ previousObject = SelectObject(hMemDC, directBmp); 

	// copy the screen dc to the memory dc
	BitBlt(hMemDC, 0, 0, width, height, hScrDC, 0, 0, SRCCOPY);

	CDialog::OnPaint();

	BOOL DrawTransparentBmp(HDC hdc, HBITMAP hbmp, RECT &rect, COLORREF colorTrans);
	HBITMAP hBmp = LoadBitmap( AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_BITMAP_MASK));
	HDC pdc = ::GetDC(m_hWnd);
	CRect rt;
	GetClientRect(&rt);
	
	BitBlt(pdc, 0, 0, rt.Width()-1, rt.Height()-1, hMemDC, 0, 0, SRCCOPY);
	//DrawTransparentBmp(pdc, directBmp, rt, RGB(0, 0, 0));
	DrawTransparentBmp(pdc, hBmp, rt, RGB(0xFF, 0xFF, 0xFF));
	DeleteObject(hMemDC);
	DeleteObject(hScrDC);
	DeleteObject(directBmp);
	DeleteObject(previousObject);
	::ReleaseDC(m_hWnd, pdc);

	// Do not call CDialog::OnPaint() for painting messages
}
