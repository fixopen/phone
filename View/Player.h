// Player.h: interface for the Player class.
//
//////////////////////////////////////////////////////////////////////

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
	Player(CWnd* owner);
	virtual ~Player();
	
	bool InitPlayer();
	bool PlayerFile(char *filename);
	bool StopPlayer();
	bool GetVideoFileInfo(LPMEDIA_INFO info);
	bool SetImageList(std::vector<CString> files);
	bool PlayerImage();
	bool StopImage();
	bool Up();
	bool Down();
	bool ExitPlayer(BOOL flag = TRUE);
	bool SetAllScreenPlayer(BOOL flag = TRUE); //全屏播放
	void SetOwner(CWnd *pWnd){owner_ = pWnd;}
	void SetPlayerOwner(CWnd *pWnd){playerOwner_ = pWnd;}
	void SetVolume(int volume)
	{
		//1-15
		//0-180
		if(volume > 15)
			volume = 15;
		plySetVolume(volume * 12);
	}


	bool isActiveMode_;
	bool isPlaying_;
	std::vector<CString> files_;
	int index_;
	int type_;
	BOOL isPlayerRun;
	//CString currentFile_;

	CxImage* image_;
	CWnd* owner_;			//playerDlg_					播放窗口
	CWnd* playerOwner_;		//MultimediaPhoneDlg			接受播放器消息的窗口
	//CWnd* 
};

#endif // !defined(AFX_PLAYER_H__606ADDB4_DE80_42E8_B118_B2F56084EB7E__INCLUDED_)
