#pragma once
#define  WAVE_BUFFER_COUNT 2

#include <mmsystem.h>
#include <string>
#include "Util/SmartPtr.h"
#include "Util/OggCodec.h"
#include "SoundSegment.h"
#include "ContactInfo.h"

enum WAVEDEV_MSG {
    WAV_PLAYBACK_FLAG       = 0x10000000,
    WAV_SPEAKER_ONLY		= 0x10000001,
    WAV_LINEOUT_ONLY		= 0x10000002,
    WAV_HANDLE_ONLY			= 0x10000004,

    WAV_SNDEFFECT_POP		= 0x20000000,
    WAV_SNDEFFECT_3D		= 0x20000001,
    WAV_SNDEFFECT_CLASSIC	= 0x20000002,
    WAV_SNDEFFECT_JAZZ		= 0x20000003,

    WAV_RECORD_FLAG			= 0x30000000,
    WAV_LINEIN_MIC			= 0x30000001,
    WAV_LOCAL_MIC			= 0x30000002,
    WAV_LOCAL_HANDLE		= 0x30000004,

    WAV_SND_SOFTMUTE_ON		= 0x40000003,
    WAV_SND_SOFTMUTE_OFF	= 0x40000004,

    WAV_MIC_GAIN   			= 0x40000001,
};

OggCodec* const GetOggCodec();

class WaveRecorder {
    friend void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
public:
    static WaveRecorder* const Instance();
    bool const Start(Util::shared_ptr<SoundSegment>& soundSegment);
    bool const Stop();
    void WriteWaveHeader();
    std::wstring const GetFilename(Util::Timestamp const& time, bool isHighQuality);
    //void Record();
    bool const IsRecording() const {
        return isRecording_;
    }
private:
    WaveRecorder();
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
    std::wstring recordFilename_;
    CFile waveFile_;
	//HWAVEOUT waveOut_;
	//PBYTE outBuffer_;
    //std::wstring path_;
    //Util::shared_ptr<SoundSegment> soundSegment_;
    //std::vector<Util::shared_ptr<SoundSegment> >soundSegments_;
    //Util::shared_ptr<ContactInfo> contactInfo_;
    //UINT recordSecond_;
    //UINT recordTimer_;
    //UINT remainRecordSecond_;
};
