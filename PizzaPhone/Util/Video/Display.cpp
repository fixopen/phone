#include "Display.h"
#include <string>

namespace Util {
    namespace Video {
        Display::Display()
        : ownerWindow_(0) {
        }

        bool Display::Init() {
            return true;
        }

        bool Display::Finally() {
            if (ownerWindow_) {
                if (render_) {
                    dxRender_->DeleteSurface(render_);
                }
                dxRender_->Finally();
                if (!ownerWindow_->Detach()) {
                    DWORD error = GetLastError();
                    TRACE1("pWnd->Detach Failed with: %i\n", error);
                }
                delete ownerWindow_;
                ownerWindow_ = 0;
            }
            return true;
        }

        bool Display::SetParameters(HWND const videoWindow, size_t const videoWidth, size_t const videoHeight, bool const isLocal) {
            isLocal_ = isLocal;
            ownerWindow_ = CWnd::FromHandle(videoWindow);
            if (!ownerWindow_) {
                DWORD error = GetLastError();
                TRACE1("pWnd->Attach() Failed with: %i\n", error);
                return false;
            }
            RECT bounds;
            ownerWindow_->GetWindowRect(&bounds);
            winWidth_ = bounds.right - bounds.left;
            winHeight_ = bounds.bottom - bounds.top;
            if (!dxRender_->Init(videoWindow, videoWidth, videoHeight, SURFACE_TYPE_RGB565)) {
                DWORD error = GetLastError();
                TRACE1("mfc_render_init() Failed with: %i\n", error);
                return false;
            }
            render_ = dxRender_->CreateSurface(SURFACE_TYPE_RGB565, 0, 0, videoWidth, videoHeight, videoWidth, videoHeight);
            if (!render_) {
                DWORD error = GetLastError();
                TRACE1("mfc_render_create_overlay() Failed with: %i\n", error);
                return false;
            }
            dxRender_->left = bounds.left;
            dxRender_->top = bounds.top;
            dxRender_->bounds.left = bounds.left;
            dxRender_->bounds.top = bounds.top;
            dxRender_->bounds.right += bounds.left + videoWidth;
            dxRender_->bounds.bottom += bounds.top + videoHeight;
            return true;
        }

        namespace {
            unsigned char localShowData[96 * 1024] = {
                0
            };
            unsigned char remoteShowData[96 * 1024] = {
                0
            };
        }

        bool Display::Show(ImageFormat const imageFormat, unsigned int const videoWidth, unsigned int const videoHeight, unsigned char const * const videoData) {
            bool result = true;
            bool isConvert = false;
            CBitmap bitmap;
            BOOL createBitmapResult = FALSE;
            unsigned char* showData = 0;
            switch (imageFormat) {
            case YUV420P:
                dxRender_->Do(render_, videoData, videoWidth, videoHeight, IMAGE_TYPE_YUV420);
                break;
            case YVU420P:
            case YUV420:
            case YVU420:
            case YUV422P:
            case YVU422P:
            case YUV422:
            case YVU422:
            case YUV444P:
            case YVU444P:
            case YUV444:
            case YVU444:
                if (isLocal_) {
                    showData = localShowData;
                } else {
                    showData = remoteShowData;
                }
                convertYUVtoRGB_(videoData, imageFormat, videoWidth, videoHeight, showData);
                isConvert = true;
            case RGB888:
                if (isConvert) {
                    createBitmapResult = bitmap.CreateBitmap(videoWidth, videoHeight, 1, 24, showData);
                } else {
                    createBitmapResult = bitmap.CreateBitmap(videoWidth, videoHeight, 1, 24, videoData);
                }
                if (createBitmapResult != FALSE) {
                    ownerWindow_->UpdateWindow();
                    CDC* pDC = ownerWindow_->GetDC();
                    CDC bitmapDC;
                    if (bitmapDC.CreateCompatibleDC(pDC) == TRUE) {
                        CBitmap* pOldBitmap = 0;
                        if ((pOldBitmap = bitmapDC.SelectObject(&bitmap)) != NULL) {
                            if (pDC->BitBlt(0, 0, winWidth_, winHeight_, &bitmapDC, 0, 0, SRCCOPY) == FALSE) {
                                result = false;
                            }
                            bitmapDC.SelectObject(pOldBitmap);
                        }
                        bitmapDC.DeleteDC();
                    }
                    bitmap.DeleteObject();
                }
                break;
            case RGB565:
                if (bitmap.CreateBitmap(videoWidth, videoHeight, 1, 16, videoData) != FALSE) {
                    ownerWindow_->UpdateWindow();
                    CDC* pDC = ownerWindow_->GetDC();
                    CDC bitmapDC;
                    if (bitmapDC.CreateCompatibleDC(pDC) == TRUE) {
                        CBitmap* pOldBitmap;
                        if ((pOldBitmap = bitmapDC.SelectObject(&bitmap)) != NULL) {
                            if (pDC->BitBlt(0, 0, winWidth_, winHeight_, &bitmapDC, 0, 0, SRCCOPY) == FALSE) {
                                result = false;
                            }
                            bitmapDC.SelectObject(pOldBitmap);
                        }
                        bitmapDC.DeleteDC();
                    }
                    bitmap.DeleteObject();
                }
                break;
            default:
                throw std::wstring(L"not support this type");
                break;
            }
            return result;
        }

        namespace {
            long int crv_tab[256];
            long int cbu_tab[256];
            long int cgu_tab[256];
            long int cgv_tab[256];
            long int tab_76309[256];
            unsigned char clp[1024]; //for clip in CCIR601
        }

        void Display::convertYUVtoRGB_(unsigned char const * const yuvData, ImageFormat const yuvFormat, size_t const width, size_t const height, unsigned char* const rgb888Data) {
            static bool isInitConvertTable = false;
            if (!isInitConvertTable) {
                initConvertTable_();
                isInitConvertTable = true;
            }
            unsigned char const * yBuffer = 0;
            unsigned char const * uBuffer = 0;
            unsigned char const * vBuffer = 0;
            switch (yuvFormat) {
            case YUV420P:
                yBuffer = yuvData;
                uBuffer = yuvData + width * height;
                vBuffer = yuvData + width * height + width * height / 4;
                break;
            case YVU420P:
                yBuffer = yuvData;
                uBuffer = yuvData + width * height + width * height / 4;
                vBuffer = yuvData + width * height;
                break;
            case YUV422P:
                break;
            case YVU422P:
                break;
            case YUV444P:
                break;
            case YVU444P:
                break;
            default:
                break;
            }
            int y1, y2, u, v; 
            unsigned char const *py1, * py2;
            int c1, c2, c3, c4;
            unsigned char* d1, * d2;

            py1 = yBuffer;
            py2 = py1 + width;
            d1 = rgb888Data;
            d2 = d1 + 3 * width;
            for (size_t i = 0; i < height; i += 2) {
                for (size_t j = 0; j < width; j += 2) {
                    u = *uBuffer++;
                    v = *vBuffer++;

                    c1 = crv_tab[v];
                    c2 = cgu_tab[u];
                    c3 = cgv_tab[v];
                    c4 = cbu_tab[u];

                    //up-left
                    y1 = tab_76309[*py1++]; 
                    *d1++ = clp[384 + ((y1 + c1) >> 16)];  
                    *d1++ = clp[384 + ((y1 - c2 - c3) >> 16)];
                    *d1++ = clp[384 + ((y1 + c4) >> 16)];

                    //down-left
                    y2 = tab_76309[*py2++];
                    *d2++ = clp[384 + ((y2 + c1) >> 16)];  
                    *d2++ = clp[384 + ((y2 - c2 - c3) >> 16)];
                    *d2++ = clp[384 + ((y2 + c4) >> 16)];

                    //up-right
                    y1 = tab_76309[*py1++];
                    *d1++ = clp[384 + ((y1 + c1) >> 16)];  
                    *d1++ = clp[384 + ((y1 - c2 - c3) >> 16)];
                    *d1++ = clp[384 + ((y1 + c4) >> 16)];

                    //down-right
                    y2 = tab_76309[*py2++];
                    *d2++ = clp[384 + ((y2 + c1) >> 16)];  
                    *d2++ = clp[384 + ((y2 - c2 - c3) >> 16)];
                    *d2++ = clp[384 + ((y2 + c4) >> 16)];
                }
                d1 += 3 * width;
                d2 += 3 * width;
                py1 += width;
                py2 += width;
            }
        }

        void Display::initConvertTable_() {
            long int const crv = 104597;
            long int const cbu = 132201;
            long int const cgu = 25675;
            long int const cgv = 53279;
            for (size_t i = 0; i < 256; ++i) {
                crv_tab[i] = (i - 128) * crv;
                cbu_tab[i] = (i - 128) * cbu;
                cgu_tab[i] = (i - 128) * cgu;
                cgv_tab[i] = (i - 128) * cgv;
                tab_76309[i] = 76309 * (i - 16);
            }

            size_t index = 0;
            for (size_t i = 0; i < 384; ++i) {
                clp[i] = 0;
            }
            index = 384;
            for (size_t i = 0; i < 256; ++i) {
                clp[index++] = i;
            }
            index = 640;
            for (size_t i = 0; i < 384; ++i) {
                clp[index++] = 255;
            }
        }
    }
}
