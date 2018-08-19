#ifndef __UTIL_AUDIO_FILEUTIL_H__
#define __UTIL_AUDIO_FILEUTIL_H__

namespace Util {
    namespace Audio {
        namespace FileUtil {
            int const ReadInt(char const* const buffer, size_t const size);
            void WriteInt(char const* const buffer, size_t const size, int const v);
        };
    }
}

#endif //__UTIL_AUDIO_FILEUTIL_H__
