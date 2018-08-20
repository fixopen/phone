#ifndef __UTIL_VIDEO_IMAGEOPATATOR_H__
#define __UTIL_VIDEO_IMAGEOPATATOR_H__

#include "../Symbols.h"
#include "../DateTime.h"
#include <string>

namespace Util {
    namespace Video {
        class SYMBOL ImageOp {
        public:
            static void DrawImage(HDC destination, RECT destPosition, std::wstring sourceFilename, RECT sourcePosition);
            static void DrawImage(HDC destination, RECT destPosition, std::wstring sourceFilename);
            static void DrawImage(HDC destination, std::wstring sourceFilename, RECT sourcePosition);
            static void DrawImage(HDC destination, std::wstring sourceFilename);
        private:
            static wchar_t const * const ImageOp::findExtension_(wchar_t const * const name);
        };
    }
}

#endif //__UTIL_VIDEO_IMAGEOPATATOR_H__
