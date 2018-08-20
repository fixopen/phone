#include "../Text/StringOp.h"
#include "../../Telephone/Logical/BusyToneDetectThread.h"

#include "WaveRecorder.h"

#define RIFF_FILE       mmioFOURCC('R', 'I', 'F', 'F')
#define RIFF_WAVE       mmioFOURCC('W', 'A', 'V', 'E')
#define RIFF_FORMAT     mmioFOURCC('f', 'm', 't', ' ')
#define RIFF_DATA       mmioFOURCC('d', 'a', 't', 'a')

#define SECONDBYTES8           2048
#define SECONDBYTES2           850
#define CHANNELS               1
#define SAMPLESPERSEC          8000
#define BITSPERSAMPLE          16
#define INP_BUFFER_SIZE        16000

namespace Util {
    namespace Audio {
        WaveRecorder* const WaveRecorder::Instance() {
            static WaveRecorder* result = 0;
            if (!result) {
                result = new WaveRecorder();
            }
            return result;
        }

        WaveRecorder::WaveRecorder()
        : dataCount_(0)
        , isBusyToneDetect_(false)
        , dataProcessor_(0)
        , speexEncoder_(0) {
        }

        namespace {
            void CALLBACK waveCallback(HWAVE hWave, UINT msg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2) {
                PWAVEHDR p = reinterpret_cast<PWAVEHDR>(dwParam1/*lParam*/);
                switch (msg) {
                    case WIM_OPEN:
                        break;
                    case WIM_DATA:
                        WaveRecorder::Instance()->Encoder()->Encode(reinterpret_cast<spx_int16_t*>(p->lpData),  p->dwBytesRecorded / sizeof(short));
                        break;
                    case WIM_CLOSE:
                        break;
                    default:
                        break;
                }
            }
        }

        std::string const WaveRecorder::SpitData() {
            std::string result;
            return result;
        }

        bool const WaveRecorder::Start(std::wstring const& filename, std::wstring const& comment, bool const isBusyToneDetect) {
            bool result = false;
            isBusyToneDetect_ = isBusyToneDetect;
            //start record
            LRESULT errorCode = MMSYSERR_NOERROR;
            errorCode = waveInOpen(&waveIn_, WAVE_MAPPER, &waveFormat_, reinterpret_cast<DWORD>(waveCallback), reinterpret_cast<DWORD>(this), CALLBACK_FUNCTION);
            //errorCode = waveInOpen(&waveIn_, WAVE_MAPPER, &waveFormat_, static_cast<DWORD>(m_hWnd), reinterpret_cast<DWORD>(this), CALLBACK_WINDOW);
            if (errorCode == MMSYSERR_NOERROR) {
                if ((errorCode = waveInMessage(waveIn_, WAV_PSTN_IN, 0, 0)) != MMSYSERR_NOERROR) {
                    CString error;
                    error.Format(L"waveInMessage Error code is %d", errorCode);
                    MessageBox(0, error, L"WaveInError", MB_OK);
                }
                //prepare record component
                waveFormat_.wFormatTag = WAVE_FORMAT_PCM;
                waveFormat_.nChannels = CHANNELS;
                waveFormat_.nSamplesPerSec = SAMPLESPERSEC;
                waveFormat_.wBitsPerSample = BITSPERSAMPLE;
                waveFormat_.nBlockAlign = waveFormat_.nChannels * waveFormat_.wBitsPerSample / 8;
                waveFormat_.nAvgBytesPerSec = waveFormat_.nBlockAlign * waveFormat_.nSamplesPerSec;
                waveFormat_.cbSize = 0;
                for (int i = 0; i < WAVE_BUFFER_COUNT; ++i) {
                    buffers_[i] = reinterpret_cast<unsigned char*>(malloc(INP_BUFFER_SIZE));
                    waveHeaders_[i] = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
                    waveHeaders_[i]->lpData = reinterpret_cast<char*>(buffers_[i]);
                    waveHeaders_[i]->dwBufferLength = INP_BUFFER_SIZE;
                    waveHeaders_[i]->dwBytesRecorded = 0;
                    waveHeaders_[i]->dwUser = 0;
                    waveHeaders_[i]->dwFlags = 0;
                    waveHeaders_[i]->dwLoops = 1;
                    waveHeaders_[i]->lpNext = NULL;
                    waveHeaders_[i]->reserved = 0;
                    if ((errorCode = waveInPrepareHeader(waveIn_, waveHeaders_[i], sizeof(WAVEHDR))) != MMSYSERR_NOERROR) {
                        CString error;
                        error.Format(L"waveInPrepareHeader Error code is %d", errorCode);
                        MessageBox(0, error, L"WaveInError", MB_OK);
                    }
                    if ((errorCode = waveInAddBuffer(waveIn_, waveHeaders_[i], sizeof(WAVEHDR))) != MMSYSERR_NOERROR) {
                        CString error;
                        error.Format(L"waveInAddBuffer Error code is %d", errorCode);
                        MessageBox(0, error, L"WaveInError", MB_OK);
                    }
                }
                dataCount_ = WAVE_BUFFER_COUNT;
                isEnding_ = false;
                isRecording_ = true;
                dataLength_ = 0;
                waveInStart(waveIn_);
                result = true;
                //oggCodec_->StartEncode(Util::Text::StringOp::FromUnicode(filename), Util::Text::StringOp::FromUnicode(comment));
            } else {
                CString error;
                error.Format(L"waveInOpen Error code is %d", errorCode);
                MessageBox(0, error, L"WaveInError", MB_OK);
            }
            return result;
        }

        bool const WaveRecorder::Stop() {
            //stop record
            bool result = true;
            isRecording_ = false;
            LRESULT errorCode = MMSYSERR_NOERROR;
            //if ((errorCode = waveInStop(waveIn_)) != MMSYSERR_NOERROR) {
            //	CString error;
            //	error.Format(L"waveInStop Error code is %d", errorCode);
            //	MessageBox(0, error, L"WaveInError", MB_OK);
            //}
            if ((errorCode = waveInReset(waveIn_)) != MMSYSERR_NOERROR) {
                CString error;
                error.Format(L"waveInReset Error code is %d", errorCode);
                MessageBox(0, error, L"WaveInError", MB_OK);
            }
            isEnding_ = true;
            return result;
        }
    }
}
