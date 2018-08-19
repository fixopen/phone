#pragma once

#define  WAVE_BUFFER_COUNT 2

#include <mmsystem.h>
#include <string>
#include "Util/SmartPtr.h"
#include "Util/OggCodec.h"
#include "SoundSegment.h"
#include "ContactInfo.h"
#include "Resource.h"

// WaveRecord dialog

class WaveRecorder : public CDialog {
	DECLARE_DYNAMIC(WaveRecorder)

public:
	static WaveRecorder* const WaveRecorder::Instance();
	WaveRecorder(CWnd* pParent = NULL);   // standard constructor
	virtual ~WaveRecorder();

// Dialog Data
	enum { IDD = IDD_DIALOG_WAVE_RECORD };

    bool const Start(Util::shared_ptr<SoundSegment>& soundSegment);
    bool const Stop();
    std::wstring const GetFilename(Util::Timestamp const& time, bool const isHighQuality);
    //void Record();
    bool const IsRecording() const {
        return isRecording_;
    }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg LRESULT OnMM_WIM_OPEN(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMM_WIM_DATA(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMM_WIM_CLOSE(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
private:
	HWAVEIN waveIn_;
	unsigned char* buffers_[WAVE_BUFFER_COUNT];
	PWAVEHDR waveHeaders_[WAVE_BUFFER_COUNT];
	WAVEFORMATEX waveFormat_;
    bool isRecording_;
    bool isLeaveWord_;
    volatile bool isEnding_;
	int dataCount_;
    DWORD dataLength_;
    OggCodec* oggCodec_;
};
