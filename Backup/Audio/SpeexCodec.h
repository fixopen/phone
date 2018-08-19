#ifndef __UTIL_AUDIO_SPEEXCODEC_H__
#define __UTIL_AUDIO_SPEEXCODEC_H__

#include <string>

#include "inc/speex/speex.h" //for encoder decoder
//#include "inc/speex/speex_preprocess.h" //for speex preprocess
//#include "inc/speex/speex_echo.h" //for echo cancel
//#include "inc/speex/speex_jitter.c" //jitter buffer
#include "OggFile.h"

namespace Util {
    namespace Audio {
        //decoder encoder
        //bitstream manipulations
        //Acoustic echo canceller
        class SpeexCodec {
        public:
            SpeexCodec();
            ~SpeexCodec(void);
            void SetOggFile(OggFile* const file) {
                file_ = file;
            }
            void SetQuality();
            void InitEncode(SpeexMode const& mode, size_t const frameSize, size_t const quality);
            std::string const Encode(spx_int16_t* in, size_t const length);
            void UninitEncode();
            void InitDecode(SpeexMode const& mode, size_t const frameSize, int const enh);
            std::wstring const Decode(char* in, size_t const length);
            void UninitDecode();
        private:
            char version_[256];
            SpeexBits bits_;
            void* enc_state_;
            void* dec_state_;
            OggFile* file_;
        };
    }
}

#endif //__UTIL_AUDIO_SPEEXCODEC_H__
