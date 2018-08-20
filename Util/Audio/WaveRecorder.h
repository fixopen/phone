#ifndef __UTIL_AUDIO_WAVERECORDER_H__
#define __UTIL_AUDIO_WAVERECORDER_H__

#define  WAVE_BUFFER_COUNT 2

#include <windef.h>
#include <mmsystem.h>
#include <string>
#include "WaveUtil.h"
#include "SpeexEncoder.h"
#include "../SmartPtr.h"
#include "../DateTime.h"

//Device->SpeexEncode->OggFileWriter
//speexCodec_ = new SpeexCodec();
//speexCodec_->InitEncode(speex_nb_mode, frameSize, quality); //speex_wb_mode, speex_uwb_mode

namespace Util {
    namespace Audio {
        class WaveRecorder {
        public:
            typedef void (*DataProcessor)(void const* const data, size_t const length);
            static WaveRecorder* const WaveRecorder::Instance();
            WaveRecorder();
            void RegisterDataProcessor(DataProcessor const dataProcessor) {
                dataProcessor_ = dataProcessor;
            }
            std::string const SpitData();
            void SetEncoder(SpeexEncoder* const speexEncoder) {
                speexEncoder_ = speexEncoder;
            }
            SpeexEncoder* const Encoder() {
                return speexEncoder_;
            }
            bool const Start(std::wstring const& filename, std::wstring const& comment = L"", bool const isBusyToneDetect = false);
            bool const Stop();
            bool const IsRecording() const {
                return isRecording_;
            }
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
            SpeexEncoder* speexEncoder_;
        };
    }
}

#endif //__UTIL_AUDIO_WAVERECORDER_H__
