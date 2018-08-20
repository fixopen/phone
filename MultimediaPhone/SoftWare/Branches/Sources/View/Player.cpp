// Player.cpp: implementation of the Player class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../resource.h"

#include "Player.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Player::Player(CWnd* owner)
: isPlaying_(0)
, owner_(owner)
{
	index_ = 0;
	type_ = 0;  //图片
	isActiveMode_ = false;
	isPlayerRun = FALSE;
	isPlayingVideo = FALSE;
}

Player::~Player()
{
	ExitPlayer();
}

bool Player::InitPlayer()
{
	if(!isPlayerRun)
	{
		if (plyCreate(10, 10, 0, 0) == 0)
		{
			//Dprintf("plyCreate ok\n");
			::Sleep(1 * 1000);
			for (int i = 0; i < 100; ++i)
			{
				if (plyInit(playerOwner_->m_hWnd) == 0)
				{
					//Dprintf("plyInit ok\n");
					::Sleep(500);
					break;
				}
				::Sleep(50);
			}
			isPlayerRun = TRUE;
			plySetRepeat(TRUE);
			plySetAutoPreRotate(FALSE);
			plySetZoom(ZOOM_STRETCH_SCREEN);
			SetVolume(SOUND_DEFAULT_VOLUME);
			Sleep(10);
			return true;
		}
		return false;
	}

	else
	{
		isPlaying_ = 1;
		isPlayingVideo = TRUE;
		plyPlay(TRUE);
	}
	return TRUE;
}

bool Player::ExitPlayer(BOOL flag)
{
	/*
	if(plyExit() == 0)
		return true;
	return false;
	*/
	if(isPlayerRun)
	{
		isPlaying_ = 0;
		isPlayingVideo = FALSE;
		static BOOL bisFirst = TRUE;
		plyPlay(FALSE);
		if(bisFirst)
		{
			plyPlay(FALSE);
			plyPlay(FALSE);
		}
		if(flag)
		{
			HWND hWnd = ::FindWindow(L"csplayer_win", L"csplayer window"); //
			::ShowWindow(hWnd, SW_HIDE);
		}
	}

	//当退出时，将m_bIsPausebyEvent 为FALSE;
	extern BOOL m_bIsPausebyEvent;
	m_bIsPausebyEvent = FALSE;

	return true;
	
}

bool Player::GetVideoFileInfo(LPMEDIA_INFO info)
{
	int ret;
    ret =  plyGetMediaInfo(info);   //debug
//	wprintf(info->szFileName);
//	Dprintf("  timer = %d %d\n",  info->nDuration, info->nTick);
	if(ret == 0)
		return true;
	return false;
}

bool Player::PlayerFile(char *filename)
{
	isPlaying_ = 1;
	isPlayingVideo = TRUE;
	if (isActiveMode_)
	{
		TCHAR wideFilename[256] = {0};
		size_t wideContentLength = MultiByteToWideChar(CP_ACP, 0, filename, strlen(filename), 0, 0);
		mbstowcs(wideFilename, filename, wideContentLength);
		if(owner_)
		{
			CRect rt;
			owner_->GetWindowRect(&rt);
			plySetWndPos(rt.left, rt.top, rt.Width(), rt.Height());
		}
		if (plyOpen(wideFilename) == 0)
		{
			plySetZoom(ZOOM_FILL_SCREEN);
			//Dprintf("plyOpen(filepath) ok\n");
			return true;
		}
		return false;
	}
	else
	{
	}
	return false;
}

bool Player::SetImageList(std::vector<CString> files)
{
	bool result = false;
	files_ = files;
	index_ = 0;
	result = true;
	return result;
}


extern void ReleaseJPG(void *ptr);
extern void JpegDraw(HDC hdc, long x, long y, long cx, long cy, int w, int h, BYTE *ptr);
extern unsigned char *JpegFileToRGB(char *fileName,
							 char *rgbbuf,
							 UINT *width,
							 UINT *height);

bool Player::PlayerImage()
{
	bool result = false;
	isPlaying_ = 2;

	if(files_.size() == 0)
		return false;
	CString filename = files_[index_];
	DWORD t, a;
	int ret1 = DMemprintf("LoadImage 0", t, a);

	DrawImage((LPCTSTR)filename);
	
	int ret2 = DMemprintf("LoadImage 1", t, a);
	
	if(ret1 != ret2)
	{
		Dprintf("ret1 - ret2 = %d\r\n",  (ret1-ret2));
		
		FILE *file;
		//static BOOL isStart = TRUE;
		file = fopen("/flashdrv/my_photo/list.txt", "a");
		
		if(file)
		{
			SYSTEMTIME curtime;
			GetLocalTime(&curtime);
			char txt[128];
			static int nTotal = 0;
			nTotal += (ret1 -ret2);
			sprintf(txt, "(Timer: %d-%d-%d %d:%d:%d) (T:A %x:%x) (%d %d)\r\n", curtime.wYear, curtime.wMonth, curtime.wDay, curtime.wHour, curtime.wMinute, curtime.wSecond, t, a, ret1-ret2, nTotal);
			fwrite(txt, sizeof(char), strlen(txt), file);
			fclose(file);
		}
	}

	return result;
}
/*
#define CXIMAGE_JPG  1

bool Player::PlayerImage()
{
	bool result = false;
	isPlaying_ = 2;

	if(files_.size() == 0)
		return false;
	CString filename = files_[index_];

	bool ret;
	Dprintf("\r\n");
	int ret1 = DMemprintf("LoadImage 0");
	CxImage* image_;
	image_ = new CxImage();
	int nJPG = filename.Find(_T(".jpg"));
	int nJPG1 = filename.Find(_T(".JPG"));
	int nBMP = filename.Find(_T(".bmp"));
	int nBMP1 = filename.Find(_T(".BMP"));
	int nTGA = filename.Find(_T(".tga"));
	char *file_;
	BYTE *ptr = NULL;
	UINT32 wFile, hFile;
	if(nJPG1 >= 0 || nJPG >= 0)
	{
		ret = image_->Load((LPCTSTR)filename, CXIMAGE_FORMAT_JPG);
	}
	else if(nBMP1 >= 0 || nBMP >= 0)
	{
		ret = image_->Load((LPCTSTR)filename, CXIMAGE_FORMAT_BMP);
	}
	else if(nTGA >= 0)
	{
		ret = image_->Load((LPCTSTR)filename, CXIMAGE_FORMAT_TGA);
	}
	else
	{
		image_->Destroy();
		delete image_;
		return false;
	}

	if(owner_ && ret)
	{
		CDC* hdc = owner_->GetDC();
		CRect rt;
		owner_->GetClientRect(rt);
		int x = 0;
		int y = 0;
		int h = image_->GetHeight();//hFile;//image_->GetHeight(); 
		int w = image_->GetWidth();//wFile;//image_->GetWidth();

		double hp = 234/(double)h;
		double wp = 480/(double)w;

		if(hp<1.0 || wp<1.0)
		{
			if(hp < wp)
			{
				w = (w*hp);
			}
			else
			{
				h = (h*wp);
			}
		}
		if(h>rt.Height())
		{
			h = rt.Height();
		}
		else
		{
			y = (rt.Height() - h)/2;
		}
		if(w>rt.Width())
		{
			w = rt.Width();
		}
		else
		{
			x = (rt.Width() - w)/2;
		}

		DMemprintf("Draw 0");
		//临时加上相框 20071217 by lxz
		if(w >= 479 && h >= 233)
			image_->Draw(hdc->m_hDC, 24, 24, 454-24+1, 210-24+1);//, 0, TRUE);	
		else
			image_->Draw(hdc->m_hDC, rt.left+x, rt.top+y, w, h);//, 0, TRUE);

		DMemprintf("Draw 2");

		if(w < rt.Width())
		{
			CBrush brsh = RGB(0, 0, 0);
			CRect rt1 = CRect(rt.left, rt.top, rt.left+x, rt.bottom);
			CRect rt2 = CRect(rt.left+x+w-1, rt.top, rt.right, rt.bottom);
			hdc->FillRect(&rt1, &brsh);
			hdc->FillRect(&rt2, &brsh);
		}
	
		owner_->ReleaseDC(hdc);
	}
	image_->Destroy();   
	delete image_;
	
	int ret2 = DMemprintf("LoadImage 1");
	
//	Dprintf("result : %d\r\n", ret1-ret2);
	static int nTotal = 0;
	if((ret1 -ret2) > 0)
	{
		FILE *file;
		//static BOOL isStart = TRUE;
		file = fopen("/flashdrv/my_photo/list.txt", "a");
		
		if(file)
		{
			int length = wcstombs(0, filename, filename.GetLength() * 2);
			char file_[64];
			memset(file_, 0, 64);
			int convLength = wcstombs(file_, filename, filename.GetLength() * 2);
			char txt[128];
			nTotal += (ret1 -ret2);
			sprintf(txt, "file:%s , error: %d %d\r\n", file_, ret1-ret2, nTotal);
			fwrite(txt, sizeof(char), strlen(txt), file);
			fclose(file);
		}
	}

	return result;
}
*/

bool Player::StopImage()
{
	isPlaying_ = 0;
	bool result = false;
	return result;
}

int Player::Up()
{
	if(files_.size() == 0)
		return 0;

	bool result = false;
	--index_;
	if (index_ < 0)
		index_ = files_.size() - 1;
	PlayerImage();
	return index_;
}

int Player::Down()
{
	if(files_.size() == 0)
		return 0;

	bool result = false;
	++index_;
	if (index_ > (files_.size() - 1))
		index_ = 0;
	PlayerImage();
	return index_;
}

bool Player::StopPlayer()
{
	if( plyStop() == 0)
		return true;
	return false;
}

bool Player::SetAllScreenPlayer(BOOL flag)
{
	if(flag)
	{
		plySetZoom(ZOOM_FIT_SCREEN);
		if(plySetWndPos(0, 0, 480, 234) == 0)
		{
			return true;
		}
	}
	else
	{
		if(owner_)
		{
			CRect rt;
			owner_->GetWindowRect(&rt);
			plySetZoom(ZOOM_STRETCH_SCREEN);
			if(plySetWndPos(rt.left, rt.top, rt.Width(), rt.Height()) == 0)
			{
				return true;
			}
		}
	}
	return false;
}

#include   <INITGUID.h>   
#include   <imaging.h> 
void   Player::DrawImage(LPCTSTR szFileImage) 
{ 
	IImagingFactory   *pImgFactory   =   NULL; 
	IImage   *pImage   =   NULL; 
	
	CoInitializeEx(NULL,   COINIT_MULTITHREADED); 
	
	//   Create   the   imaging   factory. 
	if (SUCCEEDED(CoCreateInstance   (CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void   **)&pImgFactory))) 
	{ 
		//   Load   the   image   from   the   JPG   file. 
		if (SUCCEEDED(pImgFactory-> CreateImageFromFile(szFileImage, &pImage))) 
		{ 
			//   Draw   the   image. 
			
			CDC* hdc = owner_->GetDC();
			CRect rt;
			owner_->GetClientRect(rt);
			int x = 0;
			int y = 0;
			int h = rt.Height();
			int w = rt.Width();
			/*
			ImageInfo imageInfo;
			
			pImage->GetImageInfo(&imageInfo);
			
			int h = imageInfo.Height;
			int w = imageInfo.Width;
		
			double hp = 234/(double)h;
			double wp = 480/(double)w;
			
			if(hp<1.0 || wp<1.0)
			{
				if(hp < wp)
				{
					w = (w*hp);
				}
				else
				{
					h = (h*wp);
				}
			}
			if(h>rt.Height())
			{
				h = rt.Height();
			}
			else
			{
				y = (rt.Height() - h)/2;
			}
			if(w>rt.Width())
			{
				w = rt.Width();
			}
			else
			{
				x = (rt.Width() - w)/2;
			}
			*/
			CRect rect;
			//临时加上相框 20071217 by lxz
			if(w >= 479 && h >= 233)
				rect = CRect(24, 24, 480-24-1, 234-24+1);
			else
				rect = CRect(rt.left+x, rt.top+y, rt.left+x+w-1, rt.top+y+h-1);
			pImage->Draw(hdc->m_hDC, &rect, NULL); 
						
			if(w < rt.Width())
			{
				CBrush brsh = RGB(0, 0, 0);
				CRect rt1 = CRect(rt.left, rt.top, rt.left+x, rt.bottom);
				CRect rt2 = CRect(rt.left+x+w-1, rt.top, rt.right, rt.bottom);
				hdc->FillRect(&rt1, &brsh);
				hdc->FillRect(&rt2, &brsh);
			}
			
			owner_->ReleaseDC(hdc);
			
			pImage->Release(); 
		} 
		
		pImgFactory-> Release(); 
	} 
	CoUninitialize(); 
} 

