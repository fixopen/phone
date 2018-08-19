#include "StdAfx.h"
#include "WaveRecorder.h"
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

OggCodec* const GetOggCodec() {
    static OggCodec* result = new OggCodec();
    return result;
}

WaveRecorder* const WaveRecorder::Instance() {
    static WaveRecorder* result = new WaveRecorder();
    return result;
}

WaveRecorder::WaveRecorder()
: dataCount_(0) {
    oggCodec_ = GetOggCodec();
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
    }
}

void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
    WaveRecorder* recorder = reinterpret_cast<WaveRecorder*>(dwInstance);
    PWAVEHDR p = reinterpret_cast<PWAVEHDR>(dwParam1);
	LRESULT errorCode = MMSYSERR_NOERROR;
    switch (uMsg) {
        case WIM_CLOSE:
            for (int i = 0; i < WAVE_BUFFER_COUNT; ++i) {
				if ((errorCode = waveInUnprepareHeader(recorder->waveIn_, recorder->waveHeaders_[i], sizeof(WAVEHDR))) != MMSYSERR_NOERROR) {
					CString error;
					error.Format(L"waveInUnprepareHeader Error code is %d", errorCode);
					MessageBox(0, error, L"WaveInError", MB_OK);
				}
            }
            BusyToneDetectThread::Instance()->VoiceStop();
            recorder->oggCodec_->StopEncode(true);
            recorder->isRecording_ = FALSE;
			if ((errorCode = waveInMessage(0, WAV_LOCAL_MIC, 0, 0)) != MMSYSERR_NOERROR) {
				CString error;
				error.Format(L"waveInMessage Error code is %d", errorCode);
				MessageBox(0, error, L"WaveInError", MB_OK);
			}
            break;
        case WIM_DATA:
            if (true) {
                OggCodec::PcmData* data = new OggCodec::PcmData();
                data->dataLength = p->dwBytesRecorded / sizeof(short);
                data->data = (short*)malloc(p->dwBytesRecorded + 2);
                if (data->data != NULL) {
                    memcpy(data->data, p->lpData, p->dwBytesRecorded);
                    recorder->oggCodec_->PushPcmData(data);
                    if (recorder->isLeaveWord_) {
                        BusyToneDetectThread::Instance()->AppendVoiceData(data);
                    }
                } else {
                    delete data;
                }
            }
			//--recorder->dataCount_;
            if (recorder->isEnding_) {
				if (waveInUnprepareHeader(recorder->waveIn_, p, sizeof(WAVEHDR)) != MMSYSERR_NOERROR) {
					CString error;
					error.Format(L"waveInUnprepareHeader Error code is %d", errorCode);
					MessageBox(0, error, L"WaveInError", MB_OK);
				}
				if (true/*recorder->dataCount_ <= 0*/) {
					if ((errorCode = waveInClose(recorder->waveIn_)) != MMSYSERR_NOERROR) {
						CString error;
						error.Format(L"waveInClose Error code is %d", errorCode);
						MessageBox(0, error, L"WaveInError", MB_OK);
						//WAVERR_STILLPLAYING;
					}
				}
			} else { // Send out a new buffer
				if ((errorCode = waveInUnprepareHeader(recorder->waveIn_, p, sizeof(WAVEHDR))) != MMSYSERR_NOERROR) {
					CString error;
					error.Format(L"waveInUnprepareHeader Error code is %d", errorCode);
					MessageBox(0, error, L"WaveInError", MB_OK);
				}
				//p->lpData = (char*)buffers_[i];
				p->dwBufferLength = INP_BUFFER_SIZE;
				p->dwFlags = 0;
				if ((errorCode = waveInPrepareHeader(recorder->waveIn_, p, sizeof(WAVEHDR))) != MMSYSERR_NOERROR) {
					CString error;
					error.Format(L"waveInPrepareHeader Error code is %d", errorCode);
					MessageBox(0, error, L"WaveInError", MB_OK);
				}
				if ((errorCode = waveInAddBuffer(recorder->waveIn_, p, sizeof(WAVEHDR))) != MMSYSERR_NOERROR) {
					CString error;
					error.Format(L"waveInAddBuffer Error code is %d", errorCode);
					MessageBox(0, error, L"WaveInError", MB_OK);
				}
				//++recorder->dataCount_;
			}
            break;
        case WIM_OPEN:
            break;
        default:
            break;
    }
}

bool const WaveRecorder::Start(Util::shared_ptr<SoundSegment>& soundSegment) {
    bool result = false;
    isLeaveWord_ = soundSegment->type() == sstLeaveWord;
    //start record
	LRESULT errorCode = MMSYSERR_NOERROR;
    errorCode = waveInOpen(&waveIn_, WAVE_MAPPER, &waveFormat_, (DWORD)waveInProc, reinterpret_cast<DWORD>(this), CALLBACK_FUNCTION/*CALLBACK_WINDOW*/);
    if (errorCode == MMSYSERR_NOERROR) {
        if ((errorCode = waveInMessage(waveIn_, WAV_LINEIN_MIC, 0, 0)) != MMSYSERR_NOERROR) {
			CString error;
			error.Format(L"waveInMessage Error code is %d", errorCode);
			MessageBox(0, error, L"WaveInError", MB_OK);
		}
        for (int i = 0; i < WAVE_BUFFER_COUNT; ++i) {
            waveHeaders_[i]->lpData = (char*)buffers_[i];
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
		//dataCount_ = WAVE_BUFFER_COUNT;
        isEnding_ = false;
        isRecording_ = true;
        dataLength_ = 0;
        waveInStart(waveIn_);
        result = true;
        std::wstring comment = soundSegment->name() + L"\'" + soundSegment->telephoneNumber() + L"\'" + Util::StringOp::FromInt(soundSegment->type()) + L"\'" + Util::StringOp::FromTimestamp(soundSegment->startTime()) + L"\'";
        oggCodec_->StartEncode(Util::StringOp::FromUnicode(soundSegment->filename()), Util::StringOp::FromUnicode(comment));
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

void WaveRecorder::WriteWaveHeader() {
    if (recordFilename_ == L"") {
        return;
    }
#pragma pack(2)
    struct wave_header {
        char riff[4];             /* "RIFF" */
        unsigned long size;       /* length of rest of file = size of rest of header(36) + data length */
        char wave[4];             /* "WAVE" */
        char fmt[4];              /* "fmt " */
        unsigned long fmt_len;    /* length of rest of fmt chunk = 16 */
        unsigned short tag;       /* MS PCM = 1 */
        unsigned short channels;  /* channels, mono = 1, stereo = 2 */
        unsigned long samp_rate;  /* samples per second = 44100 */
        unsigned long byte_rate;  /* bytes per second = samp_rate * byte_samp = 176400 */
        unsigned short byte_samp; /* block align (bytes per sample) = channels * bits_per_sample / 8 = 4 */
        unsigned short bit_samp;  /* bits per sample = 16 for MS PCM (format specific) */
        unsigned short res0;
        char data[4];             /* "data" */
        unsigned long length;     /* data length (bytes) */
    };
	wave_header header;

    DWORD temp = RIFF_FILE;
    memcpy(&header.riff, &temp, 4);
    header.size = 36 + dataLength_;
    temp = RIFF_WAVE;
    memcpy(&header.wave, &temp, 4);
    temp = RIFF_FORMAT;
    memcpy(&header.fmt, &temp, 4);
    header.fmt_len = sizeof(WAVEFORMATEX);
    memcpy(&header.tag, &waveFormat_, sizeof(WAVEFORMATEX));
    temp = RIFF_DATA;
    memcpy(&header.data, &temp, 4);
    header.length = dataLength_;
    CFileStatus status;
    if (CFile::GetStatus(recordFilename_.c_str(), status)) {
        waveFile_.Open(recordFilename_.c_str(), CFile::modeWrite);
    } else {
        waveFile_.Open(recordFilename_.c_str(), CFile::modeCreate | CFile::modeWrite);
    }
    waveFile_.SeekToBegin();
    waveFile_.Write(&header, sizeof(wave_header));
    waveFile_.Close();
}

std::wstring const WaveRecorder::GetFilename(Util::Timestamp const& time, bool const isHighQuality) {
    CString filename;
    if (isHighQuality) {
        oggCodec_->SetQuality(8);
        filename.Format(_T("%02d%02d%02dHQ.spx"), time.GetHour(), time.GetMinute(), time.GetSecond());
    } else {
        oggCodec_->SetQuality(2);
        filename.Format(_T("%02d%02d%02dST.spx"), time.GetHour(), time.GetMinute(), time.GetSecond());
    }
    return static_cast<LPCTSTR>(filename);
}
#if 0
void WaveRecorder::Record() {
    if (!isRecording_) {
        path_ = L""; //setting->system->soundPath();

        extern BOOL DetectDIR(LPCTSTR filepath);
        if (path_ == ssStorageCardRecordPath) {
            BOOL sd = DetectDIR(static_cast<LPCTSTR>(csStarageCard));
            if (sd) {
                if (!DetectDIR(static_cast<LPCTSTR>(csStorageCardRecordPath))) {
                    CreateDirectory((LPTSTR)(LPCTSTR)csStorageCardRecordPath, NULL);
                }
            } else {
                path_ = ssFlashRecordPath;
                if (!DetectDIR(static_cast<LPCTSTR>(csFlashRecordPath))) {
                    CreateDirectory(static_cast<LPCTSTR>(csFlashRecordPath), NULL);
                }
            }
        }

        ULARGE_INTEGER freeBytes;
        ULARGE_INTEGER totalBytes;
        GetDiskFreeSpaceEx(path_.c_str(), &freeBytes, &totalBytes, NULL);

        int secondBytes = SECONDBYTES8;
        //if (setting->system->isDeleteProtect()) {
        secondBytes = SECONDBYTES8;
        //} else {
        secondBytes = SECONDBYTES2;
        //}

        if (path_ == ssFlashRecordPath) {
            if (freeBytes.QuadPart < totalBytes.QuadPart * 0.1) {
                //WarningNoFlashDlg->ShowWindow_(SW_SHOW);
                return;
            }
            remainRecordSecond_ = (freeBytes.QuadPart - (totalBytes.QuadPart * 0.1)) / secondBytes + 1;
        } else {
            if (freeBytes.QuadPart < secondBytes*2) {
                //WarningNoFlashDlg->ShowWindow_(SW_SHOW);
                return;
            }
            remainRecordSecond_ = freeBytes.QuadPart / secondBytes + 1;
        }

        if (Start(soundSegment_)) {
            soundSegment_ = Util::shared_ptr<SoundSegment>(new SoundSegment());
            soundSegments_.push_back(soundSegment_);

            CTime t = CTime::GetCurrentTime();
            CString filename;

            //if (setting->system->isDeleteProtect()) {
            oggCodec_->SetQuality(8);
            filename.Format(_T("%02d%02d%02dHQ.spx"), t.GetHour(), t.GetMinute(), t.GetSecond());
            //} else {
            oggCodec_->SetQuality(2);
            filename.Format(_T("%02d%02d%02dST.spx"), t.GetHour(), t.GetMinute(), t.GetSecond());
            //}

            recordFilename_ = path_ + static_cast<LPCTSTR>(filename);
            soundSegment_->filename(recordFilename_);
            soundSegment_->startTime(CTime::GetCurrentTime());
            soundSegment_->contactInfoId(contactInfo_->id());
            soundSegment_->telephoneNumber(contactInfo_->telephoneNumber());
            //soundSegment_->name(contactInfo_->name());
            if (contactInfo_->type() == citMissed) {
                soundSegment_->type(sstLeaveWord);
            } else {
                soundSegment_->type(sstCallRecord);
            }
            soundSegment_->Insert();
            contactInfo_->hasSound(true);
            contactInfo_->Update();
            std::wstring comment = soundSegment_->name() + L"\'" + soundSegment_->telephoneNumber() + L"\'" + Util::StringOp::FromInt(soundSegment_->type()) + L"\'" + Util::StringOp::FromTimestamp(soundSegment_->startTime()) + L"\'";
            if (oggCodec_->StartEncode(Util::StringOp::FromUnicode(recordFilename_), Util::StringOp::FromUnicode(comment))) {
                recordSecond_ = 0;
                //SetTimer(recordTimer_, 1000, 0);
                //m_MJPGList.SetUnitIsShow(104, true);
                //m_MJPGList.SetUnitIsShow(106, true);
                //if (setting->telephone->isMustRecord()) {
                //	m_MJPGList.SetUnitIsShow(2, false);
                //	m_MJPGList.SetUnitIsShow(8, false);
                //} else {
                //	m_MJPGList.SetUnitIsShow(2, false);
                //	m_MJPGList.SetUnitIsShow(8, true);
                //}
            }
        }
    } else {
        if (Stop()) {
            //KillTimer(recordTimer_);
            if (recordSecond_ == 0) {
                recordSecond_ = 1;
            }
            soundSegment_->duration(recordSecond_);
            soundSegment_->Update();
            recordSecond_ = 0;
        }
    }
}
#endif
