#ifndef __UTIL_VIDEO_BLTRENDER_H__
#define __UTIL_VIDEO_BLTRENDER_H__

#include <WinDef.h> //for HWND
#include "VideoRender.h" //for VideoRender
#include "../Image/Image.h" //for Image::ImageFormat

namespace Util {
    namespace Video {
        class BltRender : public VideoRender {
        public:
            bool Show(Image::ImageFormat const imageFormat, unsigned int const videoWidth, unsigned int const videoHeight, unsigned char const* const videoData);
        private:
            bool isLocal_;
            HWND ownerWindow_;
            short winWidth_;
            short winHeight_;
        };
    }
}

#endif //__UTIL_VIDEO_BLTRENDER_H__
