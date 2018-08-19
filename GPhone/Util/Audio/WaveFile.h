#ifndef __UTIL_AUDIO_WAVEFILE_H__
#define __UTIL_AUDIO_WAVEFILE_H__

#include <string>

namespace Util {
    namespace Audio {
        class WaveFile {
        public:
            //if filename = "", open for write
            //else open for read
            WaveFile(std::wstring const & filename = L"");
            ~WaveFile(void);
            void ReadHeader();
            void WriteHeader();
        };
    }
}

#endif //__UTIL_AUDIO_WAVEFILE_H__
