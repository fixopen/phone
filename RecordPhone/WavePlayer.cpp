#include "StdAfx.h"
#include "WavePlayer.h"
#include "RecordPhone.h"
#include "CallDialog.h"
#include "Telephone.h"
#include "BusyToneDetectThread.h"
#include "Util/Thread.h"

#define RIFF_FILE       mmioFOURCC('R','I','F','F')
#define RIFF_WAVE       mmioFOURCC('W','A','V','E')
#define RIFF_FORMAT     mmioFOURCC('f','m','t',' ')
#define RIFF_DATA       mmioFOURCC('d','a','t','a')

#define SECONDBYTES8           2048
#define SECONDBYTES2           850
#define CHANNELS               1
#define SAMPLESPERSEC          8000
#define BITSPERSAMPLE          16
#define INP_BUFFER_SIZE        16000

WavePlayer* const WavePlayer::Instance() {
    static WavePlayer* result = 0;
	if (!result) {
		result = new WavePlayer();
		result->Create(WavePlayer::IDD);
	}
    return result;
}

// WaveRecord dialog

IMPLEMENT_DYNAMIC(WavePlayer, CDialog)

WavePlayer::WavePlayer(CWnd* pParent /*=NULL*/)
: CDialog(WavePlayer::IDD, pParent)
, dataCount_(0)
, buffer_(0)
, notify_(0) {
    oggCodec_ = GetOggCodec();
    //prepare record component
    waveFormat_.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat_.nChannels = CHANNELS;
    waveFormat_.nSamplesPerSec = SAMPLESPERSEC;
    waveFormat_.wBitsPerSample = BITSPERSAMPLE;
    waveFormat_.nBlockAlign = waveFormat_.nChannels * waveFormat_.wBitsPerSample / 8;
    waveFormat_.nAvgBytesPerSec = waveFormat_.nBlockAlign * waveFormat_.nSamplesPerSec;
    waveFormat_.cbSize = 0;
    //buffer_ = reinterpret_cast<unsigned char*>(malloc(INP_BUFFER_SIZE));
    waveHeader_ = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
}

WavePlayer::~WavePlayer() {
}

void WavePlayer::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(WavePlayer, CDialog)
	ON_MESSAGE(MM_WOM_OPEN, OnMM_WOM_OPEN)
	ON_MESSAGE(MM_WOM_DONE, OnMM_WOM_DONE)
	ON_MESSAGE(MM_WOM_CLOSE, OnMM_WOM_CLOSE)
END_MESSAGE_MAP()


// WaveRecord message handlers

LRESULT WavePlayer::OnMM_WOM_OPEN(WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
    waveOutPrepareHeader(waveOut_, waveHeader_, sizeof(WAVEHDR));
    waveOutWrite(waveOut_, waveHeader_, sizeof(WAVEHDR));
    isPlaying_ = true;
	return result;
}

LRESULT WavePlayer::OnMM_WOM_DONE(WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
    WAVEHDR* waveHeader = reinterpret_cast<WAVEHDR*>(lParam);
	waveOutUnprepareHeader(waveOut_, waveHeader, sizeof(WAVEHDR));
	free(waveHeader->lpData);
	free(waveHeader);
    if (notify_) {
        notify_();
    }
	return result;
}

LRESULT WavePlayer::OnMM_WOM_CLOSE(WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
    isPlaying_ = false;
	return result;
}

bool const WavePlayer::Start(std::wstring const& filename) {
    bool result = true;
	if (isPlaying_) {
		waveOutReset(waveOut_);
	}
	waveFormat_.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat_.nChannels = CHANNELS;
	waveFormat_.nSamplesPerSec = SAMPLESPERSEC;
	waveFormat_.wBitsPerSample = BITSPERSAMPLE;
	waveFormat_.nBlockAlign = waveFormat_.nChannels * waveFormat_.wBitsPerSample / 8;
	waveFormat_.nAvgBytesPerSec = waveFormat_.nBlockAlign * waveFormat_.nSamplesPerSec;
	waveFormat_.cbSize = 0;	
	
	if (waveOutOpen(&waveOut_, WAVE_MAPPER, &waveFormat_, (DWORD)m_hWnd, NULL, CALLBACK_WINDOW)) {
		result = false;
	}

 	//waveOutMessage(0, WAV_LINEOUT_ONLY, 0, 0);

    CFile file;
    file.Open(filename.c_str(), CFile::modeRead);
    ULONGLONG fileLength = file.GetLength();
    buffer_ = (unsigned char*)malloc(static_cast<size_t>(fileLength));
    file.Read(buffer_, static_cast<UINT>(fileLength));
    file.Close();
    waveHeader_->lpData = reinterpret_cast<LPSTR>(buffer_);
    waveHeader_->dwBufferLength = static_cast<DWORD>(fileLength);
    waveHeader_->dwBytesRecorded = 0;
    waveHeader_->dwUser = 0;
    waveHeader_->dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
    waveHeader_->dwLoops = 0; //repetitions;
    waveHeader_->lpNext = NULL;
    waveHeader_->reserved = 0;

    return result;
}

bool const WavePlayer::Pause() {
    bool result = true;
    waveOutPause(waveOut_);
    return result;
}

bool const WavePlayer::Resume() {
    bool result = true;
    waveOutRestart(waveOut_);
    return result;
}

bool const WavePlayer::Stop() {
	if (!isPlaying_) {
		return true;
	}
	waveOutReset(waveOut_);
	waveOutClose(waveOut_);
	return true;
}
