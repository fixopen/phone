#ifndef __UTIL_VIDEO_DISPLAY_H__
#define __UTIL_VIDEO_DISPLAY_H__

#include <windef.h> //for HWND
#include <afxwin.h> //for CWnd

#include "VideoCodec.h"
#include "DxRender.h"

namespace Util {
    namespace Video {
        class Display {
        public:
            Display();
            bool SetParameters(HWND const videoWindow, size_t const videoWidth = 176, size_t const videoHeight = 144, bool const isLocal = true);
            bool Show(ImageFormat const imageFormat, unsigned int const videoWidth, unsigned int const videoHeight, unsigned char const * const videoData);
            bool Init();
            bool Finally();
        private:
            void convertYUVtoRGB_(unsigned char const * const yuvData, ImageFormat const yuvFormat, size_t const width, size_t const height, unsigned char* const rgb888Data);
            void initConvertTable_();
        private:
            bool isLocal_;
            unsigned int winWidth_;
            unsigned int winHeight_;
            ImageFormat imageFormat_;
            CWnd* ownerWindow_;
            DxRender* dxRender_;
            void* render_;
        };
    }
}

#endif //__UTIL_VIDEO_DISPLAY_H__
