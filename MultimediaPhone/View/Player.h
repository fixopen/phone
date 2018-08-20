// Player.h: interface for the Player class.
//
//////////////////////////////////////////////////////////////////////
#undef _PLAYER_H_
#define _PLAYER_H_

#if !defined(AFX_PLAYER_H__606ADDB4_DE80_42E8_B118_B2F56084EB7E__INCLUDED_)
#define AFX_PLAYER_H__606ADDB4_DE80_42E8_B118_B2F56084EB7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../inc/tcpmplib/csplayer.h"
#include "../inc/cximage/ximage.h"

#include <vector>

namespace View {
    class Player
    {
    public:
        Player(CWnd* owner, MediaType mt);
        virtual ~Player();

        bool InitPlayer();
        bool PlayerFile(char *filename);
        bool PlayerFile(CString filname);
        bool StopPlayer();
        bool ExitPlayerAudio();
        int PlayPos(int percent){return plySetPos(TRACKMAX*percent/100, mt_);}
        void PlayForward(){plyForward(mt_);}
        bool GetVideoFileInfo(LPMEDIA_INFO info);
        bool SetImageList(std::vector<CString> files);
        bool PlayerImage();
        void DrawImage_(LPCTSTR   szFileImage);
        bool StopImage();
        bool PausePlayer(BOOL isPause);
        bool ResumePlayer();

        int Up();
        int Down();
        int Cur();
        bool ExitPlayer(BOOL flag = TRUE);
        bool SetAllScreenPlayer(BOOL flag = TRUE); //ȫ������
        void SetOwner(CWnd *pWnd) {
            owner_ = pWnd;
        }
        void SetPlayerOwner(CWnd *pWnd) {
            playerOwner_ = pWnd;
        }
        void SetVolume(int volume);

        MediaType mt_;
        TCHAR	m_curFilename[256];
        int		m_nPercent;

        BOOL isActiveMode_;  //�Ƿ�������״̬,���ڵ������  1 �ֶ��� 0 ����
        BOOL isPlaying_;      //0 �޲���  1 ����

        std::vector<CString> files_;
        int index_;
        BOOL isPlayerRun;
        //CString currentFile_;

        CWnd* owner_;			//playerDlg_					���Ŵ���
        CWnd* playerOwner_;		//MultimediaPhoneDlg			���ܲ�������Ϣ�Ĵ���
        //CWnd* 
    };
}

#endif // !defined(AFX_PLAYER_H__606ADDB4_DE80_42E8_B118_B2F56084EB7E__INCLUDED_)
