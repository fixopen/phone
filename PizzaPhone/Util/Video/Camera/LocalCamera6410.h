#ifndef __UTIL_VIDEO_LOCALCAMERA6410_H__
#define __UTIL_VIDEO_LOCALCAMERA6410_H__

#include <atlcomcli.h> //for CComPtr CComVariant
#include <ocidl.h> //for IPersistPropertyBag
#include <strmif.h> //for IBaseFilter IGraphBuilder ICaptureGraphBuilder2
#include <control.h> //for IMediaControl

#include <string> //for std::wstring

#include "../../PropertyBag.h" //for PropertyBag

#include "../CameraProcessFilter.h"
#include "../CameraSinkFilter.h"

typedef void (CALLBACK *CameraDataProc)(unsigned char const* const data, unsigned int const length);
typedef HRESULT (CALLBACK *RegisterServer)();

namespace Util {
    namespace Video {
        struct CameraParameters {
            int width;
            int height;
            unsigned int frameRate;
            CameraDataProc sampleCallBack;
        };

        class LocalCamera6410 {
        public:
            LocalCamera6410();
            ~LocalCamera6410();

            bool const SetParameters(CameraParameters const& params);
            bool const Open();
            bool const Close();
            bool const Start();
            bool const Stop();
        private:
            std::wstring const getFirstCameraDriver_() const;
            bool const registerServer_(HINSTANCE& module, std::wstring const& dllname) const;
            template<typename T>
            bool const createInstance_(CComPtr<T>& component, GUID const& id) {
                bool result = true;
                HRESULT r = component.CoCreateInstance(id);
                if (FAILED(r)) {
                    DWORD error = GetLastError();
                    TRACE1("component.CoCreateInstance(id) Failed with: %i\n", error);
                    result = false;
                }
                return result;
            }
        private:
            PropertyBag propBag_;
            CComPtr<IPersistPropertyBag> propertyBag_;
            CComPtr<IBaseFilter> videoCapture_;

            CameraParameters parameters_;
            CComPtr<ISampleGrabber> iSampleGrabber_;
            CComPtr<IBaseFilter> sampleGrabber_;

            CComPtr<INullRender> dumper_;
            CComPtr<IBaseFilter> nullRender_;

            CComPtr<IMediaControl> mediaControl_;
            CComPtr<IGraphBuilder> graphBuilder_;
            CComPtr<ICaptureGraphBuilder2> captureGraphBuilder_;

            HINSTANCE nullRenderModule_;
            HINSTANCE sampleGrabberModule_;
        };
    }
}

#endif //__UTIL_VIDEO_LOCALCAMERA6410_H__
