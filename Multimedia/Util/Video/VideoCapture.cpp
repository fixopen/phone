#include "VideoCapture.h"

#include <DShow.h> //for DirectShow
//Strmiids.lib
//Quartz.lib for AMGetErrorText

namespace Util {
    namespace Video {
        VideoCapture::~VideoCapture() {
        }

        void VideoCapture::Start() {
            //HRESULT hr = CoInitialize(NULL);
            //if (FAILED(hr)) {
            //    // Add error-handling code here. (Omitted for clarity.)
            //}
            ////DirectShow
            //IGraphBuilder *pGraph;
            //hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **) &pGraph);
            //IMediaControl *pControl;
            //IMediaEvent   *pEvent;
            //hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
            //hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
            //IVideoWindow *pVidWin = NULL;
            //pGraph->QueryInterface(IID_IVideoWindow, (void **)&pVidWin);
            //HWND hwnd;
            //pVidWin->put_Owner((OAHWND)hwnd);
            //pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
            //RECT rc;
            //GetClientRect(hwnd, &rc);
            //pVidWin->SetWindowPosition(0, 0, rc.right, rc.bottom);
            ////case WM_MOVE:
            ////    pVidWin->NotifyOwnerMessage((OAHWND)hWnd, msg, wParam, lParam);
            ////    break;
            //pControl->Stop();
            //pVidWin->put_Visible(OAFALSE);
            //pVidWin->put_Owner(NULL);
            //IVMRWindowlessControl *pWc = NULL;
            ////hr = InitWindowlessVMR(hwnd, pGraph, &pWc);
            //if (SUCCEEDED(hr)) {
            //    // Build the graph. For example:
            //    pGraph->RenderFile(L"Exmple.avi", 0);
            //    // Release the VMR interface when you are done.
            //    pWc->Release();
            //}
            //// Find the native video size.
            //long lWidth, lHeight;
            //hr = pWc->GetNativeVideoSize(&lWidth, &lHeight, NULL, NULL);
            //if (SUCCEEDED(hr))
            //{
            //    RECT rcSrc, rcDest;
            //    // Set the source rectangle.
            //    SetRect(&rcSrc, 0, 0, lWidth, lHeight);
            //    // Get the window client area.
            //    GetClientRect(hwnd, &rcDest);
            //    // Set the destination rectangle.
            //    SetRect(&rcDest, 0, 0, rcDest.right, rcDest.bottom);
            //    // Set the video position.
            //    hr = pWc->SetVideoPosition(&rcSrc, &rcDest);
            //}
#define WM_GRAPHNOTIFY  WM_APP + 1
            //IMediaEventEx *g_pEvent = NULL;
            //pGraph->QueryInterface(IID_IMediaEventEx, (void **)&g_pEvent);
            //g_pEvent->SetNotifyWindow((OAHWND)hwnd, WM_GRAPHNOTIFY, 0);
            ////case WM_GRAPHNOTIFY:
            ////    HandleGraphEvent();
            ////    break;
            //hr = pGraph->RenderFile(L"C:\\Example.avi", NULL);
            //if (SUCCEEDED(hr)) {
            //    // Run the graph.
            //    hr = pControl->Run();
            //    if (SUCCEEDED(hr)) {
            //        // Wait for completion.
            //        long evCode;
            //        pEvent->WaitForCompletion(INFINITE, &evCode);
            //        // Note: Do not use INFINITE in a real application, because it
            //        // can block indefinitely.
            //    }
            //}
            //// Disable event notification before releasing the graph.
            //g_pEvent->SetNotifyWindow(NULL, 0, 0);
            //g_pEvent->Release();
            //g_pEvent = NULL;
            //pControl->Release();
            //pEvent->Release();
            //pGraph->Release();
            //CoUninitialize();
        }

        //HRESULT InitWindowlessVMR(
        //    HWND hwndApp,                  // Window to hold the video.
        //    IGraphBuilder* pGraph,         // Pointer to the Filter Graph Manager.
        //    IVMRWindowlessControl** ppWc   // Receives a pointer to the VMR.
        //    )
        //{
        //    if (!pGraph || !ppWc)
        //    {
        //        return E_POINTER;
        //    }
        //    IBaseFilter* pVmr = NULL;
        //    IVMRWindowlessControl* pWc = NULL;
        //    // Create the VMR.
        //    HRESULT hr = CoCreateInstance(CLSID_VideoMixingRenderer, NULL,
        //        CLSCTX_INPROC, IID_IBaseFilter, (void**)&pVmr);
        //    if (FAILED(hr))
        //    {
        //        return hr;
        //    }

        //    // Add the VMR to the filter graph.
        //    hr = pGraph->AddFilter(pVmr, L"Video Mixing Renderer");
        //    if (FAILED(hr))
        //    {
        //        pVmr->Release();
        //        return hr;
        //    }
        //    // Set the rendering mode.
        //    IVMRFilterConfig* pConfig;
        //    hr = pVmr->QueryInterface(IID_IVMRFilterConfig, (void**)&pConfig);
        //    if (SUCCEEDED(hr))
        //    {
        //        hr = pConfig->SetRenderingMode(VMRMode_Windowless);
        //        pConfig->Release();
        //    }
        //    if (SUCCEEDED(hr))
        //    {
        //        // Set the window.
        //        hr = pVmr->QueryInterface(IID_IVMRWindowlessControl, (void**)&pWc);
        //        if( SUCCEEDED(hr))
        //        {
        //            hr = pWc->SetVideoClippingWindow(hwndApp);
        //            if (SUCCEEDED(hr))
        //            {
        //                *ppWc = pWc; // Return this as an AddRef'd pointer.
        //            }
        //            else
        //            {
        //                // An error occurred, so release the interface.
        //                pWc->Release();
        //            }
        //        }
        //    }
        //    pVmr->Release();
        //    return hr;
        //}

        //HRESULT CheckMediaType(AM_MEDIA_TYPE *pmt)
        //{
        //    if (pmt == NULL) return E_POINTER;

        //    // Check the major type. We're looking for video.
        //    if (pmt->majortype != MEDIATYPE_Video)
        //    {
        //        return VFW_E_INVALIDMEDIATYPE;
        //    }

        //    // Check the subtype. We're looking for 24-bit RGB.
        //    if (pmt->subtype != MEDIASUBTYPE_RGB24)
        //    {
        //        return VFW_E_INVALIDMEDIATYPE;
        //    }

        //    //// Check the format type and the size of the format block.
        //    //if ((pmt->formattype == FORMAT_VideoInfo) &&
        //    //    (pmt->cbFormat >= sizeof(VIDEOINFOHEADER) &&
        //    //    (pmt->pbFormat != NULL))
        //    //{
        //    //    // Now it's safe to coerce the format block pointer to the
        //    //    // correct structure, as defined by the formattype GUID.
        //    //    VIDEOINFOHEADER *pVIH = (VIDEOINFOHEADER*)pmt->pbFormat;

        //    //    // Examine pVIH (not shown). If it looks OK, return S_OK.
        //    //    return S_OK;
        //    //}

        //    return VFW_E_INVALIDMEDIATYPE;
        //}

        //HRESULT AddToRot(IUnknown *pUnkGraph, DWORD *pdwRegister)
        //{
        //    IMoniker * pMoniker = NULL;
        //    IRunningObjectTable *pROT = NULL;

        //    if (FAILED(GetRunningObjectTable(0, &pROT)))
        //    {
        //        return E_FAIL;
        //    }

        //    const size_t STRING_LENGTH = 256;

        //    WCHAR wsz[STRING_LENGTH];

        //    StringCchPrintfW(
        //        wsz, STRING_LENGTH,
        //        L"FilterGraph %08x pid %08x",
        //        (DWORD_PTR)pUnkGraph,
        //        GetCurrentProcessId()
        //        );

        //    HRESULT hr = CreateItemMoniker(L"!", wsz, &pMoniker);
        //    if (SUCCEEDED(hr))
        //    {
        //        hr = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnkGraph,
        //            pMoniker, pdwRegister);
        //        pMoniker->Release();
        //    }
        //    pROT->Release();

        //    return hr;
        //}

        //void RemoveFromRot(DWORD pdwRegister)
        //{
        //    IRunningObjectTable *pROT;
        //    if (SUCCEEDED(GetRunningObjectTable(0, &pROT))) {
        //        pROT->Revoke(pdwRegister);
        //        pROT->Release();
        //    }
        //}

        //HRESULT SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath)
        //{
        //    const WCHAR wszStreamName[] = L"ActiveMovieGraph";
        //    HRESULT hr;

        //    IStorage *pStorage = NULL;
        //    hr = StgCreateDocfile(
        //        wszPath,
        //        STGM_CREATE | STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
        //        0, &pStorage);
        //    if(FAILED(hr))
        //    {
        //        return hr;
        //    }

        //    IStream *pStream;
        //    hr = pStorage->CreateStream(
        //        wszStreamName,
        //        STGM_WRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE,
        //        0, 0, &pStream);
        //    if (FAILED(hr))
        //    {
        //        pStorage->Release();
        //        return hr;
        //    }

        //    IPersistStream *pPersist = NULL;
        //    pGraph->QueryInterface(IID_IPersistStream, (void**)&pPersist);
        //    hr = pPersist->Save(pStream, TRUE);
        //    pStream->Release();
        //    pPersist->Release();
        //    if (SUCCEEDED(hr))
        //    {
        //        hr = pStorage->Commit(STGC_DEFAULT);
        //    }
        //    pStorage->Release();
        //    return hr;
        //}

        //HRESULT LoadGraphFile(IGraphBuilder *pGraph, const WCHAR* wszName)
        //{
        //    IStorage *pStorage = 0;
        //    if (S_OK != StgIsStorageFile(wszName))
        //    {
        //        return E_FAIL;
        //    }
        //    HRESULT hr = StgOpenStorage(wszName, 0,
        //        STGM_TRANSACTED | STGM_READ | STGM_SHARE_DENY_WRITE,
        //        0, 0, &pStorage);
        //    if (FAILED(hr))
        //    {
        //        return hr;
        //    }
        //    IPersistStream *pPersistStream = 0;
        //    hr = pGraph->QueryInterface(IID_IPersistStream,
        //        reinterpret_cast<void**>(&pPersistStream));
        //    if (SUCCEEDED(hr))
        //    {
        //        IStream *pStream = 0;
        //        hr = pStorage->OpenStream(L"ActiveMovieGraph", 0,
        //            STGM_READ | STGM_SHARE_EXCLUSIVE, 0, &pStream);
        //        if(SUCCEEDED(hr))
        //        {
        //            hr = pPersistStream->Load(pStream);
        //            pStream->Release();
        //        }
        //        pPersistStream->Release();
        //    }
        //    pStorage->Release();
        //    return hr;
        //}

        //void OnPaint(HWND hwnd)
        //{
        //    PAINTSTRUCT ps;
        //    HDC         hdc;
        //    RECT        rcClient;
        //    GetClientRect(hwnd, &rcClient);
        //    hdc = BeginPaint(hwnd, &ps);
        //    IVMRWindowlessControl *g_pWc;
        //    RECT g_rcDest;
        //    if (g_pWc != NULL)
        //    {
        //        // Find the region where the application can paint by subtracting
        //        // the video destination rectangle from the client area.
        //        // (Assume that g_rcDest was calculated previously.)
        //        HRGN rgnClient = CreateRectRgnIndirect(&rcClient);
        //        HRGN rgnVideo  = CreateRectRgnIndirect(&g_rcDest);
        //        CombineRgn(rgnClient, rgnClient, rgnVideo, RGN_DIFF);

        //        // Paint on window.
        //        HBRUSH hbr = GetSysColorBrush(COLOR_BTNFACE);
        //        FillRgn(hdc, rgnClient, hbr);

        //        // Clean up.
        //        DeleteObject(hbr);
        //        DeleteObject(rgnClient);
        //        DeleteObject(rgnVideo);

        //        // Request the VMR to paint the video.
        //        HRESULT hr = g_pWc->RepaintVideo(hwnd, hdc);
        //    }
        //    else  // There is no video, so paint the whole client area.
        //    {
        //        RECT rc2;
        //        FillRect(hdc, &rc2, (HBRUSH)(COLOR_BTNFACE + 1));
        //    }
        //    EndPaint(hwnd, &ps);
        //}

        //void HandleGraphEvent()
        //{
        //    IMediaEventEx *g_pEvent;
        //    // Disregard if we don't have an IMediaEventEx pointer.
        //    if (g_pEvent == NULL)
        //    {
        //        return;
        //    }
        //    // Get all the events
        //    long evCode;
        //    LONG_PTR param1, param2;
        //    HRESULT hr;
        //    while (SUCCEEDED(g_pEvent->GetEvent(&evCode, &param1, &param2, 0)))
        //    {
        //        g_pEvent->FreeEventParams(evCode, param1, param2);
        //        switch (evCode)
        //        {
        //        case EC_COMPLETE:  // Fall through.
        //        case EC_USERABORT: // Fall through.
        //        case EC_ERRORABORT:
        //            //CleanUp();
        //            PostQuitMessage(0);
        //            return;
        //        }
        //    }
        //}

        void VideoCapture::Stop() {
        }
    }
}
