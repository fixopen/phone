#ifndef __UTIL_VIDEO_IMAGEOP_H__
#define __UTIL_VIDEO_IMAGEOP_H__

#include <windef.h>
#include <objbase.h>
#include <imaging.h>

#include <string>
#include <vector>
#include <map>

//using cximage for png & jpeg2000 & dcr
//using system for bmp & jpeg & gif
//using self draw code for animal gif

namespace Util {
    namespace Video {
        class ImageOp{
        public:
            void DrawImage(HDC const destination, RECT const& dstBounds, std::wstring const& sourceFilename, SIZE const& imageSize, RECT const& srcBounds);
            static HDC const GetMaskImage(std::wstring const& maskFilename, SIZE const& maskSize);
            static HDC const GetImage(std::wstring const& imageFilename, SIZE const& imageSize);
            static void PrepareImage(std::map<std::wstring, SIZE> const& images);
        private:
            static void drawImage_(std::wstring const& imageFilename, SIZE const& imageSize);
            static void drawMask_(std::wstring const& imageFilename, SIZE const& imageSize);
            static IImagingFactory* const beforeDraw_();
            static void draw_(IImagingFactory* const imageFactory, std::wstring const& imageFilename, SIZE const& imageSize);
            static void afterDraw_(IImagingFactory* const imageFactory);
            static wchar_t const* const findExtension_(wchar_t const* const name);
        private:
            static std::map<std::wstring, HDC> images_;
            //static std::map<std::wstring, HBITMAP> masks_;
            static std::map<std::wstring, HDC> masks_;
        };
    }
}

#endif //__UTIL_VIDEO_IMAGEOP_H__
