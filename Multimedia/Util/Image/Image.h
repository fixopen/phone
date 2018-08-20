#ifndef __UTIL_IMAGE_IMAGE_H__
#define __UTIL_IMAGE_IMAGE_H__

#pragma once

#include <string> //for std::wstring

namespace Util {
    namespace Image {
        enum ImageFormat {
            YUV420,
            YUV420P,
            YUV422,
            YUV422P,
            YUV444,
            YUV444P,
            YVU420,
            YVU420P,
            YVU422,
            YVU422P,
            YVU444,
            YVU444P,
            RGB565,
            RGB888,
            RGBA8888,
            RGB555,
            CMYK,
        };
        class Image {
        public:
            Image();
            Image(std::wstring const& filename);
            virtual ~Image();
        private:
            std::wstring filename_;
        };
    }
}

#endif //__UTIL_IMAGE_IMAGE_H__

