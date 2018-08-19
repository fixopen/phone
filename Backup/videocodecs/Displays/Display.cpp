#include "StdAfx.h"
#include "Display.h"

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
	RECT	rc;
	pWnd->GetWindowRect(&rc);
	uiWinWidth = rc.right - rc.left;
	uiWinHeight = rc.bottom - rc.top;
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
						if(pDC->BitBlt(0,0,uiWinWidth,uiWinHeight,&bitmapDC,0,0,SRCCOPY)==FALSE)
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
							ret = false;
						bitmapDC.SelectObject(pOldBitmap);	
					}
					bitmapDC.DeleteDC();
				}
				bitmap.DeleteObject();
			}
		}
	}
	return ret;
}
