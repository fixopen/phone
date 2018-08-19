#include "DxRender.h"

#define PREFIX TEXT("\n{MFC DDRAW TEST}: ")
#define PREFIX_LEN 12

namespace Util {
    namespace Video {
        namespace {
            bool const PRINT_ERRMSG(LPCTSTR szError, ...) {
                static TCHAR szBuff[128] = PREFIX;
                va_list vl;
                va_start(vl, szError);
                StringCchVPrintf(szBuff + PREFIX_LEN, (128 - PREFIX_LEN), szError, vl);
                OutputDebugString(szBuff);
                va_end(vl);
                return false;
            }

            DDPIXELFORMAT OverlayPixelFormats[] = {
                {sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('Y', 'V', '1', '2'), 0, 0, 0, 0, 0},
                {sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('Y', 'U', 'Y', 'V'),0,0,0,0,0},  // YUYV
                {sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('U', 'Y', 'V', 'Y'),0,0,0,0,0},  // UYVY
                {sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16,  0x7C00, 0x03e0, 0x001F, 0},         // 16-bit RGB 5:5:5
                {sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16,  0xF800, 0x07e0, 0x001F, 0},         // 16-bit RGB 5:6:5
                {sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0}
            };

            HRESULT WINAPI EnumSurfacesCallback(LPDIRECTDRAWSURFACE lpDDSurface, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext) {
                LPDIRECTDRAWSURFACE* ppDDSurface = (LPDIRECTDRAWSURFACE *)lpContext;
                HRESULT hr = (HRESULT)DDENUMRET_OK;
                *ppDDSurface = lpDDSurface;
                RETAILMSG(1, (L"\nEnumSurface Callback function."));
                return hr;
            }
        }
        extern "C" void _initConvTab();
        extern "C" void _yuv420ToRgb565(unsigned char const* const p_lum, unsigned char const* const p_cb, unsigned char const* const p_cr, int w_src, int h_src, unsigned char* const dest, int w_dst, int h_dst, int topdown);

        LPDIRECTDRAW DxRender::s_DirectDraw_ = NULL;
        LPDIRECTDRAWSURFACE DxRender::s_DirectDrawPrimarySurface_ = NULL;        // Primary Surface.

        bool DxRender::s_Inited_ = false;
        bool DxRender::s_Finaled_ = false;

        bool const DxRender::Init(HWND const windowHandle, DWORD const videoWidth, DWORD const videoHeight, int const surfaceType) {
            if (s_Inited_) {
                return true;
            }
            s_Inited_ = true;
            //Check the type of the surface
            switch (surfaceType) {
            case SURFACE_TYPE_YV12:
            case SURFACE_TYPE_RGB565:
                break;
            default:
                PRINT_ERRMSG(TEXT("surfaceType is not supported."));
                return false;
            }
            // Create the main DirectDraw object
            HRESULT r = DirectDrawCreate(NULL, &s_DirectDraw_, NULL);
            if (r != DD_OK) {
                return PRINT_ERRMSG(TEXT("DirectDrawCreate FAILED"));
            }
            // Get exclusive mode
            r = s_DirectDraw_->SetCooperativeLevel(windowHandle, DDSCL_NORMAL);
            if (r != DD_OK) {
                return PRINT_ERRMSG(TEXT("SetCooperativeLevel FAILED"));
            }
            // Get a primary surface interface pointer (only needed for init.)
            DDSURFACEDESC surfaceDesc;
            memset(&surfaceDesc, 0, sizeof(surfaceDesc));
            surfaceDesc.dwSize = sizeof(surfaceDesc);
            surfaceDesc.dwFlags = DDSD_CAPS;
            surfaceDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
            r = s_DirectDraw_->CreateSurface(&surfaceDesc, &s_DirectDrawPrimarySurface_, NULL);
            if (r != DD_OK) {
                return PRINT_ERRMSG(TEXT("CreateSurface s_DirectDrawPrimarySurface_ FAILED"));
            }
            // SW YUV->RGB conversion table initialization
            _initConvTab();
            return true;
        }

        void DxRender::Finally() {
            if (s_Finaled_) {
                PRINT_ERRMSG(TEXT("Has Deinited!\n"));
                return;
            }
            s_Finaled_ = true;
            s_ReleaseAllObjects_();
        }

        void* const DxRender::CreateSurface(int const surfaceType, int const x, int const y, int const srcWidth, int const srcHeight, int const dstWidth, int const dstHeight) {
            switch (surfaceType) {
            case SURFACE_TYPE_YV12:
            case SURFACE_TYPE_RGB565:
                break;
            default:
                PRINT_ERRMSG(TEXT("surfaceType is not supported."));
                return NULL;
            }
            RenderOverlay* renderOverlay = (RenderOverlay*)malloc(sizeof(RenderOverlay));
            if (renderOverlay == NULL) {
                PRINT_ERRMSG(TEXT("malloc failed."));
                return NULL;
            }
            offlineScreen_ = renderOverlay;
            // Create the overlay flipping surface. We will attempt the pixel formats
            // in our table one at a time until we find one that jives.
            DDSURFACEDESC surfaceDesc;
            memset(&surfaceDesc, 0, sizeof(surfaceDesc));
            surfaceDesc.dwSize = sizeof(surfaceDesc);
            surfaceDesc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
            surfaceDesc.dwWidth = srcWidth;
            surfaceDesc.dwHeight = srcHeight;
            surfaceDesc.ddpfPixelFormat = OverlayPixelFormats[surfaceType];
            LPDIRECTDRAWSURFACE overlaySurface = NULL; // Overlay Surface.
            HRESULT r = s_DirectDraw_->CreateSurface(&surfaceDesc, &overlaySurface, NULL);
            if (r != DD_OK) {
                PRINT_ERRMSG(TEXT("Unable to create surface!"));
                free(renderOverlay);
                return NULL;
            }
            RETAILMSG(1, (L"\n\t SURFACE TYPE %d\n", surfaceType));
            renderOverlay->directDrawSurface = overlaySurface;
            renderOverlay->x = x;
            renderOverlay->y = y;
            renderOverlay->srcWidth = srcWidth;
            renderOverlay->srcHeight = srcHeight;
            renderOverlay->dstWidth = dstWidth;
            renderOverlay->dstHeight = dstHeight;
            renderOverlay->surfaceType = surfaceType;
            return (void*)renderOverlay;
        }

        void DxRender::Do(void* renderHandle, unsigned char const* const imageData, int width, int height, int imageType) {
            switch (imageType) {
            case IMAGE_TYPE_YUV420:
            case IMAGE_TYPE_RGB565:
            case IMAGE_TYPE_YV12:
                break;
            default:
                PRINT_ERRMSG(TEXT("imageType is not supported."));
                return;
            }
            if (renderHandle == NULL) {
                PRINT_ERRMSG(TEXT("\nmfc_render_handle is NULL."));
                return;
            }
            if (imageData == NULL) {
                PRINT_ERRMSG(TEXT("Input image is NULL."));
                return;
            }
            RenderOverlay* renderOverlay = (RenderOverlay*)renderHandle;
            LPDIRECTDRAWSURFACE offlineScreenSurface = renderOverlay->directDrawSurface; // Overlay Surface.
            DDSURFACEDESC surfaceDesc;
            memset(&surfaceDesc, 0, sizeof(surfaceDesc));
            surfaceDesc.dwSize = sizeof(surfaceDesc);
            // Lock down the surface so we can modify its contents.
            HRESULT r = offlineScreenSurface->Lock(NULL, &surfaceDesc, DDLOCK_WAITNOTBUSY, NULL);
            if (r != DD_OK) {
                PRINT_ERRMSG(TEXT("Unable to lock OffScreen surface!"));
                return;
            }
            unsigned char* surface = (LPBYTE)surfaceDesc.lpSurface;
            int ySize = width * height;
            int uSize = ySize >> 2;
            switch (renderOverlay->surfaceType) {
            case SURFACE_TYPE_YV12:
                if (imageType == IMAGE_TYPE_YUV420) {
                    memcpy(surface, imageData, ySize);
                    memcpy(surface + ySize, imageData + ySize + uSize, uSize);
                    memcpy(surface + ySize + uSize, imageData + ySize, uSize);
                } else if (imageType == IMAGE_TYPE_YV12) {
                    memcpy(surface, imageData, ySize + (uSize << 1));
                } else if (imageType == IMAGE_TYPE_RGB565) {
                    PRINT_ERRMSG(TEXT("This case is not supported. : (Surface type is YV12 and Image type is RGB565)"));
                }
                break;
            case SURFACE_TYPE_RGB565:
                if (imageType == IMAGE_TYPE_YUV420) {
                    _yuv420ToRgb565(imageData, imageData + ySize, imageData + ySize + uSize, width, height, surface, width, height, 0);
                } else if (imageType == IMAGE_TYPE_YV12) {
                    _yuv420ToRgb565(imageData, imageData + ySize + uSize, imageData + ySize, width, height, surface, width, height, 0);
                } else if (imageType == IMAGE_TYPE_RGB565) {
                    memcpy(surface, imageData, ySize << 1);
                }
                break;
            default:
                break;
            }
            offlineScreenSurface->Unlock(NULL);
            //Blt to the overlay back_surface
            while (TRUE) {
                r = s_DirectDrawPrimarySurface_->Blt(&bounds, offlineScreenSurface, NULL, DDBLT_WAITNOTBUSY, NULL);
                if (SUCCEEDED(r)) {
                    break;
                }
                if (r != DDERR_WASSTILLDRAWING) {
                    return;
                }
            }
        }

        void DxRender::DeleteSurface(void* renderHandle) {
            if (renderHandle == NULL) {
                PRINT_ERRMSG(TEXT("\nmfc_render_handle is NULL."));
                return;
            }
            RenderOverlay* renderOverlay = (RenderOverlay *)renderHandle;
            LPDIRECTDRAWSURFACE overlaySurface = renderOverlay->directDrawSurface;
            if (overlaySurface != NULL) {
                overlaySurface->Release();
                overlaySurface = NULL;
            }
            free(renderHandle);
            offlineScreen_ = NULL;
            renderHandle = NULL;
        }

        void DxRender::s_ReleaseAllObjects_() {
            if (s_DirectDrawPrimarySurface_ != NULL) {
                s_DirectDrawPrimarySurface_->Release();
                s_DirectDrawPrimarySurface_ = NULL;
            }
            if (s_DirectDraw_ != NULL) {
                s_DirectDraw_->Release();
                s_DirectDraw_ = NULL;
            }
        }
    }
}
