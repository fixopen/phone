#ifndef __UTIL_AUDIO_SPEEXENCODER_H__
#define __UTIL_AUDIO_SPEEXENCODER_H__

#include <string>

#include "inc/speex/speex.h" //for encoder decoder
//#include "inc/speex/speex_preprocess.h" //for speex preprocess
//#include "inc/speex/speex_echo.h" //for echo cancel
//#include "inc/speex/speex_jitter.c" //jitter buffer
#include "OggFileWriter.h"

namespace Util {
    namespace Audio {
        //decoder encoder
        //bitstream manipulations
        //Acoustic echo canceller
        class SpeexEncoder {
        public:
            SpeexEncoder();
            ~SpeexEncoder(void);
            void SetOggFileWriter(OggFileWriter* const file) {
                file_ = file;
            }
            void SetQuality();
            void InitEncode(SpeexMode const & mode, size_t const frameSize, size_t const quality);
            std::string const Encode(spx_int16_t* in, size_t const length);
            void UninitEncode();
        private:
            SpeexBits bits_;
            void* enc_state_;
            OggFileWriter* file_;
        };
    }
}

#endif //__UTIL_AUDIO_SPEEXENCODER_H__
