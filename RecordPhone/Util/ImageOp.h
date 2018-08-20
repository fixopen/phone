#ifndef __UTIL_IMAGEOP_H__
#define __UTIL_IMAGEOP_H__

#include <windef.h>
#include <objbase.h>
#include <imaging.h>

#include <string>
#include <vector>
#include <map>

namespace Util {
    class ImageOp{
    public:
		static HDC const GetMaskImage(std::wstring const& maskFilename, SIZE const& maskSize);
        static HDC const GetImage(std::wstring const& imageFilename, SIZE const& imageSize);
		static void PrepareImage(std::map<std::wstring, SIZE> const& images);
	private:
		static void drawImage_(std::wstring const& imageFilename, SIZE const& imageSize);
		static void drawMask_(std::wstring const& imageFilename, SIZE const& imageSize);
		static IImagingFactory* const beforeDraw_();
		static void draw_(IImagingFactory* const imageFactory, std::wstring const& imageFilename, SIZE const& imageSize);
		static void afterDraw_(IImagingFactory* const imageFactory);
    private:
        static std::map<std::wstring, HDC> images_;
		//static std::map<std::wstring, HBITMAP> masks_;
		static std::map<std::wstring, HDC> masks_;
    };
}

#if 0
		//struct ImageCacheEntry {
		//    std::wstring filename;
		//    HDC dc;
		//    HBITMAP bitmap;
		//    HBITMAP oldBitmap;
		//};
		//static void DrawImage(HDC const destination, RECT const& dstBounds, std::wstring const& imageFilename, SIZE const& imageSize, RECT const& srcBounds);
		//static HDC const GetImage(std::wstring const& imageFilename, SIZE const& imageSize, HDC const hDC);
		//static wchar_t const* const findExtension_(wchar_t const* const name);
		//static int const findCacheIndex_(std::wstring const& filename);
		//static int const addToImageCache_(std::wstring const& filename, HDC const dc, SIZE const& imageSize);
		//static SIZE const getImageSize_(std::wstring const& filename);
		//static std::vector<ImageCacheEntry> imageCacheDirectory;
#endif

#endif //__UTIL_IMAGEOP_H__
