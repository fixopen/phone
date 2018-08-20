#include "DxRender.h"
#include <stdarg.h> //for vsprintf

#define PREFIX TEXT("\n{MFC DDRAW TEST}: ")
#define PREFIX_LEN 12

namespace Util {
    namespace Video {
        namespace {
            bool const PRINT_ERRMSG(LPCTSTR szError, ...) {
                static TCHAR szBuff[128] = PREFIX;
                va_list vl;
                va_start(vl, szError);
                //StringCchVPrintf(szBuff + PREFIX_LEN, (128 - PREFIX_LEN), szError, vl);
                //vsprintf(szBuff + PREFIX_LEN, szError, v1);
                OutputDebugString(szBuff);
                va_end(vl);
                return false;
            }

            DDPIXELFORMAT OverlayPixelFormats[] = {
                {sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('Y', 'V', '1', '2'), 0, 0, 0, 0, 0},
                {sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('Y', 'U', 'Y', 'V'), 0, 0, 0, 0, 0}, // YUYV
                {sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('U', 'Y', 'V', 'Y'), 0, 0, 0, 0, 0}, // UYVY
                {sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16,  0x7C00, 0x03e0, 0x001F, 0},         // 16-bit RGB 5:5:5
                {sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16,  0xF800, 0x07e0, 0x001F, 0},         // 16-bit RGB 5:6:5
                {sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0}
            };

            HRESULT WINAPI EnumSurfacesCallback(LPDIRECTDRAWSURFACE lpDDSurface, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext) {
                LPDIRECTDRAWSURFACE* ppDDSurface = (LPDIRECTDRAWSURFACE*)lpContext;
                HRESULT hr = (HRESULT)DDENUMRET_OK;
                *ppDDSurface = lpDDSurface;
                //RETAILMSG(1, (L"\nEnumSurface Callback function."));
                return hr;
            }
        }
        extern "C" void _initConvTab();
        extern "C" void _yuv420ToRgb565(unsigned char const* const p_lum, unsigned char const* const p_cb, unsigned char const* const p_cr, int w_src, int h_src, unsigned char* const dest, int w_dst, int h_dst, int topdown);

        DxManager::DxManager(HWND const windowHandle, DWORD const videoWidth, DWORD const videoHeight, int const surfaceType)
            : directDraw_(NULL)
            , directDrawPrimarySurface_(NULL)
            , inited_(false)
            , finaled_(false) {
            if (inited_) {
                return;
            }

            inited_ = true;

            //Check the type of the surface
            switch (surfaceType) {
                case SURFACE_TYPE_YV12:
                case SURFACE_TYPE_RGB565:
                    break;
                default:
                    PRINT_ERRMSG(TEXT("surfaceType is not supported."));
                    return;
            }

            //DX10, DX11
            //IDXGIFactory * pFactory = NULL;
            //CreateDXGIFactory(__uuidof(IDXGIFactory) ,(void**)&pFactory);
            //IDXGIAdapter* pAdapter;
            //std::vector<IDXGIAdapter*> vAdapters;
            //for (size_t i = 0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
            //    vAdapters.push_back(pAdapter);
            //    IDXGIOutput* pOutput = NULL;
            //    HRESULT hr = pAdapter->EnumOutputs(0, &pOutput);
            //    UINT numModes = 0;
            //    DXGI_MODE_DESC* displayModes = NULL;
            //    DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            //    // Get the number of elements
            //    hr = pOutput->GetDisplayModeList( format, 0, &numModes, NULL);
            //    displayModes = new DXGI_MODE_DESC[numModes];
            //    // Get the list
            //    hr = pOutput->GetDisplayModeList( format, 0, &numModes, displayModes);
            //    DXGI_SWAP_CHAIN_DESC sd;
            //    ZeroMemory( &sd, sizeof( sd ) );
            //    sd.BufferCount = 1;
            //    sd.BufferDesc.Width = 640;
            //    sd.BufferDesc.Height = 480;
            //    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            //    sd.BufferDesc.RefreshRate.Numerator = 60;
            //    sd.BufferDesc.RefreshRate.Denominator = 1;
            //    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            //    sd.OutputWindow = g_hWnd;
            //    sd.SampleDesc.Count = 1;
            //    sd.SampleDesc.Quality = 0;
            //    sd.Windowed = TRUE;
            //    D3D_FEATURE_LEVEL  FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
            //    UINT               numLevelsRequested = 1;
            //    D3D_FEATURE_LEVEL  FeatureLevelsSupported;
            //    if( FAILED (hr = D3D11CreateDeviceAndSwapChain( NULL,
            //        D3D_DRIVER_TYPE_HARDWARE,
            //        NULL,
            //        0,
            //        &FeatureLevelsRequested,
            //        numFeatureLevelsRequested,
            //        D3D11_SDK_VERSION,
            //        &sd,
            //        &g_pSwapChain,
            //        &g_pd3dDevice,
            //        &FeatureLevelsSupported,
            //        &g_pImmediateContext))) {
            //            return hr;
            //    }
            //    ID3D11Texture2D* pBackBuffer;
            //    // Get a pointer to the back buffer
            //    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
            //    // Create a render-target view
            //    g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
            //    // Bind the view
            //    g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, NULL );
            //    // Setup the viewport
            //    D3D11_VIEWPORT vp;
            //    vp.Width = 640;
            //    vp.Height = 480;
            //    vp.MinDepth = 0.0f;
            //    vp.MaxDepth = 1.0f;
            //    vp.TopLeftX = 0;
            //    vp.TopLeftY = 0;
            //    g_pImmediateContext->RSSetViewports( 1, &vp );
            //    ID3D11ComputeShader* g_pFinalPassCS = NULL;
            //    pd3dDevice->CreateComputeShader(
            //        pBlobFinalPassCS->GetBufferPointer(),
            //        pBlobFinalPassCS->GetBufferSize(),
            //        NULL, &g_pFinalPassCS );
            //}
            //pFactory->Release();
            //LPDIRECT3DDEVICE9 pDevice = NULL;
            //D3DPRESENT_PARAMETERS d3dpp;
            //ZeroMemory( &d3dpp, sizeof(d3dpp) );
            //d3dpp.Windowed   = TRUE;
            //d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
            //if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
            //                    D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            //                    &d3dpp, &d3dDevice ) ) )
            //    return E_FAIL;
            //D3DPRESENT_PARAMETERS Params;
            //// Initialize values for D3DPRESENT_PARAMETERS members.
            //Params.BackBufferFormat = D3DFMT_X1R5G5B5;
            //if(FAILED(m_pD3D->CheckDeviceType(Device.m_uAdapter,
            //    Device.m_DevType,
            //    Params.BackBufferFormat, Params.BackBufferFormat,
            //    FALSE)))
            //    return E_FAIL;
            //3DPRESENT_PARAMETERS Params;
            //// Initialize values for D3DPRESENT_PARAMETERS members.
            //// Use the current display mode.
            //D3DDISPLAYMODE mode;
            //if(FAILED(m_pD3D->GetAdapterDisplayMode(Device.m_uAdapter , &mode)))
            //    return E_FAIL;
            //Params.BackBufferFormat = mode.Format;
            //if(FAILED(m_pD3D->CheckDeviceType(Device.m_uAdapter, Device.m_DevType,
            //    Params.BackBufferFormat, Params.BackBufferFormat, FALSE)))
            //    return E_FAIL;
            //// Create the main DirectDraw object
            ////::D3D9CreateDevice();
            //HRESULT r = DirectDrawCreate(NULL, &directDraw_, NULL);
            //if (r != DD_OK) {
            //    PRINT_ERRMSG(TEXT("DirectDrawCreate FAILED"));
            //    return;
            //}
            //// Get exclusive mode
            //r = directDraw_->SetCooperativeLevel(windowHandle, DDSCL_NORMAL);
            //if (r != DD_OK) {
            //    PRINT_ERRMSG(TEXT("SetCooperativeLevel FAILED"));
            //    return;
            //}
            //// Get a primary surface interface pointer (only needed for init.)
            //DDSURFACEDESC surfaceDesc;
            //memset(&surfaceDesc, 0, sizeof(surfaceDesc));
            //surfaceDesc.dwSize = sizeof(surfaceDesc);
            //surfaceDesc.dwFlags = DDSD_CAPS;
            //surfaceDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
            //r = directDraw_->CreateSurface(&surfaceDesc, &directDrawPrimarySurface_, NULL);
            //if (r != DD_OK) {
            //    PRINT_ERRMSG(TEXT("CreateSurface s_DirectDrawPrimarySurface_ FAILED"));
            //    return;
            //}
            //Software YUV->RGB conversion table initialization
            //_initConvTab();
            return;
        }

        DxManager::~DxManager() {
            if (finaled_) {
                PRINT_ERRMSG(TEXT("Has Deinited!\n"));
                return;
            }

            finaled_ = true;

            if (directDrawPrimarySurface_ != NULL) {
                directDrawPrimarySurface_->Release();
                directDrawPrimarySurface_ = NULL;
            }

            if (directDraw_ != NULL) {
                directDraw_->Release();
                directDraw_ = NULL;
            }
        }

        Surface* const DxManager::CreateSurface(int const surfaceType, int const x, int const y, int const srcWidth, int const srcHeight, int const dstWidth, int const dstHeight) {
            Surface* result = new Surface(this, surfaceType, x, y, srcWidth, srcHeight, dstWidth, dstHeight);
            return result;
        }

        void DestroySurface(Surface* const surface) {
            delete surface;
        }

        Surface::Surface(DxManager* const owner, int const surfaceType, int const x, int const y, int const srcWidth, int const srcHeight, int const dstWidth, int const dstHeight)
            : owner_(owner) {
            switch (surfaceType) {
                case SURFACE_TYPE_YV12:
                case SURFACE_TYPE_RGB565:
                    break;
                default:
                    PRINT_ERRMSG(TEXT("surfaceType is not supported."));
                    return;
            }

            RenderOverlay* renderOverlay = (RenderOverlay*)malloc(sizeof(RenderOverlay));

            if (renderOverlay == NULL) {
                PRINT_ERRMSG(TEXT("malloc failed."));
                return;
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
            HRESULT r = owner_->directDraw_->CreateSurface(&surfaceDesc, &overlaySurface, NULL);

            if (r != DD_OK) {
                PRINT_ERRMSG(TEXT("Unable to create surface!"));
                free(renderOverlay);
                return;
            }

            //RETAILMSG(1, (L"\n\t SURFACE TYPE %d\n", surfaceType));
            handle_->directDrawSurface = overlaySurface;
            handle_->x = x;
            handle_->y = y;
            handle_->srcWidth = srcWidth;
            handle_->srcHeight = srcHeight;
            handle_->dstWidth = dstWidth;
            handle_->dstHeight = dstHeight;
            handle_->surfaceType = surfaceType;
            //return (void*)handle_;
            return;
        }

        Surface::~Surface() {
            if (handle_ == NULL) {
                PRINT_ERRMSG(TEXT("\nmfc_render_handle is NULL."));
                return;
            }

            LPDIRECTDRAWSURFACE overlaySurface = handle_->directDrawSurface;

            if (overlaySurface != NULL) {
                overlaySurface->Release();
                overlaySurface = NULL;
            }

            free(handle_);
            offlineScreen_ = NULL;
            handle_ = NULL;
        }

        void Surface::Do(unsigned char const* const image, int const width, int const height, int const imageType) {
            switch (imageType) {
                case IMAGE_TYPE_YUV420:
                case IMAGE_TYPE_RGB565:
                case IMAGE_TYPE_YV12:
                    break;
                default:
                    PRINT_ERRMSG(TEXT("imageType is not supported."));
                    return;
            }

            if (handle_ == NULL) {
                PRINT_ERRMSG(TEXT("\nmfc_render_handle is NULL."));
                return;
            }

            if (image == NULL) {
                PRINT_ERRMSG(TEXT("Input image is NULL."));
                return;
            }

            LPDIRECTDRAWSURFACE offlineScreenSurface = handle_->directDrawSurface; // Overlay Surface.
            DDSURFACEDESC surfaceDesc;
            memset(&surfaceDesc, 0, sizeof(surfaceDesc));
            surfaceDesc.dwSize = sizeof(surfaceDesc);
            // Lock down the surface so we can modify its contents.
            HRESULT r = offlineScreenSurface->Lock(NULL, &surfaceDesc, DDLOCK_WAIT/*NOTBUSY*/, NULL);

            if (r != DD_OK) {
                PRINT_ERRMSG(TEXT("Unable to lock OffScreen surface!"));
                return;
            }

            unsigned char* surface = (LPBYTE)surfaceDesc.lpSurface;
            int ySize = width * height;
            int uSize = ySize >> 2;

            switch (handle_->surfaceType) {
                case SURFACE_TYPE_YV12:

                    if (imageType == IMAGE_TYPE_YUV420) {
                        memcpy(surface, image, ySize);
                        memcpy(surface + ySize, image + ySize + uSize, uSize);
                        memcpy(surface + ySize + uSize, image + ySize, uSize);
                    } else if (imageType == IMAGE_TYPE_YV12) {
                        memcpy(surface, image, ySize + (uSize << 1));
                    } else if (imageType == IMAGE_TYPE_RGB565) {
                        PRINT_ERRMSG(TEXT("This case is not supported. : (Surface type is YV12 and Image type is RGB565)"));
                    }

                    break;
                case SURFACE_TYPE_RGB565:

                    if (imageType == IMAGE_TYPE_YUV420) {
                        //_yuv420ToRgb565(image, image + ySize, image + ySize + uSize, width, height, surface, width, height, 0);
                    } else if (imageType == IMAGE_TYPE_YV12) {
                        //_yuv420ToRgb565(image, image + ySize + uSize, image + ySize, width, height, surface, width, height, 0);
                    } else if (imageType == IMAGE_TYPE_RGB565) {
                        memcpy(surface, image, ySize << 1);
                    }

                    break;
                default:
                    break;
            }

            offlineScreenSurface->Unlock(NULL);

            //Blt to the overlay back_surface
            while (TRUE) {
                r = owner_->directDrawPrimarySurface_->Blt(&bounds, offlineScreenSurface, NULL, DDBLT_DONOTWAIT/*WAITNOTBUSY*/, NULL);

                if (SUCCEEDED(r)) {
                    break;
                }

                if (r != DDERR_WASSTILLDRAWING) {
                    return;
                }
            }
        }
    }
}
