#include "WavePlayer.h"

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

namespace Util {
    namespace Audio {
        WavePlayer* const WavePlayer::Instance() {
            static WavePlayer* result = 0;
            if (!result) {
                result = new WavePlayer();
                //result->Create(WavePlayer::IDD);
            }
            return result;
        }

        // WaveRecord dialog

        //IMPLEMENT_DYNAMIC(WavePlayer, CDialog)

        WavePlayer::WavePlayer(/*CWnd* pParent *//*=NULL*/)
        //: CDialog(WavePlayer::IDD, pParent)
        : dataCount_(0)
        , buffer_(0)
        , notify_(0) {
            //oggCodec_ = GetOggCodec();
        }

        //void WavePlayer::DoDataExchange(CDataExchange* pDX) {
        //    CDialog::DoDataExchange(pDX);
        //}

        //BEGIN_MESSAGE_MAP(WavePlayer, CDialog)
        //    ON_MESSAGE(MM_WOM_OPEN, OnMM_WOM_OPEN)
        //    ON_MESSAGE(MM_WOM_DONE, OnMM_WOM_DONE)
        //    ON_MESSAGE(MM_WOM_CLOSE, OnMM_WOM_CLOSE)
        //END_MESSAGE_MAP()


        // WaveRecord message handlers

        //LRESULT WavePlayer::OnMM_WOM_OPEN(WPARAM wParam, LPARAM lParam) {
        //    LRESULT result = 0;
        //    waveOutPrepareHeader(waveOut_, waveHeader_, sizeof(WAVEHDR));
        //    waveOutWrite(waveOut_, waveHeader_, sizeof(WAVEHDR));
        //    isPlaying_ = true;
        //    return result;
        //}

        //LRESULT WavePlayer::OnMM_WOM_DONE(WPARAM wParam, LPARAM lParam) {
        //    LRESULT result = 0;
        //    WAVEHDR* waveHeader = reinterpret_cast<WAVEHDR*>(lParam);
        //    waveOutUnprepareHeader(waveOut_, waveHeader, sizeof(WAVEHDR));
        //    free(waveHeader->lpData);
        //    free(waveHeader);
        //    if (notify_) {
        //        notify_();
        //    }
        //    return result;
        //}

        //LRESULT WavePlayer::OnMM_WOM_CLOSE(WPARAM wParam, LPARAM lParam) {
        //    LRESULT result = 0;
        //    isPlaying_ = false;
        //    return result;
        //}

        namespace {
            void CALLBACK waveCallback(HWAVE hWave, UINT msg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2) {
                switch (msg) {
                    case WOM_OPEN:
                        break;
                    case WOM_DONE:
                        //WAVEHDR* waveHeader = reinterpret_cast<WAVEHDR*>(dwParam1/*lParam*/);
                        //waveOutUnprepareHeader(waveOut_, waveHeader, sizeof(WAVEHDR));
                        //free(waveHeader->lpData);
                        //free(waveHeader);

                        //std::wstring decodeData = speexCodec_->Decode(data, length);
                        
                        //if (notify_) {
                        //    notify_();
                        //}
                        break;
                    case WOM_CLOSE:
                        break;
                    default:
                        break;
                }
            }
        }

        void WavePlayer::InsertData(void const* const data, size_t const length) {
            waveHeader_->lpData = reinterpret_cast<LPSTR>(const_cast<void*>(data));
            waveHeader_->dwBufferLength = static_cast<DWORD>(length);
            waveHeader_->dwBytesRecorded = 0;
            waveHeader_->dwUser = 0;
            waveHeader_->dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
            waveHeader_->dwLoops = 0; //repetitions;
            waveHeader_->lpNext = NULL;
            waveHeader_->reserved = 0;

            waveOutPrepareHeader(waveOut_, waveHeader_, sizeof(WAVEHDR));
            waveOutWrite(waveOut_, waveHeader_, sizeof(WAVEHDR));
            isPlaying_ = true;
        }

        bool const WavePlayer::Start(std::wstring const& filename) {
            bool result = true;
            if (isPlaying_) {
                waveOutReset(waveOut_);
            }
            //buffer_ = reinterpret_cast<unsigned char*>(malloc(INP_BUFFER_SIZE));
            waveHeader_ = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
            waveFormat_.wFormatTag = WAVE_FORMAT_PCM;
            waveFormat_.nChannels = CHANNELS;
            waveFormat_.nSamplesPerSec = SAMPLESPERSEC;
            waveFormat_.wBitsPerSample = BITSPERSAMPLE;
            waveFormat_.nBlockAlign = waveFormat_.nChannels * waveFormat_.wBitsPerSample / 8;
            waveFormat_.nAvgBytesPerSec = waveFormat_.nBlockAlign * waveFormat_.nSamplesPerSec;
            waveFormat_.cbSize = 0;	
            //prepare record component
            waveFormat_.wFormatTag = WAVE_FORMAT_PCM;
            waveFormat_.nChannels = CHANNELS;
            waveFormat_.nSamplesPerSec = SAMPLESPERSEC;
            waveFormat_.wBitsPerSample = BITSPERSAMPLE;
            waveFormat_.nBlockAlign = waveFormat_.nChannels * waveFormat_.wBitsPerSample / 8;
            waveFormat_.nAvgBytesPerSec = waveFormat_.nBlockAlign * waveFormat_.nSamplesPerSec;
            waveFormat_.cbSize = 0;

            if (waveOutOpen(&waveOut_, WAVE_MAPPER, &waveFormat_, (DWORD)waveCallback, 0, CALLBACK_FUNCTION)) {
            //if (waveOutOpen(&waveOut_, WAVE_MAPPER, &waveFormat_, (DWORD)m_hWnd, NULL, CALLBACK_WINDOW)) {
                result = false;
            }

            waveOutMessage(0, WAV_PSTN_OUT, 0, 0);

            if (filename != L"") {
                //CFile file;
                //file.Open(filename.c_str(), CFile::modeRead);
                //ULONGLONG fileLength = file.GetLength();
                //buffer_ = (unsigned char*)malloc(static_cast<size_t>(fileLength));
                //file.Read(buffer_, static_cast<UINT>(fileLength));
                //file.Close();
            }
            //waveHeader_->lpData = reinterpret_cast<LPSTR>(buffer_);
            //waveHeader_->dwBufferLength = static_cast<DWORD>(fileLength);
            //waveHeader_->dwBytesRecorded = 0;
            //waveHeader_->dwUser = 0;
            //waveHeader_->dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
            //waveHeader_->dwLoops = 0; //repetitions;
            //waveHeader_->lpNext = NULL;
            //waveHeader_->reserved = 0;

            //waveOutPrepareHeader(waveOut_, waveHeader_, sizeof(WAVEHDR));
            //waveOutWrite(waveOut_, waveHeader_, sizeof(WAVEHDR));
            //isPlaying_ = true;

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
    }
}
