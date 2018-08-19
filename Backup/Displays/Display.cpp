#include "StdAfx.h"
#include "Display.h"
#include "mfc_render.h"

static bool NewTest = true;
unsigned int CodeWidth = 176;
unsigned int CodeHeigt = 144;
static 	CRITICAL_SECTION criticalSection_;
static  bool crinited = false;




Display::Display(void)
{
	pWnd = NULL;
}

Display::~Display(void)
{

}
bool Display::DisplayInit()
{
	//pWnd = new CWnd;
	//if(!pWnd)
	//{
	//	DWORD error = GetLastError();
	//	TRACE1("new CWnd Failed with: %i\n", error);
	//	return false;
	//}

	return true;

}
bool Display::DisplayDeInit()
{
	if(pWnd)
	{
		MyDx_Render.mfc_render_final();
		if (pRender)
		{
			MyDx_Render.mfc_render_delete_surface(pRender);
		}
		if (crinited)
		{
			crinited = false;
			::DeleteCriticalSection(&criticalSection_);
		}
		if(!pWnd->Detach())
		{
			DWORD error = GetLastError();
			TRACE1("pWnd->Detach Failed with: %i\n", error);
		
		}
		delete pWnd;
		pWnd = NULL;
	}
	return true;

}
bool Display::SetPara(HWND hVideoWnd)
{
	pWnd = CWnd::FromHandle(hVideoWnd);
	if(!pWnd)
	//if(!pWnd->Attach(hVideoWnd))
	{
		DWORD error = GetLastError();
		TRACE1("pWnd->Attach() Failed with: %i\n", error);
		return false;
	}
	pWnd->GetWindowRect(&rc);
	uiWinWidth = rc.right - rc.left;
	uiWinHeight = rc.bottom - rc.top;
	//if (!MyDx_Render.mfc_render_init(hVideoWnd,CodeWidth,CodeHeigt,MFC_RENDER_SURFACE_TYPE_YV12))
	if (!MyDx_Render.mfc_render_init(hVideoWnd,CodeWidth,CodeHeigt,MFC_RENDER_SURFACE_TYPE_RGB565))
	{
		DWORD error = GetLastError();
		TRACE1("mfc_render_init() Failed with: %i\n", error);
		return false;
	}
	//ugly for temp test
	static bool first = true;
	if (first)
	{
		first = false;
		return TRUE;
	}
	if (!crinited)
	{
		crinited = true;
		::InitializeCriticalSection(&criticalSection_);
	}


	//pRender = MyDx_Render.mfc_render_create_surface(MFC_RENDER_SURFACE_TYPE_YV12,
	pRender = MyDx_Render.mfc_render_create_surface(MFC_RENDER_SURFACE_TYPE_RGB565,
		0, 0,
		CodeWidth, CodeHeigt,
		CodeWidth, CodeHeigt);
	if (!pRender)
	{
		DWORD error = GetLastError();
		TRACE1("mfc_render_create_overlay() Failed with: %i\n", error);
		return false;
	}
	MyDx_Render.WindowLeft = rc.left;
	MyDx_Render.WindowTop = rc.top;
	MyDx_Render.win_rc.left = rc.left;
	MyDx_Render.win_rc.top = rc.top;
	MyDx_Render.win_rc.right += rc.left + CodeWidth;
	MyDx_Render.win_rc.bottom += rc.top + CodeHeigt;
	return true;
}
bool Display::ShowPic(/*ImgFormat ImgFM,*/unsigned int Video_Width,unsigned int Video_Height,unsigned char *VideoData,bool IsRGB565)
{
	bool ret = true;
	{
	
		CBitmap bitmap;
		if(IsRGB565)
		{
			if (bitmap.CreateBitmap(Video_Width,Video_Height,1,16,VideoData)!= FALSE)
			{
				pWnd->UpdateWindow();
				CDC* pDC=pWnd->GetDC();
				CDC bitmapDC;
				if(bitmapDC.CreateCompatibleDC(pDC)==TRUE)
				{
					CBitmap* pOldBitmap;
					if((pOldBitmap = bitmapDC.SelectObject(&bitmap))!=NULL)
					{
						//if(pDC->BitBlt(0,0,uiWinWidth,uiWinHeight,&bitmapDC,0,0,SRCCOPY)==FALSE)
						if(pDC->StretchBlt(0,0,uiWinWidth,uiWinHeight,&bitmapDC,0,0,Video_Width,Video_Height,SRCCOPY)==FALSE)
							ret = false;
						bitmapDC.SelectObject(pOldBitmap);	
					}
					bitmapDC.DeleteDC();
					}
				bitmap.DeleteObject();
			}
// 			else
// 			{
// 				DWORD error = GetLastError();
// 				TRACE1("CreateBitmap() Failed with: %i\n", error);
// 			}


		}
		else 
		{
			if (NewTest){
			//	DisplayImgane ((int)VideoData,Video_Width,Video_Height,uiWinWidth,uiWinHeight,rc.left,rc.top,0);
				::EnterCriticalSection(&criticalSection_);
				MyDx_Render.mfc_render_do(pRender,VideoData,Video_Width,Video_Height,MFC_RENDER_IMAGE_TYPE_YUV420);
				//MyDx_Render.mfc_render_flip(pRender);
				::LeaveCriticalSection(&criticalSection_);
			}
			else{
				if (bitmap.CreateBitmap(Video_Width,Video_Height,1,24,VideoData)!= FALSE)
				{
					pWnd->UpdateWindow();
					CDC* pDC=pWnd->GetDC();
					CDC bitmapDC;
					if(bitmapDC.CreateCompatibleDC(pDC)==TRUE)
					{
						CBitmap* pOldBitmap;
						if((pOldBitmap = bitmapDC.SelectObject(&bitmap))!=NULL)
						{

							if(pDC->BitBlt(0,0,uiWinWidth,uiWinHeight,&bitmapDC,0,0,SRCCOPY)==FALSE)
							//if(pDC->StretchBlt(0,0,uiWinWidth,uiWinHeight,&bitmapDC,0,0,Video_Width,Video_Height,SRCCOPY)==FALSE)
								ret = false;
							bitmapDC.SelectObject(pOldBitmap);	
						}
						bitmapDC.DeleteDC();
					}
					bitmap.DeleteObject();
				}
			}
		}
	}
	return ret;
}
