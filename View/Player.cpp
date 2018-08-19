// Player.cpp: implementation of the Player class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../resource.h"
#include "../Util/stringOp.h"
#include "../MultimediaPhoneDlg.h"
#include "../MultimediaPhone.h"
#include <math.h>
#include "Player.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern void ClearPlayerReg();

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
	zoom_ = ZOOM_FIT_SCREEN;
	rotate_ = ROTATE_0;
	m_videoAllScreen = FALSE;
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
			m_plyRect.left = 0;
			m_plyRect.right = 0;
			m_plyRect.top = 0;
			m_plyRect.bottom = 0;
			plySetRepeat(TRUE, mt_);
			plySetAutoPreRotate(FALSE, mt_);
			if(mt_ == mtImage)
				plySetZoom(ZOOM_FIT_SCREEN, mt_);
			else
				plySetZoom(/*ZOOM_FIT_SCREEN*/ZOOM_100, mt_);

			SetVolume(gVoiceVolume);
			Sleep(10);
				
			extern void GMute_();
			GMute_();

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
				extern void GMute(BOOL isOn);
 				GMute(TRUE);
				hWnd =::FindWindow(L"csplayer_win0", L"csplayer window0"); //
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetScreenSaveTimer();

			}
			else if(mt_ == mtAudio)
			{	
				extern void GMute(BOOL isOn);
				GMute(TRUE);
				hWnd = ::FindWindow(L"csplayer_win1", L"csplayer window1"); //
			}
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
// 		extern void GMute(BOOL isOn);
// 		GMute(TRUE);

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
	
	ClearPlayerReg();
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
		CString s = Util::StringOp::ToCString(filename);

		if(owner_)
		{
			CRect rt;
			owner_->GetWindowRect(&rt);
			plySetWndPos(rt.left, rt.top, rt.Width(), rt.Height(), mt_);
		}
		else
		{
			plySetWndPos(0, 0, 800, 480, mt_);
		}
		if (plyOpen((LPTSTR)(LPCTSTR)s/*.GetBuffer(256)*/, mt_) == 0)
		{
			m_curFilename[0] = L'\0';
			return true;
		}
		return false;
	}

	return false;
}

bool Player::PlayerFile(CString sfilename, CRect rt)
{
	isPlaying_ = TRUE;
	//	if (isActiveMode_)
	{
		plySetWndPos(rt.left, rt.top, rt.Width(), rt.Height(), mt_);
	
		if (plyOpen((LPTSTR)(LPCTSTR)sfilename/*.GetBuffer(256)*/, mt_) == 0)
		{
			m_curFilename[0] = L'\0';
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
		}

		if (m_videoAllScreen)
		{

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
	DrawImage_((LPCTSTR)filename);
	
	return result;
}

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

bool Player::RotatePicture(CString filename)
{
	if (plyOpen((LPTSTR)(LPCTSTR)filename, mt_) == 0)
	{
		if (rotate_ < 270)
		{
			rotate_ += 90;
		}
		else
		{
			rotate_ = 0;
		}

		if (owner_)
		{
			if((rotate_ == 90) || (rotate_ == 270))
			{
				float width_ = 0.0;
				float left_ = 0.0;
				float rectWidth = m_plyRect.Width();
				float rectHeight = m_plyRect.Height();
				int width = 0;
				width_ = rectHeight*rectHeight/rectWidth;
				width = ceil(width_);
				CRect rt;
				owner_->GetWindowRect(&rt);
				left_ = (rt.Width() - width_)/2;
				rt.left += ceil(left_);
				plySetWndPos(rt.left, m_plyRect.top, width, m_plyRect.Height(), mt_);
			}
			else
			{
				plySetWndPos(m_plyRect.left, m_plyRect.top, m_plyRect.Width(), m_plyRect.Height(), mt_);
			}
		}

		plySetRotate(rotate_, mt_);
		return TRUE;
	}
	
	return FALSE;
}

bool Player::ZoomPlay(CString filename)
{
	if (plyOpen((LPTSTR)(LPCTSTR)filename, mt_) == 0)
	{
		if (zoom_ < ZOOM_200)
		{
			zoom_ += 50;
		}
		plySetZoom(zoom_, mt_);
		return TRUE;
	}

	return FALSE;
}

bool Player::NarrowPlay(CString filename)
{
	if (plyOpen((LPTSTR)(LPCTSTR)filename, mt_) == 0)
	{
		if (zoom_ > ZOOM_50)
		{
			zoom_ -= 50;
		}
		plySetZoom(zoom_, mt_);
		return TRUE;
	}
	
	return FALSE;
}

int Player::First()
{
	if (files_.size() == 0)
	{
		return 0;
	}
	
	index_ = 0;
	if(mt_ == mtImage)
		PlayerImage();
	else
		PlayerFile(files_[index_]);

	return index_;
}

int Player::Last()
{
	if (files_.size() == 0)
	{
		return 0;
	}
	
	index_ = files_.size() - 1;
	if(mt_ == mtImage)
		PlayerImage();
	else
		PlayerFile(files_[index_]);
	
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
	if( plyStop(mt_) == 0)
		return true;
	return false;
}

bool Player::PausePlayer(BOOL isPause)
{
	if(isPlayerRun)
	{
		plyPlay(!isPause, mt_);
		isPlayerRun = 1;
		return true;
	}
	return false;
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
		if(mt_ == mtImage)
		{
			plySetZoom(ZOOM_FIT_SCREEN, mt_);
		}
		else if(mt_ == mtVideo)
		{
			CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
			main->m_pMainDlg->SetTimer_(FALSE);
			m_videoAllScreen = TRUE;
			plySetZoom(ZOOM_FIT_SCREEN, mt_);
		}
		Cur();
		return true;
	}
	else
	{
		if(owner_)
		{
			if(mt_ == mtImage)
			{
				Cur();
			}
			else if(mt_ == mtVideo)
			{
				Cur();
				m_videoAllScreen = FALSE;
				CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
				main->m_pMainDlg->SetTimer_(TRUE);
			}
			return true;
		}
	}
	return false;
}

void   Player::DrawImage_(LPCTSTR szFileImage) 
{
	InitPlayer();
	isPlaying_ = TRUE;
	{
		TCHAR wideFilename[256] = {0};
	
		CString s = szFileImage;
		float width = 0;
		float height = 0;

		if(owner_)
		{
			CxImage *xImage;
			xImage = new CxImage();
			if(xImage)
			{
				bool ret = false;
				if(wcsstr(szFileImage,(L".png")) > 0)
				{
					ret = xImage->Load(szFileImage, CXIMAGE_FORMAT_PNG);
				}
				else if(wcsstr(szFileImage,(L".jpg")) > 0 || wcsstr(szFileImage,(L".JPG")) > 0)
				{
					ret = xImage->Load(szFileImage, CXIMAGE_FORMAT_JPG);
				}
				else if(wcsstr(szFileImage,(L".bmp")) > 0 || wcsstr(szFileImage,(L".BMP")) > 0)
				{
					ret = xImage->Load(szFileImage, CXIMAGE_FORMAT_BMP);
				}

				if(ret)
				{
					width = xImage->GetWidth();
					height = xImage->GetHeight();
					delete xImage;
				}
			}
			CRect rt;
			owner_->GetWindowRect(&rt);
			
			float rtWidth = rt.Width();
			float rtHeight = rt.Height();
			if(width < rtWidth && height < rtHeight)
			{
				m_plyRect.left = rt.left + ceil((rtWidth-width)/2);
				m_plyRect.right = m_plyRect.left + width;
				m_plyRect.top = rt.top + ceil((rtHeight-height)/2);
				m_plyRect.bottom = m_plyRect.top + height;
			}
			else
			{
				if(width/height > rtWidth/rtHeight)
				{
					float height_ = 0.0;
					float top_ = 0.0;
					height_ = height*rtWidth/width;
					height = ceil(height_);
					top_ += (rtHeight - height_)/2;
					rt.top += ceil(top_);
					
					m_plyRect.left = rt.left;
					m_plyRect.right = rt.left + rt.Width();
					m_plyRect.top = rt.top;
					m_plyRect.bottom = rt.top + height;
				}
				else
				{
					float width_ = 0.0;
					float left_ = 0.0;
					width_ = width*rtHeight/height;
					width = ceil(width_);
					left_ = (rtWidth - width_)/2;
					rt.left += ceil(left_);
					
					m_plyRect.left = rt.left;
					m_plyRect.right = rt.left + width;
					m_plyRect.top = rt.top;
					m_plyRect.bottom = rt.top + rt.Height();
				}
			}
		}
		if (plyOpen((LPTSTR)(LPCTSTR)s, mt_) == 0)
		{
			m_curFilename[0] = L'\0';

 			zoom_ = ZOOM_FIT_SCREEN;
			plySetZoom(zoom_, mt_);
 
			plySetRotate(0, mt_);
 			plySetWndPos(m_plyRect.left, m_plyRect.top, m_plyRect.Width(), m_plyRect.Height(), mt_);
			
			CRect rt;
			owner_->GetWindowRect(&rt);
			//	HDC hdc = ::GetDC(owner_->m_hWnd);
			HDC hdc = ::GetDC(NULL);
			CBrush bBr = RGB(0, 0, 0); 
			::FillRect(hdc, rt, (HBRUSH)bBr.m_hObject);
			//::ReleaseDC(owner_->m_hWnd, hdc);
			::ReleaseDC(NULL, hdc);
			
			return;
		}
		return;
	}
	return ;
} 

