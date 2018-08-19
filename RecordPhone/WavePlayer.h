#pragma once

#include <mmsystem.h>
#include <string>
#include "Util/SmartPtr.h"
#include "SoundSegment.h"
#include "ContactInfo.h"
#include "Resource.h"
#include "WaveUtil.h"

class WavePlayer : public CDialog {
	DECLARE_DYNAMIC(WavePlayer)

public:
	static WavePlayer* const WavePlayer::Instance();
	WavePlayer(CWnd* pParent = NULL);   // standard constructor
	virtual ~WavePlayer();

// Dialog Data
	enum { IDD = IDD_DIALOG_WAVE_PLAYER };

    bool const Start(Util::shared_ptr<SoundSegment>& soundSegment);
    bool const Pause();
    bool const Resume();
    bool const Stop();
    bool const IsPlaying() const {
        return isPlaying_;
    }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg LRESULT OnMM_WOM_OPEN(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMM_WOM_DONE(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMM_WOM_CLOSE(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
private:
	HWAVEOUT waveOut_;
	unsigned char* buffer_;
	PWAVEHDR waveHeader_;
	WAVEFORMATEX waveFormat_;
    bool isPlaying_;
    bool isLeaveWord_;
    volatile bool isEnding_;
	int dataCount_;
    DWORD dataLength_;
    OggCodec* oggCodec_;
};
