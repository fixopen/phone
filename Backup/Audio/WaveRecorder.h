#ifndef __UTIL_AUDIO_WAVERECORDER_H__
#define __UTIL_AUDIO_WAVERECORDER_H__

#define  WAVE_BUFFER_COUNT 2

#include <windef.h>
#include <mmsystem.h>
#include <string>
#include "WaveUtil.h"
#include "SpeexCodec.h"
#include "../SmartPtr.h"
#include "../DateTime.h"
//#include "../../Resource.h"

//Device->SpeexEncode->OggFileWriter
//speexCodec_ = new SpeexCodec();
//speexCodec_->InitEncode(speex_nb_mode, frameSize, quality); //speex_wb_mode, speex_uwb_mode

namespace Util {
    namespace Audio {
        class WaveRecorder/* : public CDialog*/ {
            //DECLARE_DYNAMIC(WaveRecorder)

        public:
            typedef void (*DataProcessor)(void const* const data, size_t const length);
            static WaveRecorder* const WaveRecorder::Instance();
            WaveRecorder(/*CWnd* pParent = NULL*/);   // standard constructor

            // Dialog Data
            //enum { IDD = IDD_DIALOG_WAVE_RECORDER };

            void RegisterDataProcessor(DataProcessor const dataProcessor) {
                dataProcessor_ = dataProcessor;
            }
            std::string const SpitData();
            void SetEncode(SpeexCodec* const speexCodec) {
                speexCodec_ = speexCodec;
            }
            bool const Start(std::wstring const& filename, std::wstring const& comment = L"", bool const isBusyToneDetect = false);
            //bool const Start(Util::shared_ptr<SoundSegment>& soundSegment);
            bool const Stop();
            std::wstring const GetFilename(Timestamp const& time, bool const isHighQuality);
            //void Record();
            bool const IsRecording() const {
                return isRecording_;
            }
        protected:
            //virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
            //afx_msg LRESULT OnMM_WIM_OPEN(WPARAM wParam, LPARAM lParam);
            //afx_msg LRESULT OnMM_WIM_DATA(WPARAM wParam, LPARAM lParam);
            //afx_msg LRESULT OnMM_WIM_CLOSE(WPARAM wParam, LPARAM lParam);

            //DECLARE_MESSAGE_MAP()
        private:
            HWAVEIN waveIn_;
            unsigned char* buffers_[WAVE_BUFFER_COUNT];
            PWAVEHDR waveHeaders_[WAVE_BUFFER_COUNT];
            WAVEFORMATEX waveFormat_;
            DataProcessor dataProcessor_;
            bool isRecording_;
            bool isBusyToneDetect_;
            volatile bool isEnding_;
            int dataCount_;
            DWORD dataLength_;
            SpeexCodec* speexCodec_;
            //OggCodec* oggCodec_;
        };
    }
}

#endif //__UTIL_AUDIO_WAVERECORDER_H__
