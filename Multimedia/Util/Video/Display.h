#ifndef __UTIL_VIDEO_DISPLAY_H__
#define __UTIL_VIDEO_DISPLAY_H__

#include <windef.h> //for HWND
#include "VideoDecoder.h" //for VideoDecoder
#include "DxRender.h" //for DxRender
#include "../Image/Image.h" //for Image::ImageFormat

namespace Util {
    namespace Video {
        class Display {
        public:
            Display();
            bool SetParameters(HWND const videoWindow, size_t const videoWidth = 176, size_t const videoHeight = 144, bool const isLocal = true);
            bool Show(Image::ImageFormat const imageFormat, unsigned int const videoWidth, unsigned int const videoHeight, unsigned char const* const videoData);
            bool Init();
            bool Finally();
        private:
            void convertYUVtoRGB_(unsigned char const* const yuvData, Image::ImageFormat const yuvFormat, size_t const width, size_t const height, unsigned char* const rgb888Data);
            void initConvertTable_();
        private:
            bool isLocal_;
            unsigned int winWidth_;
            unsigned int winHeight_;
            Image::ImageFormat imageFormat_;
            //CWnd* ownerWindow_;
            HWND ownerWindow_;
            DxRender* dxRender_;
            void* render_;
        };
    }
}

#endif //__UTIL_VIDEO_DISPLAY_H__
