#include "Image.h"
#include <d2d1.h> //for Direct2D

namespace Util {
    namespace Image {
        Image::Image() {
        }

        Image::Image(std::wstring const& filename) {
        }

        Image::~Image() {
        }

        //extern "C" IDirectDrawSurface* DDLoadBitmap(IDirectDraw *pdd, LPCSTR szBitmap, int dx, int dy) {
        //    HBITMAP             hbm;
        //    BITMAP              bm;
        //    DDSURFACEDESC       ddsd;
        //    IDirectDrawSurface *pdds;

        //    //
        //    //  This is the Windows CE part.
        //    //  Try to load the bitmap as a resource.
        //    //
        //    hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap,
        //        IMAGE_BITMAP, dx, dy, LR_CREATEDIBSECTION);

        //    if (hbm == NULL)
        //        return NULL;

        //    //
        //    // Get the size of the bitmap.
        //    //
        //    GetObject(hbm, sizeof(bm), &bm);

        //    //
        //    // Now, return to DirectX function calls.
        //    // Create a DirectDrawSurface for this bitmap.
        //    //
        //    memset(&ddsd, 0, sizeof(ddsd));
        //    ddsd.dwSize = sizeof(ddsd);
        //    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
        //    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
        //    ddsd.dwWidth = bm.bmWidth;
        //    ddsd.dwHeight = bm.bmHeight;

        //    if (pdd->CreateSurface(&ddsd, &pdds, NULL) != DD_OK)
        //        return NULL;

        //    DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);

        //    DeleteObject(hbm);

        //    return pdds;
        //}

        template<class Interface>
        inline void SafeRelease(Interface** ppInterfaceToRelease) {
            if (*ppInterfaceToRelease != NULL) {
                (*ppInterfaceToRelease)->Release();
                (*ppInterfaceToRelease) = NULL;
            }
        }

        void drawExam(HWND const hwnd) {
            //ID2D1Factory* pD2DFactory = NULL;
            //HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
            //// Obtain the size of the drawing area.
            //RECT rc;
            //GetClientRect(hwnd, &rc);
            //// Create a Direct2D render target
            //ID2D1HwndRenderTarget* pRT = NULL;
            //hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
            //    D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
            //    &pRT);
            //ID2D1SolidColorBrush* pBlackBrush = NULL;
            //if (SUCCEEDED(hr)) {
            //    pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black),
            //        &pBlackBrush);
            //}
            //pRT->BeginDraw();
            //pRT->DrawRectangle(
            //    D2D1::RectF(rc.left + 100.0f, rc.top + 100.0f, rc.right - 100.0f, rc.bottom - 100.0f),
            //    pBlackBrush);
            //hr = pRT->EndDraw();
            //SafeRelease(pRT);
            //SafeRelease(pBlackBrush);
            //SafeRelease(pD2DFactory);
        }
    }
}
