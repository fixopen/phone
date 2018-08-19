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

#include "../inc/csplayer.h"
#include "../inc/ximage.h"

#include <vector>

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
	bool SetImageList(std::vector<CString> files, int index = 0);
	bool PlayerImage();
	void DrawImage_(LPCTSTR   szFileImage);
	bool StopImage();
	bool PausePlayer(BOOL isPause);
	bool ResumePlayer();
	bool NarrowPlay(CString filename);   //缩小显示
	bool ZoomPlay(CString filename);	 //放大显示	
	bool RotatePicture(CString filename);    //旋转图片

	int Up();
	int Down();
	int First();
	int Last();
	int Cur();
	bool ExitPlayer(BOOL flag = TRUE);
	bool SetAllScreenPlayer(BOOL flag = TRUE); //全屏播放
	void SetOwner(CWnd *pWnd){owner_ = pWnd;}
	void SetPlayerOwner(CWnd *pWnd){playerOwner_ = pWnd;}
	void SetVolume(int volume)
	{
		//1-15
		//0-180
		if(mt_ != mtImage)
		{
			waveOutSetVolume(NULL, 0xFFFFFFFF);
			if(volume > 15)
			volume = 15;
			plySetVolume(volume * 12, mt_);
			extern int gVoiceVolume;
			gVoiceVolume = volume;
		}
	}

	MediaType mt_;
	TCHAR	m_curFilename[256];
	int		m_nPercent;
	
	BOOL isActiveMode_;  //是否是屏保状态,对于电子相册  1 手动， 0 屏保
	BOOL isPlaying_;      //0 无播放  1 播放

	std::vector<CString> files_;
	int index_;
	unsigned int zoom_;     //图片显示大小
	unsigned int rotate_;
	BOOL isPlayerRun;
	//CString currentFile_;

	CWnd* owner_;			//playerDlg_					播放窗口
	CWnd* playerOwner_;		//MultimediaPhoneDlg			接受播放器消息的窗口 
};

#endif // !defined(AFX_PLAYER_H__606ADDB4_DE80_42E8_B118_B2F56084EB7E__INCLUDED_)
