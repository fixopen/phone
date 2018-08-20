// Player.cpp: implementation of the Player class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../resource.h"
#include "../Util/stringOp.h"
#include "../MultimediaPhoneDlg.h"
#include "../MultimediaPhone.h"

#include "Player.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Player::Player(CWnd* owner, MediaType mt)
: isPlaying_(0)
, owner_(owner)
, mt_(mt)
{
	index_ = 0;
	isActiveMode_ = TRUE;
	isPlayerRun = FALSE;
}

Player::~Player()
{
	ExitPlayer();
}

int gVoiceVolume = SOUND_DEFAULT_VOLUME;
bool Player::InitPlayer()
{
	if(!isPlayerRun)
	{
		RETAILMSG(1, (TEXT("plyCreate pre-----\r\n")));
		if (plyCreate(0, 0, 0, 0, mt_) == 0)
		{
			RETAILMSG(1, (TEXT("plyCreate ok-----\r\n")));
			//Dprintf("plyCreate ok\n");
			for (int i = 0; i < 100; ++i)
			{
				if (plyInit(playerOwner_->m_hWnd, mt_) == 0)
				{
					//Dprintf("plyInit ok\n");
					RETAILMSG(1, (TEXT("plyInit ok -----\r\n")));
					::Sleep(500);
					break;
				}
				::Sleep(50);
			}
			if(i >= 100)
			{
				RETAILMSG(1, (TEXT("plyInit error -----\r\n")));
				return false;
			}

			isPlayerRun = TRUE;
			plySetRepeat(TRUE, mt_);
			plySetAutoPreRotate(FALSE, mt_);
			if(mt_ == mtImage)
				plySetZoom(ZOOM_FILL_SCREEN, mt_);
			else
				plySetZoom(/*ZOOM_FIT_SCREEN*/ZOOM_100, mt_);

			SetVolume(gVoiceVolume);
			Sleep(10);

/*
			HWND hWnd;
			if(mt_ == mtVideo)
			{
				hWnd =::FindWindow(L"csplayer_win", L"csplayer window"); //
				if(hWnd)
					PostMessage(hWnd, WM_MOUSEMOVE, 0, 0);
			}
			else if(mt_ == mtImage)
			{
				hWnd = ::FindWindow(L"csplayer_win2", L"csplayer window2"); //
				if(hWnd)
					PostMessage(hWnd, WM_MOUSEMOVE, 0, 0);
			}
*/
			RETAILMSG(1, (TEXT("playinit ok-----\r\n")));
			return true;
		}
		RETAILMSG(1, (TEXT("plyCreate error-----\r\n")));
		return false;
	}

	else
	{
		isPlaying_ = TRUE;
		plyPlay(TRUE, mt_);
	}
	return true;
}

bool Player::ExitPlayer(BOOL flag)
{
	if(isPlayerRun)
	{
		if(flag)
		{
			HWND hWnd;
			if(mt_ == mtVideo)
			{
				hWnd =::FindWindow(L"csplayer_win0", L"csplayer window0"); //
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetScreenSaveTimer();
			}
			else if(mt_ == mtAudio)
				hWnd = ::FindWindow(L"csplayer_win1", L"csplayer window1"); //
			else if(mt_ == mtImage)
			{
				hWnd = ::FindWindow(L"csplayer_win2", L"csplayer window2"); //
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetScreenSaveTimer();
			}
			if(hWnd)
				::ShowWindow(hWnd, SW_HIDE);
		}

		MEDIA_INFO info;
		GetVideoFileInfo(&info);
		int Hour,Min,Sec;
		int tick = info.nTick;
		Hour = tick / 3600 / TICKSPERSEC;
		tick -= Hour * 3600 * TICKSPERSEC;
		Min = tick / 60 / TICKSPERSEC;
		tick -= Min * 60 * TICKSPERSEC;
		Sec = tick / TICKSPERSEC;
		
		int Hour1,Min1,Sec1;
		tick = info.nDuration;
		Hour1 = tick / 3600 / TICKSPERSEC;
		tick -= Hour1 * 3600 * TICKSPERSEC;
		Min1 = tick / 60 / TICKSPERSEC;
		tick -= Min1 * 60 * TICKSPERSEC;
		Sec1 = tick / TICKSPERSEC;
		
		int n1 = Hour*3600+Min*60+Sec;
		int n2 = Hour1*3600+Min1*60+Sec1;
		m_nPercent = 0;
		if(n2 != 0)
			m_nPercent = n1*100/n2;
		memset(m_curFilename, 0, 256*2);
		memcpy(m_curFilename, info.szFileName, wcslen(info.szFileName)*2);

	//	wprintf(L"Exit Player %s %d %d\n", m_curFilename, n1, n2);
		
		isPlaying_ = FALSE;
	
		isPlayerRun = FALSE;
		plyExit(mt_);

		if(flag)
		{
			HWND hWnd;
			for(int i = 0; i < 10; i++)
			{
				if(mt_ == mtVideo)
					hWnd =::FindWindow(L"csplayer_win0", L"csplayer window0"); //
				else if(mt_ == mtAudio)
					hWnd = ::FindWindow(L"csplayer_win1", L"csplayer window1"); //
				else if(mt_ == mtImage)
					hWnd = ::FindWindow(L"csplayer_win2", L"csplayer window2"); //
				if(hWnd)
				   Sleep(50);
				else
					break;
			}
			if(hWnd)
			{
				::ShowWindow(hWnd, SW_HIDE);
			}
		}
		if(mt_ == mtVideo)
		{
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetScreenSaveTimer();
		}
		else if(mt_ == mtImage)
		{
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetScreenSaveTimer();
		}
	}

	return true;
	
}

bool Player::ExitPlayerAudio()
{
	isPlaying_ = FALSE;
	isPlayerRun = FALSE;
	plyExit(mt_);
	return true;
}
bool Player::GetVideoFileInfo(LPMEDIA_INFO info)
{
	int ret;
    ret =  plyGetMediaInfo(info, mt_);   //debug
//	wprintf(info->szFileName);
//	Dprintf("  timer = %d %d\n",  info->nDuration, info->nTick);
	if(ret == 0)
		return true;
	return false;
}

bool Player::PlayerFile(char *filename)
{
	isPlaying_ = TRUE;
//	if (isActiveMode_)
	{
		TCHAR wideFilename[256] = {0};
		/*
		size_t wideContentLength = MultiByteToWideChar(CP_ACP, 0, filename, strlen(filename), 0, 0);
		mbstowcs(wideFilename, filename, wideContentLength);
		*/
	
		CString s = Util::StringOp::ToCString(filename);

		if(owner_)
		{
			CRect rt;
			owner_->GetWindowRect(&rt);
			plySetWndPos(rt.left, rt.top, rt.Width(), rt.Height(), mt_);

// 			if(mt_ == mtImage)
// 			{
// 				HWND hWnd = ::FindWindow(L"csplayer_win2", L"csplayer window2"); 
// 				Dprintf("mtImage Hwnd %x\r\n", hWnd);
// 				if(hWnd)
// 				{
// 					::SetWindowPos(hWnd, HWND_BOTTOM, rt.left, rt.top, rt.Width(), rt.Height(), 0);
// 				}
// 			}
		}
		if (plyOpen((LPTSTR)(LPCTSTR)s/*.GetBuffer(256)*/, mt_) == 0)
		{
			m_curFilename[0] = L'\0';
		//	plySetZoom(ZOOM_FIT_SCREEN, mt_);
			//Dprintf("plyOpen(filepath) ok\n");
			return true;
		}
		return false;
	}

	return false;
}

bool Player::PlayerFile(CString filename)
{
	isPlaying_ = TRUE;
//	if (isActiveMode_)
	{
		if(owner_)
		{
			CRect rt;
			owner_->GetWindowRect(&rt);
			plySetWndPos(rt.left, rt.top, rt.Width(), rt.Height(), mt_);

// 			if(mt_ == mtImage)
// 			{
// 				HWND hWnd = ::FindWindow(L"csplayer_win2", L"csplayer window2"); 
// 				Dprintf("mtImage Hwnd %x\r\n", hWnd);
// 				if(hWnd)
// 				{
// 					::SetWindowPos(hWnd, HWND_BOTTOM, rt.left, rt.top, rt.Width(), rt.Height(), 0);
// 				}
// 			}

		}

		if (plyOpen(/*filename.GetBuffer(256)*/(LPTSTR)(LPCTSTR)filename, mt_) == 0)
		{
			m_curFilename[0] = L'\0';
		//	plySetZoom(ZOOM_FIT_SCREEN, mt_);
			//Dprintf("plyOpen(filepath) ok\n");
			return true;
		}
		return false;
	}

	return false;
}

bool Player::SetImageList(std::vector<CString> files, int index)
{
	bool result = false;
	files_ = files;
	if(index == 0)
		index_ = index;              
	result = true;
	return result;
}

extern void DrawImage(LPCTSTR szFileImage, CDC *pdc, CRect rect, BOOL isOffset = FALSE);
extern void ReleaseJPG(void *ptr);
extern void JpegDraw(HDC hdc, long x, long y, long cx, long cy, int w, int h, BYTE *ptr);
extern unsigned char *JpegFileToRGB(char *fileName,
							 char *rgbbuf,
							 UINT *width,
							 UINT *height);

bool Player::PlayerImage()
{
	bool result = false;
	isPlaying_ = TRUE;

	if(files_.size() == 0)
		return false;
	CString filename = files_[index_];
	DWORD t, a;
//	int ret1 = DMemprintf("LoadImage 0", t, a);
	DrawImage_((LPCTSTR)filename);
	
//	int ret2 = DMemprintf("LoadImage 1", t, a);
	/*
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
*/
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
	ExitPlayer(TRUE);
	return true;
}

int Player::Cur()
{
	if(files_.size() == 0)
		return 0;
	
	bool result = false;

	if (index_ >= 0 && index_ < files_.size())
	{
		if(mt_ == mtImage)
			PlayerImage();
		else
			PlayerFile(files_[index_]);
	}
		
	return index_;
}


int Player::Up()
{
	if(files_.size() == 0)
		return 0;

	bool result = false;
	--index_;
	if (index_ < 0)
		index_ = files_.size() - 1;
	if(mt_ == mtImage)
		PlayerImage();
	else
		PlayerFile(files_[index_]);

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
	if(mt_ == mtImage)
		PlayerImage();
	else
		PlayerFile(files_[index_]);
	return index_;
}

bool Player::StopPlayer()
{
	isPlayerRun = 0;
	if( plyStop(mt_) == 0)
		return true;
	return false;
}

bool Player::PausePlayer(BOOL isPause)
{
	if(isPlayerRun)
	{
		plyPlay(!isPause, mt_);
	}
	return true;
}

bool Player::ResumePlayer()
{
	if(wcslen(m_curFilename) != 0)
	{
		if(!isPlayerRun)
		{
			InitPlayer();
		}

		isPlaying_ = TRUE;
	//	if (isActiveMode_)
		{
			if(owner_)
			{
				CRect rt;
				owner_->GetWindowRect(&rt);
				plySetWndPos(rt.left, rt.top, rt.Width(), rt.Height(), mt_);

				if(mt_ == mtVideo)
				{
					CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
					main->m_pMainDlg->SetTimer_(TRUE);
				}
			}
			RETAILMSG(1, (TEXT("plyOpen pre-----\r\n")));
			if (plyOpen(m_curFilename, mt_) == 0)
			{
				RETAILMSG(1, (TEXT("plyOpen ok-----\r\n")));
				m_curFilename[0] = L'\0',
			//	plySetZoom(ZOOM_FIT_SCREEN, mt_);
				RETAILMSG(1, (TEXT("PlayPos %d-----\r\n")), m_nPercent);
				PlayPos(m_nPercent);
				RETAILMSG(1, (TEXT("PlayPos ok-----\r\n")));

				return true;
			}
		}
	}
	return false;
}

bool Player::SetAllScreenPlayer(BOOL flag)
{
	if(flag)
	{
		if(plySetWndPos(0, 0, 800, 480, mt_) == 0)
		{
			if(mt_ == mtImage)
			{
			//	plySetZoom(ZOOM_FILL_SCREEN, mt_);
				Cur();
			}
			else if(mt_ == mtVideo)
			{
				CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
				main->m_pMainDlg->SetTimer_(FALSE);
			}
// 			else
// 				plySetZoom(ZOOM_FIT_SCREEN, mt_);

			return true;
		}
	}
	else
	{
		if(owner_)
		{
			CRect rt;
			owner_->GetWindowRect(&rt);
		//	plySetZoom(ZOOM_FIT_SCREEN, mt_);
			if(plySetWndPos(rt.left, rt.top, rt.Width(), rt.Height(), mt_) == 0)
			{
				if(mt_ == mtImage)
					Cur();
				else if(mt_ == mtVideo)
				{
					CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
					main->m_pMainDlg->SetTimer_(TRUE);
				}
				return true;
			}
		}
	}
	return false;
}

//#include   <INITGUID.h>   
//#include   <imaging.h> 
void   Player::DrawImage_(LPCTSTR szFileImage) 
{ 
	/*
	CDC* hdc = owner_->GetDC();
	CRect rt;
	owner_->GetClientRect(rt);
	int x = 0;
	int y = 0;
	int h = rt.Height();
	int w = rt.Width();
	
	CRect rect;
	//临时加上相框 20071217 by lxz
	if(w >= 799 && h >= 479)
		rect = CRect(0, 0, 800, 480);
	else
		rect = CRect(rt.left+x, rt.top+y, rt.left+x+w-1, rt.top+y+h-1);
	DrawImage(szFileImage, hdc, rect);
				
	if(w < rt.Width())
	{
		CBrush brsh = RGB(0, 0, 0);
		CRect rt1 = CRect(rt.left, rt.top, rt.left+x, rt.bottom);
		CRect rt2 = CRect(rt.left+x+w-1, rt.top, rt.right, rt.bottom);
		hdc->FillRect(&rt1, &brsh);
		hdc->FillRect(&rt2, &brsh);
	}
	
	owner_->ReleaseDC(hdc);
	*/
	InitPlayer();
	isPlaying_ = TRUE;
	{
		TCHAR wideFilename[256] = {0};
	
		CString s = szFileImage;

		if(owner_)
		{
			CRect rt;
			owner_->GetWindowRect(&rt);
			plySetWndPos(rt.left, rt.top, rt.Width(), rt.Height(), mt_);

// 			if(mt_ == mtImage)
// 			{
// 				HWND hWnd = ::FindWindow(L"csplayer_win2", L"csplayer window2"); 
// 				Dprintf("mtImage Hwnd %x\r\n", hWnd);
// 				if(hWnd)
// 				{
// 					::SetWindowPos(hWnd, HWND_BOTTOM, rt.left, rt.top, rt.Width(), rt.Height(), 0);
// 					::ShowWindow(hWnd, SW_HIDE);
// 				}
// 			}
		}
		if (plyOpen((LPTSTR)(LPCTSTR)s/*.GetBuffer(256)*/, mt_) == 0)
		{
			m_curFilename[0] = L'\0';
			//plySetZoom(ZOOM_FIT_SCREEN, mt_);
			//Dprintf("plyOpen(filepath) ok\n");
			return;
		}
		return;
	}
	return ;
} 

