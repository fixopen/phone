#include <afx.h> //for TRACE1
#include "Camera6410.h"

#pragma comment(lib, "strmbase.lib")
#pragma comment(lib, "strmiids.lib")

namespace Util {
    namespace Video {
        //extern "C" {
        //    // The CLSID used by the SampleGrabber filter
        //    //{AD5DB5B4-D1AB-4F37-A60D-215154B4ECC1}
        //    GUID const CLSID_SampleGrabber = {0xAD5DB5B4, 0xD1AB, 0x4F37, {0xA6, 0x0D, 0x21, 0x51, 0x54, 0xB4, 0xEC, 0xC1}};

        //    // ISampleGrabber interface definition
        //    //{04951BFF-696A-4ADE-828D-42A5F1EDB631}
        //    GUID const IID_ISampleGrabber = {0x4951BFF, 0x696A, 0x4ADE, {0x82, 0x8D, 0x42, 0xA5, 0xF1, 0xED, 0xB6, 0x31}};

        //    // The CLSID used by the dump[NullRender] filter
        //    // {41F3D573-C28F-4a50-A3E3-160226E56EC9}
        //    GUID const CLSID_NullRender = {0x41F3D573, 0xC28F, 0x4A50, {0xA3, 0xE3, 0x16, 0x02, 0x26, 0xE5, 0x6E, 0xC9}};

        //    // INullRender interface definition
        //    // {C85F4AAF-69D8-4d86-B6B8-2E9FADE7E7A3}
        //    GUID const IID_INullRender = {0xC85F4AAF, 0x69D8, 0x4D86, {0xB6, 0xB8, 0x2E, 0x9F, 0xAD, 0xE7, 0xE7, 0xA3}};
        //}

        //Camera6410::Camera6410()
        //: nullRenderModule_(0)
        //, sampleGrabberModule_(0) {
        //    CoInitialize(0);
        //}

        //Camera6410::~Camera6410() {
        //    CoUninitialize();
        //}

        bool const Camera6410::Open() {
            CoInitialize(0);
            bool result = false;
            std::wstring name = getCameraName_();
            if (name != L"") {
                result = true;
                HRESULT r = captureGraphBuilder_.CoCreateInstance(CLSID_CaptureGraphBuilder);
                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                r = graphBuilder_.CoCreateInstance(CLSID_FilterGraph);
                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                r = captureGraphBuilder_->SetFiltergraph(graphBuilder_);
                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                r = graphBuilder_.QueryInterface(&mediaControl_);
                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                r = videoCapture_.CoCreateInstance(CLSID_VideoCapture);
                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                r = videoCapture_->QueryInterface(&propertyBag_);
                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                CComVariant cameraName = name.c_str();
                if ((cameraName.vt == VT_BSTR) == 0) {
                    Close();
                    result = false;
                }
                r = propBag_.Write(L"VCapName", &cameraName);
                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                r = propertyBag_->Load(&propBag_, 0);
                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                r = graphBuilder_->AddFilter(videoCapture_, L"Video capture source");
                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                result = registerServer_(sampleGrabberModule_, L"/FlashDrv/SampleGrabberFilter.dll");
                if (!result) {
                    Close();
                }
                r = sampleGrabber_.CoCreateInstance(CLSID_SampleGrabber);
                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                r = graphBuilder_->AddFilter(sampleGrabber_, L"Sample Filter");
                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                r = sampleGrabber_->QueryInterface(IID_ISampleGrabber, (void**)&iSampleGrabber_);
                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                r = iSampleGrabber_->RegisterCallback((MANAGEDCALLBACKPROC)parameters_.sampleCallBack);
                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                r = nullRender_.CoCreateInstance(CLSID_NullRender);
                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                result = registerServer_(nullRenderModule_, L"/FlashDrv/dump.dll");
                if (!result) {
                    Close();
                }
                r = graphBuilder_->AddFilter(nullRender_, L"Null Render Filter");
                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                r = nullRender_->QueryInterface(IID_INullRender, (void**)&dumper_);
                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                r = dumper_->SetFileName(L"/FlashDrv/test.lxk", 0);

                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                r = captureGraphBuilder_->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, videoCapture_, sampleGrabber_, nullRender_);
                //r = captureGraphBuilder_->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, videoCapture_, sampleGrabber_, nullRender_);
                //r = captureGraphBuilder_->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, videoCapture_, sampleGrabber_, 0);
                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                //r = captureGraphBuilder_->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, videoCapture_, 0, 0);
                //if (FAILED(r)) {
                //    Close();
                //    result = false;
                //}
            }
            return result;
        }

        bool const Camera6410::Start() {
            bool result = false;
            IAMStreamConfig* config = 0;
            HRESULT hr = captureGraphBuilder_->FindInterface(&PIN_CATEGORY_CAPTURE, // Preview pin.
                0, // Any media type.
                videoCapture_, // Pointer to the capture filter.
                IID_IAMStreamConfig, (void**)&config);

            if (SUCCEEDED(hr)) {
                int count = 0, size = 0;
                hr = config->GetNumberOfCapabilities(&count, &size);
                // Check the size to make sure we pass in the correct structure.
                if (size == sizeof(VIDEO_STREAM_CONFIG_CAPS)) {
                    // Use the video capabilities structure.
                    for (int formatIndex = 0; formatIndex < count; ++formatIndex) {
                        VIDEO_STREAM_CONFIG_CAPS streamConfigCaps;
                        AM_MEDIA_TYPE* mediaTypeConfig;
                        hr = config->GetStreamCaps(formatIndex, &mediaTypeConfig, (BYTE *)&streamConfigCaps);
                        if (SUCCEEDED(hr)) {
                            // Examine the format, and possibly use it.
                            if ((mediaTypeConfig->majortype == MEDIATYPE_Video) && (mediaTypeConfig->subtype == MEDIASUBTYPE_YV12) && (mediaTypeConfig->formattype == FORMAT_VideoInfo) && (mediaTypeConfig->cbFormat >= sizeof(VIDEOINFOHEADER)) && (mediaTypeConfig->pbFormat != 0)) {
                                VIDEOINFOHEADER* videoInfoHeader = (VIDEOINFOHEADER*)mediaTypeConfig->pbFormat;
                                //videoInfoHeader contains the detailed format information.
                                LONG width = videoInfoHeader->bmiHeader.biWidth;
                                LONG height = videoInfoHeader->bmiHeader.biHeight;

                                //调整大小                      
                                //videoInfoHeader->bmiHeader.biWidth = 640;
                                //videoInfoHeader->bmiHeader.biHeight = -480;
                                //videoInfoHeader->bmiHeader.biSizeImage = DIBSIZE(videoInfoHeader->bmiHeader);
                                if (((width == parameters_.width) || (width == -parameters_.width)) && ((height == parameters_.height) || ((height == -parameters_.height)))) {
                                    static bool isSet = false;
                                    if (!isSet) {
                                        hr = config->SetFormat(mediaTypeConfig); //重新设置视频格式
                                        if (SUCCEEDED(hr)) {
                                            result = true;
                                            isSet = true;
                                            DeleteMediaType(mediaTypeConfig);
                                            break;
                                        }
                                    }
                                }
                            }
                            // Delete the media type when you are done.
                            DeleteMediaType(mediaTypeConfig);
                        }
                    }
                }
                config->Release();
            }
            if (result) {
                if (FAILED(mediaControl_->Run())) {
                    result = false;
                }
            }
            return result;
        }

        bool const Camera6410::Stop() {
            bool result = true;
            if (FAILED(mediaControl_->Stop())) {
                result = false;
            }
            CoUninitialize();
            return result;
        }

        bool const Camera6410::Close() {
            if (dumper_) {
                dumper_.Release();
            }
            if (nullRender_) {
                nullRender_.Release();
            }
            if (nullRenderModule_ != 0) {
                FreeLibrary(nullRenderModule_);
                nullRenderModule_ = 0;
            }
            if (iSampleGrabber_) {
                iSampleGrabber_.Release();
            }
            if (sampleGrabber_) {
                sampleGrabber_.Release();
            }
            if (sampleGrabberModule_ != 0) {
                FreeLibrary(sampleGrabberModule_);
                sampleGrabberModule_ = 0;
            }
            if (propertyBag_) {
                propertyBag_.Release();
            }
            if (videoCapture_) {
                videoCapture_.Release();
            }
            if (mediaControl_) {
                mediaControl_->Stop();
                mediaControl_.Release();
            }
            if (graphBuilder_) {
                graphBuilder_.Release();
            }
            if (captureGraphBuilder_) {
                captureGraphBuilder_->SetFiltergraph(0);
                captureGraphBuilder_.Release();
            }
            return true;
        }

        std::wstring const Camera6410::getCameraName_() const {
            std::wstring result;
            GUID guidCamera = {
                0xCB998A05, 0x122C, 0x4166, 0x84, 0x6A, 0x93, 0x3E, 0x4D, 0x7E, 0x3C, 0x86
            };
            DEVMGR_DEVICE_INFORMATION deviceInfo;
            deviceInfo.dwSize = sizeof(deviceInfo);
            HANDLE handle = FindFirstDevice(DeviceSearchByGuid, &guidCamera, &deviceInfo);
            if (handle) {
                if (deviceInfo.hDevice) {
                    wchar_t cameraName[MAX_PATH + 1];
                    wcscpy_s(cameraName, MAX_PATH, deviceInfo.szLegacyName);
                    result = cameraName;
                }
                FindClose(handle);
            }
            return result;
        }

        bool const Camera6410::registerServer_(HINSTANCE& module, std::wstring const & dllname) const {
            bool result = false;
            module = ::LoadLibrary(dllname.c_str());
            if (module) {
                typedef HRESULT (CALLBACK *RegisterServer)();
                RegisterServer registerServer = (RegisterServer)::GetProcAddress(module, L"DllRegisterServer");
                if (registerServer) {
                    HRESULT r = registerServer();
                    if (SUCCEEDED(r)) {
                        result = true;
                    } else {
                        if (FAILED(r)) {
                            DWORD error = ::GetLastError();
                            TRACE1("DllRegisterServer() Failed with: %i\n", error);
                        }
                    }
                } else {
                    DWORD error = ::GetLastError();
                    TRACE1("GetProcAddress() Failed with: %i\n", error);
                }
                ::FreeLibrary(module);
                module = 0;
            } else {
                DWORD error = ::GetLastError();
                TRACE1("LoadLibrary() Failed with: %i\n", error);
            }
            return result;
        }
    }
}
