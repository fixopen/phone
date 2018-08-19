#ifndef __UTIL_VIDEO_CAMERA6410_H__
#define __UTIL_VIDEO_CAMERA6410_H__

#include <atlcomcli.h> //for CComPtr CComVariant
#include <streams.h> //for IMediaControl ...
#include <string> //for std::wstring
#include "Camera.h" //for Camera
#include "../PropertyBag.h" //for PropertyBag
#include "Camera/sampleuids.h"
#include "Camera/dumpuids.h"

namespace Util {
    namespace Video {
        //typedef void (CALLBACK *MANAGEDCALLBACKPROC)(BYTE* pdata, long len);

        //extern "C" {
        //    extern GUID const CLSID_SampleGrabber;
        //    extern GUID const IID_ISampleGrabber;
        //    interface ISampleGrabber : public IUnknown {
        //        STDMETHOD(RegisterCallback)(MANAGEDCALLBACKPROC callback) PURE;
        //    };


        //    extern GUID const CLSID_NullRender;
        //    extern GUID const IID_INullRender;
        //    interface INullRender : public IFileSinkFilter {
        //        //STDMETHOD(RegisterCallback)(MANAGEDCALLBACKPROC callback) PURE;
        //        //Write data streams to a file
        //        STDMETHOD (Write)(PBYTE pbData, LONG lData) PURE;
        //    };
        //}
        //CaptureGraphBuilder[captureGraphBuilder_]----(Filtergraph)----FilterGraph[graphBuilder_]--+
        //                                                                   |              |       |
        //                                                                   |QueryInterface|       |
        //                                                                   |              |       |
        //                                                             IMediaControl[mediaControl_] |
        //----VideoCapture[videoCapture_]<----------------------------------------------------------+--AddFilter
        //        |              |                                                                  |
        //        |QueryInterface|                     ("VCapName", cameraName)                     |
        //        |              |                            |Write|                               |
        //IPersistPropertyBag[propertyBag_]---(Load)----PropertyBag[propBag_]                       |
        //registerServer_(/FlashDrv/SampleGrabberFilter.dll)                                        |
        //----SampleGrabber[sampleGrabber_]<--------------------------------------------------------+--AddFilter
        //         |              |                                                                 |
        //         |QueryInterface|                                                                 |
        //         |              |                                                                 |
        //ISampleGrabber[iSampleGrabber_]----(RegisterCallback)----CameraParameters.sampleCallBack  |
        //registerServer_(/FlashDrv/dump.dll)                                                       |
        //----NullRender[nullRender_]<--------------------------------------------------------------+--AddFilter
        //      |              |
        //      |QueryInterface|
        //      |              |
        //     INullRender[dumper_]----(SetFileName)----L"/FlashDrv/test.lxk"
        class Camera6410 : public Camera {
        public:
            virtual bool const Open();
            virtual bool const Start();
            virtual bool const Stop();
            virtual bool const Close();
        private:
            std::wstring const getCameraName_() const;
            bool const registerServer_(HINSTANCE& module, std::wstring const & dllname) const;
        private:
            //sink
            CComPtr<INullRender> dumper_;
            CComPtr<IBaseFilter> nullRender_;
            HINSTANCE nullRenderModule_;

            //process
            CameraParameters parameters_;
            CComPtr<ISampleGrabber> iSampleGrabber_;
            CComPtr<IBaseFilter> sampleGrabber_;
            HINSTANCE sampleGrabberModule_;

            //source
            PropertyBag propBag_;
            CComPtr<IPersistPropertyBag> propertyBag_;
            CComPtr<IBaseFilter> videoCapture_;

            CComPtr<IMediaControl> mediaControl_;
            CComPtr<IGraphBuilder> graphBuilder_;
            CComPtr<ICaptureGraphBuilder2> captureGraphBuilder_;
        };
    }
}

#endif //__UTIL_VIDEO_CAMERA6410_H__
