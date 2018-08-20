#include "BltRender.h"
#include <WinDef.h> //for BOOL, FALSE, TRUE
#include <Windows.h> //for
#include <WinGDI.h> //for ::GetDC, ::CreateBitmap, ::CreateCompatibleDC, ::SelectObject
#include <string> //for std::wstring

namespace Util {
    namespace Video {
        bool BltRender::Show(Image::ImageFormat const imageFormat, unsigned int const videoWidth, unsigned int const videoHeight, unsigned char const* const videoData) {
            bool result = true;
            bool isConvert = false;
            HBITMAP bitmap = 0;
            //BOOL createBitmapResult = FALSE;
            unsigned char* showData = 0;

            switch (imageFormat) {
                case Image::YUV420P:
                case Image::YVU420P:
                case Image::YUV420:
                case Image::YVU420:
                case Image::YUV422P:
                case Image::YVU422P:
                case Image::YUV422:
                case Image::YVU422:
                case Image::YUV444P:
                case Image::YVU444P:
                case Image::YUV444:
                case Image::YVU444:
                    //if (isLocal_) {
                    //    showData = localShowData;
                    //} else {
                    //    showData = remoteShowData;
                    //}
                    //convertYUVtoRGB_(videoData, imageFormat, videoWidth, videoHeight, showData);
                    isConvert = true;
                case Image::RGB888:

                    if (isConvert) {
                        bitmap = ::CreateBitmap(videoWidth, videoHeight, 1, 24, showData);
                    } else {
                        bitmap = ::CreateBitmap(videoWidth, videoHeight, 1, 24, videoData);
                    }

                    if (true/*createBitmapResult != FALSE*/) {
                        ::UpdateWindow(ownerWindow_);
                        //CDC* pDC = ownerWindow_->GetDC();
                        HDC dc = ::GetDC(ownerWindow_);
                        //CDC bitmapDC;
                        HDC bitmapDC;

                        //if (bitmapDC.CreateCompatibleDC(pDC) == TRUE) {
                        if (bitmapDC = ::CreateCompatibleDC(dc)) {
                            //CBitmap* pOldBitmap = 0;
                            HGDIOBJ oldBitmap = 0;

                            if ((oldBitmap = ::SelectObject(bitmapDC, &bitmap)) != NULL) {
                                if (::BitBlt(dc, 0, 0, winWidth_, winHeight_, bitmapDC, 0, 0, SRCCOPY) == FALSE) {
                                    result = false;
                                }

                                ::SelectObject(bitmapDC, oldBitmap);
                            }

                            ::DeleteDC(bitmapDC);
                        }

                        ::DeleteObject(bitmap);
                    }

                    break;
                case Image::RGB565:
                    bitmap = ::CreateBitmap(videoWidth, videoHeight, 1, 16, videoData);

                    if (true/*createBitmapResult != FALSE*/) {
                        ::UpdateWindow(ownerWindow_);
                        //CDC* pDC = ownerWindow_->GetDC();
                        HDC dc = ::GetDC(ownerWindow_);
                        //CDC bitmapDC;
                        HDC bitmapDC;

                        //if (bitmapDC.CreateCompatibleDC(pDC) == TRUE) {
                        if (bitmapDC = ::CreateCompatibleDC(dc)) {
                            //CBitmap* pOldBitmap = 0;
                            HGDIOBJ oldBitmap = 0;

                            if ((oldBitmap = ::SelectObject(bitmapDC, &bitmap)) != NULL) {
                                if (::BitBlt(dc, 0, 0, winWidth_, winHeight_, bitmapDC, 0, 0, SRCCOPY) == FALSE) {
                                    result = false;
                                }

                                ::SelectObject(bitmapDC, oldBitmap);
                            }

                            ::DeleteDC(bitmapDC);
                        }

                        ::DeleteObject(bitmap);
                    }

                    break;
                default:
                    throw std::wstring(L"not support this type");
                    break;
            }

            return result;
        }
    }
}
