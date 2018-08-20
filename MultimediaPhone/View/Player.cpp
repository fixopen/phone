// Player.cpp: implementation of the Player class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Player.h"
#include "../MultimediaPhoneDlg.h"
#include "../MultimediaPhone.h"
#include "../resource.h"
#include "../Util/stringOp.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace View {
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
        if (!isPlayerRun)
        {
            if (plyCreate(0, 0, 0, 0, mt_) == 0)
            {
                plyInit(playerOwner_->m_hWnd, mt_);
                isPlayerRun = TRUE;
                /*::Sleep(1 * 1000);
                for (int i = 0; i < 100; ++i)
                {
                    if (plyInit(playerOwner_->m_hWnd, mt_) == 0)
                    {
                        ::Sleep(500);
                        break;
                    }
                    ::Sleep(50);
                }
                isPlayerRun = TRUE;
                plySetRepeat(TRUE, mt_);
                plySetAutoPreRotate(FALSE, mt_);
                if (mt_ == mtImage)
                    plySetZoom(ZOOM_FILL_SCREEN, mt_);
                else
                    plySetZoom(/ *ZOOM_FIT_SCREEN* /ZOOM_100, mt_);

                SetVolume(gVoiceVolume);
                Sleep(10);*/
                return true;
            }
            return false;
        }
        else
        {
            isPlaying_ = TRUE;
            plyPlay(TRUE, mt_);
        }
        return TRUE;
    }

    bool Player::ExitPlayer(BOOL flag)
    {
        if (isPlayerRun)
        {
            if (flag)
            {
                HWND hWnd;
                if (mt_ == mtVideo)
                    hWnd =::FindWindow(L"csplayer_win0", L"csplayer window0"); //
                else if (mt_ == mtAudio)
                    hWnd = ::FindWindow(L"csplayer_win1", L"csplayer window1"); //
                else if (mt_ == mtImage)
                    hWnd = ::FindWindow(L"csplayer_win2", L"csplayer window2"); //
                if (hWnd)
                    ::ShowWindow(hWnd, SW_HIDE);
            }

            MEDIA_INFO info;
            GetVideoFileInfo(&info);
            int Hour, Min, Sec;
            int tick = info.nTick;
            Hour = tick / 3600 / TICKSPERSEC;
            tick -= Hour * 3600 * TICKSPERSEC;
            Min = tick / 60 / TICKSPERSEC;
            tick -= Min * 60 * TICKSPERSEC;
            Sec = tick / TICKSPERSEC;

            int Hour1, Min1, Sec1;
            tick = info.nDuration;
            Hour1 = tick / 3600 / TICKSPERSEC;
            tick -= Hour1 * 3600 * TICKSPERSEC;
            Min1 = tick / 60 / TICKSPERSEC;
            tick -= Min1 * 60 * TICKSPERSEC;
            Sec1 = tick / TICKSPERSEC;

            int n1 = Hour * 3600 + Min * 60 + Sec;
            int n2 = Hour1 * 3600 + Min1 * 60 + Sec1;
            m_nPercent = 0;
            if (n2 != 0)
                m_nPercent = n1 * 100 / n2;
            memset(m_curFilename, 0, 256 * 2);
            memcpy(m_curFilename, info.szFileName, wcslen(info.szFileName) * 2);

            //wprintf(L"Exit Player %s %d %d\n", m_curFilename, n1, n2);

            isPlaying_ = FALSE;

            isPlayerRun = FALSE;
            plyExit(mt_);

            if (flag)
            {
                HWND hWnd;
                for (int i = 0; i < 10; ++i)
                {
                    if( mt_ == mtVideo)
                        hWnd =::FindWindow(L"csplayer_win0", L"csplayer window0"); //
                    else if (mt_ == mtAudio)
                        hWnd = ::FindWindow(L"csplayer_win1", L"csplayer window1"); //
                    else if (mt_ == mtImage)
                        hWnd = ::FindWindow(L"csplayer_win2", L"csplayer window2"); //
                    if (hWnd)
                        Sleep(50);
                    else
                        break;
                }
                if (hWnd)
                {
                    ::ShowWindow(hWnd, SW_HIDE);
                }
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
        int ret = plyGetMediaInfo(info, mt_);   //debug
        if (ret == 0)
            return true;
        return false;
    }

    bool Player::PlayerFile(char *filename)
    {
        isPlaying_ = TRUE;
        //if (isActiveMode_)
        {
            TCHAR wideFilename[256] = {0};
            CString s = Util::StringOp::ToCString(filename);

            if (owner_)
            {
                CRect rt;
                owner_->GetWindowRect(&rt);
                plySetWndPos(rt.left, rt.top, rt.Width(), rt.Height(), mt_);
            }
            //plyOpen(_T("C:/flashdrv/my_video/青花瓷.mp4"), mt_);
            if (plyOpen((LPTSTR)(LPCTSTR)s, mt_) == 0)
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
            if (owner_)
            {
                CRect rt;
                owner_->GetWindowRect(&rt);
                plySetWndPos(rt.left, rt.top, rt.Width(), rt.Height(), mt_);
            }
            //plyOpen(_T("C:/flashdrv/my_video/青花瓷.mp4"), mt_);
            if (plyOpen((LPTSTR)(LPCTSTR)filename, mt_) == 0)
            {
                m_curFilename[0] = L'\0';
                return true;
            }
            return false;
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

    extern void DrawImage(LPCTSTR szFileImage, CDC *pdc, CRect rect, BOOL isOffset = FALSE);
    extern void ReleaseJPG(void *ptr);
    extern void JpegDraw(HDC hdc, long x, long y, long cx, long cy, int w, int h, BYTE *ptr);
    extern unsigned char *JpegFileToRGB(char *fileName, char *rgbbuf, UINT *width, UINT *height);

    void Player::SetVolume(int volume)
    {
        //1-15
        //0-180
        if(volume > 15)
            volume = 15;
        plySetVolume(volume * 12, mt_);
        //extern int gVoiceVolume;
        gVoiceVolume = volume;
    }

    bool Player::PlayerImage()
    {
        bool result = false;
        isPlaying_ = TRUE;

        if(files_.size() == 0)
            return false;
        CString filename = files_[index_];
        DWORD t, a;
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
        if (plyStop(mt_) == 0)
            return true;
        return false;
    }

    bool Player::PausePlayer(BOOL isPause)
    {
        if (isPlayerRun)
        {
            plyPlay(!isPause, mt_);
        }
        return true;
    }

    bool Player::ResumePlayer()
    {
        if (wcslen(m_curFilename) != 0)
        {
            if (!isPlayerRun)
            {
                InitPlayer();
            }

            isPlaying_ = TRUE;
            //	if (isActiveMode_)
            {
                if (owner_)
                {
                    CRect rt;
                    owner_->GetWindowRect(&rt);
                    plySetWndPos(rt.left, rt.top, rt.Width(), rt.Height(), mt_);

                    if (mt_ == mtVideo)
                    {
                        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
                        main->m_pMainDlg->SetTimer_(TRUE);
                    }
                }
                if (plyOpen(m_curFilename, mt_) == 0)
                {
                    m_curFilename[0] = L'\0';
                    PlayPos(m_nPercent);
                    return true;
                }
            }
        }
        return false;
    }

    bool Player::SetAllScreenPlayer(BOOL flag)
    {
        if (flag)
        {
            if (plySetWndPos(0, 0, 800 * X_XISHU, 480 * Y_XISHU, mt_) == 0)
            {
                if (mt_ == mtImage)
                {
                    //plySetZoom(ZOOM_FILL_SCREEN, mt_);
                    Cur();
                }
                else if (mt_ == mtVideo)
                {
                    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
                    main->m_pMainDlg->SetTimer_(FALSE);
                }
                return true;
            }
        }
        else
        {
            if (owner_)
            {
                CRect rt;
                owner_->GetWindowRect(&rt);
                //plySetZoom(ZOOM_FIT_SCREEN, mt_);
                CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
                CRect position;
                main->GetWindowRect(&position);
                position.left += 7 * X_XISHU;
                position.top += 50 * Y_XISHU;
                position.right = position.left + 522 * X_XISHU;
                position.bottom = position.top + 213 * Y_XISHU;
                if (plySetWndPos(position.left, position.top, position.Width(), position.Height(), mt_) == 0)
                {
                    if (mt_ == mtImage)
                        Cur();
                    else if (mt_ == mtVideo)
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

    //#include <INITGUID.h>   
    //#include <imaging.h> 
    void Player::DrawImage_(LPCTSTR szFileImage) 
    { 
        InitPlayer();
        isPlaying_ = TRUE;
        {
            TCHAR wideFilename[256] = {0};

            CString s = szFileImage;

            if (owner_)
            {
                CRect rt;
                owner_->GetWindowRect(&rt);
                plySetWndPos(rt.left, rt.top, rt.Width(), rt.Height(), mt_);
            }
            if (plyOpen(s.GetBuffer(256), mt_) == 0)
            {
                m_curFilename[0] = L'\0';
                return;
            }
            return;
        }
        return;
    }
}
