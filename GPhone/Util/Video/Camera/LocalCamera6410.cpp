#include "LocalCamera6410.h"
#include <afx.h> //for TRACE1
#include <uuids.h> //for CLSID_CaptureGraphBuilder CLSID_FilterGraph CLSID_VideoCapture
#include <dshow.h> //for VIDEOINFOHEADER DeleteMediaType PIN_CATEGORY_CAPTURE MEDIATYPE_Video MEDIASUBTYPE_YV12 FORMAT_VideoInfo

#pragma comment(lib, "strmbase.lib") //for DeleteMediaType
#pragma comment(lib, "strmiids.lib")

namespace Util {
    namespace Video {
        LocalCamera6410::LocalCamera6410()
            : nullRenderModule_(0)
            , sampleGrabberModule_(0) {
                CoInitialize(0);
        }

        LocalCamera6410::~LocalCamera6410() {
            CoUninitialize();
        }

        bool const LocalCamera6410::SetParameters(CameraParameters const& params) {
            parameters_ = params;
            //parameters_.width = params.width;
            //parameters_.height = params.height;
            //parameters_.frameRate = params.frameRate;
            //parameters_.sampleCallBack = params.sampleCallBack;
            return true;
        }

        bool const LocalCamera6410::Open() {
            bool result = false;
            std::wstring name = getFirstCameraDriver_();
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
                result = registerServer_(sampleGrabberModule_, L"/FlashDrv/res_dat/SampleGrabberFilter.dll");
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
                result = registerServer_(nullRenderModule_, L"/FlashDrv/res_dat/dump.dll");
                if (!result) {
                    Close();
                }
                r = nullRender_.CoCreateInstance(CLSID_NullRender);
                if (FAILED(r)) {
                    Close();
                    result = false;
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
                //r = dumper_->SetFileName(L"/NandFlash/resource/test.lxk", 0);
                r = dumper_->SetFileName(L"/FlashDrv/test/test.lxk", 0);

                if (FAILED(r)) {
                    Close();
                    result = false;
                }
                r = captureGraphBuilder_->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, videoCapture_, sampleGrabber_, nullRender_);
                if (FAILED(r)) {
                    Close();
                    result = false;
                }
            }
            return result;
        }

        bool const LocalCamera6410::Close() {
            if (propertyBag_) {
                propertyBag_.Release();
                //propertyBag_ = 0;
            }
            if (videoCapture_) {
                videoCapture_.Release();
                //videoCapture_ = 0;
            }
            if (iSampleGrabber_) {
                iSampleGrabber_.Release();
                //iSampleGrabber_ = 0;
            }
            if (sampleGrabber_) {
                sampleGrabber_.Release();
                //sampleGrabber_ = 0;
            }
            if (dumper_) {
                dumper_.Release();
                //dumper_ = 0;
            }
            if (nullRender_) {
                nullRender_.Release();
                //nullRender_ = 0;
            }
            if (mediaControl_) {
                mediaControl_->Stop();
                mediaControl_.Release();
                //mediaControl_ = 0;
            }
            if (graphBuilder_) {
                graphBuilder_.Release();
                //graphBuilder_ = 0;
            }
            if (captureGraphBuilder_) {
                captureGraphBuilder_->SetFiltergraph(0);
                captureGraphBuilder_.Release();
                //captureGraphBuilder_ = 0;
            }
            if (nullRenderModule_ != 0) {
                FreeLibrary(nullRenderModule_);
                nullRenderModule_ = 0;
            }
            if (sampleGrabberModule_ != 0) {
                FreeLibrary(sampleGrabberModule_);
                sampleGrabberModule_ = 0;
            }
            return true;
        }

        bool const LocalCamera6410::Start() {
            bool result = false;
            IAMStreamConfig* config = 0;
            HRESULT hr = captureGraphBuilder_->FindInterface(&PIN_CATEGORY_CAPTURE, // Preview pin.
                0, // Any media type.
                videoCapture_, // Pointer to the capture filter.
                IID_IAMStreamConfig, (void**)&config);

            if (SUCCEEDED(hr)) {
                int iCount = 0, iSize = 0;
                hr = config->GetNumberOfCapabilities(&iCount, &iSize);
                // Check the size to make sure we pass in the correct structure.
                if (iSize == sizeof(VIDEO_STREAM_CONFIG_CAPS)) {
                    // Use the video capabilities structure.
                    for (int iFormat = 0; iFormat < iCount; ++iFormat) {
                        VIDEO_STREAM_CONFIG_CAPS scc;
                        AM_MEDIA_TYPE* mtConfig;
                        hr = config->GetStreamCaps(iFormat, &mtConfig, (BYTE *)&scc);
                        if (SUCCEEDED(hr)) {
                            /* Examine the format, and possibly use it. */
                            if ((mtConfig->majortype == MEDIATYPE_Video)
                                && (mtConfig->subtype == MEDIASUBTYPE_YV12)
                                && (mtConfig->formattype == FORMAT_VideoInfo)
                                && (mtConfig->cbFormat >= sizeof(VIDEOINFOHEADER))
                                && (mtConfig->pbFormat != 0)) {
                                    VIDEOINFOHEADER* videoInfoHeader = (VIDEOINFOHEADER*)mtConfig->pbFormat;
                                    //videoInfoHeader contains the detailed format information.
                                    LONG lWidth = videoInfoHeader->bmiHeader.biWidth;
                                    LONG lHeight = videoInfoHeader->bmiHeader.biHeight;

                                    //调整大小                      
                                    //videoInfoHeader->bmiHeader.biWidth = 640;
                                    //videoInfoHeader->bmiHeader.biHeight = -480;
                                    //videoInfoHeader->bmiHeader.biSizeImage = DIBSIZE(videoInfoHeader->bmiHeader);
                                    if (((lWidth == parameters_.width) || (lWidth == -parameters_.width))
                                        && ((lHeight == parameters_.height) || ((lHeight == -parameters_.height)))) {
                                            static int isSet = 0;
                                            if (!isSet) {
                                                hr = config->SetFormat(mtConfig); //重新设置视频格式
                                                if (SUCCEEDED(hr)) {
                                                    result = true;
                                                    isSet++;
                                                    DeleteMediaType(mtConfig);
                                                    break;
                                                }
                                            }
                                    }
                            }
                            // Delete the media type when you are done.
                            DeleteMediaType(mtConfig);
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

        bool const LocalCamera6410::Stop() {
            if (FAILED(mediaControl_->Stop())) {
                return false;
            }
            return true;
        }

        std::wstring const LocalCamera6410::getFirstCameraDriver_() const {
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

        bool const LocalCamera6410::registerServer_(HINSTANCE& module, std::wstring const& dllname) const {
            bool result = false;
            module = LoadLibrary(dllname.c_str());
            if (module) {
                RegisterServer registerServer = (RegisterServer)GetProcAddress(module, L"DllRegisterServer");
                if (registerServer) {
                    HRESULT r = registerServer();
                    if (SUCCEEDED(r)) {
                        result = true;
                    } else {
                        if (FAILED(r)) {
                            DWORD error = GetLastError();
                            TRACE1("DllRegisterServer() Failed with: %i\n", error);
                        }
                    }
                } else {
                    DWORD error = GetLastError();
                    TRACE1("GetProcAddress() Failed with: %i\n", error);
                }
                FreeLibrary(module);
                module = 0;
            } else {
                DWORD error = GetLastError();
                TRACE1("LoadLibrary() Failed with: %i\n", error);
            }
            return result;
        }
    }
}
