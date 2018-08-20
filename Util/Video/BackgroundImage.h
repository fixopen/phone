#ifndef __UTIL_VIDEO_BACKGROUNDIMAGE_H__
#define __UTIL_VIDEO_BACKGROUNDIMAGE_H__

#include <windef.h>
#include <string>
#include "Screen.h"

namespace Util {
    namespace Video {
        class BackgroundImage {
        public:
            static BOOL const drawBackground(HWND window, std::wstring const& imageFilename = L"/FlashDrv/debug/NULL.jpg", SIZE const& imageSize = Screen::size());
            static HBRUSH const setStaticProperties(HDC const dc, RECT const& bounds, HDC const backgroundDc, HFONT const font, COLORREF const textColor);
            static void setFont(HDC const dc, HFONT const& font);
        };
    }
}

#endif //__UTIL_VIDEO_BACKGROUNDIMAGE_H__
