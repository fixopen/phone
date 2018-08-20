#ifndef __UTIL_AUDIO_OGGFILEWRITER_H__
#define __UTIL_AUDIO_OGGFILEWRITER_H__

#include <string>
#include <stdio.h>
#include "inc/speex/ogg.h"

namespace Util {
    namespace Audio {
        class OggFileWriter {
        public:
            //if filename = "", open for write
            //else open for read
            OggFileWriter(std::wstring const & filename = L"");
            ~OggFileWriter();
            void WriteHeader();
            void WriteComment();
            void PushData(void* data, size_t const length);
        private:
            FILE* file_;
            ogg_stream_state* stream_;
            ogg_page page_;
            int packetNo_;
        };
    }
}

#endif //__UTIL_AUDIO_OGGFILEWRITER_H__
