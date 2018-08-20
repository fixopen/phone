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
: isPlaying_(false)
, image_(new CxImage())
, owner_(owner)
{
	index_ = 0;
	type_ = 0;  //ͼƬ
	isActiveMode_ = false;
	isPlayerRun = FALSE;
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
			Dprintf("plyCreate ok\n");
			::Sleep(1 * 1000);
			for (int i = 0; i < 100; ++i)
			{
				if (plyInit(playerOwner_->m_hWnd) == 0)
				{
					Dprintf("plyInit ok\n");
					::Sleep(500);
					break;
				}
				::Sleep(50);
			}
			plySetRepeat(TRUE);
			plySetAutoPreRotate(FALSE);
			plySetZoom(ZOOM_STRETCH_SCREEN);
			SetVolume(SOUND_DEFAULT_VOLUME);
			Sleep(10);
			isPlayerRun = TRUE;
			return true;
		}
		return false;
	}

	else
	{
		isPlaying_ = TRUE;
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
		isPlaying_ = FALSE;
		plyPlay(FALSE);
		if(flag)
		{
			HWND hWnd = ::FindWindow(L"csplayer_win", L"csplayer window"); //
			::ShowWindow(hWnd, SW_HIDE);
		}
	}
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
	isPlaying_ = true;
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
			Dprintf("plyOpen(filepath) ok\n");
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

bool Player::PlayerImage()
{
	bool result = false;
	isPlaying_ = true;

	if(files_.size() == 0)
		return false;
	CString filename = files_[index_];
	bool ret;
	if(filename != "")
		ret = image_->Load((LPCTSTR)filename, CXIMAGE_FORMAT_JPG);
	if(owner_)
	{
		CDC* hdc = owner_->GetDC();
		CRect rt;
		owner_->GetClientRect(rt);
		int x = 0;
		int y = 0;
		int h = image_->GetHeight(); 
		int w = image_->GetWidth();

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
		CBrush brsh = RGB(0, 0, 0);
		
		hdc->FillRect(&rt, &brsh);
		image_->Draw(hdc->m_hDC, rt.left+x, rt.top+y, w, h, 0, TRUE);
		owner_->ReleaseDC(hdc);
	}
		
	return result;
}

bool Player::StopImage()
{
	isPlaying_ = FALSE;
	bool result = false;
	return result;
}

bool Player::Up()
{
	if(files_.size() == 0)
		return false;

	bool result = false;
	--index_;
	if (index_ < 0)
		index_ = files_.size() - 1;
	PlayerImage();
	return result;
}

bool Player::Down()
{
	if(files_.size() == 0)
		return false;

	bool result = false;
	++index_;
	if (index_ > (files_.size() - 1))
		index_ = 0;
	PlayerImage();
	return result;
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
