#ifndef __UTIL_AUDIO_OGGFILE_H__
#define __UTIL_AUDIO_OGGFILE_H__

#include <string>
#include "inc/speex/ogg.h"

namespace Util {
    namespace Audio {
        class OggFile {
        public:
            //if filename = "", open for write
            //else open for read
            OggFile(std::wstring const & filename = L"", bool const isRead = true);
            ~OggFile();
            void ReadHeader();
            void WriteHeader();
            void PushData(void* data, size_t const length);
            std::string const ReadData(size_t const length);
        private:
            ogg_stream_state* os_;
            ogg_sync_state sync_;
            bool isRead_;
        };
    }
}

#endif //__UTIL_AUDIO_OGGFILE_H__
