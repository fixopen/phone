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
                //result->Create(WaveRecorder::IDD);
            }
            return result;
        }

        // WaveRecord dialog

        //IMPLEMENT_DYNAMIC(WaveRecorder, CDialog)

        WaveRecorder::WaveRecorder(/*CWnd* pParent *//*=NULL*/)
        //: CDialog(WaveRecorder::IDD, pParent)
        : dataCount_(0)
        , isBusyToneDetect_(false)
        , dataProcessor_(0)
        , speexCodec_(0) {
            //oggCodec_ = GetOggCodec();
        }

        //void WaveRecorder::DoDataExchange(CDataExchange* pDX) {
        //    CDialog::DoDataExchange(pDX);
        //}

        //BEGIN_MESSAGE_MAP(WaveRecorder, CDialog)
        //    ON_MESSAGE(MM_WIM_OPEN,OnMM_WIM_OPEN)
        //    ON_MESSAGE(MM_WIM_DATA,OnMM_WIM_DATA)
        //    ON_MESSAGE(MM_WIM_CLOSE,OnMM_WIM_CLOSE)
        //END_MESSAGE_MAP()

        //// WaveRecord message handlers

        //LRESULT WaveRecorder::OnMM_WIM_OPEN(WPARAM wParam, LPARAM lParam) {
        //    LRESULT result = 0;
        //    return result;
        //}

        //LRESULT WaveRecorder::OnMM_WIM_DATA(WPARAM wParam, LPARAM lParam) {
        //    LRESULT result = 0;
        //    PWAVEHDR p = reinterpret_cast<PWAVEHDR>(lParam);
        //    LRESULT errorCode = MMSYSERR_NOERROR;

        //    OggCodec::PcmData* data = new OggCodec::PcmData();
        //    data->dataLength = p->dwBytesRecorded / sizeof(short);
        //    data->data = (short*)malloc(p->dwBytesRecorded + 2);
        //    if (data->data != NULL) {
        //        memcpy(data->data, p->lpData, p->dwBytesRecorded);
        //        oggCodec_->PushPcmData(data);
        //        if (isBusyToneDetect_) {
        //            Telephone::Logical::BusyToneDetectThread::Instance()->AppendVoiceData(data);
        //        }
        //    } else {
        //        delete data;
        //    }
        //    --dataCount_;
        //    if (isEnding_) {
        //        if (dataCount_ <= 0) {
        //            Telephone::Logical::BusyToneDetectThread::Instance()->VoiceStop();
        //            oggCodec_->StopEncode(true);
        //            isBusyToneDetect_ = false;
        //            isRecording_ = FALSE;
        //            if ((errorCode = waveInMessage(0, WAV_LOCAL_MIC, 0, 0)) != MMSYSERR_NOERROR) {
        //                CString error;
        //                error.Format(L"waveInMessage Error code is %d", errorCode);
        //                MessageBox(error, L"WaveInError", MB_OK);
        //            }
        //        }
        //    } else { // Send out a new buffer
        //        if ((errorCode = waveInUnprepareHeader(waveIn_, p, sizeof(WAVEHDR))) != MMSYSERR_NOERROR) {
        //            CString error;
        //            error.Format(L"waveInUnprepareHeader Error code is %d", errorCode);
        //            MessageBox(error, L"WaveInError", MB_OK);
        //        }
        //        //p->lpData = (char*)buffers_[i];
        //        p->dwBufferLength = INP_BUFFER_SIZE;
        //        p->dwFlags = 0;
        //        if ((errorCode = waveInPrepareHeader(waveIn_, p, sizeof(WAVEHDR))) != MMSYSERR_NOERROR) {
        //            CString error;
        //            error.Format(L"waveInPrepareHeader Error code is %d", errorCode);
        //            MessageBox(error, L"WaveInError", MB_OK);
        //        }
        //        if ((errorCode = waveInAddBuffer(waveIn_, p, sizeof(WAVEHDR))) != MMSYSERR_NOERROR) {
        //            CString error;
        //            error.Format(L"waveInAddBuffer Error code is %d", errorCode);
        //            MessageBox(error, L"WaveInError", MB_OK);
        //        }
        //        ++dataCount_;
        //    }
        //    return result;
        //}

        //LRESULT WaveRecorder::OnMM_WIM_CLOSE(WPARAM wParam, LPARAM lParam) {
        //    LRESULT result = 0;
        //    return result;
        //}

        namespace {
            void CALLBACK waveCallback(HWAVE hWave, UINT msg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2) {
                PWAVEHDR p = reinterpret_cast<PWAVEHDR>(dwParam1/*lParam*/);
                switch (msg) {
                    case WIM_OPEN:
                        break;
                    case WIM_DATA:
                        WaveRecorder::Instance()->speexCodec_->Encode(p->lpData,  p->dwBytesRecorded / sizeof(short));
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
                if ((errorCode = waveInMessage(waveIn_, WAV_LINEIN_MIC, 0, 0)) != MMSYSERR_NOERROR) {
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

        //bool const WaveRecorder::Start(Util::shared_ptr<SoundSegment>& soundSegment) {
        //    bool result = false;
        //    isLeaveWord_ = soundSegment->type() == sstLeaveWord;
        //    //start record
        //    LRESULT errorCode = MMSYSERR_NOERROR;
        //    errorCode = waveInOpen(&waveIn_, WAVE_MAPPER, &waveFormat_, (DWORD)m_hWnd, reinterpret_cast<DWORD>(this), /*CALLBACK_FUNCTION*/CALLBACK_WINDOW);
        //    if (errorCode == MMSYSERR_NOERROR) {
        //        if ((errorCode = waveInMessage(waveIn_, WAV_LINEIN_MIC, 0, 0)) != MMSYSERR_NOERROR) {
        //            CString error;
        //            error.Format(L"waveInMessage Error code is %d", errorCode);
        //            MessageBox(error, L"WaveInError", MB_OK);
        //        }
        //        for (int i = 0; i < WAVE_BUFFER_COUNT; ++i) {
        //            waveHeaders_[i]->lpData = (char*)buffers_[i];
        //            waveHeaders_[i]->dwBufferLength = INP_BUFFER_SIZE;
        //            waveHeaders_[i]->dwBytesRecorded = 0;
        //            waveHeaders_[i]->dwUser = 0;
        //            waveHeaders_[i]->dwFlags = 0;
        //            waveHeaders_[i]->dwLoops = 1;
        //            waveHeaders_[i]->lpNext = NULL;
        //            waveHeaders_[i]->reserved = 0;
        //            if ((errorCode = waveInPrepareHeader(waveIn_, waveHeaders_[i], sizeof(WAVEHDR))) != MMSYSERR_NOERROR) {
        //                CString error;
        //                error.Format(L"waveInPrepareHeader Error code is %d", errorCode);
        //                MessageBox(error, L"WaveInError", MB_OK);
        //            }
        //            if ((errorCode = waveInAddBuffer(waveIn_, waveHeaders_[i], sizeof(WAVEHDR))) != MMSYSERR_NOERROR) {
        //                CString error;
        //                error.Format(L"waveInAddBuffer Error code is %d", errorCode);
        //                MessageBox(error, L"WaveInError", MB_OK);
        //            }
        //        }
        //        dataCount_ = WAVE_BUFFER_COUNT;
        //        isEnding_ = false;
        //        isRecording_ = true;
        //        dataLength_ = 0;
        //        waveInStart(waveIn_);
        //        result = true;
        //        std::wstring comment = soundSegment->name() + L"\'" + soundSegment->telephoneNumber() + L"\'" + Util::StringOp::FromInt(soundSegment->type()) + L"\'" + Util::StringOp::FromTimestamp(soundSegment->startTime()) + L"\'";
        //        oggCodec_->StartEncode(Util::StringOp::FromUnicode(soundSegment->filename()), Util::StringOp::FromUnicode(comment));
        //    } else {
        //        CString error;
        //        error.Format(L"waveInOpen Error code is %d", errorCode);
        //        MessageBox(error, L"WaveInError", MB_OK);
        //    }
        //    return result;
        //}

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

        std::wstring const WaveRecorder::GetFilename(Util::Timestamp const& time, bool const isHighQuality) {
            CString filename;
            if (isHighQuality) {
                //oggCodec_->SetQuality(8);
                filename.Format(_T("%02d%02d%02dHQ.spx"), time.GetHour(), time.GetMinute(), time.GetSecond());
            } else {
                //oggCodec_->SetQuality(2);
                filename.Format(_T("%02d%02d%02dST.spx"), time.GetHour(), time.GetMinute(), time.GetSecond());
            }
            return static_cast<LPCTSTR>(filename);
        }
    }
}
